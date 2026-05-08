#ifndef ALARM_DISPATCH_TASK_H
#define ALARM_DISPATCH_TASK_H

#include "../scheduler_task.h"
#include "alarminfo.h"
#include "alarmtype.h"

#include <QHash>
#include <QMutex>
#include <QString>

// ====================================================================
// AlarmDispatchTask —— 警报调度常驻任务（取代老 AlarmLogicSystem）
//
// 职责：
//   1. 业务侧（任意线程）通过 SharedData::getAlarmDispatchTask() 拿到本任务，
//      调用 submitAlarm / submitResolve 提交事件。
//   2. 内部按 AlarmInfo::alarmId 字符串去重；活跃集合放在 m_active。
//   3. 持久化：调用 LogDB::AlarmLogDBCon::insertRecord / updateResolve。
//   4. 派发：emit alarmPublished / alarmResolved 供 UI live log 等订阅。
//
// 数据载体：
//   使用 classes/alarminfo.h 中的 AlarmInfo（与 alarm_log 表字段对齐）。
//   AlarmInfo::alarmId 由 generateAlarmId() 生成，规则：
//     "level(1) + sourceIdentifier(5) + type(4)"
//
// 用法：
//   // 提交（推荐简化入参；alarmLevel 由 alarmtype.h 推导）
//   SharedData::getAlarmDispatchTask()->submitAlarm(
//       static_cast<int>(AlarmType::DeviceOffline),
//       static_cast<int>(AlarmSource::Device),
//       qrCode,
//       QStringLiteral("Device %1 connection lost").arg(qrCode));
//
//   // 解决（同样的 type/source/identifier 即可还原 alarmId）
//   SharedData::getAlarmDispatchTask()->submitResolve(
//       static_cast<int>(AlarmType::DeviceOffline),
//       static_cast<int>(AlarmSource::Device),
//       qrCode);
//
//   // UI 订阅：
//   connect(SharedData::getAlarmDispatchTask(),
//           &AlarmDispatchTask::alarmPublished,
//           this, &MyWidget::onAlarmPublished);
// ====================================================================
class AlarmDispatchTask : public SchedulerTask
{
    Q_OBJECT

public:
    explicit AlarmDispatchTask(QObject* parent = nullptr);
    ~AlarmDispatchTask() override = default;

    // SchedulerTask 接口 —— 本任务为常驻、无周期，仅做事件派发
    Q_INVOKABLE void start() override;
    Q_INVOKABLE void stop() override;
    QString taskType() const override { return QStringLiteral("AlarmDispatchTask"); }
    bool isPersistent() const override { return true; }

    // 简化提交（线程安全）：依据 alarmType 自动推导 alarmLevel，
    // occurTime 自动取当前时刻，alarmId 由 generateAlarmId 生成。
    // 返回构造完成后的 alarmId（可用于后续 submitResolve）。
    QString submitAlarm(int alarmType,
                        int alarmSource,
                        const QString& sourceIdentifier,
                        const QString& description);

    // 完整提交（线程安全）：调用方已构造好 AlarmInfo。
    // - occurTime 为空时自动取当前时刻
    // - alarmId 为空时调用 generateAlarmId
    // - alarmLevel 为 0 时按 alarmType 自动推导
    QString submitAlarm(AlarmInfo info);

    // 解决（线程安全）—— 重载 1：直接传 alarmId 字符串
    void submitResolve(const QString& alarmId);

    // 解决（线程安全）—— 重载 2：与 submitAlarm 对称，内部按相同规则生成
    // alarmId 后转发。业务侧通常无需缓存 alarmId。
    void submitResolve(int alarmType,
                       int alarmSource,
                       const QString& sourceIdentifier);

    // 查询某 alarmId 是否仍活跃（线程安全）
    bool isActive(const QString& alarmId) const;

    // 当前活跃警报数量（线程安全）
    int activeCount() const;

    // 清空活跃集合（不写库；保留接口给跨日重置等场景）
    void clearActive();

signals:
    // 新警报发布（去重通过后；emit 时已成功投递到 LogDB）
    void alarmPublished(const AlarmInfo& info);

    // 警报已解决（携带带有 resolvedTime 的完整 info）
    void alarmResolved(const AlarmInfo& info);

    // 警报日志插入完成信号（携带 AlarmRecord）
    void alarmLogInserted(const AlarmRecord& record);

private:
    // 把 AlarmInfo 写入 alarm_log（INSERT）
    void persistInsert(const AlarmInfo& info);

    // 把同 alarm_type + 未解决的行原位标记为已解决
    void persistResolve(const AlarmInfo& info);

    // 给一个未填齐的 AlarmInfo 补默认字段
    void normalize(AlarmInfo& info) const;

    // 启动时从 alarm_log 表加载所有 is_resolved=0 的记录到 m_active，
    // 让本次启动能继续监控上一次未解决的警报（避免重复 INSERT、可对其 submitResolve）
    void loadActiveFromDb();

    QHash<QString, AlarmInfo> m_active;
    mutable QMutex            m_mutex;
};

#endif // ALARM_DISPATCH_TASK_H
