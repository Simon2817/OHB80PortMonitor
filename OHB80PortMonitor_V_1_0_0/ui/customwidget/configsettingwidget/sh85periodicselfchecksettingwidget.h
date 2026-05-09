#ifndef SH85PERIODICSELFCHECKSETTINGWIDGET_H
#define SH85PERIODICSELFCHECKSETTINGWIDGET_H

#include "settingwidget.h"
#include "scheduler/tasks/sh85_periodic_self_check_task.h"

#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QPointer>

class SettingItemWidget;

// ====================================================================
// SH85PeriodicSelfCheckSettingWidget — SH85 定期自检配置控件
//
//   1. 自检配置item：
//      - SpinBox：执行时间间隔（如 5）
//      - ComboBox：执行单位（s / min / hour）
//      - PushButton：设置
//   2. 自检结果汇报item：
//      - Label：成功/失败次数
//      - Label：上一次是否自检成功（失败时显示几台设备失败）
//      - Label：上一次自检的开始时间和结束时间
//   3. 自检状态item：
//      - Label：自检状态（自检中 / 空闲状态）
//      - Label：空闲时间（倒计时，格式：00:05:00）
//   4. 自检开启item：
//      - ComboBox：是否启用（启用 / 停止）
// ====================================================================
class SH85PeriodicSelfCheckSettingWidget : public SettingWidget
{
    Q_OBJECT

public:
    explicit SH85PeriodicSelfCheckSettingWidget(QWidget *parent = nullptr);
    ~SH85PeriodicSelfCheckSettingWidget();

    // 设置控件是否可用（false 时整个控件不可用）
    void setEnabled(bool enabled);

    // 返回是否处于自检工作中（定期自检功能开启即为工作中）
    bool isRunning() const;

signals:
    void runningStateChanged(bool running);  // 自检状态变化信号

private slots:
    void onSetBtnClicked();
    void onEnableComboChanged(int index);
    void onCountdownTick(int remainingSeconds);  // 来自调度层的倒计时信号

    // 调度任务回调
    void onTaskStatusChanged(SH85PeriodicSelfCheckTask::Status status);
    void onTaskProgressUpdate(int current, int total);
    void onTaskRoundFinished(int successCount,
                             int failureCount,
                             const QString& startTime,
                             const QString& endTime);

private:
    void initUI();
    void initConfigItem();
    void initResultItem();
    void initStatusItem();
    void initEnableItem();

    void updateCountdownDisplay(int remainingSeconds);
    QString formatTime(int totalSeconds) const;

private:
    // ---- 自检配置item ----
    QSpinBox     *m_intervalSpinBox   = nullptr;
    QComboBox    *m_unitCombo         = nullptr;
    QPushButton  *m_setBtn            = nullptr;
    SettingItemWidget *m_configItem   = nullptr;

    // ---- 自检结果汇报item ----
    QLabel       *m_countLabel        = nullptr;  // 成功/失败次数
    QLabel       *m_lastResultLabel   = nullptr;  // 上一次是否成功/失败设备数
    QLabel       *m_timeRangeLabel    = nullptr;  // 上一次开始和结束时间
    SettingItemWidget *m_resultItem  = nullptr;

    // ---- 自检状态item ----
    QLabel       *m_statusLabel       = nullptr;  // 自检状态
    QLabel       *m_countdownLabel    = nullptr;  // 倒计时
    QProgressBar *m_progressBar       = nullptr;  // 进度条（格式：当前/总数）
    SettingItemWidget *m_statusItem  = nullptr;

    // ---- 自检开启item ----
    QComboBox    *m_enableCombo       = nullptr;
    SettingItemWidget *m_enableItem   = nullptr;

    // ---- 内部状态 ----
    bool         m_isEnabled          = false;    // 是否启用定期自检
    bool         m_isRunning          = false;    // 是否正在执行自检

    // ---- 统计信息 ----
    int          m_successCount       = 0;
    int          m_failureCount       = 0;
    QString      m_lastStartTime;
    QString      m_lastEndTime;
    int          m_lastFailedDeviceCount = 0;

    // ---- 调度任务（UI 持有指针；生命周期由 Scheduler 管理）----
    QPointer<SH85PeriodicSelfCheckTask> m_task;
};

#endif // SH85PERIODICSELFCHECKSETTINGWIDGET_H
