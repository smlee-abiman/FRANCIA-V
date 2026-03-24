#include <QtDebug>
#include <QByteArray>
#include <QHash>
#include "bitarraylistmodel.h"

BitArrayListModel::BitArrayListModel(QObject *parent)
    : QAbstractListModel(parent),
      m_labels(),
      m_values(),
      m_bitsPerBlock(8),
      m_blockBytes(1),
      m_mirrorPages(1),
      m_rowCount(0)
{
}

/// Returns the number of the labels (i.e. rowCount() not including the
/// mirrored indices.)
int BitArrayListModel::labelCount() const
{
    return m_labels.size();
}

/// Removes labels (i.e. rows) from the model.
///
/// The underlying bit-array data will remain unchanged, but the rowCount()
/// will be set to 0.
void BitArrayListModel::clearLabels()
{
    if (m_labels.empty())
        return;
    beginRemoveRows(QModelIndex(), 0, m_rowCount - 1);
    m_labels.clear();
    m_rowCount = 0;
    endRemoveRows();
    emit labelCountChanged(0);
}

/// Adds labels (i.e. rows) to the end of the model.
void BitArrayListModel::addLabels(const QVector<QString> &labels)
{
    if (labels.empty())
        return;
    if (m_labels.size() * m_mirrorPages != m_rowCount) {
        // since begin/endInsertRows() emit signals, there's a risk that
        // the client code interrupts into the loop below.
        qWarning() << "do not add labels while adding the other labels";
        return;
    }
    auto offset = m_labels.size();
    m_labels.append(labels);
    for (int i = 0; i < m_mirrorPages; ++i) {
        auto start = m_labels.size() * i + offset;
        beginInsertRows(QModelIndex(), start, start + labels.size() - 1);
        m_rowCount += labels.size();
        endInsertRows();
    }
    Q_ASSERT(m_labels.size() * m_mirrorPages == m_rowCount);
    emit labelCountChanged(m_labels.size());
}

/// Updates the values by the given bit-array.
///
/// For each byte, 8 bits will be read from LSB to MSB.
void BitArrayListModel::setValues(const QByteArray &values)
{
    if (m_values == values)
        return;
    m_values = values;

    if (m_labels.empty())
        return;
    // simply emit dataChanged for all values, which could be narrowed
    // if a few bits change frequently.
    emit dataChanged(index(0), index(m_rowCount - 1), { ValueRole });
}

/// Specifies the memory layout of the values.
///
/// This can be used to insert a certain number of padding bits per block.
///  For example, `bitsPerBlock = 30, blockBytes = 4` means there are 2-bit
/// padding for each 4-byte block.
void BitArrayListModel::setValueBlockLayout(int bitsPerBlock, int blockBytes)
{
    if (m_bitsPerBlock == bitsPerBlock && m_blockBytes == blockBytes)
        return;
    if (bitsPerBlock <= 0 || blockBytes <= 0 || bitsPerBlock > 8 * blockBytes) {
        qWarning() << "invalid layout: bitsPerblock =" << m_bitsPerBlock
                   << ", blockBytes =" << blockBytes;
        return;
    }

    m_bitsPerBlock = bitsPerBlock;
    m_blockBytes = blockBytes;

    if (m_labels.empty())
        return;
    emit dataChanged(index(0), index(m_rowCount - 1), { ValueRole });
}

/// Returns the number of the mirrored indices.
///
/// 1 means nothing mirrored.
int BitArrayListModel::mirrorPages() const
{
    return m_mirrorPages;
}

/// Changes the number of the mirrored indices.
///
/// Setting this to 2 or 3 allows us to fake ListView to behave as if it
/// were a "ring" view. For example, suppose there are 10 items and ListView
/// can show 3 items per page, the last page will display the 9th item plus
/// 10th and 11th (mod 10) which are mapped to 0th and 1st items respectively.
void BitArrayListModel::setMirrorPages(int pages)
{
    if (pages < 1) {
        qDebug() << "invalid mirror page count:" << pages;
        return;
    }

    if (m_labels.empty()) {
        m_mirrorPages = pages;
    } else if (m_mirrorPages > pages) {
        auto newRowCount = m_labels.size() * pages;
        beginRemoveRows(QModelIndex(), newRowCount, m_rowCount - 1);
        m_mirrorPages = pages;
        m_rowCount = newRowCount;
        endRemoveRows();
    } else if (m_mirrorPages < pages) {
        auto newRowCount = m_labels.size() * pages;
        beginInsertRows(QModelIndex(), m_rowCount, newRowCount - 1);
        m_mirrorPages = pages;
        m_rowCount = newRowCount;
        endInsertRows();
    }
}

QHash<int, QByteArray> BitArrayListModel::roleNames() const
{
    static const QHash<int, QByteArray> roles = {
        { LabelRole, QByteArrayLiteral("label") },
        { ValueRole, QByteArrayLiteral("value") },
    };
    return roles;
}

int BitArrayListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_rowCount;
}

QVariant BitArrayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_rowCount)
        return {};

    int row = index.row() % m_labels.size();
    switch (role) {
    case Qt::DisplayRole:
    case LabelRole:
        if (row >= m_labels.size())
            return {};
        return m_labels.at(row);
    case ValueRole: {
        auto block = row / m_bitsPerBlock;
        auto bit = row % m_bitsPerBlock;
        auto byte = block * m_blockBytes + bit / 8;
        auto mask = 1 << (bit % 8);
        if (byte >= m_values.size())
            return {};
        return static_cast<bool>(m_values.at(byte) & mask);
    }
    }

    return {};
}
