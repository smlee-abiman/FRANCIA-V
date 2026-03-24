#include "programmenu1_3.h"
#include "programmenu1_3item.h"
#include "../../teachingprogrampagemodel.h"
#include "../../teachingtimerpagemodel.h"
#include "../command/ncpcommandprog.h"
#include "../command/ncpcommandgosub.h"
#include "../command/ncpcommandout.h"
#include "../command/ncpcommandin.h"
#include <cmath>

const quint16 PROGRAM_MENU1_3_SIGNAL_DI = 0;
const quint16 PROGRAM_JIG_NUM = 8;

const QList<QList<QString>> ProgramMenu1_3::m_jigStrList = {
    {	"SCK1",	"CSC5",     "SOK1",	"CSO5" 	},
    {	"SCK2",	"CSC6",     "SOK2",	"CSO6" 	},
    {	"SCK3",	"CSC7",     "SOK3",	"CSO7" 	},
    {	"SCK4",	"CSC8",     "SOK4",	"CSO8" 	},
    {	"SCK5",	"CSC13",    "SOK5",	"CSO13" },
    {	"SCK6",	"CSC14",    "SOK6",	"CSO14" },
    {	"SCK7",	"CSC15",    "SOK7",	"CSO15" },
    {	"SCK8",	"CSC16",    "SOK8",	"CSO16" },
    {	"CCK1",	"CSC1",     "COK1",	"CSO1" 	},
    {	"CCK2",	"CSC2",     "COK2",	"CSO2"	},
    {	"CCK3",	"CSC3",     "COK3",	"CSO3"	},
    {	"CCK4",	"CSC4",     "COK4",	"CSO4"	},
    {	"CCK5",	"CSC9",     "COK5",	"CSO9"	},
    {	"CCK6",	"CSC10",	"COK6",	"CSO10"	},
    {	"CCK7",	"CSC11",	"COK7",	"CSO11"	},
    {	"CCK8",	"CSC12",	"COK8",	"CSO12"	}
};

const QList<QList<QString>> ProgramMenu1_3::m_waitSignalStrList = {
    {	"CCM5", 	"COM5" 	},
    {	"CCM6", 	"COM6" 	},
    {	"CCM7",     "COM7" 	},
    {	"CCM8",     "COM8" 	},
    {	"CCM13",    "COM13" },
    {	"CCM14",    "COM14" },
    {	"CCM15",    "COM15" },
    {	"CCM16", 	"COM16" },
    {	"CCM1", 	"COM1" 	},
    {	"CCM2", 	"COM2" 	},
    {	"CCM3",     "COM3" 	},
    {	"CCM4",     "COM4" 	},
    {	"CCM9", 	"COM9" 	},
    {	"CCM10",	"COM10"	},
    {	"CCM11",	"COM11"	},
    {	"CCM12",	"COM12"	},
};

ProgramMenu1_3::ProgramMenu1_3(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    init();
}

ProgramMenu1_3::~ProgramMenu1_3()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_3::onOneShotReadFinished);
}

void ProgramMenu1_3::init()
{
    readDataFormatCsv();
}

void ProgramMenu1_3::readDataFormatCsv()
{
    readPenSetData();
}

void ProgramMenu1_3::readPenSetData()
{
    readJigUse();
    readKey();
    readDiAddress();
    readHrAddress();
}

void ProgramMenu1_3::readJigUse()
{
    const int jigUseStartRow = 17;
    const int titleStartIndex = 335;
    const QString group = TeachingProgramPageModel::pensetGroup();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            int index = m_resource->getPenSetStringValue(group, j+jigUseStartRow,i).toInt();
            int titleIndex = titleStartIndex + j + (8*i);
            if (index == 255) {
                auto item = new ProgramMenu1_3Item(this);
                item->setType(static_cast<ProgramMenu1_3Item::Type>(i));
                item->setNo(j + 1);
                item->setTitleIndex(titleIndex);
                m_itemList.append(item);
            } else {
                continue;
            }
        }
    }
}

void ProgramMenu1_3::readKey()
{
    int keyNo;
    int column;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 381;
    const int endRow = startRow + 2;
    const int numItems = m_itemList.length();

    for (int i = 0; i < numItems; i++) {
        auto menuItem = item(i);
        if (menuItem != nullptr) {
            column = PROGRAM_JIG_NUM * menuItem->type() + menuItem->no() - 1;
            for (int row = startRow; row < endRow; row++) {
                keyNo = m_resource->getPenSetIntValue(group, row, column);
                menuItem->appendKey(keyNo);
            }
        }
    }
}

