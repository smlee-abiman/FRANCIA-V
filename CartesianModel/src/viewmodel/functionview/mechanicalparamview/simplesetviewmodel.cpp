#include "simplesetviewmodel.h"

SimpleSetViewModel::SimpleSetViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    m_simpleSetAxisInfo = new SimpleSetAxisInfo();
    initSettingPenSetData();
    //initPageCommand();
}

SimpleSetViewModel::~SimpleSetViewModel()
{
    delete m_simpleSetAxisInfo;
}

void SimpleSetViewModel::onActivate()
{
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    connect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &SimpleSetViewModel::onEndModbusPageWriteReady, Qt::DirectConnection);
    startModbusMonitoring();
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
}

void SimpleSetViewModel::onDeactivate()
{
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    if (m_pageChanging) { disconnect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &SimpleSetViewModel::onEndModbusPageWriteReady); }
    timerStop();
    stopModbusMonitoring();
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    stopModbusOneShot();
}

void SimpleSetViewModel::startModbusMonitoring()
{
    if (!m_communication) return;
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    connect(m_communication, &CommunicationThread::finish,
        this, &SimpleSetViewModel::onFinished, Qt::QueuedConnection);
#else // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    connect(m_communication, &CommunicationThread::feed,
        this, &SimpleSetViewModel::updateSimpleSet, Qt::QueuedConnection);
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
}

void SimpleSetViewModel::stopModbusMonitoring()
{
    if (!m_communication) return;
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    disconnect(m_communication, &CommunicationThread::finish,
        this, &SimpleSetViewModel::onFinished);
#else // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    disconnect(m_communication, &CommunicationThread::feed,
        this, &SimpleSetViewModel::updateSimpleSet);
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
}

void SimpleSetViewModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (!mIsModbusConnectOneShot) {
        connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &SimpleSetViewModel::onOneShotReadFinished, Qt::QueuedConnection);
        mIsModbusConnectOneShot = true;
    }
}
void SimpleSetViewModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (mIsModbusConnectOneShot){
        disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &SimpleSetViewModel::onOneShotReadFinished);
        mIsModbusConnectOneShot = false;
    }
}

void SimpleSetViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerFunctionMachineDrvParamSimpleSetPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void SimpleSetViewModel::initSettingPenSetData()
{
    if (!m_resource){
        return;
    }
    m_dispItemNum               = m_resource->getIntValueList(18, 218, 17);
    int dispInvalidCnt = 0;
    for(int i = 0; i < m_dispItemNum.count() + 1; i++){
        int drvParamDispFormatIndex = 0;
        if (dispInvalidCnt < SIMPLE_SET_DISP_FORMAT_INVALID_INDEX_LIST.count()
            && i == SIMPLE_SET_DISP_FORMAT_INVALID_INDEX_LIST.at(dispInvalidCnt)){
            drvParamDispFormatIndex = -1;
            dispInvalidCnt++;
        } else {
            drvParamDispFormatIndex = GET_SIMPLE_SET_DISP_ITEM_INDEX(m_dispItemNum.at(i).value(0));
        }
        if (drvParamDispFormatIndex < 0){
            m_axisInfoDataDispFormat.append(SIMPLE_SET_PARAM_DEFAULT_FORAMT.at(0));
        } else {
            m_axisInfoDataDispFormat.append(
                m_resource->getIntValueList(18, drvParamDispFormatIndex, 1, false)
            );
        }
    }
#if 0
    for(int i = 0; i < 17; i++){
        qDebug() << "value(0):" << m_dispItemNum.at(i).value(0);
    }
    qDebug("%s:%d m_axisInfoDataDispFormat count[%d]", __func__, __LINE__, m_axisInfoDataDispFormat.count());
    for(int i = 0; i < m_axisInfoDataDispFormat.count(); i++){
        qDebug() << "value(0):" << m_axisInfoDataDispFormat.at(i).value(0) << "value(1):" << m_axisInfoDataDispFormat.at(i).value(1)
                << "value(2):" << m_axisInfoDataDispFormat.at(i).value(2) << "value(3):" << m_axisInfoDataDispFormat.at(i).value(3)
                << "value(4):" << m_axisInfoDataDispFormat.at(i).value(4) << "value(5):" << m_axisInfoDataDispFormat.at(i).value(5)
                << "value(6):" << m_axisInfoDataDispFormat.at(i).value(6) << "value(7):" << m_axisInfoDataDispFormat.at(i).value(7);
    }
#endif
}

void SimpleSetViewModel::initPageCommand()
{
    if (!m_communication) {
        qCritical("%s:%d m_communication is invalid", __func__, __LINE__);
        return;
    }
    stModBusCommunicationData readCommand[SIMPLE_SET_MODBUS_CMD_LIST_NUM];
    for(int i = 0; i < SIMPLE_SET_MODBUS_CMD_LIST_NUM; i++){
        readCommand[i] = ModbusCommandGenerator::makeModbusReadCommand(
                            initModbusPageCmd[i].funcCode,
                            initModbusPageCmd[i].address,
                            initModbusPageCmd[i].size
                            );
        m_communication->setPageReadCommandList(Enums::Pages::FunctionMPDrvParamSimpleSet, readCommand[i]);
    }
}

