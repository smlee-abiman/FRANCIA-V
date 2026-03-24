#ifndef BITARRAYLISTMODEL_H
#define BITARRAYLISTMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QString>
#include <QVector>

class BitArrayListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int labelCount READ labelCount NOTIFY labelCountChanged)

public:
    explicit BitArrayListModel(QObject *parent = nullptr);

    int labelCount() const;
    void clearLabels();
    void addLabels(const QVector<QString> &labels);

    void setValues(const QByteArray &values);
    void setValueBlockLayout(int bitsPerBlock, int blockBytes);

    int mirrorPages() const;
    void setMirrorPages(int pages);

    enum Roles {
        LabelRole = Qt::UserRole + 1,
        ValueRole,
        // remember to update roleNames() as well
    };

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
    void labelCountChanged(int count);

private:
    QVector<QString> m_labels;
    QByteArray m_values;
    int m_bitsPerBlock;
    int m_blockBytes;
    int m_mirrorPages;
    int m_rowCount; // will converge to m_labels.size() * m_mirrorPages
};

#endif // BITARRAYLISTMODEL_H
