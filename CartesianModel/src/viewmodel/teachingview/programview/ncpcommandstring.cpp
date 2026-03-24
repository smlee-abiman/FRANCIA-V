#include "ncpcommandstring.h"
#include "../teachingprogrampagemodel.h"
#include "command/ncpcommandgosub.h"
#include "command/ncpcommandmove.h"
#include "command/ncpcommandline.h"
#include "command/ncpcommandarc.h"
#include "command/ncpcommandpath.h"
#include "command/ncpcommandout.h"
#include "programmenu/programmenu2_1.h"
#include "programmenu/programmenu2_3.h"
#include "axispointinfo.h"

typedef QString (NCPCommandString::*TO_STRING_COMMAND_FUNC)(NCPStep *, int);

const QHash<int, TO_STRING_COMMAND_FUNC> TO_STRING_COMMAND_FUNC_LIST = {
    { NCPCommand::GoSub,    &NCPCommandString::toStringGoSub    },
    { NCPCommand::Move,     &NCPCommandString::toStringMove     },
    { NCPCommand::Movep,    &NCPCommandString::toStringMove     },
    { NCPCommand::Line,     &NCPCommandString::toStringLine     },
    { NCPCommand::Linec,    &NCPCommandString::toStringLine     },
    { NCPCommand::Arc,      &NCPCommandString::toStringArc      },
    { NCPCommand::Arcc,     &NCPCommandString::toStringArc      },
    { NCPCommand::Wait,     &NCPCommandString::toStringWait     },
    { NCPCommand::Path,     &NCPCommandString::toStringPath     },
};

NCPCommandString::NCPCommandString(TeachingProgramPageModel *programPageModel, ResourceManager *resource, QObject *parent)
    : QObject(parent),
      m_programPageModel(programPageModel),
      m_resource(resource)
{
    m_axisPointInfo = AxisPointInfo::get_instance();
}

QString NCPCommandString::toTranslatedString(NCPStep *step, int language)
{
    if (step == nullptr) {
        return "";
    }

    auto command = step->command();
    if(command == nullptr) {
        return "";
    }

    QString ret;
    auto commandType = command->type();
    if (TO_STRING_COMMAND_FUNC_LIST.contains(commandType)) {
        auto func = TO_STRING_COMMAND_FUNC_LIST.value(commandType);
        ret = (this->*func)(step, language);
    } else {
        ret = step->toTranslatedString(language);
    }
    return ret;
}

QString NCPCommandString::toStringGoSub(NCPStep *step, int language)
{
    QString ret("");
    auto command = static_cast<NCPCommandGoSub *>(step->command());
    if (command->isJigProgram()) {
        ret = toStringJig(step);
    } else if (command->isStandardPalletizeProgram()) {
        int stackIndex = command->programNo() - command->startStandardPalletizeProgramNo();
        ret = toStringStandardStack(stackIndex);
    } else if (command->isFreePalletizeProgram()) {
        int stackIndex = command->programNo() - command->startFreePalletizeProgramNo();
        ret = toStringFreeStack(stackIndex);
    } else {
        ret = step->toTranslatedString(language);
    }
    return ret;
}

QString NCPCommandString::toStringMove(NCPStep *step, int language)
{
    QString ret("");
    auto command = dynamic_cast<NCPCommandMove *>(step->command());
    auto pointNo = command->pointNo();
    ret = getPointNameForMove(pointNo);
    if (command->type() == NCPCommand::Type::Movep) {
        ret += m_resource->label(41, 490, language) + " ";
    }
    int positionFormat = m_axisPointInfo->formatArbitraryPointPosition();
    int speedFormat = m_axisPointInfo->formatArbitraryPointSpeed();
    for (int axisIndex : command->axisList()) {
        auto positionHrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, axisIndex);
        auto speedHrAddress = m_axisPointInfo->getPointSpeedAddress(pointNo, axisIndex);
        ret += QString("%1(%2mm,%3),").arg(getAxisName(axisIndex))
                                        .arg(m_resource->formatData(m_modbus->getLongReceiveDataHR(positionHrAddress), positionFormat, 2, true))
                                        .arg(m_resource->formatData(m_modbus->getReceiveDataHR(speedHrAddress), speedFormat));
    }
    ret.chop(1);
    ret += QString(" P%1").arg(pointNo, 3, 10, QChar('0'));
    return ret;
}

QString NCPCommandString::toStringLine(NCPStep *step, int language)
{
    Q_UNUSED(language)
    auto ret = QString("");
    auto command = dynamic_cast<NCPCommandLine *>(step->command());
    if (command->type() == NCPCommand::Type::Linec) {
        ret += m_resource->label(37, 521) + " ";
    }
    ret += getLineInfoString(command, command->pointNo());
    return ret;
}

QString NCPCommandString::toStringArc(NCPStep *step, int language)
{
    auto ret = QString("");
    auto command = dynamic_cast<NCPCommandArc *>(step->command());
    if (command->type() == NCPCommand::Type::Arcc) {
        ret += m_resource->label(37, 521) + " ";
    }
    ret += QString("%2 %3 → %4 %5").arg(m_resource->label(41, 523, language)).arg(getLineInfoString(command, command->pointNo()))
                                      .arg(m_resource->label(41, 524, language)).arg(getLineInfoString(command, command->endPointNo()));
    return ret;
}

QString NCPCommandString::toStringWait(NCPStep *step, int language)
{
    return step->toTranslatedString(language);
}

