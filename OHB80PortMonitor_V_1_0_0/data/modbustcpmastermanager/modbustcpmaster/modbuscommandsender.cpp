#include "modbuscommandsender.h"
#include "loggermanager.h"
#include "app/applogger.h"
#include <QDateTime>
#include <QDebug>
#include <QString>

namespace {
static inline QString nowStr()
{
    return QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
}

static inline QString toHexSpaced(const QByteArray& data)
{
    QString s;
    s.reserve(data.size() * 3);
    for (unsigned char b : data) {
        s += QString::asprintf("%02X ", b);
    }
    if (!s.isEmpty()) s.chop(1);
    return s;
}
}

ModbusCommandSender::ModbusCommandSender(QTcpSocket& socket, const QString& masterId, QObject* parent)
    : QObject(parent)
    , m_masterId(const_cast<QString&>(masterId))
{
    m_socket = &socket;
    m_receiver = new ModbusCommandReceiver(socket, masterId, this);
    connect(m_receiver, &ModbusCommandReceiver::commandSucceeded,
            this, &ModbusCommandSender::onReceiverSucceeded);
    connect(m_receiver, &ModbusCommandReceiver::commandFailed,
            this, &ModbusCommandSender::onReceiverFailed);
}

void ModbusCommandSender::submit(const ModbusCommand& cmd)
{
    QMutexLocker locker(&m_mutex);

    QueueState* qs = nullptr;
    QString queueName;

    switch (cmd.module) {
        case CommandModule::InitialCommandIssuer:
            qs = &m_initialState;
            queueName = "初始下发指令队列";
            break;
        case CommandModule::BusinessCommandIssuer:
            qs = &m_businessState;
            queueName = "业务指令队列";
            break;
        case CommandModule::PeriodicCommandSender:
            qs = &m_periodicState;
            queueName = "定时指令队列";
            break;
    }

    if (!qs || !enqueue(*qs, cmd)) {
        ModbusCommand rejected = cmd;
        rejected.deviceBusy = true;
        rejected.received = false;
        rejected.timedOut = false;
        rejected.checksumError = false;
        rejected.errorMessage = QString("设备繁忙，拒绝指令下发（%1已满）").arg(queueName);
        qDebug() << "[BUSY-REJECT] [设备ID=" << m_masterId << "] " << nowStr()
                 << "id=" << rejected.id
                 << "uuid=" << rejected.uuid
                 << "queue=" << queueName;
        QString logMsg = QString("设备繁忙，指令被拒绝 - 设备ID=%1 id=%2 uuid=%3 queue=%4").arg(m_masterId).arg(rejected.id).arg(rejected.uuid).arg(queueName);
        LoggerManager::instance().log(AppLogger::getModbusMasterLogPath(m_masterId).toStdString(), Level::WARN, QString("[ModbusCommandSender][submit]：%1").arg(logMsg).toStdString());
        locker.unlock();
        emit commandFinished(rejected);
        return;
    }

    bool shouldDispatch = (m_running && !m_hasPendingCommand);
    locker.unlock();

    if (shouldDispatch) {
        dispatch();
    }
}

void ModbusCommandSender::setQueueCapacity(int capacity)
{
    QMutexLocker locker(&m_mutex);
    m_queueCapacity = capacity;
}

void ModbusCommandSender::start()
{
    m_running = true;
    if (!m_hasPendingCommand) {
        dispatch();
    }
}

void ModbusCommandSender::stop()
{
    m_running = false;
    m_hasPendingCommand = false;
    if (m_receiver) {
        m_receiver->cancelPending();
    }
}

void ModbusCommandSender::dispatch()
{
    if (!m_running || m_hasPendingCommand || !m_socket || !m_receiver) {
        return;
    }

    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        return;
    }

    if (!m_retryState.queue.isEmpty()) {
        doSend(m_retryState.queue.dequeue());
        return;
    }

    if (!m_initialState.queue.isEmpty()) {
        doSend(m_initialState.queue.dequeue());
        return;
    }

    if (!m_businessState.queue.isEmpty()) {
        doSend(m_businessState.queue.dequeue());
        return;
    }

    if (!m_periodicState.queue.isEmpty()) {
        doSend(m_periodicState.queue.dequeue());
        return;
    }
}

void ModbusCommandSender::onReceiverSucceeded(ModbusCommand cmd)
{
    finishCurrentCommand(cmd);
}

void ModbusCommandSender::onReceiverFailed(ModbusCommand cmd, bool timedOut, bool checksumError)
{
    handleFailedCommand(cmd, cmd.errorMessage, timedOut, checksumError);
}

