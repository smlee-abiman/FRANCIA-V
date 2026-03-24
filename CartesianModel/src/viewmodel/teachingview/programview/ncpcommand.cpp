#include "ncpcommand.h"

//------------------------------------------------------
// NCPCommand.
//------------------------------------------------------
NCPCommand::NCPCommand(ResourceManager *resource, QObject *parent)
    : QObject(parent),
      m_resource(resource)
{
}
//------------------------------------------------------
NCPCommand::NCPCommand(QObject *parent)
    : QObject(parent)
{
}
//------------------------------------------------------
NCPCommand::~NCPCommand()
{
}