void SimpleSetViewModel::modbusGetValuesHR(int pageNum, int itemIndex, int axisIndex)
{
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return;
    }
    mCurPageNum = pageNum;
    mCurAxisIndex = axisIndex;
    int tableTopIndex = pageTopTableIndex[pageNum - 1];
    stModBusCommunicationData readCommand[2];
    if (tableTopIndex >= 0){
        quint16 topReadAddress = SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value(axisIndex);
        quint16 topReadSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex).value(axisIndex);
        readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadHoldingRegisters, topReadAddress, topReadSize);
        quint16 readAddress = SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex+itemIndex).value(axisIndex);
        quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex+itemIndex).value(axisIndex);
        readCommand[1] = ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadHoldingRegisters, readAddress, readSize);
        setOneShotModbusCommand(readCommand[0]);
        setOneShotModbusCommand(readCommand[1]);
    } else {
        modbusIndividualGetValuesHR(pageNum, itemIndex, axisIndex);
    }
}
void SimpleSetViewModel::modbusGetValuesSR(int pageNum, int itemIndex, int axisIndex)
{
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return;
    }
    mCurPageNum = pageNum;
    mCurAxisIndex = axisIndex;
    int tableTopIndex =  pageTopTableIndex[pageNum - 1];
    if (tableTopIndex >= 0){
        quint16 readAddress = SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex+itemIndex).value(axisIndex);
        quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex+itemIndex).value(axisIndex);
        stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadInputRegisters, readAddress, readSize);
        setOneShotModbusCommand(command);
    } else {
        modbusIndividualGetValuesSR(pageNum, itemIndex, axisIndex);
    }
}

