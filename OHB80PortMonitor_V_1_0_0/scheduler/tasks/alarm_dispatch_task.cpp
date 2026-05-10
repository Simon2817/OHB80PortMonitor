#include "alarm_dispatch_task.h"

#include <QDateTime>
#include <QDebug>

#include "logdatabases/databasemanager.h"
#include "logdatabases/alarmlogdb/alarmlogdbcon.h"
#include "scheduler/tasks/operation_dispatch_task.h"
#include "app/shareddata.h"

AlarmDispatchTask::AlarmDispatchTask(QObject* parent)
    : SchedulerTask(parent)
{
    qDebug() << "[AlarmDispatchTask] constructed";
}

void AlarmDispatchTask::start()
{
    setState(Running);
    loadActiveFromDb();

    // 连接 DB 写入完成信号，用于在解决落库后 emit alarmResolvePersisted
    auto* db = LogDB::DatabaseManager::instance().alarmLogCon();
    if (db) {
        connect(db, &LogDB::AlarmLogDBCon::recordResolved,
                this, &AlarmDispatchTask::onAlarmDBRecordResolved,
                Qt::QueuedConnection);
    }

    qDebug() << "[AlarmDispatchTask] started, active=" << activeCount();
    emit progress(0, QStringLiteral("Alarm dispatcher running, active=%1").arg(activeCount()));
}

void AlarmDispatchTask::stop()
{
    clearActive();
    setState(Cancelled);
    emit finished(false, QStringLiteral("Alarm dispatcher stopped"));
    qDebug() << "[AlarmDispatchTask] stopped";
}

// =====================================================================
// 默认值补齐
// =====================================================================
void AlarmDispatchTask::normalize(AlarmInfo& info) const
{
    if (info.record.alarmLevel == 0) {
        info.record.alarmLevel = alarmTypeToLevel(info.record.alarmType);
    }
    if (info.record.occurTime.isEmpty()) {
        info.record.occurTime = QDateTime::currentDateTime()
                             .toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    }
    if (info.alarmId.isEmpty()) {
        info.alarmId = info.generateAlarmId();
    }
}

// =====================================================================
// 提交：简化入参
// =====================================================================
QString AlarmDispatchTask::submitAlarm(int alarmType,
                                       int alarmSource,
                                       const QString& sourceIdentifier,
                                       const QString& description)
{
    AlarmInfo info;
    info.record.alarmType   = alarmType;
    info.alarmSource        = alarmSource;
    info.record.qrCode      = sourceIdentifier;
    info.record.description = description;
    info.record.alarmLevel  = alarmTypeToLevel(alarmType);
    return submitAlarm(info);
}

// =====================================================================
// 提交：完整 AlarmInfo
// =====================================================================
QString AlarmDispatchTask::submitAlarm(AlarmInfo info)
{
    normalize(info);

    // NoNeed 类型（如所有 SH85 自检报警）：直接落库，不参与活跃跟踪与去重
    const int resolvedStatus = alarmTypeToResolvedStatus(info.record.alarmType);
    if (resolvedStatus == static_cast<int>(AlarmResolvedStatus::NoNeed)) {
        info.record.isResolved  = resolvedStatus;
        info.record.resolveTime.clear();
        persistInsert(info);
        emit alarmPublished(info);
        return info.alarmId;
    }

    {
        QMutexLocker locker(&m_mutex);
        // 去重：同一 alarmId 已活跃则忽略
        if (m_active.contains(info.alarmId)) {
            return info.alarmId;
        }
        info.record.isResolved = 0;
        info.record.resolveTime.clear();
        m_active.insert(info.alarmId, info);
    }

    // 持久化：写 alarm_log（DBCon 内部 QueuedConnection 异步落盘）
    persistInsert(info);

    // 记录运行日志：警报提交
    if (auto* opTask = SharedData::getOperationDispatchTask()) {
        opTask->logMessage(info.record.description);
    }

    // 派发给订阅者（live log / 业务回调）
    emit alarmPublished(info);
    return info.alarmId;
}

// =====================================================================
// 解决：按 alarmId 字符串
// =====================================================================
void AlarmDispatchTask::submitResolve(const QString& alarmId)
{
    AlarmInfo resolvedInfo;
    {
        QMutexLocker locker(&m_mutex);
        auto it = m_active.find(alarmId);
        if (it == m_active.end()) {
            return;
        }
        resolvedInfo = it.value();
        resolvedInfo.record.isResolved   = 1;
        resolvedInfo.record.resolveTime = QDateTime::currentDateTime()
                                        .toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        m_active.erase(it);
    }

    persistResolve(resolvedInfo);

    // 记录运行日志：警报解决
    if (auto* opTask = SharedData::getOperationDispatchTask()) {
        opTask->logMessage(QString("[AlarmResolved] %1").arg(resolvedInfo.record.description));
    }

    emit alarmResolved(resolvedInfo);
}

// =====================================================================
// 解决：按 type/source/identifier 还原 alarmId
// =====================================================================
void AlarmDispatchTask::submitResolve(int alarmType,
                                     int alarmSource,
                                     const QString& sourceIdentifier)
{
    AlarmInfo probe;
    probe.record.alarmType   = alarmType;
    probe.alarmSource        = alarmSource;
    probe.record.qrCode      = sourceIdentifier;
    probe.record.alarmLevel  = alarmTypeToLevel(alarmType);
    submitResolve(probe.generateAlarmId());
}

// =====================================================================
// 查询接口
// =====================================================================
bool AlarmDispatchTask::isActive(const QString& alarmId) const
{
    QMutexLocker locker(&m_mutex);
    return m_active.contains(alarmId);
}

