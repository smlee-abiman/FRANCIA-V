#include <QQmlEngine>
#include "originviewmodel.h"

#define ENABLE_ORIGIN_STATUS_FROM_COMMON_MODBUS

OriginViewModel::OriginViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    qmlRegisterType<OriginViewModel>("OriginViewModel", 1, 0, "OriginViewModel");
    initSettingPenSetData();
}

OriginViewModel::OriginViewModel(QObject *object)
    : ViewModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

OriginViewModel::~OriginViewModel()
{

}

void OriginViewModel::onActivate()
{
    startModbusMonitoring();
//    setOperationMode();
    connect(m_modbus, &ModbusManager::endModbusPageChange, this, &OriginViewModel::onEndModbusPageChange, Qt::DirectConnection);
}

void OriginViewModel::onDeactivate()
{
    if (m_pageChanging) { disconnect(m_modbus, &ModbusManager::endModbusPageChange, this, &OriginViewModel::onEndModbusPageChange); }
    stopModbusMonitoring();
}

void OriginViewModel::initSettingPenSetData()
{
    m_axisInfoDataFormat = m_resource->formatList(5, 8, 8);
#if 0
    for(int i = 0; i < 8; i++){
        qDebug() << "value(0):" << m_axisInfoDataFormat.at(i).value(0) << "value(1):" << m_axisInfoDataFormat.at(i).value(1)
                 << "value(2):" << m_axisInfoDataFormat.at(i).value(2) << "value(3):" << m_axisInfoDataFormat.at(i).value(3);
        qDebug() << "value(4):" << m_axisInfoDataFormat.at(i).value(4) << "value(5):" << m_axisInfoDataFormat.at(i).value(5)
                 << "value(6):" << m_axisInfoDataFormat.at(i).value(6) << "value(7):" << m_axisInfoDataFormat.at(i).value(7);
    }
#endif
}

void OriginViewModel::startModbusMonitoring()
{
    if (!m_communication) return;
    connect(m_communication, &CommunicationThread::finish,
        this, &OriginViewModel::onFinished, Qt::QueuedConnection);
}

void OriginViewModel::stopModbusMonitoring()
{
    if (!m_communication) return;
    disconnect(m_communication, &CommunicationThread::finish,
        this, &OriginViewModel::onFinished);
}

void OriginViewModel::initPageCommand()
{
    if (!m_communication) {
        qDebug("%s:%d m_communication is invalid", __func__, __LINE__);
        return;
    }
    stModBusCommunicationData readCommand[ORIGIN_PERIODIC_MONITOR_CMD_LIST_NUM];
    for(int i = 0; i < ORIGIN_PERIODIC_MONITOR_CMD_LIST_NUM; i++){
        readCommand[i] = ModbusCommandGenerator::makeModbusReadCommand(
                            initModbusPageCmd[i].funcCode,
                            initModbusPageCmd[i].address,
                            initModbusPageCmd[i].size
                            );
        m_communication->setPageReadCommandList(Enums::Pages::Origin, readCommand[i]);
    }
}

void OriginViewModel::setOriginStatus(int originStatus)
{
    if (m_originStatus == originStatus)
        return;

    m_originStatus = originStatus;
    emit originStatusChanged(m_originStatus);
}

int OriginViewModel::getAxisCount()
{
    return m_axisInfoDataFormat.count();
}

void OriginViewModel::startOriginCarriageMonitor()
{
#ifdef REAL_TARGET
    if (!m_communication) return;
    
    qDebug("%s:%d m_originStatus[%d]", __func__, __LINE__, m_originStatus);
    if (m_originStatus == (int)OriginStatusPauseCarriage){
        setOriginStatus((int)OriginStatusResumeCarriage);
    }
#ifdef ENABLE_ORIGIN_STATUS_FROM_COMMON_MODBUS
#else // ENABLE_ORIGIN_STATUS_FROM_COMMON_MODBUS
    modBusReadOriginStatus();
#endif // ENABLE_ORIGIN_STATUS_FROM_COMMON_MODBUS
    timerStart(500);
#else
    setOriginStatus((int)OriginStatusCarriaging);
#endif
}

void OriginViewModel::pauseOriginCarriageMonitor()
{
    setOriginStatus((int)OriginStatusPauseCarriage);
}

void OriginViewModel::stopOriginCarriageMonitor()
{
    timerStop();
#if 1
#else
    modBusClearOriginStatus();
#endif
    setOriginStatus((int)OriginStatusBeforeCarriage);
}

void OriginViewModel::setOperationMode()
{
    if (!m_communication) return;
    QList<int> writeData;
    writeData.append(0x00);
    stModBusCommunicationData writeCommand =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteSingleCoil, ORIGIN_OPERATION_MODE_DO, 0x0001, writeData);
    m_communication->setOneShotModbusCommand(writeCommand);
}

void OriginViewModel::startPageChanging()
{
    m_pageChanging = true;
}

void OriginViewModel::modBusReadOriginStatus()
{
    if (!m_communication) return;

    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadDiscreteInputs, ORIGIN_CARRIAGE_FINISH_STATUS_DI, 2);
    m_communication->setOneShotModbusCommand(readCommand);
}

void OriginViewModel::modBusClearOriginStatus()
{
    if (!m_communication) return;
    m_communication->removePageReadCommandList(Enums::Pages::Origin, ORIGIN_ONESHOT_MONITOR_ANIMATION_STATUS_INDEX);
}

