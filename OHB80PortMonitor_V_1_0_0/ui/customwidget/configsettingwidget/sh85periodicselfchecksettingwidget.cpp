#include "sh85periodicselfchecksettingwidget.h"
#include "../settingwidget/settingitemwidget.h"

#include "scheduler/scheduler.h"
#include "scheduler/tasks/sh85_periodic_self_check_task.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>

// ============================================================
// 构造 / 析构
// ============================================================

SH85PeriodicSelfCheckSettingWidget::SH85PeriodicSelfCheckSettingWidget(QWidget *parent)
    : SettingWidget(parent)
{
    setTitle("SH85 Periodic Self-check Configuration");
    initUI();
}

SH85PeriodicSelfCheckSettingWidget::~SH85PeriodicSelfCheckSettingWidget()
{
    // 控件销毁时若任务仍在运行，由调度器取消并删除
    if (m_task) {
        Scheduler::instance()->cancelTask(m_task->taskId());
        m_task = nullptr;
    }
}

// ============================================================
// UI 初始化
// ============================================================

void SH85PeriodicSelfCheckSettingWidget::initUI()
{
    initConfigItem();
    initResultItem();
    initStatusItem();
    initEnableItem();
}

void SH85PeriodicSelfCheckSettingWidget::initConfigItem()
{
    m_configItem = new SettingItemWidget(this);
    m_configItem->setTitle("Self-check Interval Configuration");
    m_configItem->setTip("Configure the interval for periodic SH85 self-check");

    // SpinBox：间隔数值
    m_intervalSpinBox = new QSpinBox(m_configItem);
    m_intervalSpinBox->setRange(1, 999);
    m_intervalSpinBox->setValue(5);  // 默认 5
    m_intervalSpinBox->setFixedWidth(80);

    // ComboBox：单位
    m_unitCombo = new QComboBox(m_configItem);
    m_unitCombo->addItem("s");
    m_unitCombo->addItem("min");
    m_unitCombo->addItem("hour");
    m_unitCombo->setCurrentIndex(1);  // 默认 min
    m_unitCombo->setFixedWidth(70);

    // PushButton：设置
    m_setBtn = new QPushButton("Set", m_configItem);
    m_setBtn->setFixedWidth(80);
    connect(m_setBtn, &QPushButton::clicked, this, &SH85PeriodicSelfCheckSettingWidget::onSetBtnClicked);

    // 直接添加各个控件到 SettingItemWidget
    m_configItem->addWidget("interval_spinbox", m_intervalSpinBox);
    m_configItem->addWidget("unit_combo", m_unitCombo);
    m_configItem->addWidget("set_btn", m_setBtn);

    addItem(m_configItem);
}

void SH85PeriodicSelfCheckSettingWidget::initResultItem()
{
    m_resultItem = new SettingItemWidget(this);
    m_resultItem->setTitle("Self-check Result Summary");
    m_resultItem->setTip("Summary of periodic self-check results");

    // Label：成功/失败次数
    m_countLabel = new QLabel("Success: 0 | Failure: 0", m_resultItem);
    m_countLabel->setStyleSheet("QLabel { color: #386487; font-weight: bold; }");

    // Label：上一次是否自检成功
    m_lastResultLabel = new QLabel("Last check: Not executed", m_resultItem);
    m_lastResultLabel->setStyleSheet("QLabel { color: #386487; }");

    // Label：上一次开始和结束时间
    m_timeRangeLabel = new QLabel("Time: -", m_resultItem);
    m_timeRangeLabel->setStyleSheet("QLabel { color: #386487; }");

    // 直接添加各个控件到 SettingItemWidget
    m_resultItem->addWidget("count_label", m_countLabel);
    m_resultItem->addWidget("last_result_label", m_lastResultLabel);
    m_resultItem->addWidget("time_range_label", m_timeRangeLabel);

    addItem(m_resultItem);
}

