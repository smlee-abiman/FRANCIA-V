#include "programmenu4.h"
#include "programmenu4_1.h"
#include "programmenu4_2.h"
#include "programmenu4_3.h"

ProgramMenu4::ProgramMenu4(ResourceManager *resource, CommunicationThread * comm)
    : ProgramMenu(resource,comm)
{
    m_subMenuList.append(new ProgramMenu4_1(resource, comm));
    m_subMenuList.append(new ProgramMenu4_2(resource, comm));
    m_subMenuList.append(new ProgramMenu4_3(resource, comm));
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        connect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged, Qt::QueuedConnection);
    }
}
//------------------------------------------------------
ProgramMenu4::~ProgramMenu4()
{
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged);
    }
    qDeleteAll(m_subMenuList);
    m_subMenuList.clear();
}
//------------------------------------------------------
void ProgramMenu4::show(NCPCommand *command)
{
    auto subMenuIndex = menuIndex();
    // select menu index by command.
    switch (command->type()) {
    case NCPCommand::Type::In:
        subMenuIndex = 0;
        break;
    case NCPCommand::Type::GoSub:
    case NCPCommand::Type::IfGoSub:
    case NCPCommand::Type::GoTo:
    case NCPCommand::Type::IfGoTo:
    case NCPCommand::Type::On:
    case NCPCommand::Type::Off:
    case NCPCommand::Type::Ret:
        subMenuIndex = 1;
        break;
    case NCPCommand::Type::CInc:
    case NCPCommand::Type::CClr:
    case NCPCommand::Type::Inc:
    case NCPCommand::Type::Dec:
        subMenuIndex = 2;
        break;
    }
    static_cast<ProgramMenu *>(m_subMenuList.at(subMenuIndex))->show(command);
    setMenuIndex(subMenuIndex);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4::createNcpCommand()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->createNcpCommand();
}
//------------------------------------------------------
ProgramMenu::CreateMethod ProgramMenu4::commandCreateMethod()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->commandCreateMethod();
}
//------------------------------------------------------
int ProgramMenu4::insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->insertCustomNcpCommand(file, program, lineIndex);
}
//------------------------------------------------------
int ProgramMenu4::replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->replaceCustomNcpCommand(file, program, lineIndex);
}

void ProgramMenu4::onNcpCommandDataChanged()
{
    emit ncpCommandDataChanged();
}
