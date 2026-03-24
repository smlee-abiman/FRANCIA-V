#ifndef OPERATIONRECORD_H
#define OPERATIONRECORD_H

#include <QObject>

class OperationHistoryData;
class OperationHistoryList;
class CommonViewModel;

class OperationRecord : public QObject
{
    Q_OBJECT
public:
    explicit OperationRecord(OperationHistoryList *list, QObject *parent = nullptr);
    static OperationRecord* create_instance(OperationHistoryList *list, QObject *parent = nullptr);
    static OperationRecord* get_instance();
    void setCommonView(CommonViewModel *viewModel);

    // Setting(d1 = 1)
    Q_INVOKABLE void recordSetManualSpeedBySpeedSetting(int currentValue, int newValue);
    Q_INVOKABLE void recordSetJogSpeed(int currentValue, int newValue);
    Q_INVOKABLE void recordSetAutoSpeed(int currentValue, int newValue);
    Q_INVOKABLE void recordSetManualSpeedByManual(int currentValue, int newValue);
    Q_INVOKABLE void recordSetMode(int mode, int currentValue, int newValue);
    Q_INVOKABLE void recordSetPositionByPosSet(int pointIndex, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetSpeedByPosSet(int pointIndex, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetPositionByPosFix(int pointIndex, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetSpeedByPosFix(int pointIndex, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackPalletizeNumber(int prevOrder, int currentOrder, int palletizeNum);
    Q_INVOKABLE void recordSetStdStackDownDeceleratePosition(int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackDownDecelerateSpeed(int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackUpDeceleratePosition(int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackUpDecelerateSpeed(int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackStartSpeed(int palletizeNum, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackStartPos(int palletizeNum, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackCurrentPoint(int palletizeNum, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackPoint(int palletizeNum, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackPitch(int palletizeNum, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetStdStackDirection(int palletizeNum, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetFreeStackCurrentPoint(int palletizeNum, int currentValue, int newValue);
    Q_INVOKABLE void recordSetFreeStackPoint(int palletizeNum, int currentValue, int newValue);
    Q_INVOKABLE void recordSetTimer(int timerId, int currentValue, int newValue);
    Q_INVOKABLE void recordSetCounter(int counterId, int currentValue, int newValue);
    Q_INVOKABLE void recordSetPass(int passId, int currentValue, int newValue);
    Q_INVOKABLE void recordProgramCopy(int programNo);
    Q_INVOKABLE void recordProgramPaste(int programNo);
    Q_INVOKABLE void recordProgramDelete(int programNo);
    Q_INVOKABLE void recordProgramSave(int programNo);
    Q_INVOKABLE void recordProgramMoveUp(int programNo);
    Q_INVOKABLE void recordProgramMoveDown(int programNo);
    Q_INVOKABLE void recordProgramReplace(int programNo);
    Q_INVOKABLE void recordProgramInsert(int programNo);
    Q_INVOKABLE void recordSetScreenBright(int currentValue, int newValue);
    Q_INVOKABLE void recordSetScreenOffTime(int currentValue, int newValue);
    Q_INVOKABLE void recordSetSystemProgram(int currentValue, int newValue);
    Q_INVOKABLE void recordSetTakeUpType(int currentValue, int newValue);
    Q_INVOKABLE void recordSetSystemMode(int id, int currentValue, int newValue);
    Q_INVOKABLE void recordSetModelType(int currentValue, int newValue);
    Q_INVOKABLE void recordSetMachineParam(int type, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSaveMachineParam();
    Q_INVOKABLE void recordSaveDriverParam();
    Q_INVOKABLE void recordInitDriverParam();
    Q_INVOKABLE void recordSetDriverParam(int itemNo, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordSetSimpleSetting(int itemNo, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordResetDrvMonitor();
    Q_INVOKABLE void recordStartDrvMonitor();
    Q_INVOKABLE void recordStopDrvMonitor();
    Q_INVOKABLE void recordAreaSet(int itemNo, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordAreaSetServo(int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordAreaSetMemory(int itemNo, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordAreaFix(int itemNo, int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordAreaFixMxSx(int currentValue, int newValue);
    Q_INVOKABLE void recordInspectionCompleted(int itemNo);
    Q_INVOKABLE void recordInspectionSaved();
    Q_INVOKABLE void recordRefuelingCompleted(int axisIndex);
    Q_INVOKABLE void recordRefuelingSaved();
    Q_INVOKABLE void recordSetProductionProgNum(int currentValue, int newValue);
    Q_INVOKABLE void recordSetOneFetchNum(int currentValue, int newValue);
    Q_INVOKABLE void recordSetNotificationTime(int currentValue, int newValue);
    Q_INVOKABLE void recordSetAcceleration(int axisIndex, int currentValue, int newValue);
    // Operation(d1 = 2)
    Q_INVOKABLE void recordOpePowerOff();
    Q_INVOKABLE void recordOpePowerOn();
    Q_INVOKABLE void recordOpeManualViewPoint(int currentValue, int newValue);
    Q_INVOKABLE void recordOpeFreeManual(int currentValue, int newValue);
    Q_INVOKABLE void recordOpeManualJog(int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordOpeManualFree(int currentValue, int newValue);
    Q_INVOKABLE void recordOpeManualServo(int axisIndex, int currentValue, int newValue);
    Q_INVOKABLE void recordOpeMonitorForceIo(int ioNo, int currentValue, int newValue);
    Q_INVOKABLE void recordOpeMonitorInverseIo(int ioNo, int currentValue, int newValue);
    Q_INVOKABLE void recordOpeMonitorReleaseIo(int ioNo, int currentValue, int newValue);
    Q_INVOKABLE void recordOpeMonitorSwapIo(int currentValue, int newValue);
    Q_INVOKABLE void recordOpeDrvMonitorReset();
    Q_INVOKABLE void recordOpeDrvMonitorStart();
    Q_INVOKABLE void recordOpeDrvMonitorStop();
    Q_INVOKABLE void recordOpeExportAlarmHistory();
    Q_INVOKABLE void recordOpeExportOpeHistory();
    Q_INVOKABLE void recordOpeExportIoHistory();
    // Display(d1 = 3)
    Q_INVOKABLE void recordDispOperableKey(int currentValue, int newValue);
    Q_INVOKABLE void recordDispEmergencyStop(int currentValue, int newValue);
    Q_INVOKABLE void recordDispSystemAlarm(int alarmNo);
    Q_INVOKABLE void recordDispAxisAlarm(int alarmNo, int axisIndex);
    Q_INVOKABLE void recordDispNormalAlarm(int alarmNo);
    Q_INVOKABLE void recordDispOperation(int no);
    // KeyCode(d1 = 4)
    Q_INVOKABLE void recordKeyMembrem(int keyCode);
    Q_INVOKABLE void recordKeyOrigin(int keyCode);
    Q_INVOKABLE void recordKeyAuto(int keyCode);
    Q_INVOKABLE void recordKeyStandardStack(int keyCode, int palletizeNum);
    Q_INVOKABLE void recordKeyFreeStack(int keyCode, int palletizeNum);
    Q_INVOKABLE void recordKeyCounter(int keyCode);
    Q_INVOKABLE void recordKeyResetAbs(int axisIndex);
    Q_INVOKABLE void recordKeySetAbs(int axisIndex);
    Q_INVOKABLE void recordKeyResetAlarmHistory();
    Q_INVOKABLE void recordKeyReturn();
    // Screen Switching(d1 = 5)
    Q_INVOKABLE void recordScreenSwitching(int dp1, int dp2);

private:
    void getdDateTime(QDateTime &dateTime);
    OperationHistoryData *createData(int d1, int d2, int d3, int d4, int d5, int d6, int d7);
signals:

public slots:
private:
    static OperationRecord  *m_instance;
    CommonViewModel         *m_commonView = nullptr;
    OperationHistoryList    *m_operationHistoryList = nullptr;
};

#endif // OPERATIONRECORD_H
