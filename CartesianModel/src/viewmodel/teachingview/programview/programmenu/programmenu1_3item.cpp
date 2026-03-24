#include "programmenu1_3item.h"
#include <QDebug>

ProgramMenu1_3Item::ProgramMenu1_3Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu1_3Item::resetData()
{
    for (int i = 0; i < m_timeList.length(); i++)
        m_timeList.replace(i, 0);
    emit timeListChanged();
    setState(false);
    setSignal(false);
    setDetect(false);
}

void ProgramMenu1_3Item::setTitleIndex(int index)
{
    if (m_titleIndex != index) {
        m_titleIndex = index;
        emit titleIndexChanged();
    }
}

void ProgramMenu1_3Item::setState(bool state)
{
    if (m_state != state) {
        m_state = state;
        emit stateChanged();
    }
}

void ProgramMenu1_3Item::setDetect(bool detect)
{
    if (m_detect != detect) {
        m_detect = detect;
        emit detectChanged();
    }
}

void ProgramMenu1_3Item::setSignal(bool signal)
{
    if (m_signal != signal) {
        m_signal = signal;
        emit signalChanged();
    }
}

int ProgramMenu1_3Item::setTime(int index, int time)
{
    if ((index < 0) || (index >= m_timeList.length())) {
        return -1;
    }
    m_timeList.replace(index, time);
    return 0;
}

int ProgramMenu1_3Item::getTime(int index)
{
    if ((index < 0) || (index >= m_timeList.length())) {
        return 0;
    }
    return m_timeList.at(index);
}

void ProgramMenu1_3Item::emitTimeList()
{
    emit timeListChanged();
}

int ProgramMenu1_3Item::getKey(int index)
{
    if (index < 0 || index >= m_keyList.length()) {
        return -1;
    }
    return m_keyList.at(index);
}

void ProgramMenu1_3Item::appendTime(int time)
{
    m_timeList.append(time);
}

void ProgramMenu1_3Item::appendKey(int key)
{
    m_keyList.append(key);
}

void ProgramMenu1_3Item::setDiAddress(quint16 address)
{
    m_diAddress = address;
}

void ProgramMenu1_3Item::appendHrAddress(quint16 address)
{
    m_hrAddressList.append(address);
}
