#include "programmenu.h"
#include "../ncpfile.h"

ProgramMenu::ProgramMenu(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource,comm)
{

}

void ProgramMenu::initData()
{

}

void ProgramMenu::show(NCPCommand *command)
{
    Q_UNUSED(command)
}

int ProgramMenu::updateModbusData()
{
    return 0;
}

NCPCommand *ProgramMenu::createNcpCommand()
{
    return nullptr;
}

int ProgramMenu::insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    Q_UNUSED(file)
    Q_UNUSED(program)
    Q_UNUSED(lineIndex)
    return 0;
}

int ProgramMenu::replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    Q_UNUSED(file)
    Q_UNUSED(program)
    Q_UNUSED(lineIndex)
    return 0;
}

void ProgramMenu::setMenuIndex(int index)
{
    if (m_menuIndex != index) {
        m_menuIndex = index;
        emit menuIndexChanged();
    }
}

ProgramMenu::CreateMethod ProgramMenu::commandCreateMethod()
{
    return CreateMethod::SimpleNcpCommand;
}

void ProgramMenu::onNcpCommandDataChanged()
{
}

