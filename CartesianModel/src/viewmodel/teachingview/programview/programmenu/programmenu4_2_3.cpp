#include "programmenu4_2_3.h"
#include "../command/ncpcommandon.h"
#include "../command/ncpcommandoff.h"

//------------------------------------------------------
// ProgramMenu4_2_3.
//------------------------------------------------------
ProgramMenu4_2_3::ProgramMenu4_2_3(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
    , mProgramNoList({0,0})
    , mSelectedIndex(-1)
    , mOpenProgramSelectionDialog(false)
{
}
//------------------------------------------------------
ProgramMenu4_2_3::~ProgramMenu4_2_3()
{
}
//------------------------------------------------------
void ProgramMenu4_2_3::show(NCPCommand *command)
{
    auto programNo = -1;
    auto selectedIndex = -1;
    // select menu index by command.
    switch (command->type()) {
    case NCPCommand::Type::On: {
            auto onCommand = dynamic_cast<NCPCommandOn*>(command);
            programNo = onCommand->programNo();
            selectedIndex = 0;
        }
        break;
     case NCPCommand::Type::Off: {
            auto offCommand = dynamic_cast<NCPCommandOff*>(command);
            programNo = offCommand->programNo();
            selectedIndex = 1;
        }
        break;
    }
    setProgramNo(selectedIndex, programNo);
    setSelectedIndex(selectedIndex);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_2_3::createNcpCommand()
{
    return (mSelectedIndex == 0) ? createNcpOnCommand()  :
           (mSelectedIndex == 1) ? createNcpOffCommand() : nullptr;
}
//------------------------------------------------------
void ProgramMenu4_2_3::onActivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_2_3::onDeactivate()
{
    // nop.
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_2_3::createNcpOnCommand()
{
    auto programNo = mProgramNoList[0];
    if (programNo <= 0) { return nullptr; }
    auto onCommand = new NCPCommandOn(m_resource, programNo);
    return onCommand;
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_2_3::createNcpOffCommand()
{
    auto programNo = mProgramNoList[1];
    if (programNo <= 0) { return nullptr; }
    auto offCommand = new NCPCommandOff(m_resource, programNo);
    return offCommand;
}
