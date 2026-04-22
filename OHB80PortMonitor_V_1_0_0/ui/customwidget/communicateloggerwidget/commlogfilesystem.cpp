#include "commlogfilesystem.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>
#include <QDateTime>
#include <QtConcurrent/QtConcurrent>

CommLogFileSystem::CommLogFileSystem(QObject *parent) : QObject(parent) {}

void CommLogFileSystem::setRootPath(const QString &path) { m_rootPath = path; }
QString CommLogFileSystem::rootPath() const               { return m_rootPath; }
void CommLogFileSystem::setPageSize(int size)              { m_pageSize = qMax(1, size); }
int  CommLogFileSystem::pageSize() const                   { return m_pageSize; }
void CommLogFileSystem::setHeaders(const QStringList &h)  { m_headers = h; }
void CommLogFileSystem::setMaxFileBytes(qint64 bytes)
{
    // 下限 64KB，避免设置过小导致爆炸式滚动
    m_maxFileBytes = qMax<qint64>(64 * 1024, bytes);
}
qint64 CommLogFileSystem::maxFileBytes() const { return m_maxFileBytes; }

void CommLogFileSystem::requestInitialize()
{
    emit loadProgress(0);
    m_currentFile = todayFilePath();
    const CommPageTable *pt = getPageTable(m_currentFile);
    m_currentPage = pt ? qMax(0, pt->pageCount() - 1) : 0;
    emit loadProgress(50);
    if (pt && pt->pageCount() > 0) {
        CommPage page = loadPageFromDisk(m_currentFile, m_currentPage, *pt);
        CommPageKey ek; CommPage ep;
        if (m_pageCache.put(page.key, page, &ek, &ep))
            if (ep.dirty) doFlushPage(ek);
        emit loadProgress(100);
        emit pageReady(page, false);
    } else {
        emit loadProgress(100);
        emit pageReady(CommPage{}, false);
    }
    emitNavigationState(false);
}

void CommLogFileSystem::requestPrevPage()
{
    emit loadProgress(0);
    QString targetFile = m_currentFile;
    int     targetPage = m_currentPage;
    if (m_currentPage > 0) {
        targetPage = m_currentPage - 1;
    } else {
        QString prev = adjacentFilePath(m_currentFile, ScrollDirection::Up);
        if (prev.isEmpty()) { emit loadFailed(QStringLiteral("Already at first page")); return; }
        targetFile = prev;
        const CommPageTable *pt = getPageTable(targetFile);
        targetPage = pt ? qMax(0, pt->pageCount() - 1) : 0;
    }
    emit loadProgress(50);
    CommPage page = doReadPage(targetFile, targetPage);
    m_currentFile = targetFile;
    m_currentPage = targetPage;
    emit loadProgress(100);
    emit pageReady(page, true);
    emitNavigationState(true);
}

void CommLogFileSystem::requestNextPage()
{
    emit loadProgress(0);
    QString targetFile = m_currentFile;
    int     targetPage = m_currentPage;
    const CommPageTable *ptCur = getPageTable(m_currentFile);
    if (ptCur && m_currentPage < ptCur->pageCount() - 1) {
        targetPage = m_currentPage + 1;
    } else {
        QString next = adjacentFilePath(m_currentFile, ScrollDirection::Down);
        if (next.isEmpty()) { emit loadFailed(QStringLiteral("Already at last page")); return; }
        targetFile = next;
        targetPage = 0;
    }
    emit loadProgress(50);
    CommPage page = doReadPage(targetFile, targetPage);
    m_currentFile = targetFile;
    m_currentPage = targetPage;
    emit loadProgress(100);
    emit pageReady(page, false);
    emitNavigationState(false);
}

void CommLogFileSystem::requestAppendLog(const QJsonObject &record)
{
    QString path = todayFilePath();
    const CommPageTable *oldPt = getPageTable(path);
    int oldPageCount = oldPt ? oldPt->pageCount() : 0;
    bool wasOnLastPage = (m_currentFile == path
                          && oldPageCount > 0
                          && m_currentPage == oldPageCount - 1);

    bool ok = CommCsvIO::appendRecord(path, m_headers, record);
    if (!ok) { emit logAppended(false, false); return; }

    if (oldPageCount > 0)
        m_pageCache.remove({path, oldPageCount - 1});
    m_ptCache.remove(path);

    const CommPageTable *newPt = getPageTable(path);
    int newPageCount = newPt ? newPt->pageCount() : 0;
    bool pageChanged = !wasOnLastPage || (newPageCount != oldPageCount);

    int lastPage = qMax(0, newPageCount - 1);
    m_currentFile = path;
    m_currentPage = lastPage;

    CommPage page = doReadPage(path, lastPage);
    emit pageReady(page, false);
    emitNavigationState(false);
    emit logAppended(true, pageChanged);
}

