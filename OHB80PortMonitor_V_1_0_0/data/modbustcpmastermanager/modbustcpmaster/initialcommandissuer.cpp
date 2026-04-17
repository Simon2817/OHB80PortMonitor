#include "initialcommandissuer.h"
#include "loggermanager.h"
#include "app/applogger.h"
#include <QDebug>

// ============================================================
// InitialCommandIssuer - 初始指令下发器实现
// ============================================================

InitialCommandIssuer::InitialCommandIssuer(ModbusCommandSender& sender, const QString& masterId, QObject* parent)
    : CyclicCommandIssuer(sender, parent)
    , m_masterId(const_cast<QString&>(masterId))
{
    setExecutionCount(1);
    connect(this, &CyclicCommandIssuer::roundFinished,
            this, &InitialCommandIssuer::onRoundComplete);
    connect(this, &CyclicCommandIssuer::logMessage,
            this, &InitialCommandIssuer::onLogMessage);
}

void InitialCommandIssuer::onRoundComplete(QList<ModbusCommand> failedCommands)
{
    qDebug() << "[InitialCommandIssuer] [设备ID=" << m_masterId << "] 完成，失败" << failedCommands.size() << "条";
    QString logMsg = QString("[InitialCommandIssuer][onRoundComplete]：设备ID=%1 完成，失败%2条").arg(m_masterId).arg(failedCommands.size());
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, logMsg.toStdString());
    emit finished(failedCommands);
    deleteLater();
}

void InitialCommandIssuer::onLogMessage(QString message)
{
    LoggerManager::instance().log(AppLogger::ModbusMasterLoggerPath(m_masterId).toStdString(), Level::INFO, QString("[InitialCommandIssuer]：设备ID=%1 %2").arg(m_masterId).arg(message).toStdString());
}