void ModbusCommandSender::doSend(ModbusCommand cmd)
{
    if (!m_socket) {
        cmd.errorMessage = "未绑定QTcpSocket";
        cmd.timedOut = false;
        cmd.checksumError = false;
        cmd.deviceBusy = false;
        emit commandFinished(cmd);
        return;
    }

    const QByteArray requestFrame = buildRequestFrame(cmd);
    if (requestFrame.isEmpty()) {
        cmd.errorMessage = "请求帧为空，无法发送";
        cmd.timedOut = false;
        cmd.checksumError = false;
        cmd.deviceBusy = false;
        emit commandFinished(cmd);
        return;
    }

    cmd.sentMs = QDateTime::currentMSecsSinceEpoch();
    cmd.sendCount++;
    cmd.received = false;
    cmd.timedOut = false;
    cmd.checksumError = false;
    cmd.deviceBusy = false;
    if (requestFrame.size() >= 2) {
        cmd.request.crc = requestFrame.right(2);
    }

    QString moduleStr;
    switch (cmd.module) {
        case CommandModule::InitialCommandIssuer:  moduleStr = "INITIAL"; break;
        case CommandModule::PeriodicCommandSender: moduleStr = "PERIODIC"; break;
        case CommandModule::BusinessCommandIssuer: moduleStr = "BUSINESS"; break;
    }

    // 拼接日志字符串
    QString logMsg = QString("[发送] %1 设备ID=%2 module=%3 id=%4 uuid=%5 sendCount=%6/%7 timeout=%8ms crc=%9 len=%10")
            .arg(nowStr())
            .arg(m_masterId)
            .arg(moduleStr)
            .arg(cmd.id)
            .arg(cmd.uuid)
            .arg(cmd.sendCount)
            .arg(cmd.maxRetryCount + 1)
            .arg(cmd.timeoutMs)
            .arg(toHexSpaced(cmd.request.crc))
            .arg(requestFrame.size());

    // frame 部分换行处理
    QString frameHex = toHexSpaced(requestFrame);
    QString logMsgWithFrame = logMsg + "\nframe=" + frameHex;

    qDebug() << logMsgWithFrame;

    // 写入文件日志
    LoggerManager::instance().log(AppLogger::getModbusFrameMessageLogPath(m_masterId).toStdString(), Level::INFO, logMsgWithFrame.toStdString());
    LoggerManager::instance().log(AppLogger::getModbusMasterLogPath(m_masterId).toStdString(), Level::INFO, QString("[ModbusCommandSender][doSend]：%1").arg(logMsgWithFrame).toStdString());

    const qint64 written = m_socket->write(requestFrame);
    if (written != requestFrame.size()) {
        handleFailedCommand(cmd, QString("发送失败: %1").arg(m_socket->errorString()), false, false);
        return;
    }

    m_socket->flush();
    const qint64 deadline = QDateTime::currentMSecsSinceEpoch() + qMin(cmd.timeoutMs, 1000);
    while (m_socket->bytesToWrite() > 0) {
        const int remaining = static_cast<int>(deadline - QDateTime::currentMSecsSinceEpoch());
        if (remaining <= 0) {
            handleFailedCommand(cmd, "等待发送缓冲区写出超时", false, false);
            return;
        }
        const int chunk = remaining < 200 ? remaining : 200;
        if (!m_socket->waitForBytesWritten(chunk)) {
            handleFailedCommand(cmd, "等待发送缓冲区写出超时", false, false);
            return;
        }
    }

    m_pendingCommand = cmd;
    m_hasPendingCommand = true;

    if (!m_receiver->beginReceive(cmd)) {
        handleFailedCommand(cmd, "接收器忙，无法登记待响应指令", false, false);
    }
}

bool ModbusCommandSender::enqueue(QueueState& qs, const ModbusCommand& cmd)
{
    if (qs.queue.size() >= m_queueCapacity) {
        return false;
    }
    qs.queue.enqueue(cmd);
    return true;
}

QByteArray ModbusCommandSender::buildRequestFrame(const ModbusCommand& cmd) const
{
    if (cmd.request.rawBytes.isEmpty()) {
        return {};
    }

    QByteArray frame = cmd.request.rawBytes;
    const quint16 crc = crc16(frame);
    frame.append(static_cast<char>(crc & 0xFF));
    frame.append(static_cast<char>((crc >> 8) & 0xFF));
    return frame;
}

void ModbusCommandSender::finishCurrentCommand(ModbusCommand cmd)
{
    m_hasPendingCommand = false;
    emit commandFinished(cmd);
    if (m_running) {
        dispatch();
    }
}

