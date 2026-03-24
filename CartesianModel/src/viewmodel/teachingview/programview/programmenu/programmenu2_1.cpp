#include "programmenu2_1.h"
#include "programmenu2_1item.h"
#include "../command/ncpcommandmove.h"
#include "../command/ncpcommandmovep.h"
#include "../../teachingprogrampagemodel.h"
#include "axispointinfo.h"
#include "../ncparbitrarypoint.h"

const quint16 CURRENT_POSITION_TOP_SR_ADDR = 2769;

ProgramMenu2_1::ProgramMenu2_1(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile)
    : ProgramMenu(resource,comm),
      m_ncpFile(ncpFile)
{
    init();
}

ProgramMenu2_1::~ProgramMenu2_1()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_1::onOneShotReadFinished);
}

void ProgramMenu2_1::setNcpFile(NCPFile *ncpFile)
{
    m_ncpFile = ncpFile;
}

void ProgramMenu2_1::init()
{
    m_axisPointInfo = AxisPointInfo::get_instance();

    for (int i = 0; i < Enums::AxisNum; i++) {
        auto item = new ProgramMenu2_1Item(this);
        m_pointDataList.append(item);
    }
}

void ProgramMenu2_1::onActivate()
{
    if (m_currentPointNo == 0) {
        auto pointNo = findFreeArbitraryPoint();
        if (pointNo == -1) {
            qWarning() << __func__ << "There were no available points.";
        } else {
            setCurrentPointNo(pointNo);
        }
    }
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_1::onOneShotReadFinished, Qt::QueuedConnection);
    modbusGetPointData(m_currentPointNo);
}

void ProgramMenu2_1::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_1::onOneShotReadFinished);
    resetPointData();
    setNoWaiting(false);
    setRelative(false);
    setCurrentPointNo(0);
}

void ProgramMenu2_1::show(NCPCommand *command)
{
    resetPointData();
    auto moveCommand = dynamic_cast<NCPCommandMove *>(command);
    setCurrentPointNo(moveCommand->pointNo());
    setNoWaiting(command->type() == NCPCommand::Type::Movep);
    setRelative(moveCommand->relative());
    modbusGetPointData(m_currentPointNo);
    for (int axis : moveCommand->axisList()) {
        if ((axis >= 0) && (axis < m_pointDataList.length())) {
            auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
            data->setSelect(true);
        }
    }
}

int ProgramMenu2_1::insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    Q_UNUSED(file)
    auto pointNo = findFreeArbitraryPoint();
    if (pointNo == -1) {
        qWarning() << __func__ << "There were no available points.";
        return -1;
    }
    auto command = createMoveCommand(pointNo);
    if (command == nullptr) {
        qWarning() << __func__ << "Command could not be generated.";
        return -1;
    }

    int ret = insertCommand(program, lineIndex, command);
    if (ret != 0) {
        qWarning() << __func__ << "Error insertCommand." << ret;
        delete command;
        command = nullptr;
        return ret;
    }
    m_ncpFile->assignPoint(pointNo);
    pointNo = findFreeArbitraryPoint();
    if (pointNo != -1) {
        setCurrentPointNo(pointNo);
        modbusGetPointData(pointNo);
    }
    return ret;
}

int ProgramMenu2_1::replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex)
{
    Q_UNUSED(file)
    bool overWrite = false;
    auto pointNo = getNcpCommandPointNo(program, lineIndex);
    if (m_axisPointInfo->getPointType(pointNo) == AxisPointInfo::PointType::Arbitrary) {
        setCurrentPointNo(pointNo);
        overWrite = true;
    } else {
        pointNo = findFreeArbitraryPoint();
        if (pointNo == -1) {
            qWarning() << __func__ << "There were no available points. 1";
            return -1;
        }
    }
    auto command = createMoveCommand(pointNo);
    if (command == nullptr) {
        qWarning() << __func__ << "Command could not be generated.";
        return -1;
    }
    program->replaceCommand(lineIndex, command);
    m_ncpFile->assignPoint(pointNo);

    if (!overWrite) {
        pointNo = findFreeArbitraryPoint();
        if (pointNo == -1) {
            qWarning() << __func__ << "There were no available points. 2";
        } else {
            setCurrentPointNo(pointNo);
            modbusGetPointData(pointNo);
        }
    }
    return 0;
}

int ProgramMenu2_1::getNcpCommandPointNo(NCPProgram *program, int lineIndex)
{
    auto pointNo = -1;
    auto step = program->step(lineIndex);
    auto command = step->command();
    auto commandType = command->type();
    if ((commandType == NCPCommand::Type::Move) ||
        (commandType == NCPCommand::Type::Movep)) {
        auto moveCommnad = dynamic_cast<NCPCommandMove *>(command);
        pointNo = moveCommnad->pointNo();
    }
    return pointNo;
}


