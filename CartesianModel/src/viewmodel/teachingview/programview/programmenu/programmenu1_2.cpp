#include "programmenu1_2.h"
#include "programmenu1_2item.h"
#include "../../teachingprogrampagemodel.h"
#include "../../teachingtimerpagemodel.h"
#include "../command/ncpcommandgosub.h"
#include "../command/ncpcommandon.h"
#include <bitset>
#include <cmath>

const quint16 PROGRAM_MENU1_2_SIGNAL_DI = 0;
const int PROGRAM_MENU1_2_NUM_ITEMS = 16;
const int PROGRAM_MENU1_2_COUNTER_HR = 2293;      // HR2293
const int PROGRAM_MENU1_2_COUNTER_HR_END = 2339 + 1;  // HR2340
const int PROGRAM_MENU1_2_COUNTER_HR_NUM = PROGRAM_MENU1_2_COUNTER_HR_END - PROGRAM_MENU1_2_COUNTER_HR + 1;

ProgramMenu1_2::ProgramMenu1_2(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    init();
}

ProgramMenu1_2::~ProgramMenu1_2()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_2::onOneShotReadFinished);
}

void ProgramMenu1_2::init()
{
    for (int i = 0; i < PROGRAM_MENU1_2_NUM_ITEMS; i++) {
        m_itemList.append(new ProgramMenu1_2Item(this));
    }
    readDataFormatCsv();
}

void ProgramMenu1_2::readDataFormatCsv()
{
    readPenSetData();
}

void ProgramMenu1_2::readPenSetData()
{
    readProgramNo();
    readKey();
    readDiAddress();
    readHrAddress();
}

void ProgramMenu1_2::readProgramNo()
{
    int programNo;
    int itemIndex = 0;
    int numItems = 0;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 9;
    const int endRow = startRow + 8;
    for (int column = 0; column < 2; column++) {
        for (int row = startRow; row < endRow; row++) {
            programNo = m_resource->getPenSetIntValue(group, row, column);
            item(itemIndex)->setProgramNo(programNo);
            itemIndex++;
            if (programNo != 0) {
                numItems++;
            }
        }
    }
    setNumItems(numItems);
}

void ProgramMenu1_2::readKey()
{
    int keyNo;
    int itemIndex = 0;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 367;
    const int endRow = startRow + 2;
    for (int column = 0; column < 10; column++) {
        ProgramMenu1_2Item *menuItem = item(itemIndex);
        for (int row = startRow; row < endRow; row++) {
            keyNo = m_resource->getPenSetIntValue(group, row, column);
            menuItem->appendKey(keyNo);
        }
        itemIndex++;
    }
}

void ProgramMenu1_2::readDiAddress()
{
    quint16 addr;
    int itemIndex = 0;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 374;
    const int endRow = startRow + 2;
    for (int column = 0; column < 10; column++) {
        ProgramMenu1_2Item *menuItem = item(itemIndex);
        for (int row = startRow; row < endRow; row++) {
            addr = static_cast<quint16>(m_resource->getPenSetIntValue(group, row, column));
            menuItem->appendDiAddress(addr);
        }
        itemIndex++;
    }
}

void ProgramMenu1_2::readHrAddress()
{
    quint16 addr;
    int itemIndex = 0;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 397;
    const int endRow = startRow + 3;
    for (int column = 0; column < 10; column++) {
        ProgramMenu1_2Item *menuItem = item(itemIndex);
        for (int row = startRow; row < endRow; row++) {
            addr = static_cast<quint16>(m_resource->getPenSetIntValue(group, row, column));
            menuItem->appendHrAddress(addr);
            menuItem->appendTime(0);
        }
        itemIndex++;
    }
}

void ProgramMenu1_2::onActivate()
{
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_2::onOneShotReadFinished, Qt::QueuedConnection);
    modbusGetTime(TIMER_TIME_TOP_HR, TIMER_TIME_HR_NUM);
    modbusGetCounter(PROGRAM_MENU1_2_COUNTER_HR, PROGRAM_MENU1_2_COUNTER_HR_NUM);
    modbusGetSignal();
}

void ProgramMenu1_2::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_2::onOneShotReadFinished);
    resetData();
}

