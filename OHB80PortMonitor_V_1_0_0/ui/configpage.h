#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QWidget>

namespace Ui {
class ConfigPage;
}

class ConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPage(QWidget *parent = nullptr);
    ~ConfigPage();

private:
    Ui::ConfigPage *ui;
};

#endif // CONFIGPAGE_H