void OriginViewModel::judgeOriginCarriage()
{
    if (!m_modbus) {
        qDebug("m_modbus is Null");
        return;
    }
    switch(m_originStatus){
    case OriginStatusBeforeCarriage:
//        qDebug("%s:%d m_originStatus[%d] DI%d:%d", __func__, __LINE__, m_originStatus, ORIGIN_CARRIAGE_STATUS_DI, m_modbus->getReceiveDataDI(ORIGIN_CARRIAGE_STATUS_DI));
        if (m_modbus->getReceiveDataDI(ORIGIN_CARRIAGE_STATUS_DI)){
            setOriginStatus((int)OriginStatusCarriaging);
        }
        break;
    case OriginStatusCarriaging:
//        qDebug("%s:%d m_originStatus[%d] DI%d:%d", __func__, __LINE__, m_originStatus, ORIGIN_CARRIAGE_FINISH_STATUS_DI, m_modbus->getReceiveDataDI(ORIGIN_CARRIAGE_FINISH_STATUS_DI));
        if (m_modbus->getReceiveDataDI(ORIGIN_CARRIAGE_FINISH_STATUS_DI)){
            setOriginStatus((int)OriginStatusFinishCarriage);
        }
        break;
    case OriginStatusResumeCarriage:
//        qDebug("%s:%d m_originStatus[%d] DI%d:%d", __func__, __LINE__, m_originStatus, ORIGIN_CARRIAGE_FINISH_STATUS_DI, m_modbus->getReceiveDataDI(ORIGIN_CARRIAGE_FINISH_STATUS_DI));
        if (m_modbus->getReceiveDataDI(ORIGIN_CARRIAGE_FINISH_STATUS_DI)){
            setOriginStatus((int)OriginStatusFinishCarriage);
        }
        break;
    default:
        qDebug("%s:%d m_originStatus[%d]", __func__, __LINE__, m_originStatus);
        break;
    }
}

void OriginViewModel::timerStart(int timeOut)
{
    if (!mIsTimerStatus) {
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void OriginViewModel::timerStop()
{
    if (mIsTimerStatus) {
        killTimer(mTimerId);
        mIsTimerStatus = false;
    }
}

void OriginViewModel::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent) return;
    if(timerEvent->timerId() == mTimerId){
//        qDebug("%s:%d m_originStatus[%d]", __func__, __LINE__, m_originStatus);
        if (m_originStatus != (int)OriginStatusPauseCarriage){
            judgeOriginCarriage();
#ifdef ENABLE_ORIGIN_STATUS_FROM_COMMON_MODBUS
#else
            modBusReadOriginStatus();
#endif
        }
    }
}

void OriginViewModel::onFinished()
{
    if (!m_modbus || !m_axisInfo) return;
    // qDebug("%s:%d m_dataLoaded[%d] getIsPageChanging[%d] m_pageChanging[%d]",__func__, __LINE__, m_dataLoaded, m_modbus->getIsPageChanging(), m_pageChanging);

    if (m_pageChanging == false && m_modbus->getIsPageChanging() == false){
        for(int i = mCurTopAxisNumber; i < mCurTopAxisNumber + ORIGIN_ONE_PAGE_MAX; i++){
            quint16 index = (quint16)(i % ORIGIN_ONE_PAGE_MAX);
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(index);
            QString originPercent = m_resource->formatData(
                    (short)m_modbus->getWordReceiveDataRG(ORIGIN_AXIS_DATA_PERCENTAGE_TOP_RG + index), m_axisInfoDataFormat.at((int)axis).value(3));
            QString encoder = m_resource->formatData(
                    (short)m_modbus->getWordReceiveDataRG(ORIGIN_AXIS_DATA_ENCODER_TOP_RG + index), m_axisInfoDataFormat.at((int)axis).value(1));
            QString torque = m_resource->formatData(
                    (short)m_modbus->getWordReceiveDataRG(ORIGIN_AXIS_DATA_TORQUE_TOP_RG + index), m_axisInfoDataFormat.at((int)axis).value(2), 2);

            m_axisInfo->setOriginDatas(axis,
                m_modbus->getReceiveDataDI(GET_ORIGIN_AXIS_DATA_HOM_DI(index)),
                m_modbus->getReceiveDataDI(GET_ORIGIN_AXIS_DATA_OVR_DI(index)),
                m_modbus->getReceiveDataDI(GET_ORIGIN_AXIS_DATA_MB_DI(index)),
                m_modbus->getWordReceiveDataRG(ORIGIN_AXIS_DATA_PERCENTAGE_TOP_RG + index),
                originPercent, encoder, torque);

            if(m_resource->axisorder().size() > 5) {
                for(int i = 5; i < 8; i++){
                    AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
                    QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
                    m_axisInfo->setPosition(axis,position);
                }
            }
        }
    }
}

void OriginViewModel::onEndModbusPageChange(int mainScreenId, int subScreenId)
{
    if ((mainScreenId == 5) && subScreenId == 0) {
        //qDebug("%s:%d mainScreenId[%d] subScreenId[%d] m_pageChanging[%d]", __func__, __LINE__, mainScreenId, subScreenId, m_pageChanging);
        disconnect(m_modbus, &ModbusManager::endModbusPageChange, this, &OriginViewModel::onEndModbusPageChange);
        m_pageChanging = false;
    }
}
