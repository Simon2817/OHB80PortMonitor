#include "uirefreshtimesettingwidget.h"
#include "../settingwidget/settingitemwidget.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/set_ui_refresh_time_task.h"
#include "app/shareddata.h"
#include "scheduler/tasks/running_logger_task.h"
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
    initLogScreenItem();
    initPropertyScreenItem();
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

void UIRefreshTimeSettingWidget::initLogScreenItem()
{
    m_logScreenItem = new SettingItemWidget(this);
    m_logScreenItem->setTitle("Log Screen Duration");
    m_logScreenItem->setTip("Display duration for log screen on device UI (seconds)");

    m_logSecSpinBox = new QSpinBox(m_logScreenItem);
    m_logSecSpinBox->setRange(0, 65535);
    m_logSecSpinBox->setValue(5);
    m_logSecSpinBox->setSuffix(" s");
    m_logSecSpinBox->setFixedWidth(120);
    m_logScreenItem->addWidget("log_sec_spin", m_logSecSpinBox);

    addItem(m_logScreenItem);
}

void UIRefreshTimeSettingWidget::initPropertyScreenItem()
{
    m_propScreenItem = new SettingItemWidget(this);
    m_propScreenItem->setTitle("Property Screen Duration");
    m_propScreenItem->setTip("Display duration for property screen on device UI (seconds)");

    m_propSecSpinBox = new QSpinBox(m_propScreenItem);
    m_propSecSpinBox->setRange(0, 65535);
    m_propSecSpinBox->setValue(5);
    m_propSecSpinBox->setSuffix(" s");
    m_propSecSpinBox->setFixedWidth(120);
    m_propScreenItem->addWidget("prop_sec_spin", m_propSecSpinBox);

    auto *setBtn = new QPushButton("Set", m_propScreenItem);
    m_propScreenItem->addWidget("ui_refresh_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked,
            this, &UIRefreshTimeSettingWidget::onSetBtnClicked);

    auto *setAllBtn = new QPushButton("Set All", m_propScreenItem);
    m_propScreenItem->addWidget("ui_refresh_set_all_btn", setAllBtn);
    connect(setAllBtn, &QPushButton::clicked,
            this, &UIRefreshTimeSettingWidget::onSetAllBtnClicked);

    addItem(m_propScreenItem);
}

// ============================================================
// 槽
// ============================================================

void UIRefreshTimeSettingWidget::onSetBtnClicked()
{
    const QString qrcode = QString::number(m_qrcodeSpinBox->value());
    submitTask(QStringList{qrcode}, m_logSecSpinBox->value(), m_propSecSpinBox->value());
}

void UIRefreshTimeSettingWidget::onSetAllBtnClicked()
{
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (qrcodes.isEmpty()) {
        QMessageBox::warning(this, "Set Failed", "No target device available");
        return;
    }
    submitTask(qrcodes, m_logSecSpinBox->value(), m_propSecSpinBox->value());
}

// ============================================================
// 提交任务
// ============================================================

void UIRefreshTimeSettingWidget::submitTask(const QStringList &qrcodes, int logSec, int propSec)
{
    const QVector<QString> qrcodeVec(qrcodes.begin(), qrcodes.end());
    auto *task = new SetUIRefreshTimeTask(qrcodeVec, logSec, propSec);

    m_logScreenItem->setStatusWaiting();
    m_propScreenItem->setStatusWaiting();

    connect(task, &SetUIRefreshTimeTask::allFinished,
            this, [this](bool /*allSuccess*/, int successCount,
                         QStringList failedQrCodes,
                         int logSecFinal, int propSecFinal) {
                const bool hasFailure = !failedQrCodes.isEmpty();
                if (!hasFailure) {
                    m_logScreenItem->setStatusOK();
                    m_propScreenItem->setStatusOK();
                    QMessageBox::information(
                        this, "Set Succeeded",
                        QString("Successfully set UI Refresh Time (log=%1s, prop=%2s) on %3 device(s)")
                            .arg(logSecFinal).arg(propSecFinal).arg(successCount));
                } else {
                    m_logScreenItem->setStatusFailed();
                    m_propScreenItem->setStatusFailed();
                    const QString failList = failedQrCodes.join(", ");
                    QMessageBox::warning(
                        this, "Set Failed",
                        QString("Failed to set UI Refresh Time (log=%1s, prop=%2s) on %3 device(s):\n%4")
                            .arg(logSecFinal).arg(propSecFinal)
                            .arg(failedQrCodes.count()).arg(failList));

                    SharedData::getRunningLoggerTask()->logMessage(
                        QString("[UI Refresh Time] Set log=%1s prop=%2s failed on device(s): %3")
                            .arg(logSecFinal).arg(propSecFinal).arg(failList));
                }
            });

    Scheduler::instance()->submitTask(task);

    qDebug() << "[ui][UIRefreshTimeSettingWidget][submitTask]：提交任务 设备数="
             << qrcodes.size() << "logSec=" << logSec << "propSec=" << propSec;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[ui][UIRefreshTimeSettingWidget][submitTask]：提交任务 设备数=%1 logSec=%2 propSec=%3")
            .arg(qrcodes.size()).arg(logSec).arg(propSec).toStdString());
}
