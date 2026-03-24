#include "programmenu2_4_2.h"
#include "programmenu2_4_2item.h"
#include "../command/ncpcommandarc.h"
#include "../command/ncpcommandarcc.h"
#include "axispointinfo.h"
#include "../ncpfile.h"
#include "../ncparbitrarypoint.h"

const quint16 CURRENT_POSITION_TOP_SR_ADDR = 2769;

ProgramMenu2_4_2::ProgramMenu2_4_2(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile)
    : ProgramMenu(resource,comm),
      m_ncpFile(ncpFile)
{
    init();
}

ProgramMenu2_4_2::~ProgramMenu2_4_2()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_4_2::onOneShotReadFinished);
}

void ProgramMenu2_4_2::setNcpFile(NCPFile *ncpFile)
{
    m_ncpFile = ncpFile;
}

void ProgramMenu2_4_2::init()
{
    m_axisPointInfo = AxisPointInfo::get_instance();

    for (int i = 0; i < Enums::AxisNum; i++) {
        auto item = new ProgramMenu2_4_2Item(this);
        m_pointDataList.append(item);
    }
}

void ProgramMenu2_4_2::onActivate()
{
    auto pointObj = m_ncpFile->findFreeArbitraryPoint();
    if (pointObj != nullptr) {
        auto pointNo = pointObj->no();
        setHalfwayPointNo(pointNo);
    }
    pointObj = m_ncpFile->findFreeArbitraryPoint(1);
    if (pointObj != nullptr) {
        auto pointNo = pointObj->no();
        setEndPointNo(pointNo);
    }
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_4_2::onOneShotReadFinished, Qt::QueuedConnection);
    modbusGetPointData(m_halfwayPointNo);
    modbusGetPointData(m_endPointNo);
}

void ProgramMenu2_4_2::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_4_2::onOneShotReadFinished);
    resetPointData();
    setConsecutive(false);
    setIsEndPoint(false);
    m_show = false;
}

void ProgramMenu2_4_2::setSpeed(int speed)
{
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged();
    }
}

void ProgramMenu2_4_2::setConsecutive(bool consecutive)
{
    if (m_consecutive != consecutive) {
        m_consecutive = consecutive;
        emit consecutiveChanged();
    }
}

void ProgramMenu2_4_2::show(NCPCommand *command)
{
    switch (command->type()) {
    case NCPCommand::Type::Arc:
        setConsecutive(false);
        break;
    case NCPCommand::Type::Arcc:
        setConsecutive(true);
        break;
    default:
        return;
    }

    m_show = true;
    clearSelect();

    auto arcCommand = static_cast<NCPCommandArc *>(command);
    setHalfwayPointNo(arcCommand->pointNo());
    setEndPointNo(arcCommand->endPointNo());
    modbusGetPointData(m_halfwayPointNo);
    modbusGetPointData(m_endPointNo);
    auto axisList = arcCommand->axisList();
    for (int axis : axisList) {
        if ((axis >= 0) && (axis < m_pointDataList.length())) {
            auto item = static_cast<ProgramMenu2_4_2Item *>(m_pointDataList.at(axis));
            item->setSelect(true);
        }
    }
}

NCPCommand *ProgramMenu2_4_2::createNcpCommand()
{
    bool invalid = true;
    NCPCommandArc *command = nullptr;
    if (m_consecutive) {
        command = new NCPCommandArcc(m_resource);
    } else {
        command = new NCPCommandArc(m_resource);
    }

    if (command != nullptr) {
        if (m_show) {
            auto pointObj = m_ncpFile->findFreeArbitraryPoint();
            if (pointObj != nullptr) {
                auto pointNo = pointObj->no();
                setHalfwayPointNo(pointNo);
            }
            pointObj = m_ncpFile->findFreeArbitraryPoint(1);
            if (pointObj != nullptr) {
                auto pointNo = pointObj->no();
                setEndPointNo(pointNo);
            }
        }
        command->setPointNo(m_halfwayPointNo);
        command->setEndPointNo(m_endPointNo);
        const int numAxis = m_pointDataList.length();
        for (int i = 0; i < numAxis; i++) {
            auto data = static_cast<ProgramMenu2_4_2Item *>(m_pointDataList.at(i));
            if (data->select()) {
                command->appendAxis(i);
                invalid = false;
            }
        }
        if (invalid) {
            delete command;
            command = nullptr;
        } else {
            modbusSetPointData(m_halfwayPointNo, false);
            modbusSetPointData(m_endPointNo, true);
            modbusGetPointData(m_halfwayPointNo);
            modbusGetPointData(m_endPointNo);
            m_ncpFile->assignPoint(m_halfwayPointNo);
            m_ncpFile->assignPoint(m_endPointNo);
            auto pointObj = m_ncpFile->findFreeArbitraryPoint();
            if (pointObj != nullptr) {
                auto pointNo = pointObj->no();
                setHalfwayPointNo(pointNo);
                modbusGetPointData(pointNo);
            }
            pointObj = m_ncpFile->findFreeArbitraryPoint(1);
            if (pointObj != nullptr) {
                auto pointNo = pointObj->no();
                setEndPointNo(pointNo);
                modbusGetPointData(pointNo);
            }
            m_dataChanged = true;
        }
    }
    m_show = false;
    return command;
}

