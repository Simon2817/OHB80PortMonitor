#include "running_logger_task.h"

#include <QDateTime>
#include <QDebug>

#include "logdatabases/databasemanager.h"
#include "logdatabases/operationlogdb/operationlogdbcon.h"
#include "logdatabases/dbtypes.h"

RunningLoggerTask::RunningLoggerTask(QObject* parent)
    : SchedulerTask(parent)
{
    qDebug() << "[RunningLoggerTask] constructed";
}

void RunningLoggerTask::start()
{
    setState(Running);
    qDebug() << "[RunningLoggerTask] started";
    emit progress(0, QStringLiteral("Running logger ready"));
}

void RunningLoggerTask::stop()
{
    setState(Cancelled);
    emit finished(false, QStringLiteral("Running logger stopped"));
    qDebug() << "[RunningLoggerTask] stopped";
}

void RunningLoggerTask::logMessage(const QString& message)
{
    log(MsgType::Message, message);
}

void RunningLoggerTask::logWarn(const QString& message)
{
    log(MsgType::Warn, message);
}

void RunningLoggerTask::logError(const QString& message)
{
    log(MsgType::Error, message);
}

void RunningLoggerTask::log(MsgType type, const QString& message)
{
    auto* db = LogDB::DatabaseManager::instance().operationLogCon();
    if (!db) {
        qWarning() << "[RunningLoggerTask] OperationLogDBCon unavailable, drop:"
                   << message;
        return;
    }

    // MsgType 数值与 LogDB::OperationLogType 对齐，可直接 cast
    const int logType = static_cast<int>(type);
    const QString occurTime =
        QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));

    db->insertRecord(occurTime, logType, message);
}