int ProgramMenu1_2::setTime(int itemIndex, int timeIndex, double time)
{
    if ((itemIndex <= -1) || (itemIndex >= m_itemList.length())) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid itemIndex." << itemIndex << timeIndex << time;
        return -1;
    }

    double d_value = time * TIMER_TIME_MAGNIFICATION;
    long correct_value = qMin(lround(d_value), static_cast<long>(TIMER_MAX_TIME));
    auto menuItem = item(itemIndex);
    menuItem->setTime(timeIndex, correct_value);
    menuItem->emitTimeList();
    return 0;
}

int ProgramMenu1_2::setCounter(int itemIndex, int counterIndex, int counter)
{
    if ((itemIndex <= -1) || (itemIndex >= m_itemList.length())) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid itemIndex." << itemIndex << counterIndex << counter;
        return -1;
    }

    auto menuItem = item(itemIndex);
    menuItem->setTime(counterIndex, counter);
    menuItem->emitTimeList();
    return 0;
}

int ProgramMenu1_2::getKeyNo(int itemIndex, int keyIndex)
{
    if ((itemIndex <= -1) || (itemIndex >= m_itemList.length())) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid itemIndex." << itemIndex << keyIndex;
        return -1;
    }
    return item(itemIndex)->getKey(keyIndex);
}

void ProgramMenu1_2::modbusSaveData(int menuIndex)
{
    quint16 hrAddress;
    auto menuItem = item(menuIndex);
    const QList<quint16> hrAddressList = menuItem->hrAddressList();
    for (int i = 0; i < hrAddressList.length(); i++) {
        hrAddress = hrAddressList.at(i);
        if (hrAddress != 0) {
            if (isCounterHrAddress(hrAddress)) {
                modbusSaveCounter(hrAddress, menuItem->getTime(i));
                modbusGetCounter(hrAddress, 2);
            } else {
                modbusSaveTime(hrAddress, menuItem->getTime(i));
                modbusGetTime(hrAddress, 1);
            }
        }
    }
}

void ProgramMenu1_2::modbusSaveTime(quint16 hrAddress, int time)
{
    m_communication->writeModbusHR(hrAddress, time);
}

void ProgramMenu1_2::modbusGetTime(quint16 hrAddress, quint16 size)
{
    m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, hrAddress, size);
}

void ProgramMenu1_2::modbusGetSignal()
{
    m_communication->readModbusDI(CommunicationThread::CallerTeachingProgramPageModel, PROGRAM_MENU1_2_SIGNAL_DI, 128);
}

void ProgramMenu1_2::modbusSaveCounter(quint16 hrAddress, int counter)
{
    QList<int> writeData;
    writeData << 0 << 0;
    writeData[0] = (counter & 0x0000FFFF);
    writeData[1] = (counter & 0xFFFF0000) >> 16;
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, 2, writeData);
    m_communication->setModbusWriteCommand(command);
}

void ProgramMenu1_2::modbusGetCounter(quint16 hrAddress, quint16 size)
{
    m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, hrAddress, size);
}

NCPCommand *ProgramMenu1_2::createNcpCommand()
{
    auto menuItem = item(menuIndex());
    if ((menuItem == nullptr) || !menuItem->use())
        return nullptr;

    modbusSaveData(menuIndex());
    NCPCommand *command = nullptr;
    if (menuItem->wait()) {
        command = new NCPCommandOn(m_resource, menuItem->programNo());
    } else {
        command = new NCPCommandGoSub(m_resource, menuItem->programNo());
    }
    return command;
}

void ProgramMenu1_2::show(NCPCommand *command)
{
    int programNo = 0;
    bool wait = false;
    if (command->type() == NCPCommand::Type::GoSub) {
        auto gosubCommand = dynamic_cast<NCPCommandGoSub *>(command);
        programNo = gosubCommand->programNo();
    } else if (command->type() == NCPCommand::Type::On) {
        auto onCommand = dynamic_cast<NCPCommandOn *>(command);
        programNo = onCommand->programNo();
        wait = true;
    } else {
        return;
    }

    resetUse();

    int index = getItemIndex(programNo);
    auto menuItem = item(index);
    if (menuItem == nullptr)
        return;

    menuItem->setWait(wait);
    setMenuIndex(index);
}

