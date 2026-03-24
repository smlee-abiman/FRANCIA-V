#include "driverparametersviewmodel.h"

DrvPrmData::DrvPrmData(long index, QString min, QString max, short scale, long format, short tenkeyType, QObject *parent)
    : QObject(parent)
{
    m_index = index;
    m_min = min;
    m_max = max;
    m_scale = scale;
    m_format = format;
    m_tenkeyType = tenkeyType;
    m_defValue = 0;
    m_value = 0;
    m_isChange = false;
}

DrvPrmData::~DrvPrmData(){

}

void DrvPrmData::setDefValue(long defValue)
{
    if(m_defValue != defValue){
        m_defValue = defValue;

        emit defValueChanged(m_defValue);
    }
}

void DrvPrmData::setValue(long value, bool firstDataLoaded)
{
    if(m_value != value){
        m_value = value;

        emit valueChanged(m_value);

        if(firstDataLoaded == true && m_isChange != true){
            m_isChange = true;
            emit isChangeChanged(m_isChange);
        }
    }
}

void DrvPrmData::setIsChange(bool isChange)
{
    if(m_isChange != isChange){
        m_isChange = isChange;

        emit isChangeChanged(m_isChange);
    }
}

DriverParametersViewModel::DriverParametersViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    if (m_modbusManager){
        //qDebug() << "IOMonitorViewModel:m_modbusManager is share";
    }

    m_pageNum = 0;
    m_pageMaxNum = m_resource->getPenSetIntValue("18", 58, 0);
    m_axisPageNum = 0;
    m_axisPageMaxNum = 2;

    m_saveStatus = 0;
    m_modelName = "";

    m_firstDataLoadedFlag = false;

    readPensetData();
    readParameter();
}

DriverParametersViewModel::~DriverParametersViewModel()
{

}

void DriverParametersViewModel::readPensetData()
{
    const int pageMax = MC_DPVM_PAGE_MAX;
    const int pageRowMax = MC_DPVM_ROW_MAX;
    const int axisMax = MC_DPVM_AXIS_MAX;

    for(int page = 0; page < pageMax; page++){
        for(int row = 0; row < pageRowMax; row++){
            int penSetRow = 63 + (page * pageRowMax) + row;
            int penSetRowFormat = 141 + (page * pageRowMax) + row;
            for(int axis = 0; axis < axisMax; axis++){
                QString indexStr = m_resource->getPenSetStringValue("18", penSetRow, axis);
                int index = -1;

                if(0 < indexStr.length()){
                    index = m_resource->getPenSetIntValue("18", penSetRow, axis);
                }
                QString min = m_resource->getPenSetStringValue("18", penSetRow, 8);
                QString max = m_resource->getPenSetStringValue("18", penSetRow, 9);
                int scale = 0;
                QString scaleStr = m_resource->getPenSetStringValue("18", penSetRow, 10);

                if(scaleStr.endsWith("10")){
                    scale = 1;
                }
                else if(scaleStr.endsWith("100")){
                    scale = 2;
                }
                else if(scaleStr.endsWith("1000")){
                    scale = 3;
                }
                int format = m_resource->getPenSetIntValue("18", penSetRowFormat, axis);
                short tenkeyType = (m_resource->formatIsDecimals(format) ? 6 : 2);

                m_dataList[page][row][axis] = new DrvPrmData(index, min, max, scale, format, tenkeyType);
                QQmlEngine::setObjectOwnership(m_dataList[page][row][axis], QQmlEngine::CppOwnership);
            }
        }
    }
}

void DriverParametersViewModel::onActivate()
{
    startMonitoring();
}

void DriverParametersViewModel::onDeactivate()
{
    stopMonitoring();
}

void DriverParametersViewModel::readParameter()
{
    QList<stModBusCommunicationData> readCommandListHR;
    QList<stModBusCommunicationData> readCommandListSR;

    for(int i = 0;; i++){
        stModBusCommunicationData readCommandListHR;
        stModBusCommunicationData readCommandListSR;
        int index = MC_DPVM_DATA_TOP + (MC_DPVM_DATA_SIZE * i);

        if(index < MC_DPVM_DATA_END){
            int size = (MC_DPVM_DATA_SIZE < MC_DPVM_DATA_END - index ? MC_DPVM_DATA_SIZE : (MC_DPVM_DATA_END - index));

            readCommandListHR =
                    ModbusCommandGenerator::makeModbusReadCommand(QModbusPdu::FunctionCode::ReadHoldingRegisters, index, size);
            readCommandListSR =
                    ModbusCommandGenerator::makeModbusReadCommand(QModbusPdu::FunctionCode::ReadInputRegisters, index, size);

            m_communication->setDelayCommandList(Enums::Pages::FunctionMechanicalParamModelSelect, readCommandListHR);
            m_communication->setDelayCommandList(Enums::Pages::FunctionMechanicalParamModelSelect, readCommandListSR);
        }
        else{
            break;
        }
    }

    connect(m_communication, &CommunicationThread::finish, this, &DriverParametersViewModel::onReadParameterFinished);
}

void DriverParametersViewModel::setValue(int row, int axis, double value)
{
    stModBusCommunicationData writeCommand[1];
    stModBusCommunicationData readCommand[1];
    QList<int> writeData;
    writeData.clear();

    DrvPrmData* data = m_dataList[m_pageNum][row][axis];
    int index = data->index();
    int scale = data->scale();
    long correct_value = value * (0 < scale ? qPow(10, scale) : 1);

    short s_high_value = (correct_value >> 16) & 0x0000FFFF;
    short s_low_value = correct_value & 0x0000FFFF;
    writeData.append((int)s_low_value);
    writeData.append((int)s_high_value);

    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, index, writeData.length(), writeData);

    m_communication->setOneShotModbusCommand(writeCommand[0]);

    readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, index, writeData.length());
    m_operationRecord->recordSetDriverParam(m_pageNum * MC_DPVM_ROW_MAX + row, axis, data->value(), correct_value);
    m_communication->setOneShotModbusCommand(readCommand[0]);
}

