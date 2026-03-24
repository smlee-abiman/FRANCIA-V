#include "drvparamviewmodel.h"

DrvParamViewModel::DrvParamViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    initSubViewPanel();
}

DrvParamViewModel::~DrvParamViewModel()
{
    qDeleteAll(m_subViewModel);
}

void DrvParamViewModel::initSubViewPanel()
{
    m_subViewModel.append(new DriverParametersViewModel(m_resource, m_communication));  // TODO ドライバパラメータサブ画面
    m_subViewModel.append(new SimpleSetViewModel(m_resource, m_communication));         // 簡易設定サブ画面
}
