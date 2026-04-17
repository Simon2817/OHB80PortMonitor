#include "metatypes.h"
#include <QDebug>
#include "modbusconnecter.h"

void MetaTypes::registerTypes()
{
    // 注册连接状态类型
    qRegisterMetaType<ModbusConnecter::ConnectionStatus>("ConnectionStatus");

    qDebug() << "All meta types registered successfully";
}
