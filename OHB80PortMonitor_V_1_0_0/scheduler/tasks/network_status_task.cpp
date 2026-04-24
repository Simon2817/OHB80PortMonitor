#include "network_status_task.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include "modbustcpmastermanager/modbustcpmaster/modbusconnecter.h"
#include "app/shareddata.h"
#include "app/applogger.h"
#include "loggermanager.h"
#include "classes/foupofohbinfo.h"
#include "ui/customwidget/alarmloggerwidget/alarmid.h"

#include <QDebug>

NetworkStatusTask::NetworkStatusTask(QObject *parent)
    : SchedulerTask(parent)
{
    qDebug() << "=============================NetworkStatusTask 调度任务开始=============================";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        "=============================NetworkStatusTask 调度任务开始=============================");
}

NetworkStatusTask::~NetworkStatusTask()
{
    qDebug() << "=============================NetworkStatusTask 调度任务结束=============================";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        "=============================NetworkStatusTask 调度任务结束=============================");
}

void NetworkStatusTask::start()
{
    setState(Running);
    m_stopped = false;
    m_totalCount = 0;
    m_lastStatusMap.clear();
    m_connections.clear();

    // 在启动设备前，先创建并启动初始化检查任务
    m_initCheckTask = new InitCheckTask(this);
    connect(m_initCheckTask, &InitCheckTask::allFinished,
            this, &NetworkStatusTask::onInitCheckFinished,
            Qt::QueuedConnection);
    m_initCheckTask->start();

    ModbusTcpMasterManager &manager = ModbusTcpMasterManager::instance();
    QStringList ids = manager.masterIds();

    // 启动所有 ModbusTcpMaster
    for (const QString &id : ids) {
        ModbusTcpMaster *master = manager.getMaster(id);
        QString ipPortStr;
        if (master) {
            ipPortStr = QString("%1:%2").arg(master->ip()).arg(master->port());
        }
        manager.startMaster(id, ModbusConnecter::ConnectionMode::AutoReconnect);
        qDebug() << "[Scheduler][NetworkStatusTask] 设备" << id << "(" << ipPortStr << ") 已启动自动重连";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
            QString("[Scheduler][NetworkStatusTask] 设备 %1 (%2) 已启动自动重连").arg(id).arg(ipPortStr).toStdString());
    }

    for (const QString &id : ids) {
        ModbusTcpMaster *master = manager.getMaster(id);
        if (!master) continue;

        QString ipPortStr = QString("%1:%2").arg(master->ip()).arg(master->port());

        ModbusConnecter *connecter = master->connector();
        if (!connecter) {
            qWarning() << "[Scheduler][NetworkStatusTask] 设备" << id << "(" << ipPortStr << ") 的 ModbusConnecter 为空，跳过";
            LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
                QString("[Scheduler][NetworkStatusTask] 设备 %1 (%2) 的 ModbusConnecter 为空，跳过").arg(id).arg(ipPortStr).toStdString());
            continue;
        }

        // 记录初始状态，并立即根据当前状态同步告警
        // （部分设备可能在信号连接之前就已完成连接，需在此初始化，避免信号丢失）
        ModbusConnecter::ConnectionStatus currentStatus = connecter->getStatus();
        m_lastStatusMap[id] = currentStatus;

        FoupOfOHBInfo* foup = SharedData::getFoupByQRCode(id);
        if (foup) {
            if (currentStatus == ModbusConnecter::ConnectionStatus::Connected) {
                foup->hasAlarm = false;
                foup->alarmId.clear();
                qDebug() << "[Scheduler][NetworkStatusTask] 设备" << id << "(" << ipPortStr << ") 初始状态已连接，告警已清除";
            } else {
                foup->hasAlarm = true;
                foup->alarmId = "111";
                qDebug() << "[Scheduler][NetworkStatusTask] 设备" << id << "(" << ipPortStr << ") 初始状态未连接，设置告警";
            }
        }

        // 监听连接状态变更信号
        auto conn = connect(connecter, &ModbusConnecter::statusChanged,
                            this, &NetworkStatusTask::onStatusChanged,
                            Qt::QueuedConnection);
        m_connections.append(conn);
        m_totalCount++;
    }

    if (m_totalCount == 0) {
        setState(Failed);
        emit finished(false, "NetworkStatusTask: 没有可监听的 ModbusConnecter");
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            "[Scheduler][NetworkStatusTask] 没有可监听的 ModbusConnecter");
        return;
    }

    qDebug() << "[Scheduler][NetworkStatusTask] 启动，监听" << m_totalCount << "个设备的网络连接状态";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][NetworkStatusTask] 启动，监听 %1 个设备的网络连接状态").arg(m_totalCount).toStdString());
    emit progress(0, QString("开始监控 %1 个设备的网络连接状态").arg(m_totalCount));
}

