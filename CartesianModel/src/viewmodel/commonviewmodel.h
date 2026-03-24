#ifndef COMMONVIEWMODEL_H
#define COMMONVIEWMODEL_H

#include <bitset>
#include <QObject>
#include <QStack>
#include "viewmodelbase.h"
#include "enums.h"
#include "corelibsystemcontrol.h"
#include "axisinfo.h"

#define MAX_USER_LEVEL              5
#define MAX_MANUAL_SPEED            6
#define OPE_MODE_TOP_DO             2296
#define MANIP_MODE1_TOP_DO          28
#define MANIP_MODE2_TOP_DO          30
#define AXIS_STATUS_TOP_DO          1674
#define MEMBLEM_SW_STATUS_DO        1184
#define MEMBLEM_SW_STATUS_BIT_NUM   8
#define MEMBLEM_SW_RELEASE_DO       3744
#define MEMBLEM_SW_RELEASE_BIT_NUM  8
#define ALARM_STATUS_TOP_DI         2624
#define ALARM_STATUS_BIT_NUM        8
#define ERROR_STATUS_TOP_DI         2632
#define ERROR_STATUS_BIT_NUM        8
#define OPE_CMD_STATUS_TOP_DI       2640
#define OPE_CMD_STATUS_BIT_NUM      8
#define SYSTEM_ALARM_STATUS_SR      2721
#define AXIS_ALARM_STATUS_TOP_SR    2713
#define AXIS_ALARM_STATUS_NUM       8
#define USERLEVEL_TOP_HR            1947
#define USERLEVEL_SIZE_NUM          1
#define DATETIME_YEAR_SR            2723
#define DATETIME_MONTH_SR           2724
#define DATETIME_DAY_SR             2725
#define DATETIME_HOUR_SR            2726
#define DATETIME_MINUTE_SR          2727
#define DATETIME_SECOND_SR          2728
#define MANUAL_SPEED_STATUS_HR      1717
#define AUTO_SPEED_STATUS_HR        1937
#define MANUAL_SPEED_STATUS_SR      2700
#define AUTO_SPEED_STATUS_SR        2701
#define MAIN_SCREEN_ID_TOP_DO       1168
#define MAIN_SCREEN_ID_BIT_NUM      8
#define SUB_SCREEN_ID_TOP_DO        1176
#define SUB_SCREEN_ID_BIT_NUM       8
#define MFREE_DO                    1676
#define RETURN_CTRL_HR              1657
#define RETURN_CTRL_VALUE           0x29
#define RESET_CTRL_HR               657
#define RESET_CTRL_VALUE            0x29
#define COMMON_MODBUS_MONITOR_CMD_LIST_NUM  1
#define USER_PASSWORD_TOP_HR        52265
#define DATE_TIME_TOP_HR            1745
#define LANGUAGE_CODE_HR            1951
#define ALARM_MSG_TYPE_START_INDEX              0
#define SYSTEM_ALARM_MSG_TYPE_START_INDEX       510
#define AXIS_ALARM_MSG_TYPE_START_INDEX         1040
#define AXIS_ALARM_MSG_TYPE2_START_INDEX        1170
#define CURRENT_FILE_NUMBER_SR      2702
#define CURRENT_FILE_NAME_SR        2703
#define CURRENT_FILE_NAME_SIZE      10
#define JOG_DIAL_MAGNIFICATION_HR   2535
#define BUZZER_INFO_HR              2542

#define GET_COUNTER_VIEW_SUB_SCREEN_ID(subPage) \
    (subPage == 0 ? Enums::SubScreenId::SubScreenIdTeachingCounterPage1 : \
     subPage == 1 ? Enums::SubScreenId::SubScreenIdTeachingCounterPage2 : \
     subPage == 2 ? Enums::SubScreenId::SubScreenIdTeachingCounterPage3 : \
     subPage == 3 ? Enums::SubScreenId::SubScreenIdTeachingCounterPage4 : \
                    Enums::SubScreenId::SubScreenIdTeachingCounterDefault)

const QList<QList<quint16>> COMMON_SYSTEM_TIME_ADDR_LIST = {
    // year(HR) month(HR) day(HR) hour(HR) minutes(HR) second(HR)
    {  1745,    1746,     1747,   1748,    1749,       1750     }
};

const QList<int> AXIS_5_POS_ADDR_LIST = {2751, 2753, 2755, 2757, 2759};

class AlarmInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString alarmNumber READ alarmNumber WRITE setAlarmNumber NOTIFY alarmNumberChanged)
    Q_PROPERTY(QString alarmDetailsColor READ alarmDetailsColor WRITE setAlarmDetailsColor NOTIFY alarmDetailsColorChanged)
    Q_PROPERTY(int msgNumber READ msgNumber WRITE setMsgNumber NOTIFY msgNumberChanged)
    Q_PROPERTY(int alarmIndex READ alarmIndex WRITE setAlarmIndex NOTIFY alarmIndexChanged)
