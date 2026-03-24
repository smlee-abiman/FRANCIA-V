#ifndef SIMPLESETVIEWMODEL_H
#define SIMPLESETVIEWMODEL_H

#include "viewmodelbase.h"
#include "simplesetaxisinfo.h"
#include <QTimerEvent>

#define SIMPLE_SET_DATA_MAX_PAGE_NUM        4
#define SIMPLE_SET_SAVE_DATA_HR             1657
#define SIMPLE_SET_SAVE_DATA                31
#define SIMPLE_SET_SAVE_TIME_HR             1950
#define DRV_PARAM_DATA_MAX_PAGE_NUM         11
#define SIMPLE_SET_MODBUS_CMD_LIST_NUM      1
#define SIMPLE_SET_MODBUS_READ_TOP_SR       2977
#define SIMPLE_SET_PARAM_LIST_NUM           18
#define SIMPLE_SET_PARAM_MAX_TORQUE_INDEX   15
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
#define SIMPLE_SET_AXIS_INFO_HR             2534
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC

// -1:pen_set.csvに該当項目Noなし
// チューニングモード
// 自動調整ゲイン1
// 自動調整ゲイン2
// 負荷慣性モーメント比
// 自動ノッチ選択
// ノッチフィルタ1周波数
// ノッチフィルタ2周波数
// ノッチフィルタ1減衰量
// ノッチフィルタ2減衰量
// 制振制御周波数1
// 制振制御周波数2
#define GET_SIMPLE_SET_DISP_ITEM_INDEX(dispItemNo)  \
    (dispItemNo == 14 ? 148 : \
     dispItemNo == 16 ? -1 : \
     dispItemNo == 17 ? -1 : \
     dispItemNo == 15 ? 149 : \
     dispItemNo == 71 ? 190 : \
     dispItemNo == 72 ? 191 : \
     dispItemNo == 75 ? 194 : \
     dispItemNo == 73 ? 192 : \
     dispItemNo == 76 ? 197 : \
     dispItemNo == 78 ? -1 : \
     dispItemNo == 79 ? 184 : \
     dispItemNo == 81 ? 186 : \
     dispItemNo == 198 ? -1 : -1)

const QList<int> SIMPLE_SET_DISP_FORMAT_INVALID_INDEX_LIST = {
    0,  4,  11, 17
};

const QList<QList<quint16>> SIMPLE_SET_PARAM_ADDR_LIST = {
    // 軸1(SR/HR)   軸2(SR/HR)  軸3(SR/HR)  軸4(SR/HR)  軸5(SR/HR) 軸6(SR/HR)  軸7(SR/HR)  軸8(SR/HR)
    {  55086,       55088,      55092,      55090,      55094,    55096,      55098,      55100 },     // page1 チューニングモード HR*****
    {  55118,       55120,      55124,      55122,      55126,    55128,      55130,      55132 },     // 自動調整ゲイン1 SR*****にデフォルト値、HR*****には設定値
    {  55134,       55136,      55140,      55138,      55142,    55144,      55146,      55148 },     // 自動調整ゲイン2 SR*****にデフォルト値、HR*****には設定値
    {  55102,       55104,      55108,      55106,      55110,    55112,      55114,      55116 },     // 慣性モーメント比 SR*****にデフォルト値、HR*****には設定値
    {  55998,       56000,      56004,      56002,      56006,    56008,      56010,      56012 },     // page2 PA1_70:自動ノッチ選択HR*****
    {  56014,       56016,      56020,      56018,      56022,    56024,      56026,      56028 },     // ノッチフィルタ周波数1SR****設定値
    {  56062,       56064,      56068,      56066,      56070,    56072,      56074,      56076 },     // ノッチフィルタ周波数2SR****設定値
    {  56030,       56032,      56036,      56034,      56038,    56040,      56042,      56044 },     // PA1_72:ノッチフィルタ1減衰量
    {  56078,       56080,      56084,      56082,      56086,    56088,      56090,      56092 },     // PA1_75:ノッチフィルタ2減衰量
    {   2977,        2978,       2980,       2979,       2981,     2982,       2983,       2984 },     // デジタル監視　整定時間（ｍｓ）
    {   2985,        2986,       2988,       2987,       2989,     2990,       2991,       2992 },     // スピード（P)
    {  56110,       56112,      56116,      56114,      56118,    56120,      56122,      56124 },     // page3 PA1_77:自動制振制御選択HR*****
    {  56126,       56128,      56132,      56130,      56134,    56136,      56138,      56140 },     // 制振制御周波数1 SR*****にデフォルト値、HR*****には設定値
    {  56158,       56160,      56164,      56162,      56166,    56168,      56170,      56172 },     // 制振制御周波数2 SR*****にデフォルト値、HR*****には設定値
    {  56254,       56256,      56260,      56258,      56262,    56264,      56266,      56268 },     // 自動検出レベル SR*****にデフォルト値、HR*****には設定値
    {      0,           0,          0,          0,          0,        0,          0,          0 },     // page4 最大トルク変化量
    {  58030,       58032,      58036,      58034,      58038,    58040,      58042,      58044 },     // 衝突検知閾値 SR*****にデフォルト値、HR*****には設定値
    {   1950,        1950,       1950,       1950,       1950,     1950,       1950,       1950 },     // 節約設定時間HR（****）
};