void ModbusCommandSender::handleFailedCommand(ModbusCommand cmd, const QString& errorMessage, bool timedOut, bool checksumError)
{
    m_hasPendingCommand = false;

    cmd.errorMessage = errorMessage;
    cmd.timedOut = timedOut;
    cmd.checksumError = checksumError;

    QString moduleStr;
    switch (cmd.module) {
        case CommandModule::InitialCommandIssuer:  moduleStr = "INITIAL"; break;
        case CommandModule::PeriodicCommandSender: moduleStr = "PERIODIC"; break;
        case CommandModule::BusinessCommandIssuer: moduleStr = "BUSINESS"; break;
    }

    const bool shouldRetry = !cmd.deviceBusy && !cmd.checksumError
                           && cmd.sendCount <= cmd.maxRetryCount;

    if (!shouldRetry) {
        qDebug() << "[FAILED] [设备ID=" << m_masterId << "] " << nowStr()
                 << "module=" << moduleStr
                 << "id=" << cmd.id
                 << "uuid=" << cmd.uuid
                 << "error=" << errorMessage
                 << "sendCount=" << cmd.sendCount
                 << "/" << (cmd.maxRetryCount + 1)
                 << (cmd.deviceBusy ? " (deviceBusy)" : QString())
                 << (cmd.checksumError ? " (checksumError)" : QString());
        QString logMsg = QString("指令失败 - 设备ID=%1 module=%2 id=%3 uuid=%4 error=%5 sendCount=%6/%7%8%9")
                .arg(m_masterId).arg(moduleStr).arg(cmd.id).arg(cmd.uuid).arg(errorMessage)
                .arg(cmd.sendCount).arg(cmd.maxRetryCount + 1)
                .arg(cmd.deviceBusy ? " (deviceBusy)" : "")
                .arg(cmd.checksumError ? " (checksumError)" : "");
        LoggerManager::instance().log(AppLogger::getModbusMasterLogPath(m_masterId).toStdString(), Level::WARN, QString("[ModbusCommandSender][handleFailedCommand]：%1").arg(logMsg).toStdString());
        emit commandFinished(cmd);
        if (m_running) {
            dispatch();
        }
        return;
    }

    qDebug() << "[RETRY] [设备ID=" << m_masterId << "] " << nowStr()
             << "module=" << moduleStr
             << "id=" << cmd.id
             << "uuid=" << cmd.uuid
             << "error=" << errorMessage
             << "sendCount=" << cmd.sendCount
             << "/" << (cmd.maxRetryCount + 1);
    QString logMsg = QString("指令重试 - 设备ID=%1 module=%2 id=%3 uuid=%4 error=%5 sendCount=%6/%7")
            .arg(m_masterId).arg(moduleStr).arg(cmd.id).arg(cmd.uuid).arg(errorMessage)
            .arg(cmd.sendCount).arg(cmd.maxRetryCount + 1);
    LoggerManager::instance().log(AppLogger::getModbusMasterLogPath(m_masterId).toStdString(), Level::INFO, QString("[ModbusCommandSender][handleFailedCommand]：%1").arg(logMsg).toStdString());

    addToRetryQueue(cmd);
}

void ModbusCommandSender::addToRetryQueue(ModbusCommand cmd)
{
    if (!enqueue(m_retryState, cmd)) {
        cmd.errorMessage = "重发队列已满，放弃发送";
        emit commandFinished(cmd);
    } else {
        QString moduleStr;
        switch (cmd.module) {
            case CommandModule::InitialCommandIssuer:  moduleStr = "INITIAL"; break;
            case CommandModule::PeriodicCommandSender: moduleStr = "PERIODIC"; break;
            case CommandModule::BusinessCommandIssuer: moduleStr = "BUSINESS"; break;
        }
        qDebug() << "[RETRY-QUEUE] [设备ID=" << m_masterId << "] " << nowStr()
                 << "module=" << moduleStr
                 << "id=" << cmd.id
                 << "uuid=" << cmd.uuid
                 << "sendCount=" << cmd.sendCount
                 << "/" << (cmd.maxRetryCount + 1);
        QString logMsg = QString("加入重发队列 - 设备ID=%1 module=%2 id=%3 uuid=%4 sendCount=%5/%6")
                .arg(m_masterId).arg(moduleStr).arg(cmd.id).arg(cmd.uuid).arg(cmd.sendCount).arg(cmd.maxRetryCount + 1);
        LoggerManager::instance().log(AppLogger::getModbusMasterLogPath(m_masterId).toStdString(), Level::INFO, QString("[ModbusCommandSender][addToRetryQueue]：%1").arg(logMsg).toStdString());
    }

    if (m_running) {
        dispatch();
    }
}

quint16 ModbusCommandSender::crc16(const QByteArray& data)
{
    quint16 crc = 0xFFFF;
    for (char byte : data) {
        crc ^= static_cast<quint8>(byte);
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x0001) {
                crc = static_cast<quint16>((crc >> 1) ^ 0xA001);
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
