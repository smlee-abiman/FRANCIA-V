#ifndef DRVPARAMVIEWMODEL_H
#define DRVPARAMVIEWMODEL_H

#include "viewmodelbase.h"
#include "simplesetviewmodel.h"
#include "driverparametersviewmodel.h"

class DrvParamViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(DriverParametersViewModel* drvPrmView READ drvPrmView CONSTANT)
    Q_PROPERTY(SimpleSetViewModel* simpleSetView READ simpleSetView CONSTANT)
public:
    explicit DrvParamViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~DrvParamViewModel();

    DriverParametersViewModel* drvPrmView() const { return (DriverParametersViewModel*)m_subViewModel.at(0); }
    SimpleSetViewModel* simpleSetView() const { return (SimpleSetViewModel*)m_subViewModel.at(1); }

private:
    QList<QObject *>    m_subViewModel;

    void initSubViewPanel();

signals:

};

#endif // DRVPARAMVIEWMODEL_H