public:
    typedef enum {
        Axis1,
        Axis2,
        Axis3,
        Axis4,
        Axis5,
        Axis6,
        Axis7,
        Axis8,
        System,
        Normal,
        Unknown,
    } Type;
    Q_ENUM(Type)

    explicit AlarmInfo(QObject *parent = nullptr);
    void setType(Type type);
    Type type() const { return m_type; }
    void setAlarmNumber(const QString &alarmNumber);
    QString alarmNumber() const { return m_alarmNumber; }
    void setAlarmDetailsColor(const QString &alarmDetailsColor);
    QString alarmDetailsColor() const { return m_alarmDetailsColor; }
    void setMsgNumber(const int &msgNumber);
    int msgNumber() const { return m_msgNumber; }
    void setAlarmIndex(const int index);
    int alarmIndex() const { return m_alarmIndex; }

private:
    Type                m_type = Unknown;
    QString             m_alarmNumber = "";
    QString             m_alarmDetailsColor = "";
    int                 m_msgNumber = 0;
    int                 m_alarmIndex = 0;

signals:
    void alarmNumberChanged(const QString &alarmNumber);
    void alarmDetailsColorChanged(const QString &alarmDetailsColor);
    void msgNumberChanged(const int &msgNumber);
    void alarmIndexChanged();
    void typeChanged();
};

class AreaSetPointParser;
class AreaSetPointInfo;

class CommonViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int manipulateMode READ manipulateMode WRITE setManipulateMode NOTIFY manipulateModeChanged)
    Q_PROPERTY(bool operationMode READ operationMode WRITE setOperationMode NOTIFY operationModeChanged)
    Q_PROPERTY(int languageCount READ languageCount WRITE setLanguageCount NOTIFY languageCountChanged)
    Q_PROPERTY(bool axisStatus READ axisStatus WRITE setAxisStatus NOTIFY axisStatusChanged)
    Q_PROPERTY(bool stepOpeStatus READ stepOpeStatus WRITE setStepOpeStatus NOTIFY stepOpeStatusChanged)
    Q_PROPERTY(int currentStatus READ currentStatus WRITE setCurrentStatus NOTIFY currentStatusChanged)
    Q_PROPERTY(int startupStatus READ startupStatus WRITE setStartupStatus NOTIFY startupStatusChanged)
    Q_PROPERTY(QList<int> axisAlarmStatus READ axisAlarmStatus WRITE setAxisAlarmStatus NOTIFY axisAlarmStatusChanged)
    Q_PROPERTY(int userLevel READ userLevel WRITE setUserLevel NOTIFY userLevelChanged)
    Q_PROPERTY(int manualSpeedStatus READ manualSpeedStatus WRITE setManualSpeedStatus NOTIFY manualSpeedStatusChanged)
    Q_PROPERTY(int autoSpeedStatus READ autoSpeedStatus WRITE setAutoSpeedStatus NOTIFY autoSpeedStatusChanged)
    Q_PROPERTY(QString topTitle READ topTitle WRITE setTopTitle NOTIFY topTitleChanged)
    Q_PROPERTY(int operatingMode READ operatingMode WRITE setOperatingMode NOTIFY operatingModeChanged)
    Q_PROPERTY(bool emergencyStatus READ emergencyStatus WRITE setEmergencyStatus NOTIFY emergencyStatusChanged)

    Q_PROPERTY(QString curDate READ curDate WRITE setCurDate NOTIFY curDateChanged)
    Q_PROPERTY(QString curTime READ curTime WRITE setCurTime NOTIFY curTimeChanged)
    Q_PROPERTY(QString curTimeUptoMinutes READ curTimeUptoMinutes WRITE setCurTimeUptoMinutes NOTIFY curTimeUptoMinutesChanged)
    Q_PROPERTY(QString year MEMBER m_year NOTIFY yearChanged)
    Q_PROPERTY(QString month MEMBER m_month NOTIFY monthChanged)
    Q_PROPERTY(QString day MEMBER m_day NOTIFY dayChanged)
    Q_PROPERTY(QString hour MEMBER m_hour NOTIFY hourChanged)
    Q_PROPERTY(QString minutes MEMBER m_minutes NOTIFY minutesChanged)
    Q_PROPERTY(QString seconds MEMBER m_seconds NOTIFY secondsChanged)

    Q_PROPERTY(bool systemTimeStatus MEMBER m_systemTimeStatus NOTIFY systemTimeStatusChanged)
    Q_PROPERTY(int curAlarmCount READ curAlarmCount WRITE setCurAlarmCount NOTIFY curAlarmCountChanged)
    Q_PROPERTY(QList<QObject *> alarmList MEMBER m_alarmList NOTIFY alarmListChanged)
    Q_PROPERTY(bool alarmListStatus MEMBER m_alarmListStatus NOTIFY alarmListStatusChanged)
    Q_PROPERTY(int errorStatus MEMBER m_errorStatus NOTIFY errorStatusChanged)

    Q_PROPERTY(QList<QString> userPassword READ userPassword WRITE setUserPassword NOTIFY userPasswordChanged)
    Q_PROPERTY(bool userLevelPassword MEMBER m_userLevelPassword NOTIFY userLevelPasswordChanged)
    Q_PROPERTY(int manualSpeedSetting MEMBER m_manualSpeedSetting NOTIFY manualSpeedSettingChanged)
    Q_PROPERTY(QList<int> languageOrder MEMBER m_languageOrder NOTIFY languageOrderChanged)
    Q_PROPERTY(QList<int> axisOrder MEMBER m_axisOrder NOTIFY axisOrderChanged)
    Q_PROPERTY(QList<int> axisTypeOrder MEMBER m_axisTypeOrder NOTIFY axisTypeOrderChanged)
    Q_PROPERTY(bool opeCmdStatus READ opeCmdStatus WRITE setOpeCmdStatus NOTIFY opeCmdStatusChanged)
    Q_PROPERTY(QString opeCmdStatusMsg MEMBER m_opeCmdStatusMsg NOTIFY opeCmdStatusMsgChanged)
    Q_PROPERTY(QString opeCmdStatusColor MEMBER m_opeCmdStatusColor NOTIFY opeCmdStatusColorChanged)
    Q_PROPERTY(bool dialogStatus READ dialogStatus WRITE setDialogStatus NOTIFY dialogStatusChanged)
    Q_PROPERTY(QString curFileNumber READ curFileNumber WRITE setCurFileNumber NOTIFY curFileNumberChanged)
    Q_PROPERTY(QString curFileName READ curFileName WRITE setCurFileName NOTIFY curFileNameChanged)
    Q_PROPERTY(int topPopupDialog READ topPopupDialog NOTIFY topPopupDialogChanged)
    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(bool permissionTimeUpdate READ permissionTimeUpdate WRITE setPermissionTimeUpdate NOTIFY permissionTimeUpdateChanged)
    Q_PROPERTY(int jogEnable READ jogEnable NOTIFY jogEnableChanged)
    Q_PROPERTY(bool bootCompleted MEMBER m_bootCompleted NOTIFY bootCompletedChanged)
    Q_PROPERTY(bool fileChange READ fileChange WRITE setFileChange NOTIFY fileChangeChanged)
    Q_PROPERTY(QList<bool> JogSpeed READ JogSpeed NOTIFY JogSpeedChanged)
    Q_PROPERTY(int currentOperateAxisOrder READ currentOperateAxisOrder NOTIFY currentOperateAxisOrderChanged)//current oprate axis
    Q_PROPERTY(QObject *axisInfo READ axisInfo CONSTANT)
    Q_PROPERTY(QList<int> jogRatio MEMBER m_JogRatio NOTIFY jogRatioChanged)
    Q_PROPERTY(QList<int> jogLimitspeed MEMBER m_JogLimitspeed NOTIFY jogLimitspeedChanged)
    Q_PROPERTY(bool freeOperate READ freeOperate NOTIFY freeOperateChanged)
    Q_PROPERTY(int currentKeyNo MEMBER m_currentKeyNo NOTIFY currentKeyNoChanged)
    Q_PROPERTY(int currentDp1 MEMBER m_currentDp1 NOTIFY currentDp1Changed)
    Q_PROPERTY(int currentDp2 MEMBER m_currentDp2 NOTIFY currentDp2Changed)

