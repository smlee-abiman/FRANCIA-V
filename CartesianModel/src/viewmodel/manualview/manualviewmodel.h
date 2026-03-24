#ifndef MANUALVIEWMODEL_H
#define MANUALVIEWMODEL_H

#include "viewmodelbase.h"
#include "commonviewmodel.h"
#include "manualioviewmodel.h"
#include "axisoperateviewmodel.h"

class CommonViewModel;

class ManualViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(ManualIOViewModel* ioView READ ioView CONSTANT)
    Q_PROPERTY(QList<QObject *> panelModel READ panelModel CONSTANT)

public:
    explicit ManualViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~ManualViewModel();

    ManualIOViewModel* ioView() const { return (ManualIOViewModel*)m_panelModel.at(0); }
    QList<QObject *> panelModel() const { return m_panelModel; }

private:
    QList<QObject *>        m_panelModel;
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();
    CommonViewModel         *m_commonView;


    void initPanel();
};

#endif // MANUALVIEWMODEL_H
