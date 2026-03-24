#include "pendantapplication.h"
#include "modbusmanager.h"

#define DEFAULT_OPERATION_TIMEOUT_MSEC (10 * 60 * 1000)
#define MIN_OPERATION_TIMEOUT_MSEC (1 * 60 * 1000)

#define EMERGENCY_STOP_STATE_DI (2662)

//------------------------------------------------------
// Application.
//------------------------------------------------------
PendantApplication* PendantApplication::m_instance = nullptr;
//------------------------------------------------------
PendantApplication::PendantApplication(int &argc, char **argv)
    : mInheritApplication(argc,argv)
    , m_phyKeyManager(nullptr)
    , m_communication(nullptr)
{
    m_operationMonitoringTimer = new QTimer(this);
    m_operationMonitoringTimer->setSingleShot(true);
    connect(m_operationMonitoringTimer, &QTimer::timeout, this, &PendantApplication::onOperationMonitorTimeout, Qt::QueuedConnection);
    startOperationMonitoring(DEFAULT_OPERATION_TIMEOUT_MSEC);
}
//------------------------------------------------------
PendantApplication::~PendantApplication()
{
    if (m_operationMonitoringTimer) {
        if (m_operationMonitoringTimer->isActive()) {
            m_operationMonitoringTimer->stop();
        }
        disconnect(m_operationMonitoringTimer, &QTimer::timeout, this, &PendantApplication::onOperationMonitorTimeout);
        delete m_operationMonitoringTimer;
        m_operationMonitoringTimer = nullptr;
    }
    if (m_phyKeyManager) {
        disconnectPhyKeyManager();
        m_phyKeyManager = nullptr;
    }
    if (m_communication) {
        disconnectCommunicationThread();
        m_communication = nullptr;
    }
}
//------------------------------------------------------
// note: already started monitoring, start monitoring again.
void PendantApplication::startOperationMonitoring(const int timeoutMSec)
{
    if (!m_operationMonitoringTimer) {
        qWarning() << "<<failed to set operation timeout time.>>";
        return;
    }
    if (timeoutMSec < MIN_OPERATION_TIMEOUT_MSEC) {
        qWarning("<<failed to set operation timeout time. not set timeoutMSec < %d.>>", MIN_OPERATION_TIMEOUT_MSEC);
        return;
    }
    m_operationMonitoringTimer->stop();
    m_operationMonitoringTimer->setInterval(timeoutMSec);
    m_operationMonitoringTimer->start();
    // for debug.
    qInfo() << "<<(re)start operation monitoring.>>"
            << "interval:" << m_operationMonitoringTimer->interval();
}
//------------------------------------------------------
void PendantApplication::onOperationMonitorTimeout()
{
    qInfo() << "<<operation monitor timeout.>>"; // for debug.
    emit operationStopped();
}
//------------------------------------------------------
// operation event.
//------------------------------------------------------
bool PendantApplication::onOperated()
{
    if (!m_mutex.tryLock()) { return false; }
    bool ret = false;
    if (!m_operationMonitoringTimer->isActive()) {
        qInfo() << "<<operation stop to start.>>"; // for debug.
        emit operationStarted();
        ret = true;
    }
    m_operationMonitoringTimer->start();   // restart.
    m_mutex.unlock();
    return ret;
}
//------------------------------------------------------
// Mouse operation.
//------------------------------------------------------
bool PendantApplication::notify(QObject* receiver, QEvent* event)
{
    bool ret = operationMonitor(event);
    return (ret || mInheritApplication::notify(receiver, event));
}
//------------------------------------------------------
bool PendantApplication::operationMonitor(QEvent* event)
{
    auto eventType = event->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::Wheel:
        // operation.
        return onOperated();
    default:
        // no operation.
        break;
    }
    return false;
}
//------------------------------------------------------
// Physical key operation.
//------------------------------------------------------
void PendantApplication::startPhyKeyMonitoring(PhyKeyManager& phyKeyManager)
{
    if (m_phyKeyManager) { return; }
    m_phyKeyManager = &phyKeyManager;
    connectPhyKeyManager();
}
//------------------------------------------------------
void PendantApplication::connectPhyKeyManager()
{
    connect(m_phyKeyManager, &PhyKeyManager::updateSelectSwInfo,          this, &PendantApplication::onPhyKeySwSelectChanged,  Qt::QueuedConnection); // select SW.
    connect(m_phyKeyManager, &PhyKeyManager::updateMemblemSwInfo,         this, &PendantApplication::onPhyKeySwMemblemChanged, Qt::QueuedConnection); // memblem SW.
    connect(m_phyKeyManager, &PhyKeyManager::operableSwStatusChanged,     this, &PendantApplication::onPhyKeySwStatusChanged,  Qt::QueuedConnection); // deadman Sw.
    connect(m_phyKeyManager, &PhyKeyManager::jogDialSwitchInvalidChanged, this, &PendantApplication::onPhyKeySwStatusChanged,  Qt::QueuedConnection); // jog dial.
    connect(m_phyKeyManager, &PhyKeyManager::jogDialSwitchValidChanged,   this, &PendantApplication::onPhyKeySwStatusChanged,  Qt::QueuedConnection); // jog dial.
    connect(m_phyKeyManager, &PhyKeyManager::jogDialSwitchReleaseChanged, this, &PendantApplication::onPhyKeySwChanged,        Qt::QueuedConnection); // jog dial.
    connect(m_phyKeyManager, &PhyKeyManager::jogScrollUpChanged,          this, &PendantApplication::onPhyKeySwStatusChanged,  Qt::QueuedConnection); // jog scroll.
    connect(m_phyKeyManager, &PhyKeyManager::jogScrollDownChanged,        this, &PendantApplication::onPhyKeySwStatusChanged,  Qt::QueuedConnection); // jog scroll.
}
//------------------------------------------------------
void PendantApplication::disconnectPhyKeyManager()
{
    disconnect(m_phyKeyManager, &PhyKeyManager::updateSelectSwInfo,          this, &PendantApplication::onPhyKeySwSelectChanged);
    disconnect(m_phyKeyManager, &PhyKeyManager::updateMemblemSwInfo,         this, &PendantApplication::onPhyKeySwMemblemChanged);
    disconnect(m_phyKeyManager, &PhyKeyManager::operableSwStatusChanged,     this, &PendantApplication::onPhyKeySwStatusChanged);
    disconnect(m_phyKeyManager, &PhyKeyManager::jogDialSwitchInvalidChanged, this, &PendantApplication::onPhyKeySwStatusChanged);
    disconnect(m_phyKeyManager, &PhyKeyManager::jogDialSwitchValidChanged,   this, &PendantApplication::onPhyKeySwStatusChanged);
    disconnect(m_phyKeyManager, &PhyKeyManager::jogDialSwitchReleaseChanged, this, &PendantApplication::onPhyKeySwChanged);
    disconnect(m_phyKeyManager, &PhyKeyManager::jogScrollUpChanged,          this, &PendantApplication::onPhyKeySwStatusChanged);
    disconnect(m_phyKeyManager, &PhyKeyManager::jogScrollDownChanged,        this, &PendantApplication::onPhyKeySwStatusChanged);
}
//------------------------------------
void PendantApplication::onPhyKeySwChanged()
{
    static_cast<void>(onOperated());
}
//------------------------------------
void PendantApplication::onPhyKeySwStatusChanged(bool /*swStatus*/)
{
    static_cast<void>(onOperated());
}
//------------------------------------------------------
void PendantApplication::onPhyKeySwMemblemChanged(PhyKeyManager::MemblemSwInfo /*memblemSwInfo*/)
{
    static_cast<void>(onOperated());
}
//------------------------------------
void PendantApplication::onPhyKeySwSelectChanged(PhyKeyManager::SelectSwitchStatus /*selectSwStatus*/)
{
    static_cast<void>(onOperated());
}
//------------------------------------------------------
// Emergency Stop.
//------------------------------------------------------
void PendantApplication::startCommunicationMonitoring(CommunicationThread& communication)
{
    if (m_communication) { return; }
    m_communication = &communication;
    connectCommunicationThread();
}
//------------------------------------------------------
void PendantApplication::connectCommunicationThread()
{
    connect(m_communication, &CommunicationThread::finish, this, &PendantApplication::onFinished, Qt::QueuedConnection);
}
//------------------------------------------------------
void PendantApplication::disconnectCommunicationThread()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &PendantApplication::onFinished);
}
//------------------------------------------------------
void PendantApplication::onFinished()
{
    auto* modbus = ModbusManager::get_instance();
    auto emergencyStop = modbus->getReceiveDataDI(EMERGENCY_STOP_STATE_DI);
    emergencyStop && onOperated();
}
