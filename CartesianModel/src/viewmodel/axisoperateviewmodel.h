#ifndef AXISOPERATEVIEWMODEL_H
#define AXISOPERATEVIEWMODEL_H

#include <QObject>
#include "viewmodelbase.h"
#include "axisdata.h"
#include "axisinfo.h"

const quint16 AXIS_POSITION_TOP_SR = 2785;
const quint16 AXIS_ENCODER_TOP_SR = 2761;
const quint16 AXIS_SPEED_TOP_SR = 2769;
const quint16 AXIS_MAX_POSITION_TOP_HR = 8656;
const quint16 AXIS_SERVO_STATUS_TOP_DI = 2680;

const QList<QList<quint16>> AXIS_ADDRESS_LIST = {
    // position(SR)   home(DI)    over(DI)    brake(DI)   encoder(SR)   torque(SR)   max(HR)   servo(DI)    servo(DO)
    {     2785,         2599,       2679,       2655,       2761,       2777,       8656,       2687,       1783}, //Y-Axis
    {     2787,         2598,       2678,       2654,       2762,       2778,       8659,       2686,       1782}, //MX-Axis
    {     2789,         2597,       2677,       2653,       2763,       2779,       8662,       2685,       1781}, //MZ-Axis
    {     2791,         2596,       2676,       2652,       2764,       2780,       8665,       2684,       1780}, //SX-Axis
    {     2793,         2595,       2675,       2651,       2765,       2781,       8668,       2683,       1779}, //SZ-Axis
    {     2795,         2594,       2674,       2650,       2766,       2782,       8671,       2682,       1778}, //6-Axis
    {     2797,         2593,       2673,       2649,       2767,       2783,       8674,       2681,       1777}, //7-Axis
    {     2799,         2592,       2672,       2648,       2768,       2784,       8677,       2680,       1776}, //8-Axis
};

typedef enum {
    Position,
    home,
    over,
    brake,
    encoder,
    torque,
    MaxPosition,
    ServoDI,
    ServoDO,
} AxisAddressType;

const quint16 MANUAL_SELECT_AXIS_TOP_DO = 2200;
const int MANUAL_SELECT_AXIS_BIT_NUM = 8;
const quint16 MOVE_POSITION_HR = 1887;
const quint16 VIEWPOINT_DO = 1594;
const quint16 VIEWPOINT_SR = 2745;
const quint16 VIEWPOINT_SR_BIT_MASK = 0x0001;

// pen_set
const QString PEN_SET_GROUP_MANUAL_OPERATE = "6";
const int PEN_SET_AXIS_OPERATE_DATA_FORMAT_TOP_ROW = 198;
const int PEN_SET_COLUMN_DATA_FORMAT_CURRENT_POSITION = 0;
const int PEN_SET_COLUMN_DATA_FORMAT_ENCODER = 1;
const int PEN_SET_COLUMN_DATA_FORMAT_MAX_POSITION = 2;

class CommonViewModel;

class AxisOperateViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QObject *axisInfo READ axisInfo CONSTANT)
    Q_PROPERTY(bool viewpoint READ viewpoint WRITE setViewpoint NOTIFY viewpointChanged)
    Q_PROPERTY(bool jogDialMode READ jogDialMode WRITE setJogDialMode NOTIFY jogDialModeChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
public:
    class AxisDataFormat {
    public:
        int currentPosition = 0;
        int encoder = 0;
        int maxPosition = 0;
    };

    explicit AxisOperateViewModel(ResourceManager *resource, CommunicationThread * comm, AxisInfo *axisInfo, CommonViewModel *commonView);
    explicit AxisOperateViewModel(QObject *object = nullptr);
    ~AxisOperateViewModel() override;

    Q_INVOKABLE int modBusServoControl(int axis, bool status);
    Q_INVOKABLE int modbusJogScrollKey(int keyNo);
    Q_INVOKABLE void viewPointControl(bool reverse);
    Q_INVOKABLE QString formatcurrentPosition(int axis, int value);
    Q_INVOKABLE QString formatMaxPosition(int axis, int value);
    Q_INVOKABLE QString formatEncoder(int axis, int value);
    Q_INVOKABLE void startPageChanging();
    void setJogDialMode(bool mode);
    void setViewpoint(bool reverse);
    bool jogDialMode() const { return  m_jogDialMode; }
    bool viewpoint() const { return m_viewPoint; }
    QObject *axisInfo() const { return m_axisInfo; }

protected:
    void onActivate() override;
    void onDeactivate() override;

signals:
    void viewpointChanged(bool reverse);
    void jogDialModeChanged();
    void dataLoadedChanged(bool dataLoaded);

public slots:
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void endModbusPageChange(int mainScreenId, int subScreenId);
    void onJogDialSpeedChanged(Enums::JogDialSpeed speed);

private:
     void init();
     void readDataFormatCsv();
     void readPenSetData();
     void setOneShotModbusCommand(stModBusCommunicationData &data);
     void modbusGetMaxPosition();
     void updateMaxPosition();
     void setDataLoaded(bool dataLoaded);
     bool dataLoaded() { return m_dataLoaded; }

private:
     AxisInfo               *m_axisInfo = nullptr;
     CommonViewModel        *m_commonView;
     bool                   m_viewPoint = false;    // true: reverse / false: normal
     QList<AxisDataFormat>  m_axisDataFormatList;
     int                    m_orgManualSpeed;
     bool                   m_jogDialMode = false;
     bool                   m_modbusAxisStatusInitialized = false;
     bool                   m_pageChanging = false;
     bool                   m_dataLoaded = false;
     const QList<int>       AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};

};

#endif // AXISOPERATEVIEWMODEL_H
