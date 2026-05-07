#include "alarmpage.h"
#include "ui_alarmpage.h"
#include "app/shareddata.h"
#include "app/alarmtype.h"
#include "scheduler/tasks/network_status_task.h"
#include "scheduler/tasks/alarm_dispatch_task.h"
#include "modbustcpmastermanager/modbustcpmaster/modbusconnecter.h"

AlarmPage::AlarmPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlarmPage)
{
    ui->setupUi(this);

    // 新 AlarmLogWidget 只需 initUi，所有警报生命周期与持久化都交给 AlarmDispatchTask
    ui->alarmLoggerWidget->initUi();

    // 连接网络状态任务 → 本页警报推送
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
    AlarmDispatchTask* dispatcher = SharedData::getAlarmDispatchTask();
    if (!dispatcher) return;

    const int alarmType   = static_cast<int>(AlarmType::DeviceOffline);
    const int alarmSource = static_cast<int>(AlarmSource::Device);

    if (status == ModbusConnecter::ConnectionStatus::Connected) {
        // 连接恢复：解决设备离线警报
        dispatcher->submitResolve(alarmType, alarmSource, masterId);
    } else {
        // 连接丢失：上报设备离线警报
        dispatcher->submitAlarm(alarmType,
                                alarmSource,
                                masterId,
                                QStringLiteral("Device %1 connection lost").arg(masterId));
    }
}