void SimpleSetViewModel::getPageValuesHR(int pageNum, int axisIndex)
{
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return;
    }
    mCurPageNum = pageNum;
    mCurAxisIndex = axisIndex;
    int tableTopIndex =  pageTopTableIndex[pageNum - 1];
    if (tableTopIndex >= 0){
        SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurAxisIndex);
        switch(mCurPageNum){
        case 1:{
            int mode = m_modbus->getReceiveDataHR(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value((int)axis));
            QString setValues[3];
            for(int i = 0; i < 3; i++){
                setValues[i] = m_resource->formatData(
                        m_modbus->getReceiveDataHR(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis)),
                        m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage1Mode(mode);
            m_simpleSetAxisInfo->setPage1SetValue1(axis, setValues[0]);
            m_simpleSetAxisInfo->setPage1SetValue2(axis, setValues[1]);
            m_simpleSetAxisInfo->setPage1SetValue3(axis, setValues[2]);
            break;
        }
        case 2:{
            int mode = m_modbus->getReceiveDataHR(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value((int)axis));
            m_simpleSetAxisInfo->setPage2Mode(axis, mode);
            break;
        }
        case 3:{
            int mode = m_modbus->getReceiveDataHR(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value((int)axis));
            QString setValues[3];
            for(int i = 0; i < 3; i++){
                setValues[i] = m_resource->formatData(
                        m_modbus->getReceiveDataHR(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis)),
                        m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage3Mode(axis, mode);
            m_simpleSetAxisInfo->setPage3SetValue1(axis, setValues[0]);
            m_simpleSetAxisInfo->setPage3SetValue2(axis, setValues[1]);
            m_simpleSetAxisInfo->setPage3SetValue3(axis, setValues[2]);
            break;
        }
#if 0
        case 4:{
            QString setValues[2];
            for(int i = 0; i < 2; i++){
                setValues[i] = m_resource->formatData(
                        m_modbus->getReceiveDataHR(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis)),
                        m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage4SetValue1(axis, setValues[0]);
            m_simpleSetAxisInfo->setPage4SetValue2(axis, setValues[1]);
            break;
        }
#endif
        default:
            break;
        }
    } else {
        SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurAxisIndex);
        switch(mCurPageNum){
        case 4:{
            QString setValues[2];
            for(int i = 0; i < 2; i++){
                setValues[i] = m_resource->formatData(
                        m_modbus->getReceiveDataHR(SIMPLE_SET_PARAM_ADDR_LIST.at(16 + i).value((int)axis)),
                        m_axisInfoDataDispFormat.at(16 + i).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage4SetValue1(axis, setValues[0]);
            m_simpleSetAxisInfo->setPage4SetValue2(axis, setValues[1]);
            break;
        }
        default:
            break;
        }
    }
}
void SimpleSetViewModel::getPageValuesSR(int pageNum, int axisIndex)
{
    if (!m_modbus || !m_simpleSetAxisInfo || !m_resource) return;
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return;
    }
    mCurPageNum = pageNum;
    mCurAxisIndex = axisIndex;
    int tableTopIndex =  pageTopTableIndex[pageNum - 1];
    if (tableTopIndex >= 0){
        SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurAxisIndex);
        switch(mCurPageNum){
        case 1:{
            QString defValues[3];
            for(int i = 0; i < 3; i++){
                defValues[i] = m_resource->formatData(
                        m_modbus->getReceiveDataRG(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis)),
                        m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage1DefValue1(axis, defValues[0]);
            m_simpleSetAxisInfo->setPage1DefValue2(axis, defValues[1]);
            m_simpleSetAxisInfo->setPage1DefValue3(axis, defValues[2]);
            break;
        }
        case 2:{
            QString anyValues[6];
            for(int i = 0; i < 6; i++){
                anyValues[i] = m_resource->formatData(
                        m_modbus->getReceiveDataRG(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis)),
                        m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage2AnyValue1(axis, anyValues[0]);
            m_simpleSetAxisInfo->setPage2AnyValue2(axis, anyValues[1]);
            m_simpleSetAxisInfo->setPage2AnyValue3(axis, anyValues[2]);
            m_simpleSetAxisInfo->setPage2AnyValue4(axis, anyValues[3]);
            m_simpleSetAxisInfo->setPage2AnyValue5(axis, anyValues[4]);
            m_simpleSetAxisInfo->setPage2AnyValue6(axis, anyValues[5]);
            break;
        }
        case 3:{
            QString defValues[3];
            for(int i = 0; i < 3; i++){
                defValues[i] = m_resource->formatData(
                        m_modbus->getReceiveDataRG(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis)),
                        m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage3DefValue1(axis, defValues[0]);
            m_simpleSetAxisInfo->setPage3DefValue2(axis, defValues[1]);
            m_simpleSetAxisInfo->setPage3DefValue3(axis, defValues[2]);
            break;
        }
#if 0
        case 4:{
            QString anyValues = m_resource->formatData(
                                    m_modbus->getReceiveDataRG(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value((int)axis)),
                                    m_axisInfoDataDispFormat.at(tableTopIndex).value((int)axis));
            QString defValues[2];
            for(int i = 0; i < 2; i++){
                defValues[i] = m_resource->formatData(
                        m_modbus->getReceiveDataRG(SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis)),
                        m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage4AnyValue1(axis, anyValues);
            m_simpleSetAxisInfo->setPage4DefValue1(axis, defValues[0]);
            m_simpleSetAxisInfo->setPage4DefValue2(axis, defValues[1]);
            break;
        }
#endif
        default:
            break;
        }
    } else {
        SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurAxisIndex);
        switch(mCurPageNum){
        case 4:{
            QString anyValues = m_resource->formatData(
                                    m_modbus->getReceiveDataRG(SIMPLE_SET_PARAM_ADDR_LIST.at(15).value((int)axis)),
                                    m_axisInfoDataDispFormat.at(15).value((int)axis));
            QString defValues = m_resource->formatData(
                                    m_modbus->getReceiveDataRG(SIMPLE_SET_PARAM_ADDR_LIST.at(16).value((int)axis)),
                                    m_axisInfoDataDispFormat.at(16).value((int)axis));
            m_simpleSetAxisInfo->setPage4AnyValue1(axis, anyValues);
            m_simpleSetAxisInfo->setPage4DefValue1(axis, defValues);
            break;
        }
        default:
            break;
        }
    }
}

void SimpleSetViewModel::updateModbusValuesHR(stModBusCommunicationData &data)
{
    if (!m_modbus || !m_simpleSetAxisInfo || !m_resource) return;
    SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurAxisIndex);
    int tableTopIndex =  pageTopTableIndex[mCurPageNum - 1];
    if (tableTopIndex >= 0){
        switch(mCurPageNum){
        case 1:{
            if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value(mCurAxisIndex)){
                int mode = (int)data.readBuf[0];
                m_simpleSetAxisInfo->setPage1Mode(mode);
            } else {
                for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                    if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex)){
                        quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex);
                        QString setValues;
                        if (readSize == 1){
                            setValues = m_resource->formatData((short)data.readBuf[0], m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                        } else {
                            int readData = ((int)data.readBuf[1] << 16) + (int)data.readBuf[0];
                            setValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                        }
                        if (i == 0){
                            m_simpleSetAxisInfo->setPage1SetValue1(axis, setValues);
                        } else if (i == 1){
                            m_simpleSetAxisInfo->setPage1SetValue2(axis, setValues);
                        } else if (i == 2){
                            m_simpleSetAxisInfo->setPage1SetValue3(axis, setValues);
                        }
                        continue;
                    }
                }
            }
            break;
        }
        case 2:{
            int mode = (int)data.readBuf[0];
            m_simpleSetAxisInfo->setPage2Mode(axis, mode);
            break;
        }
        case 3:{
            if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value(mCurAxisIndex)){
                int mode = (int)data.readBuf[0];
                m_simpleSetAxisInfo->setPage3Mode(axis, mode);
            } else {
                for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                    if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex)){
                        quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex);
                        QString setValues;
                        if (readSize == 1){
                            setValues = m_resource->formatData((short)data.readBuf[0], m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                        } else {
                            int readData = ((int)data.readBuf[1] << 16) + (int)data.readBuf[0];
                            setValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                        }
                        if (i == 0){
                            m_simpleSetAxisInfo->setPage3SetValue1(axis, setValues);
                        } else if (i == 1){
                            m_simpleSetAxisInfo->setPage3SetValue2(axis, setValues);
                        } else if (i == 2){
                            m_simpleSetAxisInfo->setPage3SetValue3(axis, setValues);
                        }
                        continue;
                    }
                }
            }
            break;
        }
#if 0
        case 4:{
            QString setValues[2];
            for(int i = 0; i < 2; i++){
                offset = SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis) - SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value((int)axis);
                setValues[i] = m_resource->formatData((short)data.readBuf[0 + offset], m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
            }
            m_simpleSetAxisInfo->setPage4SetValue1(axis, setValues[0]);
            m_simpleSetAxisInfo->setPage4SetValue2(axis, setValues[1]);
            break;
        }
