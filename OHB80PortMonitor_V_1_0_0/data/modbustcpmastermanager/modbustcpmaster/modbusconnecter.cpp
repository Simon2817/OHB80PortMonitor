#include "modbusconnecter.h"
#include "loggermanager.h"
#include "app/applogger.h"
#include <QDebug>

ModbusConnecter::ModbusConnecter(QTcpSocket& socket, const QString& host, quint16 port, const QString& masterId, QObject *parent)
    : QObject(parent)
    , m_socket(&socket)
    , m_host(host)
    , m_port(port)
    , m_masterId(const_cast<QString&>(masterId))
    , m_status(ConnectionStatus::Disconnected)
    , m_autoReconnectEnabled(false)
    , m_retryCount(0)
    , m_reconnectTimer(new QTimer(this))
    , m_connectionCheckTimer(new QTimer(this))
{
    m_reconnectTimer->setInterval(3000);
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &ModbusConnecter::onReconnectTimer);

    m_connectionCheckTimer->setInterval(30000);
    connect(m_connectionCheckTimer, &QTimer::timeout, this, &ModbusConnecter::checkConnection);

    connect(m_socket, &QTcpSocket::disconnected, this, [this]() {
        if (m_status != ConnectionStatus::Connected) {
            return;
        }

        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] Socket disconnected signal received, starting auto-reconnect";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][disconnected --- socket断开信号槽]：设备ID=%1 Socket断开信号接收，开始自动重连").arg(m_masterId).toStdString());
        stopConnectionCheck();
        setStatus(ConnectionStatus::Disconnected);

        if (m_autoReconnectEnabled) {
            m_retryCount = 0;
            startAutoReconnect();
        }

        emit connectionError("Connection lost: " + getErrorString(m_socket->error()));
    });
}

ModbusConnecter::~ModbusConnecter()
{
    stopAutoReconnect();
    stopConnectionCheck();
}

bool ModbusConnecter::connectDevice(ConnectionMode mode)
{
    if (!m_socket) {
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] QTcpSocket 指针为空";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::WARN, QString("[data][ModbusConnecter][connectDevice]：设备ID=%1 QTcpSocket 指针为空").arg(m_masterId).toStdString());
        emit connectionError("QTcpSocket 无效");
        return false;
    }

    if (mode == ConnectionMode::AutoReconnect) {
        m_autoReconnectEnabled = true;
        m_retryCount = 0;
    } else {
        m_autoReconnectEnabled = false;
        stopAutoReconnect();
    }
    
    setStatus(ConnectionStatus::Connecting);

    if (performConnection()) {
        setStatus(ConnectionStatus::Connected);

        if (m_autoReconnectEnabled) {
            QTimer::singleShot(10000, this, [this]() {
                if (m_status == ConnectionStatus::Connected) {
                    startConnectionCheck();
                }
            });
        }
        return true;
    } else {
        setStatus(ConnectionStatus::Error);
        emit connectionError(m_lastError);

        if (m_autoReconnectEnabled) {
            startAutoReconnect();
        }

        return false;
    }
}

bool ModbusConnecter::disconnectDevice(ConnectionMode mode)
{
    if (!m_socket) {
        return false;
    }

    if (mode == ConnectionMode::SingleConnection) {
        stopConnectionCheck();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->disconnectFromHost();
        }
        setStatus(ConnectionStatus::Disconnected);
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] 单次断开 - 主动断开完成";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][disconnectDevice]：设备ID=%1 单次断开 - 主动断开完成").arg(m_masterId).toStdString());

        if (m_autoReconnectEnabled) {
            m_retryCount = 0;
            startAutoReconnect();
        }
        return true;
    } else {
        stopAutoReconnect();
        stopConnectionCheck();

        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->disconnectFromHost();
        }
        setStatus(ConnectionStatus::Disconnected);
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] 完全断开 - 已停止自动重连";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][disconnectDevice]：设备ID=%1 完全断开 - 已停止自动重连").arg(m_masterId).toStdString());
        return true;
    }
}

void ModbusConnecter::setAutoReconnectInterval(int intervalMs)
{
    m_reconnectTimer->setInterval(intervalMs);
}

bool ModbusConnecter::performConnection()
{
    if (!m_socket) {
        m_lastError = "No TCP socket available";
        return false;
    }

    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        return true;
    }

    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
    }

    QString logMsg = QString("设备ID=%1 正在尝试连接服务器 %2:%3").arg(m_masterId).arg(m_host).arg(m_port);
    qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] " << logMsg;
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][performConnection]：%1").arg(logMsg).toStdString());
    m_socket->connectToHost(m_host, m_port);
    if (!m_socket->waitForConnected(3000)) {
        QString logMsg = QString("设备ID=%1 TCP 连接失败 - error=%2").arg(m_masterId).arg(m_socket->errorString());
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] " << logMsg;
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::WARN, QString("[data][ModbusConnecter][performConnection]：%1").arg(logMsg).toStdString());
        m_lastError = getErrorString(m_socket->error());
        return false;
    }

    return true;
}

