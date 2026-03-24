#include "timerdata.h"

TimerData::TimerData(QObject *parent) : QObject(parent)
{
    m_Marklabel = new Label(this);
    m_Msglabel = new Label(this);
}

void TimerData::setTime(int value)
{
    if (m_time != value) {
        m_time = value;
        emit timeChanged(value);
    }
}

void TimerData::setFormat(int format)
{
    if (m_format != format) {
        m_format = format;
    }
}

void TimerData::setStartState(bool state)
{
    if (m_startState != state) {
        m_startState = state;
        emit startStateChanged(m_startState);
    }
}

void TimerData::setUpState(bool state)
{
    if (m_upState != state) {
        m_upState = state;
        emit upStateChanged(m_upState);
    }
}

void TimerData::setMarklabelData(const QString &name, const QString &color)
{
    m_Marklabel->setName(name);
    m_Marklabel->setColor(color);
}

void TimerData::setMsglabelData(const QString &name, const QString &color)
{
    m_Msglabel->setName(name);
    m_Msglabel->setColor(color);
}

void TimerData::setMin(QString min)
{
    m_min = min;
}

void TimerData::setMax(QString max)
{
    m_max = max;
}
