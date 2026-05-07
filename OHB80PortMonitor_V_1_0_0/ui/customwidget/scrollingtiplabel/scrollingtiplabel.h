#ifndef SCROLLINGTIPLABEL_H
#define SCROLLINGTIPLABEL_H

#include <QLabel>
#include <QStringList>
#include <QQueue>
#include <QTimer>

// ====================================================================
// ScrollingTipLabel — 滚动公告栏控件
//
//   功能概述：
//     - 实时显示操作日志和警报日志
//     - 警报日志优先级最高，优先显示警报
//     - 支持滚动显示消息
//     - 支持消除消息机制
//
//   管理对象：
//     1. m_alarmQueue: 警报队列，存放 AlarmLogDBCon 的主键 id
//     2. m_operationLog: 存放一条 OperationLogDBCon 记录（QStringList）
//
//   公开接口：
//     - submitAlarmLog(const QStringList& operationLog, int alarmRecordId)
//       提交一个警报日志
//     - submitAlarmResolved(int alarmRecordId)
//       提交一个警报已解决日志
//     - submitOperationLog(const QStringList& operationLog)
//       提交一个 OperationLogDBCon 记录
//
//   私有方法：
//     - getCurrentDisplayLog()
//       获取当前待显示的日志
//
//   功能实现：
//     1. 滚动显示消息
//        - 字符串长度 > label 显示范围：滚动显示
//        - 字符串长度 <= label 显示范围：居中显示
//     2. 消除消息机制
//        - 警报已解决：从 alarm 队列中删除对应 id
//        - 提交 OperationLog：替换当前记录
//     3. 获取当前待显示日志
//        - alarm 队列不空：返回最后一个警报
//        - alarm 队列为空：返回 OperationLog 记录
// ====================================================================
class ScrollingTipLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ScrollingTipLabel(QWidget *parent = nullptr);
    ~ScrollingTipLabel();

    // 提交一个警报日志
    // operationLog: OperationLogDBCon 记录（QStringList）
    // alarmRecordId: AlarmLogDBCon 的警报记录的主键 id
    void submitAlarmLog(const QStringList& operationLog, int alarmRecordId);

    // 提交一个警报已解决日志
    // alarmRecordId: AlarmLogDBCon 的警报记录的主键 id
    void submitAlarmResolved(int alarmRecordId);

    // 提交一个 OperationLogDBCon 记录
    void submitOperationLog(const QStringList& operationLog);

private:
    // 获取当前待显示的日志
    QStringList getCurrentDisplayLog();

    // 更新显示内容
    void updateDisplay();

    // 更新样式（根据是否显示警报）
    void updateStyle(bool isAlarm);

    // 滚动显示逻辑
    void startScrolling();
    void stopScrolling();

    // 格式化日志为显示字符串
    QString formatLogToString(const QStringList& log);

private slots:
    void onScrollTimer();

private:
    // 管理对象
    QQueue<int>      m_alarmQueue;       // 警报队列，存放 AlarmLogDBCon 的主键 id
    QStringList      m_operationLog;     // 存放一条 OperationLogDBCon 记录
    QHash<int, QStringList> m_alarmLogs;  // alarmRecordId -> operationLog 映射

    // 滚动相关
    QTimer          *m_scrollTimer = nullptr;
    int              m_scrollOffset = 0;
    QString          m_currentText;
    int              m_textWidth = 0;

    // 状态相关
    bool             m_isAlarmMode = false;  // 当前是否显示警报

    // 滚动配置
    static constexpr int kScrollIntervalMs = 100;  // 滚动间隔
    static constexpr int kScrollStep = 2;           // 每次滚动像素
};

#endif // SCROLLINGTIPLABEL_H
