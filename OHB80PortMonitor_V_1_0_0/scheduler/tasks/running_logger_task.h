#ifndef RUNNING_LOGGER_TASK_H
#define RUNNING_LOGGER_TASK_H

#include "../scheduler_task.h"

#include <QString>

// ====================================================================
// RunningLoggerTask —— 运行日志采集常驻任务
//
// 取代旧 RunningLoggerCollector + RunningLoggerWidget 的组合。
//
// 职责：
//   1. 业务侧（任意线程）通过 SharedData::getRunningLoggerTask() 拿到本任务，
//      调用 logMessage / logWarn / logError / log 提交一条运行日志。
//   2. 直接落入 LogDB::OperationLogDBCon（SQLite）。OperationLogDBCon 内部已用
//      QueuedConnection 异步派发，本类无需缓冲队列。
//
// UI 实时显示：
//   订阅 LogDB::OperationLogDBCon::recordInserted 信号即可。
// ====================================================================
class RunningLoggerTask : public SchedulerTask
{
    Q_OBJECT

public:
    enum class MsgType {
        Message = 0,    // 普通运行信息（与 LogDB::OperationLogType::Message 对齐）
        Warn    = 1,    // 警告（与 LogDB::OperationLogType::Warn 对齐）
        Error   = 2     // 错误（与 LogDB::OperationLogType::Error 对齐）
    };

    explicit RunningLoggerTask(QObject* parent = nullptr);
    ~RunningLoggerTask() override = default;

    // SchedulerTask 接口 —— 常驻、无周期、无内部循环
    Q_INVOKABLE void start() override;
    Q_INVOKABLE void stop() override;
    QString taskType() const override { return QStringLiteral("RunningLoggerTask"); }
    bool isPersistent() const override { return true; }

    // 通用入口（线程安全）
    void log(MsgType type, const QString& message);

    // 便捷入口（线程安全）
    void logMessage(const QString& message);
    void logWarn(const QString& message);
    void logError(const QString& message);
};

#endif // RUNNING_LOGGER_TASK_H
