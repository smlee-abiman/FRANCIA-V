#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "ncpcommandbreak.h"

//------------------------------------------------------
// NCPCommandBreak.
//------------------------------------------------------
NCPCommandBreak::NCPCommandBreak(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandBreak::NCPCommandBreak(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}

void NCPCommandBreak::appendAxis(int axis)
{
    m_axisList.append(axis);
}

//------------------------------------------------------
bool NCPCommandBreak::parse(const QString &commandLine)
{
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);

    QRegularExpression axisRe("^A[1-8]");
    for (auto arg: splitList) {
        auto match = axisRe.match(arg);
        if (match.hasMatch()) {
            int axis = arg.right(1).toInt() - 1;
            m_axisList.append(axis);
        }
    }
    return true;
}
//------------------------------------------------------
QString NCPCommandBreak::nameString() const
{
    return m_resource->label(41, 518);
}
//------------------------------------------------------
QString NCPCommandBreak::toString() const
{
    auto string = QString("");
    for (int axis : m_axisList) {
        string.append(m_resource->label(22, axis));
        string.append(",");
    }
    string.chop(1);
    return string;
}

//------------------------------------------------------
QString NCPCommandBreak::toCode() const
{
    auto code = QString("BREAK\t");
    for (int axis : m_axisList)
        code += QString("A%1,").arg(QString::number(axis + 1));
    code.chop(1);
    return code;
}

NCPCommand* NCPCommandBreak::clone() const
{
    auto selfClone = new NCPCommandBreak(m_resource);
    if (selfClone != nullptr) {
        for (auto axis : m_axisList)
            selfClone->m_axisList.append(axis);
    }
    return selfClone;
}
