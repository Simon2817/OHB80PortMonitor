#ifndef FOUPOFOHBINFO_H
#define FOUPOFOHBINFO_H

#include <QTime>
#include <QString>

class FoupOfOHBInfo
{
public:
    FoupOfOHBInfo();
    FoupOfOHBInfo(const FoupOfOHBInfo &other);
    FoupOfOHBInfo &operator=(const FoupOfOHBInfo &other);
    ~FoupOfOHBInfo() = default;

    bool isVisibel();

    QString qrCode;
    int portId;
    QString ip;
    quint16 port;
    double inletPressure;
    double inletFlow;
    double RH;
    QTime startTime;  // 格式：08:22:35.324
    quint32 purgeTimeMs;  // 记录毫秒
    quint32 idleTimeMs;   // 记录毫秒
    bool foupIn;          // Foup 是否在位
    bool hasAlarm;        // 警报状态
    QString alarmId;      // 警报 ID
};


#endif // FOUPOFOHBINFO_H
