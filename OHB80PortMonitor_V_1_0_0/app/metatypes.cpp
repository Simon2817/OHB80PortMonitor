#include "metatypes.h"
#include <QDebug>
#include "modbusconnecter.h"
#include "modbuscommand/modbuscommand.h"
#include "modbustcpmastermanager/modbustcpmaster/firmwareupgrader.h"
#include "loggerwidget/pagetable.h"

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

    qDebug() << "All meta types registered successfully";
}
