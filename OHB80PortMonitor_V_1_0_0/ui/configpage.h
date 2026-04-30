#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QWidget>

namespace Ui {
class ConfigPage;
}

class IdlePurgeSettingWidget;
class PneumaticValvePressureSettingWidget;
class SH85SelfCheckSettingWidget;
class HumidityOffsetSettingWidget;
class PurgeFlowSettingWidget;

class ConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPage(QWidget *parent = nullptr);
    ~ConfigPage();

    void initUI();

    // 导航栏
    void initNav();

private slots:
    void navBtnClicked();

private:
    Ui::ConfigPage *ui;
    IdlePurgeSettingWidget *m_idlePurgeWidget;
    PneumaticValvePressureSettingWidget *m_pneumaticValvePressureWidget;
    SH85SelfCheckSettingWidget *m_sh85SelfCheckWidget;
    HumidityOffsetSettingWidget *m_humidityOffsetWidget;
    PurgeFlowSettingWidget *m_purgeFlowWidget;
};

#endif // CONFIGPAGE_H
