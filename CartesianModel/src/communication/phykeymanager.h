#ifndef PHYKEYMANAGER_H
#define PHYKEYMANAGER_H

#include <QObject>
#include <QBitArray>
#include <QMutex>
#include <QtGlobal>
#include <stdlib.h>
#include <cstdlib>
#include "corelibgpiocontrol.h"
#include "corelibjogcontrol.h"
#include "resourcemanager.h"
#include <QElapsedTimer>
#include <QDebug>

#define OPERABLE_SW_STATUS_DO       669
#define PHY_KEY_CHATTERING_PREVENTION_NUM   3

typedef struct {
    uchar jogDirection;
    int jogCounter;
} stUpdateJogInfo;//jog信息(方向&计数)

typedef struct {
    int magnification;
    int division;
    int multiplication;
} stJogMagnificationInfo;//jog速度档位倍率信息

class PhyKeyManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool jogDialSwitchInvalid READ jogDialSwitchInvalid WRITE setJogDialSwitchInvalid NOTIFY jogDialSwitchInvalidChanged)
    Q_PROPERTY(bool jogDialSwitchValid READ jogDialSwitchValid WRITE setJogDialSwitchValid NOTIFY jogDialSwitchValidChanged)
    Q_PROPERTY(bool jogScrollDown READ jogScrollDown WRITE setJogScrollDown NOTIFY jogScrollDownChanged)
    Q_PROPERTY(bool jogScrollUp READ jogScrollUp WRITE setJogScrollUp NOTIFY jogScrollUpChanged)
    Q_PROPERTY(bool memblemSwKey01Press READ memblemSwKey01Press WRITE setMemblemSwKey01Press NOTIFY memblemSwKey01PressChanged)
    Q_PROPERTY(bool memblemSwKey02Press READ memblemSwKey02Press WRITE setMemblemSwKey02Press NOTIFY memblemSwKey02PressChanged)
    Q_PROPERTY(bool memblemSwKey03Press READ memblemSwKey03Press WRITE setMemblemSwKey03Press NOTIFY memblemSwKey03PressChanged)
    Q_PROPERTY(bool memblemSwKey04Press READ memblemSwKey04Press WRITE setMemblemSwKey04Press NOTIFY memblemSwKey04PressChanged)
    Q_PROPERTY(bool memblemSwKey05Press READ memblemSwKey05Press WRITE setMemblemSwKey05Press NOTIFY memblemSwKey05PressChanged)
    Q_PROPERTY(bool memblemSwKey06Press READ memblemSwKey06Press WRITE setMemblemSwKey06Press NOTIFY memblemSwKey06PressChanged)
    Q_PROPERTY(bool memblemSwKey07Press READ memblemSwKey07Press WRITE setMemblemSwKey07Press NOTIFY memblemSwKey07PressChanged)
    Q_PROPERTY(bool memblemSwKey08Press READ memblemSwKey08Press WRITE setMemblemSwKey08Press NOTIFY memblemSwKey08PressChanged)
    Q_PROPERTY(bool memblemSwKey09Press READ memblemSwKey09Press WRITE setMemblemSwKey09Press NOTIFY memblemSwKey09PressChanged)
    Q_PROPERTY(bool memblemSwKey10Press READ memblemSwKey10Press WRITE setMemblemSwKey10Press NOTIFY memblemSwKey10PressChanged)
    Q_PROPERTY(bool memblemSwKey11Press READ memblemSwKey11Press WRITE setMemblemSwKey11Press NOTIFY memblemSwKey11PressChanged)
    Q_PROPERTY(bool memblemSwKey12Press READ memblemSwKey12Press WRITE setMemblemSwKey12Press NOTIFY memblemSwKey12PressChanged)
    Q_PROPERTY(bool memblemSwKey13Press READ memblemSwKey13Press WRITE setMemblemSwKey13Press NOTIFY memblemSwKey13PressChanged)
    Q_PROPERTY(bool memblemSwKey14Press READ memblemSwKey14Press WRITE setMemblemSwKey14Press NOTIFY memblemSwKey14PressChanged)
    Q_PROPERTY(bool memblemSwKey15Press READ memblemSwKey15Press WRITE setMemblemSwKey15Press NOTIFY memblemSwKey15PressChanged)
    Q_PROPERTY(bool memblemSwKey16Press READ memblemSwKey16Press WRITE setMemblemSwKey16Press NOTIFY memblemSwKey16PressChanged)
    Q_PROPERTY(bool memblemSwKey17Press READ memblemSwKey17Press WRITE setMemblemSwKey17Press NOTIFY memblemSwKey17PressChanged)
    Q_PROPERTY(bool memblemSwKey18Press READ memblemSwKey18Press WRITE setMemblemSwKey18Press NOTIFY memblemSwKey18PressChanged)
    Q_PROPERTY(int selectSwMode READ selectSwMode WRITE setSelectSwMode NOTIFY selectSwModeChanged)
    Q_PROPERTY(int jogCounter READ jogCounter WRITE setJogCounter NOTIFY jogCounterChanged)
    Q_PROPERTY(bool jogDialSwitchLongPress READ jogDialSwitchLongPress WRITE setJogDialSwitchLongPress NOTIFY jogDialSwitchLongPressChanged)
    Q_PROPERTY(bool operableSwStatus READ operableSwStatus WRITE setOperableSwStatus NOTIFY operableSwStatusChanged)
    Q_PROPERTY(Enums::JogMagnification jogMagnificationSetting MEMBER m_jogMagnificationSetting NOTIFY jogMagnificationSettingChanged)
    Q_PROPERTY(bool jogDialSwitchRelease MEMBER m_jogDialSwitchRelease NOTIFY jogDialSwitchReleaseChanged)
    Q_PROPERTY(int currentBuzzerPitch MEMBER m_currentBuzzerPitch NOTIFY currentBuzzerPitchChanged)
    Q_PROPERTY(bool currentBuzzerState MEMBER m_currentBuzzerState NOTIFY currentBuzzerStateChanged)
    Q_PROPERTY(int currentBuzzerIndex MEMBER m_currentBuzzerIndex NOTIFY currentBuzzerIndexChanged)

