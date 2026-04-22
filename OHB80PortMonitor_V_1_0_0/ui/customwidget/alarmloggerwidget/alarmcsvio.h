#pragma once
#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QHash>
#include <QList>
#include <QSet>
#include <QString>
#include <QStringList>
#include "alarmheaderconfig.h"

// ====================================================================
// AlarmCsvIO — 警报 CSV 文件读写接口
//
// 目录结构：
//   <rootDir>/
//     YYYYMM/           ← 年月文件夹
//       DD_HHmmss.csv   ← 每次启动（或跨日）新建一个文件
//
// 典型用法：
//   AlarmCsvIO csv;
//   csv.setRootDir("log");
//   csv.setHeaders({"时间", "警报ID", "是否解决", "备注"});
//   csv.writeRecord({"2026-04-09 14:30:00", "112010001", "否", ""});
//   csv.modifyRecord(csv.currentFilePath(),
//                    "警报ID", "112010001",
//                    {{"是否解决", "是"}, {"备注", "已处理"}});
//   auto rows = csv.readDay(QDate::currentDate());
// ====================================================================
class AlarmCsvIO
{
public:
    AlarmCsvIO() = default;
    explicit AlarmCsvIO(const AlarmHeaderConfig &config);

    // ----------------------------------------------------------------
    // 基本配置
    // ----------------------------------------------------------------

    // 设置根目录（调用后重置当前文件，下次写入重新创建）
    void        setRootDir(const QString &dir);
    QString     rootDir() const;

    // 设置表头（写入新文件时作为首行）
    void        setHeaders(const QStringList &headers);
    QStringList headers() const; 

    // ----------------------------------------------------------------
    // 写操作
    // ----------------------------------------------------------------

    // 写入一条记录；自动选择/创建当日文件，跨日自动新建
    bool writeRecord(const QStringList &values);

    // ----------------------------------------------------------------
    // 读操作
    // ----------------------------------------------------------------

    // 读取某一天的全部记录（合并当天所有文件，去除重复表头）
    // 返回值：第一行为表头，其余为数据行（均为 QStringList）
    QList<QStringList> readDay(const QDate &date) const;

    // 扫描 rootDir 下所有 YYYYMM/DD_*.csv，返回有日志文件的日期集合
    QSet<QDate> availableDates() const;

    // ----------------------------------------------------------------
    // 修改操作
    // ----------------------------------------------------------------

    // 按 sortHeader 列的值升序，将 values 插入到第一条大于它的记录之前。
    // 若文件不存在或无法打开则返回 false。
    bool insertSortedRecord(const QString &filePath,
                            const QString &sortHeader,
                            const QStringList &values);

    // 在指定文件中查找 keyHeader 列值等于 keyValue 的所有行，
    // 并将 changes（header -> newValue）中指定的列更新。
    // 返回是否至少修改了一行。
    bool modifyRecord(const QString &filePath,
                      const QString &keyHeader, const QString &keyValue,
                      const QHash<QString, QString> &changes);

    // ----------------------------------------------------------------
    // 路径查询
    // ----------------------------------------------------------------

    // 当前正在写入的文件路径（未开始写入时为空）
    QString currentFilePath() const;

    // 根据日期时间推算对应文件路径（不创建文件、不影响当前状态）
    QString filePathForDateTime(const QDateTime &dt) const;

private:
    // 确保当前文件存在（跨日或首次写入时自动新建）；返回文件路径
    QString ensureCurrentFile();

    // 返回指定日期对应月份目录的绝对路径
    QString monthDirPath(const QDate &date) const;

    // CSV 字段转义（含逗号/引号/换行的字段加双引号包裹）
    static QString     escapeField(const QString &field);
    static QString     buildRow(const QStringList &fields);
    static QStringList parseRow(const QString &line);

    QString     m_rootDir;
    QStringList m_headers;
    QString     m_currentFile;
    QDate       m_currentDate;
};
