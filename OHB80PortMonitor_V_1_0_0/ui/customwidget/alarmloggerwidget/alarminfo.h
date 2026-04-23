#pragma once
#include <QDateTime>
#include <QString>
#include <QStringList>
#include "alarmid.h"

// ====================================================================
// AlarmInfo — 警报完整信息载体
//
// 字段与 CSV 表头一一对应：
//   警报级别 | 发送时间 | QRCode | 警报ID | 是否解决 | 解决时间 | 具体信息
// ====================================================================
class AlarmInfo
{
public:
    AlarmInfo() = default;

    // 构造新警报（发送时间自动取当前时刻，默认未解决）
    AlarmInfo(AlarmLevel level, const QString &qrCode,
              qint64 alarmId, const QString &message)
        : m_level(level)
        , m_sendTime(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
        , m_qrCode(qrCode)
        , m_alarmId(alarmId)
        , m_resolved(false)
        , m_resolveTime()
        , m_message(message)
    {}

    AlarmLevel level()       const { return m_level; }
    QString    sendTime()    const { return m_sendTime; }
    QString    qrCode()      const { return m_qrCode; }
    qint64     alarmId()     const { return m_alarmId; }
    bool       isResolved()  const { return m_resolved; }
    QString    resolveTime() const { return m_resolveTime; }
    QString    message()     const { return m_message; }

    void setResolved(bool v)              { m_resolved = v; }
    void setResolveTime(const QString &t) { m_resolveTime = t; }
    void setMessage(const QString &msg)   { m_message = msg; }

    // 转 CSV 行，列顺序与 AlarmFileSystem::kHeaders 严格对应
    QStringList toStringList() const
    {
        return {
            alarmLevelToString(m_level),
            m_sendTime,
            m_qrCode,
            alarmIdToString(m_alarmId),
            m_resolved ? QString("Yes") : QString("No"),
            m_resolveTime,
            m_message
        };
    }

private:
    AlarmLevel m_level       = AlarmLevel::Warn;
    QString    m_sendTime;
    QString    m_qrCode;
    qint64     m_alarmId     = 0;
    bool       m_resolved    = false;
    QString    m_resolveTime;
    QString    m_message;
};
