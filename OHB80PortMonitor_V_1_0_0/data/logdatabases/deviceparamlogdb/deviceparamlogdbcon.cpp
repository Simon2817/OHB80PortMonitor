#include "deviceparamlogdbcon.h"
#include <QDateTime>
#include <QDebug>

namespace LogDB {

DeviceParamLogDBCon::DeviceParamLogDBCon(const QString& databasePath, WriteSqlDBCon* externalWriteCon, QObject* parent)
    : QObject(parent)
    , m_workerThread(nullptr)
    , m_sqlLogic(nullptr)
    , m_writeCon(externalWriteCon)
{
    Q_ASSERT_X(externalWriteCon != nullptr, "DeviceParamLogDBCon",
               "WriteSqlDBCon must be provided");
    m_workerThread = new QThread(this);
    m_sqlLogic = new DeviceParamLogSqlLogic(databasePath);

    connect(m_workerThread, &QThread::finished, m_sqlLogic, &QObject::deleteLater);
    m_sqlLogic->moveToThread(m_workerThread);

    connect(m_sqlLogic, &DeviceParamLogSqlLogic::writeExecuted,
            this, [this](const WriteResult& result) {
                m_writeCon->addWriteTask(result);
            }, Qt::DirectConnection);

    connect(m_writeCon, &WriteSqlDBCon::taskCompleted,
            this, &DeviceParamLogDBCon::onWriteTaskCompleted, Qt::QueuedConnection);

    m_workerThread->start();
}

DeviceParamLogDBCon::~DeviceParamLogDBCon()
{
    cleanup();
}

bool DeviceParamLogDBCon::initialize()
{
    QMetaObject::invokeMethod(m_sqlLogic, "initializeDatabase",
                              Qt::BlockingQueuedConnection);
    return true;
}

void DeviceParamLogDBCon::cleanup()
{
    if (m_workerThread && m_workerThread->isRunning()) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
    m_sqlLogic = nullptr;
}

void DeviceParamLogDBCon::onWriteTaskCompleted(const WriteResult& result)
{
    qDebug() << "[DeviceParamLog] Write task completed:" << result.connectionName
             << "Result:" << result.result
             << "SQL ID:" << result.sqlId
             << "Params:" << result.params;
}

QList<QVariantMap> DeviceParamLogDBCon::queryPageWithConditions(const QString& qrCode,
                                                                const QString& startTime,
                                                                const QString& endTime,
                                                                int pageSize,
                                                                int pageNumber)
{
    QList<QVariantMap> results;
    QMetaObject::invokeMethod(m_sqlLogic, "queryPageWithConditions",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QList<QVariantMap>, results),
                              Q_ARG(QString, qrCode),
                              Q_ARG(QString, startTime),
                              Q_ARG(QString, endTime),
                              Q_ARG(int, pageSize),
                              Q_ARG(int, pageNumber));
    return results;
}

int DeviceParamLogDBCon::queryTotalCount()
{
    int count = 0;
    QMetaObject::invokeMethod(m_sqlLogic, "queryTotalCount",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(int, count));
    return count;
}

int DeviceParamLogDBCon::queryTotalCountWithConditions(const QString& qrCode,
                                                       const QString& startTime,
                                                       const QString& endTime)
{
    int count = 0;
    QMetaObject::invokeMethod(m_sqlLogic, "queryTotalCountWithConditions",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(int, count),
                              Q_ARG(QString, qrCode),
                              Q_ARG(QString, startTime),
                              Q_ARG(QString, endTime));
    return count;
}

int DeviceParamLogDBCon::queryMonthRange()
{
    QVariantMap result;
    bool success = QMetaObject::invokeMethod(m_sqlLogic, "queryMonthRange",
                                             Qt::BlockingQueuedConnection,
                                             Q_RETURN_ARG(QVariantMap, result));
    if (!success) {
        return -1;
    }

    QString earliestTime = result["earliest_time"].toString();
    QString latestTime = result["latest_time"].toString();

    if (earliestTime.isEmpty() || latestTime.isEmpty() || earliestTime == "NULL" || latestTime == "NULL") {
        return 0;
    }

    QDateTime earliest = QDateTime::fromString(earliestTime, "yyyy-MM-dd HH:mm:ss");
    QDateTime latest = QDateTime::fromString(latestTime, "yyyy-MM-dd HH:mm:ss");

    if (!earliest.isValid() || !latest.isValid()) {
        return 0;
    }

    int yearDiff = latest.date().year() - earliest.date().year();
    int monthDiff = latest.date().month() - earliest.date().month();
    return yearDiff * 12 + monthDiff;
}

void DeviceParamLogDBCon::insertRecord(const QString& qrCode,
                                       const QString& recordTime,
                                       double inletPressure,
                                       double outletPressure,
                                       double inletFlow,
                                       double humidity,
                                       double temperature,
                                       int foupStatus)
{
    QMetaObject::invokeMethod(m_sqlLogic, "insertRecord",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qrCode),
                              Q_ARG(QString, recordTime),
                              Q_ARG(double, inletPressure),
                              Q_ARG(double, outletPressure),
                              Q_ARG(double, inletFlow),
                              Q_ARG(double, humidity),
                              Q_ARG(double, temperature),
                              Q_ARG(int, foupStatus));
}

void DeviceParamLogDBCon::deleteByTimeRange(const QString& startTime, const QString& endTime)
{
    QMetaObject::invokeMethod(m_sqlLogic, "deleteByTimeRange",
                              Qt::QueuedConnection,
                              Q_ARG(QString, startTime),
                              Q_ARG(QString, endTime));
}

} // namespace LogDB