public:
    explicit PhyKeyManager(QObject *parent = nullptr);
    ~PhyKeyManager();

    enum JogOperationMode {
        JogOperationModeInValid,
        JogOperationModeValid,
    };//手轮操作模式(有效&无效)
    enum SelectSwitchStatus {
        SelectSwitchStatusInvalid = -1,
        SelectSwitchStatusOff,
        SelectSwitchStatusAuto,
        SelectSwitchStatusManual,
        SelectSwitchStatusOrigin,
        SelectSwitchStatusSetting,
    };//三段选择开关使用状态
    enum MemblemSelectSwitchStatus {
        MemblemSelectSwitchStatusInvalid = 0,
        MemblemSelectSwitchStatusEven,
        MemblemSelectSwitchStatusOdd,
    };//实体gpio状态(基数&偶数)
    struct SelectSwInfo {
        bool select01;
        bool select02;
        bool select03;
    };//三段选择开关信息
    struct MemblemSwInfo {
        bool key01;
        bool key02;
        bool key03;
        bool key04;
        bool key05;
        bool key06;
        bool key07;
        bool key08;
        bool key09;
        bool key10;
        bool key11;
        bool key12;
        bool key13;
        bool key14;
        bool key15;
        bool key16;
        bool key17;
        bool key18;
    };//18个实体按键信息

    void monitorByComThreadTriger();
    void monitorByTimerTriger();

    void initResourceManager(ResourceManager* resourceManager);
    void initJogSetting(ResourceManager* resourceManager);
    Q_INVOKABLE void setJogCounterThreshold(ResourceManager* resourceManager);
    void setJogMagnification(ResourceManager* resourceManager);
    void setJogPulseThreshold(ResourceManager* resourceManager);
    Q_INVOKABLE int getJogCounterThreshold();
    Q_INVOKABLE void writeJogMagnification(int jogMagnificationIndex);
    Q_INVOKABLE void writeJogDefaultValue();
    void setJogDialSpeed(Enums::JogDialSpeed speed);
    Q_INVOKABLE void setTouchSelectStatus(bool isTouchSelectStatus);//实体选择开关不使用 屏幕切换手自原点
    Q_INVOKABLE void setStepOperate(bool isonoff);

    int jogMagnification() const { return m_jogMagnification; }
    bool jogDialSwitchInvalid() const { return m_jogDialSwitchInvalid; }
    bool jogDialSwitchValid() const { return m_jogDialSwitchValid; }
    bool jogScrollDown() const { return m_jogScrollDown; }
    bool jogScrollUp() const { return m_jogScrollUp; }
    bool memblemSwKey01Press()  { return m_memblemSwKey01Press; }
    bool memblemSwKey02Press()  { return m_memblemSwKey02Press; }
    bool memblemSwKey03Press() const { return m_memblemSwKey03Press; }
    bool memblemSwKey04Press() const { return m_memblemSwKey04Press; }
    bool memblemSwKey05Press() const { return m_memblemSwKey05Press; }
    bool memblemSwKey06Press() const { return m_memblemSwKey06Press; }
    bool memblemSwKey07Press() const { return m_memblemSwKey07Press; }
    bool memblemSwKey08Press() const { return m_memblemSwKey08Press; }
    bool memblemSwKey09Press() const { return m_memblemSwKey09Press; }
    bool memblemSwKey10Press() const { return m_memblemSwKey10Press; }
    bool memblemSwKey11Press() const { return m_memblemSwKey11Press; }
    bool memblemSwKey12Press() const { return m_memblemSwKey12Press; }
    bool memblemSwKey13Press() const { return m_memblemSwKey13Press; }
    bool memblemSwKey14Press() const { return m_memblemSwKey14Press; }
    bool memblemSwKey15Press() const { return m_memblemSwKey15Press; }
    bool memblemSwKey16Press() const { return m_memblemSwKey16Press; }
    bool memblemSwKey17Press() const { return m_memblemSwKey17Press; }
    bool memblemSwKey18Press() const { return m_memblemSwKey18Press; }
    int selectSwMode() const { return m_selectSwMode; }
    int jogCounter() const { return mJogCounter; }
    bool jogDialSwitchLongPress() const { return m_jogDialSwitchLongPress; }
    bool operableSwStatus() const { return m_operableSwStatus; }
    stUpdateJogInfo getCurJogCounterInfo() { return mUpdateJogInfo;}
    Q_INVOKABLE int getStartupSelectSwStatus() { return (int)mStartSelectSwStatus; }
    bool initLedDevice();
    void deinitLedDevice();
    void initSelectSwitchInfo();
    void setCurrentBuzzerState(bool isonoff);
    void setCurrentBuzzerIndex(quint8 type);

    enum MemblemKeyNo {
        MemblemKeyAllOff = 0,
        MemblemKey01,
        MemblemKey02,
        MemblemKey03,
        MemblemKey04,
        MemblemKey05,
        MemblemKey06,
        MemblemKey07,
        MemblemKey08,
        MemblemKey09,
        MemblemKey10,
        MemblemKey11,
        MemblemKey12,
        MemblemKey13,
        MemblemKey14,
        MemblemKey15,
        MemblemKey16,
        MemblemKey17,
        MemblemKey18
    };

    Q_INVOKABLE void ctrlLed(bool isOnOff, int ledType);
    Q_INVOKABLE void ctrlBuzzer();

 /*******************PC Demon 设置模拟按键状态 ***********************/