void CommLogFileSystem::requestCleanOldLogs()
{
    QStringList months = allMonthDirs();
    const int kKeep = 6;
    if (months.size() <= kKeep) return;
    QStringList toDelete = months.mid(0, months.size() - kKeep);
    QDir root(m_rootPath);
    for (const QString &month : toDelete) {
        QString monthPath = root.filePath(month);
        QDirIterator it(monthPath, {"*.csv"}, QDir::Files);
        while (it.hasNext()) {
            QString fp = it.next();
            CommPageTable pt;
            if (m_ptCache.get(fp, pt)) {
                for (int i = 0; i < pt.pageCount(); ++i)
                    m_pageCache.remove({fp, i});
                m_ptCache.remove(fp);
            }
        }
        QDir(monthPath).removeRecursively();
    }
}

QStringList CommLogFileSystem::filePathsForDate(const QDate &date) const
{
    QStringList result;
    QString monthDir = QDir(m_rootPath).filePath(date.toString("yyyyMM"));
    QDir dir(monthDir);
    if (!dir.exists()) return result;
    QString dayPrefix = date.toString("dd");
    QStringList files = dir.entryList({dayPrefix + "_*.csv"}, QDir::Files);
    files.sort();
    for (const QString &f : files)
        result.append(dir.absoluteFilePath(f));
    return result;
}

QStringList CommLogFileSystem::prunedFilesByTimeRange(const QStringList &files,
                                                       const QString &timeFrom,
                                                       const QString &timeTo) const
{
    if (files.size() <= 1) return files;   // 仅 0/1 个文件无需剪枝
    if (timeFrom.isEmpty() && timeTo.isEmpty()) return files;

    // 从文件名 "dd_HHmmss[_N].csv" 中提取 HHmmss 作为该分片的起始时间
    // 返回 6 位字符串（不含冒号）；解析失败返回空串
    auto extractStampHHmmss = [](const QString &path) -> QString {
        QString base = QFileInfo(path).baseName();            // "dd_HHmmss" 或 "dd_HHmmss_N"
        int first = base.indexOf('_');
        if (first < 0) return {};
        int second = base.indexOf('_', first + 1);
        QString stamp = (second < 0) ? base.mid(first + 1)
                                     : base.mid(first + 1, second - first - 1);
        if (stamp.size() != 6) return {};
        return stamp;                                         // "HHmmss"
    };

    // 将 "HH:mm:ss" 归一化为 "HHmmss" 以便与文件名戳直接字符串比较
    auto normalize = [](const QString &t) -> QString {
        QString s = t;
        s.remove(':');
        return s;
    };
    QString qFrom = normalize(timeFrom);   // 可能为空
    QString qTo   = normalize(timeTo);     // 可能为空

    // 预提取每个文件的起始戳（保持与 files 同序）
    // files 已按文件名字典序排序 → 与时间升序一致
    QVector<QString> starts;
    starts.reserve(files.size());
    for (const QString &fp : files) starts.append(extractStampHHmmss(fp));

    QStringList kept;
    kept.reserve(files.size());
    for (int i = 0; i < files.size(); ++i) {
        const QString &start = starts[i];
        // 下一个分片的起始时间即本片的"上界"；最后一片视作 "235959" 上界
        QString nextStart = (i + 1 < files.size()) ? starts[i + 1] : QStringLiteral("235959");
        if (start.isEmpty() || nextStart.isEmpty()) {
            // 解析失败时保守保留
            kept.append(files[i]);
            continue;
        }
        // 本片覆盖时间范围 [start, nextStart)
        // 若 nextStart <= qFrom → 本片整体早于目标区间，跳过
        if (!qFrom.isEmpty() && nextStart <= qFrom) continue;
        // 若 start > qTo → 本片整体晚于目标区间，跳过
        if (!qTo.isEmpty() && start > qTo) continue;
        kept.append(files[i]);
    }
    return kept;
}

