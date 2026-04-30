#include "humidityoffsetsettingwidget.h"
#include "../settingwidget/settingitemwidget.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/set_humidity_offset_task.h"
#include "app/shareddata.h"
#include "customwidget/runningloggerwidget/runningloggercollector.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <QVector>

HumidityOffsetSettingWidget::HumidityOffsetSettingWidget(QWidget *parent)
    : SettingWidget(parent)
{
    setTitle("Humidity Offset Configuration");
    initUI();
}

HumidityOffsetSettingWidget::~HumidityOffsetSettingWidget() = default;

// ============================================================
// UI 初始化
// ============================================================

void HumidityOffsetSettingWidget::initUI()
{
    initQrcodeItem();
    initThresholdItem();
    initOffsetItem();
}

void HumidityOffsetSettingWidget::initQrcodeItem()
{
    m_qrcodeItem = new SettingItemWidget(this);
    m_qrcodeItem->setTitle("Target Device");
    m_qrcodeItem->setTip("Target device QRCode (numeric) for humidity offset setting");

    m_qrcodeSpinBox = new QSpinBox(m_qrcodeItem);
    m_qrcodeSpinBox->setRange(0, 2147483647);
    m_qrcodeSpinBox->setFixedWidth(160);

    // 初始值：SharedData::getAllQrcodes() 第一个
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (!qrcodes.isEmpty()) {
        bool ok = false;
        const int v = qrcodes.first().toInt(&ok);
        if (ok) {
            m_qrcodeSpinBox->setValue(v);
        } else {
            qWarning() << "[ui][HumidityOffsetSettingWidget] qrcode 转换 int 失败:" << qrcodes.first();
        }
    }

    m_qrcodeItem->addWidget("qrcode_spin", m_qrcodeSpinBox);
    addItem(m_qrcodeItem);
}

void HumidityOffsetSettingWidget::initThresholdItem()
{
    m_thresholdItem = new SettingItemWidget(this);
    m_thresholdItem->setTitle("Humidity Offset Threshold");
    m_thresholdItem->setTip("Set humidity calibration trigger threshold (0~100 %). Register value = pct x 100");

    m_thresholdSpinBox = new QDoubleSpinBox(m_thresholdItem);
    m_thresholdSpinBox->setRange(0.0, 100.0);
    m_thresholdSpinBox->setDecimals(2);
    m_thresholdSpinBox->setSingleStep(0.1);
    m_thresholdSpinBox->setValue(5.0);
    m_thresholdSpinBox->setSuffix(" %");
    m_thresholdSpinBox->setFixedWidth(120);
    m_thresholdItem->addWidget("threshold_spin", m_thresholdSpinBox);

    auto *setBtn = new QPushButton("Set", m_thresholdItem);
    m_thresholdItem->addWidget("threshold_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked,
            this, &HumidityOffsetSettingWidget::onSetThresholdClicked);

    auto *setAllBtn = new QPushButton("Set All", m_thresholdItem);
    m_thresholdItem->addWidget("threshold_set_all_btn", setAllBtn);
    connect(setAllBtn, &QPushButton::clicked,
            this, &HumidityOffsetSettingWidget::onSetThresholdAllClicked);

    addItem(m_thresholdItem);
}

void HumidityOffsetSettingWidget::initOffsetItem()
{
    m_offsetItem = new SettingItemWidget(this);
    m_offsetItem->setTitle("Humidity Offset");
    m_offsetItem->setTip("Set humidity offset parameter (0~100 %). Register value = pct x 100");

    m_offsetSpinBox = new QDoubleSpinBox(m_offsetItem);
    m_offsetSpinBox->setRange(0.0, 100.0);
    m_offsetSpinBox->setDecimals(2);
    m_offsetSpinBox->setSingleStep(0.1);
    m_offsetSpinBox->setValue(0.0);
    m_offsetSpinBox->setSuffix(" %");
    m_offsetSpinBox->setFixedWidth(120);
    m_offsetItem->addWidget("offset_spin", m_offsetSpinBox);

    auto *setBtn = new QPushButton("Set", m_offsetItem);
    m_offsetItem->addWidget("offset_set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked,
            this, &HumidityOffsetSettingWidget::onSetOffsetClicked);

    auto *setAllBtn = new QPushButton("Set All", m_offsetItem);
    m_offsetItem->addWidget("offset_set_all_btn", setAllBtn);
    connect(setAllBtn, &QPushButton::clicked,
            this, &HumidityOffsetSettingWidget::onSetOffsetAllClicked);

    addItem(m_offsetItem);
}

