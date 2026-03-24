#include "ncpvariable.h"

//------------------------------------------------------
// NCPVariable.
//------------------------------------------------------
NCPVariable::NCPVariable(const QString &relation, const QString &symbol, const QString& symbol1, const QString &opeStr, int value, int ioAddress)
    : m_relation(relation)
    , m_symbol(symbol)
    , m_symbol1(symbol1)
    , m_opeStr(opeStr)
    , m_value(value)
    , m_ioAddress(ioAddress)
{
}
