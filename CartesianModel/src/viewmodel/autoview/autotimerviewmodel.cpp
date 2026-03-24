#include "autotimerviewmodel.h"

AutoTimerData::AutoTimerData(QObject *parent)
    : QObject(parent)
{
    m_timer = 0;
    m_start = false;
    m_up = false;
    m_mark = "";
    m_msg = "";
}

AutoTimerData::~AutoTimerData(){

}

void AutoTimerData::setTimer(int timer)
{
    if(m_timer != timer){
        m_timer = timer;

        emit timerChanged(m_timer);
    }
}

void AutoTimerData::setStart(bool start)
{
    if(m_start != start){
        m_start = start;

        emit startChanged(m_start);
    }
}

void AutoTimerData::setUp(bool up)
{
    if(m_up != up){
        m_up = up;

        emit upChanged(m_up);
    }
}

void AutoTimerData::setMsg(QString msg)
{
    if(m_msg != msg){
        m_msg = msg;
        emit msgChanged(m_msg);
    }
}

void AutoTimerData::setMark(QString mark)
{
    if(m_mark != mark){
        m_mark = mark;
        emit markChanged(m_mark);
    }
}

AutoTimerViewModel::AutoTimerViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    m_commonView = commonView;

    initTimerMsg();
}

AutoTimerViewModel::~AutoTimerViewModel(){

}

void AutoTimerViewModel::initTimerMsg()
{
    QString markName;
    QString msgName;
    for(int i = 0; i < 100; i++){
        int address = 774-((i%4)*2)+((i/4)*8);
        markName = m_resource->symbolAt(address);
        msgName = m_resource->getMessageStringValue("1", address,m_resource->language());
        m_dataList[i].setMsg(msgName);
        m_dataList[i].setMark(markName);
    }
    connect(m_resource, &ResourceManager::languageChanged, this, &AutoTimerViewModel::onLanguageChanged, Qt::QueuedConnection);
}

void AutoTimerViewModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    QString markName;
    QString msgName;
    for(int i = 0; i < 100; i++){
        int address = 774-((i%4)*2)+((i/4)*8);
        markName = m_resource->symbolAt(address);
        msgName = m_resource->getMessageStringValue("1", address,m_resource->language());
        m_dataList[i].setMsg(msgName);
        m_dataList[i].setMark(markName);
    }
}

void AutoTimerViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }

    stModBusCommunicationData readCommand[1];

    readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters, 2125, 100);

    m_communication->setOneShotReadCommand(readCommand[0]);

    connect(m_communication, &CommunicationThread::finish, this, &AutoTimerViewModel::onFinished);
    connect(m_modbusManager, &ModbusManager::endModbusPageWriteReady, this, &AutoTimerViewModel::endModbusPageWriteReady, Qt::DirectConnection);
}
void AutoTimerViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &AutoTimerViewModel::onFinished);
    disconnect(m_modbusManager, &ModbusManager::endModbusPageWriteReady, this, &AutoTimerViewModel::endModbusPageWriteReady);
}

QList<QObject*> AutoTimerViewModel::getDataList(int page)
{
    QList<QObject*> list;
    int offset = page * 6;

    for(int i = 0; i < 6; i++){
        if(offset + i < 100){
            list.append(&m_dataList[offset + i]);
        }
    }
    return list;
}

void AutoTimerViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void AutoTimerViewModel::onFinished(){
    if(m_pageChanging == false && m_modbusManager->getIsPageChanging() == false){
        for(int i = 0; i < 100; i++){
            int startDI = 775-((i%4)*2)+((i/4)*8);
            int upDI = 774-((i%4)*2)+((i/4)*8);

            m_dataList[i].setTimer(m_modbusManager->getReceiveDataHR(2125+i));
            m_dataList[i].setStart(m_modbusManager->getReceiveDataDI(startDI));
            m_dataList[i].setUp(m_modbusManager->getReceiveDataDI(upDI));
        }
        setDataLoaded(true);
    }
}

void AutoTimerViewModel::endModbusPageWriteReady(QList<int> writeData)
{
    if ( (writeData.size() == 3) &&
         (((writeData[0] == 2) && (writeData[1] == 0) && (writeData[2] == 0)) ||
          ((writeData[0] == 3) && (writeData[1] == 0) && (writeData[2] == 0)) ||
          ((writeData[0] == 4) && (writeData[1] == 0) && (writeData[2] == 0))) ) {
        m_pageChanging = false;
    }
}

void AutoTimerViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}
