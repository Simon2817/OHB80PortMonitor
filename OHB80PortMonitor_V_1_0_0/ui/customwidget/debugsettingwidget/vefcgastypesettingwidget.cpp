#include "vefcgastypesettingwidget.h"
#include "../settingwidget/settingitemwidget.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/set_vefc_gas_type_task.h"
#include "app/shareddata.h"
#include "scheduler/tasks/running_logger_task.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QVector>

VEFCGasTypeSettingWidget::VEFCGasTypeSettingWidget(QWidget *parent)
    : SettingWidget(parent)
{
    setTitle("VEFC Gas Type Configuration");
    initUI();
}

VEFCGasTypeSettingWidget::~VEFCGasTypeSettingWidget() = default;

// ============================================================
// UI
// ============================================================

void VEFCGasTypeSettingWidget::initUI()
{
    initQrcodeItem();
    initGasTypeItem();
}

void VEFCGasTypeSettingWidget::initQrcodeItem()
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
        else qWarning() << "[ui][VEFCGasTypeSettingWidget] qrcode 转换 int 失败:" << qrcodes.first();
    }

    m_qrcodeItem->addWidget("qrcode_spin", m_qrcodeSpinBox);
    addItem(m_qrcodeItem);
}

void VEFCGasTypeSettingWidget::initGasTypeItem()
{
    m_gasTypeItem = new SettingItemWidget(this);
    m_gasTypeItem->setTitle("VEFC Gas Type");
    m_gasTypeItem->setTip("Set VEFC gas type (power-down retentive). 0=CDA, 1=N2, 2=Ar, 3=CO2, 4=O2");

    m_gasTypeCombo = new QComboBox(m_gasTypeItem);
    m_gasTypeCombo->addItem("CDA", static_cast<int>(SetVEFCGasTypeTask::CDA));
    m_gasTypeCombo->addItem("N2",  static_cast<int>(SetVEFCGasTypeTask::N2));
    m_gasTypeCombo->addItem("Ar",  static_cast<int>(SetVEFCGasTypeTask::Ar));
    m_gasTypeCombo->addItem("CO2", static_cast<int>(SetVEFCGasTypeTask::CO2));
    m_gasTypeCombo->addItem("O2",  static_cast<int>(SetVEFCGasTypeTask::O2));
    m_gasTypeCombo->setFixedWidth(120);
    m_gasTypeItem->addWidget("gas_type_combo", m_gasTypeCombo);

    auto *setBtn = new QPushButton("Set", m_gasTypeItem);
    m_gasTypeItem->addWidget("gas_type_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked,
            this, &VEFCGasTypeSettingWidget::onSetBtnClicked);

    auto *setAllBtn = new QPushButton("Set All", m_gasTypeItem);
    m_gasTypeItem->addWidget("gas_type_set_all_btn", setAllBtn);
    connect(setAllBtn, &QPushButton::clicked,
            this, &VEFCGasTypeSettingWidget::onSetAllBtnClicked);

    addItem(m_gasTypeItem);
}

// ============================================================
// 槽
// ============================================================

void VEFCGasTypeSettingWidget::onSetBtnClicked()
{
    const QString qrcode = QString::number(m_qrcodeSpinBox->value());
    submitTask(QStringList{qrcode}, m_gasTypeCombo->currentData().toInt());
}

void VEFCGasTypeSettingWidget::onSetAllBtnClicked()
{
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (qrcodes.isEmpty()) {
        QMessageBox::warning(this, "Set Failed", "No target device available");
        return;
    }
    submitTask(qrcodes, m_gasTypeCombo->currentData().toInt());
}

// ============================================================
// 提交任务
// ============================================================

void VEFCGasTypeSettingWidget::submitTask(const QStringList &qrcodes, int gasType)
{
    const QVector<QString> qrcodeVec(qrcodes.begin(), qrcodes.end());
    auto *task = new SetVEFCGasTypeTask(qrcodeVec, gasType);

    const QString gasName = m_gasTypeCombo->currentText();

    m_gasTypeItem->setStatusWaiting();

    connect(task, &SetVEFCGasTypeTask::allFinished,
            this, [this, gasName](bool /*allSuccess*/, int successCount,
                                  QStringList failedQrCodes, int gasFinal) {
                const bool hasFailure = !failedQrCodes.isEmpty();
                if (!hasFailure) {
                    m_gasTypeItem->setStatusOK();
                    QMessageBox::information(
                        this, "Set Succeeded",
                        QString("Successfully set VEFC Gas Type=[%1 (0x%2)] on %3 device(s)")
                            .arg(gasName)
                            .arg(QString::number(gasFinal, 16).toUpper().rightJustified(4, '0'))
                            .arg(successCount));
                } else {
                    m_gasTypeItem->setStatusFailed();
                    const QString failList = failedQrCodes.join(", ");
                    QMessageBox::warning(
                        this, "Set Failed",
                        QString("Failed to set VEFC Gas Type=[%1 (0x%2)] on %3 device(s):\n%4")
                            .arg(gasName)
                            .arg(QString::number(gasFinal, 16).toUpper().rightJustified(4, '0'))
                            .arg(failedQrCodes.count())
                            .arg(failList));

                    SharedData::getRunningLoggerTask()->logMessage(
                        QString("[VEFC Gas Type] Set %1 failed on device(s): %2")
                            .arg(gasName).arg(failList));
                }
            });

    Scheduler::instance()->submitTask(task);

    qDebug() << "[ui][VEFCGasTypeSettingWidget][submitTask]：提交任务 设备数="
             << qrcodes.size() << "gasType=" << gasType << "(" << gasName << ")";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[ui][VEFCGasTypeSettingWidget][submitTask]：提交任务 设备数=%1 gasType=%2 (%3)")
            .arg(qrcodes.size()).arg(gasType).arg(gasName).toStdString());
}
