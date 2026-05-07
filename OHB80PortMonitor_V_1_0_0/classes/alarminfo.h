#ifndef ALARMINFO_H
#define ALARMINFO_H

#include <QString>
#include "alarmtype.h"


// 警报信息结构体
struct AlarmInfo
{
    int id;                          // 数据库记录ID
    int alarmLevel;                  // 警报级别 (AlarmLevel: Warn=1, Error=2, Fatal=3)
    int alarmType;                   // 警告类型 (AlarmType枚举值)
    int alarmSource;                 // 警报来源 (AlarmSource: Device=1, User=2, System=3)
    QString sourceIdentifier;        // 来源标识（设备qrcode或用户名等）
    QString alarmId;                 // 警报ID（根据级别+来源+类型生成）
    QString occurTime;               // 警报发生时间
    QString resolvedTime;            // 警报解决时间（Warn级别可为空）
    bool isResolved;                 // 是否已解决
    bool customerVisible;            // 客户是否可见（0:不可见 1:可见）
    QString description;             // 警报描述信息

    // 默认构造函数
    AlarmInfo()
        : id(0)
        , alarmLevel(0)
        , alarmType(0)
        , alarmSource(0)
        , isResolved(false)
        , customerVisible(true)
    {}

    // 生成警报ID
    // 格式：【警报级别(1位)】+【来源标识(5位)】+【警告类型(4位)】
    QString generateAlarmId() const;

    // 重置所有字段
    void reset()
    {
        id = 0;
        alarmLevel = 0;
        alarmType = 0;
        alarmSource = 0;
        sourceIdentifier.clear();
        alarmId.clear();
        occurTime.clear();
        resolvedTime.clear();
        isResolved = false;
        customerVisible = true;
        description.clear();
    }

    // 检查是否有效
    bool isValid() const
    {
        return id > 0 && alarmLevel > 0 && alarmType > 0 && alarmSource > 0;
    }
};

#endif // ALARMINFO_H