public:
    explicit CommonViewModel(ResourceManager *resource, CommunicationThread * comm, ModbusManager *modbusMng);
    ~CommonViewModel();

    enum OperationViewMode {
        OperationViewModeInvalid = -1,
        OperationViewModeOrigin,
        OperationViewModeManual,
        OperationViewModeAuto,
    };
    enum DateMode {
        DateModeDateTime = 0,
        DateModeDateOnly,
        DateModeTimeOnly,
    };
    enum AlertMsgType {
        AlertMsgTypeAlarm = 0,
        AlertMsgTypeSystem,
        AlertMsgTypeAxisAlarm,
    };
    stModbusPeridiocMonitor initCommonModbusPageCmd[COMMON_MODBUS_MONITOR_CMD_LIST_NUM] = {
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0xCC29, 0x0005 },                 //ユーザレベルパスワード  FuncCode:0x03 addr:HR52265 size:5
    };

    Q_INVOKABLE Enums::Pages pageChange(Enums::Pages page, int subPage = 0);
    Q_INVOKABLE void backPrePage(); ///回到上一个页面
    Q_INVOKABLE void backPrePage(Enums::Pages page, int subPage);
    Q_INVOKABLE void changeScreenId(Enums::MainScreenId mainScreenId, Enums::SubScreenId subScreenId);
    Q_INVOKABLE void changeMainScreenId(Enums::MainScreenId mainScreenId);
    Q_INVOKABLE Enums::Pages getCurrentPage() { return m_currentPage; }
    Q_INVOKABLE int getCurrentSubPage() { return m_currentSubPage; }
    Q_INVOKABLE QString getCurrentState(bool di001, bool di002, bool di003);
    Q_INVOKABLE QString getScreenNameForMenu(int menuNo);
    Q_INVOKABLE QList<int> getMenuList(int userLevel);
    Q_INVOKABLE int getUserLevel();
    Q_INVOKABLE void modbusAutoPressed();
    Q_INVOKABLE void modbusManualPressed();
    Q_INVOKABLE void modbusOriginPressed();
    Q_INVOKABLE void modbusAxisStatus(bool isMainAxis, bool isSubAxis);
    Q_INVOKABLE void modbusMemblemSwControl(bool isPress, uchar keyNo);
    Q_INVOKABLE void modbusUpdateUserLevel(int userLevel);
    Q_INVOKABLE void modbusSpeedControl(int speed);
    Q_INVOKABLE void modbusFreeControl(bool free);
    Q_INVOKABLE void modbusJogMagnificationControl(int jogMagnificationIndex);
    Q_INVOKABLE QDateTime getDateTime(int dateMode = DateModeDateTime);
    Q_INVOKABLE int getLanguage();
    Q_INVOKABLE int maxManualSpeed() const { return MAX_MANUAL_SPEED; }
    Q_INVOKABLE void modbusReturnControl(bool isPress);
    Q_INVOKABLE void modbusResetControl(bool isPress);
    Q_INVOKABLE void getModbusSystemTime();
    Q_INVOKABLE void modbusUpdateUserLevelPassword(int level, QString password);
    Q_INVOKABLE void getUserLevelPassword(int level);
    Q_INVOKABLE QList<int> getOperationAuthorityByGroupId(int groupId);
    Q_INVOKABLE int getOperationAuthority(int groupId, int userLevel);
    Q_INVOKABLE void modbusUpdateDateTime(int index, QString value);
    Q_INVOKABLE void modbusAutoSpeedControl(int speed);
    Q_INVOKABLE void modbusUpdateLanguageCode(int code);
    Q_INVOKABLE int getCurAlarmCount() { return m_curAlarmCount; }
    Q_INVOKABLE int getPointMaxValue(int axisIndex, int pointNo);
    Q_INVOKABLE int getPointMinValue(int axisIndex, int pointNo);
    Q_INVOKABLE QString formatPointMaxValue(int axisIndex, int pointNo);
    Q_INVOKABLE QString formatPointMinValue(int axisIndex, int pointNo);
    Q_INVOKABLE void setPopupDialogStatus(const int id, const bool status);
    Q_INVOKABLE bool isPopupDialogOpened(const int id);
    Q_INVOKABLE void updateTitle(int curPage);
    Q_INVOKABLE void updateOpeCmdDisplay();
    Q_INVOKABLE void modbusSetCurrentOperateAxis(int number);//1~8,not order
    Q_INVOKABLE void modbusSetBuzzer(bool isonoff, int buzzerindex);
    Q_INVOKABLE void modbusReadBuzzer();
    Q_INVOKABLE void modbusSetJogratio(int index, int value);//手轮倍率
    Q_INVOKABLE void modbusSetJoglimitspeed(int index, int value);//手轮限制速度
    Q_INVOKABLE void modbusReadJogratioSpeed();//手轮倍率 限制速度
    Q_INVOKABLE void selectPSaxis();//选择主副臂长

    int languageCount() { return m_languageCount; } // Language order that is used in Menu (Screen 11) mode.
    bool axisStatus() { return m_axisStatus; }
    bool stepOpeStatus() { return m_stepOpeStatus; }
    int currentStatus() { return m_currentStatus; }
    int startupStatus() { return m_startupStatus; }
    QList<int> axisAlarmStatus() { return m_axisAlarmStatus; }
    int userLevel() const { return m_userLevel; }
    int manualSpeedStatus() const { return m_manualSpeedStatus; }
    int autoSpeedStatus() const { return m_autoSpeedStatus; }
    QObject *axisInfo() const { return m_axisInfo; }
    QList<int> menuListUserLevel01();
    QList<int> menuListUserLevel02();
    QList<int> menuListUserLevel03();
    QList<int> menuListUserLevel04();
    QList<int> menuListUserLevel05();

    using ViewModelBase::ViewModelBase;
    int manipulateMode() const { return m_manipulateMode;}
    bool operationMode() const { return m_operationMode;}
    int operatingMode() const { return m_operatingMode;}
    bool emergencyStatus() { return m_emergencyStatus; }
    QString curDate() const { return m_curDate; }
    QString curTime() const { return m_curTime; }
    QString curTimeUptoMinutes() const { return m_curTimeUptoMinutes; }
    int curAlarmCount() const { return m_curAlarmCount;}
    QList<QObject *> alarmList() const { return m_alarmList; }
    bool alarmListStatus() const { return m_alarmListStatus; }
    QList<QString> userPassword() { return m_userPassword; }
    QList<int> getAxisOrder() { return m_axisOrder; }
    bool opeCmdStatus() { return m_opeCmdStatus; }
    QString opeCmdStatusMsg() const { return m_opeCmdStatusMsg; }
    QString opeCmdStatusColor() const { return m_opeCmdStatusColor; }
    bool dialogStatus() { return m_dialogStatus; }
    void setCurFileNumber(QString curFileNumber);
    QString curFileNumber() const { return m_curFileNumber; }
    void setCurFileName(QString curFileName);
    QString curFileName() const { return m_curFileName; }
    int topPopupDialog() const { return mPopupDialogOrderStack.top(); }
    void setLanguage(int language);
    int language() const { return m_language; }
    bool permissionTimeUpdate() const { return m_permissionTimeUpdate; }
    int curMainScreenId() const { return m_curMainScreenId; }
    int curSubScreenId() const { return m_curSubScreenId; }
    int jogEnable() const { return m_jogEnable; }
    bool fileChange() { return m_fileChange; }
    void setFileChange(bool change);
    QList<bool> JogSpeed() {return m_JogSpeed;}
    int currentOperateAxisOrder() {return m_currentOperateAxisOrder;}
    bool freeOperate() const { return m_freeOperate; }

