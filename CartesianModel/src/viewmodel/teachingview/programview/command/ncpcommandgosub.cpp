#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include "ncpcommandgosub.h"
#include "src/axispointinfo.h"

NCPCommandGoSub::NCPCommandGoSub(QObject *parent)
    : NCPCommand(parent)
{

}

NCPCommandGoSub::NCPCommandGoSub(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}

NCPCommandGoSub::NCPCommandGoSub(ResourceManager *resource, int programNo, QObject *parent)
    : NCPCommand(resource, parent)
{
    setProgramNo(programNo);
}

//------------------------------------------------------
void NCPCommandGoSub::setProgramNo(int no)
{
    const int minProgramNo = 1;
    const int maxProgramNo = 255;
    if ((no >= minProgramNo) && (no <= maxProgramNo)) {
        m_programNo = no;
    }
}

int NCPCommandGoSub::wizardMenuIndex() const
{
    int ret;
    switch (m_programNo) {
    case 102:
    case 103:
    case 109:
    case 121:
    case 122:
    case 123:
    case 124:
    case 147:
    case 149:
    case 150:
    case 151:
    case 152:
    case 153:
        ret = WizardMenuIndex::Menu1;
        break;
    default:
        if (isJigProgram()) {
            ret = WizardMenuIndex::Menu1;
        } else if (isStandardPalletizeProgram() || isFreePalletizeProgram()) {
            ret = WizardMenuIndex::Menu2;
        } else {
            ret = WizardMenuIndex::Menu4;
        }
        break;
    }
    return ret;
}


bool NCPCommandGoSub::parse(const QString& commandLine)
{
    // qDebug() << "parse " << commandLine;
    QRegularExpression commandRe("(\\s+|\\t+)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }

    // parse program number.
    QRegularExpression programNumberRe("#(\\d{3})");
    auto match = programNumberRe.match(splitList.at(1));
    if (!match.hasMatch()) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }
    auto ok = false;
    auto value = match.captured(1).toInt(&ok);
    if (!ok) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }
    setProgramNo(value);
    return true;
}

int NCPCommandGoSub::pointNo() const
{
    int ret = -1;
    if ((m_programNo >= PROGRAM_STANDARD_PALLETIZE_START) &&
        (m_programNo <= PROGRAM_STANDARD_PALLETIZE_END)) {
        ret = AxisPointInfo::getStartStandardPalletizePointNo() + m_programNo - PROGRAM_STANDARD_PALLETIZE_START;
    } else if ((m_programNo >= PROGRAM_FREE_PALLETIZE_START) &&
            (m_programNo <= PROGRAM_FREE_PALLETIZE_END)) {
        ret = AxisPointInfo::getStartFreePalletizePointNo() + m_programNo - PROGRAM_FREE_PALLETIZE_START;
    }
    return ret;
}

QString NCPCommandGoSub::nameString() const
{
    const int group = 41;
    auto headerLabelIndex = 536;
    QString ret = "";
    if (isJigProgram()) {
        ret = m_resource->label(41, 15);
    } else if (isStandardPalletizeProgram()) {
        int palletizeNo = m_programNo - startStandardPalletizeProgramNo() + 1;
        ret = QString("%1%2").arg(m_resource->label(37, 3)).arg(palletizeNo);
    } else if (isFreePalletizeProgram()) {
        int palletizeNo = m_programNo - startFreePalletizeProgramNo() + 1;
        ret = QString("%1%2").arg(m_resource->label(37, 4)).arg(palletizeNo);
    } else {
        switch (m_programNo) {
        case 102:
        case 103:
        case 109:
           headerLabelIndex = 13;
           break;
        case 141:
        case 142:
            headerLabelIndex = 351;
            break;
        case 121:
        case 122:
        case 123:
        case 124:
        case 147:
        case 149:
        case 150:
        case 151:
        case 152:
        case 153:
           headerLabelIndex = 14;
           break;
       }
       ret = m_resource->label(group, headerLabelIndex);
    }
    return ret;
}

QString NCPCommandGoSub::toString() const
{
    const int group = 41;
    int programLabelIndex = m_programNo + 36;
    QString ret = "";
    switch (m_programNo) {
    case 141:
        ret = m_resource->label(group, 609);
        break;
    case 142:
        ret = m_resource->label(group, 610);
        break;
    case 102:
    case 103:
    case 109:
    case 121:
    case 122:
    case 123:
    case 124:
    case 147:
    case 149:
    case 150:
    case 151:
    case 152:
    case 153:
       ret = QString("%1#%2").arg(m_resource->label(group, programLabelIndex))
                             .arg(m_programNo);
       break;
    default:
       ret = QString("#%1%2").arg(m_programNo, 3, 10, QChar('0'))
                             .arg(m_resource->label(group, programLabelIndex));
       break;
    }
    return ret;
}

QString NCPCommandGoSub::toCode() const
{
    return QString("GOSUB\t#%1").arg(m_programNo, 3, 10, QChar('0'));
}

NCPCommand* NCPCommandGoSub::clone() const
{
    auto selfClone = new NCPCommandGoSub(m_resource);
    if (selfClone != nullptr)
        selfClone->m_programNo = m_programNo;
    return selfClone;
}
