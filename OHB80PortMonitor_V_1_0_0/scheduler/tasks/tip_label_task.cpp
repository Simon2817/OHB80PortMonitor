#include "tip_label_task.h"

#include <QDebug>
#include <QMutexLocker>

TipLabelTask::TipLabelTask(QObject* parent)
    : SchedulerTask(parent)
{
    qDebug() << "[TipLabelTask] constructed";
}

void TipLabelTask::start()
{
    m_stopped = false;
    setState(Running);
    qDebug() << "[TipLabelTask] started";
    emit progress(0, QStringLiteral("TipLabel task ready"));
}

void TipLabelTask::stop()
{
    m_stopped = true;
    setState(Cancelled);
    emit finished(false, QStringLiteral("TipLabel task stopped"));
    qDebug() << "[TipLabelTask] stopped";
}

void TipLabelTask::submitAlarmLog(const QStringList& operationLog, int alarmRecordId)
{
    {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue({ Record::AlarmLog, operationLog, alarmRecordId });
    }
    QMetaObject::invokeMethod(this, "processQueue", Qt::QueuedConnection);
}

void TipLabelTask::submitAlarmResolved(int alarmRecordId)
{
    {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue({ Record::AlarmResolved, {}, alarmRecordId });
    }
    QMetaObject::invokeMethod(this, "processQueue", Qt::QueuedConnection);
}

void TipLabelTask::submitOperationLog(const QStringList& operationLog)
{
    {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue({ Record::OperationLog, operationLog, 0 });
    }
    QMetaObject::invokeMethod(this, "processQueue", Qt::QueuedConnection);
}

void TipLabelTask::processQueue()
{
    if (m_stopped) return;

    QQueue<Record> local;
    {
        QMutexLocker locker(&m_mutex);
        local.swap(m_queue);
    }

    while (!local.isEmpty()) {
        const Record rec = local.dequeue();
        switch (rec.type) {
        case Record::AlarmLog:
            emit alarmLogReady(rec.operationLog, rec.alarmRecordId);
            break;
        case Record::AlarmResolved:
            emit alarmResolvedReady(rec.alarmRecordId);
            break;
        case Record::OperationLog:
            emit operationLogReady(rec.operationLog);
            break;
        }
    }
}
