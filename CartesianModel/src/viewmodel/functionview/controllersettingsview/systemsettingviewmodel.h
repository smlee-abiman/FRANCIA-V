#ifndef SYSTEMSETTINGVIEWMODEL_H
#define SYSTEMSETTINGVIEWMODEL_H

#include <QObject>
#include <QByteArray>
#include <QDateTime>

#include <pendantapplication.h>
#include <viewmodelbase.h>

#include "corelibdisplaycontrol.h"

#define SCREEN_BRIGHT_HR                (1949)
#define SCREEN_OFF_TIME_HR              (1948)
#define SYSTEM_PROGRAM_HR               (2530)
#define TAKE_UP_TYPE_HR                 (2529)
#define MACHINE_NAME_HR                 (52239)
#define MACHINE_SETUP_DATE_HR           (52259)
#define MACHINE_NAME_SIZE               (10)
#define MACHINE_NAME_LENGTH             (MACHINE_NAME_SIZE*2)
#define MACHINE_SETUP_DATE_SIZE         (6)
#define ACCEPTANCE_REGISTRATIONCODE_HR  (2528)
#define ACCEPTANCE_REMAINING_TIME_SR    (2994)
#define SYSTEM_MODE_HR                  (1936)
#define SAVE_SYSTEM_SETTING_HR          (2533)

class SystemSettingViewModel : public ViewModelBase
{
    Q_OBJECT
    // data.
    Q_PROPERTY(int screenBright READ screenBright NOTIFY screenBrightChanged)
    Q_PROPERTY(int screenOffTime READ screenOffTime NOTIFY screenOffTimeChanged)
    Q_PROPERTY(int systemProgram READ systemProgram NOTIFY systemProgramChanged)
    Q_PROPERTY(int takeUpType READ takeUpType NOTIFY takeUpTypeChanged)
    Q_PROPERTY(QString machineName READ machineName NOTIFY machineNameChanged)
    Q_PROPERTY(QDateTime machineSetupDate READ machineSetupDate NOTIFY machineSetupDateChanged)
    Q_PROPERTY(int acceptanceRegistrationCode READ acceptanceRegistrationCode NOTIFY acceptanceRegistrationCodeChanged)
    Q_PROPERTY(int acceptanceRemainingTime READ acceptanceRemainingTime NOTIFY acceptanceRemainingTimeChanged)
    Q_PROPERTY(QList<bool> systemModeTbl READ systemModeTbl NOTIFY systemModeTblChanged)
    Q_PROPERTY(bool editedScreenBright READ editedScreenBright NOTIFY editedScreenBrightChanged)
    Q_PROPERTY(bool editedScreenOffTime READ editedScreenOffTime NOTIFY editedScreenOffTimeChanged)
    Q_PROPERTY(bool editedSystemProgram READ editedSystemProgram NOTIFY editedSystemProgramChanged)
    Q_PROPERTY(bool editedTakeUpType READ editedTakeUpType NOTIFY editedTakeUpTypeChanged)
    Q_PROPERTY(bool editedMachineName READ editedMachineName NOTIFY editedMachineNameChanged)
    Q_PROPERTY(bool editedMachineSetupDate READ editedMachineSetupDate NOTIFY editedMachineSetupDateChanged)
    Q_PROPERTY(bool editedAcceptanceRegistrationCode READ editedAcceptanceRegistrationCode NOTIFY editedAcceptanceRegistrationCodeChanged)
    Q_PROPERTY(QList<bool> editedSystemModeTbl READ editedSystemModeTbl NOTIFY editedSystemModeTblChanged)
    // penset.
    Q_PROPERTY(int screenOffTimeFormatNo READ screenOffTimeFormatNo CONSTANT)
    Q_PROPERTY(int machineSetupDatePressAndHoldInterval_msec READ machineSetupDatePressAndHoldInterval_msec CONSTANT)
    Q_PROPERTY(QList<bool> systemModeDisplay READ systemModeDisplay CONSTANT)

public:
    explicit SystemSettingViewModel(ResourceManager *resource, CommunicationThread * comm);
    virtual ~SystemSettingViewModel() override;

