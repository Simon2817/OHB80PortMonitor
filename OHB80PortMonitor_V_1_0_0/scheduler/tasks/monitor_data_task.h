#ifndef MONITOR_DATA_TASK_H
#define MONITOR_DATA_TASK_H

#include "../scheduler_task.h"
#include "modbustcpmastermanager/modbuscommand/modbuscommand.h"
#include "modbustcpmastermanager/modbuscommand/commandresponseparser.h"

#include <QHash>
#include <QString>
#include <QVariantMap>

class CommunicationRecorder;

class MonitorDataTask : public SchedulerTask
{
    Q_OBJECT

public:
    explicit MonitorDataTask(QObject *parent = nullptr);
    ~MonitorDataTask();

    Q_INVOKABLE void start() override;
    Q_INVOKABLE void stop() override;
    QString taskType() const override { return "MonitorDataTask"; }
    bool isPersistent() const override { return true; }

signals:
    /**
     * @brief 通讯完成信号（每条指令完成后发射，包含成功/失败/超时）
     * @param cmd 指令对象（含请求帧、响应帧、发送/响应时间等）
     * @param masterId 对应的 Master ID（qrCode）
     * @param description 描述字段（失败时为错误信息，成功时为解析后的字段值）
     */
    void communicationCompleted(ModbusCommand cmd, QString masterId, QString description);

private slots:
    // 接收 CommunicationRecorder 的 shouldEmit 信号，写入数据库
    void onCommunicationRecorded(ModbusCommand cmd, const QString& masterId);

    // 接收 PeriodicCommandSender 的 commandCompleted 信号
    // masterId 参数通过 lambda 捕获传入，对应 FoupOfOHBInfo 的 qrCode
    void onCommandCompleted(ModbusCommand cmd, const QString& masterId);

private:
    // 使用解析表分发解析，并将结果写入对应 FoupOfOHBInfo
    void updateFoupInfo(const QString& masterId, const QString& commandId, const QVariantMap& data);

    int m_totalCount = 0;
    bool m_stopped = false;

    // 通讯记录采集器（节流 UI 通讯日志的上报频率）
    CommunicationRecorder* m_recorder = nullptr;
};

#endif // MONITOR_DATA_TASK_H
