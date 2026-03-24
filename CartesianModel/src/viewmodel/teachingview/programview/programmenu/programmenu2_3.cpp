#include "programmenu2_3.h"
#include "../command/ncpcommandgosub.h"

const int PROGRAM_MENU2_3_FREE_STACK_INDEX_OFFSET = 8;
const QList<int> ProgramMenu2_3::m_programNoList = {
    161, 162, 163, 164, 165, 166, 167, 168,
    169, 170, 171, 172, 173, 174, 175, 176
};

ProgramMenu2_3::ProgramMenu2_3(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
}

void ProgramMenu2_3::onDeactivate()
{
}

NCPCommand *ProgramMenu2_3::createNcpCommand()
{
    auto programNo = m_programNoList.at(menuIndex());
    return new NCPCommandGoSub(m_resource, programNo);
}

void ProgramMenu2_3::show(NCPCommand *command)
{
    int stackIndex = 0;
    auto goSubCommand = dynamic_cast<NCPCommandGoSub *>(command);
    if (goSubCommand->isStandardPalletizeProgram()) {
        stackIndex = goSubCommand->programNo() - goSubCommand->startStandardPalletizeProgramNo();
    } else if (goSubCommand->isFreePalletizeProgram()) {
        stackIndex = PROGRAM_MENU2_3_FREE_STACK_INDEX_OFFSET + goSubCommand->programNo() - goSubCommand->startFreePalletizeProgramNo();
    }
    setMenuIndex(stackIndex);
}
