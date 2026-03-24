#include "operationhistorystring.h"
#include "operationhistorydata.h"
#include "resourcemanager.h"
#include "viewmodel/functionview/functionviewmodel.h"
#include "viewmodel/functionview/mechanicalparamview/modelselectviewmodel.h"
#include <QHash>

typedef void (OperationHistoryString::*TO_STRING_FUNC)(QString &, OperationHistoryData *, int);

const QHash<int, TO_STRING_FUNC> TO_STRING_TYPE_FUNC_LIST = {
    { 1,    &OperationHistoryString::toStringSet            },
    { 2,    &OperationHistoryString::toStringOperation      },
    { 3,    &OperationHistoryString::toStringDisplay        },
    { 4,    &OperationHistoryString::toStringKeyCode        },
    { 5,    &OperationHistoryString::toStringViewSwitching  },
};

const QHash<int, TO_STRING_FUNC> TO_STRING_SET_FUNC_LIST = {
    { 0,    &OperationHistoryString::toStringSetSpeedSetting            },
    { 1,    &OperationHistoryString::toStringSetManualOperation         },
    { 4,    &OperationHistoryString::toStringSetMode                    },
    { 5,    &OperationHistoryString::toStringSetPosSet                  },
    { 6,    &OperationHistoryString::toStringSetStack                   },
    { 7,    &OperationHistoryString::toStringSetTimer                   },
    { 8,    &OperationHistoryString::toStringSetCounter                 },
    { 9,    &OperationHistoryString::toStringSetPassSetting             },
    { 10,   &OperationHistoryString::toStringSetProg                    },
    { 11,   &OperationHistoryString::toStringSetController              },
    { 12,   &OperationHistoryString::toStringSetMachineParameter        },
    { 13,   &OperationHistoryString::toStringSetAreaSet                 },
    { 14,   &OperationHistoryString::toStringSetMaintenance             },
    { 15,   &OperationHistoryString::toStringSetProductionManagement    },
    { 17,   &OperationHistoryString::toStringSetAccSetting              },
};

const QHash<int, TO_STRING_FUNC> TO_STRING_DISPLAY_FUNC_LIST = {
    { 34,   &OperationHistoryString::toStringDisplayMovementPossible    },
    { 35,   &OperationHistoryString::toStringDisplayEmergencyStop       },
    { 30,   &OperationHistoryString::toStringDisplaySystemAlarm         },
    { 31,   &OperationHistoryString::toStringDisplayAxisAlarm           },
    { 32,   &OperationHistoryString::toStringDisplayNormalAlarm         },
    { 33,   &OperationHistoryString::toStringDisplayOperation           },
};

const QHash<int, TO_STRING_FUNC> TO_STRING_OPERATION_FUNC_LIST = {
    { 40,   &OperationHistoryString::toStringOperationPowerOff          },
    { 41,   &OperationHistoryString::toStringOperationPowerOn           },
    { 1,    &OperationHistoryString::toStringOperationManual            },
    { 3,    &OperationHistoryString::toStringOperationMonitor           },
    { 19,   &OperationHistoryString::toStringOperationHistory           },
};

const QHash<int, TO_STRING_FUNC> TO_STRING_KEY_CODE_FUNC_LIST = {
    { 42,   &OperationHistoryString::toStringKeyCodeMemblemSw           },
    { 0,    &OperationHistoryString::toStringKeyCodeOrigin              },
    { 2,    &OperationHistoryString::toStringKeyCodeAuto                },
    { 6,    &OperationHistoryString::toStringKeyCodeStandardStack       },
    { 7,    &OperationHistoryString::toStringKeyCodeFreeStack           },
    { 8,    &OperationHistoryString::toStringKeyCodeCounter             },
    { 16,   &OperationHistoryString::toStringKeyCodeAbs                 },
    { 19,   &OperationHistoryString::toStringKeyCodeHistory             },
    { 26,   &OperationHistoryString::toStringKeyCodeReturn              },
};

const QHash<int, TO_STRING_FUNC> TO_STRING_VIEW_SWITCHING_FUNC_LIST = {
    { 0,    &OperationHistoryString::toStringViewSwitchingOrigin                },
    { 1,    &OperationHistoryString::toStringViewSwitchingManual                },
    { 2,    &OperationHistoryString::toStringViewSwitchingAuto                  },
    { 3,    &OperationHistoryString::toStringViewSwitchingMonitor               },
    { 4,    &OperationHistoryString::toStringViewSwitchingTeachingMode          },
    { 5,    &OperationHistoryString::toStringViewSwitchingTeachingPos           },
    { 6,    &OperationHistoryString::toStringViewSwitchingTeachingStack         },
    { 7,    &OperationHistoryString::toStringViewSwitchingTeachingTimer         },
    { 8,    &OperationHistoryString::toStringViewSwitchingTeachingCounter       },
    { 9,    &OperationHistoryString::toStringViewSwitchingTeachingPass          },
    { 10,   &OperationHistoryString::toStringViewSwitchingTeachingProgram       },
    { 11,   &OperationHistoryString::toStringViewSwitchingFuncController        },
    { 12,   &OperationHistoryString::toStringViewSwitchingFuncMachineParam      },
    { 13,   &OperationHistoryString::toStringViewSwitchingFuncAreaSet           },
    { 14,   &OperationHistoryString::toStringViewSwitchingFuncMaintain          },
    { 15,   &OperationHistoryString::toStringViewSwitchingFuncProductionMng     },
    { 16,   &OperationHistoryString::toStringViewSwitchingFuncAbsSet            },
    { 17,   &OperationHistoryString::toStringViewSwitchingFuncDeceleration      },
    { 18,   &OperationHistoryString::toStringViewSwitchingFuncVersionMng        },
    { 19,   &OperationHistoryString::toStringViewSwitchingHistory               },
    { 21,   &OperationHistoryString::toStringViewSwitchingFile                  },
    { 22,   &OperationHistoryString::toStringViewSwitchingUser                  },
    { 23,   &OperationHistoryString::toStringViewSwitchingSystemTime            },
    { 24,   &OperationHistoryString::toStringViewSwitchingSpeed                 },
    { 25,   &OperationHistoryString::toStringViewSwitchingLanguage              },
};