/**************************PC端共通画面弹出窗口设置***************************/
#ifndef REAL_TARGET
    Q_INVOKABLE  void trigger_EmergencyStates(bool state){
        setEmergencyStatus(state);
    }//操作急停
    Q_INVOKABLE void trigger_ope(int value){
        if (value != 0){
            m_opeCmdStatusMsg = m_resource->getMessageStringValue("14", value, m_resource->language());
            emit opeCmdStatusMsgChanged(m_opeCmdStatusMsg);
            m_opeCmdStatusColor = m_resource->rgb(14, value);
            emit opeCmdStatusColorChanged(m_opeCmdStatusColor);
            setOpeCmdStatus(true);
        } else {
            setOpeCmdStatus(false);
        }
    }//动作表示
    Q_INVOKABLE void trigger_CurrentState(int error, int alarm, int system){
        int currentStatus = 0;
        mIsNotifyAlarm = false;
        //操作错误
        if(error != 0){
            updateErrorStatus((qint16)error);
            mIsNotifyAlarm = true;
        } else {
            m_errorStatus = 0;
            mIsNotifyAlarm = true;
        }

        //警报
        if(alarm != 0 && mPreAlarmStatus != alarm){
            updateAlarmList(AlertMsgType::AlertMsgTypeAlarm, 0, (quint16)alarm);
            if (mPreAlarmStatus <= 0){
                mInternalAlarmCount++;
            }
            mIsNotifyAlarm = true;
            mPreAlarmStatus = alarm;
            mIsAlarmOccurred = true;
        }else {
            if(mPreAlarmStatus != 0 && alarm == 0 && mInternalAlarmCount > 0){
                mInternalAlarmCount--;
                removeAlarmList(AlertMsgType::AlertMsgTypeAlarm, 0);
                mIsNotifyAlarm = true;
                mIsAlarmOccurred = false;
                mPreAlarmStatus = alarm;
            }
        }
        //系统警报
        if(system != 0 && mPreSystemAlarmStatus != system){
            updateAlarmList(AlertMsgType::AlertMsgTypeSystem, 0, (quint16)system);
            if(mPreSystemAlarmStatus <=0){
                mInternalAlarmCount++;
            }
            mPreSystemAlarmStatus = system;
            mIsNotifyAlarm = true;
        }else {
            if(mPreSystemAlarmStatus != 0 && system == 0 && mInternalAlarmCount > 0){
                mInternalAlarmCount--;
                removeAlarmList(AlertMsgType::AlertMsgTypeSystem, 0);
                mIsNotifyAlarm = true;
            }
            mPreSystemAlarmStatus = system;
        }
        currentStatus = (quint16(system) << 16) | (quint8(error) << 8) | quint8(alarm);
        setCurAlarmCount(mInternalAlarmCount);
        setCurrentStatus(currentStatus);
    }//操作错误、警报、系统警报
    Q_INVOKABLE void trigger_AxisState(int axisnumber, int axis_alarmcount){
        QList<int> axisAlarmStatusList;
        axisAlarmStatusList.clear();

        if(mPreAxisAlarmStauts[axisnumber] != axis_alarmcount && axis_alarmcount != 0){//更新当前轴警报
            if(isAlarmExist(static_cast<AlarmInfo::Type>(axisnumber))){//当前轴存在轴警报
                AlarmInfo *alarmInfo = new  AlarmInfo();
                alarmInfo->setType(static_cast<AlarmInfo::Type>(axisnumber));
                alarmInfo->setMsgNumber(axis_alarmcount);
                if (axis_alarmcount >= 1 && axis_alarmcount <= 64){
                    alarmInfo->setAlarmNumber((axis_alarmcount < 100 ? (axis_alarmcount < 10 ? "00" + QString::number((int)axis_alarmcount)
                                                                                             : "0" + QString::number((int)axis_alarmcount)) : QString::number((int)axis_alarmcount)));
                    alarmInfo->setAlarmIndex(AXIS_ALARM_MSG_TYPE_START_INDEX + (int)(axis_alarmcount * 2));
                    alarmInfo->setAlarmDetailsColor(m_resource->rgb(
                                                        12, AXIS_ALARM_MSG_TYPE_START_INDEX + (int)(axis_alarmcount * 2)));
                } else {
                    alarmInfo->setAlarmNumber("12-" + (axis_alarmcount - 64 < 100 ? (axis_alarmcount - 64 < 10 ? "00" + QString::number((int)(axis_alarmcount - 64))
                                                                                                               : "0" + QString::number((int)(axis_alarmcount - 64))) : QString::number((int)(axis_alarmcount - 64))));
                    alarmInfo->setAlarmIndex(AXIS_ALARM_MSG_TYPE2_START_INDEX + (int)((axis_alarmcount - 65) * 2));
                    alarmInfo->setAlarmDetailsColor(m_resource->rgb(
                                                        12, AXIS_ALARM_MSG_TYPE2_START_INDEX + (int)((axis_alarmcount - 65) * 2)));
                }
                int axisAlarmIndex = isAxis_AlarmExistForPc(static_cast<AlarmInfo::Type>(axisnumber));
                mPreAxisAlarmStauts[axisnumber] = axis_alarmcount;
                m_alarmList.replace(axisAlarmIndex,alarmInfo);
                axisAlarmStatusList.append(axis_alarmcount);
                setCurAlarmCount(mInternalAlarmCount);
                setAxisAlarmStatus(axisAlarmStatusList);
            }else {//当前轴不存在轴警报,新建
                AlarmInfo *alarmInfo = new  AlarmInfo();
                alarmInfo->setType(static_cast<AlarmInfo::Type>(axisnumber));
                alarmInfo->setMsgNumber(axis_alarmcount);
                if (axis_alarmcount >= 1 && axis_alarmcount <= 64){
                    alarmInfo->setAlarmNumber((axis_alarmcount < 100 ? (axis_alarmcount < 10 ? "00" + QString::number((int)axis_alarmcount)
                                                                                             : "0" + QString::number((int)axis_alarmcount)) : QString::number((int)axis_alarmcount)));
                    alarmInfo->setAlarmIndex(AXIS_ALARM_MSG_TYPE_START_INDEX + (int)(axis_alarmcount * 2));
                    alarmInfo->setAlarmDetailsColor(m_resource->rgb(
                                                        12, AXIS_ALARM_MSG_TYPE_START_INDEX + (int)(axis_alarmcount * 2)));
                } else {
                    alarmInfo->setAlarmNumber("12-" + (axis_alarmcount - 64 < 100 ? (axis_alarmcount - 64 < 10 ? "00" + QString::number((int)(axis_alarmcount - 64))
                                                                                                               : "0" + QString::number((int)(axis_alarmcount - 64))) : QString::number((int)(axis_alarmcount - 64))));
                    alarmInfo->setAlarmIndex(AXIS_ALARM_MSG_TYPE2_START_INDEX + (int)((axis_alarmcount - 65) * 2));
                    alarmInfo->setAlarmDetailsColor(m_resource->rgb(
                                                        12, AXIS_ALARM_MSG_TYPE2_START_INDEX + (int)((axis_alarmcount - 65) * 2)));
                }
                mPreAxisAlarmStauts[axisnumber] = axis_alarmcount;
                axisAlarmStatusList.append(axis_alarmcount);
                mInternalAlarmCount++;
                mIsNotifyAlarm = true;
                m_alarmList.replace(mInternalAlarmCount-1,alarmInfo);
                setCurAlarmCount(mInternalAlarmCount);
                setAxisAlarmStatus(axisAlarmStatusList);
            }
        } else {//清除当前轴警报
            mPreAxisAlarmStauts[axisnumber] = axis_alarmcount;
            mInternalAlarmCount--;
            removeAxisAlarmForPc(static_cast<AlarmInfo::Type>(axisnumber));
            setCurAlarmCount(mInternalAlarmCount);
        }
    }//轴警报
    int isAxis_AlarmExistForPc(AlarmInfo::Type type)
    {
        int ret = 0;
        const auto len = m_alarmList.length();
        AlarmInfo *alarm;
        for (auto i = 0; i < len; i++) {
            alarm = static_cast<AlarmInfo *>(m_alarmList.at(i));
            if (alarm->type() == type) {
                ret = i;
                break;
            }
        }
        return ret;
    }
    void removeAxisAlarmForPc(AlarmInfo::Type type){
        int axisAlarmIndex = isAxis_AlarmExistForPc(type);
        m_alarmList.removeAt(axisAlarmIndex);
        AlarmInfo *alarmInfo = new AlarmInfo(this);
        m_alarmList.append(alarmInfo);
    }
