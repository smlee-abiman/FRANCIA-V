#include "mechanicalparametersviewmodel.h"

MechanicalParam::MechanicalParam(QObject *parent)
    : QObject(parent)
{
    m_rotationRange = 0;
    m_rotationPulse = 0;
    m_maxRotation = 0;
    m_frontEndRange = 0;
    m_originCorrection = 0;
    m_beforeOriginReturnSpeed = 0;
    m_freeOperationSpeedLow = 0;
    m_freeOperationSpeedMedium = 0;
    m_freeOperationSpeedHigh = 0;
    m_accelerationAndDecelerationTimeMin = 0;
    m_accelerationAndDecelerationTimeMax = 0;
    m_refuelingRange = 0;
    m_isChangeRotationRange = false;
    m_isChangeRotationPulse = false;
    m_isChangeMaxRotation = false;
    m_isChangeFrontEndRange = false;
    m_isChangeOriginCorrection = false;
    m_isChangeBeforeOriginReturnSpeed = false;
    m_isChangeFreeOperationSpeedLow = false;
    m_isChangeFreeOperationSpeedMedium = false;
    m_isChangeFreeOperationSpeedHigh = false;
    m_isChangeAccelerationAndDecelerationTimeMin = false;
    m_isChangeAccelerationAndDecelerationTimeMax = false;
    m_isChangeRefuelingRange = false;
}

MechanicalParam::~MechanicalParam(){

}

void MechanicalParam::setRotationRange(ushort rotationRange, bool firstDataLoaded)
{
    if(m_rotationRange != rotationRange){
        m_rotationRange = rotationRange;

        emit rotationRangeChanged(m_rotationRange);

        if(firstDataLoaded == true && m_isChangeRotationRange != true){
            m_isChangeRotationRange = true;
            emit isChangeRotationRangeChanged(m_isChangeRotationRange);
        }
    }
}

void MechanicalParam::setRotationPulse(ushort rotationPulse, bool firstDataLoaded)
{
    if(m_rotationPulse != rotationPulse){
        m_rotationPulse = rotationPulse;

        emit rotationPulseChanged(m_rotationPulse);

        if(firstDataLoaded == true && m_isChangeRotationPulse != true){
            m_isChangeRotationPulse = true;
            emit isChangeRotationPulseChanged(m_isChangeRotationPulse);
        }
    }
}

void MechanicalParam::setMaxRotation(ushort maxRotation, bool firstDataLoaded)
{
    if(m_maxRotation != maxRotation){
        m_maxRotation = maxRotation;

        emit maxRotationChanged(m_maxRotation);

        if(firstDataLoaded == true && m_isChangeMaxRotation != true){
            m_isChangeMaxRotation = true;
            emit isChangeMaxRotationChanged(m_isChangeMaxRotation);
        }
    }
}

void MechanicalParam::setfrontEndRange(ushort frontEndRange, bool firstDataLoaded)
{
    if(m_frontEndRange != frontEndRange){
        m_frontEndRange = frontEndRange;

        emit frontEndRangeChanged(m_frontEndRange);

        if(firstDataLoaded == true && m_isChangeFrontEndRange != true){
            m_isChangeFrontEndRange = true;
            emit isChangeFrontEndRangeChanged(m_isChangeFrontEndRange);
        }
    }
}

void MechanicalParam::setOriginCorrection(short originCorrection, bool firstDataLoaded)
{
    if(m_originCorrection != originCorrection){
        m_originCorrection = originCorrection;

        emit originCorrectionChanged(m_originCorrection);

        if(firstDataLoaded == true && m_isChangeOriginCorrection != true){
            m_isChangeOriginCorrection = true;
            emit isChangeOriginCorrectionChanged(m_isChangeOriginCorrection);
        }
    }
}

void MechanicalParam::setBeforeOriginReturnSpeed(uchar beforeOriginReturnSpeed, bool firstDataLoaded)
{
    if(m_beforeOriginReturnSpeed != beforeOriginReturnSpeed){
        m_beforeOriginReturnSpeed = beforeOriginReturnSpeed;

        emit beforeOriginReturnSpeedChanged(m_beforeOriginReturnSpeed);

        if(firstDataLoaded == true && m_isChangeBeforeOriginReturnSpeed != true){
            m_isChangeBeforeOriginReturnSpeed = true;
            emit isChangeBeforeOriginReturnSpeedChanged(m_isChangeBeforeOriginReturnSpeed);
        }
    }
}

