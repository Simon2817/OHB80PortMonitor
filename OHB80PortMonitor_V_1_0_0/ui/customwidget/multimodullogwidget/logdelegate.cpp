#include "logdelegate.h"
#include "logmodel.h"
#include <QApplication>
#include <QStyle>

LogDelegate::LogDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    // 设置等宽字体，适合日志显示
    m_font.setFamily("Consolas");
    m_font.setPointSize(9);
}

void LogDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    // 获取颜色
    QColor textColor = index.data(LogModel::ColorRole).value<QColor>();
    QString displayText = index.data(Qt::DisplayRole).toString();

    // 绘制背景（选中状态）
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
        textColor = option.palette.highlightedText().color();
    }

    // 设置字体和颜色
    QFont font = m_font;
    if (index.data(LogModel::LevelRole).toString() == "ERROR" || 
        index.data(LogModel::LevelRole).toString() == "CRITICAL") {
        font.setBold(true); // 错误和严重级别使用粗体
    }
    painter->setFont(font);
    painter->setPen(textColor);

    // 绘制文本
    QRect textRect = option.rect.adjusted(5, 2, -5, -2); // 添加边距
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, displayText);
}

QSize LogDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFontMetrics metrics(m_font);
    QString text = index.data(Qt::DisplayRole).toString();
    QSize size = metrics.size(Qt::TextSingleLine, text);
    
    // 添加边距
    return QSize(size.width() + 10, size.height() + 4);
}
