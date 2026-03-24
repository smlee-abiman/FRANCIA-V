#include "programmenu4_3.h"
#include "programmenu4_3_1.h"
#include "programmenu4_3_2.h"
#include "programmenu4_3_3.h"
#include "programmenu4_3_4.h"

//------------------------------------------------------
// ProgramMenu4_2.
//------------------------------------------------------
ProgramMenu4_3::ProgramMenu4_3(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    m_subMenuList.append(new ProgramMenu4_3_1(resource, comm));
    m_subMenuList.append(new ProgramMenu4_3_2(resource, comm));
    m_subMenuList.append(new ProgramMenu4_3_3(resource, comm));
    m_subMenuList.append(new ProgramMenu4_3_4(resource, comm));
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        connect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged, Qt::QueuedConnection);
    }
}
//------------------------------------------------------
ProgramMenu4_3::~ProgramMenu4_3()
{
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged);
    }
    qDeleteAll(m_subMenuList);
    m_subMenuList.clear();
}
//------------------------------------------------------
void ProgramMenu4_3::show(NCPCommand *command)
{
    auto subMenuIndex = menuIndex();
    // select menu index by command.
    switch (command->type()) {
    case NCPCommand::Type::CInc:
        subMenuIndex = 0;
        break;
    case NCPCommand::Type::CClr:
        subMenuIndex = 1;
        break;
    case NCPCommand::Type::Inc:
        subMenuIndex = 2;
        break;
    case NCPCommand::Type::Dec:
        subMenuIndex = 3;
        break;
    }
    static_cast<ProgramMenu *>(m_subMenuList.at(subMenuIndex))->show(command);
    setMenuIndex(subMenuIndex);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_3::createNcpCommand()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->createNcpCommand();
}

void ProgramMenu4_3::onNcpCommandDataChanged()
{
    emit ncpCommandDataChanged();
}