QString CommLogFileSystem::extractTime(const QStringList &record, int timeCol) const
{
    if (timeCol < 0 || timeCol >= record.size()) return {};
    const QString &val = record[timeCol];
    int spaceIdx = val.indexOf(' ');
    return (spaceIdx >= 0) ? val.mid(spaceIdx + 1) : val;
}

bool CommLogFileSystem::matchesSubQuery(const QStringList &record,
                                         const CommHistoryQuery &query) const
{
    if (!query.likePattern.isEmpty()) {
        for (const QString &field : record) {
            if (field.contains(query.likePattern, Qt::CaseInsensitive))
                return true;
        }
        return false;
    }
    return true;
}

void CommLogFileSystem::requestQueryHistory(const CommHistoryQuery &query)
{
    CommHistoryResult result;

    // ---- 1. 收集当天文件列表 + 大小，用于缓存键 ----
    QStringList files = filePathsForDate(query.date);
    QVector<QPair<QString,qint64>> fileSig;
    fileSig.reserve(files.size());
    for (const QString &fp : files)
        fileSig.append({fp, QFileInfo(fp).size()});

    if (files.isEmpty()) {
        // 清掉缓存并返回空
        m_qcValid = false;
        emit historyReady(result);
        return;
    }

    // ---- 2. 检查缓存是否命中 ----
    bool cacheHit = m_qcValid
                    && m_qcDate      == query.date
                    && m_qcTimeCol   == query.timeColumnIndex
                    && m_qcTimeFrom  == query.timeFrom
                    && m_qcTimeTo    == query.timeTo
                    && m_qcLike      == query.likePattern
                    && m_qcFiles     == fileSig;

    if (!cacheHit) {
        bool hasTimeRange = query.timeColumnIndex >= 0
                            && !query.timeFrom.isEmpty()
                            && !query.timeTo.isEmpty();
        bool hasSubQuery  = !query.likePattern.isEmpty();

        // ---- 3a. 按文件名时间戳剪枝，直接跳过完全不在范围内的分片 ----
        QStringList filesToRead = hasTimeRange
            ? prunedFilesByTimeRange(files, query.timeFrom, query.timeTo)
            : files;

        // ---- 3b. 并发读取剩余分片（每个任务独立解析 ~5MB CSV，按核心并行） ----
        // QtConcurrent::blockingMapped 会在全局 QThreadPool 上派发任务并等待全部完成；
        // 当前代码已在专用查询 worker 线程上运行，blockingMapped 的阻塞等待不会影响 UI。
        QList<QVector<QStringList>> perFile = QtConcurrent::blockingMapped(
            filesToRead,
            std::function<QVector<QStringList>(const QString &)>(
                [](const QString &fp) { return CommCsvIO::readAllRecords(fp); }));

        // 合并（移动语义避免拷贝）
        int total = 0;
        for (const auto &v : perFile) total += v.size();
        QVector<QStringList> allRecords;
        allRecords.reserve(total);
        for (auto &v : perFile) {
            for (auto &r : v) allRecords.append(std::move(r));
        }

        // ---- 3c. 按时间范围精筛（文件级剪枝后仍需逐行过滤边界分片） ----
        QVector<QStringList> source;
        if (hasTimeRange) {
            source.reserve(allRecords.size());
            for (const QStringList &rec : allRecords) {
                QString t = extractTime(rec, query.timeColumnIndex);
                if (t >= query.timeFrom && t <= query.timeTo)
                    source.append(rec);
            }
        } else {
            source = std::move(allRecords);
        }

        QVector<int> matchedIndices;
        if (hasSubQuery) {
            for (int i = 0; i < source.size(); ++i) {
                if (matchesSubQuery(source[i], query))
                    matchedIndices.append(i);
            }
        }

        // 写入缓存
        m_qcDate            = query.date;
        m_qcTimeCol         = query.timeColumnIndex;
        m_qcTimeFrom        = query.timeFrom;
        m_qcTimeTo          = query.timeTo;
        m_qcLike            = query.likePattern;
        m_qcFiles           = fileSig;
        m_qcSource          = std::move(source);
        m_qcMatchedIndices  = std::move(matchedIndices);
        m_qcValid           = true;
    }

    // ---- 4. 从缓存做分页切片（热路径：翻页走这里） ----
    // 核心：分页以"结果集"为基准
    //   - 有 like 过滤时：结果集 = source[matchedIndices]，共 matchedIndices.size() 条
    //   - 无 like 过滤时：结果集 = source，共 source.size() 条
    const QVector<QStringList> &source  = m_qcSource;
    const QVector<int>         &matched = m_qcMatchedIndices;
    bool hasSubQuery = !query.likePattern.isEmpty();

    int viewSize = hasSubQuery ? matched.size() : source.size();

    result.totalRecords = viewSize;
    result.totalPages   = viewSize == 0 ? 0
                          : (viewSize + query.pageSize - 1) / query.pageSize;
    result.currentPage  = qBound(0, query.pageIndex, qMax(0, result.totalPages - 1));

    int from = result.currentPage * query.pageSize;
    int to   = qMin(from + query.pageSize, viewSize);
    result.records.reserve(to - from);
    result.highlighted.reserve(to - from);
    for (int i = from; i < to; ++i) {
        if (hasSubQuery) {
            // 只取命中 like 的那些行
            result.records.append(source[matched[i]]);
            result.highlighted.append(true); // 结果集内每行都是匹配
        } else {
            result.records.append(source[i]);
            result.highlighted.append(false);
        }
    }

    // matchedGlobalIndices 语义：相对结果集（view）的全局行下标。
    //   - 有 like 时：每行都是匹配 → [0, 1, ..., viewSize-1]，用于 widget 的"跳到首个匹配"逻辑（落在第 0 行第 0 页）
    //   - 无 like 时：不存在"匹配"概念 → 置空
    if (hasSubQuery) {
        result.matchedGlobalIndices.resize(viewSize);
        for (int i = 0; i < viewSize; ++i) result.matchedGlobalIndices[i] = i;
    } else {
        result.matchedGlobalIndices.clear();
    }

    emit historyReady(result);
}

