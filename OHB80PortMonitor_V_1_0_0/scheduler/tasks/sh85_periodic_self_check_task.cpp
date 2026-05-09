#include "sh85_periodic_self_check_task.h"

#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include "modbustcpmastermanager/modbustcpmaster/sh85selfchecker.h"
#include "app/applogger.h"
#include "app/shareddata.h"
#include "scheduler/tasks/operation_dispatch_task.h"
#include "loggermanager.h"

#include <QDateTime>
#include <QDebug>
#include <QTimer>

// ============================================================
// 构造 / 析构
// ============================================================

SH85PeriodicSelfCheckTask::SH85PeriodicSelfCheckTask(QObject *parent)
    : SchedulerTask(parent)
{
    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] 创建任务";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85PeriodicSelfCheckTask] 创建任务").toStdString());
}

SH85PeriodicSelfCheckTask::~SH85PeriodicSelfCheckTask()
{
    stopAllTimers();
    disconnectChecker();
    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] 任务销毁";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85PeriodicSelfCheckTask] 任务销毁").toStdString());
}

// ============================================================
// 配置
// ============================================================

void SH85PeriodicSelfCheckTask::setIntervalSeconds(int seconds)
{
    if (seconds <= 0) seconds = 1;
    m_intervalSec = seconds;
    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] setIntervalSeconds:" << seconds;

    // 若当前正处于 Idle 倒计时，重置剩余秒数（立即生效）
    if (m_status == Status::Idle && m_idleTimer && m_idleTimer->isActive()) {
        m_remainingSec = m_intervalSec;
        emit countdownTick(m_remainingSec);
    }
}

// ============================================================
// 生命周期
// ============================================================

void SH85PeriodicSelfCheckTask::start()
{
    setState(Running);
    m_stopRequested   = false;
    m_finishedEmitted = false;

    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] start() 间隔=" << m_intervalSec << "s";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85PeriodicSelfCheckTask] 任务启动 间隔=%1s").arg(m_intervalSec).toStdString());

    // 运行日志
    if (auto* op = SharedData::getOperationDispatchTask()) {
        op->log(OperationDispatchTask::MsgType::Message,
                QString("Periodic SH85 self-check enabled, interval=%1s").arg(m_intervalSec), 0);
    }

    // 立即执行一轮自检
    beginRound();
}

void SH85PeriodicSelfCheckTask::stop()
{
    if (m_finishedEmitted) return;
    m_finishedEmitted = true;
    m_stopRequested   = true;

    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] stop() 调用";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85PeriodicSelfCheckTask] 任务停止").toStdString());

    // 停止定时器
    stopAllTimers();

    // 终止当前正在运行的 checker（如果有）
    if (m_currentChecker) {
        disconnect(m_checkerFinishedConn);
        m_currentChecker->stop();
        m_currentChecker = nullptr;
        m_currentMaster  = nullptr;
        m_currentQrcode.clear();
    }

    // 运行日志
    if (auto* op = SharedData::getOperationDispatchTask()) {
        op->log(OperationDispatchTask::MsgType::Message,
                QString("Periodic SH85 self-check disabled"), 0);
    }

    setState(Cancelled);
    emit finished(true, QStringLiteral("SH85PeriodicSelfCheckTask stopped"));
}

// ============================================================
// 间隔倒计时
// ============================================================

void SH85PeriodicSelfCheckTask::startIdleCountdown()
{
    m_status       = Status::Idle;
    m_remainingSec = m_intervalSec;
    emit statusChanged(m_status);
    emit countdownTick(m_remainingSec);

    if (!m_idleTimer) {
        m_idleTimer = new QTimer(this);
        m_idleTimer->setInterval(1000);
        connect(m_idleTimer, &QTimer::timeout, this, &SH85PeriodicSelfCheckTask::onIdleTick);
    }
    m_idleTimer->start();
}

void SH85PeriodicSelfCheckTask::onIdleTick()
{
    if (m_stopRequested) return;
    if (m_remainingSec > 0) {
        --m_remainingSec;
        emit countdownTick(m_remainingSec);
    }
    if (m_remainingSec <= 0) {
        m_idleTimer->stop();
        beginRound();
    }
}

// ============================================================
// 一轮自检控制
// ============================================================

void SH85PeriodicSelfCheckTask::beginRound()
{
    if (m_stopRequested) return;

    m_status         = Status::Checking;
    m_roundQrcodes   = SharedData::getAllQrcodes();
    m_roundIndex     = 0;
    m_successCount   = 0;
    m_failureCount   = 0;
    m_roundStartTime = currentTimestamp();

    // 过滤掉 enable=false 的设备
    QStringList filteredQrcodes;
    for (const QString& qrcode : m_roundQrcodes) {
        FoupOfOHBInfo* foupInfo = SharedData::getFoupByQRCode(qrcode);
        if (foupInfo && foupInfo->enable()) {
            filteredQrcodes.append(qrcode);
        } else {
            qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] 跳过未启用设备 qrcode=" << qrcode;
        }
    }
    m_roundQrcodes = filteredQrcodes;

    emit statusChanged(m_status);
    emit progressUpdate(0, m_roundQrcodes.size());

    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] 一轮开始, 过滤后设备数=" << m_roundQrcodes.size();
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85PeriodicSelfCheckTask] 一轮开始 过滤后设备数=%1")
            .arg(m_roundQrcodes.size()).toStdString());

    if (m_roundQrcodes.isEmpty()) {
        // 无设备，直接结束本轮
        endRound();
        return;
    }

    startNextDevice();
}

