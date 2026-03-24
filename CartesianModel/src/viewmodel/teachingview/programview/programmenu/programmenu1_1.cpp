#include "programmenu1_1.h"
#include "programmenu1_1item.h"
#include "../../teachingprogrampagemodel.h"
#include "../../teachingtimerpagemodel.h"
#include "../command/ncpcommandgosub.h"
#include "../command/ncpcommandon.h"
#include <cmath>

const quint16 PROGRAM_MENU1_1_DETECT_HR = 2087; // mode6
const quint8 PROGRAM_MENU1_1_DETECT_BIT = 1;

ProgramMenu1_1::ProgramMenu1_1(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    init();
}

ProgramMenu1_1::~ProgramMenu1_1()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_1::onOneShotReadFinished);
}

NCPCommand *ProgramMenu1_1::createNcpCommand()
{
    modbusSaveData(menuIndex());
    NCPCommand *command = nullptr;
    int programNo = m_programNoList.at(menuIndex());
    if (programNo == 148) {
        command = new NCPCommandOn(m_resource, programNo);
    } else {
        command = new NCPCommandGoSub(m_resource, programNo);
    }
    return command;
}

void ProgramMenu1_1::show(NCPCommand *command)
{
    int programNo = 0;
    if (command->type() == NCPCommand::Type::GoSub) {
        auto gosubCommand = dynamic_cast<NCPCommandGoSub *>(command);
        programNo = gosubCommand->programNo();
    } else if (command->type() == NCPCommand::Type::On) {
        auto onCommand = dynamic_cast<NCPCommandOn *>(command);
        programNo = onCommand->programNo();
    } else {
        return;
    }
    setMenuIndex(getItemIndex(programNo));
}

void ProgramMenu1_1::setDetect(bool detect)
{
    if (m_detect != detect) {
        m_detect = detect;
        emit detectChanged();
    }
}

int ProgramMenu1_1::getItemIndex(int programNo)
{
    int ret = 0;
    for (int i = 0; i < m_programNoList.length(); i++) {
        if (programNo == m_programNoList.at(i)) {
            ret = i;
            break;
        }
    }
    return ret;
}

void ProgramMenu1_1::init()
{
    readDataFormatCsv();
}

void ProgramMenu1_1::readDataFormatCsv()
{
    readPenSetData();
}

void ProgramMenu1_1::readPenSetData()
{
    readPenSetProgramNo();
    readPenSetTimerAddress();
}

void ProgramMenu1_1::readPenSetProgramNo()
{
    m_programNoList.clear();
    int programNo;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 1;
    const int endRow = startRow + 8;
    for (int column = 0; column < 2; column++) {
        for (int row = startRow; row < endRow; row++) {
            programNo = m_resource->getPenSetIntValue(group, row, column);
            if (programNo != 0) {
                m_programNoList.append(programNo);
                m_itemList.append(new ProgramMenu1_1Item(this));
            }
        }
    }
    emit programNoListChanged();
}

void ProgramMenu1_1::readPenSetTimerAddress()
{
    int timerAddress;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 386;
    const int endRow = startRow + 3;
    const int endColumn = m_itemList.length();
    for (int column = 0; column < endColumn; column++) {
        auto item = dynamic_cast<ProgramMenu1_1Item *>(m_itemList.at(column));
        for (int row = startRow; row < endRow; row++) {
            timerAddress = m_resource->getPenSetIntValue(group, row, column);
            item->appendTimerAddress(static_cast<quint16>(timerAddress));
        }
    }
}

void ProgramMenu1_1::onActivate()
{
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_1::onOneShotReadFinished, Qt::QueuedConnection);
    modbusGetTime(TIMER_TIME_TOP_HR, TIMER_TIME_HR_NUM);
    modbusGetDetect();
}

void ProgramMenu1_1::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_1::onOneShotReadFinished);
    setDetect(false);
}

int ProgramMenu1_1::setTime(int programNo, int timerIndex, double time)
{
    int index = getItemIndex(programNo);
    auto item = dynamic_cast<ProgramMenu1_1Item *>(m_itemList.value(index, nullptr));
    if (item == nullptr) {
        return -1;
    }

    double d_value = time * TIMER_TIME_MAGNIFICATION;
    long correct_value = qMin(lround(d_value), static_cast<long>(TIMER_MAX_TIME));
    int ret =item->setTime(timerIndex, correct_value);
    emit item->timeListChanged();
    return ret;
}

