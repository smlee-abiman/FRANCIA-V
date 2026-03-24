#include "timeraddress.h"

TimerAddress::TimerAddress(QObject *parent) : QObject(parent)
{

}

void TimerAddress::setTime(quint16 addr)
{
    if (m_time != addr) {
        m_time = addr;
    }
}

void TimerAddress::setStartState(quint16 addr)
{
    if (m_startState != addr) {
        m_startState = addr;
    }
}

void TimerAddress::setUpState(quint16 addr)
{
    if (m_upState != addr) {
        m_upState = addr;
    }
}
