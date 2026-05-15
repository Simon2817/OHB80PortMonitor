#include "purgeflowsettingwidget.h"
#include "../settingwidget/settingitemwidget.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/set_purge_flow_task.h"
#include "app/shareddata.h"
#include "scheduler/tasks/operation_dispatch_task.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QVector>

PurgeFlowSettingWidget::PurgeFlowSettingWidget(QWidget *parent)
    : SettingWidget(parent)
{
    setTitle("Purge Flow Configuration");
    initUI();
}

PurgeFlowSettingWidget::~PurgeFlowSettingWidget() = default;

// ============================================================
// UI
// ============================================================

void PurgeFlowSettingWidget::initUI()
{
    initQrcodeItem();
    initFlowItem();
}

void PurgeFlowSettingWidget::initQrcodeItem()
{
    m_qrcodeItem = new SettingItemWidget(this);
    m_qrcodeItem->setTitle("Target Device");
    m_qrcodeItem->setTip("Target device QRCode (numeric, used by Set button)");

    m_qrcodeSpinBox = new QSpinBox(m_qrcodeItem);
    m_qrcodeSpinBox->setRange(0, 99999);
    m_qrcodeSpinBox->setFixedWidth(160);

    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (!qrcodes.isEmpty()) {
        bool ok = false;
        const int v = qrcodes.first().toInt(&ok);
        if (ok) m_qrcodeSpinBox->setValue(v);
        else qWarning() << "[ui][PurgeFlowSettingWidget] qrcode 转换 int 失败:" << qrcodes.first();
    }

    m_qrcodeItem->addWidget("qrcode_spin", m_qrcodeSpinBox);
    addItem(m_qrcodeItem);
}

void PurgeFlowSettingWidget::initFlowItem()
{
    m_flowItem = new SettingItemWidget(this);
    m_flowItem->setTitle("Purge Flow");
    m_flowItem->setTip("Set VEFC purge flow (only valid in FOUP IN). Register value = flow x 100");

    m_flowSpinBox = new QSpinBox(m_flowItem);
    m_flowSpinBox->setRange(0, 650);             // flow × 100 须在 16-bit 范围内
    m_flowSpinBox->setValue(35);
    m_flowSpinBox->setSuffix(" L/Min");
    m_flowSpinBox->setFixedWidth(120);
    m_flowItem->addWidget("flow_spin", m_flowSpinBox);

    auto *setBtn = new QPushButton("Set", m_flowItem);
    m_flowItem->addWidget("flow_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked,
            this, &PurgeFlowSettingWidget::onSetBtnClicked);

    auto *setAllBtn = new QPushButton("Set All", m_flowItem);
    m_flowItem->addWidget("flow_set_all_btn", setAllBtn);
    connect(setAllBtn, &QPushButton::clicked,
            this, &PurgeFlowSettingWidget::onSetAllBtnClicked);

    addItem(m_flowItem);
}

// ============================================================
// 槽
// ============================================================

void PurgeFlowSettingWidget::onSetBtnClicked()
{
    const QString qrcode = QString::number(m_qrcodeSpinBox->value());
    submitTask(QStringList{qrcode}, m_flowSpinBox->value());
}

void PurgeFlowSettingWidget::onSetAllBtnClicked()
{
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (qrcodes.isEmpty()) {
        QMessageBox::warning(this, "Set Failed", "No target device available");
        return;
    }
    submitTask(qrcodes, m_flowSpinBox->value());
}

// ============================================================
// 提交任务
// ============================================================

void PurgeFlowSettingWidget::submitTask(const QStringList &qrcodes, int flowValue)
{
    const QVector<QString> qrcodeVec(qrcodes.begin(), qrcodes.end());
    auto *task = new SetPurgeFlowTask(qrcodeVec, flowValue);

    m_flowItem->setStatusWaiting();

    connect(task, &SetPurgeFlowTask::allFinished,
            this, [this](bool /*allSuccess*/, int successCount,
                         QStringList failedQrCodes, int flowFinal) {
                const bool hasFailure = !failedQrCodes.isEmpty();
                if (!hasFailure) {
                    m_flowItem->setStatusOK();
                    QMessageBox::information(
                        this, "Set Succeeded",
                        QString("Successfully set Purge Flow=[%1] on %2 device(s)")
                            .arg(flowFinal).arg(successCount));
                } else {
                    m_flowItem->setStatusFailed();
                    const QString failList = failedQrCodes.join(", ");
                    QMessageBox::warning(
                        this, "Set Failed",
                        QString("Failed to set Purge Flow=[%1] on %2 device(s):\n%3")
                            .arg(flowFinal).arg(failedQrCodes.count()).arg(failList));
                }
            });

    Scheduler::instance()->submitTask(task);

    qDebug() << "[ui][PurgeFlowSettingWidget][submitTask]：提交任务 设备数="
             << qrcodes.size() << "flow=" << flowValue;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[ui][PurgeFlowSettingWidget][submitTask]：提交任务 设备数=%1 flow=%2")
            .arg(qrcodes.size()).arg(flowValue).toStdString());
}
