#ifndef OPERATIONHISTORYSTRING_H
#define OPERATIONHISTORYSTRING_H

#include <QObject>
#include "operationhistorydata.h"

class ResourceManager;
class FunctionViewModel;
class ModelSelectViewModel;

class OperationHistoryString : public QObject
{
    Q_OBJECT
public:
    explicit OperationHistoryString(ResourceManager *resource, FunctionViewModel *functionView, QObject *parent = nullptr);
    Q_INVOKABLE QString toString(OperationHistoryData *data, int language);

    void toStringSet(QString &string, OperationHistoryData *data, int language);
    void toStringOperation(QString &string, OperationHistoryData *data, int language);
    void toStringDisplay(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCode(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitching(QString &string, OperationHistoryData *data, int language);

    // Set
    void toStringSetSpeedSetting(QString &string, OperationHistoryData *data, int language);
    void toStringSetManualOperation(QString &string, OperationHistoryData *data, int language);
    void toStringSetMode(QString &string, OperationHistoryData *data, int language);
    void toStringSetPosSet(QString &string, OperationHistoryData *data, int language);
    void toStringSetStack(QString &string, OperationHistoryData *data, int language);
    void toStringSetTimer(QString &string, OperationHistoryData *data, int language);
    void toStringSetCounter(QString &string, OperationHistoryData *data, int language);
    void toStringSetPassSetting(QString &string, OperationHistoryData *data, int language);
    void toStringSetProg(QString &string, OperationHistoryData *data, int language);
    void toStringSetController(QString &string, OperationHistoryData *data, int language);
    void toStringSetMachineParameter(QString &string, OperationHistoryData *data, int language);
    void toStringSetAreaSet(QString &string, OperationHistoryData *data, int language);
    void toStringSetMaintenance(QString &string, OperationHistoryData *data, int language);
    void toStringSetProductionManagement(QString &string, OperationHistoryData *data, int language);
    void toStringSetAccSetting(QString &string, OperationHistoryData *data, int language);

    // Operation
    void toStringOperationPowerOff(QString &string, OperationHistoryData *data, int language);
    void toStringOperationPowerOn(QString &string, OperationHistoryData *data, int language);
    void toStringOperationManual(QString &string, OperationHistoryData *data, int language);
    void toStringOperationMonitor(QString &string, OperationHistoryData *data, int language);
    void toStringOperationHistory(QString &string, OperationHistoryData *data, int language);

    // Display
    void toStringDisplayMovementPossible(QString &string, OperationHistoryData *data, int language);
    void toStringDisplayEmergencyStop(QString &string, OperationHistoryData *data, int language);
    void toStringDisplaySystemAlarm(QString &string, OperationHistoryData *data, int language);
    void toStringDisplayAxisAlarm(QString &string, OperationHistoryData *data, int language);
    void toStringDisplayNormalAlarm(QString &string, OperationHistoryData *data, int language);
    void toStringDisplayOperation(QString &string, OperationHistoryData *data, int language);

    // KeyCode
    void toStringKeyCodeMemblemSw(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCodeOrigin(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCodeAuto(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCodeStandardStack(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCodeFreeStack(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCodeCounter(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCodeAbs(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCodeHistory(QString &string, OperationHistoryData *data, int language);
    void toStringKeyCodeReturn(QString &string, OperationHistoryData *data, int language);

    // ViewSwitching
    void toStringViewSwitchingOrigin(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingManual(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingAuto(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingMonitor(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingTeachingMode(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingTeachingPos(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingTeachingStack(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingTeachingTimer(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingTeachingCounter(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingTeachingPass(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingTeachingProgram(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFuncController(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFuncMachineParam(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFuncAreaSet(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFuncMaintain(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFuncProductionMng(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFuncAbsSet(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFuncDeceleration(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFuncVersionMng(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingHistory(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingFile(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingUser(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingSystemTime(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingSpeed(QString &string, OperationHistoryData *data, int language);
    void toStringViewSwitchingLanguage(QString &string, OperationHistoryData *data, int language);

private:
    QString getPointName(int pointNo);
    QString getAxisString(int language);
    QString getIoSymbol(int diAddress);
    QString getSystemProgChoiceName(int progIndex, int language);
    QString getFetchingMachineTypeName(int type, int language);
    QString getSystemModeName(int mode, int language);
    QString getMachineParameterName(int index, int language);
    QString getMachineParameterValue(int index, int value);
    int getMachineParameterFormatShift(int index);
    int getMachineParameterFormat(int index);
    QString getDriverParameterName(int index, int language);
    QString getAxisOperationTypeName(int type, int language);
    QString getSystemAlarmName(int index, int language);
    QString getAxisAlarmName(int index, int language);
    QString getNormalAlarmName(int index, int language);
    QString getStackOrderName(int stackIndex, int value);
    QString getSimpleSetName(int type, int language);
    QString getAreaSetName(int type, int language);
    QString getOpeMsg(int index, int language);
    QString getModelName(int index);
    QString getCounterName(int index);

private:
    ResourceManager         *m_resource = nullptr;
    ModelSelectViewModel    *m_modelSelectViewModel;
};

#endif // OPERATIONHISTORYSTRING_H
