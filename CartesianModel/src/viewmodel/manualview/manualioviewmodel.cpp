#include "manualioviewmodel.h"

InputData::InputData(int index, QObject *parent)
    : QObject(parent)
{
    m_index = index;
    m_value = 0;
}

InputData::~InputData(){

}

void InputData::setValue(bool value)
{
    if(m_value != value){
        m_value = value;

        emit valueChanged(m_value);
    }
}

OutData::OutData(int keyNo, QObject *parent)
    : QObject(parent)
{
    m_keyNo = keyNo;
}

OutData::~OutData(){

}

OutDataSignal::OutDataSignal(int di, QString onImage, QString offImage, QObject *parent)
    : QObject(parent)
{
    m_di = di;
    m_onImage = onImage;
    m_offImage = offImage;
}

OutDataSignal::~OutDataSignal(){

}

void OutDataSignal::setValue(bool value)
{
    if(m_value != value){
        m_value = value;

        emit valueChanged(m_value);
    }
}

ManualIOViewModel::ManualIOViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    m_inPageMaxNum = m_resource->getPenSetIntValue("6", 5, 0);
    m_inPageNum = 0;
    m_outPageMaxNum = m_resource->getPenSetIntValue("6", 6, 0);
    m_outPageNum = 0;

    readPensetData();
}

ManualIOViewModel::~ManualIOViewModel()
{
    m_communication->resetPageReadCommandList(Enums::Pages::ManualIO);
}

void ManualIOViewModel::onActivate()
{
    startMonitoring();
}

void ManualIOViewModel::onDeactivate()
{
    stopMonitoring();
}

void ManualIOViewModel::readPensetData()
{
    const int inPageMax = 5;
    const int outPageMax = 12;
    const int inpageItemMax = 18;
    const int outpageItemMax = 10;
    const int outSignalpageItemMax = 5;

    for(int page = 0; page < inPageMax; page++){
        for(int item = 0; item < inpageItemMax; item++){
            int index = -1;
            int pensetRow = (page * (inpageItemMax/2)) + (item/2) + 231;
            QString indexStr = m_resource->getPenSetStringValue("6", pensetRow, item%2);
            if(0 < indexStr.length()){
                index = m_resource->getPenSetIntValue("6", pensetRow, item%2);
            }
            m_inDataList[page][item] = new InputData(index);
        }
    }//输入IO
    for(int page = 0; page < outPageMax; page++){
        for(int item = 0; item < outpageItemMax; item++){
            int knPenSetRow = (page * (outpageItemMax/2)) + (item/2) + 122;
            int keyNo = m_resource->getPenSetIntValue("6", knPenSetRow, item%2);
            int itemIdx = (item/2) + (item%2 == 0 ? 0 : 5);
            m_outDataList[page][itemIdx] = new OutData(keyNo);
        }
    }
    for(int page = 0; page < outPageMax; page++){
        for(int item = 0; item < outSignalpageItemMax; item++){
            int idxPenSetRow = (page * outSignalpageItemMax) + item + 61;
            QString diStr = m_resource->getPenSetStringValue("6", idxPenSetRow, 0);
            int di = -1;
            if(0 < diStr.length()){
                di = m_resource->getPenSetIntValue("6", idxPenSetRow, 0);
            }
            QString onImage = m_resource->getPenSetStringValue("6", idxPenSetRow, 1);
            QString offImage = m_resource->getPenSetStringValue("6", idxPenSetRow, 2);
            m_outDataSignalList[page][item] = new OutDataSignal(di,onImage,offImage);
        }
    }
//    for(int page = 0; page < outPageMax; page++){
//        for(int item = 0; item < pageItemMax; item++){
//            int di = -1;
//            int pensetRow = (page * (pageItemMax/3)) + (item/3) + 276;
//            int keyNo = m_resource->getPenSetIntValue("6", pensetRow, item%3);
//            QString diStr = m_resource->getPenSetStringValue("6", pensetRow, item%3+3);
//            if(0 < diStr.length()){
//                di = m_resource->getPenSetIntValue("6", pensetRow, item%3+3);
//            }
//            m_outDataList[page][item] = new OutData(keyNo,di);
//        }
//    }//输出IO
}

void ManualIOViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &ManualIOViewModel::onFinished);
}

void ManualIOViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &ManualIOViewModel::onFinished);
}

void ManualIOViewModel::pageInit(){
    setInPageNum(1);
    setOutPageNum(1);
}

void ManualIOViewModel::setInPageNum(int inPageNum)
{
    m_inPageNum = inPageNum;

    emit inPageNumChanged(m_inPageNum);
}

void ManualIOViewModel::setOutPageNum(int outPageNum)
{
    m_outPageNum = outPageNum;

    emit outPageNumChanged(m_outPageNum);
}

QList<QObject*> ManualIOViewModel::getInDataList(int page)
{
    QList<QObject*> list;

    for(int i = 0; i < 18; i++){
        list.append(m_inDataList[page][i]);
    }

    return list;
}

QList<QObject*> ManualIOViewModel::getOutDataList(int page)
{
    QList<QObject*> list;

    for(int i = 0; i < 10; i++){
        list.append(m_outDataList[page][i]);
    }
    return list;
}

QList<QObject*> ManualIOViewModel::getOutDataSignalList(int page)
{
    QList<QObject*> list;

    for(int i = 0; i < 5; i++){
        list.append(m_outDataSignalList[page][i]);
    }
    return list;
}

void ManualIOViewModel::onFinished(){
    const int pageItemMax = 18;
    const int outSignalpageItemMax = 5;

    for(int page = 0; page < m_inPageMaxNum; page++){
        for(int item = 0; item < pageItemMax; item++){
            InputData *data = m_inDataList[page][item];

            if(data->index() != -1){
                data->setValue(m_modbusManager->getReceiveDataDI(data->index()));
            }
        }
    }

    for(int page = 0; page < m_outPageMaxNum; page++){
        for(int item = 0; item < outSignalpageItemMax; item++){
            OutDataSignal *data = m_outDataSignalList[page][item];
            if(data->di() != -1){
                data->setValue(m_modbusManager->getReceiveDataDI(data->di()));
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

