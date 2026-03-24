#ifndef DRVMONITORVIEWMODEL_H
#define DRVMONITORVIEWMODEL_H

#include "viewmodelbase.h"
#include "axisdata.h"
#include "axisinfo.h"
#include "commonviewmodel.h"
#include <QTimerEvent>
#include <QQueue>
//#define GRAPH_DRAW_SPEED_DEBUG
//#define GRAPH_DRAW_TORQUE_DEBUG

#define DRIVER_REALTIME_PERIODIC_MONITOR_CMD_LIST_NUM                1
#define GRAPH_HISTORY_NUM                                            5
#define GRAPH_AXIS_NUM_MAX                                           3
#define GRAPH_SAMPLING_200MS                                        20
#define GRAPH_SAMPLING_500MS                                        50
#define GRAPH_SAMPLING_1S                                          100
#define GRAPH_SAMPLING_2S                                          200
#define GRAPH_SAMPLING_FULL                                        201
#define GRAPH_SAMPLING_200MS_START_INDEX                           180
#define GRAPH_SAMPLING_500MS_START_INDEX                           150
#define GRAPH_SAMPLING_1S_START_INDEX                              100
#define GRAPH_SAMPLING_2S_START_INDEX                                0

const QList<QList<quint16>> REALTIME_AXIS_REAL_ADDRESS_LIST = {
    // speed(SR)  deviation(SR)  torque(SR) loadRate(SR)   resurrectionResistorRate(SR)
    {  2761,      2793,          2785,      2777,          2769 },     // Y-Axis
    {  2762,      2795,          2786,      2778,          2770 },     // MX-Axis
    {  2763,      2797,          2787,      2779,          2771 },     // MZ-Axis
    {  2764,      2799,          2788,      2780,          2772 },     // SX-Axis
    {  2765,      2801,          2789,      2781,          2773 },     // SZ-Axis
    {  2766,      2803,          2790,      2782,          2774 },     // 6-Axis
    {  2767,      2805,          2791,      2783,          2775 },     // 7-Axis
    {  2768,      2807,          2792,      2784,          2776 },     // 8-Axis
};

const QList<QList<quint16>> WAVEFORM_AXIS_ADDRESS_LIST = {
    // 1                        2                       3                       4                       5
    // speed(SR)  torque(SR)    speed(SR)  torque(SR)   speed(SR)  torque(SR)   speed(SR)  torque(SR)   speed(SR)  torque(SR)
    {  2769,      2794,         2770,      2795,        2771,      2796,        2772,      2797,        2773,      2798,        },     // Y-Axis
    {  2774,      2799,         2775,      2800,        2776,      2801,        2777,      2802,        2778,      2803,        },     // MX-Axis
    {  2779,      2804,         2780,      2805,        2781,      2806,        2782,      2807,        2783,      2808,        },     // MZ-Axis
    {  2784,      2809,         2785,      2810,        2786,      2811,        2787,      2812,        2788,      2813,        },     // SX-Axis
    {  2789,      2814,         2790,      2815,        2791,      2816,        2792,      2817,        2793,      2818,        },     // SZ-Axis
};

typedef struct {
    short   speed;
    int     deviation;
    short   torque;
} stDrvMonPeakData;

class DrvMonitorViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QObject *axisInfo READ axisInfo CONSTANT)
    Q_PROPERTY(QList<int> graphSpeedData READ graphSpeedData NOTIFY graphSpeedDataChanged)
    Q_PROPERTY(QList<int> graphTorqueData READ graphTorqueData NOTIFY graphTorqueDataChanged)
    Q_PROPERTY(QList<int> graphSpeedDataFirst READ graphSpeedDataFirst NOTIFY graphSpeedDataFirstChanged)
    Q_PROPERTY(QList<int> graphTorqueDataFirst READ graphTorqueDataFirst NOTIFY graphTorqueDataFirstChanged)
    Q_PROPERTY(QList<int> graphSpeedDataSecond READ graphSpeedDataSecond NOTIFY graphSpeedDataSecondChanged)
    Q_PROPERTY(QList<int> graphTorqueDataSecond READ graphTorqueDataSecond NOTIFY graphTorqueDataSecondChanged)
    Q_PROPERTY(QList<int> graphSpeedDataThird READ graphSpeedDataThird NOTIFY graphSpeedDataThirdChanged)
    Q_PROPERTY(QList<int> graphTorqueDataThird READ graphTorqueDataThird NOTIFY graphTorqueDataThirdChanged)

    Q_PROPERTY(QList<int> axisSpeedPeak READ axisSpeedPeak NOTIFY axisSpeedPeakChanged)
    Q_PROPERTY(QList<int> axisTorquePeak READ axisTorquePeak NOTIFY axisTorquePeakChanged)
    Q_PROPERTY(QList<int> axisRotationMax READ axisRotationMax NOTIFY axisRotationMaxChanged)

    Q_PROPERTY(bool isRTGraphDraw READ isRTGraphDraw WRITE setIsRTGraphDraw NOTIFY isRTGraphDrawChanged)

    Q_PROPERTY(int curAxisIndex READ curAxisIndex NOTIFY curAxisIndexChanged)

    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