#ifndef REAL_TARGET
    void emulatedJogDialSwOn() {
        if(mGpioControl != nullptr) {
            mGpioControl->emulateJogDialSwOn();
        }
    }
    void emulatedJogDialSwOff() {
        if(mGpioControl != nullptr) {
            mGpioControl->emulateJogDialSwOff();
        }
    }
    Q_INVOKABLE void setEmulatedDeadManSwStatus(bool isDeadManSwitch);
    Q_INVOKABLE void setEmulatedMemblemSwStatus(int memblemKeySwitchNo);
    Q_INVOKABLE void setEmulatedJogDialSwStatus(bool isJogDialSwitch);
    Q_INVOKABLE void incrementEmulatedJogCount();
    Q_INVOKABLE void decrementEmulatedJogCount();
    Q_INVOKABLE void setEmulatedSelectSwStatus(int selectSwStatus);
#endif

private:
    CoreLibJogControl                           *mJogControl;
    CoreLibGpioControl                          *mGpioControl;
    PhyKeyManager::SelectSwitchStatus           mSelectSwStatus;
    PhyKeyManager::SelectSwitchStatus           mPreSelectSwStatus;
    PhyKeyManager::SelectSwitchStatus           mStartSelectSwStatus;
    PhyKeyManager::MemblemSelectSwitchStatus    mPreMemblemSelectSwStatus;
    PhyKeyManager::MemblemSelectSwitchStatus    mMemblemSelectSwStatus;

    bool                mIsTouchSelectSw;//实体选择开关不使用 屏幕切换手自原点
    bool                mIsPreOperableSw;
    bool                mIsOperableSw;// 实体选择开关使用or不使用 false:Off true:On
    uchar               mJogOpeMode;//手轮操作是否有效(0:Invalid 1:valid)
    int                 mPreJogCounter;
    int                 mScrollPreJogCounter;
    int                 mJogCounter;                        // jogcounter
    int                 mCurJogCounter;
    uchar               mPreJogDirection;
    uchar               mJogDirection;                    // jogdirection
    const uchar         mHoldInputDataBufCount = 2; // input-buf hold count
    int                 mJogCounterThreshold;
    bool                mPreJogDialSwStatus = false;
    bool                mJogDialSwStatus = false;
    bool                mStepOperate;//step mode Jog use

    const int           mMemblemKIN0SwBitNo = 0;        // bit0:KIN0(Key1,2)
    const int           mMemblemKIN1SwBitNo = 1;        // bit1:KIN1(Key3,4)
    const int           mMemblemKIN2SwBitNo = 2;        // bit2:KIN2(Key5,6)
    const int           mMemblemKIN3SwBitNo = 3;        // bit3:KIN3(Key7,8)
    const int           mMemblemKIN4SwBitNo = 4;        // bit4:KIN4(Key9,10)
    const int           mMemblemKIN5SwBitNo = 5;        // bit5:KIN5(Key11,12)
    const int           mMemblemKIN6SwBitNo = 6;        // bit6:KIN6(Key13,14)
    const int           mMemblemKIN7SwBitNo = 7;        // bit7:KIN7(Key15,16)
    const int           mMemblemKIN8SwBitNo = 8;        // bit8:KIN8(Key17,18)
    const int           mMemblemSEL1SwBitNo = 14;       // bit14:SEL1(メンブレムSWセレクト出力1:偶数)
    const int           mMemblemSEL0SwBitNo = 15;       // bit15:SEL0(メンブレムSWセレクト出力0:奇数)
    const int           mJogDialSwBitNo = 18;           // bit18: ジョグダイヤルスイッチ入力
    const int           mSelectSwAuto = 19;
    const int           mSelectSwOrigin = 20;
    const int           mSelectSwManual = 21;
    const int           mDeadManSwBitNo = 22;           // bit22: 動作可能スイッチ入力
    const uchar         mSelectSwBitMaxNum = 3;
    bool                mIsPassOriginFlag = false;
    const int           mJogDialSwJudgeLongPressCnt = 1000; // 2秒間(2ms*1000)
    int                 mJogDialSwMonitorCounter = 0;
    int                 mHisCount = 0;

    QList<QBitArray>    mInputDataBuffer;
    SelectSwInfo        mPreSelectSwInfo;
    SelectSwInfo        mSelectSwInfo;
    MemblemSwInfo       mPreMemblemSwInfo;
    MemblemSwInfo       mMemblemSwInfo;
    MemblemSwInfo       mHisMemblemSwInfo[PHY_KEY_CHATTERING_PREVENTION_NUM];
    stUpdateJogInfo     mUpdateJogInfo;

    int                 m_jogMagnification = 0;
    int                 mInternalJogCounter = 0;
    int                 m_jogCounterDivision = 4;
    stUpdateJogInfo     mPreJogCounterInfo = {0, 0};
    stUpdateJogInfo     mJogCounterInfo = {0, 0};
    bool                mIsPassJogOriginFlag = false;
    QMutex              *mMutexJogCounterUpdate;

    int                 m_selectSwMode = -2;
    bool                m_jogDialSwitchInvalid = false;
    bool                m_jogDialSwitchValid = false;
    bool                m_jogScrollDown = false;
    bool                m_jogScrollUp = false;
    bool                m_memblemSwKey01Press = false;
    bool                m_memblemSwKey02Press = false;
    bool                m_memblemSwKey03Press = false;
    bool                m_memblemSwKey04Press = false;
    bool                m_memblemSwKey05Press = false;
    bool                m_memblemSwKey06Press = false;
    bool                m_memblemSwKey07Press = false;
    bool                m_memblemSwKey08Press = false;
    bool                m_memblemSwKey09Press = false;
    bool                m_memblemSwKey10Press = false;
    bool                m_memblemSwKey11Press = false;
    bool                m_memblemSwKey12Press = false;
    bool                m_memblemSwKey13Press = false;
    bool                m_memblemSwKey14Press = false;
    bool                m_memblemSwKey15Press = false;
    bool                m_memblemSwKey16Press = false;
    bool                m_memblemSwKey17Press = false;
    bool                m_memblemSwKey18Press = false;
    bool                m_jogDialSwitchLongPress = false;
    bool                m_jogDialSwLongPressInvalid = false;
    bool                m_operableSwStatus = false;
    int                 m_currentBuzzerPitch = 0;
    bool                m_currentBuzzerState = 0;
    int                 m_currentBuzzerIndex = 0;

    uchar               mJogDefaultMagnification = 1;
    bool                m_jogDialSwitchRelease;
    QList<stJogMagnificationInfo>   m_jogMagnificationInfo;
    QList<int>                      m_jogPulseThreshold;
    Enums::JogMagnification         m_jogMagnificationSetting = Enums::JogMagnification1;
    Enums::JogDialSpeed             m_jogDialSpeed = Enums::JogDialSpeedUnknown;

    ResourceManager                 *m_resource;
    bool                            m_calibrateLongPress = false;//长按校准屏幕
    const int                       m_calibrateLongPressCnt = 40; // 1秒間(25ms*40)
    int                             m_calibrateLongPressCounter = 0;

    bool mHisOperableSw[PHY_KEY_CHATTERING_PREVENTION_NUM];
    bool mHisJogDialSw[PHY_KEY_CHATTERING_PREVENTION_NUM];

    QElapsedTimer       *m_elapsedTimer = nullptr;

