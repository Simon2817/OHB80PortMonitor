#ifndef RUNNINGLOGGERWIDGET_H
#define RUNNINGLOGGERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QDialog>
#include <QList>

#include "loggerwidget.h"

// ====================================================================
// RunningLoggerWidget —— 运行日志控件
//
// UI 布局：
//   初始只显示一个 QPushButton，按钮上实时滚动显示最新记录。
//   若存在未解决的警报记录，按钮始终轮播待处理警报消息，
//   直到所有警报解决后才恢复显示 Message 消息。
//   点击按钮弹出模态框，内嵌 LoggerWidget 完整界面。
//
// 公开方法：
//   1. writeRecord  —— 写入一条记录
//   2. resolveAlarm —— 根据警报ID标记某条警报已解决
// ====================================================================
class RunningLoggerWidget : public QWidget
{
    Q_OBJECT

public:
    // -------- 消息类型枚举 --------
    enum class MsgType {
        Message,
        Warn,
        Error
    };

    // ====================================================================
    // 
    // ====================================================================
    struct PendingAlarm {
        QString alarmId;
        MsgType type;
        QString qrCode;
        QString message;
        QString sendTime;
    };

    explicit RunningLoggerWidget(QWidget *parent = nullptr);
    ~RunningLoggerWidget();

    // -------- 配置（initialize 之前调用）--------
    void setRootPath(const QString &path);
    void setPageSize(int size);

    // -------- 初始化 --------
    // 内部完成：setHeaders / setFormat / setItemStyler / LoggerWidget::initialize
    void initialize();

    // -------- 写入一条记录 --------
    // 若为警报记录（Warn / Error 且 alarmId 非空），
    // alarmId 相同的记录会被忽略（去重），同时加入待处理警报队列。
    void writeRecord(MsgType type,
                     const QString &qrCode,
                     const QString &alarmId,
                     const QString &message);

    // -------- 解决一条警报 --------
    // 根据 alarmId 定位，将其从待处理警报队列中移除，
    // 并将该条记录（是否解决=是，解决时间=当前时间）写入日志控件。
    void resolveAlarm(MsgType type,
                      const QString &qrCode,
                      const QString &alarmId,
                      const QString &message);

signals:
    void logWritten(bool success);

private slots:
    void onBtnClicked();
    void onScrollTick();
    void onAlarmCycleTick();

private:
    // 刷新按钮当前应显示的完整文本
    void refreshDisplayText();

    // ---- UI 成员 ----
    QPushButton  *m_btn            = nullptr;
    LoggerWidget *m_loggerWidget   = nullptr;
    QDialog      *m_dialog         = nullptr;

    // ---- 待处理警报队列 ----
    QList<PendingAlarm> m_pendingAlarms;

    // ---- 最新 Message 文本（无警报时显示）----
    QString m_latestMessageText;

    // ---- 按钮跑马灯状态 ----
    QString m_fullDisplayText;    // 当前应显示的完整文本
    QTimer *m_scrollTimer    = nullptr;
    int     m_scrollOffset   = 0;

    // ---- 警报轮播状态 ----
    QTimer *m_alarmCycleTimer = nullptr;
    int     m_currentAlarmIdx = 0;

    // ---- 配置 ----
    QString m_rootPath;
    int     m_pageSize = 50;
};

#endif // RUNNINGLOGGERWIDGET_H
