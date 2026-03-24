#include "programmenu2_2.h"
#include "teachingview/teachingprogrampagemodel.h"
#include "programmenu2_1item.h"
#include "../command/ncpcommandmove.h"
#include "../command/ncpcommandmovep.h"
#include "axispointinfo.h"

const quint16 CURRENT_POSITION_TOP_SR_ADDR = 2769;
const int PROGMENU2_PEN_SET_POINT_ADDRESS_TOP_ROW = 190;
const int PROGMENU2_PEN_SET_FORMAT_TOP_ROW = 278;

PointLabelInfo::PointLabelInfo(QObject *parent)
    : QObject(parent)
{

}

void PointLabelInfo::setLabelIndex(int index)
{
    if (m_labelIndex != index) {
        m_labelIndex = index;
        emit labelIndexChanged();
    }
}

void PointLabelInfo::setPointNo(int no)
{
    if (m_pointNo != no) {
        m_pointNo = no;
        emit pointNoChanged();
    }
}


ProgramMenu2_2::ProgramMenu2_2(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    init();
}

ProgramMenu2_2::~ProgramMenu2_2()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_2::onOneShotReadFinished);
}

void ProgramMenu2_2::init()
{
    m_axisPointInfo = AxisPointInfo::get_instance();
    auto group = TeachingProgramPageModel::pensetGroup();

    for (int i = 0; i < Enums::AxisNum; i++) {
        auto item = new ProgramMenu2_1Item(this);
        m_pointDataList.append(item);
    }

    m_pointDispOrderSetting = m_resource->getIntValueList(group.toInt(), 179, 11);
    for(int i = 0; i < 11; i++ ){
        for(int j = 0; j < 8; j++){
            if(m_pointDispOrderSetting.at(i).value(j) != 0){
                PointLabelInfo *pointLabelInfo = new PointLabelInfo(this);
                m_pointList.append(pointLabelInfo);
                m_numPoint++;
            }
        }
    }
    readPenSetData();
    if (m_pointDataList.length() >= 1) {
        auto pointLabelInfo = static_cast<PointLabelInfo *>(m_pointList.at(0));
        m_currentPointNo = pointLabelInfo->pointNo();
    }
}

void ProgramMenu2_2::readPenSetPointListData()
{
    auto group = TeachingProgramPageModel::pensetGroup();
    auto numpoint = m_pointList.length();
    PointLabelInfo *pointLabelInfo;
    int row;
    int pointNo;
    for (int i = 0; i < numpoint; i++) {
        pointLabelInfo = static_cast<PointLabelInfo *>(m_pointList.at(i));
        pointNo = m_resource->getPenSetIntValue(group,179+i/8,i%8);
        if ((pointNo >= 201) && (pointNo <= 208)) {
            row = 29 + pointNo - 120;
        } else {
            row = 29 + pointNo;
        }
        pointLabelInfo->setLabelIndex(row);
        pointLabelInfo->setPointNo(pointNo);
    }
}

void ProgramMenu2_2::readPenSetData()
{
    readPenSetPointListData();
    auto group = TeachingProgramPageModel::pensetGroup();
    int key = 0;
    for (int i = 0; i < m_numPoint; i++) {
        key = m_resource->getPenSetIntValue(group, PROGMENU2_PEN_SET_POINT_ADDRESS_TOP_ROW + i, 16);
        m_pointAddressIndexPointNoConversionMap.insert(key, i);

        key = m_resource->getPenSetIntValue(group, PROGMENU2_PEN_SET_FORMAT_TOP_ROW + i, 16);
        m_formatIndexPointNoConversionMap.insert(key, i);
    }
}

void ProgramMenu2_2::onActivate()
{
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_2::onOneShotReadFinished, Qt::QueuedConnection);
    modbusGetPointData(m_currentPointNo);
}

void ProgramMenu2_2::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &ProgramMenu2_2::onOneShotReadFinished);
    resetPointData();
    setNoWaiting(false);
    setRelative(false);
    setCurrentPointLabelIndex(0);
}

