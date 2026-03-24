#include "programmenu4_1.h"
#include "../command/ncpcommandin.h"

const quint16 OVER_TIME_HR = 2537;
const int PROGRAM_MENU4_1_NUM_ITEMS = 30;

//------------------------------------------------------
// ProgramMenu4_1.
//------------------------------------------------------
ProgramMenu4_1::ProgramMenu4_1(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
    , mOverTime(0)
{
    for (int i = 0; i < PROGRAM_MENU4_1_NUM_ITEMS; i++) {
        m_itemList.append(new ProgramMenu4_1Item());
    }
    itemAt(0)->setOptional(true);
}
//------------------------------------------------------
ProgramMenu4_1::~ProgramMenu4_1()
{
    if (m_communication) {
        disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &ProgramMenu4_1::onOneShotReadFinished);
        m_communication = nullptr;
    }
    qDeleteAll(m_itemList);
    m_itemList.clear();
}
//------------------------------------------------------
void ProgramMenu4_1::show(NCPCommand *command)
{
    auto inCommand = dynamic_cast<NCPCommandIn*>(command);
    int lastOptionalIndex = 0;
    for (int i = 0; i < PROGRAM_MENU4_1_NUM_ITEMS; i++) {
        auto item = itemAt(i);
        auto var = inCommand->variableAt(i);
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
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_1::createNcpCommand()
{
    auto command = new NCPCommandIn(m_resource);
    auto isCondition = false;
    for (int i = 0; i < PROGRAM_MENU4_1_NUM_ITEMS; i++) {
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
void ProgramMenu4_1::clearItems()
{
    for (int i = 0; i < PROGRAM_MENU4_1_NUM_ITEMS; i++) {
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
void ProgramMenu4_1::selectItem(int index)
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
void ProgramMenu4_1::deselectItem(int index)
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
void ProgramMenu4_1::setCondition(int index, const QString &symbol, const QString &opeStr, int value, int ioAddress)
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
void ProgramMenu4_1::setVariableOk(int index)
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
void ProgramMenu4_1::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerTeachingProgramPageModel) {
        return;
    }
    if (command.comSize == 0) {
        return;
    }
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        // receive HR.
        if (command.readAddress == OVER_TIME_HR) {
            updateOverTime(command);
        }
    }
}
//------------------------------------------------------
void ProgramMenu4_1::onActivate()
{
    if (!m_communication) { return; }
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
        this, &ProgramMenu4_1::onOneShotReadFinished, Qt::QueuedConnection);
    modbusGetOverTime();
}
//------------------------------------------------------
void ProgramMenu4_1::onDeactivate()
{
    if (!m_communication) { return; }
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
        this, &ProgramMenu4_1::onOneShotReadFinished);
}
//------------------------------------------------------
void ProgramMenu4_1::modbusGetOverTime()
{
    m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, OVER_TIME_HR, 1);
}
//------------------------------------------------------
void ProgramMenu4_1::updateOverTime(const stModBusCommunicationData &command)
{
    setOverTime(m_modbus->getReceiveDataHR(command.readAddress));
}
//------------------------------------------------------
ProgramMenu4_1Item *ProgramMenu4_1::itemAt(int index)
{
    if ((index < 0) || (index >= m_itemList.length())) {
        return nullptr;
    }
    return static_cast<ProgramMenu4_1Item *>(m_itemList.at(index));
}
