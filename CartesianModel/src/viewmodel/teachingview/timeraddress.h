#ifndef TIMERADDRESS_H
#define TIMERADDRESS_H

#include <QObject>

class TimerAddress : public QObject
{
    Q_OBJECT
public:
    explicit TimerAddress(QObject *parent = nullptr);
    void setTime(quint16 addr);
    void setStartState(quint16 addr);
    void setUpState(quint16 addr);

    quint16 time() const { return m_time; }
    quint16 startState() const { return m_startState; }
    quint16 upState() const { return m_upState; }

private:
    quint16     m_time = 0;
    quint16     m_startState = 0;
    quint16     m_upState = 0;
};

#endif // TIMERADDRESS_H
