#include "operationlogquerytask.h"
#include "databasemanager.h"
#include <QDebug>

OperationLogQueryTask::OperationLogQueryTask(QObject *parent)
    : SchedulerTask{parent}
    , m_db(nullptr)
    , m_logType(-1)
    , m_pageSize(500)
    , m_targetPage(0)
    , m_cancelRequested(0)
{
}

void OperationLogQueryTask::setTimeRange(const QString& startTime, const QString& endTime)
{
    m_startTime = startTime;
    m_endTime   = endTime;
}

void OperationLogQueryTask::setLogType(int logType)
{
    m_logType = logType;
}

void OperationLogQueryTask::setSearchKey(const QString& keyword)
{
    m_keyword = keyword;
}

void OperationLogQueryTask::setPageSize(int pageSize)
{
    m_pageSize = pageSize;
}

void OperationLogQueryTask::setTargetPage(int page)
{
    m_targetPage = page;
}

void OperationLogQueryTask::start()
{
    m_db = LogDB::DatabaseManager::instance().operationLogCon();
    if (!m_db) {
        setState(Failed);
        emit finished(false, "OperationLogDBCon unavailable");
        return;
    }

    setState(Running);
    executeQuery();
}

void OperationLogQueryTask::stop()
{
    // 仅设置取消标志，executeQuery 在各子查询之间检查
    m_cancelRequested.storeRelaxed(1);
    setState(Cancelled);
}

void OperationLogQueryTask::executeQuery()
{
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
            qDebug() << "[OperationLogQueryTask] 钳制后时间范围:"
                     << m_startTime << "->" << m_endTime
                     << " (DB:" << dbEarliest << "->" << dbLatest << ")";
        } else {
            qDebug() << "[OperationLogQueryTask] 请求窗口与 DB 范围无重叠，不钳制:"
                     << m_startTime << "->" << m_endTime
                     << " (DB:" << dbEarliest << "->" << dbLatest << ")";
        }
    }

    const bool hasMatchConditions = (m_logType != -1 || !m_keyword.isEmpty());

    // 1. 确定本次查询使用的页号：
    //    用户指定 (>0) → 直接使用；否则若有匹配条件则自动定位首条命中所在页；
    //    否则默认第 1 页。
    int targetPage = m_targetPage;
    if (targetPage <= 0) {
        if (hasMatchConditions) {
            // 两步法：先找范围+条件下首条命中 id，再算它在范围内的真实页号
            const int firstId = m_db->queryFirstMatchedId(
                m_startTime, m_endTime, m_logType, m_keyword);
            if (firstId > 0) {
                targetPage = m_db->queryRecordPageInRange(
                    firstId, m_startTime, m_endTime, m_pageSize);
            }
        }
        if (targetPage <= 0) {
            targetPage = 1;
        }
    }
    emit targetPageResult(targetPage);
    if (isCancelled()) { emit finished(false, "Cancelled"); return; }

    // 2. 范围内分页：该页全部记录（用于显示）
    QList<QVariantMap> currentPageRecords =
        m_db->queryPaginationInRange(m_startTime, m_endTime, m_pageSize, targetPage);
    emit currentPageResult(currentPageRecords);
    if (isCancelled()) { emit finished(false, "Cancelled"); return; }

    // 3. 该页中满足匹配条件的记录 id 集合（用于高亮 / Pre/Next 页内跳转）
    QList<int> matchedIds;
    if (hasMatchConditions) {
        // queryPageWithConditions 在子查询里 LIMIT/OFFSET 全表，对"范围分页"
        // 不能直接套用——这里改为对当前页记录在内存里按条件过滤。
        const int logType = m_logType;
        const QString kw = m_keyword.toLower();
        for (const QVariantMap& rec : currentPageRecords) {
            const int recLogType = rec.value("log_type").toInt();
            const QString desc = rec.value("description").toString();
            const bool typeOK = (logType == -1) || (recLogType == logType);
            const bool kwOK   = kw.isEmpty() || desc.toLower().contains(kw);
            if (typeOK && kwOK) {
                matchedIds.append(rec.value("id").toInt());
            }
        }
    }
    emit matchedIdsOnPageResult(matchedIds);
    if (isCancelled()) { emit finished(false, "Cancelled"); return; }

    // 4. 范围内总记录数（用于分页总页数）
    int totalCountInRange = m_db->queryTotalCountInRange(m_startTime, m_endTime);
    emit totalCountInRangeResult(totalCountInRange);
    if (isCancelled()) { emit finished(false, "Cancelled"); return; }

    // 5. 范围 + 条件总命中数（用于 X/Y 显示）
    int totalMatched = 0;
    if (hasMatchConditions) {
        totalMatched = m_db->queryTotalCountWithConditions(
            m_startTime, m_endTime, m_logType, m_keyword);
    }
    emit totalMatchedCountResult(totalMatched);
    if (isCancelled()) { emit finished(false, "Cancelled"); return; }

    // 6. 该页首条命中记录在范围+条件结果集中的全局顺序号（1-based；0 表示无命中）
    int position = 0;
    if (hasMatchConditions && !matchedIds.isEmpty()) {
        const int firstId = matchedIds.first();
        if (firstId > 0) {
            position = m_db->queryRecordPosition(
                firstId, m_startTime, m_endTime, m_logType, m_keyword);
        }
    }
    emit firstMatchedPositionResult(position);
    if (isCancelled()) { emit finished(false, "Cancelled"); return; }

    setState(Finished);
    emit finished(true, "Query completed successfully");
}
