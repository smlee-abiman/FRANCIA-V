#include "programmenu2_4.h"
#include "programmenu2_4_1.h"
#include "programmenu2_4_2.h"
#include "programmenu2_4_3.h"
#include "programmenu2_4_4.h"
#include "programmenu2_4_5.h"
#include "programmenu2_4_6.h"
#include "programmenu2_4_7.h"

ProgramMenu2_4::ProgramMenu2_4(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile)
    : ProgramMenu(resource,comm)
{
    m_subMenuList.append(new ProgramMenu2_4_1(resource, comm, ncpFile));
    m_subMenuList.append(new ProgramMenu2_4_2(resource, comm, ncpFile));
    m_subMenuList.append(new ProgramMenu2_4_3(resource, comm));
    m_subMenuList.append(new ProgramMenu2_4_4(resource, comm));
    m_subMenuList.append(new ProgramMenu2_4_5(resource, comm));
    m_subMenuList.append(new ProgramMenu2_4_6(resource, comm));
    m_subMenuList.append(new ProgramMenu2_4_7(resource, comm));
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        connect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged, Qt::QueuedConnection);
    }
}

ProgramMenu2_4::~ProgramMenu2_4()
{
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged);
    }
}

void ProgramMenu2_4::setNcpFile(NCPFile *ncpFile)
{
    dynamic_cast<ProgramMenu2_4_1 *>(m_subMenuList.at(0))->setNcpFile(ncpFile);
    dynamic_cast<ProgramMenu2_4_2 *>(m_subMenuList.at(1))->setNcpFile(ncpFile);
}

void ProgramMenu2_4::show(NCPCommand *command)
{
    int menuIndex = getMenuIndex(command);
    if ((menuIndex >= 0) && (menuIndex < m_subMenuList.length())) {
        static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex))->show(command);
        setMenuIndex(menuIndex);
    }
}

NCPCommand *ProgramMenu2_4::createNcpCommand()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->createNcpCommand();
}

int ProgramMenu2_4::getMenuIndex(NCPCommand *command)
{
    int ret = -1;
    switch (command->type()) {
    case NCPCommand::Type::Line:
    case NCPCommand::Type::Linec:
        ret = 0;
        break;
    case NCPCommand::Type::Arc:
    case NCPCommand::Type::Arcc:
        ret = 1;
        break;
    case NCPCommand::Type::Path:
        ret = 2;
        break;
    case NCPCommand::Type::Home:
    case NCPCommand::Type::Homep:
        ret = 3;
        break;
    case NCPCommand::Type::Break:
        ret = 4;
        break;
    case NCPCommand::Type::Acc:
        ret = 5;
        break;
    case NCPCommand::Type::WaitPos:
        ret = 6;
        break;
    }
    return ret;
}

void ProgramMenu2_4::onNcpCommandDataChanged()
{
    emit ncpCommandDataChanged();
}