const QList<QList<int>> STACK_ORDER_VALUE_LIST = {
    {    16,    9,   11,   18,    4,    5 },  // Stack1
    {    40,   33,   35,   42,   28,   29 },  // Stack2
    {    64,   57,   59,   66,   52,   53 },  // Stack3
    {    88,   81,   83,   90,   76,   77 },  // Stack4
    {   112,  105,  107,  114,  100,  101 },  // Stack5
    {   136,  129,  131,  138,  124,  125 },  // Stack6
    {   160,  153,  155,  162,  148,  149 },  // Stack7
    {   184,  177,  179,  186,  172,  173 },  // Stack8
};

const QStringList STACK_ORDER_STRING_LIST = {
    "軸1→軸2→軸4",
    "軸1→軸4→軸2",
    "軸4→軸1→軸2",
    "軸2→軸1→軸4",
    "軸2→軸4→軸1",
    "軸4→軸2→軸1",
};

OperationHistoryString::OperationHistoryString(ResourceManager *resource, FunctionViewModel *functionView, QObject *parent)
    : QObject(parent),
      m_resource(resource)
{
    m_modelSelectViewModel = functionView->mechanicalParamView()->modelSelectView();
}

QString OperationHistoryString::toString(OperationHistoryData *data, int language)
{
    QString ret("");
    int type = data->d1();
    if (TO_STRING_TYPE_FUNC_LIST.contains(type)) {
        auto func = TO_STRING_TYPE_FUNC_LIST.value(type);
        (this->*func)(ret, data, language);
    }
    return ret;
}


void OperationHistoryString::toStringSet(QString &string, OperationHistoryData *data, int language)
{
    int item = data->d2();
    if (TO_STRING_SET_FUNC_LIST.contains(item)) {
        auto func = TO_STRING_SET_FUNC_LIST.value(item);
        (this->*func)(string, data, language);
    }
}

