#include "alarmcsvio.h"
#include "alarmid.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QDateTime>

// ============================================================
// 内部静态 CSV 工具函数
// ============================================================

QString AlarmCsvIO::escapeField(const QString &field)
{
    if (field.contains(QLatin1Char(','))  ||
        field.contains(QLatin1Char('"'))  ||
        field.contains(QLatin1Char('\n'))) {
        return QLatin1Char('"')
               + QString(field).replace(QLatin1String("\""), QLatin1String("\"\""))
               + QLatin1Char('"');
    }
    return field;
}

QString AlarmCsvIO::buildRow(const QStringList &fields)
{
    QStringList escaped;
    escaped.reserve(fields.size());
    for (const QString &f : fields)
        escaped.append(escapeField(f));
    return escaped.join(QLatin1Char(','));
}

QStringList AlarmCsvIO::parseRow(const QString &line)
{
    QStringList result;
    QString     field;
    bool        inQuotes = false;

    for (int i = 0; i < line.size(); ++i) {
        const QChar c = line[i];
        if (inQuotes) {
            if (c == QLatin1Char('"')) {
                // 连续两个双引号 → 转义的引号字符
                if (i + 1 < line.size() && line[i + 1] == QLatin1Char('"')) {
                    field += QLatin1Char('"');
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                field += c;
            }
        } else {
            if (c == QLatin1Char('"')) {
                inQuotes = true;
            } else if (c == QLatin1Char(',')) {
                result.append(field);
                field.clear();
            } else {
                field += c;
            }
        }
    }
    result.append(field);
    return result;
}

// ============================================================
// 配置
// ============================================================

void AlarmCsvIO::setRootDir(const QString &dir)
{
    m_rootDir = dir;
    m_currentFile.clear();
    m_currentDate = QDate();
}

QString AlarmCsvIO::rootDir() const { return m_rootDir; }

void AlarmCsvIO::setHeaders(const QStringList &headers)
{
    m_headers = headers;
}

QStringList AlarmCsvIO::headers() const
{
    return m_headers;
}

AlarmCsvIO::AlarmCsvIO(const AlarmHeaderConfig &config)
    : m_headers(config.headers)
{
}

// ============================================================
// 路径辅助
// ============================================================

QString AlarmCsvIO::monthDirPath(const QDate &date) const
{
    return QDir(m_rootDir).filePath(date.toString(QStringLiteral("yyyyMM")));
}

QString AlarmCsvIO::filePathForDateTime(const QDateTime &dt) const
{
    return QDir(monthDirPath(dt.date()))
               .filePath(dt.toString(QStringLiteral("dd_HHmmss")) + QStringLiteral(".csv"));
}

QString AlarmCsvIO::currentFilePath() const { return m_currentFile; }

// ============================================================
// 内部：确保当前写入文件就绪
// ============================================================

QString AlarmCsvIO::ensureCurrentFile()
{
    if (m_rootDir.isEmpty())
        return {};

    const QDateTime now   = QDateTime::currentDateTime();
    const QDate     today = now.date();

    // 已缓存且同一天 → 直接复用
    if (!m_currentFile.isEmpty() && today == m_currentDate)
        return m_currentFile;

    m_currentDate = today;
    const QString monthDir = monthDirPath(today);

    // 查找当天已有的 DD_*.csv（每天仅一个文件）
    const QDir dir(monthDir);
    if (dir.exists()) {
        const QString dayPrefix = today.toString(QStringLiteral("dd")) + QStringLiteral("_");
        const QStringList files = dir.entryList(
            QStringList() << dayPrefix + QStringLiteral("*.csv"),
            QDir::Files, QDir::Name);
        if (!files.isEmpty()) {
            m_currentFile = dir.filePath(files.first());
            return m_currentFile;
        }
    }

    // 当天无文件 → 创建 DD_HHmmss.csv
    QDir().mkpath(monthDir);
    m_currentFile = filePathForDateTime(now);

    if (!m_headers.isEmpty()) {
        QFile f(m_currentFile);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream s(&f);
            s.setCodec("UTF-8");
            s.setGenerateByteOrderMark(true);
            s << buildRow(m_headers) << '\n';
        }
    }
    return m_currentFile;
}

// ============================================================
// 写入
// ============================================================