const QList<QList<quint16>> SIMPLE_SET_PARAM_ADDR_SIZE_LIST = {
    // 軸1(SR/HR)   軸2(SR/HR)  軸3(SR/HR)  軸4(SR/HR)  軸5(SR/HR) 軸6(SR/HR)  軸7(SR/HR)  軸8(SR/HR)
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // page1 チューニングモード HR*****
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // 自動調整ゲイン1 SR*****にデフォルト値、HR*****には設定値
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // 自動調整ゲイン2 SR*****にデフォルト値、HR*****には設定値
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // 慣性モーメント比 SR*****にデフォルト値、HR*****には設定値
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // page2 PA1_70:自動ノッチ選択HR*****
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // ノッチフィルタ周波数1SR****設定値
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // ノッチフィルタ周波数2SR****設定値
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // PA1_72:ノッチフィルタ1減衰量
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // PA1_75:ノッチフィルタ2減衰量
    {      1,           1,          1,          1,          1,        1,          1,          1 },     // デジタル監視　整定時間（ｍｓ）
    {      1,           1,          1,          1,          1,        1,          1,          1 },     // スピード（P)
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // page3 PA1_77:自動制振制御選択HR*****
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // 制振制御周波数1 SR*****にデフォルト値、HR*****には設定値
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // 制振制御周波数2 SR*****にデフォルト値、HR*****には設定値
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // 自動検出レベル SR*****にデフォルト値、HR*****には設定値
    {      0,           0,          0,          0,          0,        0,          0,          0 },     // page4 最大トルク変化量
    {      2,           2,          2,          2,          2,        2,          2,          2 },     // 衝突検知閾値 SR*****にデフォルト値、HR*****には設定値
    {      1,           1,          1,          1,          1,        1,          1,          1 },     // 節約設定時間HR（****)
};

const QList<QList<float>> SIMPLE_SET_PARAM_ADJ_MAGNI_LIST = {
    // 軸1(SR/HR)   軸2(SR/HR)  軸3(SR/HR)  軸4(SR/HR)  軸5(SR/HR) 軸6(SR/HR)  軸7(SR/HR)  軸8(SR/HR)
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // page1 チューニングモード HR*****
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // 自動調整ゲイン1 SR*****にデフォルト値、HR*****には設定値
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // 自動調整ゲイン2 SR*****にデフォルト値、HR*****には設定値
    {   10.0,        10.0,       10.0,       10.0,       10.0,     10.0,       10.0,       10.0 },     // 慣性モーメント比 SR*****にデフォルト値、HR*****には設定値
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // page2 PA1_70:自動ノッチ選択HR*****
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // ノッチフィルタ周波数1SR****設定値
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // ノッチフィルタ周波数2SR****設定値
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // PA1_72:ノッチフィルタ1減衰量
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // PA1_75:ノッチフィルタ2減衰量
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // デジタル監視　整定時間（ｍｓ）
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // スピード（P)
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // page3 PA1_77:自動制振制御選択HR*****
    {   10.0,        10.0,       10.0,       10.0,       10.0,     10.0,       10.0,       10.0 },     // 制振制御周波数1 SR*****にデフォルト値、HR*****には設定値
    {   10.0,        10.0,       10.0,       10.0,       10.0,     10.0,       10.0,       10.0 },     // 制振制御周波数2 SR*****にデフォルト値、HR*****には設定値
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // 自動検出レベル SR*****にデフォルト値、HR*****には設定値
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // page4 最大トルク変化量
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // 衝突検知閾値 SR*****にデフォルト値、HR*****には設定値
    {    1.0,         1.0,        1.0,        1.0,        1.0,      1.0,        1.0,        1.0 },     // 節約設定時間HR（****)
};

