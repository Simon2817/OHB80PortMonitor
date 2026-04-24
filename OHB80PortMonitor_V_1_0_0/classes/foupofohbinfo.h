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
    double inletPressure;         // 进气主路气压值 (CH_1, 0~10 Bar)
    double negativePressure;      // 负压值 (CH_2)
    double outletPressure;        // 出口压力（暂未使用）
    double inletFlow;             // 进气主路流量值 (CH_3, 4~200 L/Min)
    double temperature;           // 真空回路气体温度 (CH_5, 0~100℃)
    double RH;                    // 真空回路相对湿度 (CH_4, 0~100%)
    QTime startTime;              // 开始时间
    quint32 purgeTimeSec;         // FOUP IN 充气秒数 (CH_7<<16|CH_8)
    quint16 idleWorkingTimeSec;   // IdlePurge 工作计时时间（秒）
    bool foupIn;                  // FOUP 是否在位 (CH_6)
    bool oldFoupIn;               // FOUP 上一次在位状态
    bool idlePurgeEnabled;        // IdlePurge 使能状态 (0=关闭, 1=开启)
    bool hasAlarm;                // 警报状态
    QString alarmId;              // 警报 ID
    IdleState idleState;          // IdlePurge 状态 (0=空闲,1=准备,2=充气,3=充气间隔)
};


#endif // FOUPOFOHBINFO_H