/*******************************************************************************/
    void initialize();
    void initJogInfo();
    void initGpioInfo();
    void initMemblemSwitchInfo();
    void initCooperationMemblemSw();
    void initHisMemblemSwitchInfo();
    void initHisEachMemblemSwitchInfo(int keyNo);
    void initHisOperableSwitchInfo();
    void initHisJogDialSwitchInfo();
    bool analyzeJogInfo(stJogInfo &jogInfo);
    void analyzeSwitchInfo(QBitArray *bitArray);
    void analyzeDeadManSw(int bitIndex, bool value, int bufIndexCount);

    void analyzeSelectSw(int bitIndex, bool value);
    void analyzeJogDialSw(int bitIndex, bool value, int bufIndexCount);
    void analyzeCooperationMemblemSw(int bitIndex, bool value);
    void analyzeModBusCooperationMemblemSw(int bitIndex, bool value);
    void decideSelectSw(SelectSwInfo selectSwInfo);
    bool judgeUpdateMemblemSw();
    bool judgeUpdateModBusMemblemSw(bool *iSMemblemRelease);
    void notifySwitchInfo();
    void notifyReleaseMemblemSwInfo();
    void notifyJogInfo();
    void onUpdateJogDialSwInvalidInfo(bool jogDialSwitchInvalid);
    void onUpdateJogDialSwValidInfo(bool jogDialSwitchValid);
    void onUpdateJogScrollDown(bool jogScrollDown);
    void onUpdateJogScrollUp(bool jogScrollUp);
    void onUpdateMemblemSwInfo(PhyKeyManager::MemblemSwInfo memblemSwInfo);
    void onUpdateModBusMemblemSwInfo(PhyKeyManager::MemblemSwInfo memblemSwInfo);
    void onUpdateSelectSwInfo(PhyKeyManager::SelectSwitchStatus selectSwStatus);
    void onUpdateJogDialSwLongPress(bool jogDialSwitchLongPress);
    void onUpdateDeadmanSwInfo(bool operableSwStatus);
    bool judgeUpdateModBusOperableSw(bool *iSOperableSwRelease);
    void onUpdateModBusOperableSwInfo(bool operableSwStatus);
    void notifyReleaseOperableSwInfo();
    bool holdJogCounter();
    int getJogPassJogOriginOffset(int jogCounterDiff);
    void resetJogCounterInfo();
    void onUpdateJogCounterInfo(stUpdateJogInfo updateJogInfo);
    void setJogMagnification(int jogMagnification);
    void setJogMagnificationSetting(Enums::JogMagnification jogMagnification);
    void judgeJogScrollNotify();
    void updateJogDialSpeed();
    void onUpdateJogDialSwRelease(bool jogDialSwitchRelease);

    void checkCalibrateLongPress();

