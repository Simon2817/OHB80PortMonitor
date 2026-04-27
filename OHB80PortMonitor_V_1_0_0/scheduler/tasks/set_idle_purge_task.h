#ifndef SET_IDLE_PURGE_TASK_H
#define SET_IDLE_PURGE_TASK_H

#include "../scheduler_task.h"
#include "modbustcpmastermanager/modbuscommand/modbuscommand.h"

#include <QAtomicInt>
#include <QHash>
#include <QList>
#include <QStringList>
#include <QTimer>

// ====================================================================
// SetIdlePurgeTask — 批量写入 Idle Purge 参数调度任务
//
//   使用方式：
//     auto *task = new SetIdlePurgeTask(IdlePurgeProperty::Enable, 1);
//     Scheduler::instance()->submitTask(task);
//     connect(task, &SetIdlePurgeTask::allFinished, ...);
//
//   信号 allFinished：
//     allSuccess      — 是否全部成功
//     successCount    — 成功设备数
//     failedQrCodes   — 失败设备的 QRCode 列表
//     propertyName    — 被设置的属性名称（用于日志）
//     setValue        — 被设置的目标值（用于日志）
// ====================================================================

class SetIdlePurgeTask : public SchedulerTask
{
    Q_OBJECT

public:
    // 可设置的 Idle Purge 属性
    enum class IdlePurgeProperty {
        Enable,       // 使能：WriteIdlePurgeEnable  (值 1=开启, 0=关闭)
        PurgeTime,    // 充气时长：WriteIdlePurgeTime  (单位：秒)
        PurgeInterval // 充气间隔：WriteIdlePurgeInterval (单位：秒)
    };

    // 构造时确定属性与目标值
    explicit SetIdlePurgeTask(IdlePurgeProperty property,
                              quint16 value,
                              QObject *parent = nullptr);
    ~SetIdlePurgeTask();

    // SchedulerTask 接口
    void start() override;
    void stop()  override;
    QString taskType() const override { return "SetIdlePurgeTask"; }

    // 属性转可读字符串（供日志 / UI 使用）
    static QString propertyToString(IdlePurgeProperty p);

signals:
    // 全部设备执行完毕信号
    void allFinished(bool allSuccess,
                     int successCount,
                     QStringList failedQrCodes,
                     QString propertyName,
                     quint16 setValue);

private slots:
    void onCommandFinished(ModbusCommand cmd, const QString &masterId);
    void onTimeout();

private:
    // 根据属性枚举返回 Modbus 指令 ID
    QString commandIdForProperty(IdlePurgeProperty p) const;

    // 构建 Modbus registerValue（大端序，2字节）
    QByteArray buildRegisterValue(quint16 value) const;

    // 断开所有信号连接
    void disconnectAll();

    // 检查是否全部完成
    void checkAllFinished();

    // 强制结束任务（超时或正常完成均走此路径，保证只触发一次）
    void forceFinish();

private:
    IdlePurgeProperty m_property;
    quint16           m_value;

    // 执行状态
    QHash<qint64, QString>         m_pendingMap;     // uuid → qrCode
    QList<QMetaObject::Connection> m_connections;
    int        m_totalCount = 0;
    QAtomicInt m_completedCount{0};
    bool       m_stopped = false;

    // 汇总结果
    int         m_successCount = 0;
    QStringList m_failedQrCodes;

    QTimer     *m_timeoutTimer = nullptr;   // 5 秒整体超时
    bool        m_allFinishedEmitted = false; // 防止 allFinished 重复发射
};

#endif // SET_IDLE_PURGE_TASK_H
