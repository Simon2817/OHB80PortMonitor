#include "idlepurgesettingwidget.h"
#include "../settingwidget/settingitemwidget.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/operation_dispatch_task.h"
#include "app/shareddata.h"

#include <QDebug>
#include <QMessageBox>

IdlePurgeSettingWidget::IdlePurgeSettingWidget(QWidget *parent)
    : SettingWidget(parent)
    , m_prepTimeLineEdit(nullptr)
    , m_enableComboBox(nullptr)
    , m_durationSpinBox(nullptr)
    , m_intervalSpinBox(nullptr)
    , m_enableItem(nullptr)
    , m_durationItem(nullptr)
    , m_intervalItem(nullptr)
{
    setTitle("Idle Purge Configuration");
    initUI();
}

IdlePurgeSettingWidget::~IdlePurgeSettingWidget()
{
}

// ============================================================
// UI 初始化
// ============================================================

void IdlePurgeSettingWidget::initUI()
{
    initPrepTimeItem();
    initEnableItem();
    initDurationItem();
    initIntervalItem();
}

void IdlePurgeSettingWidget::initPrepTimeItem()
{
    auto *item = new SettingItemWidget(this);
    item->setTitle("Preparation Time");
    item->setTip("Fixed preparation phase duration before idle purge starts");

    m_prepTimeLineEdit = new QLineEdit(item);
    m_prepTimeLineEdit->setText("10 s");
    m_prepTimeLineEdit->setReadOnly(true);
    m_prepTimeLineEdit->setFixedWidth(120);
    item->addWidget("prep_time_edit", m_prepTimeLineEdit);

    addItem(item);
}

void IdlePurgeSettingWidget::initEnableItem()
{
    m_enableItem = new SettingItemWidget(this);
    m_enableItem->setTitle("Idle Purge Enable");
    m_enableItem->setTip("Enable or disable idle purge function for all devices");

    m_enableComboBox = new QComboBox(m_enableItem);
    m_enableComboBox->addItem("Enable",  1);
    m_enableComboBox->addItem("Disable", 0);
    m_enableComboBox->setFixedWidth(120);
    m_enableItem->addWidget("enable_combo", m_enableComboBox);

    auto *setBtn = new QPushButton("Set", m_enableItem);
    m_enableItem->addWidget("enable_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked, this, &IdlePurgeSettingWidget::onEnableSetBtnClicked);

    addItem(m_enableItem);
}

void IdlePurgeSettingWidget::initDurationItem()
{
    m_durationItem = new SettingItemWidget(this);
    m_durationItem->setTitle("Purge Duration");
    m_durationItem->setTip("Set idle purge inflation duration (seconds) for all devices");

    m_durationSpinBox = new QSpinBox(m_durationItem);
    m_durationSpinBox->setRange(1, 9999);
    m_durationSpinBox->setValue(10);
    m_durationSpinBox->setSuffix(" s");
    m_durationSpinBox->setFixedWidth(120);
    m_durationItem->addWidget("duration_spin", m_durationSpinBox);

    auto *setBtn = new QPushButton("Set", m_durationItem);
    m_durationItem->addWidget("duration_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked, this, &IdlePurgeSettingWidget::onDurationSetBtnClicked);

    addItem(m_durationItem);
}

void IdlePurgeSettingWidget::initIntervalItem()
{
    m_intervalItem = new SettingItemWidget(this);
    m_intervalItem->setTitle("Purge Interval");
    m_intervalItem->setTip("Set idle purge interval between cycles (seconds) for all devices");

    m_intervalSpinBox = new QSpinBox(m_intervalItem);
    m_intervalSpinBox->setRange(1, 99999);
    m_intervalSpinBox->setValue(5);
    m_intervalSpinBox->setSuffix(" s");
    m_intervalSpinBox->setFixedWidth(120);
    m_intervalItem->addWidget("interval_spin", m_intervalSpinBox);

    auto *setBtn = new QPushButton("Set", m_intervalItem);
    m_intervalItem->addWidget("interval_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked, this, &IdlePurgeSettingWidget::onIntervalSetBtnClicked);

    addItem(m_intervalItem);
}

// ============================================================
// 槽函数
// ============================================================

void IdlePurgeSettingWidget::onEnableSetBtnClicked()
{
    const quint16 value = static_cast<quint16>(m_enableComboBox->currentData().toInt());
    submitCommand(m_enableItem, SetIdlePurgeTask::IdlePurgeProperty::Enable, value);
}

void IdlePurgeSettingWidget::onDurationSetBtnClicked()
{
    const quint16 value = static_cast<quint16>(m_durationSpinBox->value());
    submitCommand(m_durationItem, SetIdlePurgeTask::IdlePurgeProperty::PurgeTime, value);
}

void IdlePurgeSettingWidget::onIntervalSetBtnClicked()
{
    const quint16 value = static_cast<quint16>(m_intervalSpinBox->value());
    submitCommand(m_intervalItem, SetIdlePurgeTask::IdlePurgeProperty::PurgeInterval, value);
}

// ============================================================
// 内部辅助
// ============================================================

void IdlePurgeSettingWidget::submitCommand(SettingItemWidget *item,
                                            SetIdlePurgeTask::IdlePurgeProperty property,
                                            quint16 value)
{
    auto *task = new SetIdlePurgeTask(property, value);

    item->setStatusWaiting();

    connect(task, &SetIdlePurgeTask::allFinished, this,
            [this, item](bool /*allSuccess*/, int successCount,
                         QStringList failedQrCodes,
                         QString propertyName, quint16 setValue) {
                // 只要有一台失败即视为任务失败
                const bool hasFailure = !failedQrCodes.isEmpty();
                if (!hasFailure) {
                    item->setStatusOK();
                    QMessageBox::information(
                        this,
                        "Set Succeeded",
                        QString("Successfully set [%1] to [%2] on %3 device(s)")
                            .arg(propertyName)
                            .arg(setValue)
                            .arg(successCount));
                } else {
                    item->setStatusFailed();

                    // MessageBox 提示失败设备列表
                    const QString failList = failedQrCodes.join(", ");
                    QMessageBox::warning(
                        this,
                        "Set Failed",
                        QString("Failed to set [%1] to [%2] on %3 device(s):\n%4")
                            .arg(propertyName)
                            .arg(setValue)
                            .arg(failedQrCodes.count())
                            .arg(failList));

                    // 向运行日志投递 Message 级别日志
                    const QString logMsg =
                        QString("[Idle Purge] Set %1 = %2 failed on device(s): %3")
                            .arg(propertyName).arg(setValue).arg(failList);
                    SharedData::getOperationDispatchTask()->logMessage(logMsg);
                }
            });

    Scheduler::instance()->submitTask(task);
}
