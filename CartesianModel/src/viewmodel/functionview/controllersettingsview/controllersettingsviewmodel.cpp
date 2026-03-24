#include "controllersettingsviewmodel.h"

ControllerSettingsViewModel::ControllerSettingsViewModel(ResourceManager *resource, CommunicationThread *comm)
    : ViewModelBase(resource, comm)
{
    initPanel();
}

ControllerSettingsViewModel::~ControllerSettingsViewModel()
{
    qDeleteAll(m_panelModel);
}

void ControllerSettingsViewModel::initPanel()
{
    m_panelModel.append(new SystemSettingViewModel(m_resource, m_communication));   // システム設定
    m_panelModel.append(new UserSettingViewModel(m_resource, m_communication));     // ユーザ設定
    m_panelModel.append(new NetworkSettingViewModel(m_resource, m_communication));  // ネットワーク設定
}
