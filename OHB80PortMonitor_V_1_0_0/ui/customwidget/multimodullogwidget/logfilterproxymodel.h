#ifndef LOGFILTERPROXYMODEL_H
#define LOGFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QString>

class LogFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit LogFilterProxyModel(QObject *parent = nullptr);

    // 设置过滤的QRCode
    void setFilterQRCode(const QString &qrcode);
    QString filterQRCode() const { return m_filterQRCode; }

    // 设置是否启用过滤
    void setFilterEnabled(bool enabled);
    bool isFilterEnabled() const { return m_filterEnabled; }

protected:
    // 重写过滤方法
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_filterQRCode;
    bool m_filterEnabled;
};

#endif // LOGFILTERPROXYMODEL_H
