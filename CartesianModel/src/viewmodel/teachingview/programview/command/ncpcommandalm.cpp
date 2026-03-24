#include "ncpcommandalm.h"
#include <QRegularExpression>

//------------------------------------------------------
// NCPCommandAlm.
//------------------------------------------------------
NCPCommandAlm::NCPCommandAlm(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandAlm::NCPCommandAlm(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
NCPCommandAlm::NCPCommandAlm(ResourceManager *resource, int alarmIndex, QObject *parent)
    : NCPCommand(resource, parent)
{
    //qDebug("%s:%d alarmIndex[%d]", __func__, __LINE__, alarmIndex);
    mCurAlarmIndex = alarmIndex;
}
//------------------------------------------------------
bool NCPCommandAlm::parse(const QString &commandLine)
{
    //qDebug() << "NCPCommandAlm::parse commandLine:" << commandLine;
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    QStringList splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug() << "NCPCommandAlm::parse:" << "Syntax error commandLine:[" << commandLine << "]";
        return false;
    }
    //qDebug() << "commandLine arg : " << splitList[1];
    bool is_ok;
    int num = splitList[1].toInt(&is_ok, 10);
    if (!is_ok){
        qDebug() << "splitList is not Numerical value : " << splitList[1];
        return false;
    }
    if (num <= 0){
        qDebug("%s:%d num[%d] is invalid", __func__, __LINE__, num);
        return false;
    }
    //qDebug() << "num:" << num;
    mCurAlarmIndex = num - 1;
    return true;
}
//------------------------------------------------------
QString NCPCommandAlm::nameString() const
{
    return m_resource->label(41, PEN_MSG_PROGRAM_WIZARD_ALARM, m_resource->language());
    // return m_resource->label(41, PEN_MSG_PROGRAM_WIZARD_USER_ALARM, m_resource->language());
}
//------------------------------------------------------
QString NCPCommandAlm::toString() const
{
    if (mCurAlarmIndex < 0) return "";
    //qDebug() << "NCPCommandAlm::toString mCurAlarmIndex:" << mCurAlarmIndex;
    QString argValue = ((mCurAlarmIndex + 1) < 10) ? "0" + QString::number(mCurAlarmIndex + 1) : QString::number(mCurAlarmIndex + 1);
    //qDebug() << "NCPCommandAlm::toString str:" << str;
    return argValue;
}
//------------------------------------------------------
QString NCPCommandAlm::toCode() const
{
    //qDebug() << "NCPCommandWait::toCode mCurAlarmIndex:" << mCurAlarmIndex;
    if (!m_resource) return "";
    if (mCurAlarmIndex < 0) return "";
    QString argValue = ((mCurAlarmIndex + 1) < 10) ? "0" + QString::number(mCurAlarmIndex + 1) : QString::number(mCurAlarmIndex + 1);
    //qDebug() << "NCPCommandWait::toCode argValue:" << argValue;
    return QString("ALM\t%1").arg(argValue);
}
//------------------------------------------------------
NCPCommand* NCPCommandAlm::clone() const
{
    //qDebug() << "NCPCommandAlm::clone mCurAlarmIndex:" << mCurAlarmIndex;
    return new NCPCommandAlm(m_resource, mCurAlarmIndex);
}
