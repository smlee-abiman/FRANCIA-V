#include "programmenu4_2.h"
#include "programmenu4_2_1.h"
#include "programmenu4_2_2.h"
#include "programmenu4_2_3.h"
#include "programmenu4_2_4.h"

//------------------------------------------------------
// ProgramMenu4_2.
//------------------------------------------------------
ProgramMenu4_2::ProgramMenu4_2(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    m_subMenuList.append(new ProgramMenu4_2_1(resource, comm));
    m_subMenuList.append(new ProgramMenu4_2_2(resource, comm));
    m_subMenuList.append(new ProgramMenu4_2_3(resource, comm));
    m_subMenuList.append(new ProgramMenu4_2_4(resource, comm));
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        connect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged, Qt::QueuedConnection);
    }
}
//------------------------------------------------------
ProgramMenu4_2::~ProgramMenu4_2()
{
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged);
    }
    qDeleteAll(m_subMenuList);
    m_subMenuList.clear();
}
//------------------------------------------------------
void ProgramMenu4_2::show(NCPCommand *command)
{
    auto subMenuIndex = menuIndex();
    // select menu index by command.
    switch (command->type()) {
    case NCPCommand::Type::GoSub:
    case NCPCommand::Type::IfGoSub:
        subMenuIndex = 0;
        break;
    case NCPCommand::Type::GoTo:
    case NCPCommand::Type::IfGoTo:
        subMenuIndex = 1;
        break;
    case NCPCommand::Type::On:
    case NCPCommand::Type::Off:
        subMenuIndex = 2;
        break;
    case NCPCommand::Type::Ret:
        subMenuIndex = 3;
        break;
    }
    static_cast<ProgramMenu *>(m_subMenuList.at(subMenuIndex))->show(command);
    setMenuIndex(subMenuIndex);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_2::createNcpCommand()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->createNcpCommand();
}

void ProgramMenu4_2::onNcpCommandDataChanged()
{
    emit ncpCommandDataChanged();
}