void ProgramMenu2_2::show(NCPCommand *command)
{
    auto moveCommand = dynamic_cast<NCPCommandMove *>(command);
    if (setCurrentPointLabelIndexFromPointNo(moveCommand->pointNo()) == 0) {
        setCurrentPointNo(moveCommand->pointNo());
        setNoWaiting(command->type() == NCPCommand::Type::Movep);
        setRelative(moveCommand->relative());
        modbusGetPointData(m_currentPointNo);
        for (QObject *data: m_pointDataList) {
            auto pointData = static_cast<ProgramMenu2_1Item *>(data);
            pointData->setSelect(false);
        }
        for (int axis : moveCommand->axisList()) {
            auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
            data->setSelect(true);
        }
    }
}

NCPCommand *ProgramMenu2_2::createNcpCommand()
{
    bool invalid = true;
    NCPCommandMove *command = nullptr;
    if (m_noWaiting) {
        command = new NCPCommandMovep(m_resource, nullptr);
    } else {
        command = new NCPCommandMove(m_resource, nullptr);
    }

    if (command != nullptr) {
        modbusSetPointData(m_currentPointNo);
        command->setPointNo(m_currentPointNo);
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
        }
        modbusGetPointData(m_currentPointNo);
        m_dataChanged = true;
    }
    return command;
}

void ProgramMenu2_2::setCurrentPointLabelIndex(int index)
{
    if (m_currentPointLabelIndex != index) {
        m_currentPointLabelIndex = index;
        auto pointLabelInfo = static_cast<PointLabelInfo *>(m_pointList.at(m_currentPointLabelIndex));
        auto pointNo = pointLabelInfo->pointNo();
        if (m_currentPointNo != pointNo) {
            m_currentPointNo = pointNo;
            modbusGetPointData(pointNo);
        }
        emit currentPointLabelIndexChanged();
    }
}

int ProgramMenu2_2::setCurrentPointLabelIndexFromPointNo(int pointNo)
{
    int ret = -1;
    int len = m_pointList.length();
    for (int i = 0; i < len; i++) {
        auto pointLabelInfo = static_cast<PointLabelInfo *>(m_pointList.at(i));
        if (pointLabelInfo->pointNo() == pointNo) {
            ret = 0;
            m_currentPointLabelIndex = i;
            emit currentPointLabelIndexChanged();
            break;
        }
    }
    return ret;
}

int ProgramMenu2_2::setPosition(int axis, int position)
{
    auto poinAddressIndex = m_pointAddressIndexPointNoConversionMap.value(m_currentPointNo, -1);
    if (poinAddressIndex < 0) {
        return -1;
    }
    auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
    data->setPositon(position);
    return 0;
}

int ProgramMenu2_2::setSpeed(int axis, int speed)
{
    auto poinAddressIndex = m_pointAddressIndexPointNoConversionMap.value(m_currentPointNo, -1);
    if (poinAddressIndex < 0) {
        return -1;
    }
    auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
    data->setSpeed(speed);
    return 0;
}

int ProgramMenu2_2::setCurrentPosition()
{
    auto poinAddressIndex = m_pointAddressIndexPointNoConversionMap.value(m_currentPointNo, -1);
    if (poinAddressIndex < 0) {
        return -1;
    }
    const int numAxis = m_pointDataList.length();
    for (quint16 axis = 0; axis < numAxis; axis++) {
        int position = m_modbus->getLongReceiveDataRG(CURRENT_POSITION_TOP_SR_ADDR + axis * 2);
        auto data = static_cast<ProgramMenu2_1Item *>(m_pointDataList.at(axis));
        data->setPositon(position);
    }
    return 0;
}

QString ProgramMenu2_2::formatPosition(int axis, int position)
{
    auto formatIndex = m_formatIndexPointNoConversionMap.value(m_currentPointNo, -1);
    if (formatIndex < 0) {
        return "";
    }
    auto column = axis * 2;
    auto group = TeachingProgramPageModel::pensetGroup();
    auto format = m_resource->getPenSetIntValue(group, PROGMENU2_PEN_SET_FORMAT_TOP_ROW + formatIndex, column);
    return m_resource->formatData(position, format, 2, true);
}

