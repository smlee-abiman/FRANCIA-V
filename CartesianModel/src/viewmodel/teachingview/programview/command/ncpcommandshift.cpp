#include "ncpcommandshift.h"

//------------------------------------------------------
// NCPCommandShift.
//------------------------------------------------------
NCPCommandShift::NCPCommandShift(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandShift::NCPCommandShift(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandShift::parse(const QString &/*commandLine*/)
{
    return true;
}
//------------------------------------------------------
QString NCPCommandShift::nameString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandShift::toString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandShift::toCode() const
{
    return QString("SHIFT\t");    // todo:
}
