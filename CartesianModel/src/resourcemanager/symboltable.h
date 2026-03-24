/*
 * This class is something like IndexSet class.
 * Each of value has a range and caller specify index. This class method understands which vlaue are
 * the owner of specified index. This is used for symbol_msg.csv however, this functionality is more
 * generic.
 */

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <QHash>
#include <QString>
#include <QVector>

class SymbolTable
{
public:
    SymbolTable();

    void addRange(int start, int count, const QString &symbol);
    void finalizeTable();

    QString at(int i) const;
    int indexAt(const QString &symbol);
    int size() const;

private:
    struct RangeEntry
    {
        int start;
        int count;
        QString symbol;
    };

    QVector<RangeEntry> m_ranges; // sorted entries of count > 1
    QHash<int, QString> m_uniqs; // map of entries of count == 1
    QHash<QString, int> m_symbolUniqs; // map of entries of count == 1
    int m_size;
};

#endif // SYMBOLTABLE_H