void SH85PeriodicSelfCheckTask::startNextDevice()
{
    if (m_stopRequested) return;

    // 已完成所有设备
    if (m_roundIndex >= m_roundQrcodes.size()) {
        endRound();
        return;
    }

    m_currentQrcode = m_roundQrcodes.at(m_roundIndex);
    emit progressUpdate(m_roundIndex, m_roundQrcodes.size());

    auto& mgr = ModbusTcpMasterManager::instance();
    m_currentMaster = mgr.getMaster(m_currentQrcode);

    // 设备不可用或未连接 → 视为自检失败，更新进度条
    if (!m_currentMaster || !m_currentMaster->isConnected()) {
        qWarning() << "[Scheduler][SH85PeriodicSelfCheckTask] master 不可用或未连接 qrcode=" << m_currentQrcode;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SH85PeriodicSelfCheckTask] 设备未连接 qrcode=%1").arg(m_currentQrcode).toStdString());
        if (auto* op = SharedData::getOperationDispatchTask()) {
            op->log(OperationDispatchTask::MsgType::Warn,
                    QString("Periodic SH85 self-check: device not connected, qrcode=%1").arg(m_currentQrcode), 0);
        }
        finishCurrentDevice(false);
        return;
    }

    m_currentChecker = m_currentMaster->selfChecker();
    if (!m_currentChecker) {
        qWarning() << "[Scheduler][SH85PeriodicSelfCheckTask] checker 为 null qrcode=" << m_currentQrcode;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SH85PeriodicSelfCheckTask] checker 为 null qrcode=%1").arg(m_currentQrcode).toStdString());
        if (auto* op = SharedData::getOperationDispatchTask()) {
            op->log(OperationDispatchTask::MsgType::Warn,
                    QString("Periodic SH85 self-check: checker is null, qrcode=%1").arg(m_currentQrcode), 0);
        }
        finishCurrentDevice(false);
        return;
    }

    // 连接 finished 信号（一次性）
    m_checkerFinishedConn = connect(
        m_currentChecker, &SH85SelfChecker::finished,
        this, &SH85PeriodicSelfCheckTask::onCheckerFinished,
        Qt::QueuedConnection);

    if (!m_currentChecker->start()) {
        qWarning() << "[Scheduler][SH85PeriodicSelfCheckTask] checker->start() 返回 false qrcode=" << m_currentQrcode;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SH85PeriodicSelfCheckTask] checker->start() 返回 false qrcode=%1").arg(m_currentQrcode).toStdString());
        if (auto* op = SharedData::getOperationDispatchTask()) {
            op->log(OperationDispatchTask::MsgType::Warn,
                    QString("Periodic SH85 self-check: checker start failed, qrcode=%1").arg(m_currentQrcode), 0);
        }
        disconnectChecker();
        finishCurrentDevice(false);
        return;
    }

    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] 启动设备自检 qrcode=" << m_currentQrcode
             << "(" << (m_roundIndex + 1) << "/" << m_roundQrcodes.size() << ")";
}

void SH85PeriodicSelfCheckTask::onCheckerFinished(bool success,
                                                  SH85SelfChecker::Result result,
                                                  const QString& message,
                                                  const QString& masterId)
{
    Q_UNUSED(result)
    Q_UNUSED(message)
    Q_UNUSED(masterId)

    if (m_stopRequested) return;

    disconnectChecker();
    finishCurrentDevice(success);
}

void SH85PeriodicSelfCheckTask::finishCurrentDevice(bool success)
{
    if (success) ++m_successCount;
    else         ++m_failureCount;

    m_currentChecker = nullptr;
    m_currentMaster  = nullptr;
    m_currentQrcode.clear();

    ++m_roundIndex;
    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] progressUpdate emit:" << m_roundIndex << "/" << m_roundQrcodes.size();
    emit progressUpdate(m_roundIndex, m_roundQrcodes.size());

    // 立即处理下一个设备（不引入额外间隔；如需间隔可改为 QTimer::singleShot）
    QMetaObject::invokeMethod(this, [this]() { startNextDevice(); }, Qt::QueuedConnection);
}

void SH85PeriodicSelfCheckTask::endRound()
{
    const QString endTime = currentTimestamp();

    qDebug() << "[Scheduler][SH85PeriodicSelfCheckTask] 一轮结束 success=" << m_successCount
             << "failure=" << m_failureCount;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85PeriodicSelfCheckTask] 一轮结束 success=%1 failure=%2")
            .arg(m_successCount).arg(m_failureCount).toStdString());

    emit roundFinished(m_successCount, m_failureCount, m_roundStartTime, endTime);

    if (m_stopRequested) return;

    // 进入下一次空闲倒计时
    startIdleCountdown();
}

// ============================================================
// 辅助
// ============================================================

void SH85PeriodicSelfCheckTask::stopAllTimers()
{
    if (m_idleTimer) {
        m_idleTimer->stop();
    }
}

void SH85PeriodicSelfCheckTask::disconnectChecker()
{
    if (m_checkerFinishedConn) {
        QObject::disconnect(m_checkerFinishedConn);
        m_checkerFinishedConn = QMetaObject::Connection();
    }
}

QString SH85PeriodicSelfCheckTask::currentTimestamp()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}
