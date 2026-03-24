#include "programmenu2_4_4.h"
#include "programmenu2_4_4item.h"
#include "../command/ncpcommandhome.h"
#include "../command/ncpcommandhomep.h"
#include "axispointinfo.h"

const int AXIS_ORIGIN_POINT_NO = 253;

ProgramMenu2_4_4::ProgramMenu2_4_4(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    init();
}

void ProgramMenu2_4_4::init()
{
    m_axisPointInfo = AxisPointInfo::get_instance();

    for (int i = 0; i < Enums::AxisNum; i++) {
        auto item = new ProgramMenu2_4_4Item(this);
        m_itemList.append(item);
    }
    m_axisOriginTopHrAddress = m_axisPointInfo->getPointPositionAddress(AXIS_ORIGIN_POINT_NO, 0);
}

void ProgramMenu2_4_4::onActivate()
{
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_4_4::onOneShotReadFinished, Qt::QueuedConnection);
    modbusGetAxisOriginSpeedData();
}

void ProgramMenu2_4_4::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_4_4::onOneShotReadFinished);
    resetPointData();
    setNoWaiting(false);
    setPlusDirection(false);
}

int ProgramMenu2_4_4::setSpeed(int axis, int speed)
{
    if ((axis < 0) || (axis >= m_itemList.length())) {
        return -1;
    }
    auto item = static_cast<ProgramMenu2_4_4Item *>(m_itemList.at(axis));
    item->setSpeed(speed);
    return 0;
}

QString ProgramMenu2_4_4::formatSpeed(int speed)
{
    const int format = m_axisPointInfo->formatArbitraryPointSpeed();
    return m_resource->formatData(speed, format, 0);
}

void ProgramMenu2_4_4::setNoWaiting(bool noWaiting)
{
    if (m_noWaiting != noWaiting) {
        m_noWaiting = noWaiting;
        emit noWaitingChanged();
    }
}

void ProgramMenu2_4_4::setPlusDirection(bool plusDirection)
{
    if (m_plusDirection != plusDirection) {
        m_plusDirection = plusDirection;
        emit plusDirectionChanged();
    }
}

void ProgramMenu2_4_4::clearSelect()
{
    for (QObject *item : m_itemList)
        static_cast<ProgramMenu2_4_4Item *>(item)->setSelect(false);
}

void ProgramMenu2_4_4::show(NCPCommand *command)
{
    switch (command->type()) {
    case NCPCommand::Type::Home:
        setNoWaiting(false);
        break;
    case NCPCommand::Type::Homep:
        setNoWaiting(true);
        break;
    default:
        return;
    }

    clearSelect();
    modbusGetAxisOriginSpeedData();
    auto homeCommand = static_cast<NCPCommandHome *>(command);
    auto option = homeCommand->option();
    if (option == NCPCommandHome::Option::ReturnPlusDirection) {
        setPlusDirection(true);
    } else if (option == NCPCommandHome::Option::ReturnMinusDirection) {
        setPlusDirection(false);
    }
    auto axisList = homeCommand->axisList();
    for (int axis : axisList) {
        if ((axis >= 0) && (axis < m_itemList.length())) {
            auto item = static_cast<ProgramMenu2_4_4Item *>(m_itemList.at(axis));
            item->setSelect(true);
        }
    }
}

NCPCommand *ProgramMenu2_4_4::createNcpCommand()
{
    NCPCommandHome *command = nullptr;
    if (m_noWaiting) {
        command = new NCPCommandHomep(m_resource);
    } else {
        command = new NCPCommandHome(m_resource);
    }
    if (command != nullptr) {
        if (m_plusDirection) {
            command->setOption(NCPCommandHome::Option::ReturnPlusDirection);
        } else {
            command->setOption(NCPCommandHome::Option::ReturnMinusDirection);
        }

        bool invalid = true;
        const int numAxis = m_itemList.length();
        for (int i = 0; i < numAxis; i++) {
            auto item = static_cast<ProgramMenu2_4_4Item *>(m_itemList.at(i));
            if (item->select()) {
                command->appendAxis(i);
                invalid = false;
            }
        }
        if (invalid) {
            delete command;
            command = nullptr;
        } else {
            modbusSetAxisOriginSpeedData();
            modbusGetAxisOriginSpeedData();
        }
    }
    return command;
}

void ProgramMenu2_4_4::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    Q_UNUSED(oneShotReadCaller)
    if (oneShotReadCaller != CommunicationThread::CallerTeachingProgramPageModel)
        return;

    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if (command.readAddress == m_axisOriginTopHrAddress) {
            updateAxisOriginSpeedData();
        }
    }
}

void ProgramMenu2_4_4::modbusSetAxisOriginSpeedData()
{
    for (int i = 0; i < m_itemList.length(); i++) {
        auto hrAddress = m_axisPointInfo->getPointSpeedAddress(AXIS_ORIGIN_POINT_NO, i);
        auto pointData = static_cast<ProgramMenu2_4_4Item *>(m_itemList.at(i));
        auto speed = pointData->speed();
        m_communication->writeModbusHR(hrAddress, speed);
    }
}

void ProgramMenu2_4_4::modbusGetAxisOriginSpeedData()
{
    m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, m_axisOriginTopHrAddress, m_axisPointInfo->pointDataSize());
}

void ProgramMenu2_4_4::updateAxisOriginSpeedData()
{
    auto hrAddress = m_axisPointInfo->getPointSpeedAddress(AXIS_ORIGIN_POINT_NO, 0);
    const int numAxis = m_itemList.length();
    for (int i = 0; i < numAxis; i++) {
        auto item = static_cast<ProgramMenu2_4_4Item *>(m_itemList.at(i));
        item->setSpeed(m_modbus->getReceiveDataHR(hrAddress));
        hrAddress += 3;
    }
}

void ProgramMenu2_4_4::resetPointData()
{
    for (QObject *data: m_itemList) {
        auto pointData = static_cast<ProgramMenu2_4_4Item *>(data);
        pointData->init();
    }
}
