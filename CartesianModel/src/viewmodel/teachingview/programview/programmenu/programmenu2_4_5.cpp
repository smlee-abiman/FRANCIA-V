#include "programmenu2_4_5.h"
#include "../command/ncpcommandbreak.h"

ProgramMenu2_4_5::ProgramMenu2_4_5(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    for (int i = 0; i < Enums::AxisNum; i++)
        m_selectList.append(false);
}

void ProgramMenu2_4_5::onDeactivate()
{
    clearSelect();
}

void ProgramMenu2_4_5::show(NCPCommand *command)
{
    if (command->type() != NCPCommand::Type::Break)
        return;
    clearSelect();
    auto breakCommand = static_cast<NCPCommandBreak *>(command);
    auto axisList = breakCommand->axisList();
    for (int axis : axisList) {
        if ((axis >= 0) && (axis < m_selectList.length()))
            m_selectList.replace(axis, true);
    }
    emit selectListChanged();
}


NCPCommand *ProgramMenu2_4_5::createNcpCommand()
{
    auto command = new NCPCommandBreak(m_resource);
    if (command != nullptr) {
        bool invalid = true;
        const int numAxis = m_selectList.length();
        for (int i = 0; i < numAxis; i++) {
            if (m_selectList.at(i)) {
                command->appendAxis(i);
                invalid = false;
            }
        }
        if (invalid) {
            delete command;
            command = nullptr;
        }
    }
    return command;
}

void ProgramMenu2_4_5::clearSelect()
{
    const int numAxis = m_selectList.length();
    for (int i = 0; i < numAxis; i++)
        m_selectList.replace(i, false);
    emit selectListChanged();
}
