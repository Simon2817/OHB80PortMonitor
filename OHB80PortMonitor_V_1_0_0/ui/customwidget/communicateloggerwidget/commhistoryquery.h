#pragma once
#include <QDate>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMetaType>

struct CommHistoryQuery {
    QDate   date;
    int     timeColumnIndex = -1;
    // 强制刷新缓存：Search 按钮触发时置 true；翻页按钮触发时保持 false，
    // 以便命中 LRU 缓存而不受"今天 CSV 仍在 append"的影响。
    bool    forceRefresh = false;
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
