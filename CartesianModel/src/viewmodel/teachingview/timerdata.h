#ifndef TIMERDATA_H
#define TIMERDATA_H

#include <QObject>
#include "label.h"

class TimerData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(bool startState READ startState WRITE setStartState NOTIFY startStateChanged)
    Q_PROPERTY(bool upState READ upState WRITE setUpState NOTIFY upStateChanged)
    Q_PROPERTY(Label *marklabel READ marklabel WRITE setMarklabel NOTIFY marklabelChanged)
    Q_PROPERTY(Label *msglabel READ msglabel WRITE setMsglabel NOTIFY msglabelChanged)
    Q_PROPERTY(QString min READ min CONSTANT)
    Q_PROPERTY(QString max READ max CONSTANT)
public:
    explicit TimerData(QObject *parent = nullptr);

    void setTime(int value);
    void setFormat(int format);
    void setStartState(bool state);
    void setUpState(bool state);
    void setMarklabelData(const QString &name, const QString &color);
    void setMsglabelData(const QString &name, const QString &color);
    void setMin(QString min);
    void setMax(QString max);

    int time() const { return m_time; }
    int format() const { return m_format; }
    bool startState() const { return m_startState; }
    bool upState() const { return m_upState; }
    Label* marklabel() {return  m_Marklabel;}
    Label* msglabel() {return  m_Msglabel;}
    QString min() const {return m_min;}
    QString max() const {return m_max;}

signals:
    void timeChanged(int value);
    void startStateChanged(bool state);
    void upStateChanged(bool state);
    void marklabelChanged(Label *marklabel);
    void msglabelChanged(Label *msglabel);

public slots:
    void setMarklabel(Label *marklabel){
        if(m_Marklabel != marklabel){
            m_Marklabel = marklabel;
            emit marklabelChanged(marklabel);
        }
    }
    void setMsglabel(Label *msglabel){
        if(m_Msglabel != msglabel){
            m_Msglabel = msglabel;
            emit marklabelChanged(msglabel);
        }
    }

private:
    int     m_time = 0;
    int     m_format = 0;
    bool    m_startState = false;
    bool    m_upState = false;
    Label   *m_Marklabel = nullptr;
    Label   *m_Msglabel = nullptr;
    QString m_min;
    QString m_max;
};

#endif // TIMERDATA_H
