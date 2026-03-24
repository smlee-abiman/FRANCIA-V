#include "programmenu1_2item.h"

ProgramMenu1_2Item::ProgramMenu1_2Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu1_2Item::resetData()
{
    for (int i = 0; i < m_timeList.length(); i++)
        m_timeList.replace(i, 0);
    emit timeListChanged();

    setSignal(false);
    setUse(false);
    setWait(false);
}

void ProgramMenu1_2Item::setProgramNo(int no)
{
    m_programNo = no;
}

int ProgramMenu1_2Item::setTime(int index, int time)
{
    if ((index < 0) || (index >= m_timeList.length())) {
        return -1;
    }
    m_timeList.replace(index, time);
    return 0;
}

int ProgramMenu1_2Item::getTime(int index)
{
    if ((index < 0) || (index >= m_timeList.length())) {
        return 0;
    }
    return m_timeList.at(index);
}

void ProgramMenu1_2Item::emitTimeList()
{
    emit timeListChanged();
}

void ProgramMenu1_2Item::setSignal(int signal)
{
    if (m_signal != signal) {
        m_signal = signal;
        emit signalChanged();
    }
}

void ProgramMenu1_2Item::setUse(bool use)
{
    if (m_use != use) {
        m_use = use;
        emit useChanged();
    }
}

void ProgramMenu1_2Item::setWait(bool wait)
{
    if (m_wait != wait) {
        m_wait = wait;
        emit waitChanged();
    }
}

int ProgramMenu1_2Item::getKey(int index)
{
    if (index < 0 || index >= m_keyList.length()) {
        return -1;
    }
    return m_keyList.at(index);
}

void ProgramMenu1_2Item::appendTime(int time)
{
    m_timeList.append(time);
}

void ProgramMenu1_2Item::appendKey(int key)
{
    m_keyList.append(key);
}

void ProgramMenu1_2Item::appendDiAddress(quint16 address)
{
    m_diAddressList.append(address);
}

void ProgramMenu1_2Item::appendHrAddress(quint16 address)
{
    m_hrAddressList.append(address);
}
