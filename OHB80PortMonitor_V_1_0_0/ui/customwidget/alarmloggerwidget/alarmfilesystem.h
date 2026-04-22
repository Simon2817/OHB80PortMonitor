#pragma once
#include <QString>
#include <QStringList>
#include <QHash>
#include "alarmcsvio.h"
#include "alarminfo.h"
#include "alarmheaderconfig.h"

// ====================================================================
// AlarmFileSystem — 警报文件持久化系统
//
// 职责：
//   1. 将 AlarmInfo 写入当日 CSV 文件
//   2. 根据 alarmId 修改对应记录的解决状态
//
// CSV 表头固定为：
//   警报级别 | 发送时间 | QRCode | 警报ID | 是否解决 | 解决时间 | 具体信息
//   Level | SendTime | QRCode | AlarmId | Resolved | ResolveTime | Message
//
// modifyAlarm 通过扫描当日年月目录，匹配文件名前缀 "DD_" 定位 CSV 文件，
// 无需内存映射，重启后仍可正常修改当日记录。
// ====================================================================
class AlarmFileSystem
{
public:
    AlarmFileSystem();
    explicit AlarmFileSystem(const AlarmHeaderConfig &config);

    // 设置日志根目录（转发给内部 AlarmCsvIO）
    void setRootDir(const QString &dir);
    QString rootDir() const;

    // 设置表头
    void setHeaders(const QStringList &headers);
    QStringList headers() const;

    // 写入一条警报记录（发送时间自动取当前时刻，默认标记为未解决）
    bool writeAlarm(const AlarmInfo &info);

    // 读取指定日期全部记录（第一行为表头）
    QList<QStringList> readDate(const QDate &date) const;

    // 返回 rootDir 下所有有日志文件的日期集合
    QSet<QDate> availableDates() const;

    // 清理过期月份目录：保留最近 maxMonths 个月，删除更早的月份目录。
    // 返回实际删除的月份目录数量。
    int purgeOldLogs(int maxMonths = 6);

    // 通用字段修改接口
    //
    // 参数：
    //   alarmId  — 目标警报的 ID，作为行定位索引（对应 CSV 中 "AlarmId" 列）
    //   sendTime — 该警报的发送时间（"yyyy-MM-dd HH:mm:ss"），用于定位日志文件
    //   changes  — 需要修改的字段映射，格式为 { 表头名 -> 新值 }，可同时修改多列
    //
    // 返回值：
    //   true  — 找到文件且成功修改
    //   false — 文件不存在或记录未找到
    //
    // 示例：
    //   fs.modifyAlarm(id, sendTime, {{"Resolved", "Yes"}, {"ResolveTime", "..."}});
    bool modifyAlarm(qint64 alarmId, const QString &sendTime,
                     const QHash<QString, QString> &changes);

private:
    // 根据日期定位当天唯一的 CSV 文件路径（每天仅一个文件）。
    // 若年月目录或当日文件不存在则自动创建。
    QString filePathForDate(const QDate &date) const;

    AlarmCsvIO m_csv;
};
