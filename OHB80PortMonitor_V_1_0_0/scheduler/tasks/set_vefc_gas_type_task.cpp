#include "set_vefc_gas_type_task.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include "modbustcpmastermanager/modbustcpmaster/modbuscommandsender.h"
#include "modbustcpmastermanager/modbuscommand/commandpool.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>
#include <QTimer>

namespace {
constexpr const char *kCmdId          = "WriteVEFCGasType";
constexpr int         kTotalTimeoutMs = 5000;
} // namespace

SetVEFCGasTypeTask::SetVEFCGasTypeTask(const QVector<QString> &qrcodes,
                                       int gasType,
                                       QObject *parent)
    : SchedulerTask(parent)
    , m_qrcodes(qrcodes)
    , m_gasType(gasType)
{
    qDebug() << "[Scheduler][SetVEFCGasTypeTask] 创建任务: qrcodes=" << qrcodes
             << "gasType=" << gasType;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SetVEFCGasTypeTask] 创建任务：设备数=%1 gasType=%2")
            .arg(qrcodes.size()).arg(gasType).toStdString());
}

SetVEFCGasTypeTask::~SetVEFCGasTypeTask()
{
    qDebug() << "[Scheduler][SetVEFCGasTypeTask] 任务销毁";
}

void SetVEFCGasTypeTask::start()
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
        emit allFinished(false, 0, {}, m_gasType);
        emit finished(false, "SetVEFCGasTypeTask: qrcode 列表为空");
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            "[Scheduler][SetVEFCGasTypeTask] qrcode 列表为空");
        return;
    }

    ModbusTcpMasterManager &mgr = ModbusTcpMasterManager::instance();
    CommandPool *pool = mgr.commandPool();
    if (!pool || !pool->contains(kCmdId)) {
        setState(Failed);
        emit allFinished(false, 0, {}, m_gasType);
        emit finished(false, QString("SetVEFCGasTypeTask: 指令 '%1' 不存在").arg(kCmdId));
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SetVEFCGasTypeTask] 指令 '%1' 不存在").arg(kCmdId).toStdString());
        return;
    }

    const quint16 regVal = static_cast<quint16>(qBound(0, m_gasType, 0xFFFF));
    const QByteArray regBytes = buildRegisterValue(regVal);

    qDebug() << "[Scheduler][SetVEFCGasTypeTask] gasType=" << m_gasType << "→ regVal=" << regVal;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SetVEFCGasTypeTask] gasType=%1 regVal=%2").arg(m_gasType).arg(regVal).toStdString());

    for (const QString &id : m_qrcodes) {
        ModbusTcpMaster *master = mgr.getMaster(id);
        if (!master || !master->isConnected() || !master->sender()) {
            qWarning() << "[Scheduler][SetVEFCGasTypeTask] 设备不可用，跳过:" << id;
            LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
                QString("[Scheduler][SetVEFCGasTypeTask] 设备 %1 不可用，跳过").arg(id).toStdString());
            m_failedQrCodes.append(id);
            continue;
        }

        ModbusCommandSender *sender = master->sender();
        ModbusCommand cmd = pool->clone(kCmdId);
        if (!cmd.isValid()) {
            qWarning() << "[Scheduler][SetVEFCGasTypeTask] 克隆指令失败:" << id;
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
                            this, &SetVEFCGasTypeTask::onCommandFinished,
                            Qt::QueuedConnection);
        m_connections.append(conn);

        m_pendingMap[cmd.uuid] = id;
        m_totalCount++;

        qDebug() << "[Scheduler][SetVEFCGasTypeTask] 向设备" << id << "发送 WriteVEFCGasType regVal=" << regVal;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
            QString("[Scheduler][SetVEFCGasTypeTask] 向设备 %1 发送 regVal=%2").arg(id).arg(regVal).toStdString());

        QMetaObject::invokeMethod(sender, [sender, cmd]() {
            sender->submit(cmd);
        }, Qt::QueuedConnection);
    }

    if (m_totalCount == 0) {
        disconnectAll();
        setState(Failed);
        emit allFinished(false, 0, m_failedQrCodes, m_gasType);
        emit finished(false, QString("SetVEFCGasTypeTask: 所有设备无法接收指令（失败数=%1）")
                                .arg(m_failedQrCodes.count()));
        return;
    }

    if (!m_timeoutTimer) {
        m_timeoutTimer = new QTimer(this);
        m_timeoutTimer->setSingleShot(true);
        connect(m_timeoutTimer, &QTimer::timeout,
                this, &SetVEFCGasTypeTask::onTimeout);
    }
    m_timeoutTimer->start(kTotalTimeoutMs);
}

