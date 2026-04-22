#pragma once
#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>
#include <QColor>

class CommLogPageTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CommLogPageTableModel(QObject *parent = nullptr);

    void setCsvHeaders(const QStringList &headers);
    void setRecords(const QVector<QStringList> &records);
    void setHighlightMask(const QVector<bool> &mask,
                          const QColor &color = QColor(173, 216, 230));
    const QStringList &recordAt(int row) const;

    int      rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int      columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QStringList          m_csvHeaders;
    QVector<QStringList> m_records;
    QVector<bool>        m_highlightMask;
    QColor               m_highlightColor;
};
