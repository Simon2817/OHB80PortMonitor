#include "logfilterproxymodel.h"
#include "logmodel.h"
#include <QDebug>

LogFilterProxyModel::LogFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_filterEnabled(false)
{
    // 设置动态过滤
    setDynamicSortFilter(true);
}

void LogFilterProxyModel::setFilterQRCode(const QString &qrcode)
{
    if (m_filterQRCode != qrcode) {
        m_filterQRCode = qrcode;
        invalidateFilter(); // 重新过滤
        qDebug() << "设置过滤QRCode:" << qrcode << "过滤后行数:" << rowCount();
    }
}

void LogFilterProxyModel::setFilterEnabled(bool enabled)
{
    if (m_filterEnabled != enabled) {
        m_filterEnabled = enabled;
        invalidateFilter(); // 重新过滤
        qDebug() << "设置过滤启用:" << enabled << "过滤后行数:" << rowCount();
    }
}

bool LogFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)
    
    // 如果没有启用过滤，显示所有行
    if (!m_filterEnabled || m_filterQRCode.isEmpty()) {
        return false; // 返回false表示不显示任何行
    }
    
    // 获取源模型的数据
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0);
    if (!sourceIndex.isValid()) {
        return false;
    }
    
    // 获取日志消息
    QString message = sourceModel()->data(sourceIndex, LogModel::MessageRole).toString();
    
    // 检查消息是否包含过滤的QRCode
    bool contains = message.contains(m_filterQRCode);
    
    return contains;
}
