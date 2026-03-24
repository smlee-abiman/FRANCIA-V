#include "programmenu5_2.h"
#include "programmenu5_2item.h"
#include "../command/ncpcommandout.h"
#include <QRegularExpression>

const int PROGRAM_MENU5_2_NUM_ITEMS = 30;
const QString PROGRAM_MENU5_2_SYMBOL_MSG_NAME_GROUP = "1";

ProgramMenu5_2::ProgramMenu5_2(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    menu_init();
}

void ProgramMenu5_2::onActivate()
{
    //qDebug("ProgramMenu5_2 %s:%d", __func__, __LINE__);
}

void ProgramMenu5_2::onDeactivate()
{
    //qDebug("ProgramMenu5_2 %s:%d", __func__, __LINE__);
    clearSelect();
}

void ProgramMenu5_2::menu_init()
{
    //qDebug("ProgramMenu5_2 %s:%d", __func__, __LINE__);
    for (int i = 0; i < PROGRAM_MENU5_2_NUM_ITEMS; i++) {
        m_itemList.append(new ProgramMenu5_2Item(this));
    }
    itemAt(0)->setOptional(true);
}

void ProgramMenu5_2::setCondition(int index, const QString &mark, const QString &opeStr, const QString &msg, int value)
{
    Q_UNUSED(msg)
    //qDebug() << "index:" << index << "mark:" << mark << "opeStr:" << opeStr << "msg:" << msg << "value:" << value;
    auto item = itemAt(index);
    if (item == nullptr) {
        return;
    }
    item->setMark(mark);
    item->setMsgLabelIndex(m_resource->indexAt(mark));
    item->setOpeStr(opeStr);
    item->setValue(value);
}

void ProgramMenu5_2::setVariableOk(int editIndex)
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

void ProgramMenu5_2::clearSelect()
{
    for (int i = 0; i < PROGRAM_MENU5_2_NUM_ITEMS; i++)
        itemAt(i)->init();
    itemAt(0)->setOptional(true);
    emit itemListChanged();
    setEditIndex(-1);
}

void ProgramMenu5_2::selectItem(int index)
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

void ProgramMenu5_2::deselectItem(int index)
{
    auto item = itemAt(index);
    if (item == nullptr) {
        return;
    }
    item->setSelectState(false);
    item->setMark("");
    item->setMsgLabelIndex(-1);
    item->setOpeStr("");
    item->setValue(0);

    auto prevItem = itemAt(index - 1);
    if (prevItem != nullptr)
        prevItem->setOptional(true);

    auto nextItem = itemAt(index + 1);
    if (nextItem != nullptr)
        nextItem->setOptional(false);
}

void ProgramMenu5_2::clearItems()
{
    //setLabelNo(0);
    for (int i = 0; i < PROGRAM_MENU5_2_NUM_ITEMS; i++) {
        auto item = itemAt(i);
        item->setOptional(false);
        item->setSelectState(false);
        item->setMark("");
        item->setMsgLabelIndex(-1);
        item->setOpeStr("");
        item->setValue(0);
    }
    itemAt(0)->setOptional(true);
    emit itemListChanged();
}

NCPCommand *ProgramMenu5_2::createNcpCommand()
{
    auto command = new NCPCommandOut(m_resource);
    auto isCondition = false;
    for (int i = 0; i < m_itemList.length(); i++) {
        auto item = itemAt(i);
        if (item->selectState() && item->mark().length() != 0) {
            if (item->opeStr().length() != 0){
                //qDebug() << "mark:" << item->mark() << "opeStr:" << item->opeStr() << "value:" << item->value() << "msg:" << item->msg();
                NCPCommandOut::Data data(NCPCommandOut::Data::Int, item->mark(), item->value());
                command->append(data);
            } else {
                //qDebug() << "mark:" << item->mark() << "value:" << item->value() << "msg:" << item->msg();
                NCPCommandOut::Data data(NCPCommandOut::Data::Bool, item->mark(), item->value() != 0);
                command->append(data);
            }
            isCondition = true;
        }
    }
    if (!isCondition){
        //qDebug() << "ProgramMenu5_2::createNcpCommand isCondition:" << isCondition;
        delete command;
        return nullptr;
    }
    return command;
}

void ProgramMenu5_2::show(NCPCommand *command)
{
    //qDebug() << "ProgramMenu5_2::show command type:" << command->type();
    clearItems();
    auto outCommand = dynamic_cast<NCPCommandOut*>(command);
    QList<NCPCommandOut::Data> dataList = outCommand->getDataList();
    //qDebug() << "ProgramMenu5_2::show dataList count:" << dataList.count() << "m_itemList count:" << m_itemList.count();
    for(int i = 0; i < dataList.count(); i++){
        //qDebug() << "ProgramMenu5_2::show dataList symbolStr:" << dataList.at(i).symbolStr() << "value:" << dataList.at(i).value() << "type:" << dataList.at(i).type() << "mark:" << dataList.at(i).markStr();
        auto item = itemAt(i);
        if (item == nullptr) { continue; }
        item->setMark(dataList.at(i).symbolStr());
        item->setMsgLabelIndex(m_resource->indexAt(dataList.at(i).symbolStr()));
        //qDebug() << "ProgramMenu5_2::show symbolMsg:" << symbolMsg << "opeStr:" << item->opeStr();
        if(dataList.at(i).type() == NCPCommandOut::Data::Int){
            item->setSelectState(true);
            item->setOptional((i == dataList.count() - 1) ? true: false);
            item->setOpeStr("=");
            item->setValue(dataList.at(i).value());
        } else {
            item->setSelectState(true);
            item->setOptional((i == dataList.count() - 1) ? true: false);
            item->setOpeStr("");
            item->setValue(dataList.at(i).value());
        }
    }
    if (dataList.count() < PROGRAM_MENU5_2_NUM_ITEMS){
        // next item
        auto item = itemAt(dataList.count());
        if (item != nullptr){
            item->setSelectState(false);
            item->setOptional(true);
        }
    }
}

ProgramMenu5_2Item *ProgramMenu5_2::itemAt(int index)
{
    if ((index < 0) || (index >= m_itemList.length())) {
        return nullptr;
    }
    return static_cast<ProgramMenu5_2Item *>(m_itemList.at(index));
}
