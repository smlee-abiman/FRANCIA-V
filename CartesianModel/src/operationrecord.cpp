#include "operationrecord.h"
#include "operationhistorylist.h"
#include "viewmodel/historyview/operationhistorydata.h"
#include "commonviewmodel.h"

const QHash<int, int> SCREEN_SWITCHING_DP1_D2_CNV_TABLE = {
    { 5,    0   },
    { 6,    1   },
    { 7,    2   },
    { 8,    3   },
    { 10,   4   },
    { 11,   5   },
    { 12,   6   },
    { 13,   7   },
    { 14,   8   },
    { 15,   9   },
    { 16,   10  },
    { 17,   11  },
    { 18,   12  },
    { 19,   13  },
    { 20,   14  },
    { 21,   15  },
    { 22,   16  },
    { 23,   17  },
    { 24,   18  },
    { 25,   19  },
    { 26,   21  },
    { 28,   22  },
    { 29,   23  },
    { 30,   24  },
    { 31,   25  },
};

OperationRecord::OperationRecord(OperationHistoryList *list, QObject *parent)
    : QObject(parent),
      m_operationHistoryList(list)
{

}

OperationRecord *OperationRecord::create_instance(OperationHistoryList *list, QObject *parent)
{
    if (m_instance == nullptr)
        m_instance = new OperationRecord(list, parent);
    return m_instance;
}

OperationRecord *OperationRecord::get_instance()
{
    return m_instance;
}

void OperationRecord::setCommonView(CommonViewModel *viewModel)
{
    m_commonView = viewModel;
}

void OperationRecord::recordSetManualSpeedBySpeedSetting(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 0, 0, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetJogSpeed(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 0, 1, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetAutoSpeed(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 0, 2, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetManualSpeedByManual(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 1, 0, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetMode(int mode, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 4, 0, mode, 0, newValue, currentValue));
}

void OperationRecord::recordSetPositionByPosSet(int pointIndex, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 5, 0, pointIndex, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetSpeedByPosSet(int pointIndex, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 5, 1, pointIndex, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetPositionByPosFix(int pointIndex, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 5, 2, pointIndex, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetSpeedByPosFix(int pointIndex, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 5, 3, pointIndex, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetStdStackPalletizeNumber(int palletizeNum, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 0, palletizeNum, 0, newValue, currentValue));
}

void OperationRecord::recordSetStdStackDownDeceleratePosition(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 1, 0, 4, newValue, currentValue));
}

void OperationRecord::recordSetStdStackDownDecelerateSpeed(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 2, 0, 4, newValue, currentValue));
}

void OperationRecord::recordSetStdStackUpDeceleratePosition(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 3, 0, 4, newValue, currentValue));
}

void OperationRecord::recordSetStdStackUpDecelerateSpeed(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 4, 0, 4, newValue, currentValue));
}

void OperationRecord::recordSetStdStackStartSpeed(int palletizeNum, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 5, palletizeNum, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetStdStackStartPos(int palletizeNum, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 6, palletizeNum, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetStdStackCurrentPoint(int palletizeNum, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 7, palletizeNum, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetStdStackPoint(int palletizeNum, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 8, palletizeNum, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetStdStackPitch(int palletizeNum, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 9, palletizeNum, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetStdStackDirection(int palletizeNum, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 10, palletizeNum, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetFreeStackCurrentPoint(int palletizeNum, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 11, palletizeNum, 0, newValue, currentValue));
}

void OperationRecord::recordSetFreeStackPoint(int palletizeNum, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 6, 12, palletizeNum, 0, newValue, currentValue));
}

void OperationRecord::recordSetTimer(int timerId, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 7, 0, timerId, 0, newValue, currentValue));
}

void OperationRecord::recordSetCounter(int counterId, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 8, 0, counterId, 0, newValue, currentValue));
}

void OperationRecord::recordSetPass(int passId, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 9, 0, passId, 0, newValue, currentValue));
}

void OperationRecord::recordProgramCopy(int programNo)
{
    m_operationHistoryList->add(createData(1, 10, 0, programNo, 0, 0, 0));
}

