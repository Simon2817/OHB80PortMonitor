#include "set_firmware_config_task.h"
#include "modbus_tcp_master_manager.h"
#include "modbus_tcp_master.h"
#include "ModbusTcpMaster/mtm_firmware_upgrader.h"

#include <QDebug>

SetFirmwareConfigTask::SetFirmwareConfigTask(QObject *parent)
    : SchedulerTask(parent)
{}

void SetFirmwareConfigTask::setPrepareTimeout(int ms)  { m_prepareTimeout = ms; }
void SetFirmwareConfigTask::setWaitingTime(int ms)     { m_waitingTime = ms; }
void SetFirmwareConfigTask::setSendInterval(int ms)    { m_sendInterval = ms; }
void SetFirmwareConfigTask::setTransferTimeout(int ms) { m_transferTimeout = ms; }

void SetFirmwareConfigTask::start()
{
    setState(Running);

    ModbusTcpMasterManager *mgr = ModbusTcpMasterManager::instance();
    const QStringList ids = mgr->masterIds();

    for (const QString &id : ids) {
        ModbusTcpMaster *master = mgr->getMaster(id);
        if (!master) continue;
        MtmFirmwareUpgrader *upgrader = master->firmwareUpgrader();
        if (!upgrader) continue;

        if (m_prepareTimeout.has_value())  upgrader->setPrepareTimeout(m_prepareTimeout.value());
        if (m_waitingTime.has_value())     upgrader->setWaitingTime(m_waitingTime.value());
        if (m_sendInterval.has_value())    upgrader->setSendInterval(m_sendInterval.value());
        if (m_transferTimeout.has_value()) upgrader->setTransferTimeout(m_transferTimeout.value());

        qDebug() << "[SetFirmwareConfigTask] Applied config to" << id;
    }

    if (m_prepareTimeout.has_value())  emit prepareTimeoutApplied();
    if (m_waitingTime.has_value())     emit waitingTimeApplied();
    if (m_sendInterval.has_value())    emit sendIntervalApplied();
    if (m_transferTimeout.has_value()) emit transferTimeoutApplied();

    setState(Finished);
    emit finished(true, "Firmware config applied");
}

void SetFirmwareConfigTask::stop()
{
    setState(Cancelled);
}
