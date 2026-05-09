#ifndef SH85_PERIODIC_SELF_CHECK_TASK_H
#define SH85_PERIODIC_SELF_CHECK_TASK_H

#include "../scheduler_task.h"
#include "modbustcpmastermanager/modbustcpmaster/sh85selfchecker.h"

#include <QDateTime>
#include <QList>
#include <QMetaType>
#include <QString>
#include <QStringList>

class QTimer;
class ModbusTcpMaster;

// ====================================================================
// SH85PeriodicSelfCheckTask — SH85 定期自检调度任务（普通任务）
//
//   设计说明：
//     - 由 UI（SH85PeriodicSelfCheckSettingWidget）创建并持有指针；
//     - 通过 Scheduler::submitTask 提交后由调度器拥有生命周期；
//     - 任务运行后周期性触发：每隔 m_intervalSec 秒，依次对所有
//       qrcode 调用 master->selfChecker() 执行自检；
//     - 一轮自检结束后进入空闲倒计时，到期再开始下一轮；
//     - UI 调用 stop() 后任务结束并被调度器清理。
//
//   信号：
//     countdownTick     — 距离下一次自检开始的剩余秒数（间隔倒计时）
//     statusChanged     — 状态文本（Idle / Checking）
//     progressUpdate    — 一轮自检进度（current, total）
//     roundFinished     — 一轮结束（成功数 / 失败数 / 起止时间）
// ====================================================================
class SH85PeriodicSelfCheckTask : public SchedulerTask
{
    Q_OBJECT

public:
    using Result = SH85SelfChecker::Result;

    enum class Status {
        Idle,       // 空闲倒计时中
        Checking    // 一轮自检进行中
    };
    Q_ENUM(Status)

    explicit SH85PeriodicSelfCheckTask(QObject *parent = nullptr);
    ~SH85PeriodicSelfCheckTask() override;

    // 配置间隔时长（秒）；可在 start 前或运行中修改
    void setIntervalSeconds(int seconds);
    int  intervalSeconds() const { return m_intervalSec; }

    // SchedulerTask 接口
    void start() override;
    void stop()  override;
    QString taskType() const override { return "SH85PeriodicSelfCheckTask"; }

signals:
    // 距离下一次自检的剩余秒数（间隔倒计时；Checking 期间不发出）
    void countdownTick(int remainingSeconds);

    // 当前状态：Idle / Checking
    void statusChanged(SH85PeriodicSelfCheckTask::Status status);

    // 一轮中的进度
    void progressUpdate(int current, int total);

    // 一轮结束统计
    void roundFinished(int successCount,
                       int failureCount,
                       const QString& startTime,
                       const QString& endTime);

private slots:
    void onIdleTick();                                // 1Hz 间隔倒计时
    void onCheckerFinished(bool success,
                           SH85SelfChecker::Result result,
                           const QString& message,
                           const QString& masterId);

private:
    // 一轮控制
    void beginRound();                                // 准备 qrcode 列表，发起第一台
    void startNextDevice();                           // 启动下一台设备的 checker
    void finishCurrentDevice(bool success);           // 统计当前设备结果
    void endRound();                                  // 一轮结束 → 进入下一次倒计时

    // 启动间隔倒计时
    void startIdleCountdown();
    void stopAllTimers();
    void disconnectChecker();

    // 时间戳辅助
    static QString currentTimestamp();

private:
    // 配置
    int                m_intervalSec = 5 * 60;        // 默认 5 分钟

    // 运行状态
    Status             m_status      = Status::Idle;
    int                m_remainingSec = 0;            // 当前 Idle 剩余秒数
    bool               m_stopRequested = false;
    bool               m_finishedEmitted = false;

    // 倒计时定时器（在调度器线程上运行）
    QTimer            *m_idleTimer   = nullptr;

    // 一轮内的设备列表与进度
    QStringList        m_roundQrcodes;
    int                m_roundIndex  = 0;
    int                m_successCount = 0;
    int                m_failureCount = 0;
    QString            m_roundStartTime;

    // 当前正在自检的设备
    ModbusTcpMaster   *m_currentMaster  = nullptr;
    SH85SelfChecker   *m_currentChecker = nullptr;
    QString            m_currentQrcode;
    QMetaObject::Connection m_checkerFinishedConn;
};

Q_DECLARE_METATYPE(SH85PeriodicSelfCheckTask::Status)

#endif // SH85_PERIODIC_SELF_CHECK_TASK_H
