#include "ncpcommandmove.h"
#include "axispointinfo.h"
#include <QRegularExpression>
#include "axispointinfo.h"

//------------------------------------------------------
// NCPCommandMove.
//------------------------------------------------------
NCPCommandMove::NCPCommandMove(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandMove::NCPCommandMove(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}

void NCPCommandMove::appendAxis(int axis)
{
    m_axisList.append(axis);
}

void NCPCommandMove::setRelative(bool relative)
{
    m_relative = relative;
}

void NCPCommandMove::setPointNo(int no)
{
    m_pointNo = no;
}

QString NCPCommandMove::getPointNoCode(int pointNo) const
{
    auto code = QString("");
    auto pointType = AxisPointInfo::getPointType(m_pointNo);
    if ((pointType == AxisPointInfo::PointType::StandardPalletize) ||
        (pointType == AxisPointInfo::PointType::FreePalletize)) {
        code += QString("PLT%1").arg(AxisPointInfo::getPalletizeNo(pointNo));
    } else {
        code += QString("P%1").arg(pointNo, 3, 10, QChar('0'));
    }
    return code;
}

int NCPCommandMove::getPointNo(const QString &plt) const
{
    int ret = -1;
    const int pltNo = plt.mid(3, plt.length() - 3).toInt();
    if ((pltNo >= 1) && (pltNo <= 8)) {
        ret = AxisPointInfo::getStandardPalletizePointNo(pltNo);
    } else if ((pltNo >= 9) && (pltNo <= 16)) {
        ret = AxisPointInfo::getFreePalletizePointNo(pltNo - 8);
    }
    return ret;
}

//------------------------------------------------------
bool NCPCommandMove::parse(const QString &commandLine)
{
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 3) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }

    if (splitList.at(1) == "I")
        m_relative = true;

    QRegularExpression axisRe("^A[1-8]");
    QRegularExpression pointNoRe("^P\\d\\d\\d");
    QRegularExpression pltRe("^PLT\\d+");
    for (auto arg: splitList) {
        auto axisMatch = axisRe.match(arg);
        if (axisMatch.hasMatch()) {
            int axis = arg.right(1).toInt() - 1;
            m_axisList.append(axis);
            continue;
        }
        auto pointNoMatch = pointNoRe.match(arg);
        if (pointNoMatch.hasMatch()) {
            m_pointNo = arg.mid(1, 3).toInt();
            continue;
        }
        auto pltMatch = pltRe.match(arg);
        if (pltMatch.hasMatch()) {
            int pointNo = getPointNo(arg);
            if (pointNo != -1)
                m_pointNo = pointNo;
            continue;
        }
    }
    return true;
}
//------------------------------------------------------
QString NCPCommandMove::nameString() const
{
    auto string = QString("");
    switch(AxisPointInfo::getPointType(m_pointNo)) {
    case AxisPointInfo::PointType::Arbitrary:
        string = m_resource->label(41, 17);
        break;
    case AxisPointInfo::PointType::Fix:
    case AxisPointInfo::PointType::FreePalletize:
    case AxisPointInfo::PointType::StandardPalletize:
        string = m_resource->label(41, 18);
        break;
    default:
        break;
    }
    return string;
}
//------------------------------------------------------
QString NCPCommandMove::toString() const
{
    auto string = QString("");
    if (type() == NCPCommand::Type::Movep) {
        string += m_resource->label(41, 490) + " ";
    }
    auto axisString = m_resource->label(37, 20, m_resource->language());
    for (int axis : m_axisList) {
        string.append(axisString);
        string.append(QString::number(axis + 1));
        string.append(",");
    }
    string.chop(1);
    return string;
}
//------------------------------------------------------
QString NCPCommandMove::toCode() const
{
    auto code = command() + "\t";
    if (m_relative) {
        code.append("I ");
    }
    for (int axis : m_axisList) {
        code.append("A");
        code.append(QString::number(axis + 1));
        code.append(",");
    }
    code.chop(1);
    code.append(" ");
    code.append(getPointNoCode(m_pointNo));
    return code;
}

NCPCommand* NCPCommandMove::clone() const
{
    auto selfClone = new NCPCommandMove(m_resource);
    if (selfClone != nullptr) {
        for (auto axis : m_axisList)
            selfClone->m_axisList.append(axis);
        selfClone->m_relative = m_relative;
        selfClone->m_pointNo = m_pointNo;
    }
    return selfClone;
}
