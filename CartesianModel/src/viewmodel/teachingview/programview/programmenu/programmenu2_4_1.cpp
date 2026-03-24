#include "programmenu2_4_1.h"
#include "programmenu2_1item.h"
#include "../command/ncpcommandline.h"
#include "../command/ncpcommandlinec.h"
#include "axispointinfo.h"
#include "../ncpfile.h"
#include "../ncparbitrarypoint.h"

const quint16 CURRENT_POSITION_TOP_SR_ADDR = 2769;

ProgramMenu2_4_1::ProgramMenu2_4_1(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile)
    : ProgramMenu(resource,comm),
      m_ncpFile(ncpFile)
{
    init();
}

ProgramMenu2_4_1::~ProgramMenu2_4_1()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_4_1::onOneShotReadFinished);
}

void ProgramMenu2_4_1::setNcpFile(NCPFile *ncpFile)
{
    m_ncpFile = ncpFile;
}

void ProgramMenu2_4_1::init()
{
    m_axisPointInfo = AxisPointInfo::get_instance();

    for (int i = 0; i < Enums::AxisNum; i++) {
        auto item = new ProgramMenu2_1Item(this);
        m_pointDataList.append(item);
    }
}

void ProgramMenu2_4_1::onActivate()
{
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_4_1::onOneShotReadFinished, Qt::QueuedConnection);
    if (m_show) {
        return;
    }
    auto pointObj = m_ncpFile->findFreeArbitraryPoint();
    if (pointObj != nullptr) {
        auto pointNo = pointObj->no();
        setCurrentPointNo(pointNo);
    }
    modbusGetPointData(m_currentPointNo);
}

void ProgramMenu2_4_1::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_4_1::onOneShotReadFinished);
    resetPointData();
    setConsecutive(false);
    setSpeed(0);
    m_show = false;
}

void ProgramMenu2_4_1::setSpeed(int speed)
{
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged();
    }
}

void ProgramMenu2_4_1::setConsecutive(bool consecutive)
{
    if (m_consecutive != consecutive) {
        m_consecutive = consecutive;
        emit consecutiveChanged();
    }
}

void ProgramMenu2_4_1::show(NCPCommand *command)
{
    switch (command->type()) {
    case NCPCommand::Type::Line:
        setConsecutive(false);
        break;
    case NCPCommand::Type::Linec:
        setConsecutive(true);
        break;
    default:
        return;
    }

    m_show = true;
    clearSelect();

    auto lineCommand = static_cast<NCPCommandLine *>(command);
    auto pointNo = lineCommand->pointNo();
    setCurrentPointNo(pointNo);
    modbusGetPointData(pointNo);
    for (int axis : lineCommand->axisList()) {
        if ((axis >= 0) && (axis < m_pointDataList.length())) {
            auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
            data->setSelect(true);
        }
    }
}

NCPCommand *ProgramMenu2_4_1::createNcpCommand()
{
    bool invalid = true;
    NCPCommandLine *command = nullptr;
    if (m_consecutive) {
        command = new NCPCommandLinec(m_resource);
    } else {
        command = new NCPCommandLine(m_resource);
    }

    if (command != nullptr) {
        if (m_show) {
            auto pointObj = m_ncpFile->findFreeArbitraryPoint();
            if (pointObj != nullptr) {
                auto pointNo = pointObj->no();
                setCurrentPointNo(pointNo);
            }
        }
        command->setPointNo(m_currentPointNo);
        const int numAxis = m_pointDataList.length();
        for (int i = 0; i < numAxis; i++) {
            auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(i));
            if (data->select()) {
                command->appendAxis(i);
                invalid = false;
            }
        }
        if (invalid) {
            delete command;
            command = nullptr;
        } else {
            modbusSetPointData();
            modbusGetPointData(m_currentPointNo);
            m_ncpFile->assignPoint(m_currentPointNo);
            auto pointObj = m_ncpFile->findFreeArbitraryPoint();
            if (pointObj != nullptr) {
                auto pointNo = pointObj->no();
                setCurrentPointNo(pointNo);
                modbusGetPointData(pointNo);
            }
            m_dataChanged = true;
        }
    }
    m_show = false;
    return command;
}

int ProgramMenu2_4_1::setPosition(int axis, int position)
{
    auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
    data->setPositon(position);
    return 0;
}

int ProgramMenu2_4_1::setCurrentPosition()
{
    const int numAxis = m_pointDataList.length();
    for (quint16 axis = 0; axis < numAxis; axis++) {
        int position = m_modbus->getLongReceiveDataRG(CURRENT_POSITION_TOP_SR_ADDR + axis * 2);
        auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
        data->setPositon(position);
    }
    return 0;
}

QString ProgramMenu2_4_1::formatPosition(int position)
{
    const int format = m_axisPointInfo->formatArbitraryPointPosition();
    return m_resource->formatData(position, format, 2, true);
}

QString ProgramMenu2_4_1::formatSpeed(int speed)
{
    const int format = m_axisPointInfo->formatArbitraryPointSpeed();
    return m_resource->formatData(speed, format, 0);
}

void ProgramMenu2_4_1::modbusSetPointData()
{
    QList<int> writeData;
    for (QObject *data: m_pointDataList) {
        auto pointData = static_cast<ProgramMenu2_1Item *>(data);
        auto position = pointData->position();
        writeData.append((position & 0x0000FFFF));
        writeData.append((position & 0xFFFF0000) >> 16);
        writeData.append(m_speed);
    }
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(m_currentPointNo, 0);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, m_axisPointInfo->pointDataSize(), writeData);
    m_communication->setOneShotModbusCommand(command);
}

void ProgramMenu2_4_1::modbusGetPointData(int pointNo)
{
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, 0);
    if (hrAddress >= m_axisPointInfo->positionTopHrAddr())
        m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, hrAddress, m_axisPointInfo->pointDataSize());
}

void ProgramMenu2_4_1::updatePointData(const stModBusCommunicationData &command)
{
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(m_currentPointNo, 0);
    if (command.readAddress == hrAddress) {
        const int numAxis = m_pointDataList.length();
        for (int i = 0; i < numAxis; i++) {
            auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(i));
            data->setPositon(m_modbus->getLongReceiveDataHR(hrAddress));
            hrAddress += 2;
            if (i == 0) {
                setSpeed(m_modbus->getReceiveDataHR(hrAddress));
            }
            hrAddress++;
        }
        if (m_dataChanged) {
            emit ncpCommandDataChanged();
            m_dataChanged = false;
        }
    }
}

void ProgramMenu2_4_1::setCurrentPointNo(int no)
{
    if (m_currentPointNo != no) {
        m_currentPointNo = no;
        emit currentPointNoChanged();
    }
}

void ProgramMenu2_4_1::clearSelect()
{
    for (QObject *item : m_pointDataList)
        static_cast<ProgramMenu2_1Item *>(item)->setSelect(false);
}

void ProgramMenu2_4_1::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    Q_UNUSED(oneShotReadCaller)
    if (oneShotReadCaller != CommunicationThread::CallerTeachingProgramPageModel)
        return;

    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters)
        updatePointData(command);
}

void ProgramMenu2_4_1::resetPointData()
{
    for (QObject *item : m_pointDataList)
        static_cast<ProgramMenu2_1Item *>(item)->init();
}
