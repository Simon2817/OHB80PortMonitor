#include "commlogpagetablemodel.h"

CommLogPageTableModel::CommLogPageTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

void CommLogPageTableModel::setCsvHeaders(const QStringList &headers)
{
    beginResetModel();
    m_csvHeaders = headers;
    endResetModel();
}

void CommLogPageTableModel::setRecords(const QVector<QStringList> &records)
{
    beginResetModel();
    m_records = records;
    m_highlightMask.clear();
    endResetModel();
}

void CommLogPageTableModel::setHighlightMask(const QVector<bool> &mask, const QColor &color)
{
    m_highlightMask  = mask;
    m_highlightColor = color;
    if (!m_records.isEmpty() && !m_csvHeaders.isEmpty())
        emit dataChanged(index(0, 0),
                         index(m_records.size() - 1, m_csvHeaders.size() - 1),
                         {Qt::BackgroundRole});
}

const QStringList &CommLogPageTableModel::recordAt(int row) const
{
    return m_records.at(row);
}

int CommLogPageTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_records.size();
}

int CommLogPageTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_csvHeaders.size();
}

QVariant CommLogPageTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_records.size()) return QVariant();
    const QStringList &record = m_records[index.row()];
    if (role == Qt::DisplayRole) {
        if (index.column() < record.size())
            return record[index.column()].trimmed();
        return QVariant();
    }
    if (role == Qt::BackgroundRole) {
        if (index.row() < m_highlightMask.size() && m_highlightMask[index.row()])
            return m_highlightColor;
    }
    return QVariant();
}

QVariant CommLogPageTableModel::headerData(int section, Qt::Orientation orientation,
                                            int role) const
{
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Horizontal) {
        if (section < m_csvHeaders.size()) return m_csvHeaders[section];
        return QVariant();
    }
    return section + 1;
}