void MechanicalParam::setFreeOperationSpeedLow(uchar freeOperationSpeedLow, bool firstDataLoaded)
{
    if(m_freeOperationSpeedLow != freeOperationSpeedLow){
        m_freeOperationSpeedLow = freeOperationSpeedLow;

        emit freeOperationSpeedLowChanged(m_freeOperationSpeedLow);

        if(firstDataLoaded == true && m_isChangeFreeOperationSpeedLow != true){
            m_isChangeFreeOperationSpeedLow = true;
            emit isChangeFreeOperationSpeedLowChanged(m_isChangeFreeOperationSpeedLow);
        }
    }
}

void MechanicalParam::setFreeOperationSpeedMedium(uchar freeOperationSpeedMedium, bool firstDataLoaded)
{
    if(m_freeOperationSpeedMedium != freeOperationSpeedMedium){
        m_freeOperationSpeedMedium = freeOperationSpeedMedium;

        emit freeOperationSpeedMediumChanged(m_freeOperationSpeedMedium);

        if(firstDataLoaded == true && m_isChangeFreeOperationSpeedMedium != true){
            m_isChangeFreeOperationSpeedMedium = true;
            emit isChangeFreeOperationSpeedMediumChanged(m_isChangeFreeOperationSpeedMedium);
        }
    }
}

void MechanicalParam::setFreeOperationSpeedHigh(uchar freeOperationSpeedHigh, bool firstDataLoaded)
{
    if(m_freeOperationSpeedHigh != freeOperationSpeedHigh){
        m_freeOperationSpeedHigh = freeOperationSpeedHigh;

        emit freeOperationSpeedHighChanged(m_freeOperationSpeedHigh);

        if(firstDataLoaded == true && m_isChangeFreeOperationSpeedHigh != true){
            m_isChangeFreeOperationSpeedHigh = true;
            emit isChangeFreeOperationSpeedHighChanged(m_isChangeFreeOperationSpeedHigh);
        }
    }
}

void MechanicalParam::setAccelerationAndDecelerationTimeMin(uchar accelerationAndDecelerationTimeMin, bool firstDataLoaded)
{
    if(m_accelerationAndDecelerationTimeMin != accelerationAndDecelerationTimeMin){
        m_accelerationAndDecelerationTimeMin = accelerationAndDecelerationTimeMin;

        emit accelerationAndDecelerationTimeMinChanged(m_accelerationAndDecelerationTimeMin);

        if(firstDataLoaded == true && m_isChangeAccelerationAndDecelerationTimeMin != true){
            m_isChangeAccelerationAndDecelerationTimeMin = true;
            emit isChangeAccelerationAndDecelerationTimeMinChanged(m_isChangeAccelerationAndDecelerationTimeMin);
        }
    }
}

void MechanicalParam::setAccelerationAndDecelerationTimeMax(uchar accelerationAndDecelerationTimeMax, bool firstDataLoaded)
{
    if(m_accelerationAndDecelerationTimeMax != accelerationAndDecelerationTimeMax){
        m_accelerationAndDecelerationTimeMax = accelerationAndDecelerationTimeMax;

        emit accelerationAndDecelerationTimeMaxChanged(m_accelerationAndDecelerationTimeMax);

        if(firstDataLoaded == true && m_isChangeAccelerationAndDecelerationTimeMax != true){
            m_isChangeAccelerationAndDecelerationTimeMax = true;
            emit isChangeAccelerationAndDecelerationTimeMaxChanged(m_isChangeAccelerationAndDecelerationTimeMax);
        }
    }
}

void MechanicalParam::setRefuelingRange(ulong refuelingRange, bool firstDataLoaded)
{
    if(m_refuelingRange != refuelingRange){
        m_refuelingRange = refuelingRange;

        emit refuelingRangeChanged(m_refuelingRange);

        if(firstDataLoaded == true && m_isChangeRefuelingRange != true){
            m_isChangeRefuelingRange = true;
            emit isChangeRefuelingRangeChanged(m_isChangeRefuelingRange);
        }
    }
}

