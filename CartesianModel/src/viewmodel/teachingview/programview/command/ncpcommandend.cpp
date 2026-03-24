#include "ncpcommandend.h"

//------------------------------------------------------
// NCPCommandEnd.
//------------------------------------------------------
NCPCommandEnd::NCPCommandEnd(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandEnd::NCPCommandEnd(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandEnd::parse(const QString &/*commandLine*/)
{
    return true;
}
//------------------------------------------------------
QString NCPCommandEnd::nameString() const
{
    return m_resource->label(41, PEN_MSG_PROGRAM_WIZARD_PROG_END, m_resource->language());
}
//------------------------------------------------------
QString NCPCommandEnd::toString() const
{
    return "";
}
//------------------------------------------------------
QString NCPCommandEnd::toCode() const
{
    return QString("END\t");
}
//------------------------------------------------------
NCPCommand* NCPCommandEnd::clone() const
{
    //qDebug() << "NCPCommandEnd::clone:";
    return new NCPCommandEnd(m_resource);
}
