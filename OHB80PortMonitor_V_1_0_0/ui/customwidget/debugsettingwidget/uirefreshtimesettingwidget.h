#ifndef UIREFRESHTIMESETTINGWIDGET_H
#define UIREFRESHTIMESETTINGWIDGET_H

#include "settingwidget.h"

#include <QSpinBox>
#include <QPushButton>
#include <QString>
#include <QStringList>

class SettingItemWidget;

// ====================================================================
// UIRefreshTimeSettingWidget — UI 页面刷新时间配置控件（DebugPage）
//
//   1. Target Device QRCode SpinBox(int, 0~99999, 默认第一个设备)
//   2. Log Screen Duration SpinBox + Property Screen Duration SpinBox
//      + Set + Set All
//
//   Set     → 仅作用于 SpinBox 中的设备 ID
//   Set All → 作用于 SharedData::getAllQrcodes() 全部设备
//
//   底层指令：WriteUIRefreshTime（FC 0x10, addr 0x0004, 4 字节）
//     [0..1] log 界面时长（秒）   [2..3] 属性界面时长（秒）
// ====================================================================
class UIRefreshTimeSettingWidget : public SettingWidget
{
    Q_OBJECT

public:
    explicit UIRefreshTimeSettingWidget(QWidget *parent = nullptr);
    ~UIRefreshTimeSettingWidget();

private slots:
    void onSetBtnClicked();
    void onSetAllBtnClicked();

private:
    void initUI();
    void initQrcodeItem();
    void initLogScreenItem();
    void initPropertyScreenItem();

    void submitTask(const QStringList &qrcodes, int logSec, int propSec);

private:
    QSpinBox *m_qrcodeSpinBox  = nullptr;
    QSpinBox *m_logSecSpinBox  = nullptr;
    QSpinBox *m_propSecSpinBox = nullptr;

    SettingItemWidget *m_qrcodeItem    = nullptr;
    SettingItemWidget *m_logScreenItem = nullptr;
    SettingItemWidget *m_propScreenItem = nullptr;
};

#endif // UIREFRESHTIMESETTINGWIDGET_H
