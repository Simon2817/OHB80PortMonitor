#ifndef FOUPOFOHBINFO_H
#define FOUPOFOHBINFO_H

#include <QTime>
#include <QString>

// Idle 状态枚举
enum class IdleState : int {
    Stopped = 0,   // 停止工作
    Preparing = 1, // 准备阶段
    Purging = 2,   // 吹气
    Idle = 3       // 空闲
};

class FoupOfOHBInfo
{
public:
    FoupOfOHBInfo();
    FoupOfOHBInfo(const FoupOfOHBInfo &other);
    FoupOfOHBInfo &operator=(const FoupOfOHBInfo &other);
    ~FoupOfOHBInfo() = default;

    bool isVisibel();

    QString qrCode;           // 二维码（设备标识）
    int portId;               // 端口 ID
    QString ip;               // IP 地址
    quint16 port;             // 端口号
    double inletPressure;     // 入口压力
    double outletPressure;    // 出口压力
    double inletFlow;         // 入口流量
    double temperature;       // 温度
    double RH;                // 相对湿度
    QTime startTime;          // 开始时间（格式：08:22:35.324）
    quint32 purgeTimeMs;      // 吹气时间（毫秒）
    quint32 idleTimeMs;       // 空闲时间（毫秒）
    bool foupIn;              // FOUP 是否在位
    bool oldFoupIn;           // FOUP 是否在位（上一次的状态）
    bool hasAlarm;            // 警报状态
    QString alarmId;          // 警报 ID
    IdleState idleState;      // IdlePurge 状态
};


#endif // FOUPOFOHBINFO_H
