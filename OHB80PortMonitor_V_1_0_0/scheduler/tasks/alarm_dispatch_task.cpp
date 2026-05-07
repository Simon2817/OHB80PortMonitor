#include "alarm_dispatch_task.h"

#include <QDateTime>
#include <QDebug>

#include "logdatabases/databasemanager.h"
#include "logdatabases/alarmlogdb/alarmlogdbcon.h"

AlarmDispatchTask::AlarmDispatchTask(QObject* parent)
    : SchedulerTask(parent)
{
    qDebug() << "[AlarmDispatchTask] constructed";
}

void AlarmDispatchTask::start()
{
    setState(Running);
    loadActiveFromDb();
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
    if (info.alarmLevel == 0) {
        info.alarmLevel = alarmTypeToLevel(info.alarmType);
    }
    if (info.occurTime.isEmpty()) {
        info.occurTime = QDateTime::currentDateTime()
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
    info.alarmType        = alarmType;
    info.alarmSource      = alarmSource;
    info.sourceIdentifier = sourceIdentifier;
    info.description      = description;
    info.alarmLevel       = alarmTypeToLevel(alarmType);
    return submitAlarm(info);
}

// =====================================================================
// 提交：完整 AlarmInfo
// =====================================================================
QString AlarmDispatchTask::submitAlarm(AlarmInfo info)
{
    normalize(info);

    {
        QMutexLocker locker(&m_mutex);
        // 去重：同一 alarmId 已活跃则忽略
        if (m_active.contains(info.alarmId)) {
            return info.alarmId;
        }
        info.isResolved = false;
        info.resolvedTime.clear();
        m_active.insert(info.alarmId, info);
    }

    // 持久化：写 alarm_log（DBCon 内部 QueuedConnection 异步落盘）
    persistInsert(info);

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
        resolvedInfo.isResolved   = true;
        resolvedInfo.resolvedTime = QDateTime::currentDateTime()
                                        .toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        m_active.erase(it);
    }

    persistResolve(resolvedInfo);
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
    probe.alarmType        = alarmType;
    probe.alarmSource      = alarmSource;
    probe.sourceIdentifier = sourceIdentifier;
    probe.alarmLevel       = alarmTypeToLevel(alarmType);
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

    // 全量取未解决记录（alarmLevel=-1 / qrCode=\"\" / alarmType=\"\" 表示不限）
    constexpr int kPageSize = 10000;
    const QList<QVariantMap> rows = db->queryPageWithConditions(
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
    for (const QVariantMap& row : rows) {
        AlarmInfo info;
        info.id               = row.value(QStringLiteral("id")).toInt();
        info.alarmLevel       = row.value(QStringLiteral("alarm_level")).toInt();
        info.occurTime        = row.value(QStringLiteral("occur_time")).toString();
        info.sourceIdentifier = row.value(QStringLiteral("qr_code")).toString();
        info.alarmType        = row.value(QStringLiteral("alarm_type")).toInt();
        info.isResolved       = false;
        info.resolvedTime     = row.value(QStringLiteral("resolve_time")).toString();
        info.description      = row.value(QStringLiteral("description")).toString();
        // alarm_source 未落库；generateAlarmId() 仅需 level/identifier/type，
        // 默认 Device 不影响 alarmId 与后续去重
        info.alarmSource      = static_cast<int>(AlarmSource::Device);
        info.alarmId          = info.generateAlarmId();

        if (!m_active.contains(info.alarmId)) {
            m_active.insert(info.alarmId, info);
            ++restored;
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
        info.alarmLevel,
        info.occurTime,
        info.sourceIdentifier,                // 复用 qr_code 列存设备来源
        QString::number(info.alarmType),      // alarm_type 列为 TEXT
        info.isResolved ? 1 : 0,
        info.resolvedTime,
        info.description);
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
    db->updateResolve(info.sourceIdentifier,
                      QString::number(info.alarmType),
                      info.resolvedTime);
}
