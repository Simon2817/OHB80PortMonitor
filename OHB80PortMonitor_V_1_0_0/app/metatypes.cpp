#include "metatypes.h"
#include <QDebug>
#include <QVariantMap>
#include "modbusconnecter.h"
#include "modbuscommand/modbuscommand.h"
#include "modbustcpmastermanager/modbustcpmaster/firmwareupgrader.h"
#include "loggerwidget/pagetable.h"
#include "tasks/sh85_self_check_task.h"
#include "tasks/read_vefc_flow_unit_medium_status_task.h"
#include "logdatabases/dbtypes.h"

void MetaTypes::registerTypes()
{
    // 注册连接状态类型
    qRegisterMetaType<ModbusConnecter::ConnectionStatus>("ConnectionStatus");

    // 注册 ModbusCommand 类型，用于 PeriodicCommandSender::commandCompleted 跨线程信号
    qRegisterMetaType<ModbusCommand>("ModbusCommand");

    // 注册 FirmwareUpgrader::UpgradeState，用于 deviceStateLog 等跨线程信号
    qRegisterMetaType<FirmwareUpgrader::UpgradeState>("FirmwareUpgrader::UpgradeState");

    // 注册 Page 类型，用于 LogFileSystem::pageReady 跨线程信号
    qRegisterMetaType<Page>("Page");

    // 注册 SH85SelfCheckTask::Result，用于 SH85SelfCheckTask::allFinished 跨线程信号
    qRegisterMetaType<SH85SelfCheckTask::Result>("SH85SelfCheckTask::Result");

    // 注册 ReadVEFCFlowUnitAndMediumStatusTask::DeviceStatus，用于 allFinished 跨线程信号
    qRegisterMetaType<ReadVEFCFlowUnitAndMediumStatusTask::DeviceStatus>("ReadVEFCFlowUnitAndMediumStatusTask::DeviceStatus");
    qRegisterMetaType<QList<ReadVEFCFlowUnitAndMediumStatusTask::DeviceStatus>>("QList<ReadVEFCFlowUnitAndMediumStatusTask::DeviceStatus>");

    // 注册 LogDB::WriteResult，用于 WriteSqlDBCon::taskCompleted 跨线程信号
    qRegisterMetaType<LogDB::WriteResult>("LogDB::WriteResult");
    qRegisterMetaType<QList<QVariantMap>>("QList<QVariantMap>");

    qDebug() << "All meta types registered successfully";
}
