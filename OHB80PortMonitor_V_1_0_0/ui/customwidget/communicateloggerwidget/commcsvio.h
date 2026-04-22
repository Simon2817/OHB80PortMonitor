#pragma once
#include <QString>
#include <QStringList>
#include <QVector>
#include <QJsonObject>
#include "commpagetable.h"

class CommCsvIO
{
public:
    static QStringList readHeader(const QString &filePath);
    static bool writeHeader(const QString &filePath, const QStringList &headers);

    static bool appendRecord(const QString &filePath,
                             const QStringList &headers,
                             const QJsonObject &record);

    static QVector<QStringList> getRecords(const QString &filePath, int from, int to);
    static QVector<QStringList> readAllRecords(const QString &filePath);

    static bool modifyRecords(const QString &filePath,
                              int from, int to,
                              const QVector<QStringList> &newRecords);

    static CommPageTable buildPageTable(const QString &filePath, int pageSize);

    static QStringList jsonToRow(const QStringList &headers, const QJsonObject &obj);
    static QStringList parseLine(const QString &line);
    static QString     joinLine(const QStringList &fields);
    static int         countRecords(const QString &filePath);
};
