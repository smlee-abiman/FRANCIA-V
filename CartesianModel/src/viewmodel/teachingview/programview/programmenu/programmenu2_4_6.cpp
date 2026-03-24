#include "programmenu2_4_6.h"
#include "programmenu2_4_6item.h"
#include "../command/ncpcommandacc.h"

const int PROGRAMMENU2_4_6_ACCELERATION_MAX = 26;

ProgramMenu2_4_6::ProgramMenu2_4_6(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    init();
}

void ProgramMenu2_4_6::init()
{
    for (int i = 0; i < Enums::AxisNum; i++) {
        auto item = new ProgramMenu2_4_6Item(this);
        m_itemList.append(item);
    }
}

void ProgramMenu2_4_6::onDeactivate()
{
    initItemList();
    setAccOn(true);
}

void ProgramMenu2_4_6::show(NCPCommand *command)
{
    if (command->type() != NCPCommand::Type::Acc)
        return;

    initItemList();

    auto accCommand = static_cast<NCPCommandAcc *>(command);
    setAccOn(accCommand->on());
    if (accCommand->on()) {
        auto accelerationList = accCommand->accelerationList();
        for (auto data : accelerationList) {
            int axis = data.axis();
            if ((axis >= 0) && (axis < m_itemList.length())) {
                auto item = static_cast<ProgramMenu2_4_6Item *>(m_itemList.at(axis));
                item->setSelect(true);
                int acceleration = std::max(PROGRAMMENU2_4_6_ACCELERATION_MAX - data.acceleration() - 1, 0);
                item->setAcceleration(acceleration);
            }
        }
    }
    emit itemListChanged();
}


NCPCommand *ProgramMenu2_4_6::createNcpCommand()
{
    auto command = new NCPCommandAcc(m_resource, nullptr);
    if (command != nullptr) {
        command->setOn(m_accOn);
        if (m_accOn) {
            const int num = m_itemList.length();
            bool invalid = true;
            for (int i = 0; i < num; i++) {
                auto item = static_cast<ProgramMenu2_4_6Item *>(m_itemList.at(i));
                if (item->select()) {
                    int cmdAcceleration = PROGRAMMENU2_4_6_ACCELERATION_MAX - item->acceleration() - 1;
                    command->setAcceleration(i, cmdAcceleration);
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

void ProgramMenu2_4_6::clearSelect()
{
    for (QObject *item : m_itemList)
        static_cast<ProgramMenu2_4_6Item *>(item)->setSelect(false);
}

void ProgramMenu2_4_6::setAccOn(bool on)
{
    if (m_accOn != on) {
        m_accOn = on;
        emit accOnChanged();
    }
}

void ProgramMenu2_4_6::initItemList()
{
    for (QObject *item : m_itemList)
        static_cast<ProgramMenu2_4_6Item *>(item)->init();
}