void ProgramMenu1_1::modbusSaveDetect(bool detect)
{
    int value = m_modbus->getReceiveDataHR(PROGRAM_MENU1_1_DETECT_HR);
    if (detect) {
        value |= (1 << PROGRAM_MENU1_1_DETECT_BIT);
    } else {
        value &= ~(1 << PROGRAM_MENU1_1_DETECT_BIT);
    }
    writeHR(PROGRAM_MENU1_1_DETECT_HR, value);
    modbusGetDetect();
}

void ProgramMenu1_1::modbusSaveData(int menuIndex)
{
    switch (menuIndex) {
    case 0:
    {
        int programNo = m_programNoList.at(menuIndex);
        modbusSaveTime(programNo);
        modbusGetTime(TIMER_TIME_TOP_HR, TIMER_TIME_HR_NUM);
    }
        break;
    case 2:
    {
        modbusSaveDetect(m_detect);
        int programNo = m_programNoList.at(menuIndex);
        modbusSaveTime(programNo);
        modbusGetTime(TIMER_TIME_TOP_HR, TIMER_TIME_HR_NUM);
    }
        break;
    default:
        break;
    }
}

int ProgramMenu1_1::modbusSaveTime(int programNo)
{
    int index = getItemIndex(programNo);
    auto item = dynamic_cast<ProgramMenu1_1Item *>(m_itemList.value(index, nullptr));
    if (item == nullptr) {
        return -1;
    }

    for (int i = 0; i < item->length(); i++) {
        auto hrAddress = item->getTimerAddress(i);
        if (hrAddress != 0) {
            modbusSaveTime(hrAddress, item->getTime(i));
        }
    }
    emit item->timeListChanged();
    return 0;
}

void ProgramMenu1_1::modbusSaveTime(quint16 hrAddress, int time)
{
    writeHR(hrAddress, time);
}

void ProgramMenu1_1::modbusGetTime(quint16 hrAddress, quint16 size)
{
    readHR(hrAddress, size);
}

void ProgramMenu1_1::modbusGetDetect()
{
    readHR(PROGRAM_MENU1_1_DETECT_HR, 1);
}

void ProgramMenu1_1::updateTime()
{
    for (int i = 0; i < m_itemList.length(); i++) {
        auto item = dynamic_cast<ProgramMenu1_1Item *>(m_itemList.at(i));
        for (int j = 0; j < item->length(); j++) {
            auto address = item->getTimerAddress(j);
            auto time = m_modbus->getReceiveDataHR(address);
            item->setTime(j, time);
        }
        emit item->timeListChanged();
    }
}

void ProgramMenu1_1::updateDetect()
{
    bool detect = (m_modbus->getReceiveDataHR(PROGRAM_MENU1_1_DETECT_HR) >> PROGRAM_MENU1_1_DETECT_BIT) & 0x00000001;
    setDetect(detect);
}


void ProgramMenu1_1::writeHR(quint16 addr, int value)
{
    QList<int> writeData;
    writeData.append(value);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, addr, 0x0001, writeData);
    m_communication->setModbusWriteCommand(command);

}

void ProgramMenu1_1::readHR(quint16 addr, quint16 size)
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, addr, size);
    setOneShotModbusCommand(CommunicationThread::CallerTeachingProgramPageModel, command);
}

void ProgramMenu1_1::setOneShotModbusCommand(CommunicationThread::OneShotReadModBusCaller caller, stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(caller);
    m_communication->setOneShotModbusCommand(data);
}

void ProgramMenu1_1::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::OneShotReadModBusCaller::CallerTeachingProgramPageModel)
        return;

    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if (command.readAddress == PROGRAM_MENU1_1_DETECT_HR) {
            updateDetect();
        } else if ((command.readAddress >= TIMER_TIME_TOP_HR) && (command.readAddress < (TIMER_TIME_TOP_HR + TIMER_TIME_HR_NUM))) {
            updateTime();
        }
    }
}
