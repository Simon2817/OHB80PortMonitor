#include "commlogicalfilesystem.h"
#include <QCoreApplication>
#include <QDateTime>

CommLogicalFileSystem::CommLogicalFileSystem(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<CommHistoryQuery>("CommHistoryQuery");
    qRegisterMetaType<CommHistoryResult>("CommHistoryResult");
    qRegisterMetaType<CommPage>("CommPage");
    qRegisterMetaType<QSet<QDate>>("QSet<QDate>");

    // ---- 主 worker：append / page 导航 ----
    m_fs           = new CommLogFileSystem();
    m_workerThread = new QThread(this);
    m_fs->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, m_fs, &QObject::deleteLater);

    connect(this, &CommLogicalFileSystem::_requestInitialize,
            m_fs, &CommLogFileSystem::requestInitialize);
    connect(this, &CommLogicalFileSystem::_requestPrevPage,
            m_fs, &CommLogFileSystem::requestPrevPage);
    connect(this, &CommLogicalFileSystem::_requestNextPage,
            m_fs, &CommLogFileSystem::requestNextPage);
    connect(this, &CommLogicalFileSystem::_requestAppendLog,
            m_fs, &CommLogFileSystem::requestAppendLog);
    connect(this, &CommLogicalFileSystem::_requestCleanOldLogs,
            m_fs, &CommLogFileSystem::requestCleanOldLogs);

    connect(m_fs, &CommLogFileSystem::pageReady,    this, &CommLogicalFileSystem::pageReady);
    connect(m_fs, &CommLogFileSystem::loadProgress, this, &CommLogicalFileSystem::loadProgress);
    connect(m_fs, &CommLogFileSystem::loadFailed,   this, &CommLogicalFileSystem::loadFailed);
    connect(m_fs, &CommLogFileSystem::logAppended,  this, &CommLogicalFileSystem::logAppended);
    connect(m_fs, &CommLogFileSystem::navigationStateChanged,
            this, &CommLogicalFileSystem::onNavigationStateChanged);

    // ---- 查询专用 worker：与 append 队列完全隔离 ----
    // 查询类操作（queryHistory / requestAvailableDates）只读 rootPath + 裸文件 I/O，
    // 与 append 路径的 page/ptCache 无任何共享状态，可独立运行。
    m_queryFs     = new CommLogFileSystem();
    m_queryThread = new QThread(this);
    m_queryFs->moveToThread(m_queryThread);
    connect(m_queryThread, &QThread::finished, m_queryFs, &QObject::deleteLater);

    connect(this, &CommLogicalFileSystem::_requestQueryHistory,
            m_queryFs, &CommLogFileSystem::requestQueryHistory);
    connect(this, &CommLogicalFileSystem::_requestAvailableDates,
            m_queryFs, &CommLogFileSystem::requestAvailableDates);
    connect(this, &CommLogicalFileSystem::_requestClearQueryCache,
            m_queryFs, &CommLogFileSystem::requestClearQueryCache);

    connect(m_queryFs, &CommLogFileSystem::historyReady,
            this, &CommLogicalFileSystem::historyReady);
    connect(m_queryFs, &CommLogFileSystem::availableDatesReady,
            this, &CommLogicalFileSystem::availableDatesReady);

    m_workerThread->start();
    m_queryThread->start();

    m_midnightTimer = new QTimer(this);
    m_midnightTimer->setSingleShot(true);
    connect(m_midnightTimer, &QTimer::timeout,
            this, &CommLogicalFileSystem::onMidnightCleanup);
    scheduleMidnight();
}

CommLogicalFileSystem::~CommLogicalFileSystem()
{
    // 断开所有发往两个 worker 的信号，阻止新事件入队
    disconnect(this, nullptr, m_fs, nullptr);
    disconnect(this, nullptr, m_queryFs, nullptr);
    // 清除两个工作线程事件队列中已积压的待处理事件（主要是大量 requestAppendLog）
    QCoreApplication::removePostedEvents(m_fs);
    QCoreApplication::removePostedEvents(m_queryFs);

    m_queryThread->quit();
    m_workerThread->quit();
    m_queryThread->wait();
    m_workerThread->wait();
}

void CommLogicalFileSystem::setRootPath(const QString &path)
{
    // 两个 worker 都需要知道 rootPath；仅在 worker 线程启动前调用，无跨线程竞争风险的场景下直赋
    // 若后续要求更严谨，可改为通过 signal + QueuedConnection 投递。
    m_fs->setRootPath(path);
    m_queryFs->setRootPath(path);
}
void CommLogicalFileSystem::setPageSize(int size)
{
    m_fs->setPageSize(size);
    m_queryFs->setPageSize(size);
}
void CommLogicalFileSystem::setHeaders(const QStringList &h)
{
    m_headers = h;
    m_fs->setHeaders(h);
    m_queryFs->setHeaders(h);
}
void CommLogicalFileSystem::setMaxFileBytes(qint64 bytes)
{
    m_fs->setMaxFileBytes(bytes);
    m_queryFs->setMaxFileBytes(bytes);
}
QStringList CommLogicalFileSystem::headers() const            { return m_headers; }

void CommLogicalFileSystem::initialize()           { emit _requestInitialize(); }
bool CommLogicalFileSystem::hasPrevPage()      const { return m_hasPrev; }
bool CommLogicalFileSystem::hasNextPage()      const { return m_hasNext; }
QString CommLogicalFileSystem::currentFilePath()  const { return m_currentFile; }
int     CommLogicalFileSystem::currentPageIndex() const { return m_currentPage; }
int     CommLogicalFileSystem::currentPageCount() const { return m_pageCount; }

void CommLogicalFileSystem::requestPrevPage()      { emit _requestPrevPage(); }
void CommLogicalFileSystem::requestNextPage()      { emit _requestNextPage(); }

void CommLogicalFileSystem::writeLog(const QString &qrcode, const QString &time,
                                      const QString &commandId, const QString &durationMs,
                                      const QString &request, const QString &response)
{
    emit _requestAppendLog(qrcode, time, commandId, durationMs, request, response);
}

void CommLogicalFileSystem::queryHistory(const CommHistoryQuery &query)
{
    emit _requestQueryHistory(query);
}

void CommLogicalFileSystem::requestAvailableDates()
{
    emit _requestAvailableDates();
}

void CommLogicalFileSystem::clearQueryCache()
{
    emit _requestClearQueryCache();
}

void CommLogicalFileSystem::onNavigationStateChanged(bool hasPrev, bool hasNext,
                                                      const QString &file,
                                                      int page, int pageCount)
{
    m_hasPrev     = hasPrev;
    m_hasNext     = hasNext;
    m_currentFile = file;
    m_currentPage = page;
    m_pageCount   = pageCount;
    emit navigationUpdated(hasPrev, hasNext);
}

void CommLogicalFileSystem::onMidnightCleanup()
{
    emit _requestCleanOldLogs();
    scheduleMidnight();
}

void CommLogicalFileSystem::scheduleMidnight()
{
    QDateTime now      = QDateTime::currentDateTime();
    QDateTime tomorrow = QDateTime(now.date().addDays(1), QTime(0, 0, 1));
    qint64 ms = now.msecsTo(tomorrow);
    m_midnightTimer->start(static_cast<int>(qMin(ms, (qint64)INT_MAX)));
}