void ProgramMenu1_3::readDiAddress()
{
    quint16 addr;
    int column;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 384;
    const int numItems = m_itemList.length();

    for (int i = 0; i < numItems; i++) {
        auto menuItem = item(i);
        if (menuItem != nullptr) {
            column = PROGRAM_JIG_NUM * menuItem->type() + menuItem->no() - 1;
            addr = static_cast<quint16>(m_resource->getPenSetIntValue(group, startRow, column));
            menuItem->setDiAddress(addr);
        }
    }
}

void ProgramMenu1_3::readHrAddress()
{
    quint16 addr;
    int column;
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 402;
    const int endRow = startRow + 4;
    const int numItems = m_itemList.length();

    for (int i = 0; i < numItems; i++) {
        auto menuItem = item(i);
        if (menuItem != nullptr) {
            column = PROGRAM_JIG_NUM * menuItem->type() + menuItem->no() - 1;
            for (int row = startRow; row < endRow; row++) {
                addr = static_cast<quint16>(m_resource->getPenSetIntValue(group, row, column));
                menuItem->appendHrAddress(addr);
                menuItem->appendTime(0);
            }
        }
    }
}

void ProgramMenu1_3::onActivate()
{
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_3::onOneShotReadFinished, Qt::QueuedConnection);
    modbusGetTime(TIMER_TIME_TOP_HR, TIMER_TIME_HR_NUM);
    modbusGetSignal();
}

void ProgramMenu1_3::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu1_3::onOneShotReadFinished);
    resetData();
}

void ProgramMenu1_3::setJigSingleChoice()
{
    const int numItems = m_itemList.length();
    for (int i = 0; i < numItems; i++) {
        auto menuItem = item(i);
        if (menuItem != nullptr) {
            if (menuItem->type() != ProgramMenu1_3Item::Type::S_ArmClamp) {
                menuItem->setState(false);
            }
        }
    }
    setSingleChoice(true);
}

void ProgramMenu1_3::setJigMultipleChoice()
{
    const int numItems = m_itemList.length();
    for (int i = 0; i < numItems; i++) {
        auto menuItem = item(i);
        if (menuItem != nullptr) {
            if (menuItem->type() == ProgramMenu1_3Item::Type::S_ArmClamp) {
                menuItem->setState(false);
            }
        }
    }
    setSingleChoice(false);
}

int ProgramMenu1_3::setTime(int itemIndex, int timeIndex, double time)
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

int ProgramMenu1_3::getKeyNo(int itemIndex, int keyIndex)
{
    if ((itemIndex <= -1) || (itemIndex >= m_itemList.length())) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid itemIndex." << itemIndex << keyIndex;
        return -1;
    }
    return item(itemIndex)->getKey(keyIndex);
}

void ProgramMenu1_3::modbusSaveData()
{
    quint16 hrAddress;
    for (int i = 0; i < m_itemList.length(); i++) {
        auto menuItem = item(i);
        if (!menuItem->state()) {
            continue;
        }
        auto hrAddressList = menuItem->hrAddressList();
        for (int j = 0; j < hrAddressList.length(); j++) {
            hrAddress = hrAddressList.at(j);
            if (hrAddress != 0) {
                modbusSaveTime(hrAddress, menuItem->getTime(j));
                modbusGetTime(hrAddress, 1);
            }
        }
    }
}

void ProgramMenu1_3::modbusSaveTime(quint16 hrAddress, int time)
{
    m_communication->writeModbusHR(hrAddress, time);
}

void ProgramMenu1_3::modbusGetTime(quint16 hrAddress, quint16 size)
{
    m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, hrAddress, size);
}


void ProgramMenu1_3::modbusGetSignal()
{
    m_communication->readModbusDI(CommunicationThread::CallerTeachingProgramPageModel, PROGRAM_MENU1_3_SIGNAL_DI, 128);
}

bool ProgramMenu1_3::isSArmClampItem(int index)
{
    bool ret = false;
    auto menuItem = item(index);
    if (menuItem != nullptr) {
        ret = (menuItem->type() == ProgramMenu1_3Item::Type::S_ArmClamp);
    }
    return ret;
}

void ProgramMenu1_3::initData()
{
    for (int i = 0; i < m_itemList.length(); i++) {
        auto item = this->item(i);
        item->setState(false);
        item->setDetect(false);
        item->setSignal(false);
    }
}

