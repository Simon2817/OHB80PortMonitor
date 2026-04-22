#pragma once
#include <QDate>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMetaType>

struct CommHistoryQuery {
    QDate   date;
    int     timeColumnIndex = -1;
    QString timeFrom;
    QString timeTo;
    QString qrcodeFilter;     // 精确匹配 qrcode 列（空=不过滤）
    QString commandIdFilter;  // 精确匹配 CommandId 列（空=不过滤）
    int     pageSize  = 50;
    int     pageIndex = 0;
};

struct CommHistoryResult {
    QVector<QStringList> records;
    QVector<bool>        highlighted;
    QVector<int>         matchedGlobalIndices;
    int currentPage  = 0;
    int totalPages   = 0;
    int totalRecords = 0;
};

Q_DECLARE_METATYPE(CommHistoryQuery)
Q_DECLARE_METATYPE(CommHistoryResult)
