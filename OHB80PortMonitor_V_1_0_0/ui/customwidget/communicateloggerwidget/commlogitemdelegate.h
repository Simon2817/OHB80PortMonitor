#pragma once
#include <QStyledItemDelegate>
#include <functional>
#include <QStringList>
#include "commitemstyle.h"

class CommLogPageModel;

class CommLogItemDelegate : public QStyledItemDelegate
{
public:
    using Styler = std::function<void(const QStringList &, CommItemStyle &)>;
    explicit CommLogItemDelegate(QObject *parent = nullptr);

    void setStyler(Styler fn);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

private:
    Styler m_styler;
};
