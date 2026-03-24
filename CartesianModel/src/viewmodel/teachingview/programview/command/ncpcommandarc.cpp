#include <QRegularExpression>
#include "ncpcommandarc.h"

//------------------------------------------------------
// NCPCommandArc.
//------------------------------------------------------
NCPCommandArc::NCPCommandArc(QObject *parent)
    : NCPCommandLine(parent)
{
}
//------------------------------------------------------
NCPCommandArc::NCPCommandArc(ResourceManager *resource, QObject *parent)
    : NCPCommandLine(resource, parent)
{
}

void NCPCommandArc::setEndPointNo(int no)
{
    m_endPointNo = no;
}

bool NCPCommandArc::parse(const QString &commandLine)
{
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 4) {
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
    auto arg = splitList.at(splitList.length() - 2);
    auto pointNoMatch = pointNoRe.match(arg);
    if (pointNoMatch.hasMatch()) {
        m_pointNo = arg.mid(1, 3).toInt();
    }

    arg = splitList.at(splitList.length() - 1);
    pointNoMatch = pointNoRe.match(arg);
    if (pointNoMatch.hasMatch()) {
        m_endPointNo = arg.mid(1, 3).toInt();
    }
    return true;
}

QString NCPCommandArc::nameString() const
{
    return m_resource->label(41, 515);
}

QString NCPCommandArc::toCode() const
{
    auto code = command() + "\t";
    for (int axis : m_axisList)
        code += QString("A%1,").arg(axis + 1);
    code.chop(1);
    code += (" ");
    code += QString("P%1 P%2").arg(m_pointNo, 3, 10, QChar('0')).arg(m_endPointNo, 3, 10, QChar('0'));
    return code;
}

NCPCommand* NCPCommandArc::clone() const
{
    auto selfClone = new NCPCommandArc(m_resource);
    if (selfClone != nullptr) {
        for (auto axis : m_axisList)
            selfClone->m_axisList.append(axis);
        selfClone->m_pointNo = m_pointNo;
        selfClone->m_endPointNo = m_endPointNo;
    }
    return selfClone;
}
