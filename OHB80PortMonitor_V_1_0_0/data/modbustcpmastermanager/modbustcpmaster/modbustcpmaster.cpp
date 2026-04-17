#include "modbustcpmaster.h"
#include "loggermanager.h"
#include "app/applogger.h"
#include <QDebug>

// ============================================================
// ModbusTcpMaster - Modbus TCP 主控对象实现
// ============================================================

ModbusTcpMaster::ModbusTcpMaster(const QString& ip, quint16 port, const QString& id, QObject* parent)
    : QObject(parent)
    , m_ip(ip)
    , m_port(port)
    , ID(id)
{
    m_socket = new QTcpSocket(this);
    m_connector = new ModbusConnecter(*m_socket, m_ip, m_port, ID, this);
    m_sender = new ModbusCommandSender(*m_socket, ID, this);
    m_periodicSender = new PeriodicCommandSender(*m_sender, ID, this);

    createInitialIssuerIfNeeded();

    connect(m_connector, &ModbusConnecter::statusChanged,
            this, &ModbusTcpMaster::onConnectionStatusChanged);
    connect(m_connector, &ModbusConnecter::connectionError,
            this, &ModbusTcpMaster::onConnectionError);
    connect(m_periodicSender, &PeriodicCommandSender::disconnectDevice,
            this, &ModbusTcpMaster::onPeriodicDisconnectRequested);
}

bool ModbusTcpMaster::start(ModbusConnecter::ConnectionMode mode)
{
    enterState(State::Connecting);
    return m_connector->connectDevice(mode);
}

void ModbusTcpMaster::stop(ModbusConnecter::ConnectionMode mode)
{
    pauseChildren();
    m_connector->disconnectDevice(mode);
    enterState(State::Idle);
}

ModbusConnecter* ModbusTcpMaster::connector() const
{
    return m_connector;
}

ModbusCommandSender* ModbusTcpMaster::sender() const
{
    return m_sender;
}


InitialCommandIssuer* ModbusTcpMaster::initialIssuer() const
{
    return m_initialIssuer;
}

PeriodicCommandSender* ModbusTcpMaster::periodicSender() const
{
    return m_periodicSender;
}

ModbusTcpMaster::State ModbusTcpMaster::currentState() const
{
    return m_state;
}

void ModbusTcpMaster::onConnectionStatusChanged(ModbusConnecter::ConnectionStatus status, const QString& masterId)
{
    switch (status) {
        case ModbusConnecter::ConnectionStatus::Connected:
            qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] 设备连接成功，准备启动指令发送器";
            resumeChildren();
            break;
        case ModbusConnecter::ConnectionStatus::Disconnected:
            qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] 连接断开，暂停发送器和定时发送器（不干预连接器重连）";
            if (m_initialIssuer && m_initialStarted) {
                m_initialIssuer->stop();
                m_initialStarted = false;
            }
            if (m_periodicSender && m_periodicStarted) {
                m_periodicSender->stop();
                m_periodicStarted = false;
            }
            if (m_sender) {
                m_sender->stop();
            }
            enterState(State::Connecting);
            break;
        case ModbusConnecter::ConnectionStatus::Error:
            break;
        default:
            break;
    }
}

void ModbusTcpMaster::onConnectionError(const QString& message)
{
    qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] 连接阶段错误 -" << message;
    emit errorOccurred(State::Connecting, message);
    enterState(State::Error);
}

void ModbusTcpMaster::startSender()
{
    enterState(State::SenderStartup);
    m_sender->start();
    qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] 指令发送器已启动，立即进入初始化阶段";
    QString logMsg = "[data][ModbusTcpMaster][startSender]：设备ID=" + ID + " 指令发送器已启动，立即进入初始化阶段";
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(ID).toStdString(), Level::INFO, logMsg.toStdString());
}

void ModbusTcpMaster::startInitialIssuer()
{
    createInitialIssuerIfNeeded();
    if (!m_initialIssuer || m_initialStarted) {
        return;
    }

    enterState(State::Initializing);
    m_initialStarted = true;
    m_initialIssuer->start();
}

