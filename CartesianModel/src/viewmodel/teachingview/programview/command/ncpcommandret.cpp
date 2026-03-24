#include "ncpcommandret.h"

//------------------------------------------------------
// NCPCommandRet.
//------------------------------------------------------
NCPCommandRet::NCPCommandRet(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandRet::NCPCommandRet(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
NCPCommandRet::~NCPCommandRet()
{
}
//------------------------------------------------------
QString NCPCommandRet::nameString() const
{
    return m_resource->label(41, 539, m_resource->language());
}
//------------------------------------------------------
QString NCPCommandRet::toString() const
{
    return "";
}
//------------------------------------------------------
QString NCPCommandRet::toCode() const
{
    return QString("RET\t");
}
//------------------------------------------------------
NCPCommand* NCPCommandRet::clone() const
{
    return new NCPCommandRet(m_resource);
}
