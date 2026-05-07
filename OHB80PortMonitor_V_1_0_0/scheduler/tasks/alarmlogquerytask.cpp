#include "alarmlogquerytask.h"
#include "databasemanager.h"
#include <QDebug>

AlarmLogQueryTask::AlarmLogQueryTask(QObject *parent)
    : SchedulerTask{parent}
    , m_db(nullptr)
    , m_alarmLevel(-1)
    , m_isResolved(-1)
    , m_pageNumber(0)
    , m_pageSize(500)
{
}

void AlarmLogQueryTask::start()
{
    // 获取警报日志数据库连接
    m_db = LogDB::DatabaseManager::instance().alarmLogCon();
    if (!m_db) {
        setState(Failed);
        emit finished(false, "AlarmLogDBCon unavailable");
        return;
    }

    setState(Running);
    executeQuery();
}

void AlarmLogQueryTask::stop()
{
    setState(Finished);
}

void AlarmLogQueryTask::setPageNumber(int pageNumber)
{
    m_pageNumber = pageNumber;
}

void AlarmLogQueryTask::setPageSize(int pageSize)
{
    m_pageSize = pageSize;
}

void AlarmLogQueryTask::setAlarmLevel(int alarmLevel)
{
    m_alarmLevel = alarmLevel;
}

void AlarmLogQueryTask::setQRCode(const QString& qrCode)
{
    m_qrCode = qrCode;
}

void AlarmLogQueryTask::setAlarmType(const QString& alarmType)
{
    m_alarmType = alarmType;
}

void AlarmLogQueryTask::setIsResolved(int isResolved)
{
    m_isResolved = isResolved;
}

void AlarmLogQueryTask::setOccurTimeRange(const QString& startTime, const QString& endTime)
{
    m_startTime = startTime;
    m_endTime = endTime;
}

void AlarmLogQueryTask::executeQuery()
{
    if (!m_db) {
        setState(Failed);
        emit finished(false, "Database connection not available");
        return;
    }

    // 时间范围钳制：
    //   - 开始 / 结束任一为空 → 用数据库的最早 / 最晚时间补齐
    //   - 请求窗口与 DB 范围有重叠 → 把超出部分钳制到 DB 边界
    //   - 请求窗口完全落在 DB 范围之外 → 保持原值，SQL 自然返回空集
    //   - 数据库为空 → 不钳制，原样下发
    QString dbEarliest;
    QString dbLatest;
    m_db->queryTimeBounds(dbEarliest, dbLatest);
    if (!dbEarliest.isEmpty() && !dbLatest.isEmpty()) {
        const QString effStart = m_startTime.isEmpty() ? dbEarliest : m_startTime;
        const QString effEnd   = m_endTime.isEmpty()   ? dbLatest   : m_endTime;
        const bool noOverlap = (effStart > dbLatest) || (effEnd < dbEarliest);

        if (!noOverlap) {
            if (m_startTime.isEmpty() || m_startTime < dbEarliest) {
                m_startTime = dbEarliest;
            }
            if (m_endTime.isEmpty() || m_endTime > dbLatest) {
                m_endTime = dbLatest;
            }
            if (m_startTime > m_endTime) {
                qSwap(m_startTime, m_endTime);
            }
            qDebug() << "[AlarmLogQueryTask] 钳制后时间范围:"
                     << m_startTime << "->" << m_endTime
                     << " (DB:" << dbEarliest << "->" << dbLatest << ")";
        } else {
            qDebug() << "[AlarmLogQueryTask] 请求窗口与 DB 范围无重叠，不钳制:"
                     << m_startTime << "->" << m_endTime
                     << " (DB:" << dbEarliest << "->" << dbLatest << ")";
        }
    }

    // 有条件查询：当前页中所有满足条件的记录
    if (m_pageNumber > 0) {
        QList<QVariantMap> pageRecords = m_db->queryPageWithConditions(
            m_alarmLevel, m_qrCode, m_alarmType, m_isResolved,
            m_startTime, m_endTime,
            m_pageSize, m_pageNumber);
        emit pageWithConditionsResult(pageRecords);
    }

    // 有条件查询：总记录数
    int totalCountWithConditions = m_db->queryTotalCountWithConditions(
        m_alarmLevel, m_qrCode, m_alarmType, m_isResolved,
        m_startTime, m_endTime);
    emit totalCountWithConditionsResult(totalCountWithConditions);

    setState(Finished);
    emit finished(true, "Query completed successfully");
}
