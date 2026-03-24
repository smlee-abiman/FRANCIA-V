#include <QRegularExpression>
#include "ncpcommandline.h"

//------------------------------------------------------
// NCPCommandLine.
//------------------------------------------------------
NCPCommandLine::NCPCommandLine(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandLine::NCPCommandLine(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}

void NCPCommandLine::appendAxis(int axis)
{
    m_axisList.append(axis);
}

void NCPCommandLine::setPointNo(int no)
{
    m_pointNo = no;
}

//------------------------------------------------------
bool NCPCommandLine::parse(const QString &commandLine)
{
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 3) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }

    QRegularExpression axisRe("^A[1-8]");
    for (auto arg: splitList) {
        auto axisMatch = axisRe.match(arg);
        if (axisMatch.hasMatch()) {
            int axis = arg.right(1).toInt() - 1;
            m_axisList.append(axis);
        }
    }

    QRegularExpression pointNoRe("^P\\d\\d\\d");
    auto arg = splitList.last();
    auto pointNoMatch = pointNoRe.match(arg);
    if (pointNoMatch.hasMatch()) {
        m_pointNo = arg.mid(1, 3).toInt();
    }
    return true;
}
//------------------------------------------------------
QString NCPCommandLine::nameString() const
{
    return m_resource->label(41, 514);
}
//------------------------------------------------------
QString NCPCommandLine::toString() const
{
    auto axisString = m_resource->label(37, 20);
    auto string = QString("");
    if ((type() == NCPCommand::Type::Linec) ||
        (type() == NCPCommand::Type::Arcc)) {
        string += m_resource->label(37, 521) + " ";
    }
    for (int axis : m_axisList)
        string += QString("%1%2,").arg(axis + 1).arg(axisString);
    string.chop(1);
    return string;
}
//------------------------------------------------------
QString NCPCommandLine::toCode() const
{
    auto code = command() + "\t";
    for (int axis : m_axisList)
        code += QString("A%1,").arg(axis + 1);
    code.chop(1);
    code += (" ");
    code += QString("P%1").arg(m_pointNo, 3, 10, QChar('0'));
    return code;
}

NCPCommand* NCPCommandLine::clone() const
{
    auto selfClone = new NCPCommandLine(m_resource);
    if (selfClone != nullptr) {
        for (auto axis : m_axisList)
            selfClone->m_axisList.append(axis);
        selfClone->m_pointNo = m_pointNo;
    }
    return selfClone;
}
