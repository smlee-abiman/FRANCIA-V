#include "monitorviewmodel.h"

MonitorViewModel::MonitorViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm),
      m_commonView(commonView)
{
    initPanel();
}

MonitorViewModel::~MonitorViewModel()
{
    qDeleteAll(m_panelModel);
}

void MonitorViewModel::initPanel()
{
    m_panelModel.append(new IOMonitorViewModel(m_resource, m_communication));
    m_panelModel.append(new IOForceViewModel(m_resource, m_communication));
    m_panelModel.append(new DrvMonitorViewModel(m_resource, m_communication, m_axisInfo, m_commonView));
    m_panelModel.append(new InternalMonitorViewModel(m_resource, m_communication));
}
