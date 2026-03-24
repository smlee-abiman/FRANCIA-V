#include "programmenu1_4.h"
#include "programmenu1_4item.h"
#include "../command/ncpcommandoutp.h"

const int PROGRAM_MENU1_4_NUM_ITEMS = 30;

ProgramMenu1_4::ProgramMenu1_4(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    init();
}

ProgramMenu1_4::~ProgramMenu1_4()
{
}

void ProgramMenu1_4::init()
{
    for (int i = 0; i < PROGRAM_MENU1_4_NUM_ITEMS; i++)
        m_itemList.append(new ProgramMenu1_4Item(this));
    itemAt(0)->setOptional(true);
}

void ProgramMenu1_4::onActivate()
{
}

void ProgramMenu1_4::onDeactivate()
{
    clearSelect();
}

void ProgramMenu1_4::show(NCPCommand *command)
{
    if (command->type() != NCPCommand::Type::Outp)
        return;

    auto outpCommand = dynamic_cast<NCPCommandOutp *>(command);
    QList<NCPCommandOut::Data> dataList = outpCommand->getDataList();
    if (dataList.length() == 0)
        return;

    clearSelect();

    for (int i = 0; i < dataList.length(); i++) {
        auto data = dataList.at(i);
        auto item = itemAt(i);
        item->setMark(data.symbolStr());
        item->setMsgLabelIndex(m_resource->indexAt(data.symbolStr()));
        item->setValue(data.value());
        item->setSelectState(true);
        item->setOptional(false);
    }

    auto item = itemAt(dataList.length() - 1);
    item->setOptional(true);

    item = itemAt(dataList.length());
    if (item != nullptr)
        item->setOptional(true);
}

void ProgramMenu1_4::setCondition(int index, const QString &mark, const QString &msg, int value)
{
    Q_UNUSED(msg)
    auto item = itemAt(index);
    if (item == nullptr) {
        return;
    }
    item->setMark(mark);
    item->setMsgLabelIndex(m_resource->indexAt(mark));
    item->setValue(value);
}

void ProgramMenu1_4::setVariableOk(int editIndex)
{
    int lastOptionalIndex = 0;
    int len = m_itemList.length();
    for (int i = 0; i < len; i++) {
        if (itemAt(i)->optional())
            lastOptionalIndex = i;
    }

    if (editIndex < lastOptionalIndex)
        return;

    if (lastOptionalIndex == 0) {
        auto nextItem = itemAt(lastOptionalIndex + 1);
        if (nextItem != nullptr)
            nextItem->setOptional(true);
    } else {
        auto prevItem = itemAt(lastOptionalIndex - 1);
        if (prevItem != nullptr)
            prevItem->setOptional(false);

        auto nextItem = itemAt(lastOptionalIndex + 1);
        if (nextItem != nullptr)
            nextItem->setOptional(true);
    }
}

void ProgramMenu1_4::clearSelect()
{
    for (int i = 0; i < PROGRAM_MENU1_4_NUM_ITEMS; i++)
        itemAt(i)->init();
    itemAt(0)->setOptional(true);
    emit itemListChanged();
    setEditIndex(-1);
}

void ProgramMenu1_4::selectItem(int index)
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

void ProgramMenu1_4::deselectItem(int index)
{
    auto item = itemAt(index);
    if (item == nullptr) {
        return;
    }
    item->setSelectState(false);
    item->setMark("");
    item->setMsgLabelIndex(-1);
    item->setValue(0);

    auto prevItem = itemAt(index - 1);
    if (prevItem != nullptr)
        prevItem->setOptional(true);

    auto nextItem = itemAt(index + 1);
    if (nextItem != nullptr)
        nextItem->setOptional(false);
}

NCPCommand *ProgramMenu1_4::createNcpCommand()
{
    auto command = new NCPCommandOutp(m_resource);
    for (int i = 0; i < m_itemList.length(); i++) {
        auto item = itemAt(i);
        if (item->selectState() && item->mark().length() != 0) {
            NCPCommandOut::Data data(NCPCommandOut::Data::Bool, item->mark(), item->value() != 0);
            command->append(data);
        }
    }
    return command;
}

ProgramMenu1_4Item *ProgramMenu1_4::itemAt(int index)
{
    if ((index < 0) || (index >= m_itemList.length())) {
        return nullptr;
    }
    return static_cast<ProgramMenu1_4Item *>(m_itemList.at(index));
}
