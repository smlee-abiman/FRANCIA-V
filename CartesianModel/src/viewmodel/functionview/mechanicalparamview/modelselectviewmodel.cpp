#include "modelselectviewmodel.h"

ModelAxisData::ModelAxisData(QObject *parent)
    : QObject(parent)
{
    m_makerName = "";
    m_capacity = 0;
    m_encoderType = 0;
    m_stroke = 0;
}

ModelAxisData::~ModelAxisData(){

}

void ModelAxisData::setMakerName(QString makerName)
{
    if(m_makerName != makerName){
        m_makerName = makerName;

        emit makerNameChanged(m_makerName);
    }
}

void ModelAxisData::setCapacity(int capacity)
{
    if(m_capacity != capacity){
        m_capacity = capacity;

        emit capacityChanged(m_capacity);
    }
}

void ModelAxisData::setEncoderType(int encoderType)
{
    if(m_encoderType != encoderType){
        m_encoderType = encoderType;

        emit encoderTypeChanged(m_encoderType);
    }
}

void ModelAxisData::setStroke(long stroke)
{
    if(m_stroke != stroke){
        m_stroke = stroke;

        emit strokeChanged(m_stroke);
    }
}

ModelSelectViewModel::ModelSelectViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    if (m_modbusManager){
        //qDebug() << "IOMonitorViewModel:m_modbusManager is share";
    }

    m_pageNum = 0;
    m_pageMaxNum = 2;

    m_modelName = "";
    m_selectedModelIdx = 0;

    m_saveStatus = 0;

    for(int i = 0; i < 8; i++){
        m_dataList[i] = new ModelAxisData();
    }

    stModBusCommunicationData readCommand[4];

    readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadInputRegisters, MC_MSVM_MODEL_SR, 125);
    readCommand[1] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadInputRegisters, MC_MSVM_MODEL_SR + (125 * 1), 125);
    readCommand[2] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadInputRegisters, MC_MSVM_MODEL_SR + (125 * 2), MC_MSVM_MODEL_SIZE - (125 * 2));
    readCommand[3] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadInputRegisters, MC_MSVM_MAKER_SR, MC_MSVM_MAKER_SIZE);

    m_communication->setDelayCommandList(Enums::Pages::FunctionMechanicalParamModelSelect, readCommand[0]);
    m_communication->setDelayCommandList(Enums::Pages::FunctionMechanicalParamModelSelect, readCommand[1]);
    m_communication->setDelayCommandList(Enums::Pages::FunctionMechanicalParamModelSelect, readCommand[2]);
    m_communication->setDelayCommandList(Enums::Pages::FunctionMechanicalParamModelSelect, readCommand[3]);
}

ModelSelectViewModel::~ModelSelectViewModel()
{
    m_communication->resetPageReadCommandList(Enums::Pages::FunctionMechanicalParamModelSelect);
}

void ModelSelectViewModel::onActivate()
{
    startMonitoring();
}

void ModelSelectViewModel::onDeactivate()
{
    stopMonitoring();
}

void ModelSelectViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_modbusManager, &ModbusManager::endModbusPageChange, this, &ModelSelectViewModel::endModbusPageChange, Qt::DirectConnection);
}
void ModelSelectViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_modbusManager, &ModbusManager::endModbusPageChange, this, &ModelSelectViewModel::endModbusPageChange);
    disconnect(m_communication, &CommunicationThread::finish, this, &ModelSelectViewModel::onModelListFinished);
    disconnect(m_communication, &CommunicationThread::finish, this, &ModelSelectViewModel::onFinished);
    disconnect(m_communication, &CommunicationThread::finish, this, &ModelSelectViewModel::onModelChangeFinished);
}

void ModelSelectViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void ModelSelectViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;

        emit dataLoadedChanged(m_dataLoaded);
    }
}

void ModelSelectViewModel::pageInit(){
    setPageNum(1);
}

void ModelSelectViewModel::saveModelType(int modelIdx){
    stModBusCommunicationData writeCommand[2];
    QList<int> writeData;

    setSaveStatus(1);

    writeData.clear();
    writeData.append(modelIdx);
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, MC_MSVM_MODEL_SET1_HR, 1, writeData);
    writeData.clear();
    writeData.append(33);
    writeCommand[1] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, MC_MSVM_MODEL_SET2_HR, 1, writeData);

    m_communication->setModbusWriteCommand(writeCommand[0]);
    m_communication->setModbusWriteCommand(writeCommand[1]);
    m_operationRecord->recordSetModelType(m_selectedModelIdx, modelIdx);

    connect(m_communication, &CommunicationThread::finish, this, &ModelSelectViewModel::onModelChangeFinished);
}