void MechanicalParam::resetIsChange()
{
    m_isChangeRotationRange = false;
    m_isChangeRotationPulse = false;
    m_isChangeMaxRotation = false;
    m_isChangeFrontEndRange = false;
    m_isChangeOriginCorrection = false;
    m_isChangeBeforeOriginReturnSpeed = false;
    m_isChangeFreeOperationSpeedLow = false;
    m_isChangeFreeOperationSpeedMedium = false;
    m_isChangeFreeOperationSpeedHigh = false;
    m_isChangeAccelerationAndDecelerationTimeMin = false;
    m_isChangeAccelerationAndDecelerationTimeMax = false;
    m_isChangeRefuelingRange = false;

    emit isChangeRotationRangeChanged(m_isChangeRotationRange);
    emit isChangeRotationPulseChanged(m_isChangeRotationPulse);
    emit isChangeMaxRotationChanged(m_isChangeMaxRotation);
    emit isChangeFrontEndRangeChanged(m_isChangeFrontEndRange);
    emit isChangeOriginCorrectionChanged(m_isChangeOriginCorrection);
    emit isChangeBeforeOriginReturnSpeedChanged(m_isChangeBeforeOriginReturnSpeed);
    emit isChangeFreeOperationSpeedLowChanged(m_isChangeFreeOperationSpeedLow);
    emit isChangeFreeOperationSpeedMediumChanged(m_isChangeFreeOperationSpeedMedium);
    emit isChangeFreeOperationSpeedHighChanged(m_isChangeFreeOperationSpeedHigh);
    emit isChangeAccelerationAndDecelerationTimeMinChanged(m_isChangeAccelerationAndDecelerationTimeMin);
    emit isChangeAccelerationAndDecelerationTimeMaxChanged(m_isChangeAccelerationAndDecelerationTimeMax);
    emit isChangeRefuelingRangeChanged(m_isChangeRefuelingRange);
}

MechanicalParametersViewModel::MechanicalParametersViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    if (m_modbusManager){
        //qDebug() << "IOMonitorViewModel:m_modbusManager is share";
    }

    m_commonView = commonView;

    pageCommandInit();

    m_pageNum = 0;
    m_pageMaxNum = 2;
    m_axisPageNum = 0;
    m_axisPageMaxNum = 2;

    m_saveStatus = 0;
    m_modelName = "";

    m_firstDataLoadedFlag = false;

    for(int i = 0; i < 8; i++){
        m_dataList[i] = new MechanicalParam();
        QQmlEngine::setObjectOwnership(m_dataList[i], QQmlEngine::CppOwnership);
    }
}

MechanicalParametersViewModel::~MechanicalParametersViewModel()
{

}

void MechanicalParametersViewModel::onActivate()
{
    startMonitoring();
    readParameter();
}

void MechanicalParametersViewModel::onDeactivate()
{
    stopMonitoring();
}

void MechanicalParametersViewModel::pageCommandInit()
{

}

void MechanicalParametersViewModel::readParameter()
{
    stModBusCommunicationData readCommand[5];

    readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, MC_MPVM_DATA1_HR, MC_MPVM_DATA_SIZE);
    readCommand[1] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, MC_MPVM_DATA2_HR, MC_MPVM_DATA_SIZE);
    readCommand[2] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, MC_MPVM_DATA3_HR, MC_MPVM_DATA_SIZE);
    readCommand[3] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, MC_MPVM_DATA4_HR, MC_MPVM_DATA_SIZE);
    readCommand[4] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, MC_MPVM_DATA5_HR, MC_MPVM_DATA_SIZE);

    m_communication->setOneShotModbusCommand(readCommand[0]);
    m_communication->setOneShotModbusCommand(readCommand[1]);
    m_communication->setOneShotModbusCommand(readCommand[2]);
    m_communication->setOneShotModbusCommand(readCommand[3]);
    m_communication->setOneShotModbusCommand(readCommand[4]);
}

