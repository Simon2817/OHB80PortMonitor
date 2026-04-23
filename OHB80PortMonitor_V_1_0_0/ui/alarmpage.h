#ifndef ALARMPAGE_H
#define ALARMPAGE_H

#include <QWidget>
#include "modbusconnecter.h"

namespace Ui {
class AlarmPage;
}

class AlarmLoggerWidget;

class AlarmPage : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmPage(QWidget *parent = nullptr);
    ~AlarmPage();

    AlarmLoggerWidget* alarmLogger() const;

private slots:
    void onNetworkStatusChanged(ModbusConnecter::ConnectionStatus status, const QString &masterId);

private:
    Ui::AlarmPage *ui;
};

#endif // ALARMPAGE_H
