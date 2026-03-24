#include "ncpcommandaspd.h"

//------------------------------------------------------
// NCPCommandAspd.
//------------------------------------------------------
NCPCommandAspd::NCPCommandAspd(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandAspd::NCPCommandAspd(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandAspd::parse(const QString &/*commandLine*/)
{
    return true;    // todo:
}
//------------------------------------------------------
QString NCPCommandAspd::nameString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandAspd::toString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandAspd::toCode() const
{
    return QString("ASPD\t");    // todo:
}
