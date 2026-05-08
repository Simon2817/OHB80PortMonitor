#ifndef OPERATIONRECORD_H
#define OPERATIONRECORD_H

#include <QString>
#include <QMetaType>

// ====================================================================
// OperationRecord —— 与 operation_log 表结构对齐的纯数据记录类
//
// 表结构：
//   id INTEGER PRIMARY KEY AUTOINCREMENT,
//   occur_time TEXT NOT NULL,
//   log_type INTEGER NOT NULL,
//   description TEXT NOT NULL,
//   user_permission INTEGER NOT NULL DEFAULT 0
// ====================================================================
struct OperationRecord
{
    int     id              = 0;     // 主键
    QString occurTime;               // 发生时间 "yyyy-MM-dd HH:mm:ss"
    int     logType         = 0;     // 日志类型（LogType：0=Message, 1=Warn, 2=Error）
    QString description;             // 操作描述
    int     userPermission  = 0;     // 触发该操作的用户权限（UserPermission）

    void reset()
    {
        id = 0;
        occurTime.clear();
        logType = 0;
        description.clear();
        userPermission = 0;
    }
};

Q_DECLARE_METATYPE(OperationRecord)

#endif // OPERATIONRECORD_H