void SH85PeriodicSelfCheckSettingWidget::initStatusItem()
{
    m_statusItem = new SettingItemWidget(this);
    m_statusItem->setTitle("Self-check Status");
    m_statusItem->setTip("Current status and countdown to next self-check");

    // ProgressBar：进度条（格式：当前/总数）
    m_progressBar = new QProgressBar(m_statusItem);
    m_progressBar->setRange(0, 80);  // 假设总数为80
    m_progressBar->setValue(0);
    m_progressBar->setFormat("%v/%m");  // 格式：当前值/最大值
    m_progressBar->setFixedHeight(35);
    m_progressBar->setFixedWidth(250);
    m_progressBar->setTextVisible(true);

    // Label：自检状态
    m_statusLabel = new QLabel("Status: Idle", m_statusItem);
    m_statusLabel->setStyleSheet("QLabel { color: #386487; font-weight: bold; }");

    // Label：倒计时
    m_countdownLabel = new QLabel("Next check in: 00:05:00", m_statusItem);
    m_countdownLabel->setStyleSheet("QLabel { color: #386487; }");

    // 直接添加各个控件到 SettingItemWidget
    m_statusItem->addWidget("progress_bar", m_progressBar);
    m_statusItem->addWidget("status_label", m_statusLabel);
    m_statusItem->addWidget("countdown_label", m_countdownLabel);

    addItem(m_statusItem);
}

void SH85PeriodicSelfCheckSettingWidget::initEnableItem()
{
    m_enableItem = new SettingItemWidget(this);
    m_enableItem->setTitle("Enable Periodic Self-check");
    m_enableItem->setTip("Enable or disable periodic self-check functionality");

    // ComboBox：启用/停止
    m_enableCombo = new QComboBox(m_enableItem);
    m_enableCombo->addItem("Stop");
    m_enableCombo->addItem("Enable");
    m_enableCombo->setCurrentIndex(0);  // 默认 Stop
    m_enableCombo->setFixedWidth(120);
    connect(m_enableCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SH85PeriodicSelfCheckSettingWidget::onEnableComboChanged);

    m_enableItem->addWidget("enable_combo", m_enableCombo);
    addItem(m_enableItem);
}

// ============================================================
// 槽 - 按钮点击
// ============================================================

void SH85PeriodicSelfCheckSettingWidget::onSetBtnClicked()
{
    const int interval = m_intervalSpinBox->value();
    const QString unit = m_unitCombo->currentText();

    // 计算总秒数
    int totalSec = interval;
    if (unit == "min") {
        totalSec = interval * 60;
    } else if (unit == "hour") {
        totalSec = interval * 3600;
    }

    qDebug() << "[SH85PeriodicSelfCheckSettingWidget] Interval set to:"
             << interval << unit << "(" << totalSec << "seconds)";

    // 若任务已运行，实时下发新的间隔
    if (m_task) {
        QMetaObject::invokeMethod(m_task.data(), "setIntervalSeconds",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, totalSec));
    }
}

void SH85PeriodicSelfCheckSettingWidget::onEnableComboChanged(int index)
{
    const bool enable = (index == 1);  // 1 = Enable

    if (enable == m_isEnabled) {
        return;  // 状态未变化
    }

    m_isEnabled = enable;

    emit runningStateChanged(enable);

    if (enable) {
        // ---- 启用定期自检：创建任务并提交调度器 ----
        qDebug() << "[SH85PeriodicSelfCheckSettingWidget] Periodic self-check enabled";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
            QString("[ui][SH85PeriodicSelfCheckSettingWidget] Periodic self-check enabled").toStdString());

        // 计算当前 UI 配置的间隔时长
        const int interval = m_intervalSpinBox->value();
        const QString unit = m_unitCombo->currentText();
        int totalSec = interval;
        if (unit == "min")  totalSec = interval * 60;
        else if (unit == "hour") totalSec = interval * 3600;

        auto* task = new SH85PeriodicSelfCheckTask();
        task->setIntervalSeconds(totalSec);
        m_task = task;

        // 连接任务信号到 UI（QueuedConnection：跨线程）
        connect(task, &SH85PeriodicSelfCheckTask::countdownTick,
                this, &SH85PeriodicSelfCheckSettingWidget::onCountdownTick,
                Qt::QueuedConnection);
        connect(task, &SH85PeriodicSelfCheckTask::statusChanged,
                this, &SH85PeriodicSelfCheckSettingWidget::onTaskStatusChanged,
                Qt::QueuedConnection);
        connect(task, &SH85PeriodicSelfCheckTask::progressUpdate,
                this, &SH85PeriodicSelfCheckSettingWidget::onTaskProgressUpdate,
                Qt::QueuedConnection);
        connect(task, &SH85PeriodicSelfCheckTask::roundFinished,
                this, &SH85PeriodicSelfCheckSettingWidget::onTaskRoundFinished,
                Qt::QueuedConnection);

        Scheduler::instance()->submitTask(task);
    } else {
        // ---- 停止定期自检：取消任务，调度器会 stop+deleteLater ----
        qDebug() << "[SH85PeriodicSelfCheckSettingWidget] Periodic self-check disabled";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
            QString("[ui][SH85PeriodicSelfCheckSettingWidget] Periodic self-check disabled").toStdString());

        if (m_task) {
            const QString taskId = m_task->taskId();
            Scheduler::instance()->cancelTask(taskId);
            m_task = nullptr;  // QPointer 在对象 deleteLater 后自动置空，这里显式清理
        }

        m_statusLabel->setText("Status: Idle");
        m_countdownLabel->setText("Next check in: --:--:--");
    }
}