#endif

public slots:
    void setManipulateMode(int manipulateMode);
    void setOperationMode(bool operationMode);
    void setLanguageCount(int languageCount);
    void setAxisStatus(bool axisStatus);
    void setStepOpeStatus(bool stepOpeStatus);
    void setCurrentStatus(int currentStatus);
    void setStartupStatus(int startupStatus);
    void setAxisAlarmStatus(QList<int> axisAlarmStatus);
    void onFinished();
    void onUpdateSelectSwInfo(PhyKeyManager::SelectSwitchStatus selectSwStatus);
    void onUpdateMemblemSwInfo(PhyKeyManager::MemblemSwInfo memblemSwInfo);
    void onUpdateReleaseMemblemSwInfo(int memblemSwReleaseInfo);
    void setUserLevel(int userLevel);
    void setManualSpeedStatus(int manualSpeedStatus);
    void setAutoSpeedStatus(int autoSpeedStatus);
    void setOperatingMode(int operatingMode);
    void setEmergencyStatus(bool emergencyStatus);
    void setCurDate(QString curDate);
    void setCurTime(QString curTime);
    void setCurTimeUptoMinutes(QString curTimeUptoMinutes);
    void onOneShotReadFinished(stModBusCommunicationData command/*, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller*/);
    void setCurAlarmCount(int curAlarmCount);
    void setCurAlarmCount(int curAlarmCount, bool alarmListStatusChange);
    void setUserPassword(QList<QString> userPassword);
    void update(stModBusCommunicationData command);
    void setOpeCmdStatus(bool opeCmdStatus);
    void setDialogStatus(bool dialogStatus);
    void setPermissionTimeUpdate(bool permissionTimeUpdate);
    void setjogEnable(int jogEnable);
    void onIsFirstReadCommonModbusChanged(bool isFirstReadCommonModbus);
    void onlinkAlarmChanged(bool isonoff);
    void onUpdateSingleTimer();

