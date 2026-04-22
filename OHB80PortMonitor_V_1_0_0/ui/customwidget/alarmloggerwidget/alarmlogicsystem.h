#pragma once
#include <QHash>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QSet>
#include "alarminfo.h"
#include "alarmfilesystem.h"
#include "alarmheaderconfig.h"

// ====================================================================
// AlarmLogicSystem — 警报逻辑处理器
//
// 职责：
//   1. 接收警报提交，按 alarmId 去重（相同 ID 已存在则忽略）
//   2. 新警报经验证后通过 alarmPublished 信号向外广播
//   3. 接收解决提交，清理本地记录并通过 alarmResolved 信号广播
//
// 其他控件可直接连接 alarmPublished / alarmResolved 信号以获取推送。
// ====================================================================
class AlarmLogicSystem : public QObject
{
    Q_OBJECT
public:
    explicit AlarmLogicSystem(QObject *parent = nullptr);
    explicit AlarmLogicSystem(const AlarmHeaderConfig &config, QObject *parent = nullptr);

    // 
    void setRootDir(const QString &dir);
    QString rootDir() const;

    // 
    void setHeaders(const QStringList &headers);
    QStringList headers() const;

    // 提交一条警报；若 alarmId 已在活跃列表中则忽略（去重）
    void submitAlarm(AlarmLevel level, const QString &qrCode,
                     qint64 alarmId, const QString &message);

    // 提交警报解决；若 alarmId 存在则清理并发布 alarmResolved
    void submitResolve(qint64 alarmId);

    // 查询某条警报是否仍处于活跃（未解决）状态
    bool isActive(qint64 alarmId) const;

    // 当前活跃警报数量
    int activeCount() const;

    // 清空活跃警报列表（跨日重置时调用）
    void clearActive();

    // 读取指定日期全部记录（第一行为表头，同步）
    QList<QStringList> readDate(const QDate &date) const;

    // 返回所有有日志文件的日期集合（同步，仅扫描目录名，很快）
    QSet<QDate> availableDates() const;

    // ---- 异步接口（QtConcurrent，结果通过信号返回）----

    // 异步写入：结果通过 writeAlarmFinished(bool) 通知
    void writeAlarmAsync(const AlarmInfo &info);

    // 异步读取：结果通过 readDateFinished(rows) 通知
    void readDateAsync(const QDate &date);

    // 异步扫描可用日期：结果通过 availableDatesReady(dates) 通知
    void availableDatesAsync();

    // 异步清理过期月份：结果通过 purgeFinished(deletedCount) 通知
    void purgeOldLogsAsync(int maxMonths = 6);

signals:
    // 新警报发布（已通过去重验证）
    void alarmPublished(const AlarmInfo &info);

    // 警报已解决（携带完整警报信息）
    void alarmResolved(const AlarmInfo &info);

    // 异步 I/O 结果信号
    void writeAlarmFinished(bool success);
    void readDateFinished(const QList<QStringList> &rows);
    void availableDatesReady(const QSet<QDate> &dates);
    void purgeFinished(int deletedCount);

private:
    AlarmFileSystem          m_fs;
    QHash<qint64, AlarmInfo> m_active;
    mutable QMutex           m_fsMutex;
};