void OperationHistoryString::toStringSetSpeedSetting(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2→%3").arg(m_resource->label(50, 18, language)).arg(data->d7()).arg(data->d6());
        break;
    case 1:
        string += QString("%1  %2→%3").arg(m_resource->label(50, 19, language)).arg(data->d7()).arg(data->d6());
        break;
    case 2:
        string += QString("%1  %2→%3%").arg(m_resource->label(50, 20, language)).arg((data->d7() + 1) * 10).arg((data->d6() + 1) * 10);
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringSetManualOperation(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2→%3").arg(m_resource->label(50, 21, language)).arg(data->d7()).arg(data->d6());
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringSetMode(QString &string, OperationHistoryData *data, int language)
{
    if (data->d3() != 0) {
        return;
    }
    QString onOff = data->d6() == 0 ? "OFF" : "ON";
    string += QString("%1  %2%3 = %4").arg(m_resource->label(50, 22, language)).arg(m_resource->label(34, 1, language)).arg(data->d4() + 1, 2, 10, QChar('0')).arg(onOff);
}

void OperationHistoryString::toStringSetPosSet(QString &string, OperationHistoryData *data, int language)
{
    auto axisString = getAxisString(language);
    switch (data->d3()) {
    case 0:
        string += QString("%1(%2) %3 = %4→%5mm %6%7").arg(m_resource->label(50, 23, language)).arg(m_resource->label(50, 25, language)).arg(getPointName(data->d4())).arg(m_resource->formatData(data->d7(), 66, 2)).arg(m_resource->formatData(data->d6(), 66, 2)).arg(axisString).arg(data->d5() + 1);
        break;
    case 1:
        string += QString("%1(%2) %3 = %4→%5% %6%7").arg(m_resource->label(50, 23, language)).arg(m_resource->label(50, 26, language)).arg(getPointName(data->d4())).arg(data->d7()).arg(data->d6()).arg(axisString).arg(data->d5() + 1);
        break;
    case 2:
        string += QString("%1(%2) %3 = %4→%5mm %6%7").arg(m_resource->label(50, 24, language)).arg(m_resource->label(50, 25, language)).arg(getPointName(data->d4())).arg(m_resource->formatData(data->d7(), 66, 2)).arg(m_resource->formatData(data->d6(), 66, 2)).arg(axisString).arg(data->d5() + 1);
        break;
    case 3:
        string += QString("%1(%2) %3 = %4→%5% %6%7").arg(m_resource->label(50, 24, language)).arg(m_resource->label(50, 26, language)).arg(getPointName(data->d4())).arg(data->d7()).arg(data->d6()).arg(axisString).arg(data->d5() + 1);
        break;
    default:
        return;
    }
}

void OperationHistoryString::toStringSetStack(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1%2 %3 = %4 → %5").arg(m_resource->label(50, 27, language)).arg(data->d4() + 1).arg(m_resource->label(50, 30, language)).arg(getStackOrderName(data->d4(), data->d7())).arg(getStackOrderName(data->d4(), data->d6()));
        break;
    case 1:
        string += QString("%1(%2) = %3→%4mm %5%6").arg(m_resource->label(50, 28, language)).arg(m_resource->label(50, 25, language)).arg(m_resource->formatData(data->d7(), 64, 2)).arg(m_resource->formatData(data->d6(), 64, 2)).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 2:
        string += QString("%1(%2) = %3→%4% %5%6").arg(m_resource->label(50, 28, language)).arg(m_resource->label(50, 26, language)).arg(data->d7()).arg(data->d6()).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 3:
        string += QString("%1(%2) = %3→%4mm %5%6").arg(m_resource->label(50, 29, language)).arg(m_resource->label(50, 25, language)).arg(m_resource->formatData(data->d7(), 64, 2)).arg(m_resource->formatData(data->d6(), 64, 2)).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 4:
        string += QString("%1(%2) = %3→%4% %5%6").arg(m_resource->label(50, 29, language)).arg(m_resource->label(50, 26, language)).arg(data->d7()).arg(data->d6()).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 5:
        string += QString("%1%2 %3 = %4→%5% %6%7").arg(m_resource->label(50, 27, language)).arg(data->d4() + 1).arg(m_resource->label(50, 33, language)).arg(data->d7()).arg(data->d6()).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 6:
        string += QString("%1%2 %3 = %4→%5mm %6%7").arg(m_resource->label(50, 27, language)).arg(data->d4() + 1).arg(m_resource->label(50, 34, language)).arg(m_resource->formatData(data->d7(), 66, 2)).arg(m_resource->formatData(data->d6(), 66, 2)).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 7:
        string += QString("%1%2 %3 = %4→%5 %6%7").arg(m_resource->label(50, 27, language)).arg(data->d4() + 1).arg(m_resource->label(50, 35, language)).arg(data->d7()).arg(data->d6()).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 8:
        string += QString("%1%2 %3 = %4→%5 %6%7").arg(m_resource->label(50, 27, language)).arg(data->d4() + 1).arg(m_resource->label(50, 36, language)).arg(data->d7()).arg(data->d6()).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 9:
        string += QString("%1%2 %3 = %4→%5mm %6%7").arg(m_resource->label(50, 27, language)).arg(data->d4() + 1).arg(m_resource->label(50, 37, language)).arg(m_resource->formatData(data->d7(), 50, 2)).arg(m_resource->formatData(data->d6(), 50, 2)).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 10:
    {
        QString beforeSign = data->d7() == 0 ? "-" : "+";
        QString afterSign = data->d6() == 0 ? "-" : "+";
        string += QString("%1%2 %3 = %4→%5 %6%7").arg(m_resource->label(50, 27, language)).arg(data->d4() + 1).arg(m_resource->label(50, 38, language)).arg(beforeSign).arg(afterSign).arg(getAxisString(language)).arg(data->d5() + 1);
    }
        break;
    case 11:
        string += QString("%1%2 %3 = %4→%5").arg(m_resource->label(50, 39, language)).arg(data->d4() + 1).arg(m_resource->label(50, 35, language)).arg(data->d7()).arg(data->d6());
        break;
    case 12:
        string += QString("%1%2 %3 = %4→%5").arg(m_resource->label(50, 39, language)).arg(data->d4() + 1).arg(m_resource->label(50, 36, language)).arg(data->d7()).arg(data->d6());
        break;
    default:
        return;
    }
}

void OperationHistoryString::toStringSetTimer(QString &string, OperationHistoryData *data, int language)
{
    string += QString("%1 T%2 = %3→%4sec").arg(m_resource->label(50, 41, language)).arg(data->d4() + 1).arg(m_resource->formatData(data->d7(), 50, 2)).arg(m_resource->formatData(data->d6(), 50, 2));
}

void OperationHistoryString::toStringSetCounter(QString &string, OperationHistoryData *data, int language)
{
    string += QString("%1  %2 = %3→%4").arg(m_resource->label(50, 42, language)).arg(getCounterName(data->d4())).arg(data->d7()).arg(data->d6());
}

void OperationHistoryString::toStringSetPassSetting(QString &string, OperationHistoryData *data, int language)
{
    QString passName("");
    int passIndex = data->d4();
    if ((passIndex >= 0) && (passIndex <= 5)) {
        passName = m_resource->label(40, 1 + passIndex, language);
    }
    string += QString("%1  %2 = %3→%4mm").arg(m_resource->label(50, 43, language)).arg(passName).arg(m_resource->formatData(data->d7(), 81, 2)).arg(m_resource->formatData(data->d6(), 81, 2));
}

void OperationHistoryString::toStringSetProg(QString &string, OperationHistoryData *data, int language)
{
    int d3 = data->d3();
    if ((d3 >= 0) && (d3 <= 7)) {
        int labelIndex = 44 + d3;
        string += QString("%1").arg(m_resource->label(50, labelIndex, language));

    }
}

void OperationHistoryString::toStringSetController(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 52, language)).arg(data->d7()).arg(data->d6());
        break;
    case 1:
        string += QString("%1 = %2→%3min").arg(m_resource->label(50, 53, language)).arg(data->d7()).arg(data->d6());
        break;
    case 2:
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 54, language)).arg(getSystemProgChoiceName(data->d7(), language)).arg(getSystemProgChoiceName(data->d6(), language));
        break;
    case 3:
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 55, language)).arg(getFetchingMachineTypeName(data->d7(), language)).arg(getFetchingMachineTypeName(data->d6(), language));
        break;
    case 4:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1 = %2→%3").arg(getSystemModeName(data->d4(), language)).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringSetMachineParameter(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 57, language)).arg(getModelName(data->d7())).arg(getModelName(data->d6()));
        break;
    case 1:
    {
        auto dataType = data->d4();
        string += QString("%1 = %2→%3 %4%5").arg(getMachineParameterName(dataType, language)).arg(getMachineParameterValue(dataType, data->d7())).arg(getMachineParameterValue(dataType, data->d6())).arg(getAxisString(language)).arg(data->d5() + 1);
    }
        break;
    case 2:
        string += QString("%1").arg(m_resource->label(50, 58, language));
        break;
    case 3:
        string += QString("%1  %2").arg(m_resource->label(50, 59, language)).arg(m_resource->label(50, 60, language));
        break;
    case 4:
        string += QString("%1  %2").arg(m_resource->label(50, 59, language)).arg(m_resource->label(50, 61, language));
        break;
    case 5:
        string += QString("%1 = %2→%3 %4%5").arg(getDriverParameterName(data->d4(), language)).arg(data->d7()).arg(data->d6()).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 6:
        string += QString("%1  %2 = %3→%4 %5%6").arg(m_resource->label(50, 62, language)).arg(getSimpleSetName(data->d4(), language)).arg(data->d7()).arg(data->d6()).arg(getAxisString(language)).arg(data->d5() + 1);
        break;
    case 7:
        string += QString("%1  %2").arg(m_resource->label(50, 63, language)).arg(m_resource->label(50, 64, language));
        break;
    case 8:
        string += QString("%1  %2").arg(m_resource->label(50, 63, language)).arg(m_resource->label(50, 65, language));
        break;
    case 9:
        string += QString("%1  %2").arg(m_resource->label(50, 63, language)).arg(m_resource->label(50, 66, language));
        break;
    default:
        break;
    }

}