public:
    DrvMonitorViewModel(ResourceManager *resource, CommunicationThread * comm, AxisInfo *axisInfo, CommonViewModel *commonView);
    ~DrvMonitorViewModel();
    QObject *axisInfo() const { return m_axisInfo; }

    enum TimerSettingValue {
        TimerSettingValue200ms = 0,
        TimerSettingValue500ms,
        TimerSettingValue1s,
        TimerSettingValue2s,
        TimerSettingValueMax,
    };
    enum GraphDrawMode {
        GraphDrawModeInvalid = -1,
        GraphDrawModeOneAxis,
        GraphDrawModeThreeAxis,
        GraphDrawModeMax,
    };
    enum GraphOrder {
        GraphOrderOneAxis = 0,
        GraphOrderThreeAxisFirst,
        GraphOrderThreeAxisSecond,
        GraphOrderThreeAxisThird,
        GraphOrderMax,
    };

    Q_INVOKABLE void setSubViewIndex(int index);
    Q_INVOKABLE int getAxisCount();
    Q_INVOKABLE void initAxisPeakData(bool isFirst = true);
    Q_INVOKABLE int getPeakTorqueData(int axisIndex);
    Q_INVOKABLE int getRealTimePeakTorqueData(int axisIndex);
    Q_INVOKABLE int getPeakSpeedData(int axisIndex);
    Q_INVOKABLE void setTimerValue(int graphOrder, int timerValue);
    Q_INVOKABLE void setGraphDrawMode(int drawMode);
    Q_INVOKABLE int getGraphDrawMode() { return mGraphDrawMode; }
    Q_INVOKABLE void setCurAxis(int curTopAxisIndex);
    Q_INVOKABLE void ctrlRTGraphDraw(bool isRTDraw);
    Q_INVOKABLE void startPageChanging();
    Q_INVOKABLE int getMinSpeedData(int axisIndex);
    Q_INVOKABLE int getMinTorqueData(int axisIndex);

    QList<int> graphSpeedData() const { return m_speedDispDataList_axis1; }
    QList<int> graphTorqueData() const { return m_torqueDispDataList_axis1; }
    QList<int> graphSpeedDataFirst() const { return m_speedDispDataList_axis1; }
    QList<int> graphTorqueDataFirst() const { return m_torqueDispDataList_axis1; }
    QList<int> graphSpeedDataSecond() const { return m_speedDispDataList_axis2; }
    QList<int> graphTorqueDataSecond() const { return m_torqueDispDataList_axis2; }
    QList<int> graphSpeedDataThird() const { return m_speedDispDataList_axis3; }
    QList<int> graphTorqueDataThird() const { return m_torqueDispDataList_axis3; }

    QList<int> axisSpeedPeak() const { return m_axisSpeedPeak; }
    QList<int> axisTorquePeak() const { return m_axisTorquePeak; }
    QList<int> axisRotationMax() const { return m_axisRotationMax; }

    void setIsRTGraphDraw(bool isRTGraphDraw);
    bool isRTGraphDraw() const { return m_isRTGraphDraw; }

    int curAxisIndex() const { return mCurAxisIndex; }
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }

protected:
    void onActivate() override;
    void onDeactivate() override;
    void timerEvent(QTimerEvent *timerEvent);

