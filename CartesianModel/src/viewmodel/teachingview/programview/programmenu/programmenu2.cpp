#include "programmenu2.h"
#include "programmenu2_1.h"
#include "programmenu2_2.h"
#include "programmenu2_3.h"
#include "programmenu2_4.h"
#include "axispointinfo.h"
#include "../command/ncpcommandmove.h"
#include "teachingview/teachingprogrampagemodel.h"

ProgramMenu2::ProgramMenu2(ResourceManager *resource, CommunicationThread * comm, NCPFile *ncpFile, AxisInfo *axisInfo)
    : ProgramMenu(resource,comm),m_axisInfo(axisInfo)
{
    m_axisPointInfo = AxisPointInfo::get_instance();

    m_subMenuList.append(new ProgramMenu2_1(resource, comm, ncpFile));
    m_subMenuList.append(new ProgramMenu2_2(resource, comm));
    m_subMenuList.append(new ProgramMenu2_3(resource, comm));
    m_subMenuList.append(new ProgramMenu2_4(resource, comm, ncpFile));
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        connect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged, Qt::QueuedConnection);
    }
}

ProgramMenu2::~ProgramMenu2()
{
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged);
    }
}

void ProgramMenu2::setNcpFile(NCPFile *ncpFile)
{
    dynamic_cast<ProgramMenu2_1 *>(m_subMenuList.at(0))->setNcpFile(ncpFile);
    dynamic_cast<ProgramMenu2_4 *>(m_subMenuList.at(3))->setNcpFile(ncpFile);
}

void ProgramMenu2::show(NCPCommand *command)
{
    int menuIndex = getMenuIndex(command);
    if ((menuIndex >= 0) && (menuIndex < m_subMenuList.length())) {
        static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex))->show(command);
        setMenuIndex(menuIndex);
    }
}

ProgramMenu::CreateMethod ProgramMenu2::commandCreateMethod()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->commandCreateMethod();
}

NCPCommand *ProgramMenu2::createNcpCommand()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->createNcpCommand();
}

int ProgramMenu2::insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->insertCustomNcpCommand(file, program, lineIndex);
}

int ProgramMenu2::replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->replaceCustomNcpCommand(file, program, lineIndex);
}

int ProgramMenu2::getMenuIndex(NCPCommand *command)
{
    int ret = -1;
    switch (command->type()) {
    case NCPCommand::Type::Move:
    case NCPCommand::Type::Movep:
        ret = getMoveMenuIndex(command);
        break;
    case NCPCommand::Type::GoSub:
        ret = 2;
        break;
    case NCPCommand::Type::Line:
    case NCPCommand::Type::Linec:
    case NCPCommand::Type::Arc:
    case NCPCommand::Type::Arcc:
    case NCPCommand::Type::Path:
    case NCPCommand::Type::Home:
    case NCPCommand::Type::Homep:
    case NCPCommand::Type::Break:
    case NCPCommand::Type::Acc:
    case NCPCommand::Type::WaitPos:
        ret = 3;
        break;
    }
    return ret;
}

int ProgramMenu2::getMoveMenuIndex(NCPCommand *command)
{
    int ret = 0;
    int pointNo = dynamic_cast<NCPCommandMove *>(command)->pointNo();
    switch (m_axisPointInfo->getPointType(pointNo)) {
    case AxisPointInfo::PointType::Arbitrary:
        ret = 0;
        break;
    case AxisPointInfo::PointType::Fix:
    case AxisPointInfo::PointType::FreePalletize:
    case AxisPointInfo::PointType::StandardPalletize:
        ret = 1;
        break;
    default:
        ret = -1;
    }
    return ret;
}

void ProgramMenu2::onNcpCommandDataChanged()
{
    emit ncpCommandDataChanged();
}
