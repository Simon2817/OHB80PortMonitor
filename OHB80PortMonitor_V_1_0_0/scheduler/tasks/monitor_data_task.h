#ifndef MONITOR_DATA_TASK_H
#define MONITOR_DATA_TASK_H

#include "../scheduler_task.h"
#include "modbustcpmastermanager/modbuscommand/modbuscommand.h"

#include <QHash>
#include <QString>
#include <QVariantMap>

class MonitorDataTask : public SchedulerTask
{
    Q_OBJECT

public:
    explicit MonitorDataTask(QObject *parent = nullptr);

    Q_INVOKABLE void start() override;
    Q_INVOKABLE void stop() override;
    QString taskType() const override { return "MonitorDataTask"; }
    bool isPersistent() const override { return true; }

private slots:
    // 接收 PeriodicCommandSender 的 commandCompleted 信号
    // masterId 参数通过 lambda 捕获传入，对应 FoupOfOHBInfo 的 qrCode
    void onCommandCompleted(ModbusCommand cmd, const QString& masterId);

private:
    // 解析 ReadFoupStatus 响应帧（18 字节，9 个寄存器）
    QVariantMap parseReadFoupStatusResponse(const ModbusCommand& cmd) const;

    // 将解析后的数据写入共享的 FoupOfOHBInfo
    void updateFoupInfo(const QString& masterId, const QVariantMap& data);

    int m_totalCount = 0;
    bool m_stopped = false;
};

#endif // MONITOR_DATA_TASK_H