    quint16 screenBright() const { return m_screenBright; }
    quint16 screenOffTime() const { return m_screenOffTime; }
    quint16 systemProgram() const { return m_systemProgram; }
    quint16 takeUpType() const { return m_takeUpType; }
    QString machineName() const { return QString(m_machineName); }
    QDateTime machineSetupDate() const { return m_machineSetupDate; }
    quint16 acceptanceRegistrationCode() const { return m_acceptanceRegistrationCode; }
    quint16 acceptanceRemainingTime() const { return m_acceptanceRemainingTime; }
    QList<bool> systemModeTbl() const;
    bool editedScreenBright() const { return m_editedScreenBright; }
    bool editedScreenOffTime() const { return m_editedScreenOffTime; }
    bool editedSystemProgram() const { return m_editedSystemProgram; }
    bool editedTakeUpType() const { return m_editedTakeUpType; }
    bool editedMachineName() const { return m_editedMachineName; }
    bool editedMachineSetupDate() const { return m_editedMachineSetupDate; }
    bool editedAcceptanceRegistrationCode() const { return m_editedAcceptanceRegistrationCode; }
    QList<bool> editedSystemModeTbl() const { return m_editedSystemModeTbl; }

    int screenOffTimeFormatNo() const { return m_screenOffTimeFormatNo; }
    int machineSetupDatePressAndHoldInterval_msec() const { return m_machineSetupDatePressAndHoldInterval_msec; }
    QList<bool> systemModeDisplay() const { return  m_systemModeDisplay; }

    Q_INVOKABLE void editScreenBright(const int screenBright);
    Q_INVOKABLE void editScreenOffTime(const int screenOffTime);
    Q_INVOKABLE void editSystemProgram(const int systemProgram);
    Q_INVOKABLE void editTakeUpType(const int takeUpType);
    Q_INVOKABLE void editMachineName(const QString &machineName);
    Q_INVOKABLE void resetMachineSetupDateToCurrentDate();
    Q_INVOKABLE void editAcceptanceRegistrationCode(const int acceptanceRegistrationCode);
    Q_INVOKABLE void editSystemMode(int index, bool systemMode);
    Q_INVOKABLE void saveSystemSettings();

