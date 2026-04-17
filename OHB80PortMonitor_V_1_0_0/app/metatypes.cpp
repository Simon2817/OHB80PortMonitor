#include "metatypes.h"
#include <QDebug>
#include "modbusconnecter.h"
#include "modbuscommand/modbuscommand.h"

void MetaTypes::registerTypes()
{
    // 注册连接状态类型
    qRegisterMetaType<ModbusConnecter::ConnectionStatus>("ConnectionStatus");

    // 注册 ModbusCommand 类型，用于 PeriodicCommandSender::commandCompleted 跨线程信号
    qRegisterMetaType<ModbusCommand>("ModbusCommand");

    qDebug() << "All meta types registered successfully";
}
