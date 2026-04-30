#include "set_purge_flow_task.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include "modbustcpmastermanager/modbustcpmaster/modbuscommandsender.h"
#include "modbustcpmastermanager/modbuscommand/commandpool.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>
#include <QTimer>

namespace {
constexpr const char *kCmdId          = "WritePurgeFlow";
constexpr int         kTotalTimeoutMs = 5000;
} // namespace

// ============================================================
// 构造 / 析构
// ============================================================

SetPurgeFlowTask::SetPurgeFlowTask(const QVector<QString> &qrcodes,
                                   int flowValue,
                                   QObject *parent)
    : SchedulerTask(parent)
    , m_qrcodes(qrcodes)
    , m_flowValue(flowValue)
{
    qDebug() << "[Scheduler][SetPurgeFlowTask] 创建任务: qrcodes=" << qrcodes
             << "flow=" << flowValue;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SetPurgeFlowTask] 创建任务：设备数=%1 flow=%2")
            .arg(qrcodes.size()).arg(flowValue).toStdString());
}

SetPurgeFlowTask::~SetPurgeFlowTask()
{
    qDebug() << "[Scheduler][SetPurgeFlowTask] 任务销毁";
}

// ============================================================
// start()
// ============================================================

void SetPurgeFlowTask::start()
{
    setState(Running);
    m_stopped       = false;
    m_totalCount    = 0;
    m_completedCount.storeRelease(0);
    m_pendingMap.clear();
    m_connections.clear();
    m_successCount  = 0;
    m_failedQrCodes.clear();
    m_allFinishedEmitted = false;

    if (m_qrcodes.isEmpty()) {
        setState(Failed);
        emit allFinished(false, 0, {}, m_flowValue);
        emit finished(false, "SetPurgeFlowTask: qrcode 列表为空");
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            "[Scheduler][SetPurgeFlowTask] qrcode 列表为空");
        return;
    }

    ModbusTcpMasterManager &mgr = ModbusTcpMasterManager::instance();
    CommandPool *pool = mgr.commandPool();
    if (!pool || !pool->contains(kCmdId)) {
        setState(Failed);
        emit allFinished(false, 0, {}, m_flowValue);
        emit finished(false, QString("SetPurgeFlowTask: 指令 '%1' 不存在").arg(kCmdId));
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SetPurgeFlowTask] 指令 '%1' 不存在").arg(kCmdId).toStdString());
        return;
    }

    // 流量 × 100 转寄存器值
    const qint64 raw = static_cast<qint64>(m_flowValue) * kRegisterScale;
    const quint16 regVal = static_cast<quint16>(qBound<qint64>(0, raw, 0xFFFF));
    const QByteArray regBytes = buildRegisterValue(regVal);

    qDebug() << "[Scheduler][SetPurgeFlowTask] flow=" << m_flowValue
             << "→ regVal=" << regVal;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SetPurgeFlowTask] flow %1 → regVal %2")
            .arg(m_flowValue).arg(regVal).toStdString());

    for (const QString &id : m_qrcodes) {
        ModbusTcpMaster *master = mgr.getMaster(id);
        if (!master || !master->isConnected() || !master->sender()) {
            qWarning() << "[Scheduler][SetPurgeFlowTask] 设备不可用，跳过:" << id;
            LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
                QString("[Scheduler][SetPurgeFlowTask] 设备 %1 不可用，跳过").arg(id).toStdString());
            m_failedQrCodes.append(id);
            continue;
        }

        ModbusCommandSender *sender = master->sender();
        ModbusCommand cmd = pool->clone(kCmdId);
        if (!cmd.isValid()) {
            qWarning() << "[Scheduler][SetPurgeFlowTask] 克隆指令失败:" << id;
            m_failedQrCodes.append(id);
            continue;
        }

        cmd.module = CommandModule::BusinessCommandIssuer;
        cmd.request.registerValue = regBytes;
        cmd.request.byteCount     = static_cast<quint8>(regBytes.size());

        if (cmd.request.functionCode == 0x06
            && cmd.request.rawBytes.size() >= 6
            && regBytes.size() >= 2) {
            cmd.request.rawBytes[4] = regBytes[0];
            cmd.request.rawBytes[5] = regBytes[1];
        }
        cmd.response.registerValue = regBytes;
        if (cmd.response.rawBytes.size() >= 6 && regBytes.size() >= 2) {
            cmd.response.rawBytes[4] = regBytes[0];
            cmd.response.rawBytes[5] = regBytes[1];
        }

        auto conn = connect(sender, &ModbusCommandSender::commandFinished,
                            this, &SetPurgeFlowTask::onCommandFinished,
                            Qt::QueuedConnection);
        m_connections.append(conn);

        m_pendingMap[cmd.uuid] = id;
        m_totalCount++;

        qDebug() << "[Scheduler][SetPurgeFlowTask] 向设备" << id << "发送 WritePurgeFlow regVal=" << regVal;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
            QString("[Scheduler][SetPurgeFlowTask] 向设备 %1 发送 WritePurgeFlow regVal=%2")
                .arg(id).arg(regVal).toStdString());

        QMetaObject::invokeMethod(sender, [sender, cmd]() {
            sender->submit(cmd);
        }, Qt::QueuedConnection);
    }

    if (m_totalCount == 0) {
        disconnectAll();
        setState(Failed);
        emit allFinished(false, 0, m_failedQrCodes, m_flowValue);
        emit finished(false, QString("SetPurgeFlowTask: 所有设备无法接收指令（失败数=%1）")
                                .arg(m_failedQrCodes.count()));
        return;
    }

    if (!m_timeoutTimer) {
        m_timeoutTimer = new QTimer(this);
        m_timeoutTimer->setSingleShot(true);
        connect(m_timeoutTimer, &QTimer::timeout,
                this, &SetPurgeFlowTask::onTimeout);
    }
    m_timeoutTimer->start(kTotalTimeoutMs);
}

