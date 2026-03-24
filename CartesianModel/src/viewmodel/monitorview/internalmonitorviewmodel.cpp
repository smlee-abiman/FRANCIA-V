#include "internalmonitorviewmodel.h"

InternalMonitorData::InternalMonitorData(QObject *parent)
    : QObject(parent)
{
    m_value = 0;
}

InternalMonitorData::~InternalMonitorData(){

}

void InternalMonitorData::setValue(bool value)
{
    if(m_value != value){
        m_value = value;
        emit valueChanged(m_value);
    }
}

InternalMonitorViewModel::InternalMonitorViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    initMonitorData();
}

InternalMonitorViewModel::~InternalMonitorViewModel()
{

}

void InternalMonitorViewModel::onActivate()
{
    //initPageCommand();
    startModbusMonitoring();
    connect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &InternalMonitorViewModel::onEndModbusPageWriteReady, Qt::DirectConnection);
}

void InternalMonitorViewModel::onDeactivate()
{
    stopModbusMonitoring();
    //deinitPageCommand();
}

void InternalMonitorViewModel::initMonitorData()
{
    if ((m_curBankNo < 0 || m_curBankNo > 4)
        || (m_curBankContents < 0 || m_curBankContents > 1)){
        qCritical("%s:%d m_curBankNo[%d] or m_curBankContents[%d] is invalid",
            __func__, __LINE__, m_curBankNo, m_curBankContents);
        return;
    }
    for(int i = 0; i < bankIOModbusPageCmd[m_curBankNo][m_curBankContents].size; i++){
        InternalMonitorData *data = new InternalMonitorData(this);
        m_bankIOList.append(data);
    }
}

void InternalMonitorViewModel::initPageCommand()
{
    if (!m_communication) {
        qCritical("%s:%d m_communication is invalid", __func__, __LINE__);
        return;
    }
    stModBusCommunicationData readCommand[INTERNAL_PERIODIC_MONITOR_CMD_LIST_NUM];
    for(int i = 0; i < INTERNAL_PERIODIC_MONITOR_CMD_LIST_NUM; i++){
        readCommand[i] = ModbusCommandGenerator::makeModbusReadCommand(
                            initModbusPageCmd[i].funcCode,
                            initModbusPageCmd[i].address,
                            initModbusPageCmd[i].size
                            );
        m_communication->setPageReadCommandList(Enums::Pages::MonitorInternal, readCommand[i]);
    }
}

void InternalMonitorViewModel::deinitPageCommand()
{
    if (!m_communication) return;
    m_communication->resetPageReadCommandList(Enums::Pages::MonitorInternal);
}

void InternalMonitorViewModel::updatePageCommand()
{
    //deinitPageCommand();
    if ((m_curBankNo < 0 || m_curBankNo > 4)
        || (m_curBankContents < 0 || m_curBankContents > 1)){
        qCritical("%s:%d m_curBankNo[%d] or m_curBankContents[%d] is invalid",
            __func__, __LINE__, m_curBankNo, m_curBankContents);
        return;
    }
    QList<int> writeData;
    writeData.append(bankOffsetNumber[m_curBankNo][m_curBankContents]);
    stModBusCommunicationData writeCommand =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, INTERNAL_BANK_OFFSET_NUMBER_HR, 1, writeData);
    m_communication->setModbusWriteCommand(writeCommand);
}

void InternalMonitorViewModel::startModbusMonitoring()
{
    if (!m_communication) return;
    connect(m_communication, &CommunicationThread::finish,
        this, &InternalMonitorViewModel::onFinished, Qt::QueuedConnection);
}

void InternalMonitorViewModel::stopModbusMonitoring()
{
    if (!m_communication) return;
    disconnect(m_communication, &CommunicationThread::finish,
        this, &InternalMonitorViewModel::onFinished);
}

void InternalMonitorViewModel::setCurBankNo(int curBankNo)
{
    if (m_curBankNo == curBankNo)
        return;

    m_curBankNo = curBankNo;
    updatePageCommand();
    emit curBankNoChanged(m_curBankNo);
}

void InternalMonitorViewModel::setCurBankContents(int curBankContents)
{
    if (m_curBankContents == curBankContents)
        return;

    m_curBankContents = curBankContents;
    updatePageCommand();
    emit curBankContentsChanged(m_curBankContents);
}

void InternalMonitorViewModel::setBankIOList(QList<QObject *>& bankIOList)
{
    if (m_bankIOList != bankIOList) {
        m_bankIOList = bankIOList;
        emit bankIOListChanged(m_bankIOList);
    }
}

void InternalMonitorViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

int InternalMonitorViewModel::getBankOffsetNum()
{
    if ((m_curBankNo < 0 || m_curBankNo > 4)
        || (m_curBankContents < 0 || m_curBankContents > 1)){
        qCritical("%s:%d m_curBankNo[%d] or m_curBankContents[%d] is invalid",
            __func__, __LINE__, m_curBankNo, m_curBankContents);
        return -1;
    }
    return bankOffsetNumber[m_curBankNo][m_curBankContents];
}

void InternalMonitorViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;

        emit dataLoadedChanged(m_dataLoaded);
    }
}

void InternalMonitorViewModel::onFinished()
{
    if (!m_modbus || !m_resource) return;
    if ((m_curBankNo < 0 || m_curBankNo > 4)
        || (m_curBankContents < 0 || m_curBankContents > 1)){
        qCritical("%s:%d m_curBankNo[%d] or m_curBankContents[%d] is invalid",
            __func__, __LINE__, m_curBankNo, m_curBankContents);
        return;
    }
    if(m_pageChanging == false && m_modbus->getIsPageChanging() == false){
        int ioTopSRAddr = INTERNAL_BANK_IO_TOP_SR;
        for(int col = 0; col < 16; col++){
            for(int row = 0; row < 16; row++){
                InternalMonitorData *data = static_cast<InternalMonitorData *>(m_bankIOList.at(col * 16 + row));
                data->setValue(
                    ((((m_modbus->getWordReceiveDataRG(ioTopSRAddr + col)) & (1 << (15 - row)))
                        >> (15 - row)) == 0x01 ? true : false)
                );
            }
        }//bits parsing

        for(int col = 0; col < 16; col++){
            quint16 data1 = m_modbus->getWordReceiveDataRG(ioTopSRAddr + col);
            quint8 index1 = (data1 >> 8) & 0xFF;
            quint8 index2 = data1 & 0xFF;
            //qDebug() << "data1::::" << data1 << "index1:::" << index1 << "index2" << index2;
            m_bankIOByteList.replace((col*2),index1);
            m_bankIOByteList.replace((col*2+1),index2);
        }//bytes parsing
        emit bankIOByteListChanged(m_bankIOByteList);
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

void InternalMonitorViewModel::onEndModbusPageWriteReady(QList<int> writeData)
{
    if(writeData.length() == 3
        && (writeData[0] == 5 && (writeData[1] >= 0 && writeData[1] <= 9) && writeData[2] == 0)){
        qDebug("%s:%d m_curBankNo[%d] or m_curBankContents[%d] writeData[1]=%d", __func__, __LINE__, m_curBankNo, m_curBankContents, writeData[1]);
        qDebug("%s:%d m_pageChanging[%d]", __func__, __LINE__, m_pageChanging);
        disconnect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &InternalMonitorViewModel::onEndModbusPageWriteReady);
        m_pageChanging = false;
    }
}
