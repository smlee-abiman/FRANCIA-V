#ifndef MONITORVIEW_H
#define MONITORVIEW_H

#include "viewmodelbase.h"
#include "iomonitorviewmodel.h"
#include "ioforceviewmodel.h"
#include "drvmonitorviewmodel.h"
#include "internalmonitorviewmodel.h"
#include "axisinfo.h"
#include "commonviewmodel.h"

class MonitorViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(IOMonitorViewModel* ioView READ ioView CONSTANT)
    Q_PROPERTY(IOForceViewModel* ioForceView READ ioForceView CONSTANT)
    Q_PROPERTY(DrvMonitorViewModel* drvMonitorView READ drvMonitorView CONSTANT)
    Q_PROPERTY(InternalMonitorViewModel* internalMonitorView READ internalMonitorView CONSTANT)
public:
    explicit MonitorViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~MonitorViewModel();

    IOMonitorViewModel* ioView() const { return (IOMonitorViewModel*)m_panelModel.at(0); }
    IOForceViewModel* ioForceView() const { return (IOForceViewModel*)m_panelModel.at(1); }
    DrvMonitorViewModel* drvMonitorView() const { return (DrvMonitorViewModel*)m_panelModel.at(2); }
    InternalMonitorViewModel* internalMonitorView() const { return (InternalMonitorViewModel*)m_panelModel.at(3); }

private:
    QList<QObject *>    m_panelModel;
    AxisInfo            *m_axisInfo = AxisInfo::get_instance();
    CommonViewModel     *m_commonView = nullptr;

    void initPanel();
};

#endif // MONITORVIEW_H