#endif
        default:
            break;
        }
    } else {
        switch(mCurPageNum){
        case 4:{
            for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(16 + i).value(mCurAxisIndex)){
                    quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(16 + i).value(mCurAxisIndex);
                    QString setValues;
                    if (readSize == 1){
                        setValues = m_resource->formatData((short)data.readBuf[0], m_axisInfoDataDispFormat.at(15 + i).value((int)axis));
                    } else {
                        int readData = ((int)data.readBuf[1] << 16) + (int)data.readBuf[0];
                        setValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(15 + i).value((int)axis));
                    }
                    if (i == 0){
                        m_simpleSetAxisInfo->setPage4SetValue1(axis, setValues);
                    } else if (i == 1){
                        m_simpleSetAxisInfo->setPage4SetValue2(axis, setValues);
                    }
                    continue;
                }
            }
            break;
        }
        default:
            break;
        }
    }
}
void SimpleSetViewModel::updateModbusValuesSR(stModBusCommunicationData &data)
{
    if (!m_modbus || !m_simpleSetAxisInfo || !m_resource) return;
    SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurAxisIndex);
    int tableTopIndex =  pageTopTableIndex[mCurPageNum - 1];
    if (tableTopIndex >= 0){
        switch(mCurPageNum){
        case 1:{
            for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex)){
                    quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex);
                    QString defValues;
                    if (readSize == 1){
                        defValues = m_resource->formatData((short)data.readBuf[0], m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                    } else {
                        int readData = ((int)data.readBuf[1] << 16) + (int)data.readBuf[0];
                        defValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                    }
                    if (i == 0){
                        m_simpleSetAxisInfo->setPage1DefValue1(axis, defValues);
                    } else if (i == 1){
                        m_simpleSetAxisInfo->setPage1DefValue2(axis, defValues);
                    } else if (i == 2){
                        m_simpleSetAxisInfo->setPage1DefValue3(axis, defValues);
                    }
                    continue;
                }
            }
            break;
        }
        case 2:{
            for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex)){
                    quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex);
                    QString anyValues;
                    if (readSize == 1){
                        anyValues = m_resource->formatData((short)data.readBuf[0], m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                    } else {
                        int readData = ((int)data.readBuf[1] << 16) + (int)data.readBuf[0];
                        anyValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                    }
                    if (i == 0){
                        m_simpleSetAxisInfo->setPage2AnyValue1(axis, anyValues);
                    } else if (i == 1){
                        m_simpleSetAxisInfo->setPage2AnyValue2(axis, anyValues);
                    } else if (i == 2){
                        m_simpleSetAxisInfo->setPage2AnyValue3(axis, anyValues);
                    } else if (i == 3){
                        m_simpleSetAxisInfo->setPage2AnyValue4(axis, anyValues);
                    } else if (i == 4){
                        m_simpleSetAxisInfo->setPage2AnyValue5(axis, anyValues);
                    } else if (i == 5){
                        m_simpleSetAxisInfo->setPage2AnyValue6(axis, anyValues);
                    }
                    continue;
                }
            }
            break;
        }
        case 3:{
            for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex)){
                    quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex + (i + 1)).value(mCurAxisIndex);
                    QString defValues;
                    if (readSize == 1){
                        defValues = m_resource->formatData((short)data.readBuf[0], m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                    } else {
                        int readData = ((int)data.readBuf[1] << 16) + (int)data.readBuf[0];
                        defValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                    }
                    if (i == 0){
                        m_simpleSetAxisInfo->setPage3DefValue1(axis, defValues);
                    } else if (i == 1){
                        m_simpleSetAxisInfo->setPage3DefValue2(axis, defValues);
                    } else if (i == 2){
                        m_simpleSetAxisInfo->setPage3DefValue3(axis, defValues);
                    }
                    continue;
                }
            }
            break;
        }
#if 0
        case 4:{
            QString anyValues = m_resource->formatData((short)data.readBuf[0], m_axisInfoDataDispFormat.at(tableTopIndex).value((int)axis));
            QString defValues[2];
            for(int i = 0; i < 2; i++){
                offset = SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + (i + 1)).value((int)axis) - SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex).value((int)axis);
                if (offset >= 0){
                    defValues[i] = m_resource->formatData((short)data.readBuf[0 + offset], m_axisInfoDataDispFormat.at(tableTopIndex + (i + 1)).value((int)axis));
                } else {
                    defValues[i] = "";
                }
            }
            m_simpleSetAxisInfo->setPage4AnyValue1(axis, anyValues);
            m_simpleSetAxisInfo->setPage4DefValue1(axis, defValues[0]);
            m_simpleSetAxisInfo->setPage4DefValue2(axis, defValues[1]);
            break;
        }
#endif
        default:
            break;
        }
    } else {
        switch(mCurPageNum){
        case 4:{
            for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                if (data.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(16 + i).value(mCurAxisIndex)){
                    quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(16 + i).value(mCurAxisIndex);
                    if (i == 0){
                        QString defValues;
                        if (readSize == 1){
                            defValues = m_resource->formatData((short)data.readBuf[0], m_axisInfoDataDispFormat.at(15 + i).value((int)axis));
                        } else {
                            int readData = ((int)data.readBuf[1] << 16) + (int)data.readBuf[0];
                            defValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(15 + i).value((int)axis));
                        }
                        m_simpleSetAxisInfo->setPage4DefValue1(axis, defValues);
                    } else if (i == 1){
                        // NOP
                    }
                    continue;
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

void SimpleSetViewModel::modbusIndividualGetValuesHR(int pageNum, int itemIndex, int axisIndex)
{
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return;
    }
    switch(pageNum){
    case 4:{
        stModBusCommunicationData readCommand;
        quint16 readAddress = ((itemIndex == 1) ? SIMPLE_SET_PARAM_ADDR_LIST.at(16).value(axisIndex) : SIMPLE_SET_PARAM_ADDR_LIST.at(17).value(axisIndex));
        quint16 readSize = ((itemIndex == 1) ? SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(16).value(axisIndex) : SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(17).value(axisIndex));
        readCommand = ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadHoldingRegisters, readAddress, readSize);
        setOneShotModbusCommand(readCommand);
        break;
    }
    default:
        break;
    }
}

void SimpleSetViewModel::modbusIndividualGetValuesSR(int pageNum, int itemIndex, int axisIndex)
{
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return;
    }
    switch(pageNum){
    case 4:{
        stModBusCommunicationData readCommand;
        quint16 readAddress = ((itemIndex == 1) ? SIMPLE_SET_PARAM_ADDR_LIST.at(16).value(axisIndex) : SIMPLE_SET_PARAM_ADDR_LIST.at(16).value(axisIndex));
        quint16 readSize = ((itemIndex == 1) ? SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(16).value(axisIndex) : SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(16).value(axisIndex));
        readCommand = ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadInputRegisters, readAddress, readSize);
        setOneShotModbusCommand(readCommand);
        break;
    }
    default:
        break;
    }
}