int MechanicalParametersViewModel::getIndex(int axisNum, int dataNum)
{
    int index = 0;

    switch(dataNum){
    case 0:
        index = MC_MPVM_ROTETION_RANGE_TOP_HR + axisNum;
        break;
    case 1:
        index = MC_MPVM_ROTETION_PULSE_TOP_HR + axisNum;
        break;
    case 2:
        index = MC_MPVM_MAX_ROTETION_TOP_HR + axisNum;
        break;
    case 3:
        index = MC_MPVM_FRONT_END_RANGE_TOP_HR + axisNum;
        break;
    case 4:
        index = MC_MPVM_ORIGIN_CORRECTION_TOP_HR + axisNum;
        break;
    case 5:
        index = MC_MPVM_BEFORE_ORIGIN_RETURN_SPEED_TOP_HR + axisNum;
        break;
    case 6:
        index = MC_MPVM_OPERATION_SPEED_L_TOP_HR + axisNum;
        break;
    case 7:
        index = MC_MPVM_OPERATION_SPEED_M_TOP_HR + axisNum;
        break;
    case 8:
        index = MC_MPVM_OPERATION_SPEED_H_TOP_HR + axisNum;
        break;
    case 9:
        index = MC_MPVM_ACCELERATION_AND_DECELERATION_TIME_MIN_HR + axisNum;
        break;
    case 10:
        index = MC_MPVM_ACCELERATION_AND_DECELERATION_TIME_MAX_HR + axisNum;
        break;
    case 11:
        index = MC_MPVM_REFUELING_RANGE_TOP_HR + (axisNum * 2);
        break;
    }

    return index;
}

void MechanicalParametersViewModel::setValue(int axisNum, int dataNum, double value)
{
    stModBusCommunicationData writeCommand[1];
    QList<int> writeData;
    writeData.clear();

    int index = getIndex(axisNum, dataNum);
    int format = m_resource->getPenSetIntValue("18", 41 + dataNum, 3);
    DataFormat dataFormat = DataFormat::fromCode(format);
    // 浮動小数点による値丸め対策
    double d_value = value * (0 < dataFormat.fraction ? qPow(10, dataFormat.fraction) : 1);
    long correct_value = lround(d_value);

    switch(dataNum){
    case 0:
    case 1:
    case 2:
    case 3:
        // ushort
        {
            ushort s_value = correct_value & 0x0000FFFF;
            writeData.append((int)s_value);
        }
        break;
    case 4:
        // short
        {
            short s_value = (short)correct_value;
            writeData.append((int)s_value);
        }
        break;
    case 5:
    case 6:
    case 8:
    case 9:
        // byte(上位ビット）
        {
            short s_value = correct_value & 0x000000FF;
            short beforeData = m_modbusManager->getReceiveDataHR(index);
            short newData = (s_value << 8) + (beforeData & 0x000000FF);
            writeData.append((int)newData);
        }
        break;
    case 7:
    case 10:
        // byte(下位ビット）
    {
        short s_value = correct_value & 0x000000FF;
        short beforeData = m_modbusManager->getReceiveDataHR(index);
        short newData = (beforeData & 0x0000FF00) + s_value;
        writeData.append((int)newData);
    }
        break;
    case 11:
        // long
        ushort s_high_value = (correct_value >> 16) & 0x0000FFFF;
        ushort s_low_value = correct_value & 0x0000FFFF;
        writeData.append((int)s_low_value);
        writeData.append((int)s_high_value);
        break;
    }


    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, index, writeData.length(), writeData);

    m_communication->setOneShotModbusCommand(writeCommand[0]);
    m_operationRecord->recordSetMachineParam(dataNum, axisNum, getValue(axisNum, dataNum), correct_value);
    readParameter();
}

