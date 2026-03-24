#include <QQmlEngine>
#include "manualviewmodel.h"

ManualViewModel::ManualViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm),
      m_commonView(commonView)
{
    initPanel();
}

ManualViewModel::~ManualViewModel()
{
    qDeleteAll(m_panelModel);
}

void ManualViewModel::initPanel()
{
    m_panelModel.append(new ManualIOViewModel(m_resource, m_communication));
    m_panelModel.append(new AxisOperateViewModel(m_resource, m_communication, m_axisInfo, m_commonView));
}