void SimpleSetViewModel::modbusSetCalcValue(int pageNum, int itemIndex, int axisIndex, int writeValue, float decimalWriteValue)
{
    //axisIndex = ((axisIndex == 3) ? 2 : ((axisIndex == 2) ? 3 : axisIndex));
    qDebug("%s:%d pageNum[%d] itemIndex[%d] axisIndex[%d] writeValue[%d] decimalWriteValue[%3.2f]",
        __func__, __LINE__, pageNum, itemIndex, axisIndex, writeValue, decimalWriteValue);
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    int tableTopIndex =  pageTopTableIndex[pageNum - 1];
    if (itemIndex < 0 || (tableTopIndex >= 0 && itemIndex >= pageItemNum[pageNum - 1])){
        return;
    }
    quint16 writeSize = 0;
    float adjMagni = 1.0;
    QList<int> writeData;
    if (tableTopIndex >= 0){
        writeSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(tableTopIndex+itemIndex).value(axisIndex);
        adjMagni = SIMPLE_SET_PARAM_ADJ_MAGNI_LIST.at(tableTopIndex+itemIndex).value(axisIndex);
        if (writeSize == 1){
            if (adjMagni > 1.0){
                writeValue = (int)(decimalWriteValue * adjMagni);
            }
            writeData.append(writeValue);
        } else {
            if (adjMagni > 1.0){
                writeValue = (int)(decimalWriteValue * adjMagni);
            }
            writeData.append((writeValue & 0x0000FFFF));
            writeData.append((writeValue & 0xFFFF0000) >> 16);
        }
        stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleRegisters, SIMPLE_SET_PARAM_ADDR_LIST.at(tableTopIndex + itemIndex).value(axisIndex), writeSize, writeData);
        m_communication->setModbusWriteCommand(command);
    } else {
        writeSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(15+itemIndex).value(axisIndex);
        adjMagni = SIMPLE_SET_PARAM_ADJ_MAGNI_LIST.at(15+itemIndex).value(axisIndex);
        if (writeSize == 1){
            if (adjMagni > 1.0){
                writeValue = (int)(decimalWriteValue * adjMagni);
            }
            writeData.append(writeValue);
        } else {
            if (adjMagni > 1.0){
                writeValue = (int)(decimalWriteValue * adjMagni);
            }
            writeData.append((writeValue & 0x0000FFFF));
            writeData.append((writeValue & 0xFFFF0000) >> 16);
        }
        stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleRegisters, SIMPLE_SET_PARAM_ADDR_LIST.at(15 + itemIndex).value(axisIndex), writeSize, writeData);
        m_communication->setModbusWriteCommand(command);
    }
}

void SimpleSetViewModel::setSaveStatus(int saveStatus)
{
    if (m_saveStatus == saveStatus){
        return;
    }
    m_saveStatus = saveStatus;
    emit saveStatusChanged(m_saveStatus);
}

void SimpleSetViewModel::modbusSaveCalcValue()
{
    QList<int> writeData;
    int writeValue = SIMPLE_SET_SAVE_DATA;
    setSaveStatus(1);
    writeData.append(writeValue);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, SIMPLE_SET_SAVE_DATA_HR, 0x0001, writeData);
    m_communication->setModbusWriteCommand(command);
    mTimerCnt = 0;
    mIsSaveStart = true;
    connect(m_communication, &CommunicationThread::finish, this, &SimpleSetViewModel::onDataSaveFinished);
}

void SimpleSetViewModel::modbusGetValues(int pageNum, int itemIndex, int axisIndex, bool isHR)
{
    //axisIndex = ((axisIndex == 3) ? 2 : ((axisIndex == 2) ? 3 : axisIndex));
    qDebug("%s:%d pageNum[%d] itemIndex[%d] axisIndex[%d] isHR[%d]", __func__, __LINE__, pageNum, itemIndex, axisIndex, isHR);
    if (isHR){
        modbusGetValuesHR(pageNum, itemIndex, axisIndex);
    } else {
        modbusGetValuesSR(pageNum, itemIndex, axisIndex);
    }
}

