#ifndef ORIGINVIEWMODEL_H
#define ORIGINVIEWMODEL_H

#include "viewmodelbase.h"
#include "axisinfo.h"
#include <QTimerEvent>

#define ORIGIN_ONE_PAGE_MAX                                 8
#define ORIGIN_AXIS_DATA_HOM_TOP_DI                         2599
#define ORIGIN_AXIS_DATA_OVR_TOP_DI                         2679
#define ORIGIN_AXIS_DATA_MB_TOP_DI                          2655
#define ORIGIN_AXIS_DATA_PERCENTAGE_TOP_RG                  2801
#define ORIGIN_AXIS_DATA_POS_TOP_RG                         2785
#define ORIGIN_AXIS_DATA_ENCODER_TOP_RG                     2761
#define ORIGIN_AXIS_DATA_TORQUE_TOP_RG                      2777
#define ORIGIN_CARRIAGE_STATUS_DI                           2705
#define ORIGIN_CARRIAGE_FINISH_STATUS_DI                    2704
#define ORIGIN_PERIODIC_MONITOR_CMD_LIST_NUM                7
#define ORIGIN_ONESHOT_MONITOR_ANIMATION_STATUS_INDEX       ORIGIN_PERIODIC_MONITOR_CMD_LIST_NUM
#define ORIGIN_ONESHOT_MONITOR_ANIMATION_STATUS_CMD_NUM     2
#define ORIGIN_OPERATION_MODE_DO                            1676

#define GET_ORIGIN_AXIS_DATA_POS_RG(index)  \
    (index == 0 ? ORIGIN_AXIS_DATA_POS_TOP_RG : \
     index == 1 ? ORIGIN_AXIS_DATA_POS_TOP_RG + 2 : \
     index == 2 ? ORIGIN_AXIS_DATA_POS_TOP_RG + 4 : \
     index == 3 ? ORIGIN_AXIS_DATA_POS_TOP_RG + 6 : \
     index == 4 ? ORIGIN_AXIS_DATA_POS_TOP_RG + 8 : \
     index == 5 ? ORIGIN_AXIS_DATA_POS_TOP_RG + 10 : \
     index == 6 ? ORIGIN_AXIS_DATA_POS_TOP_RG + 12 : \
     index == 7 ? ORIGIN_AXIS_DATA_POS_TOP_RG + 14 : ORIGIN_AXIS_DATA_POS_TOP_RG)

#define GET_ORIGIN_AXIS_DATA_HOM_DI(index)  \
    (index == 0 ? ORIGIN_AXIS_DATA_HOM_TOP_DI : \
     index == 1 ? ORIGIN_AXIS_DATA_HOM_TOP_DI - 1 : \
     index == 2 ? ORIGIN_AXIS_DATA_HOM_TOP_DI - 2 : \
     index == 3 ? ORIGIN_AXIS_DATA_HOM_TOP_DI - 3 : \
     index == 4 ? ORIGIN_AXIS_DATA_HOM_TOP_DI - 4 : \
     index == 5 ? ORIGIN_AXIS_DATA_HOM_TOP_DI - 5 : \
     index == 6 ? ORIGIN_AXIS_DATA_HOM_TOP_DI - 6 : \
     index == 7 ? ORIGIN_AXIS_DATA_HOM_TOP_DI - 7 : ORIGIN_AXIS_DATA_HOM_TOP_DI)

#define GET_ORIGIN_AXIS_DATA_OVR_DI(index)  \
    (index == 0 ? ORIGIN_AXIS_DATA_OVR_TOP_DI : \
     index == 1 ? ORIGIN_AXIS_DATA_OVR_TOP_DI - 1 : \
     index == 2 ? ORIGIN_AXIS_DATA_OVR_TOP_DI - 2 : \
     index == 3 ? ORIGIN_AXIS_DATA_OVR_TOP_DI - 3 : \
     index == 4 ? ORIGIN_AXIS_DATA_OVR_TOP_DI - 4 : \
     index == 5 ? ORIGIN_AXIS_DATA_OVR_TOP_DI - 5 : \
     index == 6 ? ORIGIN_AXIS_DATA_OVR_TOP_DI - 6 : \
     index == 7 ? ORIGIN_AXIS_DATA_OVR_TOP_DI - 7 : ORIGIN_AXIS_DATA_OVR_TOP_DI)