void SetPurgeFlowTask::stop()
{
    m_stopped = true;
    if (m_timeoutTimer) m_timeoutTimer->stop();
    disconnectAll();
    setState(Cancelled);
    emit finished(false, "SetPurgeFlowTask: 任务被取消");
}

void SetPurgeFlowTask::onCommandFinished(ModbusCommand cmd, const QString &masterId)
{
    if (m_stopped) return;
    if (!m_pendingMap.contains(cmd.uuid)) return;
    m_pendingMap.remove(cmd.uuid);

    const bool success = cmd.received && !cmd.timedOut && !cmd.checksumError && !cmd.deviceBusy;

    if (success) {
        ++m_successCount;
        qDebug() << "[Scheduler][SetPurgeFlowTask] 设备" << masterId << "设置成功";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
            QString("[Scheduler][SetPurgeFlowTask] 设备 %1 设置成功").arg(masterId).toStdString());
    } else {
        m_failedQrCodes.append(masterId);
        qWarning() << "[Scheduler][SetPurgeFlowTask] 设备" << masterId
                   << "设置失败 timedOut=" << cmd.timedOut
                   << "checksumError=" << cmd.checksumError
                   << "deviceBusy=" << cmd.deviceBusy;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SetPurgeFlowTask] 设备 %1 设置失败: timedOut=%2 checksumError=%3 deviceBusy=%4")
                .arg(masterId).arg(cmd.timedOut).arg(cmd.checksumError).arg(cmd.deviceBusy).toStdString());
    }

    checkAllFinished();
}

void SetPurgeFlowTask::checkAllFinished()
{
    const int done = m_completedCount.fetchAndAddOrdered(1) + 1;
    if (done < m_totalCount) return;
    forceFinish();
}

void SetPurgeFlowTask::onTimeout()
{
    qWarning() << "[Scheduler][SetPurgeFlowTask] 超时，剩余" << m_pendingMap.size() << "台设备未响应";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
        QString("[Scheduler][SetPurgeFlowTask] 超时，剩余 %1 台设备未响应")
            .arg(m_pendingMap.size()).toStdString());
    forceFinish();
}

void SetPurgeFlowTask::forceFinish()
{
    if (m_allFinishedEmitted) return;
    m_allFinishedEmitted = true;

    if (m_timeoutTimer) m_timeoutTimer->stop();
    disconnectAll();

    for (const QString &qr : m_pendingMap.values())
        m_failedQrCodes.append(qr);
    m_pendingMap.clear();

    const bool allSuccess = m_failedQrCodes.isEmpty();
    setState(allSuccess ? Finished : Failed);

    emit allFinished(allSuccess, m_successCount, m_failedQrCodes, m_flowValue);
    emit finished(allSuccess,
                  allSuccess
                      ? QString("SetPurgeFlowTask: flow=%1 设置完成（%2 台）").arg(m_flowValue).arg(m_successCount)
                      : QString("SetPurgeFlowTask: flow=%1 完成，%2 台成功，%3 台失败")
                            .arg(m_flowValue).arg(m_successCount).arg(m_failedQrCodes.count()));

    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(),
        allSuccess ? Level::INFO : Level::WARN,
        QString("[Scheduler][SetPurgeFlowTask] 任务结束: flow=%1 %2 台成功，%3 台失败")
            .arg(m_flowValue).arg(m_successCount).arg(m_failedQrCodes.count()).toStdString());
}

QByteArray SetPurgeFlowTask::buildRegisterValue(quint16 value) const
{
    QByteArray bytes(2, 0);
    bytes[0] = static_cast<char>((value >> 8) & 0xFF);
    bytes[1] = static_cast<char>(value & 0xFF);
    return bytes;
}

void SetPurgeFlowTask::disconnectAll()
{
    for (const QMetaObject::Connection &conn : qAsConst(m_connections))
        QObject::disconnect(conn);
    m_connections.clear();
}