void OperationRecord::recordProgramPaste(int programNo)
{
    m_operationHistoryList->add(createData(1, 10, 1, programNo, 0, 0, 0));
}

void OperationRecord::recordProgramDelete(int programNo)
{
    m_operationHistoryList->add(createData(1, 10, 2, programNo, 0, 0, 0));
}

void OperationRecord::recordProgramSave(int programNo)
{
    m_operationHistoryList->add(createData(1, 10, 3, programNo, 0, 0, 0));
}

void OperationRecord::recordProgramMoveUp(int programNo)
{
    m_operationHistoryList->add(createData(1, 10, 4, programNo, 0, 0, 0));
}

void OperationRecord::recordProgramMoveDown(int programNo)
{
    m_operationHistoryList->add(createData(1, 10, 5, programNo, 0, 0, 0));
}

void OperationRecord::recordProgramReplace(int programNo)
{
    m_operationHistoryList->add(createData(1, 10, 6, programNo, 0, 0, 0));
}

void OperationRecord::recordProgramInsert(int programNo)
{
    m_operationHistoryList->add(createData(1, 10, 7, programNo, 0, 0, 0));
}

void OperationRecord::recordSetScreenBright(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 11, 0, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetScreenOffTime(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 11, 1, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetSystemProgram(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 11, 2, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetTakeUpType(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 11, 3, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetSystemMode(int id, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 11, 4, id, 0, newValue, currentValue));
}

void OperationRecord::recordSetModelType(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 12, 0, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetMachineParam(int type, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 12, 1, type, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSaveMachineParam()
{
    m_operationHistoryList->add(createData(1, 12, 2, 0, 0, 0, 0));
}

void OperationRecord::recordSaveDriverParam()
{
    m_operationHistoryList->add(createData(1, 12, 3, 0, 0, 0, 0));
}

void OperationRecord::recordInitDriverParam()
{
    m_operationHistoryList->add(createData(1, 12, 4, 0, 0, 0, 0));
}

void OperationRecord::recordSetDriverParam(int itemNo, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 12, 5, itemNo, axisIndex, newValue, currentValue));
}

void OperationRecord::recordSetSimpleSetting(int itemNo, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 12, 6, itemNo, axisIndex, newValue, currentValue));
}

void OperationRecord::recordResetDrvMonitor()
{
    m_operationHistoryList->add(createData(1, 12, 7, 0, 0, 0, 0));
}

void OperationRecord::recordStartDrvMonitor()
{
    m_operationHistoryList->add(createData(1, 12, 8, 0, 0, 0, 0));
}

void OperationRecord::recordStopDrvMonitor()
{
    m_operationHistoryList->add(createData(1, 12, 9, 0, 0, 0, 0));
}

void OperationRecord::recordAreaSet(int itemNo, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 13, 0, itemNo, axisIndex, newValue, currentValue));
}

void OperationRecord::recordAreaSetServo(int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 13, 1, 6, axisIndex, newValue, currentValue));
}

void OperationRecord::recordAreaSetMemory(int itemNo, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 13, 2, itemNo + 10, axisIndex, newValue, currentValue));
}

void OperationRecord::recordAreaFix(int itemNo, int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 13, 3, itemNo, axisIndex, newValue, currentValue));
}

void OperationRecord::recordAreaFixMxSx(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 13, 4, 6, 0, newValue, currentValue));
}

void OperationRecord::recordInspectionCompleted(int itemNo)
{
    m_operationHistoryList->add(createData(1, 14, 0, itemNo, 0, 0, 0));
}

void OperationRecord::recordInspectionSaved()
{
    m_operationHistoryList->add(createData(1, 14, 1, 0, 0, 0, 0));
}

void OperationRecord::recordRefuelingCompleted(int axisIndex)
{
    m_operationHistoryList->add(createData(1, 14, 2, axisIndex, 0, 0, 0));
}

void OperationRecord::recordRefuelingSaved()
{
    m_operationHistoryList->add(createData(1, 14, 3, 0, 0, 0, 0));
}

void OperationRecord::recordSetProductionProgNum(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 15, 0, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetOneFetchNum(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 15, 1, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetNotificationTime(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 15, 2, 0, 0, newValue, currentValue));
}

