#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QSet>
#include <QDate>
#include <QHash>
#include "commlrucache.h"
#include "commpagetable.h"
#include "commcsvio.h"
#include "commhistoryquery.h"

// 历史查询缓存键：(date, timeRange, qrcode, cmdId) 完全决定命中
// 不含文件大小——今天的 CSV 持续 append 不应让翻页缓存失效；
// 用户若要刷新，点 Search 按钮将触发 forceRefresh，由 worker 主动清除该 key。
struct CommQueryKey {
    QDate   date;
    int     timeCol = -2;
    QString timeFrom;
    QString timeTo;
    QString qrcode;
    QString cmdId;

    bool operator==(const CommQueryKey &o) const {
        return date == o.date && timeCol == o.timeCol
            && timeFrom == o.timeFrom && timeTo == o.timeTo
            && qrcode == o.qrcode && cmdId == o.cmdId;
    }
};

inline uint qHash(const CommQueryKey &k, uint seed = 0) Q_DECL_NOTHROW {
    uint h = qHash(k.date, seed);
    h ^= qHash(k.timeCol) ^ qHash(k.timeFrom) ^ qHash(k.timeTo)
       ^ qHash(k.qrcode) ^ qHash(k.cmdId);
    return h;
}

// 历史查询缓存值：经时间过滤的整天记录集 + 子查询命中索引
struct CommQueryValue {
    QVector<QStringList> source;
    QVector<int>         matchedIndices;
};

class CommLogFileSystem : public QObject
{
    Q_OBJECT
public:
    enum class ScrollDirection { Up, Down };

    explicit CommLogFileSystem(QObject *parent = nullptr);

    void    setRootPath(const QString &path);
    QString rootPath() const;
    void    setPageSize(int size);
    int     pageSize() const;
    void    setHeaders(const QStringList &headers);

    // 单个子文件字节上限；写入时若当前文件超过该阈值会自动滚动到新文件
    void    setMaxFileBytes(qint64 bytes);
    qint64  maxFileBytes() const;

public slots:
    void requestInitialize();
    void requestPrevPage();
    void requestNextPage();
    void requestAppendLog(const QString &qrcode, const QString &time, const QString &commandId,
                          const QString &durationMs, const QString &request, const QString &response);
    void requestAppendBatch(const QVector<QStringList> &records);
    void requestCleanOldLogs();
    void requestQueryHistory(const CommHistoryQuery &query);
    void requestAvailableDates();
    // 释放历史查询 LRU 缓存（空闲超时后由上层调用，用于回收内存）
    void requestClearQueryCache();

signals:
    void pageReady(const CommPage &page, bool isPrev);
    void loadProgress(int percent);
    void loadFailed(const QString &reason);
    void logAppended(bool success, bool pageChanged);
    void navigationStateChanged(bool hasPrev, bool hasNext,
                                 const QString &currentFile,
                                 int currentPage, int pageCount);
    void historyReady(const CommHistoryResult &result);
    void availableDatesReady(const QSet<QDate> &dates);

private:
    CommPage        doReadPage(const QString &filePath, int pageIndex);
    bool            doFlushPage(const CommPageKey &key);
    const CommPageTable *getPageTable(const QString &filePath);
    const CommPageTable *loadPageTable(const QString &filePath);
    CommPage        loadPageFromDisk(const QString &filePath, int pageIndex,
                                     const CommPageTable &pt);
    QString         todayFilePath();
    QString         adjacentFilePath(const QString &filePath, ScrollDirection dir) const;
    QStringList     allLogFiles() const;
    QStringList     allMonthDirs() const;
    QStringList     filePathsForDate(const QDate &date) const;
    // 按文件名里的 HHmmss 时间戳剪枝：仅保留与 [timeFrom, timeTo] 可能有交集的分片
    // timeFrom/timeTo 为 "HH:mm:ss" 格式；空字符串表示不限制
    QStringList     prunedFilesByTimeRange(const QStringList &files,
                                           const QString &timeFrom,
                                           const QString &timeTo) const;
    bool            matchesSubQuery(const QStringList &record,
                                    const CommHistoryQuery &query) const;
    QString         extractTime(const QStringList &record, int timeCol) const;
    bool            hasPrevInternal() const;
    bool            hasNextInternal();
    void            emitNavigationState(bool isPrev);

    QString     m_rootPath;
    int         m_pageSize = 50;
    qint64      m_maxFileBytes = 5 * 1024 * 1024;  // 默认 5 MB 分片
    QStringList m_headers;
    QString     m_todayFilePath;
    QString     m_currentFile;
    int         m_currentPage = 0;
    CommPageTable m_ptBuf;

    CommLRUCache<CommPageKey, CommPage>      m_pageCache{3};
    CommLRUCache<QString,     CommPageTable> m_ptCache{5};

    // ---- 历史查询 LRU 缓存（仅在查询专用 worker 上使用） ----
    // 缓存"过滤后的整天记录集 + 子匹配索引"，Key=(date,timeRange,qrcode,cmdId)。
    // 【容量必须为 1】：单个 Value 持有整天几十万行 QStringList，
    // 容量过大会在内存中同时保留多日记录（实测容量 5 时常驻 > 1GB）。
    // 翻页时 Key 不变，容量 1 即可 100% 命中；切换查询条件本就需要重读。
    CommLRUCache<CommQueryKey, CommQueryValue> m_queryCache{1};
};
