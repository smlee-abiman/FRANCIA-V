#include "ncpcommandsrv.h"

//------------------------------------------------------
// NCPCommandSrv.
//------------------------------------------------------
NCPCommandSrv::NCPCommandSrv(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandSrv::NCPCommandSrv(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandSrv::parse(const QString &/*commandLine*/)
{
    return true;
}
//------------------------------------------------------
QString NCPCommandSrv::nameString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandSrv::toString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandSrv::toCode() const
{
    return QString("SRV\t");    // todo:
}
