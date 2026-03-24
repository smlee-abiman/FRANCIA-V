#include "programmenu2_4_7.h"
#include "../command/ncpcommandwaitpos.h"

ProgramMenu2_4_7::ProgramMenu2_4_7(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{

}

void ProgramMenu2_4_7::onDeactivate()
{
    m_checked = false;
    emit checkedChanged();
}

void ProgramMenu2_4_7::show(NCPCommand *command)
{
    Q_UNUSED(command)
}

NCPCommand *ProgramMenu2_4_7::createNcpCommand()
{
    NCPCommand *command = nullptr;
    if (m_checked) {
        command = new NCPCommandWaitPOS(m_resource);
    }
    return command;
}