#define GET_ORIGIN_AXIS_DATA_MB_DI(index)  \
    (index == 0 ? ORIGIN_AXIS_DATA_MB_TOP_DI : \
     index == 1 ? ORIGIN_AXIS_DATA_MB_TOP_DI - 1 : \
     index == 2 ? ORIGIN_AXIS_DATA_MB_TOP_DI - 2 : \
     index == 3 ? ORIGIN_AXIS_DATA_MB_TOP_DI - 3 : \
     index == 4 ? ORIGIN_AXIS_DATA_MB_TOP_DI - 4 : \
     index == 5 ? ORIGIN_AXIS_DATA_MB_TOP_DI - 5 : \
     index == 6 ? ORIGIN_AXIS_DATA_MB_TOP_DI - 6 : \
     index == 7 ? ORIGIN_AXIS_DATA_MB_TOP_DI - 7 : ORIGIN_AXIS_DATA_MB_TOP_DI)

class OriginViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QObject *axisInfo READ axisInfo CONSTANT)
    Q_PROPERTY(int originStatus READ originStatus WRITE setOriginStatus NOTIFY originStatusChanged)

public:
    enum OriginCarriageStatus{
        OriginStatusBeforeCarriage,
        OriginStatusCarriaging,
        OriginStatusPauseCarriage,
        OriginStatusResumeCarriage,
        OriginStatusFinishCarriage,
    };

    explicit OriginViewModel(ResourceManager *resource, CommunicationThread * comm);
    explicit OriginViewModel(QObject *object = nullptr);
    ~OriginViewModel();

    QObject *axisInfo() const { return m_axisInfo; }
    int originStatus(){ return m_originStatus; }

    Q_INVOKABLE int getAxisCount();
    Q_INVOKABLE void startOriginCarriageMonitor();
    Q_INVOKABLE void pauseOriginCarriageMonitor();
    Q_INVOKABLE void stopOriginCarriageMonitor();
    Q_INVOKABLE void setOperationMode();
    Q_INVOKABLE void startPageChanging();

protected:
    void onActivate() override;
    void onDeactivate() override;
    void timerEvent(QTimerEvent *timerEvent) override;

private:
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();
    int                     mAxisCount = 8;
    int                     mCurTopAxisNumber = 0;
    QList<QList<quint16>>   m_axisInfoDataFormat;

    int                     m_originStatus = OriginCarriageStatus::OriginStatusBeforeCarriage;
    int                     mTimerId;
    bool                    mIsTimerStatus = false;
    bool                    m_pageChanging = false;
    const QList<int>        AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};

    stModbusPeridiocMonitor initModbusPageCmd[ORIGIN_PERIODIC_MONITOR_CMD_LIST_NUM] = {
        { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0240, 0x0008 },        // HOM          FuncCode:0x02 addr:DI576 size:8
        { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0248, 0x0008 },        // OVR          FuncCode:0x02 addr:DI584 size:8
        { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0670, 0x0008 },        // MB           FuncCode:0x02 addr:DI1648 size:8
        { QModbusPdu::FunctionCode::ReadInputRegisters, 0x00A0, 0x0008 },        // 原点百分比    FuncCode:0x04 addr:SR160 size:8
        { QModbusPdu::FunctionCode::ReadInputRegisters, 0x01F0, 0x0010 },        // 位置          FuncCode:0x04 addr:SR496 size:16
        { QModbusPdu::FunctionCode::ReadInputRegisters, 0x0208, 0x0008 },        // エンコーダ    FuncCode:0x04 addr:SR520 size:8
        { QModbusPdu::FunctionCode::ReadInputRegisters, 0x0240, 0x0010 },        // トルク        FuncCode:0x04 addr:SR576 size:16
    };

    void initSettingPenSetData();
    void startModbusMonitoring();
    void stopModbusMonitoring();
    void initPageCommand();
    void judgeOriginCarriage();
    void modBusReadOriginStatus();
    void modBusClearOriginStatus();
    void timerStart(int timeOut);
    void timerStop();

public slots:
    void onFinished();
    void setOriginStatus(int originStatus);
    void onEndModbusPageChange(int mainScreenId, int subScreenId);

signals:
    void originStatusChanged(int originStatus);
};

#endif // ORIGINVIEWMODEL_H
