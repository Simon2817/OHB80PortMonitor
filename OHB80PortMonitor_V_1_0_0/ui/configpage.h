#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QWidget>

namespace Ui {
class ConfigPage;
}

class IdlePurgeSettingWidget;

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
};

#endif // CONFIGPAGE_H
