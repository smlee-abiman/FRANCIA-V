#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include "ncpcommandon.h"

NCPCommandOn::NCPCommandOn(QObject *parent)
    : NCPCommand(parent)
{

}

NCPCommandOn::NCPCommandOn(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{

}

NCPCommandOn::NCPCommandOn(ResourceManager *resource, int programNo, QObject *parent)
    : NCPCommand(resource, parent)
{
    setProgramNo(programNo);
}

//------------------------------------------------------
void NCPCommandOn::setProgramNo(int no)
{
    const int minProgramNo = 1;
    const int maxProgramNo = 255;
    if ((no >= minProgramNo) && (no <= maxProgramNo)) {
        m_programNo = no;
    }
}

int NCPCommandOn::wizardMenuIndex() const
{
    int ret;
    switch (m_programNo) {
    case 121:
    case 122:
    case 123:
    case 124:
    case 148:
        ret = WizardMenuIndex::Menu1;
        break;
    default:
        ret = WizardMenuIndex::Menu4;
        break;
    }
    return ret;
}


bool NCPCommandOn::parse(const QString& commandLine)
{
//    qDebug() << "parse " << commandLine;
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

QString NCPCommandOn::nameString() const
{
    const int group = 41;
    QString ret;
    switch (m_programNo) {
    case 121:
    case 122:
    case 123:
    case 124:
        ret = m_resource->label(group, 14);
        break;
    case 148:
        ret = m_resource->label(group, 13);
        break;
    default:
        ret = m_resource->label(group, 623);
        break;
    }
    return ret;
}

QString NCPCommandOn::toString() const
{
    const int group = 41;
    int programLabelIndex = m_programNo + 36;
    QString ret;
    switch (m_programNo) {
    case 121:
    case 122:
    case 123:
    case 124:
        ret = QString("%1 %2#%3").arg(m_resource->label(group, 303)).arg(m_resource->label(group, programLabelIndex)).arg(m_programNo);
        break;
    case 148:
        ret = QString("%1#%2").arg(m_resource->label(group, programLabelIndex)).arg(m_programNo);
        break;
    default:
        ret = QString("");
        auto programNoStr = QString("#%1").arg(programNo(), 3, 10, QChar('0'));
        ret.append(programNoStr);
        if (1 <= programNo() && programNo() <= 255) {
            auto programName = QString("(%1)").arg(m_resource->label(group, programLabelIndex));
            ret.append(programName);
        }
        break;
    }
    return ret;
}

QString NCPCommandOn::toCode() const
{
    return QString("ON\t#%1").arg(m_programNo, 3, 10, QChar('0'));
}

NCPCommand* NCPCommandOn::clone() const
{
    auto selfClone = new NCPCommandOn(m_resource);
    if (selfClone != nullptr)
        selfClone->m_programNo = m_programNo;
    return selfClone;
}