QString NCPCommandString::toStringPath(NCPStep *step, int language)
{
    Q_UNUSED(language)
    auto command = dynamic_cast<NCPCommandPath *>(step->command());
    auto ret = QString("");
    ret.append(command->on() ? "ON" : "OFF");
    if (command->on()) {
        ret.append(" ");
        int positionFormat = m_axisPointInfo->formatArbitraryPointPosition();
        for (auto data : command->positionList()) {
            ret += QString("%1(%2mm),").arg(getAxisName(data.axis())).arg(m_resource->formatData(data.position(), positionFormat, 2, true));
        }
        ret.chop(1);
    }
    return ret;
}

QString NCPCommandString::toStringJig(NCPStep *step)
{
    QString ret("");
    auto command = static_cast<NCPCommandGoSub *>(step->command());
    int programNo = command->programNo();

    auto program = m_programPageModel->ncpFile()->getProgram(programNo);
    if (program == nullptr) {
        return ret;
    }

    auto numSteps = program->numSteps();
    for (int i = 0; i < numSteps; i++) {
        auto step = program->step(i);
        if (step == nullptr)
            continue;
        auto command = step->command();
        if (command->type() == NCPCommand::Type::Out) {
            auto outCommand = dynamic_cast<NCPCommandOut *>(command);
            if (outCommand->isJigType()) {
                ret += outCommand->toJigOpenCloseString();
                ret += outCommand->toJigString();
                break;
            }
        }
    }
    return ret;
}


QString NCPCommandString::toStringStandardStack(int stackIndex)
{
    int palletizeNo = stackIndex + 1;
    int pointNo = m_axisPointInfo->getStandardPalletizePointNo(palletizeNo);
    auto menu = static_cast<ProgramMenu2_3 *>(m_programPageModel->progMenu2()->getProgMenu(2));
    int type = menu->getCurrentStackType(stackIndex);
    QString ret = getStackPointInfoString(type, pointNo);
    return ret;
}

QString NCPCommandString::toStringFreeStack(int stackIndex)
{
    int palletizeNo = stackIndex + 1;
    int pointNo = m_axisPointInfo->getFreePalletizePointNo(palletizeNo);
    QString ret = getStackPointInfoString(0, pointNo);
    return ret;
}

QString NCPCommandString::getPointNameForMove(int pointNo)
{
    QString ret("");
    switch(m_axisPointInfo->getPointType(pointNo)) {
    case AxisPointInfo::PointType::Fix:
    {
        QString pointName = m_axisPointInfo->getPointName(pointNo);
        if (!pointName.isEmpty())
            ret += QString("[%1]").arg(pointName);
    }
        break;
    case AxisPointInfo::PointType::FreePalletize:
        ret = QString("%2%3 ").arg(m_resource->label(37, 4)).arg(m_axisPointInfo->getPalletizeNo(pointNo));
        break;
    case AxisPointInfo::PointType::StandardPalletize:
        ret = QString("%2%3 ").arg(m_resource->label(37, 3)).arg(m_axisPointInfo->getPalletizeNo(pointNo));
        break;
    default:
        break;
    }
    return ret;
}

QString NCPCommandString::getStackPointInfoString(int type, int pointNo)
{
    int positionFormat = m_axisPointInfo->formatArbitraryPointPosition();
    int speedFormat = m_axisPointInfo->formatArbitraryPointSpeed();
    QString ret("");
    QList<int> axisList = (type == 0) ? QList<int>({0, 1, 3}) : (type == 1) ? QList<int>({0, 2, 4}) : QList<int>({0, 1, 3, 2, 4});

    for (int axis : axisList) {
        auto distance = 0;
        auto positionHrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, axis);
        auto position = m_modbus->getLongReceiveDataHR(positionHrAddress);
        if(axis == 3 ){
            distance = m_modbus->getLongReceiveDataHR(7609);
            if(m_resource->formatData(position, positionFormat, 2, true) == "****.**" ){
                position = distance;
            }else if(m_resource->formatData(position, positionFormat, 2, true) != "****.**"){
                position += distance;
            }
        }else if(axis == 4){
            distance = m_modbus->getLongReceiveDataHR(7612);
            if(m_resource->formatData(position, positionFormat, 2, true) == "****.**" ){
                position = distance;
            }else if(m_resource->formatData(position, positionFormat, 2, true) != "****.**"){
                position += distance;
            }
        }
        auto speedHrAddress = m_axisPointInfo->getPointSpeedAddress(pointNo, axis);
        auto speed = m_modbus->getReceiveDataHR(speedHrAddress);
        ret += QString("%1(%2mm,%3) ").arg(getAxisName(axis))
                .arg(m_resource->formatData(position, positionFormat, 2, true)).arg(m_resource->formatData(speed, speedFormat));
    }
    ret.chop(1);
    return ret;
}

QString NCPCommandString::getLineInfoString(NCPCommandLine *command, int pointNo)
{
    QString ret("");
    int positionFormat = m_axisPointInfo->formatArbitraryPointPosition();
    for (int axisIndex : command->axisList()) {
        auto positionHrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, axisIndex);
        ret += QString("%1(%2mm),").arg(getAxisName(axisIndex))
                                        .arg(m_resource->formatData(m_modbus->getLongReceiveDataHR(positionHrAddress), positionFormat, 2, true));
    }
    auto speedHrAddress = m_axisPointInfo->getPointSpeedAddress(pointNo, 0);
    int speedFormat = m_axisPointInfo->formatArbitraryPointSpeed();
    ret += QString("%1(%2)").arg(m_resource->label(41, 488)).arg(m_resource->formatData(m_modbus->getReceiveDataHR(speedHrAddress), speedFormat));
    ret += QString(" P%1").arg(pointNo, 3, 10, QChar('0'));
    return ret;
}

QString NCPCommandString::getAxisName(int axisIndex)
{
    return m_resource->label(22, axisIndex);
}

