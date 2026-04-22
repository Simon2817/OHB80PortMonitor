#include "commcsvio.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDir>
#include <QJsonValue>

QStringList CommCsvIO::parseLine(const QString &line)
{
    QStringList result;
    QString field;
    bool inQuotes = false;
    for (int i = 0; i < line.size(); ++i) {
        QChar c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"'; ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                field += c;
            }
        } else {
            if (c == '"') {
                inQuotes = true;
            } else if (c == ',') {
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

QString CommCsvIO::joinLine(const QStringList &fields)
{
    QStringList escaped;
    for (const QString &f : fields) {
        if (f.contains(',') || f.contains('"') || f.contains('\n'))
            escaped.append('"' + QString(f).replace('"', "\"\"") + '"');
        else
            escaped.append(f);
    }
    return escaped.join(',');
}

QStringList CommCsvIO::jsonToRow(const QStringList &headers, const QJsonObject &obj)
{
    QStringList row;
    row.reserve(headers.size());
    for (const QString &h : headers) {
        QJsonValue v = obj.value(h);
        if (v.isUndefined() || v.isNull())
            row.append(QString());
        else if (v.isString())
            row.append(v.toString());
        else
            row.append(v.toVariant().toString());
    }
    return row;
}

QStringList CommCsvIO::readHeader(const QString &filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return {};
    QTextStream in(&f);
    in.setCodec("UTF-8");
    if (in.atEnd()) return {};
    return parseLine(in.readLine());
}

bool CommCsvIO::writeHeader(const QString &filePath, const QStringList &headers)
{
    QDir().mkpath(QFileInfo(filePath).absolutePath());
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;
    QTextStream out(&f);
    out.setCodec("UTF-8");
    out << joinLine(headers) << '\n';
    return true;
}

bool CommCsvIO::appendRecord(const QString &filePath,
                              const QStringList &headers,
                              const QJsonObject &record)
{
    QDir().mkpath(QFileInfo(filePath).absolutePath());
    bool needHeader = !QFile::exists(filePath);
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&f);
    out.setCodec("UTF-8");
    if (needHeader) out << joinLine(headers) << '\n';
    out << joinLine(jsonToRow(headers, record)) << '\n';
    return true;
}

QVector<QStringList> CommCsvIO::getRecords(const QString &filePath, int from, int to)
{
    QVector<QStringList> result;
    if (from > to) return result;
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return result;
    QTextStream in(&f);
    in.setCodec("UTF-8");
    if (in.atEnd()) return result;
    in.readLine();
    int idx = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (idx >= from && idx <= to) result.append(parseLine(line));
        if (idx > to) break;
        ++idx;
    }
    return result;
}

QVector<QStringList> CommCsvIO::readAllRecords(const QString &filePath)
{
    QVector<QStringList> result;
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return result;
    QTextStream in(&f);
    in.setCodec("UTF-8");
    if (in.atEnd()) return result;
    in.readLine();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty()) result.append(parseLine(line));
    }
    return result;
}

bool CommCsvIO::modifyRecords(const QString &filePath, int from, int to,
                               const QVector<QStringList> &newRecords)
{
    if (from > to || newRecords.size() != (to - from + 1)) return false;
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&f);
    in.setCodec("UTF-8");
    QStringList lines;
    while (!in.atEnd()) lines.append(in.readLine());
    f.close();
    for (int i = from; i <= to; ++i) {
        int lineIdx = i + 1;
        if (lineIdx < lines.size())
            lines[lineIdx] = joinLine(newRecords[i - from]);
    }
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;
    QTextStream out(&f);
    out.setCodec("UTF-8");
    for (const QString &l : lines) out << l << '\n';
    return true;
}

CommPageTable CommCsvIO::buildPageTable(const QString &filePath, int pageSize)
{
    CommPageTable pt;
    pt.filePath = filePath;
    pt.pageSize = pageSize;
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return pt;
    QTextStream in(&f);
    in.setCodec("UTF-8");
    if (in.atEnd()) return pt;
    in.readLine();

    CommPageTableEntry entry;
    entry.offset      = f.pos();
    entry.recordCount = 0;
    int recordInPage  = 0;

    while (!in.atEnd()) {
        in.readLine();
        ++recordInPage;
        ++pt.totalRecords;
        if (recordInPage == pageSize) {
            entry.recordCount = recordInPage;
            pt.entries.append(entry);
            entry.offset      = f.pos();
            entry.recordCount = 0;
            recordInPage      = 0;
        }
    }
    if (recordInPage > 0) {
        entry.recordCount = recordInPage;
        pt.entries.append(entry);
    }
    return pt;
}

int CommCsvIO::countRecords(const QString &filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;
    QTextStream in(&f);
    in.setCodec("UTF-8");
    if (in.atEnd()) return 0;
    in.readLine();
    int count = 0;
    while (!in.atEnd()) { in.readLine(); ++count; }
    return count;
}