    // for debug.
    void display();

private:
    bool validScreenBright(const quint16 &screenBright);
    void setScreenBright(const quint16 &screenBright);
    bool validScreenOffTime(const quint16 &screenOffTime);
    void setScreenOffTime(const quint16 &screenOffTime);
    bool validSystemProgram(const quint16 &systemProgram);
    void setSystemProgram(const quint16 &systemProgram);
    bool validTakeUpType(const quint16 &takeUpType);
    void setTakeUpType(const quint16 &takeUpType);
    void setMachineName(const QByteArray &machineName);
    void setMachineSetupDate(const QDateTime &machineSetupDate);
    void setAcceptanceRegistrationCode(const quint16 &acceptanceRegistrationCode);
    void setAcceptanceRemainingTime(const quint16 &acceptanceRemainingTime);
    void setSystemMode(quint16 systemMode);
    void setEditedScreenBright(bool editedScreenBright);
    void setEditedScreenOffTime(bool editedScreenOffTime);
    void setEditedSystemProgram(bool editedSystemProgram);
    void setEditedTakeUpType(bool editedTakeUpType);
    void setEditedMachineName(bool editedMachineName);
    void setEditedMachineSetupDate(bool editedMachineSetupDate);
    void setEditedAcceptanceRegistrationCode(bool editedAcceptanceRegistrationCode);
    void setEditedSystemMode(int index, bool systemMode);
    void setEditedSystemModeAll(bool editedSystemMode);
    void resetAllEdited();

signals:
    void screenBrightChanged(quint16);
    void screenOffTimeChanged(quint16);
    void systemProgramChanged(quint16);
    void takeUpTypeChanged(quint16);
    void machineNameChanged(QString);
    void machineSetupDateChanged(QDateTime);
    void systemModeTblChanged(QList<bool>);
    void acceptanceRegistrationCodeChanged(quint16);
    void acceptanceRemainingTimeChanged(quint16);
    void editedScreenBrightChanged(bool);
    void editedScreenOffTimeChanged(bool);
    void editedSystemProgramChanged(bool);
    void editedTakeUpTypeChanged(bool);
    void editedMachineNameChanged(bool);
    void editedMachineSetupDateChanged(bool);
    void editedAcceptanceRegistrationCodeChanged(bool);
    void editedSystemModeTblChanged(QList<bool>);

public slots:
    void onOperationStarted();
    void onOperationStopped();
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

protected:
    void onActivate() override;
    void onDeactivate() override;

private:
    void init();
    void connectToOperationNotification();
    void disconnectToOperationNotification();
    void startOperationMonitoring(const int screenOffTime);
    void decrementNumOfRemainingInitialModbusReads();
    void openDisplayControl();
    void closeDisplayControl();
    void setLcdBackLightBrightness(const int screenBright);
    void readPenSetData();
    void readPenSetSystemModeDisplay();
    void startModbusPeriodic();
    void stopModbusPeriodic();
    void startModbusOneShot();
    void stopModbusOneShot();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void writeModbusHR(quint16 hrAddress, const QList<int> &writeData);
    void readModbus(const QModbusPdu::FunctionCode funcCode, const quint16 &address, const quint16 &range);
    void readModbusHR(const quint16 &hrAddress, const quint16 &range);
    void readModbusSR(const quint16 &srAddress, const quint16 &range);
    void modbusGetScreenBright();
    void modbusGetScreenOffTime();
    void modbusGetSystemProgram();
    void modbusGetTakeUpType();
    void modbusGetMachineName();
    void modbusGetMachineSetupDate();
    void modbusGetAcceptanceRegistrationCode();
    void modbusGetAcceptanceRemainingTime();
    void modbusGetSystemMode();
    void updateScreenBright(const stModBusCommunicationData &command);
    void updateScreenOffTime(const stModBusCommunicationData &command);
    void updateSystemProgram(const stModBusCommunicationData &command);
    void updateTakeUpType(const stModBusCommunicationData &command);
    void updateMachineName(const stModBusCommunicationData &command);
    void updateMachineSetupDate(const stModBusCommunicationData &command);
    void updateAcceptanceRegistrationCode(const stModBusCommunicationData &command);
    void updateAcceptanceRemainingTime(const stModBusCommunicationData &command);
    void updateSystemMode(const stModBusCommunicationData &command);

private:
    PendantApplication              *m_application;
    CoreLibDisPlayControl           m_displayControl;
    AxisInfo                        *m_axisInfo = AxisInfo::get_instance();

    bool                            m_isReadyDisplayControl;
    bool                            mIsModbusConnectOneShot;
    quint16                         m_numOfRemainingInitialModbusReads;
    quint16                         m_screenBright;
    quint16                         m_screenOffTime;
    quint16                         m_systemProgram;
    quint16                         m_takeUpType;
    QByteArray                      m_machineName;
    QDateTime                       m_machineSetupDate;
    quint16                         m_acceptanceRegistrationCode;
    quint16                         m_acceptanceRemainingTime;
    quint16                         m_systemMode;
    int                             m_screenOffTimeFormatNo;
    int                             m_machineSetupDatePressAndHoldInterval_msec;
    bool                            m_editedScreenBright;
    bool                            m_editedScreenOffTime;
    bool                            m_editedSystemProgram;
    bool                            m_editedTakeUpType;
    bool                            m_editedMachineName;
    bool                            m_editedMachineSetupDate;
    bool                            m_editedAcceptanceRegistrationCode;
    QList<bool>                     m_editedSystemModeTbl;
    QList<bool>                     m_systemModeDisplay;
    const QList<int>                AXIS_3_POS_ADDR_LIST = {2761, 2763, 2765};
};

#endif // SYSTEMSETTINGVIEWMODEL_H
