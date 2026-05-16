#ifndef DEVICEENABLESETTINGWIDGET_H
#define DEVICEENABLESETTINGWIDGET_H

#include "settingwidget.h"

#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QString>

class SettingItemWidget;

// ====================================================================
// DeviceEnableSettingWidget — OHB 设备可用性配置控件
//
//   1. Target Device QRCode SpinBox（int，初始值来自 SharedData::getAllQrcodes() 第一个）
//   2. 设备状态 ComboBox（Enable/Disable）+ Set 按钮
//
//   按钮说明：
//     - Set → 仅对 SpinBox 中的设备 ID 生效
// ====================================================================
class DeviceEnableSettingWidget : public SettingWidget
{
    Q_OBJECT

public:
    explicit DeviceEnableSettingWidget(QWidget *parent = nullptr);
    ~DeviceEnableSettingWidget();

private slots:
    void onSetClicked();

private:
    void initUI();
    void initQrcodeItem();
    void initStatusItem();

private:
    QSpinBox        *m_qrcodeSpinBox = nullptr;
    QComboBox       *m_statusComboBox = nullptr;

    SettingItemWidget *m_qrcodeItem = nullptr;
    SettingItemWidget *m_statusItem = nullptr;
};

#endif // DEVICEENABLESETTINGWIDGET_H
