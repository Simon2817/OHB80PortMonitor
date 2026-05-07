#ifndef TIP_LABEL_TASK_H
#define TIP_LABEL_TASK_H

#include "../scheduler_task.h"

#include <QMutex>
#include <QQueue>
#include <QStringList>

// ====================================================================
// TipLabelTask —— 滚动公告栏数据采集常驻任务
//
// 职责：
//   1. 业务侧（任意线程）通过 SharedData::getTipLabelTask() 拿到本任务，
//      调用 submitAlarmLog / submitAlarmResolved / submitOperationLog 提交日志。
//   2. 内部使用 QQueue<Record> 缓存提交的记录，以生产者-消费者方式消费。
//      生产者：任意线程调用 submit* 方法入队，QMutex 保护。
//      消费者：processQueue() 槽运行在调度器线程，通过 Qt::QueuedConnection 触发。
//   3. 消费后通过信号通知 ScrollingTipLabel 控件更新显示。
//
// 用法：
//   // 提交警报日志
//   SharedData::getTipLabelTask()->submitAlarmLog(operationLog, alarmRecordId);
//
//   // 提交警报已解决
//   SharedData::getTipLabelTask()->submitAlarmResolved(alarmRecordId);
//
//   // 提交操作日志
//   SharedData::getTipLabelTask()->submitOperationLog(operationLog);
//
//   // UI 端在 UIDemo6::connectTipLabelTask() 中订阅（自动调用）：
//   connect(SharedData::getTipLabelTask(), &TipLabelTask::alarmLogReady,
//           ui->scrollingTipLabel, &ScrollingTipLabel::submitAlarmLog);
//   connect(SharedData::getTipLabelTask(), &TipLabelTask::alarmResolvedReady,
//           ui->scrollingTipLabel, &ScrollingTipLabel::submitAlarmResolved);
//   connect(SharedData::getTipLabelTask(), &TipLabelTask::operationLogReady,
//           ui->scrollingTipLabel, &ScrollingTipLabel::submitOperationLog);
// ====================================================================
class TipLabelTask : public SchedulerTask
{
    Q_OBJECT

public:
    explicit TipLabelTask(QObject* parent = nullptr);
    ~TipLabelTask() override = default;

    // SchedulerTask 接口 —— 常驻、无周期
    Q_INVOKABLE void start() override;
    Q_INVOKABLE void stop() override;
    QString taskType() const override { return QStringLiteral("TipLabelTask"); }
    bool isPersistent() const override { return true; }

    // 提交警报日志（线程安全）
    // operationLog: OperationLogDBCon 记录（QStringList）
    // alarmRecordId: AlarmLogDBCon 的警报记录的主键 id
    void submitAlarmLog(const QStringList& operationLog, int alarmRecordId);

    // 提交警报已解决（线程安全）
    // alarmRecordId: AlarmLogDBCon 的警报记录的主键 id
    void submitAlarmResolved(int alarmRecordId);

    // 提交操作日志（线程安全）
    // operationLog: OperationLogDBCon 记录（QStringList）
    void submitOperationLog(const QStringList& operationLog);

signals:
    // 派发给 ScrollingTipLabel::submitAlarmLog
    void alarmLogReady(const QStringList& operationLog, int alarmRecordId);

    // 派发给 ScrollingTipLabel::submitAlarmResolved
    void alarmResolvedReady(int alarmRecordId);

    // 派发给 ScrollingTipLabel::submitOperationLog
    void operationLogReady(const QStringList& operationLog);

private slots:
    void processQueue();

private:
    struct Record {
        enum Type { AlarmLog, AlarmResolved, OperationLog };
        Type        type;
        QStringList operationLog;
        int         alarmRecordId = 0;
    };

    QQueue<Record>  m_queue;
    mutable QMutex  m_mutex;
    bool            m_stopped = false;
};

#endif // TIP_LABEL_TASK_H