void CommLogFileSystem::requestAvailableDates()
{
    QSet<QDate> dates;
    QStringList files = allLogFiles();
    for (const QString &fp : files) {
        QFileInfo fi(fp);
        QString monthDirName = fi.dir().dirName();
        QString dayStr       = fi.baseName().left(2);
        QDate d = QDate::fromString(monthDirName + dayStr, "yyyyMMdd");
        if (d.isValid()) dates.insert(d);
    }
    emit availableDatesReady(dates);
}

CommPage CommLogFileSystem::doReadPage(const QString &filePath, int pageIndex)
{
    CommPageKey key{filePath, pageIndex};
    CommPage cached;
    if (m_pageCache.get(key, cached)) return cached;
    const CommPageTable *pt = getPageTable(filePath);
    if (!pt || pageIndex < 0 || pageIndex >= pt->pageCount()) return CommPage{};
    CommPage page = loadPageFromDisk(filePath, pageIndex, *pt);
    CommPageKey ek; CommPage ep;
    if (m_pageCache.put(key, page, &ek, &ep))
        if (ep.dirty) doFlushPage(ek);
    return page;
}

bool CommLogFileSystem::doFlushPage(const CommPageKey &key)
{
    CommPage page;
    if (!m_pageCache.get(key, page) || !page.dirty) return true;
    const CommPageTable *pt = getPageTable(key.filePath);
    if (!pt || key.pageIndex >= pt->pageCount()) return false;
    int from = key.pageIndex * pt->pageSize;
    int to   = from + pt->entries[key.pageIndex].recordCount - 1;
    bool ok  = CommCsvIO::modifyRecords(key.filePath, from, to, page.records);
    if (ok) { page.dirty = false; m_pageCache.put(key, page); }
    return ok;
}

const CommPageTable *CommLogFileSystem::getPageTable(const QString &filePath)
{
    if (m_ptCache.get(filePath, m_ptBuf)) return &m_ptBuf;
    return loadPageTable(filePath);
}

const CommPageTable *CommLogFileSystem::loadPageTable(const QString &filePath)
{
    CommPageTable pt = CommCsvIO::buildPageTable(filePath, m_pageSize);
    pt.fileDate = QDate::fromString(
        QFileInfo(filePath).dir().dirName() + QFileInfo(filePath).baseName().left(2),
        "yyyyMMdd");
    QString evictedPath; CommPageTable evictedPt;
    if (m_ptCache.put(filePath, pt, &evictedPath, &evictedPt)) {
        for (int i = 0; i < evictedPt.pageCount(); ++i) {
            doFlushPage({evictedPath, i});
            m_pageCache.remove({evictedPath, i});
        }
    }
    m_ptCache.get(filePath, m_ptBuf);
    return &m_ptBuf;
}

