#include "ncpcommandwait.h"
#include <QDebug>
#include <QRegularExpression>

const int PEN_SET_DELAY_TIMER_DIDO_ADDR_TOP_INDEX = 33;
#define TIMER_ITEM_VALUE_TOP_HR             2125

//------------------------------------------------------
// NCPCommandWait.
//------------------------------------------------------
NCPCommandWait::NCPCommandWait(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandWait::NCPCommandWait(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
    initSymbolListPenSetData();
}
//------------------------------------------------------
NCPCommandWait::NCPCommandWait(ResourceManager *resource, QString timerValue, QObject *parent)
    : NCPCommand(resource, parent)
{
    //qDebug() << "NCPCommandWait::NCPCommandWait timerValue:" << timerValue;
    initSymbolListPenSetData();
    mTimerValue = timerValue;
}
//------------------------------------------------------
NCPCommandWait::NCPCommandWait(ResourceManager *resource, QString timerValue, int timerListIndex, int symbolListIndex,
    int category, QObject *parent)
    : NCPCommand(resource, parent)
{
    //qDebug() << "NCPCommandWait::NCPCommandWait timerValue:" << timerValue << "category:" << category << "timerListIndex:" << timerListIndex << "symbolListIndex:" << symbolListIndex;
    initSymbolListPenSetData();
    mTimerValue = timerValue;
    mCurCategory = category;
    mTimerListIndex = timerListIndex;
    mSymbolListIndex = symbolListIndex;
}
//------------------------------------------------------
void NCPCommandWait::initSymbolListPenSetData()
{
    if (!m_resource) return;
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 8; j++) {
            int index = m_resource->getPenSetStringValue("16", (j + PEN_SET_DELAY_TIMER_DIDO_ADDR_TOP_INDEX), i).toInt();
            if (index == 0){
                continue;
            }
            //qDebug() << "index:" << index << "symbolAt:" << m_resource->symbolAt(index);
            QString symbolTitle = m_resource->symbolAt(index);
            if(m_resource->getPenSetStringValue("16", (j + PEN_SET_DELAY_TIMER_DIDO_ADDR_TOP_INDEX), i) != NULL){
                m_symbolIndex.append(index);
                m_symbolTitle.append(symbolTitle);
            }else {
                continue;
            }
        }
    }
#if 0
    for(int i = 0; i < m_symbolTitle.count(); i++){
        qDebug() << "m_symbolIndex:" << m_symbolIndex.at(i) << "m_symbolTitle:" << m_symbolTitle.at(i);
    }
#endif
}

void NCPCommandWait::getTimerAndSymbolIndex(QString &symbolTitle, QString *timerValue, int *timerIndex, int *symbolIndex)
{
    //qDebug() << "symbolTitle:" << symbolTitle;
    if (!m_modbus) return;
    if (!timerIndex || !symbolIndex || !timerValue) return;

    int i = 0;
    for(i = 0; i < m_symbolTitle.count(); i++){
        if (m_symbolTitle.at(i) == symbolTitle){
            //qDebug() << "i:" << i << "m_symbolTitle:" << m_symbolTitle.at(i);
            break;
        }
    }
    if (i >= m_symbolTitle.count()){
        *timerIndex = -1;
        *symbolIndex = -1;
    } else {
        *timerIndex = i;
        *symbolIndex = m_symbolIndex.at(i);
        int targetTimerValue = m_modbus->getReceiveDataHR(TIMER_ITEM_VALUE_TOP_HR + i);
        double timer_Value = (double)((double)targetTimerValue / 100.0);
        *timerValue = QString::number(timer_Value, 'f', 2);
    }
}

