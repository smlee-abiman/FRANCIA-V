#ifndef AUTOAXISVIEWMODEL_H
#define AUTOAXISVIEWMODEL_H

#include "viewmodelbase.h"

#define AUTO_AXIS_DATA_POS_TOP_RG                       2777
#define AUTO_AXIS_DATA_ENCODER_TOP_RG                   2761
#define AUTO_AXIS_DATA_TORQUE_TOP_RG                    2769
#define AXIS_MAX_NUM                                    8

class AutoAxisViewModel : public ViewModelBase
{
    Q_OBJECT

public:
    AutoAxisViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~AutoAxisViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void startPageChanging();

    bool pageIng() {return m_pageIng;}

signals:

private:
    ModbusManager       *m_modbusManager =  ModbusManager::get_instance();
    AxisInfo            *m_axisInfo = AxisInfo::get_instance();

    bool                m_pageChanging = false;
    bool                m_pageIng = false;

private slots:
    void onFinished();
    void endModbusPageWriteReady(QList<int> writeData);
};

#endif // AUTOAXISVIEWMODEL_H
