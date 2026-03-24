#ifndef FUNCTIONVIEWMODEL_H
#define FUNCTIONVIEWMODEL_H

#include "viewmodelbase.h"
#include "controllersettingsview/controllersettingsviewmodel.h"
#include "mechanicalparamview/mechanicalparamviewmodel.h"
#include "productionview/productionviewmodel.h"
#include "abssetview/abssetviewmodel.h"
#include "commonviewmodel.h"
#include "versionviewmodel.h"
#include "accelerationview/accelerationviewmodel.h"

class FunctionViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(ControllerSettingsViewModel* controllerSettingsView READ controllerSettingsView CONSTANT)
    Q_PROPERTY(MechanicalParamViewModel* mechanicalParamView READ mechanicalParamView CONSTANT)
    Q_PROPERTY(ProductionViewModel* productionView READ productionView CONSTANT)
    Q_PROPERTY(AbsSetViewModel* absSetView READ absSetView CONSTANT)
    Q_PROPERTY(AccelerationViewModel* accelerationView READ accelerationView CONSTANT)
    Q_PROPERTY(VersionViewModel* versionView READ versionView CONSTANT)

public:
    explicit FunctionViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~FunctionViewModel();

    ControllerSettingsViewModel* controllerSettingsView() const { return (ControllerSettingsViewModel*)m_funcPanelModel.at(0); }
    MechanicalParamViewModel* mechanicalParamView() const { return (MechanicalParamViewModel*)m_funcPanelModel.at(1); }
    ProductionViewModel* productionView() const { return (ProductionViewModel*)m_funcPanelModel.at(4); }
    AbsSetViewModel* absSetView() const { return (AbsSetViewModel*)m_funcPanelModel.at(5); }
    AccelerationViewModel* accelerationView() const { return (AccelerationViewModel*)m_funcPanelModel.at(6); }
    VersionViewModel* versionView() const { return (VersionViewModel*)m_funcPanelModel.at(7); }

private:
    QList<QObject *>    m_funcPanelModel;
    CommonViewModel *m_commonView;

    void initFuncPanel();

signals:

};

#endif // FUNCTIONVIEWMODEL_H