void SH85PeriodicSelfCheckSettingWidget::onCountdownTick(int remainingSeconds)
{
    updateCountdownDisplay(remainingSeconds);
}

// ============================================================
// 调度任务回调
// ============================================================

void SH85PeriodicSelfCheckSettingWidget::onTaskStatusChanged(SH85PeriodicSelfCheckTask::Status status)
{
    if (status == SH85PeriodicSelfCheckTask::Status::Idle) {
        m_statusLabel->setText("Status: Idle");
    } else {
        m_statusLabel->setText("Status: Checking");
        m_countdownLabel->setText("Next check in: Checking...");
    }
}

void SH85PeriodicSelfCheckSettingWidget::onTaskProgressUpdate(int current, int total)
{
    qDebug() << "[SH85PeriodicSelfCheckSettingWidget] onTaskProgressUpdate:" << current << "/" << total;
    if (m_progressBar) {
        m_progressBar->setRange(0, qMax(1, total));
        m_progressBar->setValue(current);
    }
}

void SH85PeriodicSelfCheckSettingWidget::onTaskRoundFinished(int successCount,
                                                             int failureCount,
                                                             const QString& startTime,
                                                             const QString& endTime)
{
    m_successCount += successCount;
    m_failureCount += failureCount;
    m_lastFailedDeviceCount = failureCount;
    m_lastStartTime = startTime;
    m_lastEndTime   = endTime;

    if (m_countLabel) {
        m_countLabel->setText(QString("Success: %1 | Failure: %2")
                                  .arg(m_successCount).arg(m_failureCount));
    }
    if (m_lastResultLabel) {
        if (failureCount == 0) {
            m_lastResultLabel->setText("Last check: All devices succeeded");
        } else {
            m_lastResultLabel->setText(QString("Last check: %1 device(s) failed").arg(failureCount));
        }
    }
    if (m_timeRangeLabel) {
        m_timeRangeLabel->setText(QString("Time: %1 ~ %2").arg(startTime, endTime));
    }
}

// ============================================================
// 倒计时显示
// ============================================================

void SH85PeriodicSelfCheckSettingWidget::updateCountdownDisplay(int remainingSeconds)
{
    const QString timeStr = formatTime(remainingSeconds);
    m_countdownLabel->setText(QString("Next check in: %1").arg(timeStr));
}

QString SH85PeriodicSelfCheckSettingWidget::formatTime(int totalSeconds) const
{
    const int hours = totalSeconds / 3600;
    const int minutes = (totalSeconds % 3600) / 60;
    const int seconds = totalSeconds % 60;

    return QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

// ============================================================
// enable 方法
// ============================================================

void SH85PeriodicSelfCheckSettingWidget::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);

    // 禁用/启用所有子控件
    if (m_configItem) m_configItem->setEnabled(enabled);
    if (m_resultItem) m_resultItem->setEnabled(enabled);
    if (m_statusItem) m_statusItem->setEnabled(enabled);
    if (m_enableItem) m_enableItem->setEnabled(enabled);
}

bool SH85PeriodicSelfCheckSettingWidget::isRunning() const
{
    return m_isEnabled;
}
