#pragma once
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QJsonObject>
#include <QStringList>
#include "commlogfilesystem.h"

class CommLogicalFileSystem : public QObject
{
    Q_OBJECT
public:
    explicit CommLogicalFileSystem(QObject *parent = nullptr);
    ~CommLogicalFileSystem();

    void setRootPath(const QString &path);
    void setPageSize(int size);
    void setHeaders(const QStringList &headers);
    void setMaxFileBytes(qint64 bytes);  // 单个子文件字节上限，超过自动滚动新文件
    QStringList headers() const;

    void initialize();

    bool    hasPrevPage()      const;
    bool    hasNextPage()      const;
    QString currentFilePath()  const;
    int     currentPageIndex() const;
    int     currentPageCount() const;

    void requestPrevPage();
    void requestNextPage();

    void writeLog(const QString &qrcode, const QString &time, const QString &commandId,
                  const QString &durationMs, const QString &request, const QString &response);
    void queryHistory(const CommHistoryQuery &query);
    void requestAvailableDates();

signals:
    void pageReady(const CommPage &page, bool isPrev);
    void loadProgress(int percent);
    void loadFailed(const QString &reason);
    void logAppended(bool success, bool pageChanged);
    void navigationUpdated(bool hasPrev, bool hasNext);
    void historyReady(const CommHistoryResult &result);
    void availableDatesReady(const QSet<QDate> &dates);

    void _requestInitialize();
    void _requestPrevPage();
    void _requestNextPage();
    void _requestAppendLog(const QString &qrcode, const QString &time, const QString &commandId,
                           const QString &durationMs, const QString &request, const QString &response);
    void _requestCleanOldLogs();
    void _requestQueryHistory(const CommHistoryQuery &query);
    void _requestAvailableDates();

private slots:
    void onNavigationStateChanged(bool hasPrev, bool hasNext,
                                   const QString &file, int page, int pageCount);
    void onMidnightCleanup();

private:
    void scheduleMidnight();

    CommLogFileSystem *m_fs           = nullptr;  // append / page 导航（高频写）
    QThread           *m_workerThread = nullptr;

    // 查询专用 worker：与写入完全独立，避免查询请求排在大量 append 事件之后被饿死
    CommLogFileSystem *m_queryFs      = nullptr;  // 仅承载 queryHistory / availableDates
    QThread           *m_queryThread  = nullptr;

    QTimer            *m_midnightTimer = nullptr;

    bool    m_hasPrev     = false;
    bool    m_hasNext     = false;
    QString m_currentFile;
    int     m_currentPage = 0;
    int     m_pageCount   = 0;

    QStringList m_headers;
};
