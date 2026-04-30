#ifndef SH85_SELF_CHECK_TASK_H
#define SH85_SELF_CHECK_TASK_H

#include "../scheduler_task.h"
#include "modbustcpmastermanager/modbuscommand/modbuscommand.h"

#include <QElapsedTimer>
#include <QHash>
#include <QList>
#include <QMetaType>
#include <QString>
#include <QTimer>

// ====================================================================
// SH85SelfCheckTask — SH85 湿度传感器自检调度任务
//
//   流程概要（详见 docs/SH85_SELF_CHECK.md）：
//     1. 下发 StartSelfCheck（不允许超时重发）
//     2. 启动 70s 整体计时器，每 1s 通过 countdownTick 反馈剩余秒数
//     3. elapsed >= 5s 阶段 1：下发 ReadSelfCheckStatus，必须返回 value==1
//     4. elapsed >= 60s 阶段 2：循环下发 ReadSelfCheckStatus，
//        UI 状态文本 "Checking (N)"（10s 倒计时）
//        - value==2          → 自检成功
//        - value==3/4/5      → 各类失败原因
//        - value==1          → 立即重发（直到 70s 兜底）
//        - timedOut          → Network Error
//     5. 70s 仍为 value==1 → 自检功能异常 SelfCheckFunctionAbnormal
//
//   信号：
//     countdownTick   — 整体倒计时（剩余秒数 70 → 0），UI 按钮文案
//     statusChanged   — 状态文本（"Checking (N)" / "Network Error" / 终态文案）
//     allFinished     — 任务结束（携带成功标志和具体 Result）
// ====================================================================
class SH85SelfCheckTask : public SchedulerTask
{
    Q_OBJECT

public:
    // 自检最终结果
    enum class Result {
        Success,                    // value == 2，自检成功
        NetworkError,               // 任意阶段指令超时
        DeviceNotEntered,           // 阶段 1 value != 1，设备未进入自检
        HumidityExceeded,           // value == 3
        SensorCommError,            // value == 4
        ThresholdParamError,        // value == 5
        SelfCheckFunctionAbnormal,  // 70s 兜底仍为 value == 1
        Cancelled                   // 外部 stop()
    };
    Q_ENUM(Result)

    explicit SH85SelfCheckTask(const QString &qrcode, QObject *parent = nullptr);
    ~SH85SelfCheckTask();

    // SchedulerTask 接口
    void start() override;
    void stop()  override;
    QString taskType() const override { return "SH85SelfCheckTask"; }

    // ---- 时间常量 ----
    static constexpr int kTotalDurationMs = 70000; // 整体兜底
    static constexpr int kPhase1ProbeMs   = 5000;  // 阶段 1 触发时刻
    static constexpr int kPhase2StartMs   = 60000; // 阶段 2 起点
    static constexpr int kCountdownTickMs = 1000;  // 1Hz tick

signals:
    // 整体倒计时（剩余秒数：70 → 0），驱动 UI 按钮文案
    void countdownTick(int remainingSeconds, const QString &qrcode);

    // 阶段性状态文本（"Checking (N)" / "Network Error" / 终态文案）
    void statusChanged(const QString &text, const QString &qrcode);

    // 任务结束
    void allFinished(bool success,
                     SH85SelfCheckTask::Result result,
                     const QString &qrcode);

private slots:
    void onCommandFinished(ModbusCommand cmd, const QString &masterId);
    void onTick();

private:
    // 内部阶段
    enum class Phase {
        Idle,
        StartingSelfCheck,    // 等 StartSelfCheck 响应
        WaitingPhase1,        // 0 ~ 5s
        ReadingStatusEarly,   // 等阶段 1 ReadSelfCheckStatus 响应
        WaitingPhase2,        // 5s ~ 60s
        PollingStatus,        // 60s 起循环读取，等响应中
        Done
    };

    // 待响应指令分类（用于在 onCommandFinished 区分回调归属）
    enum class CmdKind { StartSelfCheck, ReadStatusEarly, ReadStatusPoll };

    // ---- 私有方法 ----
    bool submitStartSelfCheck();
    bool submitReadStatus(CmdKind kind);
    bool ensureMasterAndConnect();        // 校验 master / 连接 / 建立信号连接
    quint16 parseStatusValue(const ModbusCommand &cmd) const;
    int  remainingSecondsTotal()  const;  // 70 - elapsed（秒，floor，>=0）
    int  remainingSecondsPhase2() const;  // 70 - elapsed（仅阶段 2 用）

    void disconnectAll();
    void finishWith(bool success, Result result, const QString &uiText);

    static QString resultToText(Result r);

    // ---- 成员 ----
    QString  m_qrcode;
    Phase    m_phase = Phase::Idle;

    QElapsedTimer m_elapsed;
    QTimer       *m_tickTimer = nullptr;

    // 已发送但等待响应的指令：uuid → 分类
    QHash<qint64, CmdKind>         m_pendingMap;
    QList<QMetaObject::Connection> m_connections;

    // 标志位
    bool m_stopped         = false;
    bool m_finishedEmitted = false;

    // 阶段切换标记，避免 onTick 重复触发
    bool m_phase1Triggered = false;   // 5s 阶段是否已下发
    bool m_phase2Triggered = false;   // 60s 阶段是否已下发
    int  m_lastPhase2Sec   = -1;      // 上一次发出的阶段 2 倒计时秒数（去重）
};

// 跨线程信号（QueuedConnection）需要将枚举注册为 Qt 元类型
Q_DECLARE_METATYPE(SH85SelfCheckTask::Result)

#endif // SH85_SELF_CHECK_TASK_H