void ModbusConnecter::setStatus(ConnectionStatus status)
{
    if (m_status != status) {
        ConnectionStatus oldStatus = m_status;
        m_status = status;
        emit statusChanged(status, m_masterId);

        QString oldStatusStr;
        switch (oldStatus) {
            case ConnectionStatus::Disconnected:
                oldStatusStr = "已断开";
                break;
            case ConnectionStatus::Connecting:
                oldStatusStr = "连接中";
                break;
            case ConnectionStatus::Connected:
                oldStatusStr = "已连接";
                break;
            case ConnectionStatus::Error:
                oldStatusStr = "错误";
                break;
        }

        QString newStatusStr;
        switch (status) {
            case ConnectionStatus::Disconnected:
                newStatusStr = "已断开";
                break;
            case ConnectionStatus::Connecting:
                newStatusStr = "连接中";
                break;
            case ConnectionStatus::Connected:
                newStatusStr = "已连接";
                break;
            case ConnectionStatus::Error:
                newStatusStr = "错误";
                break;
        }
        QString logMsg = QString("设备ID=%1 状态转变：%2 -> %3").arg(m_masterId).arg(oldStatusStr).arg(newStatusStr);
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] " << logMsg;
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][setStatus]：%1").arg(logMsg).toStdString());
    }
}

void ModbusConnecter::startAutoReconnect()
{
    if (!m_autoReconnectEnabled || m_reconnectTimer->isActive()) {
        return;
    }
    
    qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] Starting auto-reconnect";
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][startAutoReconnect]：设备ID=%1 开始自动重连").arg(m_masterId).toStdString());
    m_reconnectTimer->start();
}

void ModbusConnecter::stopAutoReconnect()
{
    if (m_reconnectTimer->isActive()) {
        m_reconnectTimer->stop();
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] Stopped auto-reconnect";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][stopAutoReconnect]：设备ID=%1 停止自动重连").arg(m_masterId).toStdString());
    }
    
    m_autoReconnectEnabled = false;
}

void ModbusConnecter::startConnectionCheck()
{
    if (!m_connectionCheckTimer->isActive()) {
        m_connectionCheckTimer->start();
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] Started connection check";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][startConnectionCheck]：设备ID=%1 启动连接心跳检查").arg(m_masterId).toStdString());
    }
}

void ModbusConnecter::stopConnectionCheck()
{
    if (m_connectionCheckTimer->isActive()) {
        m_connectionCheckTimer->stop();
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] Stopped connection check";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][stopConnectionCheck]：设备ID=%1 停止连接心跳检查").arg(m_masterId).toStdString());
    }
}

void ModbusConnecter::onReconnectTimer()
{
    if (!m_autoReconnectEnabled || !m_socket) {
        return;
    }

    m_retryCount++;
    QString logMsg = QString("设备ID=%1 第 %2 次重连尝试").arg(m_masterId).arg(m_retryCount);
    qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] " << logMsg;
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][onReconnectTimer]：%1").arg(logMsg).toStdString());

    if (performConnection()) {
        setStatus(ConnectionStatus::Connected);
        m_retryCount = 0;

        QTimer::singleShot(10000, this, [this]() {
            if (m_status == ConnectionStatus::Connected) {
                startConnectionCheck();
            }
        });
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] Reconnection successful";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][onReconnectTimer]：设备ID=%1 重连成功").arg(m_masterId).toStdString());
    } else {
        setStatus(ConnectionStatus::Error);
        emit connectionError(m_lastError);
        m_reconnectTimer->start();
    }
}

void ModbusConnecter::checkConnection()
{
    if (!m_socket || m_status != ConnectionStatus::Connected) {
        return;
    }

    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] Connection lost (socket disconnected), starting auto-reconnect";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::WARN, QString("[data][ModbusConnecter][checkConnection]：设备ID=%1 连接丢失（socket断开），开始自动重连").arg(m_masterId).toStdString());
        setStatus(ConnectionStatus::Disconnected);

        if (m_autoReconnectEnabled) {
            m_retryCount = 0;
            startAutoReconnect();
        }
        emit connectionError("Connection lost: " + getErrorString(m_socket->error()));
    } else {
        qDebug() << "ModbusConnecter: [设备ID=" << m_masterId << "] Connection check - socket OK";
        LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[data][ModbusConnecter][checkConnection]：设备ID=%1 连接检查 - socket正常").arg(m_masterId).toStdString());
    }
}

QString ModbusConnecter::getErrorString(QAbstractSocket::SocketError errorCode) const
{
    if (!m_socket) {
        return QString("Socket 无效");
    }

    if (errorCode == QAbstractSocket::UnknownSocketError) {
        return m_socket->errorString();
    }

    return m_socket->errorString();
}
