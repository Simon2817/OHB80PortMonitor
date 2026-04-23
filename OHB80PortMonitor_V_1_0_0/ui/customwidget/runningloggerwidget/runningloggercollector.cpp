#include "runningloggercollector.h"

#include <QGlobalStatic>

// Q_GLOBAL_STATIC: 线程安全的懒初始化，程序退出时自动析构
Q_GLOBAL_STATIC(RunningLoggerCollector, s_instance)

// =====================================================================
// 单例获取
// =====================================================================
RunningLoggerCollector* RunningLoggerCollector::instance()
{
    return s_instance;
}

// =====================================================================
// 构造
// =====================================================================
RunningLoggerCollector::RunningLoggerCollector(QObject* parent)
    : QObject(parent)
{
    // 定时器运行于主线程，每 50ms 将缓冲队列中的条目提交给 RunningLoggerWidget
    m_flushTimer = new QTimer(this);
    m_flushTimer->setInterval(50);
    connect(m_flushTimer, &QTimer::timeout, this, &RunningLoggerCollector::onFlushTick);
    m_flushTimer->start();
}

// =====================================================================
// 绑定目标控件（须在主线程调用）
// =====================================================================
void RunningLoggerCollector::setTarget(RunningLoggerWidget* widget)
{
    m_target = widget;
}

// =====================================================================
// 线程安全：写入一条日志（任意线程可调用）
// =====================================================================
void RunningLoggerCollector::log(RunningLoggerWidget::MsgType type,
                                 const QString& qrCode,
                                 const QString& alarmId,
                                 const QString& message)
{
    QMutexLocker locker(&m_queueMutex);
    m_queue.enqueue({ type, qrCode, alarmId, message });
}

void RunningLoggerCollector::logMessage(const QString& message)
{
    log(RunningLoggerWidget::MsgType::Message, QString(), QString(), message);
}

// =====================================================================
// 定时刷新：在主线程将缓冲队列中的条目提交给 RunningLoggerWidget
// =====================================================================
void RunningLoggerCollector::onFlushTick()
{
    if (!m_target) return;

    // 队列为空时快速返回，避免不必要的 swap 开销
    {
        QMutexLocker locker(&m_queueMutex);
        if (m_queue.isEmpty()) return;
    }

    // 每次最多提交一批，避免单帧阻塞 UI
    QQueue<LogEntry> localQueue;
    {
        QMutexLocker locker(&m_queueMutex);
        localQueue.swap(m_queue);
    }

    // 每帧最多提交 kMaxFlushPerTick 条，剩余条目下一帧继续消费
    int count = 0;
    while (!localQueue.isEmpty() && count < kMaxFlushPerTick) {
        const LogEntry entry = localQueue.dequeue();
        m_target->writeRecord(entry.type, entry.qrCode, entry.alarmId, entry.message);
        ++count;
    }

    // 若本帧有剩余，将其归还队列头部（保持顺序）
    if (!localQueue.isEmpty()) {
        QMutexLocker locker(&m_queueMutex);
        while (!localQueue.isEmpty()) {
            m_queue.prepend(localQueue.takeLast());
        }
    }
}
