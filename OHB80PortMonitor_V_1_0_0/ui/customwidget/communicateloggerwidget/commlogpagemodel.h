#pragma once
#include <QAbstractListModel>
#include <QStringList>
#include <QVector>
#include <QColor>

class CommLogPageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CommLogPageModel(QObject *parent = nullptr);

    void setCsvHeaders(const QStringList &headers);
    void setFormat(const QString &format, const QStringList &args);
    void setRecords(const QVector<QStringList> &records);
    void setHighlightMask(const QVector<bool> &mask,
                          const QColor &color = QColor(173, 216, 230));
    const QStringList &recordAt(int row) const;
    int      rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QString  formatRow(const QStringList &record) const;

private:
    struct FieldSlot {
        int     colIndex;
        QString prefix;
        QString suffix;
    };
    void rebuildSlots();

    QStringList          m_csvHeaders;
    QString              m_format;
    QStringList          m_formatArgs;
    QList<FieldSlot>     m_slots;
    QVector<QStringList> m_records;
    QVector<bool>        m_highlightMask;
    QColor               m_highlightColor;
};