void DriverParametersViewModel::initParameter()
{
    stModBusCommunicationData writeCommand[1];
    QList<int> writeData;

    m_firstDataLoadedFlag = false;

    setSaveStatus(1);

    writeData.clear();
    writeData.append(28);
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, MC_DPVM_SAVE_HR, 1, writeData);

    m_communication->setModbusWriteCommand(writeCommand[0]);
    m_operationRecord->recordInitDriverParam();
    connect(m_communication, &CommunicationThread::finish, this, &DriverParametersViewModel::onDataInitFinished);
}

void DriverParametersViewModel::saveParameter()
{
    stModBusCommunicationData writeCommand[1];
    QList<int> writeData;

    setSaveStatus(1);

    writeData.clear();
    writeData.append(31);
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, MC_DPVM_SAVE_HR, 1, writeData);

    m_communication->setModbusWriteCommand(writeCommand[0]);
    m_operationRecord->recordSaveDriverParam();
    connect(m_communication, &CommunicationThread::finish, this, &DriverParametersViewModel::onDataSaveFinished);
}

void DriverParametersViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &DriverParametersViewModel::onFinished);
}
void DriverParametersViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &DriverParametersViewModel::onFinished);
}

void DriverParametersViewModel::pageInit(){
    setPageNum(0);
    setAxisPageNum(0);
}

void DriverParametersViewModel::setPageNum(int pageNum)
{
    m_pageNum = pageNum;

    emit pageNumChanged(m_pageNum);
}

void DriverParametersViewModel::setAxisPageNum(int axisPageNum)
{
    m_axisPageNum = axisPageNum;

    emit axisPageNumChanged(m_axisPageNum);
}

void DriverParametersViewModel::setSaveStatus(int saveStatus)
{
    m_saveStatus = saveStatus;

    emit saveStatusChanged(m_saveStatus);
}

void DriverParametersViewModel::setModelName(QString modelName)
{
    m_modelName = modelName;

    emit modelNameChanged(m_modelName);
}

QObject* DriverParametersViewModel::getData(int page, int row, int axis)
{
    QObject* data;

    if(page < MC_DPVM_PAGE_MAX && row < MC_DPVM_ROW_MAX && axis < MC_DPVM_AXIS_MAX){
        data = m_dataList[page][row][axis];
    }
    else{
        data= new DrvPrmData(-1, "0", "0", 0, 0, 0);
    }

    return data;
}

void DriverParametersViewModel::firstDataLoad()
{
    for(int page = 0; page < MC_DPVM_PAGE_MAX; page++){
        for(int row = 0; row < MC_DPVM_ROW_MAX; row++){
            for(int axis = 0; axis < MC_DPVM_AXIS_MAX; axis++){
                DrvPrmData* data = m_dataList[page][row][axis];

                data->setDefValue(m_modbusManager->getLongReceiveDataRG(data->index()));
                data->setValue(m_modbusManager->getLongReceiveDataHR(data->index()), m_firstDataLoadedFlag);
            }
        }
    }

    m_firstDataLoadedFlag = true;
}

void DriverParametersViewModel::updateData()
{
    for(int row = 0; row < MC_DPVM_ROW_MAX; row++){
        for(int axis = 0; axis < MC_DPVM_AXIS_MAX; axis++){
            DrvPrmData* data = m_dataList[m_pageNum][row][axis];

            data->setDefValue(m_modbusManager->getLongReceiveDataRG(data->index()));
            data->setValue(m_modbusManager->getLongReceiveDataHR(data->index()), m_firstDataLoadedFlag);
        }
    }
}

void DriverParametersViewModel::onFinished(){

    QByteArray modelNameByteAry;
    QString modelName;
    for(int idx = 0; idx < 8; idx++){
        short s = m_modbusManager->getReceiveDataRG(MC_DPVM_MODEL_SR_TOP + idx);
        char c_high = (s >> 8) & 0xFF;
        char c_low = s & 0xFF;

        modelNameByteAry.append(c_high);
        modelNameByteAry.append(c_low);
    }
    modelName = QString(modelNameByteAry);
    setModelName(modelName);
    updateData();

    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void DriverParametersViewModel::onReadParameterFinished(){

    disconnect(m_communication, &CommunicationThread::finish, this, &DriverParametersViewModel::onReadParameterFinished);

    firstDataLoad();
}

void DriverParametersViewModel::onDataSaveFinished(){

    disconnect(m_communication, &CommunicationThread::finish, this, &DriverParametersViewModel::onDataSaveFinished);

    setSaveStatus(2);

    for(int page = 0; page < MC_DPVM_PAGE_MAX; page++){
        for(int row = 0; row < MC_DPVM_ROW_MAX; row++){
            for(int axis = 0; axis < MC_DPVM_AXIS_MAX; axis++){
                DrvPrmData* data = m_dataList[page][row][axis];

                data->setIsChange(false);
            }
        }
    }
}

void DriverParametersViewModel::onDataInitFinished()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &DriverParametersViewModel::onDataInitFinished);

    setSaveStatus(2);

    for(int page = 0; page < MC_DPVM_PAGE_MAX; page++){
        for(int row = 0; row < MC_DPVM_ROW_MAX; row++){
            for(int axis = 0; axis < MC_DPVM_AXIS_MAX; axis++){
                DrvPrmData* data = m_dataList[page][row][axis];

                data->setIsChange(false);
            }
        }
    }

    updateData();
    m_firstDataLoadedFlag = true;
}