signals:
    void manipulateModeChanged(int manipulateMode);
    void operationModeChanged(bool operationMode);
    void languageCountChanged(int languageCount);
    void axisStatusChanged(bool axisStatus);
    void stepOpeStatusChanged(bool stepOpeStatus);
    void currentStatusChanged(int currentStatus);
    void startupStatusChanged(int startupStatus);
    void axisAlarmStatusChanged(QList<int> axisAlarmStatus);
    void userLevelChanged(int userLevel);
    void manualSpeedStatusChanged(int manualSpeedStatus);
    void autoSpeedStatusChanged(int autoSpeedStatus);
    void topTitleChanged(QString &newValue);
    void operatingModeChanged(int operatingMode);
    void emergencyStatusChanged(bool emergencyStatus);
    void curDateChanged(QString &curDate);
    void curTimeChanged(QString &curTime);
    void curTimeUptoMinutesChanged(QString &curTimeUptoMinutes);
    void yearChanged();
    void monthChanged();
    void dayChanged();
    void hourChanged();
    void minutesChanged();
    void secondsChanged();
    void systemTimeStatusChanged(bool systemTimeStatus);
    void curAlarmCountChanged(int curAlarmCount);
    void alarmListChanged(QList<QObject *> alarmList);
    void alarmListStatusChanged(bool alarmListStatus);
    void errorStatusChanged();
    void opeCmdStatusChanged(bool opeCmdStatus);
    void opeCmdStatusMsgChanged(QString &opeCmdStatusMsg);
    void opeCmdStatusColorChanged(QString &opeCmdStatusColor);
    void userPasswordChanged(QList<QString> userPassword);
    void userLevelPasswordChanged(bool userLevelPassword);
    void manualSpeedSettingChanged();
    void languageOrderChanged();
    void axisOrderChanged();
    void axisTypeOrderChanged();
    void dialogStatusChanged(bool dialogStatus);
    void curFileNumberChanged(QString curFileNumber);
    void curFileNameChanged(QString curFileName);
    void topPopupDialogChanged(int);
    void languageChanged(int language);
    void alarmOccurred(const AlarmInfo &alarmInfo, const QDateTime &dateTime);
    void permissionTimeUpdateChanged(bool permissionTimeUpdate);
    void jogEnableChanged(int jogEnable);
    void bootCompletedChanged();
    void fileChangeChanged(bool change);
    void JogSpeedChanged(QList<bool> speed);
    void currentOperateAxisOrderChanged();
    void jogRatioChanged(QList<int> jogradio);
    void jogLimitspeedChanged(QList<int> speed);
    void freeOperateChanged(bool free);
    void currentKeyNoChanged(int keyno);
    void currentDp1Changed(int dp1);
    void currentDp2Changed(int dp2);