int ProgramMenu1_3::insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    int ret;
    if (singleChoice()) {
        ret = procCreateSingleChoiceCommand(file, program, lineIndex, true);
    } else {
        ret = procCreateMultipleChoiceCommand(file, program, lineIndex, true);
        QTimer::singleShot(0, this, &ProgramMenu1_3::onUpdateListReq);
    }
    return ret;
}

int ProgramMenu1_3::replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    int ret;
    if (singleChoice()) {
        ret = procCreateSingleChoiceCommand(file, program, lineIndex, false);
    } else {
        ret = procCreateMultipleChoiceCommand(file, program, lineIndex, false);
        QTimer::singleShot(0, this, &ProgramMenu1_3::onUpdateListReq);
    }
    return ret;
}

NCPStep *ProgramMenu1_3::createStep(NCPCommand *command)
{
    auto step = new NCPStep(command);
    if (step == nullptr) {
        delete command;
    }
    return step;
}

NCPCommand *ProgramMenu1_3::createGoSubCommand(int programNo)
{
    return new NCPCommandGoSub(m_resource, programNo);
}


NCPStep *ProgramMenu1_3::createOutCommand1()
{
    auto command = new NCPCommandOut(m_resource);
    if (command == nullptr)
        return nullptr;

    const int openIndex = (m_open == 0) ? 2 : 0;
    for (int i = 0; i < m_itemList.length(); i++) {
        auto *menuItem = item(i);
        if (!menuItem->state())
            continue;
        int jigStrIndex = PROGRAM_JIG_NUM * menuItem->type() + menuItem->no() - 1;
        auto jigStr = m_jigStrList.at(jigStrIndex).at(openIndex);
        NCPCommandOut::Data jigData(NCPCommandOut::Data::Bool, jigStr, true);
        command->append(jigData);

        auto detectStr = m_jigStrList.at(jigStrIndex).at(openIndex + 1);
        NCPCommandOut::Data detectData(NCPCommandOut::Data::Bool, detectStr, menuItem->detect());
        command->append(detectData);
    }
    return createStep(command);
}

NCPStep *ProgramMenu1_3::createInCommand()
{
    auto command = new NCPCommandIn(m_resource);
    if (command == nullptr)
        return nullptr;

     const int openIndex = (m_open == 0) ? 1 : 0;
     for (int i = 0; i < m_itemList.length(); i++) {
         auto *menuItem = item(i);
         if (!menuItem->state())
             continue;
         int waitSignalStrIndex = PROGRAM_JIG_NUM * menuItem->type() + menuItem->no() - 1;
         auto waitSignalStr = m_waitSignalStrList.at(waitSignalStrIndex).at(openIndex);
         auto ioAddress = m_resource->indexAt(waitSignalStr);
         auto var = new NCPVariable(",", waitSignalStr, "", "", 1, ioAddress);
         command->append(var);
     }
    return createStep(command);
}

NCPStep *ProgramMenu1_3::createOutCommand2()
{
    auto command = new NCPCommandOut(m_resource);
    if (command == nullptr)
        return nullptr;

    const int openIndex = (m_open == 0) ? 2 : 0;
    for (int i = 0; i < m_itemList.length(); i++) {
        auto *menuItem = item(i);
        if (!menuItem->state())
            continue;
        int jigStrIndex = PROGRAM_JIG_NUM * menuItem->type() + menuItem->no() - 1;
        auto jigStr = m_jigStrList.at(jigStrIndex).at(openIndex);
        NCPCommandOut::Data data(NCPCommandOut::Data::Bool, jigStr, false);
        command->append(data);
    }
    return createStep(command);
}

int ProgramMenu1_3::insertCommand(NCPProgram *program, int lineIndex, NCPCommand *command)
{
    auto step = new NCPStep(command);
    if (step == nullptr) {
        return -1;
    }
    program->insertStep(lineIndex, step);
    return 0;
}