void ModbusTcpMaster::onInitialFinished(QList<ModbusCommand> failedCommands)
{
    qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] 初始化完成，失败指令数："
             << failedCommands.size();
    QString logMsg = QString("[data][ModbusTcpMaster][onInitialFinished]：设备ID=%1 初始化完成，失败指令数：%2").arg(ID).arg(failedCommands.size());
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(ID).toStdString(), Level::INFO, logMsg.toStdString());

    m_initialStarted = false;

    if (!failedCommands.isEmpty()) {
        QStringList ids;
        for (const auto& cmd : failedCommands) {
            ids << cmd.id;
        }
        emit errorOccurred(State::Initializing,
            QString("初始化完成，%1 条指令下发失败：%2")
                .arg(failedCommands.size())
                .arg(ids.join(", ")));
    }
    m_initialStarted = true;

    if (m_initialIssuer) {
        m_initialIssuer->deleteLater();
        m_initialIssuer = nullptr;
    }

    startPeriodicSender();
}

void ModbusTcpMaster::startPeriodicSender()
{
    if (m_periodicStarted) {
        enterState(State::Running);
        return;
    }

    // 检查 periodicSender 的队列是否为空，避免空队列启动
    if (m_periodicSender && m_periodicSender->commandQueue().isEmpty()) {
        qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] 定时发送器队列为空，跳过启动，直接进入运行状态";
        QString logMsg = "[data][ModbusTcpMaster][startPeriodicSender]：设备ID=" + ID + " 定时发送器队列为空，跳过启动，直接进入运行状态";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(ID).toStdString(), Level::INFO, logMsg.toStdString());
        enterState(State::Running);
        return;
    }

    enterState(State::PeriodicStartup);
    m_periodicStarted = true;
    m_periodicSender->start();

    qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] 定时发送器已启动，进入正常运行状态";
    QString logMsg = "[data][ModbusTcpMaster][startPeriodicSender]：设备ID=" + ID + " 定时发送器已启动，进入正常运行状态";
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(ID).toStdString(), Level::INFO, logMsg.toStdString());
    enterState(State::Running);
}

void ModbusTcpMaster::onPeriodicDisconnectRequested()
{
    const QString msg = QString("定时发送器连续失败达到阈值（%1 次），触发断开重连")
                            .arg(PeriodicCommandSender::MAX_CONSECUTIVE_FAILURES);
    qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] " << msg;
    QString logMsg = QString("[data][ModbusTcpMaster][onPeriodicDisconnectRequested]：设备ID=%1 %2").arg(ID).arg(msg);
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(ID).toStdString(), Level::WARN, logMsg.toStdString());

    emit errorOccurred(State::Running, msg);

    pauseChildren();
    m_connector->disconnectDevice(ModbusConnecter::ConnectionMode::SingleConnection);
    enterState(State::Error);
}

void ModbusTcpMaster::createInitialIssuerIfNeeded()
{
    if (m_initialIssuer) {
        return;
    }

    m_initialIssuer = new InitialCommandIssuer(*m_sender, ID, this);
    connect(m_initialIssuer, &InitialCommandIssuer::finished,
            this, &ModbusTcpMaster::onInitialFinished);
}

void ModbusTcpMaster::pauseChildren()
{
    if (m_initialIssuer && m_initialStarted) {
        m_initialIssuer->stop();
        m_initialStarted = false;
    }

    if (m_periodicSender && m_periodicStarted) {
        m_periodicSender->stop();
        m_periodicStarted = false;
    }

    if (m_sender) {
        m_sender->stop();
    }
}

void ModbusTcpMaster::resumeChildren()
{
    startSender();
    startInitialIssuer();
    if (!m_initialIssuer || m_initialStarted == true) {
        startPeriodicSender();
    }
}

void ModbusTcpMaster::enterState(State state)
{
    if (m_state == state) return;
    State oldState = m_state;
    m_state = state;
    emit stateChanged(state);
    qDebug() << "ModbusTcpMaster: [设备ID=" << ID << "] 状态切换: " << stateToString(oldState) << " -> " << stateToString(state);
    QString logMsg = QString("[data][ModbusTcpMaster][enterState]：设备ID=%1 状态切换: %2 -> %3").arg(ID).arg(stateToString(oldState)).arg(stateToString(state));
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(ID).toStdString(), Level::INFO, logMsg.toStdString());
}

QString ModbusTcpMaster::stateToString(State state)
{
    switch (state) {
        case State::Idle: return "空闲";
        case State::Connecting: return "连接中";
        case State::SenderStartup: return "指令发送器已启动";
        case State::Initializing: return "初始指令下发中";
        case State::PeriodicStartup: return "启动定时发送器中";
        case State::Running: return "正常运行";
        case State::Error: return "错误状态";
        default: return "未知状态";
    }
}
