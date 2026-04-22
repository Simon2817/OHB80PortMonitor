#include "commlogtableitemdelegate.h"
#include "commlogpagetablemodel.h"
#include <QPainter>
#include <QStyle>

CommLogTableItemDelegate::CommLogTableItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void CommLogTableItemDelegate::setStyler(Styler fn)
{
    m_styler = std::move(fn);
}

void CommLogTableItemDelegate::paint(QPainter *painter,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    CommItemStyle itemStyle;
    bool hasStyle = false;
    if (m_styler) {
        const CommLogPageTableModel *m =
            qobject_cast<const CommLogPageTableModel*>(index.model());
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

    // 根据表头名决定对齐方式：Request/Response 左对齐，其他列居中
    const QString header = index.model()->headerData(index.column(), Qt::Horizontal).toString();
    int alignment = (header == QLatin1String("Request") || header == QLatin1String("Response"))
        ? (Qt::AlignLeft | Qt::AlignVCenter)
        : Qt::AlignCenter;
    painter->drawText(textRect, alignment, opt.text);

    painter->restore();
}
