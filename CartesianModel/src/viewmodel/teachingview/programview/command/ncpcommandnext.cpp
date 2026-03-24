#include "ncpcommandnext.h"

//------------------------------------------------------
// NCPCommandNext.
//------------------------------------------------------
NCPCommandNext::NCPCommandNext(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandNext::NCPCommandNext(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandNext::parse(const QString &/*commandLine*/)
{
    return true;
}
//------------------------------------------------------
QString NCPCommandNext::nameString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandNext::toString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandNext::toCode() const
{
    return QString("NEXT\t");    // todo:
}
