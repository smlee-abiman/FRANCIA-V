#include "programmenu1_1item.h"

ProgramMenu1_1Item::ProgramMenu1_1Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu1_1Item::appendTimerAddress(quint16 address)
{
    m_timerAddressList.append(address);
    m_timeList.append(0);
}

quint16 ProgramMenu1_1Item::getTimerAddress(int index)
{
    if (index <= -1 || index >= m_timerAddressList.length()) {
        return 0;
    }
    return m_timerAddressList.at(index);
}

int ProgramMenu1_1Item::setTime(int index, int value)
{
    if (index <= -1 || index >= m_timeList.length()) {
        return -1;
    }

    m_timeList.replace(index, value);
    return 0;
}

int ProgramMenu1_1Item::getTime(int index)
{
    if (index <= -1 || index >= m_timeList.length()) {
        return 0;
    }
    return m_timeList.at(index);
}