int ProgramMenu2_4_2::setHalfwayPosition(int axis, int position)
{
    auto item = static_cast<ProgramMenu2_4_2Item *>(m_pointDataList.at(axis));
    item->setHalfwayPosition(position);
    return 0;
}

int ProgramMenu2_4_2::setEndPosition(int axis, int position)
{
    auto item = static_cast<ProgramMenu2_4_2Item *>(m_pointDataList.at(axis));
    item->setEndPosition(position);
    return 0;
}

int ProgramMenu2_4_2::setCurrentPosition()
{
    const int numAxis = m_pointDataList.length();
    for (quint16 axis = 0; axis < numAxis; axis++) {
        int position = m_modbus->getLongReceiveDataRG(CURRENT_POSITION_TOP_SR_ADDR + axis * 2);
        auto data = static_cast<ProgramMenu2_4_2Item *>(m_pointDataList.at(axis));
        if (m_isEndPoint) {
            data->setEndPosition(position);
        } else {
            data->setHalfwayPosition(position);
        }
    }
    return 0;
}

QString ProgramMenu2_4_2::formatPosition(int position)
{
    const int format = m_axisPointInfo->formatArbitraryPointPosition();
    return m_resource->formatData(position, format, 2, true);
}

QString ProgramMenu2_4_2::formatSpeed(int speed)
{
    const int format = m_axisPointInfo->formatArbitraryPointSpeed();
    return m_resource->formatData(speed, format, 0);
}

void ProgramMenu2_4_2::modbusSetPointData(int pointNo, bool endPoint)
{
    QList<int> writeData;
    for (QObject *data: m_pointDataList) {
        auto pointData = static_cast<ProgramMenu2_4_2Item *>(data);
        auto position = endPoint ? pointData->endPosition() : pointData->halfwayPosition();
        writeData.append((position & 0x0000FFFF));
        writeData.append((position & 0xFFFF0000) >> 16);
        writeData.append(m_speed);
    }
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, 0);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, m_axisPointInfo->pointDataSize(), writeData);
    m_communication->setOneShotModbusCommand(command);
}

void ProgramMenu2_4_2::modbusGetPointData(int pointNo)
{
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, 0);
    if (hrAddress >= m_axisPointInfo->positionTopHrAddr())
        m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, hrAddress, m_axisPointInfo->pointDataSize());
}

void ProgramMenu2_4_2::updatePointData(const stModBusCommunicationData &command)
{
    bool endPoint = false;
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(m_halfwayPointNo, 0);
    if (command.readAddress != hrAddress) {
        hrAddress = m_axisPointInfo->getPointPositionAddress(m_endPointNo, 0);
        if (command.readAddress != hrAddress) {
            return;
        }
        endPoint = true;
    }

    const int numAxis = m_pointDataList.length();
    for (int i = 0; i < numAxis; i++) {
        auto data = static_cast<ProgramMenu2_4_2Item *>(m_pointDataList.at(i));
        if (endPoint) {
            data->setEndPosition(m_modbus->getLongReceiveDataHR(hrAddress));
        } else {
            data->setHalfwayPosition(m_modbus->getLongReceiveDataHR(hrAddress));
        }
        hrAddress += 2;
        if (i == 0) {
            setSpeed(m_modbus->getReceiveDataHR(hrAddress));
        }
        hrAddress++;
    }
    if (m_dataChanged && endPoint) {
        emit ncpCommandDataChanged();
        m_dataChanged = false;
    }
}

void ProgramMenu2_4_2::setHalfwayPointNo(int no)
{
    if (m_halfwayPointNo != no) {
        m_halfwayPointNo = no;
        emit halfwayPointNoChanged();
    }
}

void ProgramMenu2_4_2::setEndPointNo(int no)
{
    if (m_endPointNo != no) {
        m_endPointNo = no;
        emit endPointNoChanged();
    }
}

void ProgramMenu2_4_2::setIsEndPoint(bool isEndPoint)
{
    if (m_isEndPoint != isEndPoint) {
        m_isEndPoint = isEndPoint;
        emit isEndPointChanged();
    }
}

void ProgramMenu2_4_2::clearSelect()
{
    for (QObject *item : m_pointDataList)
        static_cast<ProgramMenu2_4_2Item *>(item)->setSelect(false);
}

void ProgramMenu2_4_2::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerTeachingProgramPageModel)
        return;

    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters)
        updatePointData(command);
}

void ProgramMenu2_4_2::resetPointData()
{
    for (QObject *item : m_pointDataList)
        static_cast<ProgramMenu2_4_2Item *>(item)->init();
}