void OperationHistoryString::toStringSetAreaSet(QString &string, OperationHistoryData *data, int language)
{
    auto axisString = getAxisString(language);
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2 = %3→%4mm %5%6").arg(m_resource->label(44, 1, language)).arg(getAreaSetName(data->d4(), language)).arg(m_resource->formatData(data->d7(), 66, 2)).arg(m_resource->formatData(data->d6(), 66, 2)).arg(axisString).arg(data->d5() + 1);
        break;
    case 1:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1  %2 = %3→%4 %5%6").arg(m_resource->label(44, 1, language)).arg(m_resource->label(50, 179, language)).arg(beforeOnOff).arg(afterOnOff).arg(axisString).arg(data->d5() + 1);
    }
        break;
    case 2:
        string += QString("%1  %2 = %3→%4mm %5%6 %7").arg(m_resource->label(44, 1, language)).arg(getAreaSetName(data->d4() - 10, language)).arg(m_resource->formatData(data->d7(), 66, 2)).arg(m_resource->formatData(data->d6(), 66, 2)).arg(axisString).arg(data->d5() + 1).arg(m_resource->label(44, 24, language));
        break;
    case 3:
        string += QString("%1  %2 = %3→%4mm %5%6").arg(m_resource->label(44, 2, language)).arg(getAreaSetName(data->d4(), language)).arg(m_resource->formatData(data->d7(), 66, 2)).arg(m_resource->formatData(data->d6(), 66, 2)).arg(axisString).arg(data->d5() + 1);
        break;
    case 4:
        string += QString("%1  %2 = %3→%4mm").arg(m_resource->label(44, 2, language)).arg(m_resource->label(44, 23, language)).arg(m_resource->formatData(data->d7(), 66, 2)).arg(m_resource->formatData(data->d6(), 66, 2));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringSetMaintenance(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2").arg(m_resource->label(50, 69, language)).arg(m_resource->label(50, 70, language));
        break;
    case 1:
        string += QString("%1  %2").arg(m_resource->label(50, 69, language)).arg(m_resource->label(50, 71, language));
        break;
    case 2:
    {
        auto axisString = getAxisString(language);
        string += QString("%1%2 %3").arg(axisString).arg(data->d4() + 1).arg(m_resource->label(50, 72, language));
    }
        break;
    case 3:
        string += QString("%1  %2").arg(m_resource->label(50, 73, language)).arg(m_resource->label(50, 71, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringSetProductionManagement(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 74, language)).arg(data->d7()).arg(data->d6());
        break;
    case 1:
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 75, language)).arg(data->d7()).arg(data->d6());
        break;
    case 2:
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 76, language)).arg(data->d7()).arg(data->d6());
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringSetAccSetting(QString &string, OperationHistoryData *data, int language)
{
    if (data->d3() != 0) {
        return;
    }
    int beforeValue = 0;
    int afterValue  = 0;
    if (data->d4() == 8) {
        beforeValue = data->d7();
        afterValue  = data->d6();
    } else {
        beforeValue = data->d7() + 1;
        afterValue  = data->d6() + 1;
    }
    string += QString("%1 = %2→%3").arg(m_resource->label(50, 77, language)).arg(beforeValue).arg(afterValue);
}

