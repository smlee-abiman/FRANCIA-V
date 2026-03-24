#include "ncpcommandset.h"

//------------------------------------------------------
// NCPCommandSet.
//------------------------------------------------------
NCPCommandSet::NCPCommandSet(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandSet::NCPCommandSet(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandSet::parse(const QString &/*commandLine*/)
{
    return true;
}
//------------------------------------------------------
QString NCPCommandSet::nameString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandSet::toString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandSet::toCode() const
{
    return QString("SET\t");    // todo:
}
