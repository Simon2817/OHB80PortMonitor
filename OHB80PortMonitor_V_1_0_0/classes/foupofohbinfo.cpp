#include "foupofohbinfo.h"

FoupOfOHBInfo::FoupOfOHBInfo()
    : qrCode(""),
    portId(0),
    ip("169.254.173.207"),
    port(8899),
    inletPressure(0),
    inletFlow(0),
    RH(0),
    startTime(QTime::currentTime()),
    purgeTimeMs(0),
    idleTimeMs(0),
    foupIn(false),
    hasAlarm(false),
    alarmId("")
{

}

FoupOfOHBInfo::FoupOfOHBInfo(const FoupOfOHBInfo &other)
    : qrCode(other.qrCode),
      portId(other.portId),
      ip(other.ip),
      port(other.port),
      inletPressure(other.inletPressure),
      inletFlow(other.inletFlow),
      RH(other.RH),
      startTime(other.startTime),
      purgeTimeMs(other.purgeTimeMs),
      idleTimeMs(other.idleTimeMs),
      foupIn(other.foupIn),
      hasAlarm(other.hasAlarm),
      alarmId(other.alarmId)
{
}

FoupOfOHBInfo &FoupOfOHBInfo::operator=(const FoupOfOHBInfo &other)
{
    if (this != &other)
    {
        qrCode = other.qrCode;
        portId = other.portId;
        ip = other.ip;
        port = other.port;
        inletPressure = other.inletPressure;
        inletFlow = other.inletFlow;
        RH = other.RH;
        startTime = other.startTime;
        purgeTimeMs = other.purgeTimeMs;
        idleTimeMs = other.idleTimeMs;
        foupIn = other.foupIn;
        hasAlarm = other.hasAlarm;
        alarmId = other.alarmId;
    }
    return *this;
}

bool FoupOfOHBInfo::isVisibel()
{
    if (qrCode.isEmpty() || portId <= 0)
        return false;
    return true;
}
