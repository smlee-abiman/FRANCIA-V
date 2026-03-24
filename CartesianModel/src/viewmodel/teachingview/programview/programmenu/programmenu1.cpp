#include "programmenu1.h"
#include "programmenu1_1.h"
#include "programmenu1_2.h"
#include "programmenu1_3.h"
#include "programmenu1_4.h"
#include "../command/ncpcommandgosub.h"
#include "../command/ncpcommandon.h"

/*********************main***********************/
ProgramMenu1::ProgramMenu1(ResourceManager *resource, CommunicationThread * comm)
    : ProgramMenu(resource,comm)
{
    m_subMenuList.append(new ProgramMenu1_1(resource, comm));
    m_subMenuList.append(new ProgramMenu1_2(resource, comm));
    m_subMenuList.append(new ProgramMenu1_3(resource, comm));
    m_subMenuList.append(new ProgramMenu1_4(resource, comm));
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        connect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged, Qt::QueuedConnection);
    }
}

ProgramMenu1::~ProgramMenu1()
{
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged);
    }
}

void ProgramMenu1::initData()
{
    static_cast<ProgramMenu *>(m_subMenuList.at(2))->initData();
}

NCPCommand *ProgramMenu1::createNcpCommand()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->createNcpCommand();
}

ProgramMenu::CreateMethod ProgramMenu1::commandCreateMethod()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->commandCreateMethod();
}

int ProgramMenu1::insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->insertCustomNcpCommand(file, program, lineIndex);
}

int ProgramMenu1::replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->replaceCustomNcpCommand(file, program, lineIndex);
}

void ProgramMenu1::onNcpCommandDataChanged()
{
    emit ncpCommandDataChanged();
}

void ProgramMenu1::show(NCPCommand *command)
{
    int menuIndex = getMenuIndex(command);
    if ((menuIndex >= 0) && (menuIndex < m_subMenuList.length())) {
        static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex))->show(command);
        setMenuIndex(menuIndex);
    }
}

int ProgramMenu1::getIndex(int programNo)
{
    int ret = -1;
    switch (programNo) {
    case 102:
    case 103:
    case 109:
    case 148:
        ret = 0;
        break;
    case 121:
    case 122:
    case 123:
    case 124:
    case 147:
    case 149:
    case 150:
    case 151:
    case 152:
    case 153:
        ret = 1;
        break;
    default:
        break;
    }
    return ret;
}

int ProgramMenu1::getMenuIndex(NCPCommand *command)
{
    int ret = -1;
    switch (command->type()) {
    case NCPCommand::Type::Outp:
        ret = 3;
        break;
    case NCPCommand::Type::On:
    {
        auto onCommand = dynamic_cast<NCPCommandOn *>(command);
        ret = getIndex(onCommand->programNo());
    }
        break;
    case NCPCommand::Type::GoSub:
    {
        auto gosubCommand = dynamic_cast<NCPCommandGoSub *>(command);
        if (gosubCommand->isJigProgram()) {
            ret = 2;
        } else {
            ret = getIndex(gosubCommand->programNo());
        }
    }
        break;
    }
    return ret;
}