// ============================================================
// 槽函数
// ============================================================

void HumidityOffsetSettingWidget::onSetThresholdClicked()
{
    const QString qrcode = QString::number(m_qrcodeSpinBox->value());
    submitTask(QStringList{qrcode}, /*isThreshold=*/true,
               m_thresholdSpinBox->value(), m_thresholdItem);
}

void HumidityOffsetSettingWidget::onSetThresholdAllClicked()
{
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (qrcodes.isEmpty()) {
        QMessageBox::warning(this, "Set Failed", "No target device available");
        return;
    }
    submitTask(qrcodes, /*isThreshold=*/true,
               m_thresholdSpinBox->value(), m_thresholdItem);
}

void HumidityOffsetSettingWidget::onSetOffsetClicked()
{
    const QString qrcode = QString::number(m_qrcodeSpinBox->value());
    submitTask(QStringList{qrcode}, /*isThreshold=*/false,
               m_offsetSpinBox->value(), m_offsetItem);
}

void HumidityOffsetSettingWidget::onSetOffsetAllClicked()
{
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (qrcodes.isEmpty()) {
        QMessageBox::warning(this, "Set Failed", "No target device available");
        return;
    }
    submitTask(qrcodes, /*isThreshold=*/false,
               m_offsetSpinBox->value(), m_offsetItem);
}

// ============================================================
// 内部辅助
// ============================================================

void HumidityOffsetSettingWidget::submitTask(const QStringList &qrcodes,
                                             bool isThreshold,
                                             double valuePct,
                                             SettingItemWidget *targetItem)
{
    const QVector<QString> qrcodeVec(qrcodes.begin(), qrcodes.end());
    auto *task = new SetHumidityOffsetTask(qrcodeVec);
    if (isThreshold)
        task->setThreshold(valuePct);
    else
        task->setOffset(valuePct);

    targetItem->setStatusWaiting();

    const QString fieldName = isThreshold ? "Threshold" : "Offset";

    connect(task, &SetHumidityOffsetTask::allFinished,
            this, [this, targetItem, fieldName, valuePct]
                  (bool /*allSuccess*/, int successCount,
                   QStringList failedQrCodes,
                   bool /*thresholdSet*/, double /*thresholdPct*/,
                   bool /*offsetSet*/,    double /*offsetPct*/) {
                const bool hasFailure = !failedQrCodes.isEmpty();
                if (!hasFailure) {
                    targetItem->setStatusOK();
                    QMessageBox::information(
                        this,
                        "Set Succeeded",
                        QString("Successfully set Humidity %1 to [%2 %] on %3 device(s)")
                            .arg(fieldName).arg(valuePct).arg(successCount));
                } else {
                    targetItem->setStatusFailed();
                    const QString failList = failedQrCodes.join(", ");
                    QMessageBox::warning(
                        this,
                        "Set Failed",
                        QString("Failed to set Humidity %1=[%2 %] on %3 device(s):\n%4")
                            .arg(fieldName).arg(valuePct)
                            .arg(failedQrCodes.count()).arg(failList));

                    const QString logMsg =
                        QString("[Humidity %1] Set %2%% failed on device(s): %3")
                            .arg(fieldName).arg(valuePct).arg(failList);
                    RunningLoggerCollector::instance()->logMessage(logMsg);
                }
            });

    Scheduler::instance()->submitTask(task);

    qDebug() << "[ui][HumidityOffsetSettingWidget][submitTask]：提交任务"
             << "field=" << fieldName << "设备数=" << qrcodes.size() << "value=" << valuePct << "%";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[ui][HumidityOffsetSettingWidget][submitTask]：提交任务 field=%1 设备数=%2 value=%3%%")
            .arg(fieldName).arg(qrcodes.size()).arg(valuePct).toStdString());
}
