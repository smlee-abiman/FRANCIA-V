#ifndef NCPVARIABLE_H
#define NCPVARIABLE_H

#include <QObject>

//------------------------------------------------------
// NCPVariable.
//------------------------------------------------------
class NCPVariable {
public:
    explicit NCPVariable(const QString& relation, const QString& symbol, const QString& symbol1, const QString& opeStr, int value, int ioAddress);
    QString relation() const { return m_relation; }
    QString symbol() const { return m_symbol; }
    QString symbol1() const { return m_symbol1; }
    QString opeStr() const { return m_opeStr; }
    int value() const { return m_value; }
    int ioAddress() const { return m_ioAddress; }
private:
    QString     m_relation;
    QString     m_symbol;
    QString     m_symbol1;
    QString     m_opeStr;
    int         m_value;
    int         m_ioAddress;
};

#endif // NCPVARIABLE_H