void NetworkStatusTask::stop()
{
    m_stopped = true;
    disconnectAll();
    setState(Cancelled);
    emit finished(false, "网络状态监控任务被取消");
    qDebug() << "[Scheduler][NetworkStatusTask] 已停止";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        "[Scheduler][NetworkStatusTask] 已停止");
}

void NetworkStatusTask::onStatusChanged(ModbusConnecter::ConnectionStatus status, const QString &masterId)
{
    if (m_stopped) return;

    // 获取上一次状态
    ModbusConnecter::ConnectionStatus lastStatus = m_lastStatusMap.value(masterId, ModbusConnecter::ConnectionStatus::Disconnected);
    m_lastStatusMap[masterId] = status;

    // 状态未改变则跳过
    if (status == lastStatus) return;

    // 发射状态变更信号供外部使用
    emit statusChanged(status, masterId);

    QString statusStr;
    switch (status) {
        case ModbusConnecter::ConnectionStatus::Disconnected: statusStr = "已断开"; break;
        case ModbusConnecter::ConnectionStatus::Connecting:   statusStr = "连接中"; break;
        case ModbusConnecter::ConnectionStatus::Connected:    statusStr = "已连接"; break;
        case ModbusConnecter::ConnectionStatus::Error:        statusStr = "错误";   break;
    }

    // 获取 IP 和端口信息
    ModbusTcpMasterManager &manager = ModbusTcpMasterManager::instance();
    ModbusTcpMaster *master = manager.getMaster(masterId);
    QString ipPortStr;
    if (master) {
        ipPortStr = QString("%1:%2").arg(master->ip()).arg(master->port());
    }

    qDebug() << "[Scheduler][NetworkStatusTask] 设备" << masterId << "(" << ipPortStr << ") 连接状态变更:" << statusStr;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][NetworkStatusTask] 设备 %1 (%2) 连接状态变更: %3").arg(masterId).arg(ipPortStr).arg(statusStr).toStdString());

    // 当连接断开或出错时，设置告警
    FoupOfOHBInfo* foup = SharedData::getFoupByQRCode(masterId);
    if (!foup) {
        qWarning() << "[Scheduler][NetworkStatusTask] 未找到设备" << masterId << "(" << ipPortStr << ") 对应的 FoupOfOHBInfo";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][NetworkStatusTask] 未找到设备 %1 (%2) 对应的 FoupOfOHBInfo").arg(masterId).arg(ipPortStr).toStdString());
        return;
    }

    if (status == ModbusConnecter::ConnectionStatus::Connected) {
        // 连接成功：清除告警
        foup->hasAlarm = false;
        foup->alarmId.clear();
        qDebug() << "[Scheduler][NetworkStatusTask] 设备" << masterId << "(" << ipPortStr << ") 连接成功，告警已清除";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
            QString("[Scheduler][NetworkStatusTask] 设备 %1 (%2) 连接成功，告警已清除").arg(masterId).arg(ipPortStr).toStdString());
    } else {
        // 断开或出错：设置告警
        foup->hasAlarm = true;
        foup->alarmId = alarmIdToString(makeAlarmId(foup->qrCode.toInt(), AlarmCode::SoftwareConnectionLost));
        foup->startTime = QTime(0, 0, 0);
        qDebug() << "[Scheduler][NetworkStatusTask] 设备" << masterId << "(" << ipPortStr << ") 连接异常，已设置告警 alarmId=" << foup->alarmId;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][NetworkStatusTask] 设备 %1 (%2) 连接异常，已设置告警 alarmId=%3").arg(masterId).arg(ipPortStr).arg(foup->alarmId).toStdString());
    }
}

void NetworkStatusTask::onInitCheckFinished(bool allSuccess, int successCount, int failCount,
                                             const QStringList &failedMasterIds)
{
    qDebug() << "[Scheduler][NetworkStatusTask] InitCheckTask 已完成，释放任务";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][NetworkStatusTask] InitCheckTask 完成: 总=%1, 成功=%2, 失败=%3")
            .arg(successCount + failCount).arg(successCount).arg(failCount).toStdString());

    // 释放 InitCheckTask
    if (m_initCheckTask) {
        m_initCheckTask->deleteLater();
        m_initCheckTask = nullptr;
    }

    // 转发汇总信号给外部
    emit allInitFinished(allSuccess, successCount, failCount, failedMasterIds);
}

void NetworkStatusTask::disconnectAll()
{
    for (const QMetaObject::Connection &conn : qAsConst(m_connections))
        QObject::disconnect(conn);
    m_connections.clear();
}
