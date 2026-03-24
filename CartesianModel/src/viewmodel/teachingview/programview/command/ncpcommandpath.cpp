#include <QRegularExpressionMatch>
#include "ncpcommandpath.h"

//------------------------------------------------------
// NCPCommandPath.
//------------------------------------------------------
NCPCommandPath::NCPCommandPath(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandPath::NCPCommandPath(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandPath::parse(const QString &commandLine)
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
        int pos = 0;
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
                pos = static_cast<int>(arg.toDouble(&ok) * 100);
                if (ok) {
                    NCPCommandPath::Data data(axis, pos);
                    m_positionList.append(data);
                }
                phase = 0;
            }
        }
    }
    return true;
}
//------------------------------------------------------
QString NCPCommandPath::nameString() const
{
    return m_resource->label(41, 516);
}
//------------------------------------------------------
QString NCPCommandPath::toString() const
{
    auto string = QString("");
    string.append(m_on ? "ON" : "OFF");
    if (m_on) {
        string.append(" ");
        for (auto data : m_positionList) {
            string.append(QString("%1(%2mm),").arg(m_resource->label(22, data.axis())).arg(static_cast<double>(data.position()) / 100, 0, 'f', 2));
        }
        string.chop(1);
    }
    return string;
}
//------------------------------------------------------
QString NCPCommandPath::toCode() const
{
    auto code = QString("PATH\t");
    code.append(m_on ? "ON" : "OFF");
    if (m_on) {
        code.append(" ");
        for (auto data : m_positionList)
            code.append(QString("A%1 %2,").arg(QString::number(data.axis() + 1)).arg(static_cast<double>(data.position()) / 100, 0, 'f', 2));
        code.chop(1);
    }
    return code;
}

NCPCommand* NCPCommandPath::clone() const
{
    auto selfClone = new NCPCommandPath(m_resource);
    if (selfClone != nullptr) {
        selfClone->setOn(m_on);
        for (auto data : m_positionList)
            selfClone->m_positionList.append(data);
    }
    return selfClone;
}

void NCPCommandPath::setPosition(int axis, int position)
{
    NCPCommandPath::Data data(axis, position);
    bool found = false;
    const int num = m_positionList.length();
    for (int i = 0; i < num; i++) {
        if (m_positionList.at(i).axis() == axis) {
            m_positionList.replace(i, data);
            found = true;
            break;
        }
    }
    if (!found) {
        m_positionList.append(data);
    }
}

void NCPCommandPath::setOn(bool on)
{
    m_on = on;
}

void NCPCommandPath::setOn(const QString &str)
{
    if (str == "ON") {
        m_on = true;
    } else if (str == "OFF") {
        m_on = false;
    }
}

