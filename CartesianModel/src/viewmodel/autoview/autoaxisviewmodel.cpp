#include "autoaxisviewmodel.h"

AutoAxisViewModel::AutoAxisViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{

}

AutoAxisViewModel::~AutoAxisViewModel()
{

}

void AutoAxisViewModel::startMonitoring()
{
    m_pageIng = true;
    if (!m_communication){
        //qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &AutoAxisViewModel::onFinished);
    connect(m_modbusManager, &ModbusManager::endModbusPageWriteReady, this, &AutoAxisViewModel::endModbusPageWriteReady, Qt::DirectConnection);
}
void AutoAxisViewModel::stopMonitoring()
{
    m_pageIng = false;
    if (!m_communication){
        //qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &AutoAxisViewModel::onFinished);
    disconnect(m_modbusManager, &ModbusManager::endModbusPageWriteReady, this, &AutoAxisViewModel::endModbusPageWriteReady);
}

void AutoAxisViewModel::startPageChanging()
{
    m_pageChanging = true;
}

void AutoAxisViewModel::onFinished(){
    if(m_pageChanging == false && m_modbusManager->getIsPageChanging() == false){
        for(int i = 0; i < AXIS_MAX_NUM; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString encoder = m_resource->formatData((short)m_modbusManager->getWordReceiveDataRG(AUTO_AXIS_DATA_ENCODER_TOP_RG + i), 64);
            QString torque = m_resource->formatData((short)m_modbusManager->getWordReceiveDataRG(AUTO_AXIS_DATA_TORQUE_TOP_RG + i), 1328,2);
            m_axisInfo->setEncoder(axis, encoder);
            m_axisInfo->setTorque(axis,torque);
        }
    }
}

void AutoAxisViewModel::endModbusPageWriteReady(QList<int> writeData)
{
    if ((writeData.size() == 3) &&
        ((writeData[0] == 2) && (writeData[1] == 0) && (writeData[2] == 0))) {
        m_pageChanging = false;
    }
}
