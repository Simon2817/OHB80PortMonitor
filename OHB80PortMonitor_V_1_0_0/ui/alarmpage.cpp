#include "alarmpage.h"
#include "ui_alarmpage.h"
#include "app/shareddata.h"
#include "app/customlogger.h"
#include "scheduler/tasks/network_status_task.h"
#include "modbustcpmastermanager/modbustcpmaster/modbusconnecter.h"
#include "customwidget/alarmloggerwidget/alarmid.h"

AlarmPage::AlarmPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlarmPage)
{
    ui->setupUi(this);

    // Set alarm log root directory
    ui->alarmLoggerWidget->setRootDir(CustomLogger::AlarmLoggerPath());

    // Get network status monitoring task and connect signal
    NetworkStatusTask* networkTask = SharedData::getNetworkStatusTask();
    if (networkTask) {
        connect(networkTask, &NetworkStatusTask::statusChanged,
                this, &AlarmPage::onNetworkStatusChanged,
                Qt::QueuedConnection);
    }
}

AlarmPage::~AlarmPage()
{
    delete ui;
}

void AlarmPage::onNetworkStatusChanged(ModbusConnecter::ConnectionStatus status, const QString &masterId)
{
    qint64 alarmId = makeAlarmId(masterId.toInt(), AlarmCode::SoftwareConnectionLost);

    if (status == ModbusConnecter::ConnectionStatus::Connected) {
        // Connection successful: submit error resolution
        ui->alarmLoggerWidget->submitResolve(alarmId);
    } else {
        // Connection failed or disconnected: report SoftwareConnectionLost alarm
        ui->alarmLoggerWidget->submitAlarm(AlarmLevel::Error, masterId, alarmId, "Connection Lost");
    }
}
