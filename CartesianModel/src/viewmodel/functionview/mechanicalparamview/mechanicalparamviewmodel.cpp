#include "mechanicalparamviewmodel.h"

MechanicalParamViewModel::MechanicalParamViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm)
{
    m_commonView = commonView;

    initPanel();
}

MechanicalParamViewModel::~MechanicalParamViewModel()
{
    qDeleteAll(m_panelModel);
}

void MechanicalParamViewModel::initPanel()
{
    m_panelModel.append(new ModelSelectViewModel(m_resource, m_communication));             // 機種選択
    m_panelModel.append(new MechanicalParametersViewModel(m_resource, m_communication, m_commonView));    // 機械パラメータ
    m_panelModel.append(new DrvParamViewModel(m_resource, m_communication));                // ドライバパラメータ
}