void SetVEFCGasTypeTask::stop()
{
    m_stopped = true;
    if (m_timeoutTimer) m_timeoutTimer->stop();
    disconnectAll();
    setState(Cancelled);
    emit finished(false, "SetVEFCGasTypeTask: 任务被取消");
}

void SetVEFCGasTypeTask::onCommandFinished(ModbusCommand cmd, const QString &masterId)
{
    if (m_stopped) return;
    if (!m_pendingMap.contains(cmd.uuid)) return;
    m_pendingMap.remove(cmd.uuid);

    const bool success = cmd.received && !cmd.timedOut && !cmd.checksumError && !cmd.deviceBusy;

    if (success) {
        ++m_successCount;
        qDebug() << "[Scheduler][SetVEFCGasTypeTask] 设备" << masterId << "设置成功";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
            QString("[Scheduler][SetVEFCGasTypeTask] 设备 %1 设置成功").arg(masterId).toStdString());
    } else {
        m_failedQrCodes.append(masterId);
        qWarning() << "[Scheduler][SetVEFCGasTypeTask] 设备" << masterId
                   << "设置失败 timedOut=" << cmd.timedOut
                   << "checksumError=" << cmd.checksumError
                   << "deviceBusy=" << cmd.deviceBusy;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SetVEFCGasTypeTask] 设备 %1 设置失败: timedOut=%2 checksumError=%3 deviceBusy=%4")
                .arg(masterId).arg(cmd.timedOut).arg(cmd.checksumError).arg(cmd.deviceBusy).toStdString());
    }

    checkAllFinished();
}

void SetVEFCGasTypeTask::checkAllFinished()
{
    const int done = m_completedCount.fetchAndAddOrdered(1) + 1;
    if (done < m_totalCount) return;
    forceFinish();
}

void SetVEFCGasTypeTask::onTimeout()
{
    qWarning() << "[Scheduler][SetVEFCGasTypeTask] 超时，剩余" << m_pendingMap.size() << "台设备未响应";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
        QString("[Scheduler][SetVEFCGasTypeTask] 超时，剩余 %1 台设备未响应")
            .arg(m_pendingMap.size()).toStdString());
    forceFinish();
}

void SetVEFCGasTypeTask::forceFinish()
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

    emit allFinished(allSuccess, m_successCount, m_failedQrCodes, m_gasType);
    emit finished(allSuccess,
                  allSuccess
                      ? QString("SetVEFCGasTypeTask: gasType=%1 设置完成（%2 台）")
                            .arg(m_gasType).arg(m_successCount)
                      : QString("SetVEFCGasTypeTask: gasType=%1 完成，%2 台成功，%3 台失败")
                            .arg(m_gasType).arg(m_successCount).arg(m_failedQrCodes.count()));

    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(),
        allSuccess ? Level::INFO : Level::WARN,
        QString("[Scheduler][SetVEFCGasTypeTask] 任务结束: gasType=%1 %2 台成功，%3 台失败")
            .arg(m_gasType).arg(m_successCount).arg(m_failedQrCodes.count()).toStdString());
}

QByteArray SetVEFCGasTypeTask::buildRegisterValue(quint16 value) const
{
    QByteArray bytes(2, 0);
    bytes[0] = static_cast<char>((value >> 8) & 0xFF);
    bytes[1] = static_cast<char>(value & 0xFF);
    return bytes;
}

void SetVEFCGasTypeTask::disconnectAll()
{
    for (const QMetaObject::Connection &conn : qAsConst(m_connections))
        QObject::disconnect(conn);
    m_connections.clear();
}