QString ProgramMenu2_2::formatSpeed(int axis, int speed)
{
    auto formatIndex = m_formatIndexPointNoConversionMap.value(m_currentPointNo, -1);
    if (formatIndex < 0) {
        return "";
    }
    auto column = axis * 2 + 1;
    auto group = TeachingProgramPageModel::pensetGroup();
    auto format = m_resource->getPenSetIntValue(group, PROGMENU2_PEN_SET_FORMAT_TOP_ROW + formatIndex, column);
    return m_resource->formatData(speed, format, 0);
}

void ProgramMenu2_2::modbusSetPointData(int pointNo)
{
    auto poinAddressIndex = m_pointAddressIndexPointNoConversionMap.value(pointNo, -1);
    if (poinAddressIndex < 0) {
        return;
    }
    auto column = 0;
    auto group = TeachingProgramPageModel::pensetGroup();
    quint16 hrAddress = static_cast<quint16>(m_resource->getPenSetIntValue(group, PROGMENU2_PEN_SET_POINT_ADDRESS_TOP_ROW + poinAddressIndex, column));
    if (hrAddress >= m_axisPointInfo->positionTopHrAddr()) {
        QList<int> writeData;
        for (QObject * pointData : m_pointDataList) {
            auto point = static_cast<ProgramMenu2_1Item *>(pointData);
            auto position = point->position();
            auto speed = point->speed();
            writeData.append((position & 0x0000FFFF));
            writeData.append((position & 0xFFFF0000) >> 16);
            writeData.append(speed);
        }
        stModBusCommunicationData command = ModbusCommandGenerator::makeModbusWriteCommand(QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, m_axisPointInfo->pointDataSize(), writeData);
        m_communication->setOneShotModbusCommand(command);
    }
}

void ProgramMenu2_2::modbusGetPointData(int pointNo)
{
    auto poinAddressIndex = m_pointAddressIndexPointNoConversionMap.value(pointNo, -1);
    if (poinAddressIndex < 0) {
        return;
    }
    auto column = 0;
    auto group = TeachingProgramPageModel::pensetGroup();
    quint16 hrAddress = static_cast<quint16>(m_resource->getPenSetIntValue(group, PROGMENU2_PEN_SET_POINT_ADDRESS_TOP_ROW + poinAddressIndex, column));
    if (hrAddress >= m_axisPointInfo->positionTopHrAddr())
        m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, hrAddress, 24);
}

void ProgramMenu2_2::updatePointData(const stModBusCommunicationData &command)
{
    auto poinAddressIndex = m_pointAddressIndexPointNoConversionMap.value(m_currentPointNo, -1);
    if (poinAddressIndex < 0) {
        return;
    }
    quint16 hrAddress = static_cast<quint16>(m_resource->getPenSetIntValue(TeachingProgramPageModel::pensetGroup(), PROGMENU2_PEN_SET_POINT_ADDRESS_TOP_ROW + poinAddressIndex, 0));
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

void ProgramMenu2_2::setCurrentPointNo(int no)
{
    if (m_currentPointNo != no) {
        m_currentPointNo = no;
        emit currentPointNoChanged();
    }
}

void ProgramMenu2_2::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerTeachingProgramPageModel)
        return;

    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters)
        updatePointData(command);
}

void ProgramMenu2_2::setRelative(bool relative)
{
    if (m_relative != relative) {
        m_relative = relative;
        emit relativeChanged();
    }
}

void ProgramMenu2_2::setNoWaiting(bool noWaiting)
{
    if (m_noWaiting != noWaiting) {
        m_noWaiting = noWaiting;
        emit noWaitingChanged();
    }
}

void ProgramMenu2_2::resetPointData()
{
    for (QObject *data: m_pointDataList) {
        auto pointData = static_cast<ProgramMenu2_1Item *>(data);
        pointData->init();
    }
}
