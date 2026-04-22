#include "alarmfilesystem.h"
#include "alarmid.h"
#include <QDir>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QFile>
#include <QTextStream>

AlarmFileSystem::AlarmFileSystem()
{
    // Use default headers from AlarmHeaderConfig
}

AlarmFileSystem::AlarmFileSystem(const AlarmHeaderConfig &config)
    : m_csv(config)
{
}

void AlarmFileSystem::setRootDir(const QString &dir)
{
    m_csv.setRootDir(dir);
}

QString AlarmFileSystem::rootDir() const
{
    return m_csv.rootDir();
}

void AlarmFileSystem::setHeaders(const QStringList &headers)
{
    m_csv.setHeaders(headers);
}

QStringList AlarmFileSystem::headers() const
{
    return m_csv.headers();
}

// ----------------------------------------------------------------
// 写入一条警报记录（按 SendTime 升序插入）
// ----------------------------------------------------------------
bool AlarmFileSystem::writeAlarm(const AlarmInfo &info)
{
    const QDate date = QDateTime::fromString(
        info.sendTime(), QStringLiteral("yyyy-MM-dd HH:mm:ss")).date();
    const QString filePath = filePathForDate(date.isValid() ? date : QDate::currentDate());
    return m_csv.insertSortedRecord(filePath, QStringLiteral("SendTime"), info.toStringList());
}

QList<QStringList> AlarmFileSystem::readDate(const QDate &date) const
{
    return m_csv.readDay(date);
}

QSet<QDate> AlarmFileSystem::availableDates() const
{
    return m_csv.availableDates();
}

// ----------------------------------------------------------------
// 清理过期月份目录（保留最近 maxMonths 个月）
// ----------------------------------------------------------------
int AlarmFileSystem::purgeOldLogs(int maxMonths)
{
    if (maxMonths <= 0) return 0;

    const QDir rootDir(m_csv.rootDir());
    if (!rootDir.exists()) return 0;

    // 扫描所有格式为 YYYYMM 的子目录（纯6位数字）
    const QStringList entries = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    QStringList monthDirs;
    for (const QString &name : entries) {
        if (name.length() == 6 && QDate::fromString(name, QStringLiteral("yyyyMM")).isValid())
            monthDirs.append(name);
    }

    // 按名称升序（即时间从旧到新）删除超出保留期的月份
    const int toDelete = monthDirs.size() - maxMonths;
    if (toDelete <= 0) return 0;

    int deleted = 0;
    for (int i = 0; i < toDelete; ++i) {
        QDir dir(rootDir.filePath(monthDirs.at(i)));
        if (dir.removeRecursively())
            ++deleted;
    }
    return deleted;
}

// ----------------------------------------------------------------
// 私有辅助：根据日期定位当天唯一的 CSV 文件（每天仅一个）
//   1. 找年月目录 rootDir/YYYYMM/
//   2. 查找已有的 DD_*.csv（每天仅一个）
//   3. 若不存在则创建 DD_HHmmss.csv（为后期文件回滚做准备）
// ----------------------------------------------------------------
QString AlarmFileSystem::filePathForDate(const QDate &date) const
{
    // Step 1: year-month directory
    const QString monthPath = QDir(m_csv.rootDir()).filePath(
        date.toString(QStringLiteral("yyyyMM")));
    QDir monthDir(monthPath);

    // Step 2: find existing DD_*.csv (one per day)
    if (monthDir.exists()) {
        const QString dayPrefix = date.toString(QStringLiteral("dd")) + QStringLiteral("_");
        const QStringList files = monthDir.entryList(
            QStringList() << dayPrefix + QStringLiteral("*.csv"),
            QDir::Files, QDir::Name);
        if (!files.isEmpty())
            return monthDir.filePath(files.first());
    }

    // Step 3: no file for that day — create DD_HHmmss.csv
    monthDir.mkpath(QStringLiteral("."));
    const QString fileName = date.toString(QStringLiteral("dd")) + QStringLiteral("_")
                           + QTime::currentTime().toString(QStringLiteral("HHmmss"))
                           + QStringLiteral(".csv");
    const QString filePath = monthDir.filePath(fileName);

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out.setGenerateByteOrderMark(true);
        out << m_csv.headers().join(QStringLiteral(",")) << QStringLiteral("\n");
    }
    return filePath;
}

// ----------------------------------------------------------------
// 通用修改：以 alarmId 为行索引，通过 sendTime 的日期定位文件
// ----------------------------------------------------------------
bool AlarmFileSystem::modifyAlarm(qint64 alarmId, const QString &sendTime,
                                  const QHash<QString, QString> &changes)
{
    const QDateTime dt = QDateTime::fromString(sendTime, QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    if (!dt.isValid())
        return false;

    const QString filePath = filePathForDate(dt.date());
    return m_csv.modifyRecord(filePath, QStringLiteral("AlarmId"),
                              alarmIdToString(alarmId), changes);
}
