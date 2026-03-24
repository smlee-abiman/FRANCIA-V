#include <QRegularExpressionMatch>
#include "ncpcommandacc.h"

//------------------------------------------------------
// NCPCommandAcc.
//------------------------------------------------------
NCPCommandAcc::NCPCommandAcc(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandAcc::NCPCommandAcc(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandAcc::parse(const QString &commandLine)
{
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug() << "Syntax error commandLine:[" << commandLine << "]";
        return false;
    }

    auto onOff = splitList.at(1);
    setOn(onOff);

    if (m_on) {
        const int num = splitList.length();
        QRegularExpression axisRe("^A[1-8]");
        int phase = 0;
        int axis = 0;
        int acceleration = 0;
        bool ok;
        for (int i = 2; i < num; i++) {
            auto arg = splitList.at(i);
            if (phase == 0) {
                auto match = axisRe.match(arg);
                if (match.hasMatch()) {
                    axis = arg.right(1).toInt(&ok) - 1;
                    if (ok) {
                        phase = 1;
                    }
                }
            } else if (phase == 1) {
                acceleration = arg.toInt(&ok);
                if (ok) {
                    NCPCommandAcc::Data data(axis, acceleration);
                    m_accelerationList.append(data);
                }
                phase = 0;
            }
        }
    }
    return true;
}
//------------------------------------------------------
QString NCPCommandAcc::nameString() const
{
    return m_resource->label(41, 519);
}
//------------------------------------------------------
QString NCPCommandAcc::toString() const
{
    auto string = QString("");
    string.append(m_on ? "ON" : "OFF");
    if (m_on) {
        string.append(" ");
        for (auto data : m_accelerationList)
            string.append(QString("%1(%2) ,").arg(m_resource->label(22, data.axis())).arg(ACCELERATION_MAX - data.acceleration()));
        string.chop(2);
    }
    return string;
}
//------------------------------------------------------
QString NCPCommandAcc::toCode() const
{
    auto code = QString("ACC\t");
    code.append(m_on ? "ON" : "OFF");
    if (m_on) {
        code.append(",");
        for (auto data : m_accelerationList)
            code.append(QString("A%1 %2,").arg(QString::number(data.axis() + 1)).arg(data.acceleration()));
        code.chop(1);
    }
    return code;
}

NCPCommand* NCPCommandAcc::clone() const
{
    auto selfClone = new NCPCommandAcc(m_resource);
    if (selfClone != nullptr) {
        selfClone->setOn(m_on);
        for (auto data : m_accelerationList)
            selfClone->m_accelerationList.append(data);
    }
    return selfClone;
}

void NCPCommandAcc::setAcceleration(int axis, int acceleration)
{
    NCPCommandAcc::Data data(axis, acceleration);
    bool found = false;
    const int num = m_accelerationList.length();
    for (int i = 0; i < num; i++) {
        if (m_accelerationList.at(i).axis() == axis) {
            m_accelerationList.replace(i, data);
            found = true;
            break;
        }
    }
    if (!found) {
        m_accelerationList.append(data);
    }
}

void NCPCommandAcc::setOn(bool on)
{
    m_on = on;
}

void NCPCommandAcc::setOn(const QString &str)
{
    if (str == "ON") {
        m_on = true;
    } else if (str == "OFF") {
        m_on = false;
    }
}
