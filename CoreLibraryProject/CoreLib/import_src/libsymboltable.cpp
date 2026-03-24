#include <QtDebug>
#include <algorithm>
#include "libsymboltable.h"

LibSymbolTable::LibSymbolTable() : m_ranges(), m_uniqs(), m_size(0) {}

/// Adds the specified symbol at (start, start + count) range.
///
/// finalizeTable() must be called once you've registered all symbols.
/// Otherwise, lookup would fail.
void LibSymbolTable::addRange(int start, int count, const QString &symbol)
{
    if (count <= 0) {
        qDebug() << "bad symbol count for" << symbol << ":" << count;
        return;
    }
    if (count == 1) {
        m_uniqs[start] = symbol;
    } else {
        m_ranges.push_back({ start, count, symbol });
    }
    m_size += count;
}

/// Prepares the symbol table for lookup.
void LibSymbolTable::finalizeTable()
{
    std::sort(m_ranges.begin(), m_ranges.end(),
              [](const auto &a, const auto &b) { return a.start < b.start; });
}

QString LibSymbolTable::at(int index) const
{
    // try O(1) lookup for symbols of count == 1
    auto q = m_uniqs.find(index);
    if (q != m_uniqs.end()) {
        return *q;
    }

    RangeEntry query = { index, 1, {} };
    auto p = std::upper_bound(m_ranges.cbegin(), m_ranges.cend(), query,
                              [](auto &a, auto &b) { return a.start < b.start; });
    if (p == m_ranges.cbegin()) {
        qDebug() << "no possible match for symbol at" << index;
        return {};
    }

    p = std::prev(p);
    if (p->start + p->count <= index) {
        qDebug() << "match out of range for symbol at" << index;
        return {};
    }

    // Replace ?
    QString num = QString::number(index - p->start + 1);
    if (p->symbol.startsWith('?')) {
        return num + p->symbol.mid(1, -1);
    } else if (p->symbol.endsWith('?')) {
        QString str = p->symbol;
        str.chop(1);
        return str + num;
    } else {
        QString str = p->symbol;
        str.replace('?', num, Qt::CaseInsensitive);
        return str;
    }
}

int LibSymbolTable::size() const
{
    return m_size;
}