void ProgramMenu1_2::updateSignal()
{
    quint16 diAddr;
    for (int i = 0; i < m_itemList.length(); i++) {
        ProgramMenu1_2Item *menuItem = item(i);
        const QList<quint16> diaddresList = menuItem->diAddressList();
        std::bitset<32> bits;
        for (int j = 0; j < diaddresList.length(); j++) {
            diAddr = diaddresList.at(j);
            bits[j] = m_modbus->getReceiveDataDI(diAddr);
        }
        menuItem->setSignal(static_cast<int>(bits.to_ulong()));
    }
}

void ProgramMenu1_2::updateTime(quint16 hrAddress, quint16 size)
{
    if (size == 1) {
        for (int i = 0; i < m_itemList.length(); i++) {
            auto menuItem = item(i);
            auto hrAddressList = menuItem->hrAddressList();
            for (int j = 0; j < hrAddressList.length(); j++) {
                if (hrAddress == hrAddressList.at(j)) {
                    auto time = m_modbus->getReceiveDataHR(hrAddress);
                    menuItem->setTime(j, time);
                    menuItem->emitTimeList();
                    break;
                }
            }
        }
    } else {
        quint16 hrAddr;
        int time;
        for (int i = 0; i < m_itemList.length(); i++) {
            auto menuItem = item(i);
            auto hrAddressList = menuItem->hrAddressList();
            for (int j = 0; j < hrAddressList.length(); j++) {
                hrAddr = hrAddressList.at(j);
                time = (hrAddr == 0) ? 0 : m_modbus->getReceiveDataHR(hrAddr);
                menuItem->setTime(j, time);
            }
            menuItem->emitTimeList();
        }
    }
}

void ProgramMenu1_2::updateCounter()
{
    int hrAddress;
    for (int i = 0; i < m_itemList.length(); i++) {
        auto menuItem = item(i);
        auto hrAddressList = menuItem->hrAddressList();
        for (int j = 0; j < hrAddressList.length(); j++) {
            hrAddress = hrAddressList.at(j);
            if (isCounterHrAddress(hrAddress)) {
                int value = m_modbus->getLongReceiveDataHR(hrAddress);
                menuItem->setTime(j, value);
            }
            menuItem->emitTimeList();
        }
    }
}

void ProgramMenu1_2::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::OneShotReadModBusCaller::CallerTeachingProgramPageModel)
        return;
    if (command.funcCode == QModbusPdu::FunctionCode::ReadDiscreteInputs) {
        if (command.readAddress == PROGRAM_MENU1_2_SIGNAL_DI) {
            updateSignal();
        }
    } else if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if ((command.readAddress >= TIMER_TIME_TOP_HR) && (command.readAddress < (TIMER_TIME_TOP_HR + TIMER_TIME_HR_NUM))) {
            updateTime(command.readAddress, command.comSize);
        } else if (isCounterHrAddress(command.readAddress)) {
            updateCounter();
        }
    }
}

void ProgramMenu1_2::resetData()
{
    for (int i = 0; i < m_itemList.length(); i++)
        item(i)->resetData();
}

void ProgramMenu1_2::resetUse()
{
    for (int i = 0; i < m_itemList.length(); i++) {
        auto menuItem = item(i);
        menuItem->setUse(false);
    }
}

bool ProgramMenu1_2::isCounterHrAddress(int itemIndex, int timeIndex)
{
    auto menuItem = item(itemIndex);
    if (menuItem == nullptr) {
        return false;
    }

    auto hrAddressList = menuItem->hrAddressList();
    return isCounterHrAddress(hrAddressList.at(timeIndex));
}

bool ProgramMenu1_2::isCounterHrAddress(int address)
{
    return (address >= PROGRAM_MENU1_2_COUNTER_HR) && (address <= PROGRAM_MENU1_2_COUNTER_HR_END);
}

void ProgramMenu1_2::setNumItems(int num)
{
    if (m_numItems != num) {
        m_numItems = num;
        emit numItemsChanged();
    }
}

int ProgramMenu1_2::getItemIndex(int programNo)
{
    int ret = 0;
    for (int i = 0; i < m_itemList.length(); i++) {
        auto menuItem = item(i);
        if (programNo == menuItem->programNo()) {
            ret = i;
            break;
        }
    }
    return ret;
}
