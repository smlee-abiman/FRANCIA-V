#include "autocounterviewmodel.h"

AutoCounterData::AutoCounterData(QObject *parent)
    : QObject(parent)
{
    m_now = 0;
    m_setting = 0;
}

AutoCounterData::~AutoCounterData(){

}

void AutoCounterData::setNow(int now)
{
    if(m_now != now){
        m_now = now;

        emit nowChanged(m_now);
    }
}

void AutoCounterData::setSetting(int setting)
{
    if(m_setting != setting){
        m_setting = setting;

        emit settingChanged(m_setting);
    }
}

AutoCounterViewModel::AutoCounterViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    m_commonView = commonView;

    initcounterMsgIndex();
}

AutoCounterViewModel::~AutoCounterViewModel()
{

}

void AutoCounterViewModel::startMonitoring()
{
    m_pageIng = true;
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }

    stModBusCommunicationData readCommand[1];

    readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, 2293, 16);

    m_communication->setOneShotReadCommand(readCommand[0]);

    connect(m_communication, &CommunicationThread::finish, this, &AutoCounterViewModel::onFinished);
    connect(m_modbusManager, &ModbusManager::endModbusPageWriteReady, this, &AutoCounterViewModel::endModbusPageWriteReady, Qt::DirectConnection);
}
void AutoCounterViewModel::stopMonitoring()
{
    m_pageIng = false;
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &AutoCounterViewModel::onFinished);
    disconnect(m_modbusManager, &ModbusManager::endModbusPageWriteReady, this, &AutoCounterViewModel::endModbusPageWriteReady);
}

void AutoCounterViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

QList<QObject*> AutoCounterViewModel::getDataList(int page)
{
    QList<QObject*> list;
    if(page == 0){
        for(int i = 0; i < 6; i++){
            list.append(&m_autoCounterDataAry[i]);
        }
    }else {
        for(int i = 6; i < 8; i++){
            list.append(&m_autoCounterDataAry[i]);
        }
    }
    return list;
}

void AutoCounterViewModel::initcounterMsgIndex()
{
    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            if(m_resource->getPenSetIntValue("14", j+9, i) != 0){
                m_counterMsgIndex.append(m_resource->getPenSetIntValue("14", j+9, i));
            }
        }
    }
}

void AutoCounterViewModel::onFinished(){
    if(m_pageChanging == false && m_modbusManager->getIsPageChanging() == false){
        long nowTopSR = -1;
        long settingTopHR = -1;

        switch(m_commonView->getCurrentPage()){
        case Enums::Pages::AutoAxisCounter:
            nowTopSR= 2793;
            settingTopHR= 2293;
            break;
        case Enums::Pages::AutoProductMngCounter:
            nowTopSR= 2781;
            settingTopHR= 2293;
            break;
        case Enums::Pages::AutoIOCounter:
            nowTopSR= 2777;
            settingTopHR= 2293;
            break;
        default:
            break;
        }

        if(nowTopSR != -1 && settingTopHR != -1){
            for(int i = 0; i < 8; i++){
                m_autoCounterDataAry[i].setNow(m_modbusManager->getLongReceiveDataRG(nowTopSR + (i*2)));
                m_autoCounterDataAry[i].setSetting(m_modbusManager->getLongReceiveDataHR(settingTopHR + (i*2)));
            }
        }

        setDataLoaded(true);
    }
}

void AutoCounterViewModel::endModbusPageWriteReady(QList<int> writeData)
{
    if ( (writeData.size() == 3) &&
         (((writeData[0] == 2) && (writeData[1] == 0) && (writeData[2] == 0)) ||
          ((writeData[0] == 3) && (writeData[1] == 0) && (writeData[2] == 0)) ||
          ((writeData[0] == 4) && (writeData[1] == 0) && (writeData[2] == 0))) ) {
        m_pageChanging = false;
    }
}

void AutoCounterViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}
