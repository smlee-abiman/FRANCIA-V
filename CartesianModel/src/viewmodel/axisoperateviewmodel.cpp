#include "axisoperateviewmodel.h"
#include "commonviewmodel.h"

AxisOperateViewModel::AxisOperateViewModel(ResourceManager *resource, CommunicationThread * comm, AxisInfo *axisInfo, CommonViewModel *commonView)
    : ViewModelBase(resource, comm),
      m_axisInfo(axisInfo),
      m_commonView(commonView)
{
    init();
}

AxisOperateViewModel::AxisOperateViewModel(QObject *object)
    : ViewModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

AxisOperateViewModel::~AxisOperateViewModel()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &AxisOperateViewModel::onOneShotReadFinished);
    disconnect(m_communication, &CommunicationThread::finish, this, &AxisOperateViewModel::onFinished);
    disconnect(m_modbus, &ModbusManager::endModbusPageChange, this, &AxisOperateViewModel::endModbusPageChange);
    disconnect(m_communication->getPhyKeyManagerInstance(), SIGNAL(jogDialSpeedChanged(Enums::JogDialSpeed)), this, SLOT(onJogDialSpeedChanged(Enums::JogDialSpeed)));
}

void AxisOperateViewModel::updateMaxPosition()
{
    for (int i = 0; i < AXIS_ADDRESS_LIST.length(); i++) {
        AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
        QList<quint16> addressList = AXIS_ADDRESS_LIST.at(i);
        QString maxPosition = QString::number(m_modbus->getLongReceiveDataHR(addressList.at(AxisAddressType::MaxPosition)));
        m_axisInfo->setMaxPosition(axis, maxPosition);
    }
}

void AxisOperateViewModel::modbusGetMaxPosition()
{
    quint16 numAxis = static_cast<quint16>(m_axisInfo->numAxis());
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, AXIS_MAX_POSITION_TOP_HR, 6 * numAxis);
    setOneShotModbusCommand(command);
}

int AxisOperateViewModel::modBusServoControl(int axis, bool status)
{
    if (axis >= AXIS_ADDRESS_LIST.length()) {
        return -1;
    }

    quint16 doAddress = AXIS_ADDRESS_LIST.at(axis).at(AxisAddressType::ServoDO);
    QList<int> writeData;
    writeData.append(status ? 0 : 1);
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, doAddress, 0x0001, writeData);
    m_communication->setModbusWriteCommand(command);
    return 0;
}

void AxisOperateViewModel::viewPointControl(bool reverse)
{
    m_operationRecord->recordOpeManualViewPoint(m_viewPoint, reverse);
    QList<int> writeData;
    writeData.append(reverse ? 1 : 0);
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, VIEWPOINT_DO, 0x0001, writeData);
    setOneShotModbusCommand(command);
}

QString AxisOperateViewModel::formatcurrentPosition(int axis, int value)
{
    if ((axis < 0) || (axis >= m_axisDataFormatList.length())) {
        return "";
    }
    AxisDataFormat dataFormat = m_axisDataFormatList.at(axis);
    return m_resource->formatData(value, dataFormat.currentPosition, 2);
}

QString AxisOperateViewModel::formatMaxPosition(int axis, int value)
{
    if ((axis < 0) || (axis >= m_axisDataFormatList.length())) {
        return "";
    }
    AxisDataFormat dataFormat = m_axisDataFormatList.at(axis);
    return m_resource->formatData(value, dataFormat.maxPosition, 2, true);
}

QString AxisOperateViewModel::formatEncoder(int axis, int value)
{
    if ((axis < 0) || (axis >= m_axisDataFormatList.length())) {
        return "";
    }
    AxisDataFormat dataFormat = m_axisDataFormatList.at(axis);
    return m_resource->formatData(value, dataFormat.encoder);
}

int AxisOperateViewModel::modbusJogScrollKey(int keyNo)
{
    if (keyNo < 0)
        return -1;
    QList<int> writeData;
    for(int i = 0; i < MEMBLEM_SW_STATUS_BIT_NUM; i++)
        writeData.append((keyNo >> i) & 0x01);

    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, MEMBLEM_SW_STATUS_DO, MEMBLEM_SW_STATUS_BIT_NUM, writeData);
    m_communication->setJogModbusCommand(command);
    return 0;
}

void AxisOperateViewModel::setViewpoint(bool reverse)
{
    if (m_viewPoint != reverse) {
        m_viewPoint = reverse;
        emit viewpointChanged(m_viewPoint);
    }
}

void AxisOperateViewModel::onActivate()
{
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &AxisOperateViewModel::onOneShotReadFinished, Qt::QueuedConnection);
    connect(m_communication, &CommunicationThread::finish, this, &AxisOperateViewModel::onFinished, Qt::QueuedConnection);
    if (!m_modbusAxisStatusInitialized) {
        m_commonView->modbusAxisStatus(true, false);
        m_modbusAxisStatusInitialized = true;
    }
    modbusGetMaxPosition();
}

void AxisOperateViewModel::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &AxisOperateViewModel::onFinished);
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &AxisOperateViewModel::onOneShotReadFinished);
}

