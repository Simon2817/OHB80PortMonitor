#include "shareddata.h"
#include "appconfig.h"
#include "networkconfig.h"
#include "qrcodeconfig.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/network_status_task.h"
#include "scheduler/tasks/monitor_data_task.h"
#include "scheduler/tasks/alarm_dispatch_task.h"
#include "scheduler/tasks/running_logger_task.h"
#include <QDebug>
#include <QHash>

QVector<SetOfOHBInfo> SharedData::setOfOHBInfoList;
bool SharedData::s_modbusManagerInitialized = false;
NetworkStatusTask* SharedData::s_networkStatusTask = nullptr;
MonitorDataTask* SharedData::s_monitorDataTask = nullptr;
AlarmDispatchTask* SharedData::s_alarmDispatchTask = nullptr;
RunningLoggerTask* SharedData::s_runningLoggerTask = nullptr;

SharedData::SharedData() {

    if (setOfOHBInfoList.isEmpty()) {
        setOfOHBInfoList.reserve(20);
        QVector<int> uiIds = {2,3,4,5,6,7,8,9,10,11,12,13,36,37,38,39,40,41,42,43};
        
        // 读取网络配置
        QVector<OHBNetworkInfo> networkInfos = AppConfig::getInstance().getNetworkConfig().readNetworkConfig();
        // 读取二维码配置
        QVector<QString> qrCodeInfos = AppConfig::getInstance().getQRCodeConfig().readQRCodeMapping();
        // 索引
        int index = 0;

        // 设置线程池为最大线程数，避免所有 Master 共用一个线程导致事件循环阻塞
        ModbusTcpMasterManager::instance().setThreadCount(ModbusTcpMasterPool::ThreadCountMode::MaxThreads);

        for (int i = 0; i < 20; ++i) {
            SetOfOHBInfo setInfo;
            setInfo.setUiId(uiIds[i]);
            
            QVector<FoupOfOHBInfo> foups;
            for (int j = 0; j < 4; ++j) {
                FoupOfOHBInfo foup;
                foup.qrCode = qrCodeInfos.at(index);
                foup.ip = networkInfos.at(index).ip;
                foup.port = networkInfos.at(index++).port;
                foup.inletPressure = 0;
                foup.inletFlow = 0;
                foup.RH = 0;
                foup.foupIn = false;
                foup.hasAlarm = true;
                foups.append(foup);

                // static int num = 0;
                // if (!foup.ip.isEmpty() && foup.port > 0 && num <= 0) {
                if (!foup.ip.isEmpty() && foup.port > 0) {
                    ModbusTcpMasterManager::instance().addMaster(foup.ip, foup.port, foup.qrCode);
                    // num++;
                }
                else {
                    qDebug() << "Invalid IP or port for foup:" << foup.qrCode;
                }
            }
            
            // 一次性设置整个 Foup 队列
            setInfo.setFoups(foups);
            setOfOHBInfoList.append(setInfo);
        }
        
        qDebug() << "SharedData initialized" << setOfOHBInfoList.size() << "OHB items from config";

    }
}

QSharedPointer<SetOfOHBInfo> SharedData::getSetOfOHBInfoByUiId(int uiId)
{
    // 遍历 setOfOHBInfoList 查找匹配的 uiId
    for (int i = 0; i < setOfOHBInfoList.size(); ++i) {
        if (setOfOHBInfoList[i].getUiId() == uiId) {
            // 创建一个指向该对象的智能指针
            // 注意：这里返回的是指向静态列表中对象的指针，生命周期由SharedData管理
            return QSharedPointer<SetOfOHBInfo>(&setOfOHBInfoList[i], [](SetOfOHBInfo*) {
                // 空的删除器，因为对象由SharedData管理，不需要删除
            });
        }
    }
    
    // 未找到时返回空指针
    return QSharedPointer<SetOfOHBInfo>(nullptr);
}

QStringList SharedData::getAllQrcodes()
{
    QStringList qrcodes;
    for (const SetOfOHBInfo& setInfo : setOfOHBInfoList) {
        for (const FoupOfOHBInfo& foup : setInfo.getFoups()) {
            if (!foup.qrCode.isEmpty()) {
                qrcodes << foup.qrCode;
            }
        }
    }
    return qrcodes;
}

FoupOfOHBInfo* SharedData::getFoupByQRCode(const QString& qrCode)
{
    // 遍历所有 SetOfOHBInfo，查找匹配的 qrCode
    for (int i = 0; i < setOfOHBInfoList.size(); ++i) {
        SetOfOHBInfo& setInfo = setOfOHBInfoList[i];
        QVector<FoupOfOHBInfo>& foups = setInfo.getFoups();
        for (int j = 0; j < foups.size(); ++j) {
            if (foups[j].qrCode == qrCode) {
                // 返回指向该 Foup 的指针
                return &foups[j];
            }
        }
    }
    
    // 未找到时返回空指针
    return nullptr;
}

void SharedData::initScheduler()
{
    // 启动调度器
    Scheduler* scheduler = Scheduler::instance();
    scheduler->start();

    // 提交网络状态监控任务（长驻任务）
    // NetworkStatusTask 内部会在设备启动前先创建并管理 InitCheckTask
    if (!s_networkStatusTask) {
        s_networkStatusTask = new NetworkStatusTask();
        scheduler->submitTask(s_networkStatusTask);
        qDebug() << "[SharedData] 已提交网络状态监控任务";
    }

    // 创建并提交监控数据任务（长驻任务）
    if (!s_monitorDataTask) {
        s_monitorDataTask = new MonitorDataTask();
        QString monitorTaskId = scheduler->submitTask(s_monitorDataTask);
        qDebug() << "[SharedData] 已提交监控数据任务, TaskID:" << monitorTaskId;
    }

    // 提交警报调度任务（长驻，取代老 AlarmLogicSystem）
    if (!s_alarmDispatchTask) {
        s_alarmDispatchTask = new AlarmDispatchTask();
        QString id = scheduler->submitTask(s_alarmDispatchTask);
        qDebug() << "[SharedData] 已提交警报调度任务, TaskID:" << id;
    }

    // 提交运行日志采集任务（长驻，取代老 RunningLoggerCollector）
    if (!s_runningLoggerTask) {
        s_runningLoggerTask = new RunningLoggerTask();
        QString id = scheduler->submitTask(s_runningLoggerTask);
        qDebug() << "[SharedData] 已提交运行日志采集任务, TaskID:" << id;
    }

    qDebug() << "[SharedData] 调度器已启动，所有常驻任务已提交";
}

NetworkStatusTask* SharedData::getNetworkStatusTask()
{
    return s_networkStatusTask;
}

MonitorDataTask* SharedData::getMonitorDataTask()
{
    return s_monitorDataTask;
}

AlarmDispatchTask* SharedData::getAlarmDispatchTask()
{
    return s_alarmDispatchTask;
}

RunningLoggerTask* SharedData::getRunningLoggerTask()
{
    return s_runningLoggerTask;
}