int MechanicalParametersViewModel::getValue(int axisNum, int dataNum)
{
    int ret = 0;
    auto mechanicalParam = m_dataList[axisNum];
    switch(dataNum){
    case 0:
        ret = mechanicalParam->rotationRange();
        break;
    case 1:
        ret = mechanicalParam->rotationPulse();
        break;
    case 2:
        ret = mechanicalParam->maxRotation();
        break;
    case 3:
        ret = mechanicalParam->frontEndRange();
        break;
    case 4:
        ret = mechanicalParam->originCorrection();
        break;
    case 5:
        ret = mechanicalParam->beforeOriginReturnSpeed();
        break;
    case 6:
        ret = mechanicalParam->freeOperationSpeedLow();
        break;
    case 7:
        ret = mechanicalParam->freeOperationSpeedMedium();
        break;
    case 8:
        ret = mechanicalParam->freeOperationSpeedHigh();
        break;
    case 9:
        ret = mechanicalParam->accelerationAndDecelerationTimeMin();
        break;
    case 10:
        ret = mechanicalParam->accelerationAndDecelerationTimeMax();
        break;
    case 11:
        ret = mechanicalParam->refuelingRange();
        break;
    }
    return ret;
}

void MechanicalParametersViewModel::initParameter()
{
    stModBusCommunicationData writeCommand[1];
    QList<int> writeData;

    setSaveStatus(1);

    writeData.clear();
    writeData.append(28);
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, MC_MPVM_SAVE_HR, 1, writeData);

    m_communication->setOneShotModbusCommand(writeCommand[0]);

    connect(m_communication, &CommunicationThread::finish, this, &MechanicalParametersViewModel::onDataInitFinished);
}

void MechanicalParametersViewModel::saveParameter()
{
    stModBusCommunicationData writeCommand[1];
    QList<int> writeData;

    setSaveStatus(1);

    writeData.clear();
    writeData.append(31);
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, MC_MPVM_SAVE_HR, 1, writeData);

    m_communication->setOneShotModbusCommand(writeCommand[0]);
    m_operationRecord->recordSaveMachineParam();
    connect(m_communication, &CommunicationThread::finish, this, &MechanicalParametersViewModel::onDataSaveFinished);
}

QList<QObject*> MechanicalParametersViewModel::getDataList(int axisPage)
{
    QList<QObject*> list;

    QList<int> axisOrder = m_commonView->getAxisOrder();

    if(axisPage == 0){
        for(int i = 0; i < 5; i++){
            if(i < axisOrder.length()){
                list.append(m_dataList[axisOrder[i]]);
            }
        }
    }
    else{
        for(int i = 0; i < 3; i++){
            if(5+i < axisOrder.length()){
                list.append(m_dataList[axisOrder[5+i]]);
            }
        }
    }

    return list;
}

void MechanicalParametersViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &MechanicalParametersViewModel::onFinished);
}
void MechanicalParametersViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &MechanicalParametersViewModel::onFinished);
}

void MechanicalParametersViewModel::pageInit(){
    setPageNum(0);
    setAxisPageNum(0);
}

void MechanicalParametersViewModel::setPageNum(int pageNum)
{
    m_pageNum = pageNum;

    emit pageNumChanged(m_pageNum);
}

void MechanicalParametersViewModel::setAxisPageNum(int axisPageNum)
{
    m_axisPageNum = axisPageNum;

    emit axisPageNumChanged(m_axisPageNum);
}

void MechanicalParametersViewModel::setSaveStatus(int saveStatus)
{
    m_saveStatus = saveStatus;

    emit saveStatusChanged(m_saveStatus);
}

void MechanicalParametersViewModel::setModelName(QString modelName)
{
    m_modelName = modelName;

    emit modelNameChanged(m_modelName);
}

