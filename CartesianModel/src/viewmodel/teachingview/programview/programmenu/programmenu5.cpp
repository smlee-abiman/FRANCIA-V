#include "programmenu5.h"
#include "programmenu5_1.h"
#include "programmenu5_2.h"

ProgramMenu5::ProgramMenu5(ResourceManager *resource, CommunicationThread * comm)
    : ProgramMenu(resource,comm)
{
    m_subMenuList.append(new ProgramMenu5_1(resource, comm));
    m_subMenuList.append(new ProgramMenu5_2(resource, comm));
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        connect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged, Qt::QueuedConnection);
    }
}

ProgramMenu5::~ProgramMenu5()
{
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged);
    }
}

NCPCommand *ProgramMenu5::createNcpCommand()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->createNcpCommand();
}

void ProgramMenu5::show(NCPCommand *command)
{
    //qDebug() << "ProgramMenu5::show command type:" << command->type() << "menuIndex:" << Menu5_Index();
    auto subMenuIndex = menuIndex();
    switch (command->type()) {
    case NCPCommand::Type::Pause:
        subMenuIndex = 0;
        break;
    case NCPCommand::Type::End:
        subMenuIndex = 0;
        break;
    case NCPCommand::Type::Alm:
        subMenuIndex = 0;
        break;
    case NCPCommand::Type::Out:
        subMenuIndex = 1;
        break;
    default:
        return;
    }
    static_cast<ProgramMenu *>(m_subMenuList.at(subMenuIndex))->show(command);
    setMenuIndex(subMenuIndex);
}

void ProgramMenu5::onNcpCommandDataChanged()
{
    emit ncpCommandDataChanged();
}
