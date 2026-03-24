#include "ncpcommandoff.h"

//------------------------------------------------------
// NCPCommandOff.
//------------------------------------------------------
NCPCommandOff::NCPCommandOff(QObject *parent)
    : NCPCommandOn(parent)
{
}
//------------------------------------------------------
NCPCommandOff::NCPCommandOff(ResourceManager *resource, QObject *parent)
    : NCPCommandOn(resource, parent)
{
}
//------------------------------------------------------
NCPCommandOff::NCPCommandOff(ResourceManager *resource, int programNo, QObject *parent)
    : NCPCommandOn(resource, programNo, parent)
{
}
//------------------------------------------------------
QString NCPCommandOff::nameString() const
{
    return m_resource->label(41, 624);
}
//------------------------------------------------------
QString NCPCommandOff::toString() const
{
    auto string = QString("");
    auto programNoStr = QString("#%1").arg(programNo(), 3, 10, QChar('0'));
    string.append(programNoStr);
    if (1 <= programNo() && programNo() <= 255) {
        auto programName = QString("(%1)").arg(m_resource->label(41, 37 + (programNo() - 1)));
        string.append(programName);
    }
    return string;
}
//------------------------------------------------------
QString NCPCommandOff::toCode() const
{
    return QString("OFF\t#%1").arg(programNo(), 3, 10, QChar('0'));
}
//------------------------------------------------------
NCPCommand *NCPCommandOff::clone() const
{
    return new NCPCommandOff(m_resource, programNo());
}
