#include "programmenu4_2_4.h"

#include "../command/ncpcommandret.h"

//------------------------------------------------------
// ProgramMenu4_2_4.
//------------------------------------------------------
ProgramMenu4_2_4::ProgramMenu4_2_4(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
    , mUse(false)
{
}
//------------------------------------------------------
ProgramMenu4_2_4::~ProgramMenu4_2_4()
{
}
//------------------------------------------------------
void ProgramMenu4_2_4::show(NCPCommand */*command*/)
{
    // memo: 
    // No confirmation of the input command is performed.
    // Because it is dedicated to the RET command.
    setUse(true);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_2_4::createNcpCommand()
{
    if (!mUse) { return nullptr; }
    return new NCPCommandRet(m_resource);
}
//------------------------------------------------------
void ProgramMenu4_2_4::onActivate()
{
}
//------------------------------------------------------
void ProgramMenu4_2_4::onDeactivate()
{
}
