#include "ncpcommandpause.h"
#include <QRegularExpression>

//------------------------------------------------------
// NCPCommandPause.
//------------------------------------------------------
NCPCommandPause::NCPCommandPause(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandPause::NCPCommandPause(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
NCPCommandPause::NCPCommandPause(ResourceManager *resource, int category, QObject *parent)
    : NCPCommand(resource, parent)
{
    mCurCategory = category;
}
//------------------------------------------------------
bool NCPCommandPause::parse(const QString &commandLine)
{
    //qDebug() << "NCPCommandPause::parse commandLine:" << commandLine;
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    QStringList splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug() << "NCPCommandPause::parse:" << "Syntax error commandLine:[" << commandLine << "]";
        return false;
    }
    //qDebug() << "commandLine arg : " << splitList[1];
    if (splitList[1] == "ON"){
        mCurCategory = (int)PauseCategory::PauseCategoryOn;
    } else if (splitList[1] == "OFF"){
        mCurCategory = (int)PauseCategory::PauseCategoryOff;
    } else {
        mCurCategory = (int)PauseCategory::PauseCategoryInvalid;
    }
    //qDebug() << "NCPCommandWait::parse mCurCategory : " << mCurCategory;
    return true;
}
//------------------------------------------------------
QString NCPCommandPause::nameString() const
{
    //qDebug() << "NCPCommandPause::toString mCurCategory:" << mCurCategory;
    if (mCurCategory < 0) return "";
    QString str = "";
    if (mCurCategory == (int)PauseCategory::PauseCategoryOn){
        str = m_resource->label(41, PEN_MSG_PROGRAM_WIZARD_PAUSE_ON, m_resource->language());
    } else if (mCurCategory == (int)PauseCategory::PauseCategoryOff){
        str = m_resource->label(41, PEN_MSG_PROGRAM_WIZARD_PAUSE_OFF, m_resource->language());
    } else {
        str = "";
    }
    return str;
}
//------------------------------------------------------
QString NCPCommandPause::toString() const
{
    return "";
}
//------------------------------------------------------
QString NCPCommandPause::toCode() const
{
    if (mCurCategory == (int)PauseCategory::PauseCategoryOn){
        return QString("PAUSE\t%1").arg("ON");
    } else if (mCurCategory == (int)PauseCategory::PauseCategoryOff){
        return QString("PAUSE\t%1").arg("OFF");
    } else {
        return "";
    }
}
//------------------------------------------------------
NCPCommand* NCPCommandPause::clone() const
{
    //qDebug() << "NCPCommandPause::clone mCurCategory:" << mCurCategory;
    if (mCurCategory == 0){
        return new NCPCommandPause(m_resource, (int)NCPCommandPause::PauseCategory::PauseCategoryOn);
    } else {
        return new NCPCommandPause(m_resource, (int)NCPCommandPause::PauseCategory::PauseCategoryOff);
    }
}
