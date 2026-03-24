#include "programmenu4_2_1.h"

const int PROGRAM_MENU4_2_1_NUM_ITEMS = 30;

//------------------------------------------------------
// ProgramMenu4_2_1.
//------------------------------------------------------
ProgramMenu4_2_1::ProgramMenu4_2_1(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
    , m_programNo(0)
    , m_openProgramSelectionDialog(false)
{
    for (int i = 0; i < PROGRAM_MENU4_2_1_NUM_ITEMS; i++) {
        m_itemList.append(new ProgramMenu4_2_1Item());
    }
    itemAt(0)->setOptional(true);
}
//------------------------------------------------------
ProgramMenu4_2_1::~ProgramMenu4_2_1()
{
    qDeleteAll(m_itemList);
    m_itemList.clear();
}
//------------------------------------------------------
void ProgramMenu4_2_1::show(NCPCommand *command)
{
    auto subMenuIndex = menuIndex();
    // select menu index by command.
    switch (command->type()) {
    case NCPCommand::Type::GoSub: {
            auto gosubCommand = dynamic_cast<NCPCommandGoSub*>(command);
            show(gosubCommand);
            subMenuIndex = 0;
        }
        break;
    case NCPCommand::Type::IfGoSub: {
            auto ifgosubCommand = dynamic_cast<NCPCommandIfGoSub*>(command);
            show(ifgosubCommand);
            subMenuIndex = 1;
        }
        break;
    }
    setMenuIndex(subMenuIndex);
}
//------------------------------------------------------
void ProgramMenu4_2_1::show(NCPCommandGoSub* command)
{
    clearItems();
    setProgramNo(command->programNo());
}
//------------------------------------------------------
void ProgramMenu4_2_1::show(NCPCommandIfGoSub* command)
{
    int lastOptionalIndex = 0;
    for (int i = 0; i < PROGRAM_MENU4_2_1_NUM_ITEMS; i++) {
        auto item = itemAt(i);
        auto var = command->variableAt(i);
        item->setOptional(false);
        if (var == nullptr) {
            item->setSelectState(false);
            item->setRelation("and");
            item->setSymbol("");
            item->setOpeStr("");
            item->setValue(0);
            item->setIoAddress(-1);
        } else {
            lastOptionalIndex = i;
            item->setSelectState(true);
            item->setRelation(var->relation());
            item->setSymbol(var->symbol());
            item->setOpeStr(var->opeStr());
            item->setValue(var->value());
            item->setIoAddress(var->ioAddress());
        }
    }
    auto item = itemAt(lastOptionalIndex);
    item->setOptional(true);

    auto nextItem = itemAt(lastOptionalIndex + 1);
    if (nextItem != nullptr)
        nextItem->setOptional(true);

    emit itemListChanged();

    setProgramNo(command->programNo());
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_2_1::createNcpCommand()
{
    return (m_menuIndex == 0) ? createNcpCommandGoSub() :
           (m_menuIndex == 1) ? createNcpCommandIfGoSub() : nullptr;
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_2_1::createNcpCommandGoSub()
{
    if (m_programNo <= 0) { return nullptr; }
    auto onCommand = new NCPCommandGoSub(m_resource, m_programNo);
    return onCommand;
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_2_1::createNcpCommandIfGoSub()
{
    if (m_programNo <= 0) { return nullptr; }
    auto command = new NCPCommandIfGoSub(m_resource, m_programNo);
    auto isCondition = false;
    for (int i = 0; i < PROGRAM_MENU4_2_1_NUM_ITEMS; i++) {
        auto item = itemAt(i);
        if (item->selectState() && item->symbol() != "") {
            auto var = new NCPVariable(item->programRelation(),
                                       item->symbol(),
                                       item->symbol1(),
                                       item->programOpeStr(),
                                       item->value(),
                                       item->ioAddress()
                                       );
            command->append(var);
            isCondition = true;
        }
    }
    if (!isCondition) {
        delete command;
        return nullptr;
    }
    return command;
}
//------------------------------------------------------
void ProgramMenu4_2_1::clearItems()
{
    setProgramNo(0);
    for (int i = 0; i < PROGRAM_MENU4_2_1_NUM_ITEMS; i++) {
        auto item = itemAt(i);
        item->setOptional(false);
        item->setSelectState(false);
        item->setRelation("and");
        item->setSymbol("");
        item->setOpeStr("");
        item->setValue(0);
        item->setIoAddress(-1);
    }
    itemAt(0)->setOptional(true);
    emit itemListChanged();
}
//------------------------------------------------------
void ProgramMenu4_2_1::selectItem(int index)
{
    auto item = itemAt(index);
    if (item == nullptr) {
        return;
    }
    item->setSelectState(true);

    auto prevItem = itemAt(index - 1);
    if (prevItem != nullptr)
        prevItem->setOptional(false);
}
//------------------------------------------------------
void ProgramMenu4_2_1::deselectItem(int index)
{
    auto item = itemAt(index);
    if (item == nullptr) {
        return;
    }
    item->setSelectState(false);
    item->setRelation("and");
    item->setSymbol("");
    item->setOpeStr("");
    item->setValue(0);
    item->setIoAddress(-1);

    auto prevItem = itemAt(index - 1);
    if (prevItem != nullptr)
        prevItem->setOptional(true);

    auto nextItem = itemAt(index + 1);
    if (nextItem != nullptr)
        nextItem->setOptional(false);
}
//------------------------------------------------------
void ProgramMenu4_2_1::setCondition(int index, const QString &symbol, const QString &opeStr, int value, int ioAddress)
{
    auto item = itemAt(index);
    if (item == nullptr) {
        return;
    }
    item->setSymbol(symbol);
    item->setOpeStr(opeStr);
    item->setValue(value);
    item->setIoAddress(ioAddress);
}
//------------------------------------------------------
void ProgramMenu4_2_1::setVariableOk(int index)
{
    int lastOptionalIndex = 0;
    int len = m_itemList.length();
    for (int i = len - 1; 0 <= i; i--) {
        if (itemAt(i)->optional()) {
            lastOptionalIndex = i;
            break;
        }
    }
    if (index < lastOptionalIndex)
        return;

    auto prevItem = itemAt(lastOptionalIndex - 1);
    if (prevItem != nullptr)
        prevItem->setOptional(false);

    auto nextItem = itemAt(lastOptionalIndex + 1);
    if (nextItem != nullptr)
        nextItem->setOptional(true);
}
//------------------------------------------------------
void ProgramMenu4_2_1::onActivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_2_1::onDeactivate()
{
    // nop.
}
//------------------------------------------------------
ProgramMenu4_2_1Item *ProgramMenu4_2_1::itemAt(int index)
{
    if ((index < 0) || (index >= m_itemList.length())) {
        return nullptr;
    }
    return dynamic_cast<ProgramMenu4_2_1Item *>(m_itemList.at(index));
}
