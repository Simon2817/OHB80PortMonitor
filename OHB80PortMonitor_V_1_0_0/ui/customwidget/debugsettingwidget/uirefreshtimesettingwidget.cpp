#include "uirefreshtimesettingwidget.h"
#include "../settingwidget/settingitemwidget.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/set_ui_refresh_time_task.h"
#include "app/shareddata.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QVector>

UIRefreshTimeSettingWidget::UIRefreshTimeSettingWidget(QWidget *parent)
    : SettingWidget(parent)
{
    setTitle("UI Refresh Time Configuration");
    initUI();
}

UIRefreshTimeSettingWidget::~UIRefreshTimeSettingWidget() = default;

// ============================================================
// UI
// ============================================================

void UIRefreshTimeSettingWidget::initUI()
{
    initQrcodeItem();
    initLogoItem();
    initParamTotalItem();
    initParamSwitchItem();
}

void UIRefreshTimeSettingWidget::initQrcodeItem()
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
        else qWarning() << "[ui][UIRefreshTimeSettingWidget] qrcode 转换 int 失败:" << qrcodes.first();
    }

    m_qrcodeItem->addWidget("qrcode_spin", m_qrcodeSpinBox);
    addItem(m_qrcodeItem);
}

void UIRefreshTimeSettingWidget::initLogoItem()
{
    m_logoItem = new SettingItemWidget(this);
    m_logoItem->setTitle("Logo Screen Duration");
    m_logoItem->setTip("Logo screen display duration (seconds)");

    m_logoSecSpinBox = new QSpinBox(m_logoItem);
    m_logoSecSpinBox->setRange(0, 65535);
    m_logoSecSpinBox->setValue(5);
    m_logoSecSpinBox->setSuffix(" s");
    m_logoSecSpinBox->setFixedWidth(120);
    m_logoItem->addWidget("logo_sec_spin", m_logoSecSpinBox);

    addItem(m_logoItem);
}

void UIRefreshTimeSettingWidget::initParamTotalItem()
{
    m_paramTotalItem = new SettingItemWidget(this);
    m_paramTotalItem->setTitle("Param Screen Total Duration");
    m_paramTotalItem->setTip("Parameter screen total display duration (seconds)");

    m_paramTotalSpinBox = new QSpinBox(m_paramTotalItem);
    m_paramTotalSpinBox->setRange(0, 65535);
    m_paramTotalSpinBox->setValue(5);
    m_paramTotalSpinBox->setSuffix(" s");
    m_paramTotalSpinBox->setFixedWidth(120);
    m_paramTotalItem->addWidget("param_total_spin", m_paramTotalSpinBox);

    addItem(m_paramTotalItem);
}

void UIRefreshTimeSettingWidget::initParamSwitchItem()
{
    m_paramSwitchItem = new SettingItemWidget(this);
    m_paramSwitchItem->setTitle("Param Page Switch Interval");
    m_paramSwitchItem->setTip("Parameter page switch interval (seconds)");

    m_paramSwitchSpinBox = new QSpinBox(m_paramSwitchItem);
    m_paramSwitchSpinBox->setRange(0, 65535);
    m_paramSwitchSpinBox->setValue(5);
    m_paramSwitchSpinBox->setSuffix(" s");
    m_paramSwitchSpinBox->setFixedWidth(120);
    m_paramSwitchItem->addWidget("param_switch_spin", m_paramSwitchSpinBox);

    auto *setBtn = new QPushButton("Set", m_paramSwitchItem);
    m_paramSwitchItem->addWidget("ui_refresh_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked,
            this, &UIRefreshTimeSettingWidget::onSetBtnClicked);

    auto *setAllBtn = new QPushButton("Set All", m_paramSwitchItem);
    m_paramSwitchItem->addWidget("ui_refresh_set_all_btn", setAllBtn);
    connect(setAllBtn, &QPushButton::clicked,
            this, &UIRefreshTimeSettingWidget::onSetAllBtnClicked);

    addItem(m_paramSwitchItem);
}

// ============================================================
// 槽
// ============================================================

void UIRefreshTimeSettingWidget::onSetBtnClicked()
{
    const QString qrcode = QString::number(m_qrcodeSpinBox->value());
    submitTask(QStringList{qrcode},
              m_logoSecSpinBox->value(),
              m_paramTotalSpinBox->value(),
              m_paramSwitchSpinBox->value());
}

void UIRefreshTimeSettingWidget::onSetAllBtnClicked()
{
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (qrcodes.isEmpty()) {
        QMessageBox::warning(this, "Set Failed", "No target device available");
        return;
    }
    submitTask(qrcodes,
              m_logoSecSpinBox->value(),
              m_paramTotalSpinBox->value(),
              m_paramSwitchSpinBox->value());
}

// ============================================================
// 提交任务
// ============================================================

void UIRefreshTimeSettingWidget::submitTask(const QStringList &qrcodes,
                                             int logoSec, int paramTotalSec, int paramSwitchSec)
{
    const QVector<QString> qrcodeVec(qrcodes.begin(), qrcodes.end());
    auto *task = new SetUIRefreshTimeTask(qrcodeVec, logoSec, paramTotalSec, paramSwitchSec);

    m_logoItem->setStatusWaiting();
    m_paramTotalItem->setStatusWaiting();
    m_paramSwitchItem->setStatusWaiting();

    connect(task, &SetUIRefreshTimeTask::allFinished,
            this, [this](bool /*allSuccess*/, int successCount,
                         QStringList failedQrCodes,
                         int logoFinal, int totalFinal, int switchFinal) {
                const bool hasFailure = !failedQrCodes.isEmpty();
                if (!hasFailure) {
                    m_logoItem->setStatusOK();
                    m_paramTotalItem->setStatusOK();
                    m_paramSwitchItem->setStatusOK();
                    QMessageBox::information(
                        this, "Set Succeeded",
                        QString("Successfully set UI Refresh Time (logo=%1s, total=%2s, switch=%3s) on %4 device(s)")
                            .arg(logoFinal).arg(totalFinal).arg(switchFinal).arg(successCount));
                } else {
                    m_logoItem->setStatusFailed();
                    m_paramTotalItem->setStatusFailed();
                    m_paramSwitchItem->setStatusFailed();
                    const QString failList = failedQrCodes.join(", ");
                    QMessageBox::warning(
                        this, "Set Failed",
                        QString("Failed to set UI Refresh Time (logo=%1s, total=%2s, switch=%3s) on %4 device(s):\n%5")
                            .arg(logoFinal).arg(totalFinal).arg(switchFinal)
                            .arg(failedQrCodes.count()).arg(failList));
                }
            });

    Scheduler::instance()->submitTask(task);

    qDebug() << "[ui][UIRefreshTimeSettingWidget][submitTask]：提交任务 设备数="
             << qrcodes.size() << "logoSec=" << logoSec
             << "paramTotalSec=" << paramTotalSec << "paramSwitchSec=" << paramSwitchSec;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[ui][UIRefreshTimeSettingWidget][submitTask]：提交任务 设备数=%1 logoSec=%2 paramTotalSec=%3 paramSwitchSec=%4")
            .arg(qrcodes.size()).arg(logoSec).arg(paramTotalSec).arg(paramSwitchSec).toStdString());
}
