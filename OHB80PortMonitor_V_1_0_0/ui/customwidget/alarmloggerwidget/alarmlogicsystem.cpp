#include "alarmlogicsystem.h"
#include <QDateTime>
#include <QFutureWatcher>
#include <QtConcurrent>

AlarmLogicSystem::AlarmLogicSystem(QObject *parent)
    : QObject(parent)
{}

AlarmLogicSystem::AlarmLogicSystem(const AlarmHeaderConfig &config, QObject *parent)
    : QObject(parent)
    , m_fs(config)
{}

void AlarmLogicSystem::setRootDir(const QString &dir)
{
    m_fs.setRootDir(dir);
}

QString AlarmLogicSystem::rootDir() const
{
    return m_fs.rootDir();
}

void AlarmLogicSystem::setHeaders(const QStringList &headers)
{
    m_fs.setHeaders(headers);
}

QStringList AlarmLogicSystem::headers() const
{
    return m_fs.headers();
}

void AlarmLogicSystem::submitAlarm(AlarmLevel level, const QString &qrCode,
                                   qint64 alarmId, const QString &message)
{
    if (m_active.contains(alarmId))
        return;

    const AlarmInfo info(level, qrCode, alarmId, message);
    m_active.insert(alarmId, info);

    emit alarmPublished(info);
}

void AlarmLogicSystem::submitResolve(qint64 alarmId)
{
    auto it = m_active.find(alarmId);
    if (it == m_active.end())
        return;

    AlarmInfo info = it.value();
    m_active.erase(it);

    info.setResolved(true);
    info.setResolveTime(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    writeAlarmAsync(info);  // 异步写入，不阻塞 UI

    emit alarmResolved(info);
}

bool AlarmLogicSystem::isActive(qint64 alarmId) const
{
    return m_active.contains(alarmId);
}

int AlarmLogicSystem::activeCount() const
{
    return m_active.size();
}

void AlarmLogicSystem::clearActive()
{
    m_active.clear();
}

QList<QStringList> AlarmLogicSystem::readDate(const QDate &date) const
{
    return m_fs.readDate(date);
}

QSet<QDate> AlarmLogicSystem::availableDates() const
{
    QMutexLocker locker(&m_fsMutex);
    return m_fs.availableDates();
}

// ----------------------------------------------------------------
// 异步接口
// ----------------------------------------------------------------

void AlarmLogicSystem::writeAlarmAsync(const AlarmInfo &info)
{
    auto *watcher = new QFutureWatcher<bool>(this);
    connect(watcher, &QFutureWatcher<bool>::finished,
            this, [this, watcher]() {
                emit writeAlarmFinished(watcher->result());
                watcher->deleteLater();
            });
    watcher->setFuture(QtConcurrent::run([this, info]() -> bool {
        QMutexLocker locker(&m_fsMutex);
        return m_fs.writeAlarm(info);
    }));
}

void AlarmLogicSystem::readDateAsync(const QDate &date)
{
    auto *watcher = new QFutureWatcher<QList<QStringList>>(this);
    connect(watcher, &QFutureWatcher<QList<QStringList>>::finished,
            this, [this, watcher]() {
                emit readDateFinished(watcher->result());
                watcher->deleteLater();
            });
    watcher->setFuture(QtConcurrent::run([this, date]() -> QList<QStringList> {
        QMutexLocker locker(&m_fsMutex);
        return m_fs.readDate(date);
    }));
}

void AlarmLogicSystem::availableDatesAsync()
{
    auto *watcher = new QFutureWatcher<QSet<QDate>>(this);
    connect(watcher, &QFutureWatcher<QSet<QDate>>::finished,
            this, [this, watcher]() {
                emit availableDatesReady(watcher->result());
                watcher->deleteLater();
            });
    watcher->setFuture(QtConcurrent::run([this]() -> QSet<QDate> {
        QMutexLocker locker(&m_fsMutex);
        return m_fs.availableDates();
    }));
}

void AlarmLogicSystem::purgeOldLogsAsync(int maxMonths)
{
    auto *watcher = new QFutureWatcher<int>(this);
    connect(watcher, &QFutureWatcher<int>::finished,
            this, [this, watcher]() {
                emit purgeFinished(watcher->result());
                watcher->deleteLater();
            });
    watcher->setFuture(QtConcurrent::run([this, maxMonths]() -> int {
        QMutexLocker locker(&m_fsMutex);
        return m_fs.purgeOldLogs(maxMonths);
    }));
}