//------------------------------------------------------
bool NCPCommandWait::parse(const QString &commandLine)
{
    //qDebug() << "NCPCommandWait::parse commandLine:" << commandLine;
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    QStringList splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug() << "NCPCommandWait::parse:" << "Syntax error commandLine:[" << commandLine << "]";
        return false;
    }
    if (splitList[1] == "POS"){
        //qDebug() << "NCPCommandWait::parse:" << "WAIT POS is not valid on WAIT command";
        return false;
    }
    bool is_ok;
    double num = splitList[1].toDouble(&is_ok);
    if (!is_ok){
        //qDebug() << "splitList is not Numerical value : " << splitList[1];
    }
    if (num != 0){
        mCurCategory = 0;
        mTimerValue = splitList[1];
        mTimerListIndex = -1;
        mSymbolListIndex = -1;
    } else {
        mCurCategory = 1;
        mTimerValue = "";
        QString argValue = splitList[1];
        QString timerValue = "";
        int timerListIndex = 0;
        int symbolListIndex = 0;
        getTimerAndSymbolIndex(argValue, &timerValue, &timerListIndex, &symbolListIndex);
        //qDebug() << "argValue: " << argValue << "timerValue:" << timerValue << "timerListIndex:" << timerListIndex << "symbolListIndex:" << symbolListIndex;
        mTimerValue = timerValue;
        mTimerListIndex = timerListIndex;
        mSymbolListIndex = symbolListIndex;
    }
    return true;
}

//------------------------------------------------------
QString NCPCommandWait::nameString() const
{
    QString str = "";
    if (mCurCategory == 0){
        str = m_resource->label(41, PEN_MSG_PROGRAM_WIZARD_DELAY_TIME);
    } else if (mCurCategory == 1){
        str = m_resource->label(41, PEN_MSG_PROGRAM_WIZARD_TIMER_ITEM);
    }
    return str;
}
//------------------------------------------------------
QString NCPCommandWait::toString() const
{
    if (mCurCategory < 0) return "";
    //qDebug() << "NCPCommandWait::toString mCurCategory:" << mCurCategory << "mTimerValue:" << mTimerValue;
    QString str = "";
    if (mCurCategory == 0){
        str = QString("%1%2").arg(mTimerValue).arg("S");
    } else if (mCurCategory == 1){
        //qDebug() << "NCPCommandWait::toString mTimerListIndex:" << mTimerListIndex;
        int timerValue = m_modbus->getReceiveDataHR(TIMER_ITEM_VALUE_TOP_HR + mTimerListIndex);
        double timer_Value = (double)((double)timerValue / 100.0);
        QString tmerValueStr = QString::number(timer_Value, 'f', 2);
        str = QString("%1%2%3%4%5%6%7").arg(m_resource->symbolAt(mSymbolListIndex)).arg("(")
            .arg(m_resource->label(41, PEN_MSG_PROGRAM_WIZARD_TIMER_ITEM)).arg(QString::number(mTimerListIndex+1)).arg(")").arg(tmerValueStr).arg("S");
    }
    //qDebug() << "NCPCommandWait::toString str:" << str;
    return str;
}
//------------------------------------------------------
QString NCPCommandWait::toCode() const
{
    if (!m_resource) return "";
    if (mCurCategory < 0) return "";
    //qDebug() << "NCPCommandWait::toCode mCurCategory:" << mCurCategory << "mTimerListIndex:" << mTimerListIndex << "mSymbolListIndex:" << mSymbolListIndex;
    QString ret = "";
    if (mCurCategory == 0){
        ret = QString("WAIT\t%1").arg(mTimerValue);
    } else if (mCurCategory == 1){
        ret = QString("WAIT\t%1").arg(m_resource->symbolAt(mSymbolListIndex));
    }
    //qDebug() << "NCPCommandWait::toCode ret:" << ret;
    return ret;
}

NCPCommand* NCPCommandWait::clone() const
{
    //qDebug() << "NCPCommandWait::clone mCurCategory:" << mCurCategory << "mTimerValue" << mTimerValue
    //    << "mTimerListIndex:" << mTimerListIndex << "mSymbolListIndex:" << mSymbolListIndex;
    if (mCurCategory == 0){
        return new NCPCommandWait(m_resource, mTimerValue, -1, -1, 0);
    } else {
        return new NCPCommandWait(m_resource, mTimerValue, mTimerListIndex, mSymbolListIndex, 1);
    }
}
