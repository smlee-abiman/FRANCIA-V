#include <QRegularExpressionMatch>
#include "ncpcommandhome.h"

//------------------------------------------------------
// NCPCommandHome.
//------------------------------------------------------
NCPCommandHome::NCPCommandHome(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandHome::NCPCommandHome(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}

void NCPCommandHome::setOption(NCPCommandHome::Option option)
{
    m_option = option;
}

void NCPCommandHome::appendAxis(int axis)
{
    m_axisList.append(axis);
}

//------------------------------------------------------
bool NCPCommandHome::parse(const QString &commandLine)
{
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug() << "Syntax error commandLine:[" << commandLine << "]";
        return false;
    }

    setOption(splitList.at(1));

    int axis = 0;
    bool ok;
    QRegularExpression axisRe("^A[1-8]");
    for (auto arg: splitList) {
        auto axisMatch = axisRe.match(arg);
        if (axisMatch.hasMatch()) {
            axis = arg.right(1).toInt(&ok) - 1;
            if (ok)
                m_axisList.append(axis);
        }
    }
    return true;
}
//------------------------------------------------------
QString NCPCommandHome::nameString() const
{
    return m_resource->label(41, 517);
}
//------------------------------------------------------
QString NCPCommandHome::toString() const
{
    auto string = QString("");
    if (type() == NCPCommand::Type::Homep) {
        string += m_resource->label(41, 490) + " ";
    }

    if (m_option == NCPCommandHome::Option::ReturnPlusDirection) {
        string.append(m_resource->label(41, 528));
    } else if (m_option == NCPCommandHome::Option::ReturnMinusDirection) {
        string.append(m_resource->label(41, 529));
    }
    string.append(" ");
    for (int axis : m_axisList)
        string.append(QString("%1, ").arg(m_resource->label(22, axis)));
    if (m_axisList.length() >= 1)
        string.chop(2);
    return string;
}
//------------------------------------------------------
QString NCPCommandHome::toCode() const
{
    auto code = command() + "\t";
    if (m_option == NCPCommandHome::Option::ReturnPlusDirection) {
        code.append("P ");
    } else if(m_option == NCPCommandHome::Option::ReturnMinusDirection) {
        code.append("M ");
    }
    for (int axis : m_axisList) {
        code.append("A");
        code.append(QString::number(axis + 1));
        code.append(",");
    }
    code.chop(1);
    return code;
}

NCPCommand* NCPCommandHome::clone() const
{
    auto selfClone = new NCPCommandHome(m_resource);
    if (selfClone != nullptr) {
        selfClone->m_option = m_option;
        for (auto axis : m_axisList)
            selfClone->m_axisList.append(axis);
    }
    return selfClone;
}

void NCPCommandHome::setOption(const QString &str)
{
    if (str == "P") {
        m_option = Option::ReturnPlusDirection;
    } else if (str == "M") {
        m_option = Option::ReturnMinusDirection;
    } else if (str == "PZ") {
    } else if (str == "MZ") {
    } else if (str == "SET") {
    } else if (str == "CLR") {
    } else {
        QRegularExpression axisRe("^A[1-8]");
        auto match = axisRe.match(str);
        if (match.hasMatch()) {
            m_option = Option::ReturnMinusDirection;
        }
    }
}