public slots:
    void setJogDialSwitchInvalid(bool jogDialSwitchInvalid){
        if (m_jogDialSwitchInvalid == jogDialSwitchInvalid)
            return;

        m_jogDialSwitchInvalid = jogDialSwitchInvalid;
    }
    void setJogDialSwitchValid(bool jogDialSwitchValid){
        if (m_jogDialSwitchValid == jogDialSwitchValid)
            return;

        m_jogDialSwitchValid = jogDialSwitchValid;
    }
    void setJogScrollDown(bool jogScrollDown){
        if (m_jogScrollDown == jogScrollDown)
            return;

        m_jogScrollDown = jogScrollDown;
    }
    void setJogScrollUp(bool jogScrollUp){
        if (m_jogScrollUp == jogScrollUp)
            return;

        m_jogScrollUp = jogScrollUp;
    }
    void setMemblemSwKey01Press(bool memblemSwKey01Press){
        if (m_memblemSwKey01Press == memblemSwKey01Press)
            return;

        m_memblemSwKey01Press = memblemSwKey01Press;
        emit memblemSwKey01PressChanged(memblemSwKey01Press);
    }
    void setMemblemSwKey02Press(bool memblemSwKey02Press){
        if (m_memblemSwKey02Press == memblemSwKey02Press)
            return;

        m_memblemSwKey02Press = memblemSwKey02Press;
        emit memblemSwKey02PressChanged(memblemSwKey02Press);
    }
    void setMemblemSwKey03Press(bool memblemSwKey03Press){
        if (m_memblemSwKey03Press == memblemSwKey03Press)
            return;

        m_memblemSwKey03Press = memblemSwKey03Press;
        emit memblemSwKey03PressChanged(memblemSwKey03Press);
    }
    void setMemblemSwKey04Press(bool memblemSwKey04Press){
        if (m_memblemSwKey04Press == memblemSwKey04Press)
            return;

        m_memblemSwKey04Press = memblemSwKey04Press;
        emit memblemSwKey04PressChanged(memblemSwKey04Press);
    }
    void setMemblemSwKey05Press(bool memblemSwKey05Press){
        if (m_memblemSwKey05Press == memblemSwKey05Press)
            return;

        m_memblemSwKey05Press = memblemSwKey05Press;
        emit memblemSwKey05PressChanged(memblemSwKey05Press);
    }
    void setMemblemSwKey06Press(bool memblemSwKey06Press){
        if (m_memblemSwKey06Press == memblemSwKey06Press)
            return;

        m_memblemSwKey06Press = memblemSwKey06Press;
        emit memblemSwKey06PressChanged(memblemSwKey06Press);
    }
    void setMemblemSwKey07Press(bool memblemSwKey07Press){
        if (m_memblemSwKey07Press == memblemSwKey07Press)
            return;

        m_memblemSwKey07Press = memblemSwKey07Press;
        emit memblemSwKey07PressChanged(memblemSwKey07Press);
    }
    void setMemblemSwKey08Press(bool memblemSwKey08Press){
        if (m_memblemSwKey08Press == memblemSwKey08Press)
            return;

        m_memblemSwKey08Press = memblemSwKey08Press;
        emit memblemSwKey08PressChanged(memblemSwKey08Press);
    }
    void setMemblemSwKey09Press(bool memblemSwKey09Press){
        if (m_memblemSwKey09Press == memblemSwKey09Press)
            return;

        m_memblemSwKey09Press = memblemSwKey09Press;
        emit memblemSwKey09PressChanged(memblemSwKey09Press);
    }
    void setMemblemSwKey10Press(bool memblemSwKey10Press){
        if (m_memblemSwKey10Press == memblemSwKey10Press)
            return;

        m_memblemSwKey10Press = memblemSwKey10Press;
        emit memblemSwKey10PressChanged(memblemSwKey10Press);
    }
    void setMemblemSwKey11Press(bool memblemSwKey11Press){
        if (m_memblemSwKey11Press == memblemSwKey11Press)
            return;

        m_memblemSwKey11Press = memblemSwKey11Press;
        emit memblemSwKey11PressChanged(memblemSwKey11Press);
    }
    void setMemblemSwKey12Press(bool memblemSwKey12Press){
        if (m_memblemSwKey12Press == memblemSwKey12Press)
            return;

        m_memblemSwKey12Press = memblemSwKey12Press;
        emit memblemSwKey12PressChanged(memblemSwKey12Press);
    }

    void setMemblemSwKey13Press(bool memblemSwKey13Press){
        if (m_memblemSwKey13Press == memblemSwKey13Press)
            return;

        m_memblemSwKey13Press = memblemSwKey13Press;
        emit memblemSwKey13PressChanged(memblemSwKey13Press);
    }
    void setMemblemSwKey14Press(bool memblemSwKey14Press){
        if (m_memblemSwKey14Press == memblemSwKey14Press)
            return;

        m_memblemSwKey14Press = memblemSwKey14Press;
        emit memblemSwKey14PressChanged(memblemSwKey14Press);
    }
    void setMemblemSwKey15Press(bool memblemSwKey15Press){
        if (m_memblemSwKey15Press == memblemSwKey15Press)
            return;

        m_memblemSwKey15Press = memblemSwKey15Press;
        emit memblemSwKey15PressChanged(memblemSwKey15Press);
    }
    void setMemblemSwKey16Press(bool memblemSwKey16Press){
        if (m_memblemSwKey16Press == memblemSwKey16Press)
            return;

        m_memblemSwKey16Press = memblemSwKey16Press;
        emit memblemSwKey16PressChanged(memblemSwKey16Press);
    }
    void setMemblemSwKey17Press(bool memblemSwKey17Press){
        if (m_memblemSwKey17Press == memblemSwKey17Press)
            return;

        m_memblemSwKey17Press = memblemSwKey17Press;
        emit memblemSwKey17PressChanged(memblemSwKey17Press);
    }
    void setMemblemSwKey18Press(bool memblemSwKey18Press){
        if (m_memblemSwKey18Press == memblemSwKey18Press)
            return;

        m_memblemSwKey18Press = memblemSwKey18Press;
        emit memblemSwKey18PressChanged(memblemSwKey18Press);
    }

    void setSelectSwMode(int selectSwMode){
        m_selectSwMode = selectSwMode;
        emit selectSwModeChanged(m_selectSwMode);
    }
    void setJogCounter(int jogCounter){
        mJogCounter = jogCounter;
        emit jogCounterChanged(mJogCounter);
    }
    void setJogDialSwitchLongPress(bool jogDialSwitchLongPress){
        if (m_jogDialSwitchLongPress == jogDialSwitchLongPress)
            return;

        m_jogDialSwitchLongPress = jogDialSwitchLongPress;
    }
    void setOperableSwStatus(bool operableSwStatus){
        if (m_operableSwStatus == operableSwStatus)
            return;

        m_operableSwStatus = operableSwStatus;
        emit operableSwStatusChanged(m_operableSwStatus);
    }

