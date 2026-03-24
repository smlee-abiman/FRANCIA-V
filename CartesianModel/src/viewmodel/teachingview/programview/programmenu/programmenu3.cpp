#include "programmenu3.h"
#include "programmenu3_1.h"
#include "programmenu3_2.h"
#include "../command/ncpcommandwait.h"

ProgramMenu3::ProgramMenu3(ResourceManager *resource, CommunicationThread * comm)
    : ProgramMenu(resource,comm)
{
    m_subMenuList.append(new ProgramMenu3_1(resource, comm));
    m_subMenuList.append(new ProgramMenu3_2(resource, comm));
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        connect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged, Qt::QueuedConnection);
    }
}

ProgramMenu3::~ProgramMenu3()
{
    for (QObject *obj : m_subMenuList) {
        auto menu = static_cast<ProgramMenu *>(obj);
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &ProgramMenu::onNcpCommandDataChanged);
    }
}

NCPCommand *ProgramMenu3::createNcpCommand()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->createNcpCommand();
}

void ProgramMenu3::show(NCPCommand *command)
{
    //qDebug() << "ProgramMenu3::show command type:" << command->type() << "menuIndex:" << menuIndex() << "wizardMenuIndex:" << command->wizardMenuIndex();
    switch (command->type()) {
    case NCPCommand::Type::Wait:
        int subMenuIndex = (int)(static_cast<NCPCommandWait *>(command)->getCmdCategory());
        //qDebug() << "subMenuIndex:" << subMenuIndex;
        static_cast<ProgramMenu *>(m_subMenuList.at(subMenuIndex))->show(command);
        setMenuIndex(subMenuIndex);
        break;
    }
}

int ProgramMenu3::updateModbusData()
{
    return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex()))->updateModbusData();
}

void ProgramMenu3::onNcpCommandDataChanged()
{
    emit ncpCommandDataChanged();
}