void SimpleSetViewModel::modbusSetPageMode(int pageNum, int axisIndex, int writeValue)
{
    if (!m_communication) return;
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return;
    }
    //axisIndex = ((axisIndex == 3) ? 2 : ((axisIndex == 2) ? 3 : axisIndex));
    qDebug("%s:%d pageNum[%d] axisIndex[%d] writeValue[%d]", __func__, __LINE__, pageNum, axisIndex, writeValue);
    int pageTopIndex = pageTopTableIndex[pageNum - 1];
    if (pageTopIndex >= 0){
        quint16 pageModeHrAddr = (quint16)SIMPLE_SET_PARAM_ADDR_LIST.at(pageTopIndex).value(axisIndex);
        quint16 writeSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(pageTopIndex).value(axisIndex);
        qDebug("%s:%d pageTopIndex[%d] pageModeHrAddr[%d]", __func__, __LINE__, pageTopIndex, pageModeHrAddr);
        QList<int> writeData;
        if (writeSize == 1){
            writeData.append(writeValue);
        } else {
            writeData.append((writeValue & 0x0000FFFF));
            writeData.append((writeValue & 0xFFFF0000) >> 16);
        }
        stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleRegisters, pageModeHrAddr, writeSize, writeData);
        m_communication->setModbusWriteCommand(command);
        modbusGetPageMode(pageNum, axisIndex);
    }
}

void SimpleSetViewModel::modbusGetPageMode(int pageNum, int axisIndex)
{
    if (!m_communication) return;
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return;
    }
    qDebug("%s:%d pageNum[%d] axisIndex[%d]", __func__, __LINE__, pageNum, axisIndex);
    int pageTopIndex = pageTopTableIndex[pageNum - 1];
    if (pageTopIndex >= 0){
        stModBusCommunicationData readCommand;
        quint16 pageModeHrAddr = (quint16)SIMPLE_SET_PARAM_ADDR_LIST.at(pageTopIndex).value(axisIndex);
        quint16 readSize = SIMPLE_SET_PARAM_ADDR_SIZE_LIST.at(pageTopIndex).value(axisIndex);
        readCommand = ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadHoldingRegisters, pageModeHrAddr, readSize);
        setOneShotModbusCommand(readCommand);
    }
}

void SimpleSetViewModel::getPageValues(int pageNum, int axisIndex, bool isHR)
{
    //axisIndex = ((axisIndex == 3) ? 2 : ((axisIndex == 2) ? 3 : axisIndex));
    qDebug("%s:%d pageNum[%d] axisIndex[%d] isHR[%d]", __func__, __LINE__, pageNum, axisIndex, isHR);
    if (isHR){
        getPageValuesHR(pageNum, axisIndex);
    } else {
        getPageValuesSR(pageNum, axisIndex);
    }
}

int SimpleSetViewModel::getDispFormat(int pageNum, int itemIndex, int axisIndex)
{
    if (pageNum <= 0 || pageNum > SIMPLE_SET_DATA_MAX_PAGE_NUM){
        return -1;
    }
    if (itemIndex < 0 || itemIndex >= pageItemNum[pageNum - 1]){
        return -1;
    }
    if (axisIndex < 0 || axisIndex > 7){
        return -1;
    }
    //axisIndex = ((axisIndex == 3) ? 2 : ((axisIndex == 2) ? 3 : axisIndex));
    int tableTopIndex =  pageTopTableIndex[pageNum - 1];
    if (tableTopIndex >= 0){
        return m_axisInfoDataDispFormat.at(tableTopIndex + itemIndex).value(axisIndex);
    } else {
        return m_axisInfoDataDispFormat.at(15 + itemIndex).value(axisIndex);
    }
}

QString SimpleSetViewModel::modbusGetMaxTorqueValue(int axisIndex, int torque)
{
    QString result = "";
    if (!m_resource) {
        return result;
    }
    result = m_resource->formatData(
                (short)torque,
                m_axisInfoDataDispFormat.at(SIMPLE_SET_PARAM_MAX_TORQUE_INDEX).value((int)axisIndex), 2);
    return result;
}

void SimpleSetViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerFunctionMachineDrvParamSimpleSetPageModel){
        //qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }
    int tableTopIndex =  pageTopTableIndex[mCurPageNum - 1];
    qDebug("%s:%d command.readAddress[%d] command.funcCode[%d] mCurPageNum[%d] tableTopIndex[%d]",
        __func__, __LINE__, command.readAddress, command.funcCode, mCurPageNum, tableTopIndex);
    if (tableTopIndex >= 0){
        if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters){
            updateModbusValuesHR(command);
        } else if (command.funcCode == QModbusPdu::FunctionCode::ReadInputRegisters) {
            updateModbusValuesSR(command);
        }
    } else {
        if (command.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(16).value(mCurAxisIndex)
            || command.readAddress == SIMPLE_SET_PARAM_ADDR_LIST.at(17).value(mCurAxisIndex)){
            if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters){
                updateModbusValuesHR(command);
            } else if (command.funcCode == QModbusPdu::FunctionCode::ReadInputRegisters){
                updateModbusValuesSR(command);
            }
        }
    }
}

void SimpleSetViewModel::updateSimpleSet(stModBusCommunicationData command)
{
    if (!m_modbus || !m_communication) return;
    if (command.funcCode == QModbusPdu::FunctionCode::ReadInputRegisters
        && command.readAddress == SIMPLE_SET_MODBUS_READ_TOP_SR){
        //qDebug("%s:%d funcCode[%d] readAddress[%d] size[%d]", __func__, __LINE__, command.funcCode, command.readAddress, command.comSize);
        stopModbusMonitoring();
        m_communication->resetPageReadCommandList(Enums::Pages::FunctionMPDrvParamSimpleSet);
    }
}

void SimpleSetViewModel::onDataSaveFinished()
{
    qDebug("%s:%d m_saveStatus[%d]", __func__, __LINE__, m_saveStatus);
    mIsSaveEnd = true;
    disconnect(m_communication, &CommunicationThread::finish, this, &SimpleSetViewModel::onDataSaveFinished);
}

