#ifndef LOGDELEGATE_H
#define LOGDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QFont>

class LogDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LogDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QFont m_font;
};

#endif // LOGDELEGATE_H
