#pragma once
#include <QString>
#include <QVector>
#include <QDate>
#include <QMetaType>

struct CommPageTableEntry {
    qint64 offset;
    int    recordCount;
};

struct CommPageTable {
    QString  filePath;
    int      pageSize     = 50;
    int      totalRecords = 0;
    QVector<CommPageTableEntry> entries;
    QDate    fileDate;

    int  pageCount() const { return entries.size(); }
    bool isValid()   const { return !filePath.isEmpty() && !entries.isEmpty(); }
};

struct CommPageKey {
    QString filePath;
    int     pageIndex;
    bool operator==(const CommPageKey &o) const
    { return filePath == o.filePath && pageIndex == o.pageIndex; }
};
inline uint qHash(const CommPageKey &k, uint seed = 0)
{ return qHash(k.filePath, seed) ^ qHash(k.pageIndex, seed + 1); }

struct CommPage {
    CommPageKey          key;
    QVector<QStringList> records;
    bool                 dirty = false;
};
Q_DECLARE_METATYPE(CommPage)