void OperationRecord::recordSetAcceleration(int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(1, 17, 0, axisIndex, 0, newValue, currentValue));
}

void OperationRecord::recordOpePowerOff()
{
    m_operationHistoryList->add(createData(2, 40, 0, 0, 0, 0, 0));
}

void OperationRecord::recordOpePowerOn()
{
    m_operationHistoryList->add(createData(2, 41, 0, 0, 0, 0, 0));
}

void OperationRecord::recordOpeManualViewPoint(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 1, 0, 0, 0, newValue, currentValue));
}

void OperationRecord::recordOpeFreeManual(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 1, 1, 0, 0, newValue, currentValue));
}

void OperationRecord::recordOpeManualJog(int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 1, 2, axisIndex, 0, newValue, currentValue));
}

void OperationRecord::recordOpeManualFree(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 1, 3, 0, 0, newValue, currentValue));
}

void OperationRecord::recordOpeManualServo(int axisIndex, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 1, 4, axisIndex, 0, newValue, currentValue));
}

void OperationRecord::recordOpeMonitorForceIo(int ioNo, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 3, 0, ioNo, 0, newValue, currentValue));
}

void OperationRecord::recordOpeMonitorInverseIo(int ioNo, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 3, 1, ioNo, 0, newValue, currentValue));
}

void OperationRecord::recordOpeMonitorReleaseIo(int ioNo, int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 3, 2, ioNo, 0, newValue, currentValue));
}

void OperationRecord::recordOpeMonitorSwapIo(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(2, 3, 3, 0, 0, newValue, currentValue));
}

void OperationRecord::recordOpeDrvMonitorReset()
{
    m_operationHistoryList->add(createData(2, 3, 4, 0, 0, 0, 0));
}

void OperationRecord::recordOpeDrvMonitorStart()
{
    m_operationHistoryList->add(createData(2, 3, 5, 0, 0, 0, 0));
}

void OperationRecord::recordOpeDrvMonitorStop()
{
    m_operationHistoryList->add(createData(2, 3, 6, 0, 0, 0, 0));
}

void OperationRecord::recordOpeExportAlarmHistory()
{
    m_operationHistoryList->add(createData(2, 19, 0, 0, 0, 0, 0));
}

void OperationRecord::recordOpeExportOpeHistory()
{
    m_operationHistoryList->add(createData(2, 19, 1, 0, 0, 0, 0));
}

void OperationRecord::recordOpeExportIoHistory()
{
    m_operationHistoryList->add(createData(2, 19, 2, 0, 0, 0, 0));
}

void OperationRecord::recordDispOperableKey(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(3, 34, 0, 0, 0, newValue, currentValue));
}

void OperationRecord::recordDispEmergencyStop(int currentValue, int newValue)
{
    if (currentValue != newValue)
        m_operationHistoryList->add(createData(3, 35, 0, 0, 0, newValue, currentValue));
}

void OperationRecord::recordDispSystemAlarm(int alarmNo)
{
    m_operationHistoryList->add(createData(3, 30, 0, alarmNo, 0, 0, 0));
}

void OperationRecord::recordDispAxisAlarm(int alarmNo, int axisIndex)
{
    m_operationHistoryList->add(createData(3, 31, 0, alarmNo, axisIndex, 0, 0));
}

void OperationRecord::recordDispNormalAlarm(int alarmNo)
{
    m_operationHistoryList->add(createData(3, 32, 0, alarmNo, 0, 0, 0));
}

void OperationRecord::recordDispOperation(int no)
{
    m_operationHistoryList->add(createData(3, 33, 0, no, 0, 0, 0));
}

