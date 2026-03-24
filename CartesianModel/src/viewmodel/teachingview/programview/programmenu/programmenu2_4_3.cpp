#include "programmenu2_4_3.h"
#include "programmenu2_4_3item.h"
#include "../command/ncpcommandpath.h"

ProgramMenu2_4_3::ProgramMenu2_4_3(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    init();
}

void ProgramMenu2_4_3::init()
{
    for (int i = 0; i < Enums::AxisNum; i++) {
        auto item = new ProgramMenu2_4_3Item(this);
        m_itemList.append(item);
    }
}

void ProgramMenu2_4_3::onDeactivate()
{
    initItemList();
    setPassOn(true);
}

int ProgramMenu2_4_3::setPosition(int axis, int position)
{
    auto item = static_cast<ProgramMenu2_4_3Item *>(m_itemList.at(axis));
    item->setPosition(position);
    return 0;
}

QString ProgramMenu2_4_3::formatPosition(int position)
{
    const int format = 66;
    return m_resource->formatData(position, format, 2, true);
}

void ProgramMenu2_4_3::clearSelect()
{
    for (QObject *item : m_itemList)
        static_cast<ProgramMenu2_4_3Item *>(item)->setSelect(false);
}

void ProgramMenu2_4_3::initItemList()
{
    for (QObject *item : m_itemList)
        static_cast<ProgramMenu2_4_3Item *>(item)->init();
}

void ProgramMenu2_4_3::show(NCPCommand *command)
{
    if (command->type() != NCPCommand::Type::Path)
        return;

    for (QObject *item : m_itemList)
        static_cast<ProgramMenu2_4_3Item *>(item)->init();

    auto pathCommand = static_cast<NCPCommandPath *>(command);
    setPassOn(pathCommand->on());
    if (pathCommand->on()) {
        auto positionList = pathCommand->positionList();
        for (auto data : positionList) {
            int axis = data.axis();
            if ((axis >= 0) && (axis < m_itemList.length())) {
                auto item = static_cast<ProgramMenu2_4_3Item *>(m_itemList.at(axis));
                item->setSelect(true);
                item->setPosition(data.position());
            }
        }
    }
    emit itemListChanged();
}

NCPCommand *ProgramMenu2_4_3::createNcpCommand()
{
    auto command = new NCPCommandPath(m_resource);
    if (command != nullptr) {
        command->setOn(m_passOn);
        if (m_passOn) {
            const int num = m_itemList.length();
            bool invalid = true;
            for (int i = 0; i < num; i++) {
                auto item = static_cast<ProgramMenu2_4_3Item *>(m_itemList.at(i));
                if (item->select()) {
                    command->setPosition(i, item->position());
                    invalid = false;
                }
            }
            if (invalid) {
                delete command;
                command = nullptr;
            }
        }
    }
    return command;
}

void ProgramMenu2_4_3::setPassOn(bool on)
{
    if (m_passOn != on) {
        m_passOn = on;
        emit passOnChanged();
    }
}