bool AlarmCsvIO::writeRecord(const QStringList &values)
{
    const QString path = ensureCurrentFile();
    if (path.isEmpty())
        return false;

    QFile f(path);
    if (!f.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream s(&f);
    s.setCodec("UTF-8");
    s << buildRow(values) << '\n';
    return true;
}

// ============================================================
// 读取某天全部数据
// ============================================================

QList<QStringList> AlarmCsvIO::readDay(const QDate &date) const
{
    QList<QStringList> result;

    QDir dir(monthDirPath(date));
    if (!dir.exists())
        return result;

    // 匹配 "DD_*.csv"，按文件名（时间）升序排列
    const QString     prefix = date.toString(QStringLiteral("dd")) + QLatin1Char('_');
    const QStringList files  = dir.entryList({prefix + QStringLiteral("*.csv")},
                                              QDir::Files, QDir::Name);

    bool headerEmitted = false;

    for (const QString &filename : files) {
        QFile f(dir.filePath(filename));
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QTextStream s(&f);
        s.setCodec("UTF-8");

        bool isFirstRow = true;
        while (!s.atEnd()) {
            const QString line = s.readLine();
            if (line.trimmed().isEmpty())
                continue;

            const QStringList row = parseRow(line);

            if (isFirstRow) {
                isFirstRow = false;
                if (!headerEmitted) {
                    result.append(row);   // 仅保留第一个文件的表头
                    headerEmitted = true;
                }
                continue;               // 其余文件的表头行跳过
            }
            result.append(row);
        }
    }
    return result;
}

// ============================================================
// 可用日期扫描
// ============================================================

QSet<QDate> AlarmCsvIO::availableDates() const
{
    QSet<QDate> dates;
    if (m_rootDir.isEmpty())
        return dates;

    const QDir root(m_rootDir);
    const QStringList monthDirs = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &monthName : monthDirs) {
        if (monthName.length() != 6)
            continue;
        bool ok;
        const int year  = monthName.left(4).toInt(&ok);  if (!ok) continue;
        const int month = monthName.right(2).toInt(&ok); if (!ok) continue;

        const QDir monthDir(root.filePath(monthName));
        const QStringList files = monthDir.entryList(
            QStringList() << QStringLiteral("??_*.csv"), QDir::Files);
        for (const QString &fn : files) {
            const int day = fn.left(2).toInt(&ok); if (!ok) continue;
            const QDate date(year, month, day);
            if (date.isValid())
                dates.insert(date);
        }
    }
    return dates;
}

// ============================================================
// 有序插入记录
// ============================================================

bool AlarmCsvIO::insertSortedRecord(const QString &filePath,
                                     const QString &sortHeader,
                                     const QStringList &values)
{
    // --- 读取全部行（表头 + 数据）---
    QList<QStringList> dataRows;
    QStringList headerRow;

    QFile f(filePath);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream s(&f);
        s.setCodec("UTF-8");
        bool first = true;
        while (!s.atEnd()) {
            const QString line = s.readLine();
            if (line.trimmed().isEmpty())
                continue;
            if (first) {
                headerRow = parseRow(line);
                first = false;
            } else {
                dataRows.append(parseRow(line));
            }
        }
        f.close();
    }

    // --- 确定排序列索引 ---
    const int sortCol = headerRow.indexOf(sortHeader);
    const QString newVal = (sortCol >= 0 && sortCol < values.size())
                           ? values.at(sortCol) : QString();

    // --- 找到第一条 sortVal > newVal 的位置（按字符串升序，yyyy-MM-dd HH:mm:ss 可直接比较）---
    int insertIdx = dataRows.size();
    if (sortCol >= 0 && !newVal.isEmpty()) {
        for (int i = 0; i < dataRows.size(); ++i) {
            const QString rowVal = sortCol < dataRows.at(i).size()
                                   ? dataRows.at(i).at(sortCol) : QString();
            if (rowVal > newVal) {
                insertIdx = i;
                break;
            }
        }
    }
    dataRows.insert(insertIdx, values);

    // --- 回写 ---
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&f);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true);
    out << buildRow(headerRow) << '\n';
    for (const QStringList &row : dataRows)
        out << buildRow(row) << '\n';

    return true;
}

// ============================================================
// 修改记录
// ============================================================

bool AlarmCsvIO::modifyRecord(const QString &filePath,
                               const QString &keyHeader, const QString &keyValue,
                               const QHash<QString, QString> &changes)
{
    // --- 读取全部行 ---
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QStringList lines;
    {
        QTextStream s(&f);
        s.setCodec("UTF-8");
        while (!s.atEnd())
            lines.append(s.readLine());
    }
    f.close();

    if (lines.isEmpty())
        return false;

    // --- 解析表头，确定列索引 ---
    const QStringList headerRow = parseRow(lines.first());
    const int keyCol = headerRow.indexOf(keyHeader);
    if (keyCol < 0)
        return false;

    QHash<int, QString> colChanges;
    for (auto it = changes.cbegin(); it != changes.cend(); ++it) {
        const int col = headerRow.indexOf(it.key());
        if (col >= 0)
            colChanges.insert(col, it.value());
    }
    if (colChanges.isEmpty())
        return false;

    // --- 查找并修改匹配行 ---
    bool modified = false;
    for (int i = 1; i < lines.size(); ++i) {
        if (lines[i].trimmed().isEmpty())
            continue;

        QStringList row = parseRow(lines[i]);
        if (keyCol >= row.size() || row[keyCol] != keyValue)
            continue;

        for (auto it = colChanges.cbegin(); it != colChanges.cend(); ++it) {
            while (row.size() <= it.key())
                row.append(QString());
            row[it.key()] = it.value();
        }
        lines[i] = buildRow(row);
        modified  = true;
    }

    if (!modified)
        return false;

    // --- 回写 ---
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&f);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true);
    for (const QString &line : lines)
        out << line << '\n';

    return true;
}
