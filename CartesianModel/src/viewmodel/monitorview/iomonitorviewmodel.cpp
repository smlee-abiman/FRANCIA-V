#include "iomonitorviewmodel.h"

IOMonitorData::IOMonitorData(int index, QObject *parent)
    : QObject(parent)
{
    m_index = index;
    m_value = 0;
}

IOMonitorData::~IOMonitorData(){

}

void IOMonitorData::setValue(bool value)
{
    if(m_value != value){
        m_value = value;

        emit valueChanged(m_value);
    }
}


IOMonitorViewModel::IOMonitorViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    int maxPage  = m_resource->getPenSetIntValue("8", 5, 0);
    m_pageMaxNum = (maxPage / 3) + (0 < maxPage % 3 ? 1 : 0);
    m_pageNum = 0;

    readPensetData();
}

IOMonitorViewModel::~IOMonitorViewModel()
{

}

void IOMonitorViewModel::onActivate()
{
    startMonitoring();
}

void IOMonitorViewModel::onDeactivate()
{
    stopMonitoring();
}

void IOMonitorViewModel::readPensetData()
{
    const int pageMax = 6;
    const int pageItemMax = 18;

    for(int item = 0; item < pageMax * pageItemMax; item++){
        int index = -1;
        QString indexStr = m_resource->getPenSetStringValue("8", 11 + item, 0);
        if(0 < indexStr.length() && indexStr != "-"){
            index = m_resource->getPenSetIntValue("8", 11 + item, 0);
        }

        m_dataList[item/18][item%18] = new IOMonitorData(index);
    }
}

void IOMonitorViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &IOMonitorViewModel::onFinished);
}
void IOMonitorViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &IOMonitorViewModel::onFinished);
}

void IOMonitorViewModel::setPageNum(int pageNum)
{
    m_pageNum = pageNum;

    emit pageNumChanged(m_pageNum);
}

QList<QObject*> IOMonitorViewModel::getDataList(int page)
{
    const int pageItemMax = 18;
    QList<QObject*> list;

    for(int i = 0; i < pageItemMax; i++){
        list.append(m_dataList[page][i]);
    }

    return list;
}

void IOMonitorViewModel::onFinished(){
    const int pageItemMax = 18;

    for(int page = 0; page < m_pageMaxNum; page++){
        for(int item = 0; item < pageItemMax; item++){
            IOMonitorData *data = m_dataList[page][item];

            if(data->index() != -1){
                data->setValue(m_modbusManager->getReceiveDataDI(data->index()));
            }
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