void AxisOperateViewModel::init()
{
    readDataFormatCsv();
    connect(m_modbus, &ModbusManager::endModbusPageChange, this, &AxisOperateViewModel::endModbusPageChange, Qt::DirectConnection);
    connect(m_communication->getPhyKeyManagerInstance(), SIGNAL(jogDialSpeedChanged(Enums::JogDialSpeed)), this, SLOT(onJogDialSpeedChanged(Enums::JogDialSpeed)));
}

void AxisOperateViewModel::readDataFormatCsv()
{
    readPenSetData();
}

void AxisOperateViewModel::readPenSetData()
{
    m_axisDataFormatList.clear();
    int end = PEN_SET_AXIS_OPERATE_DATA_FORMAT_TOP_ROW + m_axisInfo->numAxis();
    for (int row = PEN_SET_AXIS_OPERATE_DATA_FORMAT_TOP_ROW; row < end; row++) {
        AxisDataFormat data;
        data.currentPosition = m_resource->getPenSetIntValue(PEN_SET_GROUP_MANUAL_OPERATE, row, PEN_SET_COLUMN_DATA_FORMAT_CURRENT_POSITION);
        data.encoder = m_resource->getPenSetIntValue(PEN_SET_GROUP_MANUAL_OPERATE, row, PEN_SET_COLUMN_DATA_FORMAT_ENCODER);
        data.maxPosition = m_resource->getPenSetIntValue(PEN_SET_GROUP_MANUAL_OPERATE, row, PEN_SET_COLUMN_DATA_FORMAT_MAX_POSITION);
        m_axisDataFormatList.append(data);
    }
}

void AxisOperateViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerManualAxisOpeViewModel);
    m_communication->setOneShotModbusCommand(data);
}

void AxisOperateViewModel::onFinished()
{
    if (m_pageChanging == false && m_modbus->getIsPageChanging() == false) {
        for (int i = 0; i < AXIS_ADDRESS_LIST.length(); i++) {
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QList<quint16> addressList = AXIS_ADDRESS_LIST.at(i);

            bool home =  m_modbus->getReceiveDataDI(addressList.at(AxisAddressType::home));
            bool over =  m_modbus->getReceiveDataDI(addressList.at(AxisAddressType::over));
            bool brake =  m_modbus->getReceiveDataDI(addressList.at(AxisAddressType::brake));
            QString encoder = m_resource->formatData((short)m_modbus->getWordReceiveDataRG(addressList.at(AxisAddressType::encoder)),64);
            QString torque = m_resource->formatData((int)m_modbus->getWordReceiveDataRG(addressList.at(AxisAddressType::torque)), 1328, 2);
            auto servo = m_modbus->getReceiveDataDI(addressList.at(AxisAddressType::ServoDI)) == 0;
            m_axisInfo->setHome(axis,home);
            m_axisInfo->setOver(axis,over);
            m_axisInfo->setBrake(axis,brake);
            m_axisInfo->setEncoder(axis,encoder);
            m_axisInfo->setTorque(axis,torque);
            m_axisInfo->setServo(axis,servo);

            if(m_resource->axisorder().size() > 5) {
                for(int i = 5; i < 8; i++){
                    AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
                    QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
                    m_axisInfo->setPosition(axis,position);
                }
            }
        }
        setViewpoint(m_modbus->getReceiveDataRG(VIEWPOINT_SR) & VIEWPOINT_SR_BIT_MASK);
        setDataLoaded(true);
    }
}

void AxisOperateViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::OneShotReadModBusCaller::CallerManualAxisOpeViewModel)
        return;

    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if (command.readAddress == AXIS_MAX_POSITION_TOP_HR) {
            updateMaxPosition();
        }
    }
}

void AxisOperateViewModel::endModbusPageChange(int mainScreenId, int subScreenId)
{
    PhyKeyManager *phyKeyManager = m_communication->getPhyKeyManagerInstance();
    if (!phyKeyManager)
        return;

    if ((mainScreenId == 6) && (subScreenId == 2)) {
        m_pageChanging = false;
    }
}

void AxisOperateViewModel::onJogDialSpeedChanged(Enums::JogDialSpeed speed)
{
    switch (speed) {
    case Enums::JogDialSpeedSlow:
    case Enums::JogDialSpeedMiddle:
    case Enums::JogDialSpeedHigh:
    {
        int sp = 3 + speed;
        m_commonView->modbusSpeedControl(sp);
    }
        break;
    default:
        break;
    }
}

void AxisOperateViewModel::setJogDialMode(bool mode)
{
    if (m_jogDialMode != mode) {
        m_jogDialMode = mode;
        if (m_jogDialMode) {
            m_orgManualSpeed = m_commonView->manualSpeedStatus();
        } else {
            auto phyKeyManager = m_communication->getPhyKeyManagerInstance();
            if (phyKeyManager != nullptr)
                phyKeyManager->setJogDialSpeed(Enums::JogDialSpeedUnknown);
            m_commonView->modbusSpeedControl(m_orgManualSpeed);
        }
        emit jogDialModeChanged();
    }
}

void AxisOperateViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}
void AxisOperateViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}

