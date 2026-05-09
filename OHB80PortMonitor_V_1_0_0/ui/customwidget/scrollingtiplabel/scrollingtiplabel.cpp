#include "scrollingtiplabel.h"

#include <QFontMetrics>
#include "app/alarmtype.h"
#include "data/logdatabases/dbtypes.h"

ScrollingTipLabel::ScrollingTipLabel(QWidget *parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setWordWrap(false);

    m_scrollTimer = new QTimer(this);
    connect(m_scrollTimer, &QTimer::timeout, this, &ScrollingTipLabel::onScrollTimer);

    // 初始状态：无消息，隐藏控件
    updateDisplay();
}

ScrollingTipLabel::~ScrollingTipLabel()
{
    stopScrolling();
}

void ScrollingTipLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
    QLabel::mousePressEvent(event);
}

static QString makeAlarmKey(const AlarmRecord& record)
{
    return QStringLiteral("%1|%2").arg(record.qrCode).arg(record.alarmType);
}

void ScrollingTipLabel::submitAlarmLog(const AlarmRecord& record)
{
    const QString key = makeAlarmKey(record);
    m_alarmLogs[key] = record;
    if (!m_alarmQueue.contains(key))
        m_alarmQueue.enqueue(key);

    updateDisplay();
}

void ScrollingTipLabel::submitAlarmResolved(const AlarmRecord& record)
{
    const QString key = makeAlarmKey(record);

    // 从警报队列中移除对应的警报记录
    QQueue<QString> newQueue;
    while (!m_alarmQueue.isEmpty()) {
        QString k = m_alarmQueue.dequeue();
        if (k != key) {
            newQueue.enqueue(k);
        }
    }
    m_alarmQueue = newQueue;

    // 从映射中移除
    m_alarmLogs.remove(key);

    // 更新显示
    updateDisplay();
}

void ScrollingTipLabel::submitOperationLog(const OperationRecord& record)
{
    m_operationLog = record;
    updateDisplay();
}

QString ScrollingTipLabel::getCurrentDisplayText()
{
    if (!m_alarmQueue.isEmpty())
        return formatAlarmRecord(m_alarmLogs.value(m_alarmQueue.last()));

    return formatOperationRecord(m_operationLog);
}

void ScrollingTipLabel::updateDisplay()
{
    QString text = getCurrentDisplayText();

    if (text.isEmpty()) {
        setText("");
        stopScrolling();
        updateStyle(false);
        hide();  // 没有消息时隐藏控件
        return;
    }

    show();  // 有消息时显示控件

    // 判断是否为警报模式
    bool isAlarm = !m_alarmQueue.isEmpty();
    m_isAlarmMode = isAlarm;

    // 更新样式
    updateStyle(isAlarm);

    QFontMetrics fm(font());
    int labelWidth = width();
    m_textWidth = fm.horizontalAdvance(text);

    // 字符串长度大于 label 显示范围，滚动显示
    if (m_textWidth > labelWidth) {
        m_currentText = text;
        m_scrollOffset = 0;
        setText(text);
        startScrolling();
    }
    // 字符串长度小于等于 label 显示范围，居中显示
    else {
        setAlignment(Qt::AlignCenter);
        setText(text);
        stopScrolling();
    }
}

void ScrollingTipLabel::updateStyle(bool isAlarm)
{
    if (isAlarm) {
        // 警报模式样式：红色背景，更显眼
        setStyleSheet(
            "QLabel {"
            "  background-color: #fff5f5;"
            "  border: 1px solid #ff6b6b;"
            "  border-radius: 4px;"
            "  padding: 6px 10px;"
            "  color: #c92a2a;"
            "  font-size: 13px;"
            "  font-weight: 500;"
            "}"
        );
    } else {
        // 普通模式样式：浅色背景，简洁
        setStyleSheet(
            "QLabel {"
            "  background-color: #f8f9fa;"
            "  border: 1px solid #dee2e6;"
            "  border-radius: 4px;"
            "  padding: 6px 10px;"
            "  color: #495057;"
            "  font-size: 13px;"
            "}"
        );
    }
}

void ScrollingTipLabel::startScrolling()
{
    if (m_scrollTimer && !m_scrollTimer->isActive()) {
        setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_scrollTimer->start(kScrollIntervalMs);
    }
}

void ScrollingTipLabel::stopScrolling()
{
    if (m_scrollTimer && m_scrollTimer->isActive()) {
        m_scrollTimer->stop();
    }
    m_scrollOffset = 0;
}

QString ScrollingTipLabel::formatAlarmRecord(const AlarmRecord& record) const
{
    if (record.occurTime.isEmpty() && record.description.isEmpty())
        return {};
    return QString("[%1][%2] %3: %4")
        .arg(record.occurTime,
             alarmLevelName(record.alarmLevel),
             alarmTypeName(record.alarmType),
             record.description);
}

QString ScrollingTipLabel::formatOperationRecord(const OperationRecord& record) const
{
    if (record.occurTime.isEmpty() && record.description.isEmpty())
        return {};
    return QString("[%1] %2: %3")
        .arg(record.occurTime,
             LogDB::operationLogTypeName(record.logType),
             record.description);
}

void ScrollingTipLabel::onScrollTimer()
{
    if (m_currentText.isEmpty()) {
        stopScrolling();
        return;
    }

    QFontMetrics fm(font());
    int labelWidth = width();

    // 更新滚动偏移
    m_scrollOffset += kScrollStep;

    // 检查是否滚动完毕
    if (m_scrollOffset >= m_textWidth) {
        // 整个字符串滚动完毕，从头开始
        m_scrollOffset = 0;
    }

    // 计算显示的文本（使用滚动偏移）
    QString displayText = m_currentText;

    // 如果文本宽度超过标签宽度，使用滚动效果
    if (m_textWidth > labelWidth) {
        // 简单的滚动实现：从 offset 开始显示
        int charsToSkip = fm.horizontalAdvance(displayText.left(m_scrollOffset));
        // 这里简化处理，实际可能需要更精确的字符级滚动
        // 暂时使用省略号表示滚动效果
        if (m_scrollOffset > 0) {
            displayText = displayText.mid(m_scrollOffset / 8); // 粗略估计字符宽度
            displayText += " ... " + m_currentText.left(m_scrollOffset / 8);
        }
    }

    setText(displayText);
}