signals:
    void updateSelectSwInfo(PhyKeyManager::SelectSwitchStatus selectSwStatus);
    void updateJogDialSwInvalidInfo(bool jogDialSwInvalid);
    void updateJogDialSwValidInfo(bool jogDialSwValid);
    void updateJogScrollDown(bool jogScrollStatus);
    void updateJogScrollUp(bool jogScrollStatus);
    void updateMemblemSwInfo(PhyKeyManager::MemblemSwInfo memblemSwInfo);
    void updateReleaseMemblemSwInfo(int releaseMemblemInfo);
    void updateOperableSwInfo(bool operableSwStatus);
    void updateReleaseOperableSwInfo(bool releaseOperableSwStatus);

    void jogDialSwitchInvalidChanged(bool jogDialSwitchInvalid);
    void jogDialSwitchValidChanged(bool jogDialSwitchValid);
    void jogScrollDownChanged(bool jogScrollDown);
    void jogScrollUpChanged(bool jogScrollUp);
    void memblemSwKey01PressChanged(bool memblemSwKey01Press);
    void memblemSwKey02PressChanged(bool memblemSwKey02Press);
    void memblemSwKey03PressChanged(bool memblemSwKey03Press);
    void memblemSwKey04PressChanged(bool memblemSwKey04Press);
    void memblemSwKey05PressChanged(bool memblemSwKey05Press);
    void memblemSwKey06PressChanged(bool memblemSwKey06Press);
    void memblemSwKey07PressChanged(bool memblemSwKey07Press);
    void memblemSwKey08PressChanged(bool memblemSwKey08Press);
    void memblemSwKey09PressChanged(bool memblemSwKey09Press);
    void memblemSwKey10PressChanged(bool memblemSwKey10Press);
    void memblemSwKey11PressChanged(bool memblemSwKey11Press);
    void memblemSwKey12PressChanged(bool memblemSwKey12Press);
    void memblemSwKey13PressChanged(bool memblemSwKey13Press);
    void memblemSwKey14PressChanged(bool memblemSwKey14Press);
    void memblemSwKey15PressChanged(bool memblemSwKey15Press);
    void memblemSwKey16PressChanged(bool memblemSwKey16Press);
    void memblemSwKey17PressChanged(bool memblemSwKey17Press);
    void memblemSwKey18PressChanged(bool memblemSwKey18Press);
    void selectSwModeChanged(int selectSwMode);
    void jogCounterChanged(int jogCounter);
    void jogDialSwitchLongPressChanged(bool jogDialSwitchLongPress);
    void operableSwStatusChanged(bool operableSwStatus);
    void jogMagnificationSettingChanged();
    void updateJogCounterInfo(int jogCounter);
    void jogDialSwitchReleaseChanged();
    void currentBuzzerPitchChanged();
    void currentBuzzerStateChanged();
    void currentBuzzerIndexChanged(int currentBuzzerIndex);
    void jogDialSpeedChanged(Enums::JogDialSpeed speed);
};

#endif // PHYKEYMANAGER_H
