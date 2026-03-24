#include "ncpcommandwaitpos.h"

//------------------------------------------------------
// NCPCommandWaitPOS.
//------------------------------------------------------
NCPCommandWaitPOS::NCPCommandWaitPOS(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandWaitPOS::NCPCommandWaitPOS(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandWaitPOS::parse(const QString &/*commandLine*/)
{
    return true;
}
//------------------------------------------------------
QString NCPCommandWaitPOS::nameString() const
{
    return m_resource->label(41, 520);
}
//------------------------------------------------------
QString NCPCommandWaitPOS::toString() const
{
    return "";
}
//------------------------------------------------------
QString NCPCommandWaitPOS::toCode() const
{
    return QString("WAIT\tPOS");
}

NCPCommand* NCPCommandWaitPOS::clone() const
{
    return new NCPCommandWaitPOS(m_resource);
}
