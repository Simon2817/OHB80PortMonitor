#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <QVector>
#include <QStringList>
#include <QSharedPointer>
#include "classes/setofohbinfo.h"
#include "classes/foupofohbinfo.h"

class NetworkStatusTask;
class MonitorDataTask;
class AlarmDispatchTask;
class RunningLoggerTask;

class SharedData
{
public:
    SharedData();
    
    static QVector<SetOfOHBInfo> setOfOHBInfoList;
    
    // 根据 uiId 获取 SetOfOHBInfo 对象
    static QSharedPointer<SetOfOHBInfo> getSetOfOHBInfoByUiId(int uiId);
    
    // 根据 qrCode 获取 FoupOfOHBInfo 指针（用于直接修改）
    static FoupOfOHBInfo* getFoupByQRCode(const QString& qrCode);

    // 收集所有设备的 qrCode 列表
    static QStringList getAllQrcodes();

    // 初始化调度器并启动常驻任务
    static void initScheduler();

    // 获取网络状态监控任务
    static NetworkStatusTask* getNetworkStatusTask();

    // 获取监控数据任务
    static MonitorDataTask* getMonitorDataTask();

    // 获取警报调度任务（所有 submitAlarm / submitResolve 入口）
    static AlarmDispatchTask* getAlarmDispatchTask();

    // 获取运行日志采集任务（logMessage / logWarn / logError 入口）
    static RunningLoggerTask* getRunningLoggerTask();

private:
    static bool s_modbusManagerInitialized;
    static NetworkStatusTask* s_networkStatusTask;
    static MonitorDataTask* s_monitorDataTask;
    static AlarmDispatchTask* s_alarmDispatchTask;
    static RunningLoggerTask* s_runningLoggerTask;
};

#endif // SHAREDDATA_H