int AlarmDispatchTask::activeCount() const
{
    QMutexLocker locker(&m_mutex);
    return m_active.size();
}

QList<AlarmInfo> AlarmDispatchTask::activeAlarms() const
{
    QMutexLocker locker(&m_mutex);
    return m_active.values();
}

void AlarmDispatchTask::clearActive()
{
    QMutexLocker locker(&m_mutex);
    m_active.clear();
}

// =====================================================================
// 启动加载：从 alarm_log 恢复 is_resolved=0 的警报到 m_active
// =====================================================================
void AlarmDispatchTask::loadActiveFromDb()
{
    auto* db = LogDB::DatabaseManager::instance().alarmLogCon();
    if (!db) {
        qWarning() << "[AlarmDispatchTask] AlarmLogDBCon unavailable, skip restore";
        return;
    }

    // 全量取未解决记录（alarmLevel=-1 / qrCode="" / alarmType="" 表示不限）
    constexpr int kPageSize = 10000;
    const QList<AlarmRecord> rows = db->queryPageWithConditions(
        /*alarmLevel*/ -1,
        /*qrCode*/ QString(),
        /*alarmType*/ QString(),
        /*isResolved*/ 0,
        /*startTime*/ QString(),
        /*endTime*/ QString(),
        /*pageSize*/ kPageSize,
        /*pageNumber*/ 1);

    QMutexLocker locker(&m_mutex);
    int restored = 0;
    for (const AlarmRecord& row : rows) {
        AlarmInfo info;
        info.record.id              = row.id;
        info.record.alarmLevel      = row.alarmLevel;
        info.record.occurTime       = row.occurTime;
        info.record.qrCode          = row.qrCode;
        info.record.alarmType       = row.alarmType;
        info.record.isResolved      = 0;
        info.record.resolveTime     = row.resolveTime;
        info.record.description     = row.description;
        info.record.userPermission  = row.userPermission;
        // alarm_source 未落库；generateAlarmId() 仅需 level/identifier/type，
        // 默认 Device 不影响 alarmId 与后续去重
        info.alarmSource      = static_cast<int>(AlarmSource::Device);
        info.alarmId          = info.generateAlarmId();

        if (!m_active.contains(info.alarmId)) {
            m_active.insert(info.alarmId, info);
            ++restored;

            // 发出信号让 ScrollingTipLabel 等订阅者显示恢复的警报
            // 必须在锁外 emit，避免死锁（emit 可能触发回调访问 m_active）
            locker.unlock();
            emit alarmPublished(info);
            // 记录运行日志：警报恢复
            if (auto* opTask = SharedData::getOperationDispatchTask()) {
                opTask->logError(info.record.description);
            }
            locker.relock();
        }
    }
    qDebug() << "[AlarmDispatchTask] restored" << restored
             << "unresolved alarms from alarm_log";
}

// =====================================================================
// 持久化：INSERT
// =====================================================================
void AlarmDispatchTask::persistInsert(const AlarmInfo& info)
{
    auto* db = LogDB::DatabaseManager::instance().alarmLogCon();
    if (!db) {
        qWarning() << "[AlarmDispatchTask] AlarmLogDBCon unavailable, drop:"
                   << info.alarmId;
        return;
    }

    db->insertRecord(
        info.record.alarmLevel,
        info.record.occurTime,
        info.record.qrCode,                          // 复用 qr_code 列存设备来源
        QString::number(info.record.alarmType),      // alarm_type 列为 TEXT
        info.record.isResolved,
        info.record.resolveTime,
        info.record.description,
        info.record.userPermission);

    // 发出插入完成信号，供 UI 接收显示
    emit alarmLogInserted(info.record);
}

// =====================================================================
// 持久化：UPDATE（按 alarm_type 把同类型未解决行原位标为已解决）
// =====================================================================
void AlarmDispatchTask::persistResolve(const AlarmInfo& info)
{
    auto* db = LogDB::DatabaseManager::instance().alarmLogCon();
    if (!db) {
        qWarning() << "[AlarmDispatchTask] AlarmLogDBCon unavailable, skip resolve:"
                   << info.alarmId;
        return;
    }
    // updateResolve 按 (qr_code, alarm_type) 联合定位单条
    db->updateResolve(info.record.qrCode,
                      QString::number(info.record.alarmType),
                      info.record.resolveTime);
}

// =====================================================================
// DB 写入完成回调：查询完整记录并 emit alarmResolvePersisted
// =====================================================================
void AlarmDispatchTask::onAlarmDBRecordResolved(const QString& qrCode, const QString& alarmType, const QString& resolveTime)
{
    auto* db = LogDB::DatabaseManager::instance().alarmLogCon();
    if (!db) return;

    // 查询该 (qrCode, alarmType, isResolved=1, resolveTime) 的完整记录
    const QList<AlarmRecord> records = db->queryPageWithConditions(
        /*alarmLevel*/ -1,
        /*qrCode*/ qrCode,
        /*alarmType*/ alarmType,
        /*isResolved*/ 1,
        /*startTime*/ QString(),
        /*endTime*/ QString(),
        /*pageSize*/ 1,
        /*pageNumber*/ 1);

    if (records.isEmpty()) {
        qWarning() << "[AlarmDispatchTask] resolve record not found after DB write:"
                   << qrCode << alarmType << resolveTime;
        return;
    }

    // 取最新的一条（按 resolveTime 匹配）
    for (const AlarmRecord& rec : records) {
        if (rec.resolveTime == resolveTime) {
            emit alarmResolvePersisted(rec);
            return;
        }
    }
    qWarning() << "[AlarmDispatchTask] resolve record with matching resolveTime not found:"
               << qrCode << alarmType << resolveTime;
}