void ModelSelectViewModel::setPageNum(int pageNum)
{
    if(m_pageNum != pageNum){
        m_pageNum = pageNum;

        emit pageNumChanged(m_pageNum);
    }
}

void ModelSelectViewModel::setSaveStatus(int saveStatus)
{
    if(m_saveStatus != saveStatus){
        m_saveStatus = saveStatus;

        emit saveStatusChanged(m_saveStatus);
    }
}

void ModelSelectViewModel::setModelName(QString modelName)
{
    if(m_modelName != modelName){
        m_modelName = modelName;

        emit modelNameChanged(m_modelName);
    }
}

void ModelSelectViewModel::setSelectedModelIdx(int selectedModelIdx)
{
    if(m_selectedModelIdx != selectedModelIdx){
        m_selectedModelIdx = selectedModelIdx;

        emit selectedModelIdxChanged(m_selectedModelIdx);
    }
}

void ModelSelectViewModel::onFinished(){
    if(m_pageChanging == false && m_modbusManager->getIsPageChanging() == false){
        QByteArray makerNameByteAry;

        for(int idx = 0; idx < 8; idx++){
            short s = m_modbusManager->getReceiveDataRG(MC_MSVM_MODEL_SR_TOP + idx);
            char c_high = (s >> 8) & 0xFF;
            char c_low = s & 0xFF;

            makerNameByteAry.append(c_high);
            makerNameByteAry.append(c_low);
        }

        if(0 < m_modelList.count() && m_dataLoaded == false){
            QString modelName = QString(makerNameByteAry);
            int selectedModelIdx = m_modelList.indexOf(modelName);

            setModelName(modelName);
            setSelectedModelIdx((selectedModelIdx != -1 ? selectedModelIdx : 0));

            setDataLoaded(true);
        }

        for(int i = 0; i < 8; i++){
            short s_capacity = m_modbusManager->getReceiveDataRG(MC_MSVM_CAPACITY_SR_TOP + i);
            short s_encoderType = m_modbusManager->getReceiveDataRG(MC_MSVM_ENCODERTYPE_SR_TOP + i);
            long stroke = m_modbusManager->getLongReceiveDataRG(MC_MSVM_STROKE_SR_TOP + (i * 2));

            char b_capacity = s_capacity & 0x000F;
            char b_encoderType = s_encoderType & 0x000F;

            m_dataList[i]->setCapacity((int)b_capacity);
            m_dataList[i]->setEncoderType((int)b_encoderType);
            m_dataList[i]->setStroke(stroke);
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

void ModelSelectViewModel::onModelListFinished(){

    disconnect(m_communication, &CommunicationThread::finish, this, &ModelSelectViewModel::onModelListFinished);

    if(m_modelList.count() == 0){
        for(int i = 0; i < 32; i++){
            QByteArray makerNameByteAry;
            QString makerName;
            for(int idx = 0; idx < 8; idx++){
                short s = m_modbusManager->getReceiveDataRG(MC_MSVM_MODEL_SR + (i * 8) + idx);
                char c_high = (s >> 8) & 0xFF;
                char c_low = s & 0xFF;

                makerNameByteAry.append(c_high);
                makerNameByteAry.append(c_low);
            }
            makerName = QString(makerNameByteAry);

            if(0 < makerName.length()){
                m_modelList.append(makerName);
            }
        }

        modelListChanged(m_modelList);

        m_dataLoaded = false;
    }

    for(int i = 0; i < 8; i++){
        QByteArray makerNameByteAry;
        QString makerName;
        for(int idx = 0; idx < 8; idx++){
            short s = m_modbusManager->getReceiveDataRG(MC_MSVM_MAKER_SR + (i * 8) + idx);
            char c_high = (s >> 8) & 0xFF;
            char c_low = s & 0xFF;

            makerNameByteAry.append(c_high);
            makerNameByteAry.append(c_low);
        }
        makerName = QString(makerNameByteAry);
        m_dataList[i]->setMakerName(makerName);
    }

    connect(m_communication, &CommunicationThread::finish, this, &ModelSelectViewModel::onFinished);
}

void ModelSelectViewModel::onModelChangeFinished(){

    disconnect(m_communication, &CommunicationThread::finish, this, &ModelSelectViewModel::onModelChangeFinished);

    setSaveStatus(2);
}

void ModelSelectViewModel::endModbusPageChange(int mainScreenId, int subScreenId){
    if(mainScreenId == 18 && subScreenId == 0){
        disconnect(m_modbusManager, &ModbusManager::endModbusPageChange, this, &ModelSelectViewModel::endModbusPageChange);

        m_pageChanging = false;

        connect(m_communication, &CommunicationThread::finish, this, &ModelSelectViewModel::onModelListFinished);
    }
}