void MechanicalParametersViewModel::onFinished(){

    QByteArray modelNameByteAry;
    QString modelName;
    for(int idx = 0; idx < 8; idx++){
        short s = m_modbusManager->getReceiveDataRG(MC_MPVM_MODEL_SR_TOP + idx);
        char c_high = (s >> 8) & 0xFF;
        char c_low = s & 0xFF;

        modelNameByteAry.append(c_high);
        modelNameByteAry.append(c_low);
    }

    modelName = QString(modelNameByteAry);
    setModelName(modelName);
    onReadParameterFinished();

    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void MechanicalParametersViewModel::onReadParameterFinished(){

    for(int i = 0; i < 8; i++){
        ushort rotationRange = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_ROTETION_RANGE_TOP_HR + i);
        ushort rotationPulse = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_ROTETION_PULSE_TOP_HR + i);
        ushort maxRotation = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_MAX_ROTETION_TOP_HR + i);
        ushort frontEndRange = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_FRONT_END_RANGE_TOP_HR + i);
        short originCorrection = (short)m_modbusManager->getReceiveDataHR(MC_MPVM_ORIGIN_CORRECTION_TOP_HR + i);
        ushort beforeOriginReturnSpeedShort = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_BEFORE_ORIGIN_RETURN_SPEED_TOP_HR + i);
        ushort freeOperationSpeedLowShort = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_OPERATION_SPEED_L_TOP_HR + i);
        ushort freeOperationSpeedMediumShort = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_OPERATION_SPEED_M_TOP_HR + i);
        ushort freeOperationSpeedHighShort = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_OPERATION_SPEED_H_TOP_HR + i);
        ushort accelerationAndDecelerationTimeMinShort = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_ACCELERATION_AND_DECELERATION_TIME_MIN_HR + i);
        ushort accelerationAndDecelerationTimeMaxShort = (ushort)m_modbusManager->getReceiveDataHR(MC_MPVM_ACCELERATION_AND_DECELERATION_TIME_MAX_HR + i);
        ulong refuelingRange = (ulong)m_modbusManager->getLongReceiveDataHR(MC_MPVM_REFUELING_RANGE_TOP_HR  + (i * 2));

        uchar beforeOriginReturnSpeed = (uchar)((beforeOriginReturnSpeedShort >> 8) & 0x00FF);
        uchar freeOperationSpeedLow = (uchar)((freeOperationSpeedLowShort >> 8) & 0x00FF);
        uchar freeOperationSpeedMedium = (uchar)(freeOperationSpeedMediumShort & 0x00FF);
        uchar freeOperationSpeedHigh = (uchar)((freeOperationSpeedHighShort >> 8) & 0x00FF);
        uchar accelerationAndDecelerationTimeMin = (uchar)((accelerationAndDecelerationTimeMinShort >> 8) & 0x00FF);
        uchar accelerationAndDecelerationTimeMax = (uchar)(accelerationAndDecelerationTimeMaxShort & 0x00FF);

        m_dataList[i]->setRotationRange(rotationRange, m_firstDataLoadedFlag);
        m_dataList[i]->setRotationPulse(rotationPulse, m_firstDataLoadedFlag);
        m_dataList[i]->setMaxRotation(maxRotation, m_firstDataLoadedFlag);
        m_dataList[i]->setfrontEndRange(frontEndRange, m_firstDataLoadedFlag);
        m_dataList[i]->setOriginCorrection(originCorrection, m_firstDataLoadedFlag);
        m_dataList[i]->setBeforeOriginReturnSpeed(beforeOriginReturnSpeed, m_firstDataLoadedFlag);
        m_dataList[i]->setFreeOperationSpeedLow(freeOperationSpeedLow, m_firstDataLoadedFlag);
        m_dataList[i]->setFreeOperationSpeedMedium(freeOperationSpeedMedium, m_firstDataLoadedFlag);
        m_dataList[i]->setFreeOperationSpeedHigh(freeOperationSpeedHigh, m_firstDataLoadedFlag);
        m_dataList[i]->setAccelerationAndDecelerationTimeMin(accelerationAndDecelerationTimeMin, m_firstDataLoadedFlag);
        m_dataList[i]->setAccelerationAndDecelerationTimeMax(accelerationAndDecelerationTimeMax, m_firstDataLoadedFlag);
        m_dataList[i]->setRefuelingRange(refuelingRange, m_firstDataLoadedFlag);
    }

    m_firstDataLoadedFlag = true;
}

void MechanicalParametersViewModel::onDataInitFinished(){

    disconnect(m_communication, &CommunicationThread::finish, this, &MechanicalParametersViewModel::onDataInitFinished);

    setSaveStatus(2);

    readParameter();
}

void MechanicalParametersViewModel::onDataSaveFinished(){

    disconnect(m_communication, &CommunicationThread::finish, this, &MechanicalParametersViewModel::onDataSaveFinished);

    setSaveStatus(2);

    for(int i = 0; i < 8; i++){
        m_dataList[i]->resetIsChange();
    }

    readParameter();
}
