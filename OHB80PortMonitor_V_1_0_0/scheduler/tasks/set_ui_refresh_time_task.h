#ifndef SET_UI_REFRESH_TIME_TASK_H
#define SET_UI_REFRESH_TIME_TASK_H

#include "../scheduler_task.h"
#include "modbustcpmastermanager/modbuscommand/modbuscommand.h"

#include <QAtomicInt>
#include <QHash>
#include <QList>
#include <QStringList>
#include <QTimer>
#include <QVector>

// ====================================================================
// SetUIRefreshTimeTask — 设置 UI 页面刷新时间（下位机屏幕）
//
//   底层指令：WriteUIRefreshTime（FC 0x10, addr 0x0004，2 寄存器，4 字节）
//   数据布局（大端）：
//     [0..1] logScreenSec      — log 界面显示时长（秒）
//     [2..3] propertyScreenSec — 属性页面显示时长（秒）
//
//   信号 allFinished：
//     allSuccess        — 是否全部成功
//     successCount      — 成功设备数
//     failedQrCodes     — 失败设备 QRCode 列表
//     logScreenSec      — 设置的 log 界面时长
//     propertyScreenSec — 设置的属性页面时长
// ====================================================================
class SetUIRefreshTimeTask : public SchedulerTask
{
    Q_OBJECT

public:
    explicit SetUIRefreshTimeTask(const QVector<QString> &qrcodes,
                                  int logScreenSec,
                                  int propertyScreenSec,
                                  QObject *parent = nullptr);
    ~SetUIRefreshTimeTask();

    void start() override;
    void stop()  override;
    QString taskType() const override { return "SetUIRefreshTimeTask"; }

signals:
    void allFinished(bool allSuccess,
                     int successCount,
                     QStringList failedQrCodes,
                     int logScreenSec,
                     int propertyScreenSec);

private slots:
    void onCommandFinished(ModbusCommand cmd, const QString &masterId);
    void onTimeout();

private:
    QByteArray buildPayload() const;
    void disconnectAll();
    void checkAllFinished();
    void forceFinish();

private:
    QVector<QString> m_qrcodes;
    int              m_logScreenSec;
    int              m_propertyScreenSec;

    QHash<qint64, QString>         m_pendingMap;
    QList<QMetaObject::Connection> m_connections;
    int        m_totalCount = 0;
    QAtomicInt m_completedCount{0};
    bool       m_stopped = false;

    int         m_successCount = 0;
    QStringList m_failedQrCodes;

    QTimer *m_timeoutTimer       = nullptr;
    bool    m_allFinishedEmitted = false;
};

#endif // SET_UI_REFRESH_TIME_TASK_H
