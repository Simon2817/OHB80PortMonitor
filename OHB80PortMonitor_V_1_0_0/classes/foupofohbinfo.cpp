#include "foupofohbinfo.h"

FoupOfOHBInfo::FoupOfOHBInfo()
    : qrCode(""),
    portId(0),
    ip("169.254.173.207"),
    port(8899),
    inletPressure(0),
    outletPressure(0),
    inletFlow(0),
    temperature(0),
    RH(0),
    startTime(QTime::currentTime()),
    purgeTimeMs(0),
    idleTimeMs(0),
    foupIn(false),
    oldFoupIn(false),
    hasAlarm(false),
    alarmId(""),
    idleState(IdleState::Stopped)
{

}

FoupOfOHBInfo::FoupOfOHBInfo(const FoupOfOHBInfo &other)
    : qrCode(other.qrCode),
      portId(other.portId),
      ip(other.ip),
      port(other.port),
      inletPressure(other.inletPressure),
      outletPressure(other.outletPressure),
      inletFlow(other.inletFlow),
      temperature(other.temperature),
      RH(other.RH),
      startTime(other.startTime),
      purgeTimeMs(other.purgeTimeMs),
      idleTimeMs(other.idleTimeMs),
      foupIn(other.foupIn),
      oldFoupIn(other.oldFoupIn),
      hasAlarm(other.hasAlarm),
      alarmId(other.alarmId),
      idleState(other.idleState)
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
        outletPressure = other.outletPressure;
        inletFlow = other.inletFlow;
        temperature = other.temperature;
        RH = other.RH;
        startTime = other.startTime;
        purgeTimeMs = other.purgeTimeMs;
        idleTimeMs = other.idleTimeMs;
        foupIn = other.foupIn;
        oldFoupIn = other.oldFoupIn;
        hasAlarm = other.hasAlarm;
        alarmId = other.alarmId;
        idleState = other.idleState;
    }
    return *this;
}

bool FoupOfOHBInfo::isVisibel()
{
    if (qrCode.isEmpty() || portId <= 0)
        return false;
    return true;
}
