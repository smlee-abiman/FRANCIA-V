#include "functionviewmodel.h"

FunctionViewModel::FunctionViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm)
{
    m_commonView = commonView;
    initFuncPanel();
}

FunctionViewModel::~FunctionViewModel()
{
    qDeleteAll(m_funcPanelModel);
}

void FunctionViewModel::initFuncPanel()
{
    m_funcPanelModel.append(new ControllerSettingsViewModel(m_resource, m_communication));
    m_funcPanelModel.append(new MechanicalParamViewModel(m_resource, m_communication, m_commonView));
    m_funcPanelModel.append(nullptr);               // TODO 区域設定
    m_funcPanelModel.append(nullptr);               // TODO メンテナンス
    m_funcPanelModel.append(new ProductionViewModel(m_resource, m_communication, m_commonView));
    m_funcPanelModel.append(new AbsSetViewModel(m_resource, m_communication, m_commonView));
    m_funcPanelModel.append(new AccelerationViewModel(m_resource, m_communication));
    m_funcPanelModel.append(new VersionViewModel(m_resource, m_communication));
}
