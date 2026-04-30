#include "vefcflowunitmediumstatuswidget.h"
#include "../settingwidget/settingitemwidget.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/read_vefc_flow_unit_medium_status_task.h"
#include "app/shareddata.h"
#include "customwidget/runningloggerwidget/runningloggercollector.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <QVector>

VEFCFlowUnitMediumStatusWidget::VEFCFlowUnitMediumStatusWidget(QWidget *parent)
    : SettingWidget(parent)
{
    setTitle("VEFC Flow Unit / Medium Status");
    initUI();
}

VEFCFlowUnitMediumStatusWidget::~VEFCFlowUnitMediumStatusWidget() = default;

// ============================================================
// UI
// ============================================================

void VEFCFlowUnitMediumStatusWidget::initUI()
{
    initQrcodeItem();
    initReadItem();
}

void VEFCFlowUnitMediumStatusWidget::initQrcodeItem()
{
    m_qrcodeItem = new SettingItemWidget(this);
    m_qrcodeItem->setTitle("Target Device");
    m_qrcodeItem->setTip("Target device QRCode (numeric, used by Read button)");

    m_qrcodeSpinBox = new QSpinBox(m_qrcodeItem);
    m_qrcodeSpinBox->setRange(0, 99999);
    m_qrcodeSpinBox->setFixedWidth(160);

    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (!qrcodes.isEmpty()) {
        bool ok = false;
        const int v = qrcodes.first().toInt(&ok);
        if (ok) m_qrcodeSpinBox->setValue(v);
        else qWarning() << "[ui][VEFCFlowUnitMediumStatusWidget] qrcode 转换 int 失败:" << qrcodes.first();
    }

    m_qrcodeItem->addWidget("qrcode_spin", m_qrcodeSpinBox);
    addItem(m_qrcodeItem);
}

void VEFCFlowUnitMediumStatusWidget::initReadItem()
{
    m_readItem = new SettingItemWidget(this);
    m_readItem->setTitle("Flow Unit / Medium Status");
    m_readItem->setTip("Read VEFC flow unit & medium config status. Default unit=L/Min, default medium=CDA");

    auto *readBtn = new QPushButton("Read", m_readItem);
    m_readItem->addWidget("status_read_btn", readBtn);
    connect(readBtn, &QPushButton::clicked,
            this, &VEFCFlowUnitMediumStatusWidget::onReadBtnClicked);

    auto *readAllBtn = new QPushButton("Read All", m_readItem);
    m_readItem->addWidget("status_read_all_btn", readAllBtn);
    connect(readAllBtn, &QPushButton::clicked,
            this, &VEFCFlowUnitMediumStatusWidget::onReadAllBtnClicked);

    addItem(m_readItem);
}

// ============================================================
// 槽
// ============================================================

void VEFCFlowUnitMediumStatusWidget::onReadBtnClicked()
{
    const QString qrcode = QString::number(m_qrcodeSpinBox->value());
    submitTask(QStringList{qrcode});
}

void VEFCFlowUnitMediumStatusWidget::onReadAllBtnClicked()
{
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (qrcodes.isEmpty()) {
        QMessageBox::warning(this, "Read Failed", "No target device available");
        return;
    }
    submitTask(qrcodes);
}

// ============================================================
// 提交任务
// ============================================================

void VEFCFlowUnitMediumStatusWidget::submitTask(const QStringList &qrcodes)
{
    using Task = ReadVEFCFlowUnitAndMediumStatusTask;
    const QVector<QString> qrcodeVec(qrcodes.begin(), qrcodes.end());
    auto *task = new Task(qrcodeVec);

    m_readItem->setStatusWaiting();

    connect(task, &Task::allFinished,
            this, [this](bool /*allSuccess*/, int successCount,
                         QList<Task::DeviceStatus> results) {
                // 收集失败行
                QStringList failureLines;
                for (const Task::DeviceStatus &st : results) {
                    if (st.allOk()) continue;

                    if (st.commFailed) {
                        failureLines.append(QString("Device %1: Communication FAILED").arg(st.qrcode));
                    } else {
                        QStringList parts;
                        if (!st.unitOk)   parts.append("Unit");
                        if (!st.mediumOk) parts.append("Medium");
                        failureLines.append(
                            QString("Device %1: %2 config FAILED")
                                .arg(st.qrcode)
                                .arg(parts.join(" + ")));
                    }
                }

                if (failureLines.isEmpty()) {
                    m_readItem->setStatusOK();
                    QMessageBox::information(
                        this, "Read Succeeded",
                        QString("All %1 device(s) passed:\nUnit OK + Medium OK")
                            .arg(successCount));
                } else {
                    m_readItem->setStatusFailed();
                    const QString detail = failureLines.join("\n");
                    QMessageBox::warning(
                        this, "Read Failed",
                        QString("%1/%2 device(s) passed.\n\nFailures:\n%3")
                            .arg(successCount).arg(results.size()).arg(detail));

                    RunningLoggerCollector::instance()->logMessage(
                        QString("[VEFC Flow Unit/Medium] %1/%2 passed; failures: %3")
                            .arg(successCount).arg(results.size()).arg(failureLines.join("; ")));
                }
            });

    Scheduler::instance()->submitTask(task);

    qDebug() << "[ui][VEFCFlowUnitMediumStatusWidget][submitTask]：提交任务 设备数=" << qrcodes.size();
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[ui][VEFCFlowUnitMediumStatusWidget][submitTask]：提交任务 设备数=%1")
            .arg(qrcodes.size()).toStdString());
}
