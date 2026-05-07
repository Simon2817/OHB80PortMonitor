#include "logcleanupscheduler.h"
#include <QTimer>
#include <QDebug>

namespace LogDB {

LogCleanupScheduler::LogCleanupScheduler(const Config& config, QObject* parent)
    : QObject(parent)
    , m_config(config)
    , m_timer(nullptr)
    , m_lastCheckedDate(QDate::currentDate())
{
}

LogCleanupScheduler::~LogCleanupScheduler()
{
    stop();
}

void LogCleanupScheduler::setMonthRangeProvider(MonthRangeProvider provider)
{
    m_monthRangeProvider = std::move(provider);
}

void LogCleanupScheduler::setDeleteByRangeFn(DeleteByRangeFn fn)
{
    m_deleteByRangeFn = std::move(fn);
}

void LogCleanupScheduler::start()
{
    if (!m_timer) {
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &LogCleanupScheduler::onTimerTick);
    }
    if (!m_timer->isActive()) {
        m_timer->start(m_config.checkIntervalMs);
    }
}

void LogCleanupScheduler::stop()
{
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
    }
}

void LogCleanupScheduler::triggerCleanupCheck()
{
    performCleanupCheck();
}

void LogCleanupScheduler::onTimerTick()
{
    QDate currentDate = QDate::currentDate();
    if (currentDate == m_lastCheckedDate) {
        return; // 同一天内不重复检查
    }
    m_lastCheckedDate = currentDate;
    performCleanupCheck();
}

void LogCleanupScheduler::performCleanupCheck()
{
    if (!m_monthRangeProvider || !m_deleteByRangeFn) {
        qWarning() << "[LogCleanupScheduler] Provider or delete function not set";
        return;
    }

    QVariantMap monthRange = m_monthRangeProvider();
    QString earliestDate = monthRange.value("earliest_date").toString();
    QString latestDate = monthRange.value("latest_time").toString();

    if (earliestDate.isEmpty() || latestDate.isEmpty()) {
        return;
    }

    int monthDiff = calculateMonthDifference(earliestDate, latestDate);
    if (monthDiff <= m_config.retainMonths) {
        return;
    }

    // 计算要删除的时间区间：从最早日期开始到 +cleanupMonths 个月
    QDate earliest = QDate::fromString(earliestDate.left(10), "yyyy-MM-dd");
    if (!earliest.isValid()) {
        qWarning() << "[LogCleanupScheduler] Invalid earliest date:" << earliestDate;
        return;
    }
    QDate cutoffDate = earliest.addMonths(m_config.cleanupMonths);

    QString startTime = earliest.toString("yyyy-MM-dd 00:00:00");
    QString endTime = cutoffDate.toString("yyyy-MM-dd 23:59:59");

    qDebug() << "[LogCleanupScheduler] Cleaning up logs from"
             << startTime << "to" << endTime
             << "(monthDiff=" << monthDiff << ")";

    m_deleteByRangeFn(startTime, endTime);
}

int LogCleanupScheduler::calculateMonthDifference(const QString& earliestDate, const QString& latestDate)
{
    QDate earliest = QDate::fromString(earliestDate.left(10), "yyyy-MM-dd");
    QDate latest = QDate::fromString(latestDate.left(10), "yyyy-MM-dd");
    if (!earliest.isValid() || !latest.isValid()) {
        return 0;
    }
    int years = latest.year() - earliest.year();
    int months = latest.month() - earliest.month();
    return years * 12 + months;
}

} // namespace LogDB