int ProgramMenu2_1::insertCommand(NCPProgram *program, int lineIndex, NCPCommand *command)
{
    auto step = new NCPStep(command);
    if (step == nullptr) {
        qWarning() << __func__ << "Step could not be generated.";
        return -1;
    }
    program->insertStep(lineIndex, step);
    return 0;
}

NCPCommand *ProgramMenu2_1::createMoveCommand(int pointNo)
{
    bool invalid = true;
    NCPCommandMove *command = nullptr;
    if (m_noWaiting) {
        command = new NCPCommandMovep(m_resource, nullptr);
    } else {
        command = new NCPCommandMove(m_resource, nullptr);
    }

    if (command != nullptr) {
        command->setPointNo(pointNo);
        command->setRelative(m_relative);
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
            modbusSetPointData(pointNo);
            modbusGetPointData(pointNo);
            m_dataChanged = true;
        }
    }
    return command;
}

int ProgramMenu2_1::setPosition(int axis, int position)
{
    auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
    data->setPositon(position);
    return 0;
}

int ProgramMenu2_1::setSpeed(int axis, int speed)
{
    auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
    data->setSpeed(speed);
    return 0;
}

int ProgramMenu2_1::setCurrentPosition()
{
    const int numAxis = m_pointDataList.length();
    for (quint16 axis = 0; axis < numAxis; axis++) {
        int position = m_modbus->getLongReceiveDataRG(CURRENT_POSITION_TOP_SR_ADDR + axis * 2);
        auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
        data->setPositon(position);
    }
    return 0;
}

QString ProgramMenu2_1::formatPosition(int position)
{
    const int format = m_axisPointInfo->formatArbitraryPointPosition();
    return m_resource->formatData(position, format, 2, true);
}

QString ProgramMenu2_1::formatSpeed(int speed)
{
    const int format = m_axisPointInfo->formatArbitraryPointSpeed();
    return m_resource->formatData(speed, format, 0);
}

void ProgramMenu2_1::modbusSetPointData(int pointNo)
{
    QList<int> writeData;
    for (QObject *data: m_pointDataList) {
        auto pointData = static_cast<ProgramMenu2_1Item *>(data);
        auto position = pointData->position();
        writeData.append((position & 0x0000FFFF));
        writeData.append((position & 0xFFFF0000) >> 16);
        writeData.append(pointData->speed());
    }
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, 0);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, m_axisPointInfo->pointDataSize(), writeData);
    m_communication->setOneShotModbusCommand(command);
}

void ProgramMenu2_1::modbusGetPointData(int pointNo)
{
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, 0);
    if (hrAddress >= m_axisPointInfo->positionTopHrAddr())
        m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, hrAddress, m_axisPointInfo->pointDataSize());
}

void ProgramMenu2_1::updatePointData(const stModBusCommunicationData &command)
{
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(m_currentPointNo, 0);
    if (command.readAddress == hrAddress) {
        const int numAxis = m_pointDataList.length();
        for (int i = 0; i < numAxis; i++) {
            auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(i));
            data->setPositon(m_modbus->getLongReceiveDataHR(hrAddress));
            hrAddress += 2;
            data->setSpeed(m_modbus->getReceiveDataHR(hrAddress));
            hrAddress++;
        }
        if (m_dataChanged) {
            emit ncpCommandDataChanged();
            m_dataChanged = false;
        }
    }
}

void ProgramMenu2_1::resetPointData()
{
    for (QObject *data: m_pointDataList) {
        auto pointData = static_cast<ProgramMenu2_1Item *>(data);
        pointData->init();
    }
}

void ProgramMenu2_1::setCurrentPointNo(int no)
{
    if (m_currentPointNo != no) {
        m_currentPointNo = no;
        emit currentPointNoChanged();
    }
}

void ProgramMenu2_1::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    Q_UNUSED(oneShotReadCaller)
    if (oneShotReadCaller != CommunicationThread::CallerTeachingProgramPageModel)
        return;

    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters)
        updatePointData(command);
}

void ProgramMenu2_1::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerTeachingProgramPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void ProgramMenu2_1::setRelative(bool relative)
{
    if (m_relative != relative) {
        m_relative = relative;
        emit relativeChanged();
    }
}

void ProgramMenu2_1::setNoWaiting(bool noWaiting)
{
    if (m_noWaiting != noWaiting) {
        m_noWaiting = noWaiting;
        emit noWaitingChanged();
    }
}

int ProgramMenu2_1::findFreeArbitraryPoint()
{
    int pointNo = -1;
    auto pointObj = m_ncpFile->findFreeArbitraryPoint();
    if (pointObj != nullptr) {
        pointNo = pointObj->no();
    }
    return pointNo;
}
