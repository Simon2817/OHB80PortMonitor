#include "periodiccommandsender.h"
#include "loggermanager.h"
#include "app/applogger.h"
#include <QDebug>

// ============================================================
// PeriodicCommandSender - 定时循环指令发送器实现
// ============================================================

PeriodicCommandSender::PeriodicCommandSender(ModbusCommandSender& sender, const QString& masterId, QObject* parent)
    : CyclicCommandIssuer(sender, parent)
    , m_masterId(const_cast<QString&>(masterId))
{
    setExecutionCount(0);  // 无限循环
    // 指令成功信号转发为 commandCompleted，并附加 masterId
    connect(this, &CyclicCommandIssuer::commandSucceeded,
            this, [this](ModbusCommand cmd) {
                emit commandCompleted(cmd, m_masterId);
            });
    connect(this, &CyclicCommandIssuer::roundFinished,
            this, &PeriodicCommandSender::onRoundComplete);
    connect(this, &CyclicCommandIssuer::logMessage,
            this, &PeriodicCommandSender::onLogMessage);
}

void PeriodicCommandSender::onRoundComplete(QList<ModbusCommand> failedCommands)
{
    if (failedCommands.isEmpty()) {
        m_consecutiveFailRounds = 0;
        return;
    }

    m_consecutiveFailRounds++;
    qDebug() << "[PeriodicCommandSender] [设备ID=" << m_masterId << "] 本轮失败" << failedCommands.size() << "条"
             << "连续失败轮次:" << m_consecutiveFailRounds;
    QString logMsg = QString("[PeriodicCommandSender][onRoundComplete]：设备ID=%1 本轮失败%2条 连续失败轮次:%3")
            .arg(m_masterId).arg(failedCommands.size()).arg(m_consecutiveFailRounds);
    LoggerManager::instance().log(AppLogger::getModbusMasterLogPath(m_masterId).toStdString(), Level::WARN, logMsg.toStdString());

    if (m_consecutiveFailRounds >= MAX_CONSECUTIVE_FAILURES) {
        qDebug() << "[PeriodicCommandSender] [设备ID=" << m_masterId << "] 连续失败达到阈値，请求断开设备";
        QString disconnectMsg = QString("[PeriodicCommandSender][onRoundComplete]：设备ID=%1 连续失败达到阈值，请求断开设备").arg(m_masterId);
        LoggerManager::instance().log(AppLogger::getModbusMasterLogPath(m_masterId).toStdString(), Level::WARN, disconnectMsg.toStdString());
        stop();
        m_consecutiveFailRounds = 0;
        emit disconnectDevice();
    }
}

void PeriodicCommandSender::onLogMessage(QString message)
{
    LoggerManager::instance().log(AppLogger::getModbusMasterLogPath(m_masterId).toStdString(), Level::INFO, QString("[PeriodicCommandSender]：设备ID=%1 %2").arg(m_masterId).arg(message).toStdString());
}
