#include "abssetviewmodel.h"

AbsData::AbsData(short resetKeyNo, short setKeyNo, QObject *parent)
    : QObject(parent)
{
    m_encoder = 0;
    m_bm = false;
    m_alert = false;
    m_resetKeyNo = resetKeyNo;
    m_setKeyNo = setKeyNo;
}

AbsData::~AbsData(){

}

void AbsData::setEncoder(int encoder)
{
    if(m_encoder != encoder){
        m_encoder = encoder;

        emit encoderChanged(m_encoder);
    }
}

void AbsData::setBm(bool bm)
{
    if(m_bm != bm){
        m_bm = bm;

        emit bmChanged(m_bm);
    }
}

void AbsData::setAlert(bool alert)
{
    if(m_alert != alert){
        m_alert = alert;

        emit alertChanged(m_alert);
    }
}

AbsSetViewModel::AbsSetViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    if (m_modbusManager){
        //qDebug() << "IOMonitorViewModel:m_modbusManager is share";
    }

    m_commonView = commonView;

    m_pageNum = 0;
    m_pageMaxNum = 2;

    for(int i = 0; i < 8; i++){
        m_dataList[i] = new AbsData(abs_reset_key[i], abs_set_key[i]);
        //m_dataList[i] = new AbsData(81+i, 89+i);
        QQmlEngine::setObjectOwnership(m_dataList[i], QQmlEngine::CppOwnership);
    }
}

AbsSetViewModel::~AbsSetViewModel()
{

}

void AbsSetViewModel::onActivate()
{
    startMonitoring();
}

void AbsSetViewModel::onDeactivate()
{
    stopMonitoring();
}

QList<QObject*> AbsSetViewModel::getDataList(int page)
{
    QList<QObject*> list;

    QList<int> axisOrder = m_commonView->getAxisOrder();

    if(page == 0){
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

QList<QObject*> AbsSetViewModel::getDataAllList()
{
    QList<QObject*> list;

    QList<int> axisOrder = m_commonView->getAxisOrder();

    for(int i = 0; i < 8; i++){
        if(i < axisOrder.length()){
            list.append(m_dataList[axisOrder[i]]);
        }
    }

    return list;
}

void AbsSetViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &AbsSetViewModel::onFinished);
}
void AbsSetViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &AbsSetViewModel::onFinished);
}

void AbsSetViewModel::setPageNum(int pageNum)
{
    m_pageNum = pageNum;

    emit pageNumChanged(m_pageNum);
}

void AbsSetViewModel::onFinished(){

    for(int i = 0; i < 8; i++){
        if(i == 2){
            m_dataList[i]->setEncoder(m_modbusManager->getWordReceiveDataRG(2761+i));
            m_dataList[i]->setBm(m_modbusManager->getReceiveDataDI(2655-i));
            m_dataList[i]->setAlert(m_modbusManager->getReceiveDataDI(2588));
        }
        else if(i == 3){
            m_dataList[i]->setEncoder(m_modbusManager->getWordReceiveDataRG(2761+i));
            m_dataList[i]->setBm(m_modbusManager->getReceiveDataDI(2655-i));
            m_dataList[i]->setAlert(m_modbusManager->getReceiveDataDI(2589));
        }
        else{
            m_dataList[i]->setEncoder(m_modbusManager->getWordReceiveDataRG(2761+i));
            m_dataList[i]->setBm(m_modbusManager->getReceiveDataDI(2655-i));
            m_dataList[i]->setAlert(m_modbusManager->getReceiveDataDI(2591-i));
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
