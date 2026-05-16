#include "deviceenablesettingwidget.h"
#include "../settingwidget/settingitemwidget.h"
#include "app/shareddata.h"
#include "app/ohbdeviceconfig.h"
#include "classes/foupofohbinfo.h"

#include <QDebug>

DeviceEnableSettingWidget::DeviceEnableSettingWidget(QWidget *parent)
    : SettingWidget(parent)
{
    setTitle("Device Enable Configuration");
    initUI();
}

DeviceEnableSettingWidget::~DeviceEnableSettingWidget() = default;

// ============================================================
// UI 初始化
// ============================================================

void DeviceEnableSettingWidget::initUI()
{
    initQrcodeItem();
    initStatusItem();
}

void DeviceEnableSettingWidget::initQrcodeItem()
{
    m_qrcodeItem = new SettingItemWidget(this);
    m_qrcodeItem->setTitle("Target Device");
    m_qrcodeItem->setTip("Target device QRCode (numeric) for enable/disable setting");

    m_qrcodeSpinBox = new QSpinBox(m_qrcodeItem);
    m_qrcodeSpinBox->setRange(0, 99999);
    m_qrcodeSpinBox->setFixedWidth(160);

    // 初始值：SharedData::getAllQrcodes() 第一个
    const QStringList qrcodes = SharedData::getAllQrcodes();
    if (!qrcodes.isEmpty()) {
        bool ok = false;
        const int v = qrcodes.first().toInt(&ok);
        if (ok) {
            m_qrcodeSpinBox->setValue(v);
        } else {
            qWarning() << "[ui][DeviceEnableSettingWidget] qrcode 转换 int 失败:" << qrcodes.first();
        }
    }

    m_qrcodeItem->addWidget("qrcode_spin", m_qrcodeSpinBox);
    addItem(m_qrcodeItem);
}

void DeviceEnableSettingWidget::initStatusItem()
{
    m_statusItem = new SettingItemWidget(this);
    m_statusItem->setTitle("Device Status");
    m_statusItem->setTip("Set device enable/disable status");

    m_statusComboBox = new QComboBox(m_statusItem);
    m_statusComboBox->addItem("Enable", true);
    m_statusComboBox->addItem("Disable", false);
    m_statusComboBox->setFixedWidth(120);
    m_statusItem->addWidget("status_combo", m_statusComboBox);

    auto *setBtn = new QPushButton("Set", m_statusItem);
    m_statusItem->addWidget("set_btn", setBtn);
    connect(setBtn, &QPushButton::clicked,
            this, &DeviceEnableSettingWidget::onSetClicked);

    addItem(m_statusItem);
}

// ============================================================
// 槽函数
// ============================================================

void DeviceEnableSettingWidget::onSetClicked()
{
    const QString qrcode = QString::number(m_qrcodeSpinBox->value());
    const bool enable = m_statusComboBox->currentData().toBool();

    m_statusItem->setStatusWaiting();

    // 持久化到配置文件
    bool persistSuccess = OHBDeviceConfig::getInstance().setDeviceEnable(qrcode, enable);

    FoupOfOHBInfo* foup = SharedData::getFoupByQRCode(qrcode);
    if (foup) {
        foup->setEnable(enable);
        if (persistSuccess) {
            m_statusItem->setStatusOK();
            qDebug() << "[ui][DeviceEnableSettingWidget] Set device enable - QRCode:"
                     << qrcode << "Enable:" << enable;
        } else {
            m_statusItem->setStatusFailed();
            qWarning() << "[ui][DeviceEnableSettingWidget] Failed to persist enable config for QRCode:"
                       << qrcode;
        }
    } else {
        m_statusItem->setStatusFailed();
        qWarning() << "[ui][DeviceEnableSettingWidget] Failed to find FoupOfOHBInfo for QRCode:"
                   << qrcode;
    }
}
