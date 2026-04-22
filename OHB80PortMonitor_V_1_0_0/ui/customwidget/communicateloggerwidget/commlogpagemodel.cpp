#include "commlogpagemodel.h"

CommLogPageModel::CommLogPageModel(QObject *parent) : QAbstractListModel(parent) {}

void CommLogPageModel::setCsvHeaders(const QStringList &headers)
{
    m_csvHeaders = headers;
    rebuildSlots();
}

void CommLogPageModel::setFormat(const QString &format, const QStringList &args)
{
    m_format     = format;
    m_formatArgs = args;
    rebuildSlots();
}

void CommLogPageModel::rebuildSlots()
{
    m_slots.clear();
    if (m_format.isEmpty() || m_formatArgs.isEmpty()) return;

    static const QString kClose = QStringLiteral("])}>");
    QStringList parts = m_format.split("{}");
    int n = qMin(parts.size() - 1, m_formatArgs.size());
    QString nextPrefix = parts[0];
    for (int i = 0; i < n; ++i) {
        FieldSlot slot;
        slot.colIndex = m_csvHeaders.indexOf(m_formatArgs[i]);
        slot.prefix   = nextPrefix;
        const QString inter = (i + 1 < parts.size()) ? parts[i + 1] : QString();
        int slen = 0;
        while (slen < inter.size() && kClose.contains(inter[slen])) ++slen;
        slot.suffix = inter.left(slen);
        nextPrefix  = inter.mid(slen);
        m_slots.append(slot);
    }
    if (!m_slots.isEmpty())
        m_slots.last().suffix += nextPrefix;
    beginResetModel();
    endResetModel();
}

void CommLogPageModel::setRecords(const QVector<QStringList> &records)
{
    beginResetModel();
    m_records = records;
    m_highlightMask.clear();
    endResetModel();
}

void CommLogPageModel::setHighlightMask(const QVector<bool> &mask, const QColor &color)
{
    m_highlightMask  = mask;
    m_highlightColor = color;
    if (!m_records.isEmpty())
        emit dataChanged(index(0), index(m_records.size() - 1), {Qt::BackgroundRole});
}

const QStringList &CommLogPageModel::recordAt(int row) const
{
    return m_records.at(row);
}

int CommLogPageModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_records.size();
}

QVariant CommLogPageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_records.size()) return QVariant();
    if (role == Qt::DisplayRole) return formatRow(m_records[index.row()]);
    if (role == Qt::BackgroundRole) {
        if (index.row() < m_highlightMask.size() && m_highlightMask[index.row()])
            return m_highlightColor;
    }
    return QVariant();
}

QString CommLogPageModel::formatRow(const QStringList &record) const
{
    if (!m_slots.isEmpty()) {
        QString result;
        for (const FieldSlot &slot : m_slots) {
            if (slot.colIndex < 0 || slot.colIndex >= record.size()) continue;
            const QString val = record[slot.colIndex].trimmed();
            if (val.isEmpty()) continue;
            result += slot.prefix + val + slot.suffix;
        }
        return result;
    }
    QStringList fields;
    for (const QString &f : record)
        if (!f.trimmed().isEmpty()) fields.append(f);
    if (fields.isEmpty()) return QString();
    QString result;
    for (int i = 0; i < fields.size(); ++i)
        result += (i < fields.size() - 1) ? '[' + fields[i] + ']'
                                           : QStringLiteral(": ") + fields[i];
    return result;
}