CommPage CommLogFileSystem::loadPageFromDisk(const QString &filePath, int pageIndex,
                                               const CommPageTable &pt)
{
    int from = pageIndex * pt.pageSize;
    int to   = from + pt.entries[pageIndex].recordCount - 1;
    CommPage page;
    page.key     = {filePath, pageIndex};
    page.records = CommCsvIO::getRecords(filePath, from, to);
    page.dirty   = false;
    return page;
}

QString CommLogFileSystem::todayFilePath()
{
    QDate today = QDate::currentDate();

    // ---- 1. 复用已缓存路径（日期匹配 && 大小未超阈值） ----
    if (!m_todayFilePath.isEmpty()) {
        QDate cached = QDate::fromString(
            QFileInfo(m_todayFilePath).dir().dirName()
            + QFileInfo(m_todayFilePath).baseName().left(2), "yyyyMMdd");
        if (cached == today
            && QFileInfo(m_todayFilePath).size() < m_maxFileBytes) {
            return m_todayFilePath;
        }
        // 日期变化或文件已超阈值 → 需要新分片
        m_todayFilePath.clear();
    }

    // ---- 2. 扫描当天已有的分片，若最新一片仍未写满则复用 ----
    QString monthDir  = QDir(m_rootPath).filePath(today.toString("yyyyMM"));
    QString dayPrefix = today.toString("dd");
    QDir    dir(monthDir);
    QStringList existing = dir.entryList({dayPrefix + "_*.csv"}, QDir::Files);
    existing.sort();
    if (!existing.isEmpty()) {
        QString latest = dir.absoluteFilePath(existing.last());
        if (QFileInfo(latest).size() < m_maxFileBytes) {
            m_todayFilePath = latest;
            return m_todayFilePath;
        }
        // 最新一片已满 → 继续往下走新建分片
    }

    // ---- 3. 新建分片：dd_HHmmss.csv（同秒冲突时追加 _N 序号） ----
    QDir().mkpath(monthDir);
    QString stamp    = QDateTime::currentDateTime().toString("dd_HHmmss");
    QString candidate = QDir(monthDir).filePath(stamp + ".csv");
    int suffix = 1;
    while (QFile::exists(candidate)) {
        candidate = QDir(monthDir).filePath(
            QString("%1_%2.csv").arg(stamp).arg(suffix++));
    }
    m_todayFilePath = candidate;
    if (!m_headers.isEmpty())
        CommCsvIO::writeHeader(m_todayFilePath, m_headers);
    return m_todayFilePath;
}

QString CommLogFileSystem::adjacentFilePath(const QString &filePath,
                                              ScrollDirection dir) const
{
    QStringList all = allLogFiles();
    int idx = all.indexOf(filePath);
    if (idx < 0) return {};
    if (dir == ScrollDirection::Up   && idx > 0)            return all[idx - 1];
    if (dir == ScrollDirection::Down && idx < all.size()-1) return all[idx + 1];
    return {};
}

QStringList CommLogFileSystem::allLogFiles() const
{
    QStringList files;
    QDirIterator it(m_rootPath, {"*.csv"}, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) files.append(it.next());
    files.sort();
    return files;
}

QStringList CommLogFileSystem::allMonthDirs() const
{
    QDir root(m_rootPath);
    QStringList dirs = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    dirs.sort();
    return dirs;
}

bool CommLogFileSystem::hasPrevInternal() const
{
    if (m_currentPage > 0) return true;
    return !adjacentFilePath(m_currentFile, ScrollDirection::Up).isEmpty();
}

bool CommLogFileSystem::hasNextInternal()
{
    const CommPageTable *pt = getPageTable(m_currentFile);
    if (pt && m_currentPage < pt->pageCount() - 1) return true;
    return !adjacentFilePath(m_currentFile, ScrollDirection::Down).isEmpty();
}

void CommLogFileSystem::emitNavigationState(bool /*isPrev*/)
{
    const CommPageTable *pt = getPageTable(m_currentFile);
    int pageCount = pt ? pt->pageCount() : 0;
    emit navigationStateChanged(hasPrevInternal(), hasNextInternal(),
                                 m_currentFile, m_currentPage, pageCount);
}
