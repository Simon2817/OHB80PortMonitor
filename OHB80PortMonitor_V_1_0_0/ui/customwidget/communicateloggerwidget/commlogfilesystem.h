#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QSet>
#include "commlrucache.h"
#include "commpagetable.h"
#include "commcsvio.h"
#include "commhistoryquery.h"

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
    void requestAppendLog(const QJsonObject &record);
    void requestCleanOldLogs();
    void requestQueryHistory(const CommHistoryQuery &query);
    void requestAvailableDates();

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

    // ---- 历史查询 1 项缓存（仅在查询专用 worker 上使用） ----
    // 缓存 "过滤后的整天记录集 + 子匹配索引"，相同 (date,timeRange,qrcode,cmdId) 只读盘一次；
    // 翻页只是对缓存做切片，避免每次重读整天 CSV。
    // 缓存键还包含当天所有文件的 (路径, 大小)，保证今天的文件被 append 后自动失效。
    QDate                         m_qcDate;
    int                           m_qcTimeCol = -2;   // -2 = 未初始化
    QString                       m_qcTimeFrom;
    QString                       m_qcTimeTo;
    QString                       m_qcQrcode;
    QString                       m_qcCmdId;
    QVector<QPair<QString,qint64>> m_qcFiles;         // 文件路径 + 大小
    QVector<QStringList>          m_qcSource;         // 经时间过滤后的完整记录集
    QVector<int>                  m_qcMatchedIndices; // 经子查询过滤的 source 内下标
    bool                          m_qcValid = false;
};