#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
void SimpleSetViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void SimpleSetViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}

void SimpleSetViewModel::setCurAxisInfo(int axisIndex)
{
    if (axisIndex < 0 || axisIndex > 7){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return;
    }
    //int realAxisIndex = ((axisIndex == 3) ? 2 : ((axisIndex == 2) ? 3 : axisIndex));
    qDebug("%s:%d axisIndex[%d] m_currentAxisIndex[%d]", __func__, __LINE__, axisIndex, m_currentAxisIndex);
    mCurrentAxisIndex = axisIndex;
    //mModbusAxisIndex = realAxisIndex;
    mModbusAxisIndex = axisIndex;
    m_currentAxisIndex = axisIndex;
    emit currentAxisIndexChanged(m_currentAxisIndex);
    modbusSetAxisInfo(mModbusAxisIndex);
}

void SimpleSetViewModel::modbusSetAxisInfo(int axisIndex)
{
    if (!m_modbus || !m_communication) return;
    if (axisIndex < 0 || axisIndex > 7){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return;
    }
    connect(m_modbus, &ModbusManager::endModbusAxisWriteReady, this, &SimpleSetViewModel::endModbusAxisWriteReady, Qt::DirectConnection);
    QList<int> writeData;
    writeData.append(axisIndex);
    stModBusCommunicationData writeCommand =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister, SIMPLE_SET_AXIS_INFO_HR, 1, writeData);
    m_communication->setModbusWriteCommand(writeCommand);
}

void SimpleSetViewModel::startAxisChanging()
{
    setAxisDataLoaded(false);
    m_axisChanging = true;
}

void SimpleSetViewModel::startPageScroll()
{
    qDebug("%s:%d m_pageScrolled[%d] m_pageScroll[%d]", __func__, __LINE__, m_pageScrolled, m_pageScroll);
    setPageScrolled(false);
    m_pageScroll = true;
    timerStart(25);
}

void SimpleSetViewModel::timerStart(int timeOut)
{
    if (!mIsTimerStatus) {
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void SimpleSetViewModel::timerStop()
{
    if (mIsTimerStatus) {
        killTimer(mTimerId);
        mPageScrollTimerCnt = 0;
        mIsTimerStatus = false;
    }
}

void SimpleSetViewModel::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent) return;
    mPageScrollTimerCnt++;
    if (mPageScrollTimerCnt >= 1){
        m_pageScroll = false;
        timerStop();
        qDebug("%s:%d mPageScrollTimerCnt[%d] m_pageScroll[%d]", __func__, __LINE__, mPageScrollTimerCnt, m_pageScroll);
    }
}

void SimpleSetViewModel::setAxisDataLoaded(bool axisDataLoaded)
{
    if(m_axisDataLoaded != axisDataLoaded){
        m_axisDataLoaded = axisDataLoaded;

        emit axisDataLoadedChanged(m_axisDataLoaded);
    }
}

void SimpleSetViewModel::setPageScrolled(bool pageScrolled)
{
    if(m_pageScrolled != pageScrolled){
        m_pageScrolled = pageScrolled;

        emit pageScrolledChanged(m_pageScrolled);
    }
}

void SimpleSetViewModel::endModbusAxisWriteReady(QList<int> writeData)
{
    if (!m_modbus) return;
    qDebug("%s:%d mModbusAxisIndex[%d] writeData[0]=[%d]", __func__, __LINE__, mModbusAxisIndex, writeData[0]);
    if (writeData.length() == 1 && writeData[0] == mModbusAxisIndex){
        qDebug("%s:%d m_axisChanging[%d]", __func__, __LINE__, m_axisChanging);
        m_axisChanging = false;
        disconnect(m_modbus, &ModbusManager::endModbusAxisWriteReady, this, &SimpleSetViewModel::endModbusAxisWriteReady);
    }
}

void SimpleSetViewModel::onEndModbusPageWriteReady(QList<int> writeData)
{
    if(writeData.length() == 3
        && (writeData[0] == 13 && writeData[1] == 0 && writeData[2] == 0)){
        qDebug("%s:%d m_pageChanging[%d]", __func__, __LINE__, m_pageChanging);
        disconnect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &SimpleSetViewModel::onEndModbusPageWriteReady);
        m_pageChanging = false;
    }
}