const QList<QList<int>> SIMPLE_SET_PARAM_DEFAULT_FORAMT = {
    { 16,16,16,16,16,16,16,16 }
};

#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
// SR
const QList<QList<quint16>> SIMPLE_SET_COMMON_PARAM_ADDR_LIST = {
    { 2761, 2763, 2765, 2767                    },   // page1:チューニングモード/自動調整ゲイン1/自動調整ゲイン2/慣性モーメント比
    { 2769, 2771, 2773, 2775, 2777, 2779, 2781  },   // page2:PA1_70_自動ノッチ選択/ノッチフィルタ周波数1/ノッチフィルタ周波数2/PA1_72_ノッチフィルタ1減衰量/PA1_75_ノッチフィルタ2減衰量/整定時間（ｍｓ）/スピード（P)
    { 2783, 2785, 2787, 2789                    },   // page3:PA1_77_自動制振制御選択/制振制御周波数1/制振制御周波数2/自動検出レベル
    { 2791, 2793                                },   // page4:最大トルク変化量/衝突検知閾値/節約設定時間HR
};

const QList<QList<quint16>> SIMPLE_SET_COMMON_PARAM_ADDR_SIZE_LIST = {
    { 2,    2,    2,   2                        },   // page1:チューニングモード/自動調整ゲイン1/自動調整ゲイン2/慣性モーメント比
    { 2,    2,    2,   2,   2,   2,    2        },   // page2:PA1_70_自動ノッチ選択/ノッチフィルタ周波数1/ノッチフィルタ周波数2/PA1_72_ノッチフィルタ1減衰量/PA1_75_ノッチフィルタ2減衰量/整定時間（ｍｓ）/スピード（P)
    { 2,    2,    2,   2                        },   // page3:PA1_77_自動制振制御選択/制振制御周波数1/制振制御周波数2/自動検出レベル
    { 2,    2                                   },   // page4:最大トルク変化量/衝突検知閾値/節約設定時間HR
};
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC

class SimpleSetViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QObject *simpleSetAxisInfo READ simpleSetAxisInfo CONSTANT)
    Q_PROPERTY(int saveStatus READ saveStatus WRITE setSaveStatus NOTIFY saveStatusChanged)
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    Q_PROPERTY(bool axisDataLoaded READ axisDataLoaded NOTIFY axisDataLoadedChanged)
    Q_PROPERTY(bool pageScrolled READ pageScrolled NOTIFY pageScrolledChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    Q_PROPERTY(int currentAxisIndex MEMBER m_currentAxisIndex NOTIFY currentAxisIndexChanged)
public:
    explicit SimpleSetViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~SimpleSetViewModel();

    Q_INVOKABLE void startModbusOneShot();
    Q_INVOKABLE void stopModbusOneShot();
    Q_INVOKABLE void modbusSetCalcValue(int pageNum, int itemIndex, int axisIndex, int writeValue, float decimalWriteValue);
    Q_INVOKABLE void modbusSaveCalcValue();
    Q_INVOKABLE void modbusGetValues(int pageNum, int itemIndex, int axisIndex, bool isHR);
    Q_INVOKABLE void modbusSetPageMode(int pageNum, int axisIndex, int writeValue);
    Q_INVOKABLE void getPageValues(int pageNum, int axisIndex, bool isHR);
    Q_INVOKABLE int getDispFormat(int pageNum, int itemIndex, int axisIndex);
    QObject *simpleSetAxisInfo() const { return m_simpleSetAxisInfo; }
    Q_INVOKABLE QString modbusGetMaxTorqueValue(int axisIndex, int torque);
    Q_INVOKABLE void modbusGetPageMode(int pageNum, int axisIndex);
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    Q_INVOKABLE void setCurAxisInfo(int axisIndex);
    Q_INVOKABLE void startAxisChanging();
    Q_INVOKABLE void startPageScroll();
    void setAxisDataLoaded(bool axisDataLoaded);
    bool axisDataLoaded() { return m_axisDataLoaded; }
    void setPageScrolled(bool pageScrolled);
    bool pageScrolled() { return m_pageScrolled; }
    Q_INVOKABLE void startPageChanging();
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    void setSaveStatus(int saveStatus);
    int saveStatus() const { return m_saveStatus; }

private:
    SimpleSetAxisInfo                   *m_simpleSetAxisInfo = nullptr;
    AxisInfo                            *m_axisInfo = AxisInfo::get_instance();

    QList<QList<quint16>>               m_axisInfoDataTable;
    QList<QList<int>>                   m_axisInfoDataDispFormat;
    QList<QList<int>>                   m_dispItemNum;
    QList<QList<QString>>               m_drvParamDataStrTable;
    QList<QList<int>>                   m_drvParamDataTable[DRV_PARAM_DATA_MAX_PAGE_NUM];
    QList<QList<int>>                   m_drvParamDispFormat[DRV_PARAM_DATA_MAX_PAGE_NUM];
    int                                 pageTopTableIndex[SIMPLE_SET_DATA_MAX_PAGE_NUM] = { 0, 4, 11, -1};
    int                                 pageItemNum[SIMPLE_SET_DATA_MAX_PAGE_NUM] = { 4, 7, 4, 2};
    int                                 pageReadSize[SIMPLE_SET_DATA_MAX_PAGE_NUM] = { 49, 81, 49, 1};
    int                                 mCurPageNum = 1;
    int                                 mCurAxisIndex = 0;
    bool                                mIsModbusConnectOneShot = false;
    const QList<int>                    AXIS_3_POS_ADDR_LIST = {2805, 2807, 2809};

    stModbusPeridiocMonitor initModbusPageCmd[SIMPLE_SET_MODBUS_CMD_LIST_NUM] = {
        { QModbusPdu::FunctionCode::ReadInputRegisters, 0x0BA1, 0x0010 },        // デジタル監視　整定時間（ｍｓ）～ スピード（P)   FuncCode:0x04 addr:SR2977 size:16
    };
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    int       mCurrentAxisIndex = 0;
    int       mModbusAxisIndex = 0;
    bool      m_axisChanging = false;
    bool      m_axisDataLoaded = false;
    bool      m_pageScroll = false;
    int       mTimerId = 0;
    bool      mIsTimerStatus = false;
    int       mPageScrollTimerCnt = 0;
    bool      m_pageScrolled = false;
    bool      m_pageChanging = false;
    bool      m_dataLoaded = false;

    void modbusSetAxisInfo(int axisIndex);
    void timerStart(int timeOut);
    void timerStop();
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    int m_saveStatus = 0;
    bool mIsSaveStart = false;
    bool mIsSaveEnd = false;
    int mTimerCnt = 0;
    int       m_currentAxisIndex = 0;

    void initSettingPenSetData();
    void initPageCommand();
    void startModbusMonitoring();
    void stopModbusMonitoring();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void modbusGetValuesHR(int pageNum, int itemIndex, int axisIndex);
    void modbusGetValuesSR(int pageNum, int itemIndex, int axisIndex);
    void getPageValuesHR(int pageNum, int axisIndex);
    void getPageValuesSR(int pageNum, int axisIndex);
    void updateModbusValuesHR(stModBusCommunicationData &data);
    void updateModbusValuesSR(stModBusCommunicationData &data);
    void modbusIndividualGetValuesHR(int pageNum, int itemIndex, int axisIndex);
    void modbusIndividualGetValuesSR(int pageNum, int itemIndex, int axisIndex);

protected:
    void onActivate() override;
    void onDeactivate() override;
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    void timerEvent(QTimerEvent *timerEvent);
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void updateSimpleSet(stModBusCommunicationData command);
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    void endModbusAxisWriteReady(QList<int> writeData);
    void onEndModbusPageWriteReady(QList<int> writeData);
    void onFinished();
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    void onDataSaveFinished();

signals:
    void saveStatusChanged(int saveStatus);
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    void axisDataLoadedChanged(bool axisDataLoaded);
    void pageScrolledChanged(bool pageScrolled);
    void dataLoadedChanged(bool dataLoaded);
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    void currentAxisIndexChanged(int currentAxisIndex);
};

#endif // SIMPLESETVIEWMODEL_H
