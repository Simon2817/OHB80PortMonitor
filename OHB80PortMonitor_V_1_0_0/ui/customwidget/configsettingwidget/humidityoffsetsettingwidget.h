#ifndef HUMIDITYOFFSETSETTINGWIDGET_H
#define HUMIDITYOFFSETSETTINGWIDGET_H

#include "settingwidget.h"

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QString>
#include <QStringList>

class SettingItemWidget;

// ====================================================================
// HumidityOffsetSettingWidget — 湿度校准（offset）配置控件
//
//   1. Target Device QRCode SpinBox（int，初始值来自 SharedData::getAllQrcodes() 第一个）
//   2. 湿度校准触发阈值（%）+ Set + Set All
//   3. 湿度 offset 参数（%）  + Set + Set All
//
//   按钮说明：
//     - Set     → 仅对 SpinBox 中的设备 ID 生效
//     - Set All → 对 SharedData::getAllQrcodes() 全部设备生效
//   两个参数独立工作：
//     - Threshold → SetHumidityOffsetTask::setThreshold(...) only
//     - Offset    → SetHumidityOffsetTask::setOffset(...) only
//
//   寄存器值 = 百分比 × 100（例：18% → 1800）
// ====================================================================
class HumidityOffsetSettingWidget : public SettingWidget
{
    Q_OBJECT

public:
    explicit HumidityOffsetSettingWidget(QWidget *parent = nullptr);
    ~HumidityOffsetSettingWidget();

private slots:
    void onSetThresholdClicked();
    void onSetThresholdAllClicked();
    void onSetOffsetClicked();
    void onSetOffsetAllClicked();

private:
    void initUI();
    void initQrcodeItem();
    void initThresholdItem();
    void initOffsetItem();

    // 提交任务（仅设置一项）
    void submitTask(const QStringList &qrcodes,
                    bool isThreshold,
                    double valuePct,
                    SettingItemWidget *targetItem);

private:
    QSpinBox        *m_qrcodeSpinBox     = nullptr;
    QDoubleSpinBox  *m_thresholdSpinBox  = nullptr;
    QDoubleSpinBox  *m_offsetSpinBox     = nullptr;

    SettingItemWidget *m_qrcodeItem    = nullptr;
    SettingItemWidget *m_thresholdItem = nullptr;
    SettingItemWidget *m_offsetItem    = nullptr;
};

#endif // HUMIDITYOFFSETSETTINGWIDGET_H
