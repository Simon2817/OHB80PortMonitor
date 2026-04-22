#include "commlogitemdelegate.h"
#include "commlogpagemodel.h"
#include <QPainter>
#include <QStyle>

CommLogItemDelegate::CommLogItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void CommLogItemDelegate::setStyler(Styler fn)
{
    m_styler = std::move(fn);
}

void CommLogItemDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    CommItemStyle itemStyle;
    bool hasStyle = false;
    if (m_styler) {
        const CommLogPageModel *m = qobject_cast<const CommLogPageModel*>(index.model());
        if (m && index.row() < m->rowCount()) {
            m_styler(m->recordAt(index.row()), itemStyle);
            hasStyle = true;
        }
    }

    painter->save();

    if (opt.state & QStyle::State_Selected) {
        painter->fillRect(opt.rect, opt.palette.highlight());
    } else if (hasStyle && itemStyle.applyBackground) {
        painter->fillRect(opt.rect, itemStyle.background);
    } else {
        QVariant bg = index.data(Qt::BackgroundRole);
        if (bg.isValid())
            painter->fillRect(opt.rect, bg.value<QColor>());
    }

    QColor textColor;
    if (hasStyle && itemStyle.applyForeground)
        textColor = itemStyle.foreground;
    else if (opt.state & QStyle::State_Selected)
        textColor = opt.palette.color(QPalette::HighlightedText);
    else
        textColor = opt.palette.color(QPalette::Text);

    if (hasStyle && itemStyle.applyFont)
        painter->setFont(itemStyle.font);

    painter->setPen(textColor);
    QRect textRect = opt.rect.adjusted(4, 0, -4, 0);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, opt.text);

    painter->restore();
}