void OperationRecord::recordKeyMembrem(int keyCode)
{
    if ((keyCode >= Enums::MemblemKeyNoKey1) && (keyCode <= Enums::MemblemKeyNoKey18)) {
        m_operationHistoryList->add(createData(4, 42, keyCode, 0, 0, 0, 0));
    } else if ((keyCode >= Enums::CounterKeyResetBase) && (keyCode < Enums::PalletizeKeyReset)) {
        recordKeyCounter(keyCode);
    } else {
        switch (keyCode) {
        case Enums::PalletizeKeyReset:
        case Enums::PalletizeKeyAllReset:
        {
            auto mainScreenId = m_commonView->curMainScreenId();
            if (mainScreenId == Enums::MainScreenIdTeachingPalletize) {
                auto subScreenId = m_commonView->curSubScreenId();
                if ((subScreenId >= 0) && (subScreenId <= 7)) {
                    recordKeyStandardStack(keyCode, subScreenId);
                } else if ((subScreenId >= 8 && subScreenId <= 15)) {
                    recordKeyFreeStack(keyCode, subScreenId - 8);
                }
            }
        }
            break;
        case 242:
        case 243:
        {
            auto mainScreenId = m_commonView->curMainScreenId();
            if (mainScreenId == Enums::MainScreenIdOrigin) {
                recordKeyOrigin(keyCode);
            } else if (mainScreenId == Enums::MainScreenIdAuto) {
                recordKeyAuto(keyCode);
            }
        }
            break;
        case 240:
        case 241:
        case 244:
        case 245:
        case 246:
            recordKeyAuto(keyCode);
            break;
        default:
            break;
        }
    }
}

void OperationRecord::recordKeyOrigin(int keyCode)
{
    m_operationHistoryList->add(createData(4, 0, keyCode, 0, 0, 0, 0));
}

void OperationRecord::recordKeyAuto(int keyCode)
{
    m_operationHistoryList->add(createData(4, 2, keyCode, 0, 0, 0, 0));
}

void OperationRecord::recordKeyStandardStack(int keyCode, int palletizeNum)
{
    m_operationHistoryList->add(createData(4, 6, keyCode, palletizeNum, 0, 0, 0));
}

void OperationRecord::recordKeyFreeStack(int keyCode, int palletizeNum)
{
    m_operationHistoryList->add(createData(4, 7, keyCode, palletizeNum, 0, 0, 0));
}

void OperationRecord::recordKeyCounter(int keyCode)
{
    m_operationHistoryList->add(createData(4, 8, 0, keyCode, 0, 0, 0));
}

void OperationRecord::recordKeyResetAbs(int axisIndex)
{
    m_operationHistoryList->add(createData(4, 16, 0, axisIndex, 0, 0, 0));
}

void OperationRecord::recordKeySetAbs(int axisIndex)
{
    m_operationHistoryList->add(createData(4, 16, 1, axisIndex, 0, 0, 0));
}

void OperationRecord::recordKeyResetAlarmHistory()
{
    m_operationHistoryList->add(createData(4, 19, 247, 0, 0, 0, 0));
}

void OperationRecord::recordKeyReturn()
{
    m_operationHistoryList->add(createData(4, 26, 247, 0, 0, 0, 0));
}

void OperationRecord::recordScreenSwitching(int dp1, int dp2)
{
    int d2 = SCREEN_SWITCHING_DP1_D2_CNV_TABLE.value(dp1, -1);
    if (d2 >= 0)
        m_operationHistoryList->add(createData(5, d2, dp1, dp2, 0, 0, 0));
}

OperationHistoryData *OperationRecord::createData(int d1, int d2, int d3, int d4, int d5, int d6, int d7)
{
    auto data = new OperationHistoryData();
    if (data == nullptr) {
        qWarning() << __func__ << "Failed to generate data." << d1 << d2 << d3 << d4 << d5 << d6 << d7;
    } else {
        QDateTime dateTime;
        getdDateTime(dateTime);
        data->setDateTime(dateTime);
        data->setD1(d1);
        data->setD2(d2);
        data->setD3(d3);
        data->setD4(d4);
        data->setD5(d5);
        data->setD6(d6);
        data->setD7(d7);
    }
    return data;
}

void OperationRecord::getdDateTime(QDateTime &dateTime)
{
#ifdef REAL_TARGET
    if (m_commonView)
        dateTime = m_commonView->getDateTime();
#else
    dateTime = QDateTime::currentDateTime();
#endif
}

OperationRecord* OperationRecord::m_instance = nullptr;