void SimpleSetViewModel::onFinished()
{
    if (mIsSaveStart){
        //qDebug("%s:%d m_saveStatus[%d] mIsSaveEnd[%d]", __func__, __LINE__, m_saveStatus, mIsSaveEnd);
        mTimerCnt++;
        if (mTimerCnt >= 25 && mIsSaveEnd){
            setSaveStatus(2);
            mTimerCnt = 0;
            mIsSaveStart = false;
            mIsSaveEnd = false;
        }
    }
    if (!m_modbus || !m_resource) return;
    //qDebug("%s:%d mCurPageNum[%d] mCurrentAxisIndex[%d] m_pageChanging[%d] m_dataLoaded[%d] m_pageScroll[%d] m_pageScrolled[%d]",
    //    __func__, __LINE__, mCurPageNum, mCurrentAxisIndex, m_pageChanging, m_dataLoaded, m_pageScroll, m_pageScrolled);
    if(((m_axisChanging == false && m_modbus->getIsAxisWriteReady() == false) && m_pageScroll == false)
        && (m_pageChanging == false && m_modbus->getIsPageWriteReady() == false)){
        switch(mCurPageNum){
            case 1:{
                int tableTopIndex =  pageTopTableIndex[mCurPageNum - 1];
                SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurrentAxisIndex);
                for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                    quint16 readSize = SIMPLE_SET_COMMON_PARAM_ADDR_SIZE_LIST.at(mCurPageNum - 1).value(i);
                    QString setValues = "";
                    int readData = 0;
                    if (readSize == 2){
                        readData = m_modbus->getLongReceiveDataRG(SIMPLE_SET_COMMON_PARAM_ADDR_LIST.at(mCurPageNum - 1).value(i));
                        setValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(tableTopIndex + i).value((int)axis));
                    } else {
                        // NOP
                    }
                    if (i == 0){
                        m_simpleSetAxisInfo->setPage1Mode(readData);
                    } else if (i == 1){
                        m_simpleSetAxisInfo->setPage1SetValue1(axis, setValues);
                    } else if (i == 2){
                        m_simpleSetAxisInfo->setPage1SetValue2(axis, setValues);
                    } else if (i == 3){
                        m_simpleSetAxisInfo->setPage1SetValue3(axis, setValues);
                    }
                }
                break;
            }
            case 2:{
                int tableTopIndex =  pageTopTableIndex[mCurPageNum - 1];
                SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurrentAxisIndex);
                for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                    quint16 readSize = SIMPLE_SET_COMMON_PARAM_ADDR_SIZE_LIST.at(mCurPageNum - 1).value(i);
                    QString anyValues = "";
                    int readData = 0;
                    if (readSize == 2){
                        readData = m_modbus->getLongReceiveDataRG(SIMPLE_SET_COMMON_PARAM_ADDR_LIST.at(mCurPageNum - 1).value(i));
                        anyValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(tableTopIndex + i).value((int)axis));
                    } else {
                        // NOP
                    }
                    if (i == 0){
                        m_simpleSetAxisInfo->setPage2Mode(axis, readData);
                    } else if (i == 1){
                        m_simpleSetAxisInfo->setPage2AnyValue1(axis, anyValues);
                    } else if (i == 2){
                        m_simpleSetAxisInfo->setPage2AnyValue2(axis, anyValues);
                    } else if (i == 3){
                        m_simpleSetAxisInfo->setPage2AnyValue3(axis, anyValues);
                    } else if (i == 4){
                        m_simpleSetAxisInfo->setPage2AnyValue4(axis, anyValues);
                    } else if (i == 5){
                        m_simpleSetAxisInfo->setPage2AnyValue5(axis, anyValues);
                    } else if (i == 6){
                        m_simpleSetAxisInfo->setPage2AnyValue6(axis, anyValues);
                    }
                    continue;
                }
                break;
            }
            case 3:{
                int tableTopIndex =  pageTopTableIndex[mCurPageNum - 1];
                SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurrentAxisIndex);
                for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                    quint16 readSize = SIMPLE_SET_COMMON_PARAM_ADDR_SIZE_LIST.at(mCurPageNum - 1).value(i);
                    QString setValues = "";
                    int readData = 0;
                    if (readSize == 2){
                        readData = m_modbus->getLongReceiveDataRG(SIMPLE_SET_COMMON_PARAM_ADDR_LIST.at(mCurPageNum - 1).value(i));
                        setValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(tableTopIndex + i).value((int)axis));
                    } else {
                        // NOP
                    }
                    if (i == 0){
                        m_simpleSetAxisInfo->setPage3Mode(axis, readData);
                    } else if (i == 1){
                        m_simpleSetAxisInfo->setPage3SetValue1(axis, setValues);
                    } else if (i == 2){
                        m_simpleSetAxisInfo->setPage3SetValue2(axis, setValues);
                    } else if (i == 3){
                        m_simpleSetAxisInfo->setPage3SetValue3(axis, setValues);
                    }
                    continue;
                }
                break;
            }
            case 4:{
                //int tableTopIndex =  pageTopTableIndex[mCurPageNum - 1];
                SimpleSetAxisInfo::SimpleSetAxis axis = static_cast<SimpleSetAxisInfo::SimpleSetAxis>(mCurrentAxisIndex);
                for(int i = 0; i < pageItemNum[mCurPageNum - 1]; i++){
                    quint16 readSize = SIMPLE_SET_COMMON_PARAM_ADDR_SIZE_LIST.at(mCurPageNum - 1).value(i);
                    if (i == 0){
                        //最大トルク変化量
                        QString anyValues = "";
                        if (readSize == 2){
                            int readData = m_modbus->getLongReceiveDataRG(SIMPLE_SET_COMMON_PARAM_ADDR_LIST.at(mCurPageNum - 1).value(i));
                            anyValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(15 + i).value((int)axis));
                        } else {
                            // NOP
                        }
                        m_simpleSetAxisInfo->setPage4AnyValue1(axis, anyValues);
                    } else if (i == 1){
                        QString setValues = "";
                        if (readSize == 2){
                            int readData = m_modbus->getLongReceiveDataRG(SIMPLE_SET_COMMON_PARAM_ADDR_LIST.at(mCurPageNum - 1).value(i));
                            setValues = m_resource->formatData(readData, m_axisInfoDataDispFormat.at(15 + i).value((int)axis));
                        } else {
                            // NOP
                        }
                        m_simpleSetAxisInfo->setPage4SetValue1(axis, setValues);
                    }
                    continue;
                }
                break;
            }
            default:
                break;
        }
        setAxisDataLoaded(true);
        setPageScrolled(true);
        setDataLoaded(true);
    }

    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC

