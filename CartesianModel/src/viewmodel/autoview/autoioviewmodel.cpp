#include "autoioviewmodel.h"

AutoIOData::AutoIOData(int index, QObject *parent)
    : QObject(parent)
{
    m_index = index;
    m_value = 0;
}

AutoIOData::~AutoIOData(){

}

void AutoIOData::setValue(bool value)
{
    if(m_value != value){
        m_value = value;
        emit valueChanged(m_value);
    }
}

AutoIOViewModel::AutoIOViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    readPensetData();
}

AutoIOViewModel::~AutoIOViewModel(){

}

void AutoIOViewModel::readPensetData()
{
    for(int page = 0; page < 17; page++){
        for(int item = 0; item < 6; item++){
            int index = -1;
            QString indexStr = m_resource->getPenSetStringValue("7", 46 + item, page);
            if(indexStr.length() > 0){
                index = m_resource->getPenSetIntValue("7", 46 + item, page);
                AutoIOData *data = new AutoIOData(index);
                m_ioDataList.append(data);
            }else {
                continue;
            }
        }
    }
}

void AutoIOViewModel::startMonitoring()
{
    m_pageIng = true;
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &AutoIOViewModel::onFinished);
    connect(m_modbusManager, &ModbusManager::endModbusPageWriteReady, this, &AutoIOViewModel::endModbusPageWriteReady, Qt::DirectConnection);
}
void AutoIOViewModel::stopMonitoring()
{
    m_pageIng = false;
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &AutoIOViewModel::onFinished);
    disconnect(m_modbusManager, &ModbusManager::endModbusPageWriteReady, this, &AutoIOViewModel::endModbusPageWriteReady);
}

void AutoIOViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void AutoIOViewModel::onFinished(){
    if(m_pageChanging == false && m_modbusManager->getIsPageChanging() == false){
        for(int i = 0; i < m_ioDataList.length(); i++){
            AutoIOData *data = static_cast<AutoIOData *>(m_ioDataList.at(i));
            if(data->index() != -1){
                data->setValue(m_modbusManager->getReceiveDataDI(data->index()));
            }
        }
        setDataLoaded(true);
    }
}

void AutoIOViewModel::endModbusPageWriteReady(QList<int> writeData)
{
    if ((writeData.size() == 3) &&
        ((writeData[0] == 4) && (writeData[1] == 0) && (writeData[2] == 0))) {
        m_pageChanging = false;
    }
}

void AutoIOViewModel::setIoDataList(QList<QObject *> list)
{
    if(m_ioDataList != list){
        m_ioDataList = list;
        emit ioDataListChanged(m_ioDataList);
    }
}

void AutoIOViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}