private:
    AxisInfo                    *m_axisInfo = nullptr;
    CommonViewModel             *m_commonView = nullptr;
    QList<QList<quint16>>       m_axisInfoDataFormat;
    int                         mSubViewIndex = 0;
    stDrvMonPeakData            prePeakData[AxisInfo::Axis::Axis8 + 1];
    stDrvMonPeakData            preMinData[AxisInfo::Axis::Axis8 + 1];
    int                         mTimerSettingValue[GraphDrawMode::GraphDrawModeMax];
    int                         mTimerSettingThreeAxisValue[GraphOrder::GraphOrderMax];
    int                         mGraphDrawMode = -1;
    int                         mPreGraphDrawMode = -1;
    int                         mCurTopAxis = 0;
    int                         mCurAxisIndex = 0;
    int                         mCurTopAxisIndex = 0;
    QList<int>                  m_axisSpeedPeak;
    QList<int>                  m_axisTorquePeak;
    QList<int>                  m_axisRotationMax;
    int                         mTimerId[GraphOrder::GraphOrderMax];
    bool                        mIsTimerStatus[GraphOrder::GraphOrderMax];
    int                         mTimerCnt[GraphOrder::GraphOrderMax];
    QList<int>                  mCmnAxisOrder;
    bool                        m_isRTGraphDraw = false;

    QQueue<int>                 m_torqueDispDataList_axis1;
    QQueue<int>                 m_torqueDispDataList_axis2;
    QQueue<int>                 m_torqueDispDataList_axis3;

    QQueue<int>                 m_speedDispDataList_axis1;
    QQueue<int>                 m_speedDispDataList_axis2;
    QQueue<int>                 m_speedDispDataList_axis3;

    QQueue<int>                 m_torqueDataListArray[8];
    QQueue<int>                 m_speedDataListArray[8];
    int                         m_axisSpeedPeakArray[8];
    int                         m_axisTorquePeakArray[8];
    int                         m_axisRotationMaxArray[8];

    QList<int>                  m_lastSpeedIndexes[8];
    QList<int>                  m_lastTorqueIndexes[8];

    bool                        mIsModbusMonitor = false;
    int                         mDetailGraphMonitorCnt = 0;
    bool                        m_pageChanging = false;
    bool                        m_dataLoaded = false;
    QMutex                      *mMutexTimerSet;
    bool                        mIsMutexTimerSet = false;
    QMutex                      *mMutexGraphDraw;
    bool                        mIsMutexGraphDraw = false;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2819, 2821, 2823};

    stModbusPeridiocMonitor initModbusPageCmd[DRIVER_REALTIME_PERIODIC_MONITOR_CMD_LIST_NUM] = {
        { QModbusPdu::FunctionCode::ReadInputRegisters, 0x0218, 0x0048 },        // 速度/偏差（P)/トルク/負荷率/回生抵抗率    FuncCode:0x04 addr:SR536 size:72
    };

    int graphDrawBufCount[TimerSettingValue::TimerSettingValueMax] = {
        GRAPH_SAMPLING_200MS + 1,
        GRAPH_SAMPLING_500MS + 1,
        GRAPH_SAMPLING_1S + 1,
        GRAPH_SAMPLING_2S + 1
    };

    void initSettingPenSetData();
    void initPageCommand();
    void initGraphDataAndNotify();
    void deinitPageCommand();
    void reinitPageCommand();
    void startModbusMonitoring();
    void stopModbusMonitoring();
    void drawAxisGraph(int axis);
    void judgePeakDataNotify(int axis, int speedValue, int torqueValue);
    void timerStart(int mode, int timeOut);
    void timerStop(int mode);
    void updateOneAxisData(int axis);
    void updateThreeAxisData(int axis);
#ifdef GRAPH_DRAW_SPEED_DEBUG
    void debugDrawSpeedGraph(int axis, int *curSpeedValue);
#endif // GRAPH_DRAW_SPEED_DEBUG
#ifdef GRAPH_DRAW_TORQUE_DEBUG
    void debugDrawTorqueGraph(int axis, int *curTorqueValue);
#endif // GRAPH_DRAW_TORQUE_DEBUG
    void lockTimerSetMutex();
    void unlockTimerSetMutex();
    void lockGraphDrawMutex();
    void unlockGraphDrawMutex();

    void DispDataUpdate();
    void DispDataQueueCopy(QQueue<int> *dest, QQueue<int> *src, TimerSettingValue mode);

    void dataUpdate();
    void dataUpdateWave();

public slots:
    void onFinished();

private slots:
    void endModbusPageChange(QList<int> writeData);

signals:
    void graphSpeedDataChanged(QList<int> graphSpeedData);
    void graphTorqueDataChanged(QList<int> graphTorqueData);
    void graphSpeedDataFirstChanged(QList<int> graphSpeedDataFirst);
    void graphTorqueDataFirstChanged(QList<int> graphTorqueDataFirst);
    void graphSpeedDataSecondChanged(QList<int> graphSpeedDataSecond);
    void graphTorqueDataSecondChanged(QList<int> graphTorqueDataSecond);
    void graphSpeedDataThirdChanged(QList<int> graphSpeedDataThird);
    void graphTorqueDataThirdChanged(QList<int> graphTorqueDataThird);
    void axisSpeedPeakChanged(QList<int> axisSpeedPeak);
    void axisTorquePeakChanged(QList<int> axisTorquePeak);
    void axisRotationMaxChanged(QList<int> axisRotationMax);
    void isRTGraphDrawChanged(bool isRTGraphDraw);
    void axis1SpeedChanged(QList<int> axis1Speed);
    void axis1TorqueChanged(QList<int> axis1Torque);
    void axis2SpeedChanged(QList<int> axis2Speed);
    void axis2TorqueChanged(QList<int> axis2Torque);
    void axis3SpeedChanged(QList<int> axis3Speed);
    void axis3TorqueChanged(QList<int> axis3Torque);
    void curAxisIndexChanged(int curTopAxisIndex);
    void dataLoadedChanged(bool dataLoaded);
};

#endif // DRVMONITORVIEWMODEL_H
