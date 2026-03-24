#ifndef MECHANICALPARAMVIEWMODEL_H
#define MECHANICALPARAMVIEWMODEL_H

#include "viewmodelbase.h"
#include "modelselectviewmodel.h"
#include "mechanicalparametersviewmodel.h"
#include "drvparamviewmodel.h"
#include "commonviewmodel.h"

class MechanicalParamViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(ModelSelectViewModel* modelSelectView READ modelSelectView CONSTANT)
    Q_PROPERTY(MechanicalParametersViewModel* mechaParamView READ mechaParamView CONSTANT)
    Q_PROPERTY(DrvParamViewModel* drvParamView READ drvParamView CONSTANT)
public:
    explicit MechanicalParamViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~MechanicalParamViewModel();

    ModelSelectViewModel* modelSelectView() const { return (ModelSelectViewModel*)m_panelModel.at(0); }
    MechanicalParametersViewModel* mechaParamView() const { return (MechanicalParametersViewModel*)m_panelModel.at(1); }
    DrvParamViewModel* drvParamView() const { return (DrvParamViewModel*)m_panelModel.at(2); }

private:
    CommonViewModel *m_commonView;

    QList<QObject *>    m_panelModel;

    void initPanel();

signals:

};

#endif // MECHANICALPARAMVIEWMODEL_H