int ProgramMenu1_3::procCreateSingleChoiceCommand(NCPFile *file, NCPProgram *program, int lineIndex, bool insert)
{
    if (file == nullptr)
        return -1;

    ProgramMenu1_3Item* menuItem = nullptr;
    const int numItems = m_itemList.length();
    for (int i = 0; i < numItems; i++) {
        menuItem = item(i);
        if ((menuItem->type() == ProgramMenu1_3Item::Type::S_ArmClamp) &&
             menuItem->state()) {
            break;
        } else {
            menuItem = nullptr;
        }
    }
    if (menuItem == nullptr)
        return -1;

    const int no = (m_open == 0) ? 142 : 141;
    auto command = createGoSubCommand(no);

    if (insert) {
        if (insertCommand(program, lineIndex, command) != 0) {
            delete command;
            return -1;
        }
    } else {
        program->replaceCommand(lineIndex, command);
    }
    modbusSaveData();
    return 0;
}

int ProgramMenu1_3::procCreateMultipleChoiceCommand(NCPFile *file, NCPProgram *program, int lineIndex, bool insert)
{
    bool selected = false;
    for (int i = 0; i < m_itemList.length(); i++) {
        auto menuItem = item(i);
        if (menuItem->state()) {
            selected = true;
            break;
        }
    }

    if (!selected) {
        return -1;
    }

    auto jigProgramNo = file->getAvailableJigProgramNo();
    if (jigProgramNo < 0) {
        return -1;
    }

    if (procCreateMainCommand(program, lineIndex, jigProgramNo, insert) != 0) {
        return -1;
    }

    procCreateSubProgram(file, jigProgramNo);
    modbusSaveData();
    return 0;
}

int ProgramMenu1_3::procCreateMainCommand(NCPProgram *program, int lineIndex, int jigProgramNo, bool insert)
{
    auto command = createGoSubCommand(jigProgramNo);
    if (command == nullptr) {
        return -1;
    }
    if (insert) {
        auto step = new NCPStep(command);
        if (step == nullptr) {
            delete command;
            return -1;
        }
        program->insertStep(lineIndex, step);
    } else {
        program->replaceCommand(lineIndex, command);
    }
    return 0;
}

NCPProgram *ProgramMenu1_3::procCreateSubProgram(NCPFile *file, int programNo)
{
    auto program = file->createProgram(programNo);
    if (program != nullptr) {
        program->insertStep(0, createOutCommand1());
        program->insertStep(1, createInCommand());
        program->insertStep(2, createOutCommand2());
    }
    return program;
}

void ProgramMenu1_3::setOpen(int open)
{
    if((open < 0) || (open > 1))
        return;
    if (m_open != open) {
        m_open = open;
        emit openChanged();
    }
}

void ProgramMenu1_3::setSingleChoice(bool choice)
{
    if (m_singleChoice != choice) {
        m_singleChoice = choice;
        emit singleChoiceChanged();
    }
}

void ProgramMenu1_3::updateSignal()
{
    quint16 diAddr;
    for (int i = 0; i < m_itemList.length(); i++) {
        auto menuItem = item(i);
        diAddr = menuItem->diAddress();
        menuItem->setSignal(m_modbus->getReceiveDataDI(diAddr));
    }
}

void ProgramMenu1_3::updateTime(quint16 hrAddress, quint16 size)
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
            auto *menuItem = item(i);
            const QList<quint16> hrAddressList = menuItem->hrAddressList();
            for (int j = 0; j < hrAddressList.length(); j++) {
                hrAddr = hrAddressList.at(j);
                time = (hrAddr == 0) ? 0 : m_modbus->getReceiveDataHR(hrAddr);
                menuItem->setTime(j, time);
            }
            menuItem->emitTimeList();
        }
    }
}

void ProgramMenu1_3::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::OneShotReadModBusCaller::CallerTeachingProgramPageModel)
        return;
    if (command.funcCode == QModbusPdu::FunctionCode::ReadDiscreteInputs) {
        if (command.readAddress == PROGRAM_MENU1_3_SIGNAL_DI) {
            updateSignal();
        }
    } else if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if ((command.readAddress >= TIMER_TIME_TOP_HR) && (command.readAddress < (TIMER_TIME_TOP_HR + TIMER_TIME_HR_NUM))) {
            updateTime(command.readAddress, command.comSize);
        }
    }
}

void ProgramMenu1_3::resetData()
{
    for (int i = 0; i < m_itemList.length(); i++)
        item(i)->resetData();
}

ProgramMenu1_3Item *ProgramMenu1_3::item(int index)
{
    if ((index <= -1) || (index >= m_itemList.length()))
        return nullptr;
    return static_cast<ProgramMenu1_3Item *>(m_itemList.at(index));
}

void ProgramMenu1_3::onUpdateListReq()
{
    emit ncpCommandDataChanged();
}
