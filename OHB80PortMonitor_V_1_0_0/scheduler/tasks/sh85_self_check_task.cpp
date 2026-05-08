#include "sh85_self_check_task.h"

#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include "modbustcpmastermanager/modbustcpmaster/sh85selfchecker.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>

// ============================================================
// 构造 / 析构
// ============================================================

SH85SelfCheckTask::SH85SelfCheckTask(const QString &qrcode, QObject *parent)
    : SchedulerTask(parent)
    , m_qrcode(qrcode)
{
    qDebug() << "[Scheduler][SH85SelfCheckTask] 创建任务: qrcode=" << qrcode;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85SelfCheckTask] 创建任务: qrcode=%1").arg(qrcode).toStdString());
}

SH85SelfCheckTask::~SH85SelfCheckTask()
{
    // 断开 checker 信号连接
    for (const QMetaObject::Connection &c : qAsConst(m_checkerConnections))
        QObject::disconnect(c);
    m_checkerConnections.clear();

    qDebug() << "[Scheduler][SH85SelfCheckTask] 任务销毁: qrcode=" << m_qrcode;
}

// ============================================================
// start()
// ============================================================

void SH85SelfCheckTask::start()
{
    setState(Running);
    m_stopped         = false;
    m_finishedEmitted = false;

    if (m_qrcode.isEmpty()) {
        finishWith(false, Result::StartCommandFailed, "Network Error");
        return;
    }

    if (!ensureMaster()) {
        finishWith(false, Result::StartCommandFailed, "Network Error");
        return;
    }

    m_checker = m_master->selfChecker();
    if (!m_checker) {
        qWarning() << "[Scheduler][SH85SelfCheckTask] master->selfChecker() 返回 null";
        finishWith(false, Result::StartCommandFailed, "Network Error");
        return;
    }

    // 连接 checker 信号
    m_checkerConnections.append(
        connect(m_checker, &SH85SelfChecker::started,
                this, &SH85SelfCheckTask::onCheckerStarted, Qt::QueuedConnection));
    m_checkerConnections.append(
        connect(m_checker, &SH85SelfChecker::countdownTick,
                this, &SH85SelfCheckTask::onCheckerCountdownTick, Qt::QueuedConnection));
    m_checkerConnections.append(
        connect(m_checker, &SH85SelfChecker::errorOccurred,
                this, &SH85SelfCheckTask::onCheckerErrorOccurred, Qt::QueuedConnection));
    m_checkerConnections.append(
        connect(m_checker, &SH85SelfChecker::finished,
                this, &SH85SelfCheckTask::onCheckerFinished, Qt::QueuedConnection));

    // 启动 checker
    if (!m_checker->start()) {
        qWarning() << "[Scheduler][SH85SelfCheckTask] checker->start() 返回 false";
        finishWith(false, Result::StartCommandFailed, "Network Error");
        return;
    }

    // 不再需要启动定时器，自检时序由 SH85SelfChecker 内部控制
}

// ============================================================
// stop()
// ============================================================

void SH85SelfCheckTask::stop()
{
    if (m_finishedEmitted) return;
    m_stopped = true;
    qDebug() << "[Scheduler][SH85SelfCheckTask] stop() 被调用: qrcode=" << m_qrcode;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85SelfCheckTask] 任务被取消: qrcode=%1").arg(m_qrcode).toStdString());

    // 停止 checker
    if (m_checker) {
        m_checker->stop();
    }
}

// ============================================================
// Master 校验
// ============================================================

bool SH85SelfCheckTask::ensureMaster()
{
    ModbusTcpMasterManager &mgr = ModbusTcpMasterManager::instance();
    m_master = mgr.getMaster(m_qrcode);
    if (!m_master || !m_master->isConnected()) {
        qWarning() << "[Scheduler][SH85SelfCheckTask] master 不可用 / 未连接: qrcode=" << m_qrcode;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SH85SelfCheckTask] 设备不可用: qrcode=%1").arg(m_qrcode).toStdString());
        return false;
    }
    return true;
}

// ============================================================
// Checker 信号槽
// ============================================================

void SH85SelfCheckTask::onCheckerStarted(const QString& masterId)
{
    qDebug() << "[Scheduler][SH85SelfCheckTask] checker started masterId=" << masterId;
}

void SH85SelfCheckTask::onCheckerCountdownTick(int remainingSeconds, const QString& masterId)
{
    Q_UNUSED(masterId)
    if (m_finishedEmitted) return;

    // 转发倒计时信号给 UI
    emit countdownTick(remainingSeconds, m_qrcode);

    // 最后 10s 轮询阶段：发出 "Checking (N)" 状态文本驱动按钮
    const int pollWindowSec = SH85SelfChecker::kPollWindowMs / 1000;
    if (remainingSeconds <= pollWindowSec) {
        emit statusChanged(QString("Checking (%1)").arg(remainingSeconds), m_qrcode);
    }
}

void SH85SelfCheckTask::onCheckerErrorOccurred(SH85SelfChecker::Result result, const QString& message, const QString& masterId)
{
    qDebug() << "[Scheduler][SH85SelfCheckTask] checker error result=" << SH85SelfChecker::resultToString(result)
             << "message=" << message << "masterId=" << masterId;

    // 发出 UI 状态
    emit statusChanged(message, m_qrcode);
}

void SH85SelfCheckTask::onCheckerFinished(bool success, SH85SelfChecker::Result result, const QString& message, const QString& masterId)
{
    qDebug() << "[Scheduler][SH85SelfCheckTask] checker finished success=" << success
             << "result=" << SH85SelfChecker::resultToString(result)
             << "message=" << message << "masterId=" << masterId;

    finishWith(success, result, message);
}

// ============================================================
// 内部辅助
// ============================================================

void SH85SelfCheckTask::finishWith(bool success, SH85SelfChecker::Result result, const QString &uiText)
{
    if (m_finishedEmitted) return;
    m_finishedEmitted = true;

    // 断开 checker 信号连接
    for (const QMetaObject::Connection &c : qAsConst(m_checkerConnections))
        QObject::disconnect(c);
    m_checkerConnections.clear();

    setState(success ? Finished : (result == Result::Cancelled ? Cancelled : Failed));

    emit statusChanged(uiText, m_qrcode);
    emit allFinished(success, result, m_qrcode);
    emit finished(success,
                  QString("SH85SelfCheckTask: qrcode=%1 result=%2 (%3)")
                      .arg(m_qrcode).arg(resultToText(result)).arg(uiText));

    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(),
        success ? Level::INFO : Level::WARN,
        QString("[Scheduler][SH85SelfCheckTask] 任务结束 qrcode=%1 result=%2 ui='%3'")
            .arg(m_qrcode).arg(resultToText(result)).arg(uiText).toStdString());
}

QString SH85SelfCheckTask::resultToText(SH85SelfChecker::Result r)
{
    return SH85SelfChecker::resultToString(r);
}