/*************************************************************************************/
private:
    CsvReader           *mCsvReader;
    ModbusManager       *mModbusMng;
    ModbusCommunication *mModbusCom;
    AxisInfo            *m_axisInfo = AxisInfo::get_instance();

    Enums::Pages        m_currentPage = Enums::Pages::InvalidPage;
    int                 m_currentSubPage = 0;
    Enums::Pages        m_prePage = Enums::Pages::InvalidPage;
    Enums::Pages        m_curLeftPage = Enums::Pages::InvalidPage;
    int                 m_preSubPage = 0;
    QList<int>          m_userLevelMenu[MAX_USER_LEVEL];
    int                 m_manipulateMode = 0;
    bool                m_operationMode = false;
    QList<int>          m_languageOrder;
    QList<int>          m_axisOrder;         // 軸番号の並び.
    QList<int>          m_axisTypeOrder;     // 軸種別(Y軸,SX軸,MX軸等)の並び.
    int                 m_languageCount = 0;
    bool                m_axisStatus = true;                   // true:製品側 false:副側軸
    bool                m_stepOpeStatus = false;
    int                 m_currentStatus = 0;
    int                 m_startupStatus = 0;
    QList<int>          m_axisAlarmStatus;
    int                 m_userLevel = 0;  // MODIFIED : testUserlevel change IJ.YI 25.07.10
    int                 m_manualSpeedStatus = 0;
    int                 m_autoSpeedStatus = 0;
    QString             m_pageTitle;
    Enums::MainScreenId m_curMainScreenId = Enums::MainScreenId::MainScreenIdInvalid;
    Enums::SubScreenId  m_curSubScreenId = Enums::SubScreenId::SubScreenIdInvalid;
    int                 m_operatingMode = 0;
    bool                m_emergencyStatus = false;
    QString             m_curDate;
    QString             m_curTime;
    QString             m_curTimeUptoMinutes;
    int                 mPreDay = 0;
    int                 mPreSeconds = 0;
    bool                mIsLedDeviceInitialized = false;
    bool                mIsGreenLed = false;
    bool                mIsBlueLed = false;
    bool                mIsRedLed = false;
    QString             m_year;
    QString             m_month;
    QString             m_day;
    QString             m_hour;
    QString             m_minutes;
    QString             m_seconds;
    bool                m_systemTimeStatus = false;
    int                 mInternalAlarmCount = 0;
    int                 m_curAlarmCount = 0;
    int                 mPreAlarmCount = 0;
    int                 m_preOpeCmdCount = 0;
    QList<QObject *>    m_alarmList;
    bool                m_alarmListStatus = false;
    int                 m_errorStatus = 0;
    bool                m_opeCmdStatus = false;
    quint8              m_opeCmdValue = 0;
    QString             m_opeCmdStatusMsg = "";
    QString             m_opeCmdStatusColor = "";
    QList<QString>      m_userPassword;
    bool                mIsUpdateUserPassword = false;
    int                 mUserLevelPassword[MAX_USER_LEVEL];
    bool                m_userLevelPassword = false;
    QList<QList<int>>   m_userLevelOperationAuthority;
    int                 m_manualSpeed0001Existence;                     // 0.001有無
    int                 m_manualSpeed001Existence;                      // 0.01有無
    int                 m_manualSpeedSetting;
    quint8              mPreAlarmStatus = 0;
    qint16              mPreSystemAlarmStatus = 0;
    int                 mPreAxisAlarmStauts[AXIS_ALARM_STATUS_NUM];
    bool                mIsNotifyAlarm = false;
    bool                mModbusConnectFlag = false;
    bool                mIsNotifyOpeCmd = false;
    bool                m_dialogStatus = false;
    AreaSetPointParser  *m_areaSetPointParser;
    QList<QList<AreaSetPointInfo *>>    m_pointMaxMinList;
    QString             m_curFileNumber = "";
    QString             m_curFileName = "";
    bool                mIsAlarmOccurred = false;
    bool                mIsSettingSystemData = false;
    CoreLibSystemControl    mCoreLibSystemControl;
    QStack<int>             mPopupDialogOrderStack;
    int                 m_language = -1;
    bool                mIsAlreadyLanguage = false;
    bool                m_permissionTimeUpdate = false;
    int                 m_jogEnable;
    bool                m_isFirstReadCommonModbus = false;
    bool                mIsAlreadyCommonModbus = false;
    bool                m_bootCompleted = false;
    bool                m_fileChange = false;
    QList<bool>         m_JogSpeed = {false,false,false,false};
    QList<int>          m_currentOperateAxisDi;//当前被操作轴的modbus地址
    int                 m_currentOperateAxisOrder = -1;//当前操作的轴号
    QList<int>          m_JogRatio = {0,0,0};
    QList<int>          m_JogLimitspeed = {0,0,0};
    bool                m_freeOperate = false;
    int                 m_currentKeyNo = -1;
    int                 m_currentDp1 = -1;
    int                 m_currentDp2 = -1;

    void initJogInfo();
    void initSettingPenSetData();
    void readPointMaxMinSetting();
    QList<int> initLanguageOrder();
    void initAxisOrder();
    void initModbusCommunication();
    void initAlarmList();
    void initPageCommand();
    void terminateModbusCommunication();
    void initInternalInfo();
    bool initLedDevice();
    void deinitLedDevice();
    void initOperatedAxisAddr();//初始化各轴操作状态的modbus地址
    void modbusUpdateCurrentOperateAxis();//更新当前操作的轴

    QList<int> getMenuList(QVector<int> row1, QVector<int> row2, QVector<int> row3, QVector<int> row4, QVector<int> row5);
    void writeManipulateMode(int manipMode);
    void writeManipulateModeExistModeSw(int manipMode);
    void writeManipulateModeUnExistModeSw(int manipMode);
    void writeOperationMode();
    bool judgeOperationMode();
    void judgeManipulateMode();
    bool judgeAxisAlarmStatus(QList<int> curAxisAlarm);
    void updateAxisStatus();
    void updateStepOpeStatus();
    void updateCurrentStatus();
    void updateAxisAlarmStatus();
    void updateOpeCmdStatus();
    void updateUserLevel();
    void updateManualOpeSpeedStatus();
    void updateAutoOpeSpeedStatus();
    void updateTopTitleLocal(Enums::Pages currentPage, Enums::Pages curLeftPage, bool isForce = false);
    void updateOperatingStatus();
    void updateEmergencyStopStatus();
    void setTopTitle(QString& newValue);
    QString topTitle() const;
    void updateScreenId(Enums::Pages currentPage, int subPage = 0);
    void updateDateTime();
    void controlLedDevice(bool isOnOff, int ledType);
    void updateSystemTime(stModBusCommunicationData command);
    void updateAlarmList(int alarmType, int axisNumber, quint16 msgNo);
    void updateAlarmMsgList();
    void updateErrorStatus(quint16 msgNo);
    bool judgeUpdateUserPassword(QList<QString> curUserPassword);
    void updateUserPassword(stModBusCommunicationData command);
    void updateUserPasswordIndividual(stModBusCommunicationData command);
    void updateUserPassword(quint16 userPassword);
    int checkAlarmOrder(int alarmType, int axisNumber = 0);
    bool isAlarmExist(AlarmInfo::Type type);
    void removeAlarmList(int alarmType, int axisNumber);
    int getAxisAlarmRemoveIndex(int axisNumber);
    void updateCurFileInfo();
    void openPopupDialog(const int id);  // const int id ... Enums::PopupDialogStatus.
    void closePopupDialog(const int id); // const int id ... Enums::PopupDialogStatus.
    void updateLanguageInfo();
    void updateScreenTitle(Enums::Pages currentPage);
    QString updateSpecialScreenTitle(Enums::Pages prePage);
    void notifyAlarm(const AlarmInfo &alarmInfo);
    void updateFileChange();
    void updateBuzzerInfo(stModBusCommunicationData command);
    void updateAxisCurrentPosition();
    void updateJogRatioSpeedInfo(stModBusCommunicationData command);
};

#endif // COMMONVIEWMODEL_H
