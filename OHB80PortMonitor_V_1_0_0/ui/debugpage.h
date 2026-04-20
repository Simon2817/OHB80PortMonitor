#ifndef DEBUGPAGE_H
#define DEBUGPAGE_H

#include <QWidget>

namespace Ui {
class DebugPage;
}

class FirmwareUpdateConfigSettingWidget;
class FirmwareUpdateSettingWidget;

class DebugPage : public QWidget
{
    Q_OBJECT

public:
    explicit DebugPage(QWidget *parent = nullptr);
    ~DebugPage();

    void initUI();

    // 导航栏
    void initNav();

private slots:
    void navBtnClicked();

private:
    Ui::DebugPage *ui;
    FirmwareUpdateConfigSettingWidget *m_firmwareConfigWidget;
    FirmwareUpdateSettingWidget       *m_firmwareUpdateWidget;
};

#endif // DEBUGPAGE_H
