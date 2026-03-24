#include "ncpcommandjog.h"

//------------------------------------------------------
// NCPCommandJog.
//------------------------------------------------------
NCPCommandJog::NCPCommandJog(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandJog::NCPCommandJog(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandJog::parse(const QString &/*commandLine*/)
{
    return true;
}
//------------------------------------------------------
QString NCPCommandJog::nameString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandJog::toString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandJog::toCode() const
{
    return QString("JOG\t");    // todo:
}
