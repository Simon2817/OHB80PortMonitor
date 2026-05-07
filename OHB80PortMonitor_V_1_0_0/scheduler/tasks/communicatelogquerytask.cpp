#include "communicatelogquerytask.h"
#include "databasemanager.h"
#include <QDebug>

CommunicateLogQueryTask::CommunicateLogQueryTask(QObject *parent)
    : SchedulerTask{parent}
    , m_db(nullptr)
    , m_execStatus(-1)
    , m_retryCount(-1)
    , m_pageNumber(0)
    , m_pageSize(500)
    , m_sortOrder(LogDB::SortOrder::Desc)
{
}

void CommunicateLogQueryTask::start()
{
    // 获取通讯日志数据库连接
    m_db = LogDB::DatabaseManager::instance().communicateLogCon();
    if (!m_db) {
        setState(Failed);
        emit finished(false, "CommunicateLogDBCon unavailable");
        return;
    }

    setState(Running);
    executeQuery();
}

void CommunicateLogQueryTask::stop()
{
    setState(Finished);
}

void CommunicateLogQueryTask::setPageNumber(int pageNumber)
{
    m_pageNumber = pageNumber;
}

void CommunicateLogQueryTask::setPageSize(int pageSize)
{
    m_pageSize = pageSize;
}

void CommunicateLogQueryTask::setCommandId(const QString& commandId)
{
    m_commandId = commandId;
}

void CommunicateLogQueryTask::setQRCode(const QString& qrCode)
{
    m_qrCode = qrCode;
}

void CommunicateLogQueryTask::setExecStatus(int execStatus)
{
    m_execStatus = execStatus;
}

void CommunicateLogQueryTask::setRetryCount(int retryCount)
{
    m_retryCount = retryCount;
}

void CommunicateLogQueryTask::setSendTimeRange(const QString& startTime, const QString& endTime)
{
    m_startTime = startTime;
    m_endTime = endTime;
}

void CommunicateLogQueryTask::setSortOrder(LogDB::SortOrder sortOrder)
{
    m_sortOrder = sortOrder;
}

void CommunicateLogQueryTask::executeQuery()
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
    //     （不能钳制为单点 dbEarliest 或 dbLatest，否则会误命中边界记录）
    //   - 数据库为空 → 不钳制，原样下发
    QString dbEarliest;
    QString dbLatest;
    m_db->queryTimeBounds(dbEarliest, dbLatest);
    if (!dbEarliest.isEmpty() && !dbLatest.isEmpty()) {
        // 把空字符串视为 [-∞, +∞]，便于判断重叠
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
            qDebug() << "[CommunicateLogQueryTask] 钳制后时间范围:"
                     << m_startTime << "->" << m_endTime
                     << " (DB:" << dbEarliest << "->" << dbLatest << ")";
        } else {
            qDebug() << "[CommunicateLogQueryTask] 请求窗口与 DB 范围无重叠，不钳制:"
                     << m_startTime << "->" << m_endTime
                     << " (DB:" << dbEarliest << "->" << dbLatest << ")";
        }
    }

    // 有条件查询：当前页中所有满足条件的记录
    if (m_pageNumber > 0) {
        QList<QVariantMap> pageRecords = m_db->queryPageWithConditions(
            m_commandId, m_qrCode, m_execStatus, m_retryCount,
            m_startTime, m_endTime,
            m_pageSize, m_pageNumber,
            m_sortOrder);
        emit pageWithConditionsResult(pageRecords);
    }

    // 有条件查询：总记录数
    int totalCountWithConditions = m_db->queryTotalCountWithConditions(
        m_commandId, m_qrCode, m_execStatus, m_retryCount,
        m_startTime, m_endTime);
    emit totalCountWithConditionsResult(totalCountWithConditions);

    setState(Finished);
    emit finished(true, "Query completed successfully");
}
