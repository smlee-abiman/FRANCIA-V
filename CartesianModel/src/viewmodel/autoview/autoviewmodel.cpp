#include "autoviewmodel.h"

AutoViewModel::AutoViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    if (m_modbusManager){
        //qDebug() << "IOMonitorViewModel:m_modbusManager is share";
    }

    m_commonView = commonView;

    initPanel();
}

AutoViewModel::~AutoViewModel()
{
    qDeleteAll(m_panelModel);
}

void AutoViewModel::onActivate()
{
    startMonitoring();
}

void AutoViewModel::onDeactivate()
{
    stopMonitoring();
}

void AutoViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &AutoViewModel::onFinished);
}
void AutoViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &AutoViewModel::onFinished);
}

void AutoViewModel::setSelectTab(int selectTab)
{
    if(m_selectTab != selectTab){
        m_selectTab = selectTab;

        emit selectTabChanged(m_selectTab);
    }
}

void AutoViewModel::setOneCycle(bool oneCycle)
{
    if(m_oneCycle != oneCycle){
        m_oneCycle = oneCycle;

        emit oneCycleChanged(m_oneCycle);
    }
}

void AutoViewModel::setCycle(bool cycle)
{
    if(m_cycle != cycle){
        m_cycle = cycle;

        emit cycleChanged(m_cycle);
    }
}

void AutoViewModel::setIsRun(bool isRun)
{
    if(m_isRun != isRun){
        m_isRun = isRun;

        emit isRunChanged(m_isRun);
    }
}

void AutoViewModel::setStep(bool step)
{
    if(m_step != step){
        m_step = step;

        emit stepChanged(m_step);
    }
}

void AutoViewModel::setIsFront(bool isFront)
{
    if(m_isFront != isFront){
        m_isFront = isFront;

        emit isFrontChanged(m_isFront);
    }
}

void AutoViewModel::setIsBack(bool isBack)
{
    if(m_isBack != isBack){
        m_isBack = isBack;

        emit isBackChanged(m_isBack);
    }
}

void AutoViewModel::setautoRunning(bool autoRunning)
{
    if(m_autoRunning != autoRunning){
        m_autoRunning = autoRunning;

        emit autoRunningChanged(autoRunning);
    }
}

void AutoViewModel::initPanel()
{
    m_panelModel.append(new AutoAxisViewModel(m_resource, m_communication));
    m_panelModel.append(new AutoProdViewModel(m_resource, m_communication, m_commonView));
    m_panelModel.append(new AutoIOViewModel(m_resource, m_communication));
    m_panelModel.append(new AutoTimerViewModel(m_resource, m_communication, m_commonView));
    m_panelModel.append(new AutoCounterViewModel(m_resource, m_communication, m_commonView));
}

void AutoViewModel::onFinished(){
    setOneCycle(m_modbusManager->getReceiveDataDI(2669));
    setCycle(m_modbusManager->getReceiveDataDI(2670));
    setIsRun(m_modbusManager->getReceiveDataDI(2671));
    setStep(m_modbusManager->getReceiveDataDI(2613));
    setIsFront(m_modbusManager->getReceiveDataDI(2623));
    setIsBack(m_modbusManager->getReceiveDataDI(2622));
    setautoRunning(m_modbusManager->getReceiveDataDI(2615));

    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            bool isInAxisPage = static_cast<AutoAxisViewModel*>(m_panelModel.at(0))->pageIng();
            bool isInProPage = static_cast<AutoProdViewModel*>(m_panelModel.at(1))->pageIng();
            bool isInIoPage = static_cast<AutoIOViewModel*>(m_panelModel.at(2))->pageIng();

            if(isInAxisPage){
                QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST_1.at(i-5)),66, 2);
                m_axisInfo->setPosition(axis,position);
            }else if(isInProPage){
                QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST_2.at(i-5)),66, 2);
                m_axisInfo->setPosition(axis,position);
            }else if(isInIoPage){
                QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST_3.at(i-5)),66, 2);
                m_axisInfo->setPosition(axis,position);
            }
        }
    }
}