void OperationHistoryString::toStringOperation(QString &string, OperationHistoryData *data, int language)
{
    int item = data->d2();
    if (TO_STRING_OPERATION_FUNC_LIST.contains(item)) {
        auto func = TO_STRING_OPERATION_FUNC_LIST.value(item);
        (this->*func)(string, data, language);
    }
}

void OperationHistoryString::toStringOperationPowerOff(QString &string, OperationHistoryData *data, int language)
{
    if (data->d3() != 0) {
        return;
    }
    string += QString("%1").arg(m_resource->label(50, 78, language));
}

void OperationHistoryString::toStringOperationPowerOn(QString &string, OperationHistoryData *data, int language)
{
    if (data->d3() != 0) {
        return;
    }
    string += QString("%1").arg(m_resource->label(50, 79, language));
}

void OperationHistoryString::toStringOperationManual(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 80, language)).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    case 1:
        string += QString("%1=%2→%3").arg(m_resource->label(50, 81, language)).arg(getAxisOperationTypeName(data->d7(), language)).arg(getAxisOperationTypeName(data->d6(), language));
        break;
    case 2:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1  %2%3 %4→%5").arg(m_resource->label(50, 82, language)).arg(getAxisString(language)).arg(data->d4() + 1).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    case 3:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 83, language)).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    case 4:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1%2 %3 %4→%5").arg(getAxisString(language)).arg(data->d4() + 1).arg(m_resource->label(41, 9, language)).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringOperationMonitor(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1  %2 = %3→%4").arg(m_resource->label(50, 84, language)).arg(getIoSymbol(data->d4())).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    case 1:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1  %2 = %3→%4").arg(m_resource->label(50, 85, language)).arg(getIoSymbol(data->d4())).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    case 2:
        string += QString("%1  %2%3").arg(m_resource->label(50, 86, language)).arg(getIoSymbol(data->d4())).arg(m_resource->label(50, 178, language));
        break;
    case 3:
        string += QString("%1  %2→%3").arg(m_resource->label(50, 87, language)).arg(getIoSymbol(data->d7())).arg(getIoSymbol(data->d6()));
        break;
    case 4:
        string += QString("%1  %2").arg(m_resource->label(50, 63, language)).arg(m_resource->label(50, 64, language));
        break;
    case 5:
        string += QString("%1  %2").arg(m_resource->label(50, 63, language)).arg(m_resource->label(50, 65, language));
        break;
    case 6:
        string += QString("%1  %2").arg(m_resource->label(50, 63, language)).arg(m_resource->label(50, 66, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringOperationHistory(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1").arg(m_resource->label(50, 88, language));
        break;
    case 1:
        string += QString("%1").arg(m_resource->label(50, 89, language));
        break;
    case 2:
        string += QString("%1").arg(m_resource->label(50, 90, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringDisplay(QString &string, OperationHistoryData *data, int language)
{
    int item = data->d2();
    if (TO_STRING_DISPLAY_FUNC_LIST.contains(item)) {
        auto func = TO_STRING_DISPLAY_FUNC_LIST.value(item);
        (this->*func)(string, data, language);
    }
}

void OperationHistoryString::toStringDisplayMovementPossible(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 91, language)).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringDisplayEmergencyStop(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
    {
        QString beforeOnOff = data->d7() == 0 ? "OFF" : "ON";
        QString afterOnOff = data->d6() == 0 ? "OFF" : "ON";
        string += QString("%1 = %2→%3").arg(m_resource->label(50, 92, language)).arg(beforeOnOff).arg(afterOnOff);
    }
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringDisplaySystemAlarm(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2").arg(m_resource->label(50, 93, language)).arg(getSystemAlarmName(data->d4() - 1, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringDisplayAxisAlarm(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2").arg(m_resource->label(50, 94, language)).arg(getAxisAlarmName(data->d4() - 1, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringDisplayNormalAlarm(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2").arg(m_resource->label(50, 95, language)).arg(getNormalAlarmName(data->d4() - 1, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringDisplayOperation(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2").arg(m_resource->label(50, 96, language)).arg(getOpeMsg(data->d4(), language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringKeyCode(QString &string, OperationHistoryData *data, int language)
{
    int item = data->d2();
    if (TO_STRING_KEY_CODE_FUNC_LIST.contains(item)) {
        auto func = TO_STRING_KEY_CODE_FUNC_LIST.value(item);
        (this->*func)(string, data, language);
    }
}

void OperationHistoryString::toStringKeyCodeMemblemSw(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 201: string += m_resource->label(50, 97, language); break;
    case 202: string += m_resource->label(50, 98, language); break;
    case 203: string += m_resource->label(50, 99, language); break;
    case 204: string += m_resource->label(50, 100, language); break;
    case 205: string += m_resource->label(50, 101, language); break;
    case 206: string += m_resource->label(50, 102, language); break;
    case 207: string += m_resource->label(50, 103, language); break;
    case 208: string += m_resource->label(50, 104, language); break;
    case 209: string += m_resource->label(50, 105, language); break;
    case 210: string += m_resource->label(50, 106, language); break;
    case 211: string += m_resource->label(50, 107, language); break;
    case 212: string += m_resource->label(50, 108, language); break;
    case 213: string += m_resource->label(50, 109, language); break;
    case 214: string += m_resource->label(50, 110, language); break;
    case 215: string += m_resource->label(50, 111, language); break;
    case 216: string += m_resource->label(50, 112, language); break;
    case 217: string += m_resource->label(50, 113, language); break;
    case 218: string += m_resource->label(50, 114, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringKeyCodeOrigin(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 242: string += QString("%1  %2").arg(m_resource->label(50, 115, language)).arg(m_resource->label(50, 118, language)); break;
    case 243: string += QString("%1  %2").arg(m_resource->label(50, 115, language)).arg(m_resource->label(50, 119, language)); break;
    default: break;
    }
}

void OperationHistoryString::toStringKeyCodeAuto(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 240: string += m_resource->label(50, 117, language); break;
    case 241: string += m_resource->label(50, 116, language); break;
    case 242: string += m_resource->label(50, 118, language); break;
    case 243: string += m_resource->label(50, 119, language); break;
    case 244: string += m_resource->label(50, 120, language); break;
    case 245: string += m_resource->label(50, 121, language); break;
    case 246: string += m_resource->label(50, 122, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringKeyCodeStandardStack(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 73:
        string += QString("%1%2 %3").arg(m_resource->label(50, 145, language)).arg(data->d4() + 1).arg(m_resource->label(37, 15, language));
        break;
    case 74:
        string += QString("%1  %2").arg(m_resource->label(50, 145, language)).arg(m_resource->label(37, 16, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringKeyCodeFreeStack(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 73:
        string += QString("%1%2 %3").arg(m_resource->label(50, 146, language)).arg(data->d4() + 1).arg(m_resource->label(37, 15, language));
        break;
    case 74:
        string += QString("%1  %2").arg(m_resource->label(50, 146, language)).arg(m_resource->label(37, 16, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringKeyCodeCounter(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
    {
        int d4 = data->d4();
        int no = 0;
        if ((d4 >= 65) && (d4 <= 72)) {
            no = d4 - 65;
            string += QString("%1  %2").arg(getCounterName(no)).arg(m_resource->label(39, 2, language));
        }
    }
        break;
    default:
        break;
    }

}

void OperationHistoryString::toStringKeyCodeAbs(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 0:
        string += QString("%1  %2%3").arg(m_resource->label(50, 123, language)).arg(getAxisString(language)).arg(data->d4() + 1);
        break;
    case 1:
        string += QString("%1  %2%3").arg(m_resource->label(50, 124, language)).arg(getAxisString(language)).arg(data->d4() + 1);
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringKeyCodeHistory(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 247:
        string += QString("%1  %2").arg(m_resource->label(50, 125, language)).arg(m_resource->label(50, 8, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringKeyCodeReturn(QString &string, OperationHistoryData *data, int language)
{
    switch (data->d3()) {
    case 247:
        string += QString("%1  %2").arg(m_resource->label(50, 125, language)).arg(m_resource->label(50, 8, language));
        break;
    default:
        break;
    }
}

void OperationHistoryString::toStringViewSwitching(QString &string, OperationHistoryData *data, int language)
{
    int item = data->d2();
    if (TO_STRING_VIEW_SWITCHING_FUNC_LIST.contains(item)) {
        string += m_resource->label(50, 126, language);
        string += " ";
        auto func = TO_STRING_VIEW_SWITCHING_FUNC_LIST.value(item);
        (this->*func)(string, data, language);
    }
}

void OperationHistoryString::toStringViewSwitchingOrigin(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 5) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 127, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingManual(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 6) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 128, language); break;
    case 1: string += m_resource->label(50, 129, language); break;
    case 2: string += m_resource->label(50, 130, language); break;
    case 3: string += m_resource->label(50, 131, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingAuto(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 7) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 11: string += m_resource->label(50, 132, language); break;
    case 12: string += m_resource->label(50, 133, language); break;
    case 21: string += m_resource->label(50, 134, language); break;
    case 22: string += m_resource->label(50, 135, language); break;
    case 31: string += m_resource->label(50, 136, language); break;
    case 32: string += m_resource->label(50, 137, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingMonitor(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 8) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 138, language); break;
    case 1: string += m_resource->label(50, 139, language); break;
    case 2: string += m_resource->label(50, 140, language); break;
    case 3: string += m_resource->label(50, 141, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingTeachingMode(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 10) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 142, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingTeachingPos(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 11) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 143, language); break;
    case 1: string += m_resource->label(50, 144, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingTeachingStack(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 12) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 145, language); break;
    case 1: string += m_resource->label(50, 146, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingTeachingTimer(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 13) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 147, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingTeachingCounter(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 14) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 148, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingTeachingPass(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 15) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 149, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingTeachingProgram(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 16) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 150, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFuncController(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 17) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 151, language); break;
    case 1: string += m_resource->label(50, 152, language); break;
    case 2: string += m_resource->label(50, 153, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFuncMachineParam(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 18) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 154, language); break;
    case 1: string += m_resource->label(50, 155, language); break;
    case 2: string += m_resource->label(50, 156, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFuncAreaSet(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 19) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 157, language); break;
    case 1: string += m_resource->label(50, 158, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFuncMaintain(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 20) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 159, language); break;
    case 1: string += m_resource->label(50, 160, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFuncProductionMng(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 21) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 161, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFuncAbsSet(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 22) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 162, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFuncDeceleration(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 23) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 163, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFuncVersionMng(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 24) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 164, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingHistory(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 25) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 165, language); break;
    case 1: string += m_resource->label(50, 166, language); break;
    case 2: string += m_resource->label(50, 167, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingFile(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 26) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 168, language); break;
    case 1: string += m_resource->label(50, 169, language); break;
    case 2: string += m_resource->label(50, 170, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingUser(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 28) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 171, language); break;
    case 1: string += m_resource->label(50, 172, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingSystemTime(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 29) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 173, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingSpeed(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 30) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 174, language); break;
    case 1: string += m_resource->label(50, 175, language); break;
    case 2: string += m_resource->label(50, 176, language); break;
    default: break;
    }
}

void OperationHistoryString::toStringViewSwitchingLanguage(QString &string, OperationHistoryData *data, int language)
{
    int dp1 = data->d3();
    if (dp1 != 31) {
        return;
    }

    int dp2 = data->d4();
    switch (dp2) {
    case 0: string += m_resource->label(50, 177, language); break;
    default: break;
    }
}

QString OperationHistoryString::getPointName(int pointNo)
{
    int labelIndex = 0;
    if ((pointNo >= 1) && (pointNo <= 80)) {
        labelIndex = 29 + pointNo;
    } else if ((pointNo >= 201) && (pointNo <= 208)) {
        labelIndex = 110 + pointNo - 201;
    } else {
        return "";
    }
    return m_resource->label(36, labelIndex);
}

QString OperationHistoryString::getAxisString(int language)
{
    return m_resource->label(37, 20, language);
}

QString OperationHistoryString::getIoSymbol(int diAddress)
{
    return m_resource->symbolAt(diAddress);
}

QString OperationHistoryString::getSystemProgChoiceName(int progIndex, int language)
{
    QString ret("");
    switch (progIndex) {
    case 0:
        ret = m_resource->label(42, 77, language);
        break;
    case 1:
        ret = m_resource->label(42, 78, language);
        break;
    default:
        break;
    }
    return ret;
}

QString OperationHistoryString::getFetchingMachineTypeName(int type, int language)
{
    QString ret("");
    switch (type) {
    case 0:
        ret = m_resource->label(42, 95, language);
        break;
    case 1:
        ret = m_resource->label(42, 97, language);
        break;
    case 2:
        ret = m_resource->label(42, 96, language);
        break;
    default:
        break;
    }
    return ret;
}

QString OperationHistoryString::getSystemModeName(int mode, int language)
{
    QString ret("");
    if ((mode >= 0) && (mode <= 15)) {
        int labelIndex = 79 + mode;
        ret = m_resource->label(42, labelIndex, language);
    }
    return ret;
}

QString OperationHistoryString::getMachineParameterName(int index, int language)
{
    QString ret("");
    if ((index >= 0) && (index <= 11)) {
        int labelIndex = 30 + index;
        ret = m_resource->label(43, labelIndex, language);
    }
    return ret;
}



QString OperationHistoryString::getMachineParameterValue(int index, int value)
{
    return m_resource->formatData(value, getMachineParameterFormat(index), getMachineParameterFormatShift(index), false);
}

int OperationHistoryString::getMachineParameterFormatShift(int index)
{
    int ret = 0;
    switch (index) {
    case 0:     // 1回転距離
    case 3:     // 手前完了距離
    case 4:     // 原点補正
    case 9:     // 加減速時間[最小]
    case 10:    // 加減速時間[最大]
        ret = 2;
        break;
    case 1:     // 1回転パルス数
    case 2:     // 最大回転数
    case 5:     // 原点復帰前速度
    case 6:     // 自由操作速度[低]
    case 7:     // 自由操作速度[中]
    case 8:     // 自由操作速度[高]
    case 11:    // 給油距離
        ret = 0;
        break;
    }
    return ret;
}

int OperationHistoryString::getMachineParameterFormat(int index)
{
    return m_resource->getPenSetIntValue("18", 41 + index, 3);
}

QString OperationHistoryString::getDriverParameterName(int index, int language)
{
    QString ret("");
    if ((index >= 0) && (index <= 76)) {
        int labelIndex = 47 + index * 2;
        ret = m_resource->label(43, labelIndex, language);
    }
    return ret;
}

QString OperationHistoryString::getAxisOperationTypeName(int type, int language)
{
    QString ret("");
    switch (type) {
    case 0:
        ret = m_resource->label(31, 25, language);
        break;
    case 1:
        ret = m_resource->label(31, 26, language);
        break;
    default:
        break;
    }
    return ret;
}

QString OperationHistoryString::getSystemAlarmName(int index, int language)
{
    int alarmNo = 510 + index * 2 + 1;
    return m_resource->getMessageStringValue("12", alarmNo, language);
}

QString OperationHistoryString::getAxisAlarmName(int index, int language)
{
    int alarmNo = 1040 + index * 2 + 1;
    return m_resource->getMessageStringValue("12", alarmNo, language);
}

QString OperationHistoryString::getNormalAlarmName(int index, int language)
{
    int alarmNo = index * 2 + 1;
    return m_resource->getMessageStringValue("12", alarmNo, language);
}

QString OperationHistoryString::getStackOrderName(int stackIndex, int value)
{
    if (stackIndex >= STACK_ORDER_VALUE_LIST.length()) {
        return "";
    }

    QString ret = "";
    QList<int> list = STACK_ORDER_VALUE_LIST.at(stackIndex);
    for (int i = 0; i < list.length(); i++) {
        if (value == list.at(i)) {
            ret = STACK_ORDER_STRING_LIST.at(i);
            break;
        }
    }
    return ret;
}

QString OperationHistoryString::getSimpleSetName(int type, int language)
{
    QString ret("");
    switch (type)
    {
    case 14:    // チューニングモード
        ret = m_resource->label(43, 207, language);
        break;
    case 16:    // 自動調整ゲイン１
        ret = m_resource->label(43, 214, language);
        break;
    case 17:    // 自動調整ゲイン２
        ret = m_resource->label(43, 215, language);
        break;
    case 15:    // 慣性モーメント比
        ret = m_resource->label(43, 216, language);
        break;
    case 71:    // フィルター検出起動
        ret = m_resource->label(43, 220, language);
        break;
    case 72:    // 共振周波数 フィルター1
        ret = QString("%1  %21").arg(m_resource->label(43, 223, language)).arg(m_resource->label(43, 226, language));
        break;
    case 75:    // 共振周波数 フィルター2
        ret = QString("%1  %22").arg(m_resource->label(43, 223, language)).arg(m_resource->label(43, 226, language));
        break;
    case 73:    // 減衰量 フィルター1
        ret = QString("%1  %21").arg(m_resource->label(43, 224, language)).arg(m_resource->label(43, 226, language));
        break;
    case 76:    // 減衰量 フィルター2
        ret = QString("%1  %22").arg(m_resource->label(43, 224, language)).arg(m_resource->label(43, 226, language));
        break;
    case 78:    // 自動調整
        ret = m_resource->label(43, 230, language);
        break;
    case 79:    // 制振制御設定0
        ret = m_resource->label(43, 229, language) = QString("0");
        break;
    case 81:    // 制振制御設定1
        ret = m_resource->label(43, 229, language) = QString("1");
        break;
    case 87:    // 自動検出レベル
        ret = m_resource->label(43, 231, language);
        break;
    case 0:    // 最大トルク変化量
        ret = m_resource->label(43, 276, language);
        break;
    case 198:  // 衝突検知閾値[%]
        ret = m_resource->label(43, 277, language);
        break;
    default:
        break;
    }
    return ret;
}

QString OperationHistoryString::getAreaSetName(int type, int language)
{
    QString ret("");
    switch (type)
    {
    case 0: ret = m_resource->label(44, 7, language); break;
    case 1: ret = m_resource->label(44, 9, language); break;
    case 2: ret = m_resource->label(44, 8, language); break;
    case 3: ret = m_resource->label(44, 10, language); break;
    case 4: ret = m_resource->label(44, 11, language); break;
    case 5: ret = m_resource->label(44, 12, language); break;
    default: break;
    }
    return ret;
}

QString OperationHistoryString::getOpeMsg(int index, int language)
{
    QString ret = m_resource->getMessageStringValue("14", index, language);
    QStringList list = ret.split("\n");
    return list.length() >= 1 ? list.at(0) : QString("");
}

QString OperationHistoryString::getModelName(int index)
{
    QString ret("");
    auto modelList = m_modelSelectViewModel->modelList();
    if (index >= modelList.length()) {
        return ret;
    }

    return modelList.at(index);
}

QString OperationHistoryString::getCounterName(int index)
{
    QString ret("");
    if ((index >= 0) && (index <= 15)) {
        ret = m_resource->label(39, 5 + index);
    }
    return ret;
}
