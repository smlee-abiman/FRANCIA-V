#include "accelerationviewmodel.h"

AccelerationTimerData::AccelerationTimerData(QObject *parent)
    : QObject(parent)
{
    m_timer = 0;
}

AccelerationTimerData::~AccelerationTimerData(){

}

void AccelerationTimerData::setTimer(uchar timer)
{
    if(m_timer != timer){
        m_timer = timer;

        emit timerChanged(m_timer);
    }
}

AccelerationData::AccelerationData(long accelerationAddress, QObject *parent)
    : QObject(parent)
{
    m_accelerationAddress = accelerationAddress;
    m_acceleration = 0;

    for(int i = 0; i < 26; i++){
        m_timerAry[i] = new AccelerationTimerData();
    }
}
AccelerationData::~AccelerationData(){

}

void AccelerationData::setAcceleration(short acceleration)
{
    if(m_acceleration != acceleration){
        m_acceleration = acceleration;

        emit accelerationChanged(m_acceleration);
        emit timerChanged(timer());
    }
}

AccelerationViewModel::AccelerationViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    if (m_modbusManager){
        //qDebug() << "IOMonitorViewModel:m_modbusManager is share";
    }

    m_allAcceleration = 10;
    m_interpolationAcceleration = 0;

    for(int i = 0; i < 8; i++){
        m_dataList[i] = new AccelerationData(1939+i);
        QQmlEngine::setObjectOwnership(m_dataList[i], QQmlEngine::CppOwnership);
    }
}

AccelerationViewModel::~AccelerationViewModel()
{

}

void AccelerationViewModel::onActivate()
{
    startMonitoring();

    stModBusCommunicationData readCommand[3];

    readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, 1938, 9);
    readCommand[1] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, 2531, 1);
    readCommand[2] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, 53502, 104);

    m_communication->setOneShotModbusCommand(readCommand[0]);
    m_communication->setOneShotModbusCommand(readCommand[1]);
    m_communication->setOneShotModbusCommand(readCommand[2]);
}

void AccelerationViewModel::onDeactivate()
{
    stopMonitoring();
}

int AccelerationViewModel::writeAcceleration(long hrAddr, short value)
{
    int ret = 0;
    if(0 <= value && value < 26){
        stModBusCommunicationData writeCommand[1];
        stModBusCommunicationData readCommand[2];
        QList<int> writeData;

        writeData.clear();
        int acceleration = 25-value;
        writeData.append(acceleration);
        writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddr, writeData.length(), writeData);
        readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, hrAddr, writeData.length());
        readCommand[1] = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, 53502, 104);

        m_communication->setOneShotModbusCommand(writeCommand[0]);
        m_communication->setOneShotModbusCommand(readCommand[0]);
        m_communication->setOneShotModbusCommand(readCommand[1]);
    } else {
        ret = -1;
    }
    return ret;
}

int AccelerationViewModel::writeAllAcceleration(short value)
{
    int ret = 0;
    if(10 <= value && value <= 100){
        stModBusCommunicationData writeCommand[1];
        stModBusCommunicationData readCommand[2];
        QList<int> writeData;

        writeData.clear();
        writeData.append(value);
        writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, 1938, writeData.length(), writeData);
        readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, 1938, 9);
        readCommand[1] = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, 53502, 104);

        m_communication->setOneShotModbusCommand(writeCommand[0]);
        m_communication->setOneShotModbusCommand(readCommand[0]);
        m_communication->setOneShotModbusCommand(readCommand[1]);
    } else {
        ret = -1;
    }
    return ret;
}

int AccelerationViewModel::writeInterpolationAcceleration(short value)
{
    int ret = 0;
    if(0 <= value && value < 26){
        stModBusCommunicationData writeCommand[1];
        stModBusCommunicationData readCommand[2];
        QList<int> writeData;

        writeData.clear();
        writeData.append(value);
        writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, 2531, writeData.length(), writeData);
        readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, 2531, writeData.length());
        readCommand[1] = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, 53502, 104);

        m_communication->setOneShotModbusCommand(writeCommand[0]);
        m_communication->setOneShotModbusCommand(readCommand[0]);
        m_communication->setOneShotModbusCommand(readCommand[1]);
    } else {
        ret = -1;
    }
    return ret;
}

QObject* AccelerationViewModel::getData(int axisIndex)
{
    if(0 <= axisIndex && axisIndex < 8){
        return m_dataList[axisIndex];
    }
    else{
        qWarning("axisIndex is out of range. axisIndex:%d", axisIndex);
        return nullptr;
    }
}

void AccelerationViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &AccelerationViewModel::onFinished);
}
void AccelerationViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &AccelerationViewModel::onFinished);
}

void AccelerationViewModel::setAllAcceleration(short allAcceleration)
{
    if(m_allAcceleration != allAcceleration){
        m_allAcceleration = allAcceleration;

        emit allAccelerationChanged(m_allAcceleration);
    }
}

void AccelerationViewModel::setInterpolationAcceleration(short interpolationAcceleration)
{
    if(m_interpolationAcceleration != interpolationAcceleration){
        m_interpolationAcceleration = interpolationAcceleration;

        emit interpolationAccelerationChanged(m_interpolationAcceleration);
    }
}

void AccelerationViewModel::onFinished(){

    setAllAcceleration(m_modbusManager->getReceiveDataHR(1938));
    setInterpolationAcceleration(m_modbusManager->getReceiveDataHR(2531));

    for(int i = 0; i < 8; i++){
        int acceleration = 25-m_modbusManager->getReceiveDataHR(1939+i);
        m_dataList[i]->setAcceleration(acceleration);

        for(int j = 0; j < 26; j++){
            long index = 53598 + i - ((j / 2) * 8);
            ushort s_data = m_modbusManager->getReceiveDataHR(index);
            uchar c_high = (s_data >> 8) & 0x00FF;
            uchar c_low = s_data & 0x00FF;

            uchar c_data = (j%2 == 0 ? c_low : c_high);

            m_dataList[i]->getTimer(j)->setTimer(c_data);
        }
    }

    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

