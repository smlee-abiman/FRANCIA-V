#ifndef ACCELERATIONVIEWMODEL_H
#define ACCELERATIONVIEWMODEL_H

#include "viewmodelbase.h"
#include <QQmlEngine>

class AccelerationTimerData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uchar timer READ timer NOTIFY timerChanged)

public:
    explicit AccelerationTimerData(QObject *parent = nullptr);
    ~AccelerationTimerData();

    void setTimer(uchar timer);

    uchar timer() const { return m_timer; }

private:
    uchar m_timer;

signals:
    void timerChanged(uchar timer);
};

class AccelerationData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long accelerationAddress READ accelerationAddress CONSTANT)
    Q_PROPERTY(short acceleration READ acceleration NOTIFY accelerationChanged)
    Q_PROPERTY(AccelerationTimerData* timer READ timer NOTIFY timerChanged)

public:
    explicit AccelerationData(long accelerationAddress, QObject *parent = nullptr);
    ~AccelerationData();

    Q_INVOKABLE AccelerationTimerData* getTimer(int key) const { return m_timerAry[key]; }

    void setAcceleration(short acceleration);

    long accelerationAddress() const { return m_accelerationAddress; }
    short acceleration() const { return m_acceleration; }
    AccelerationTimerData* timer() const { return m_timerAry[m_acceleration]; }

private:
    long m_accelerationAddress;
    short m_acceleration;
    AccelerationTimerData *m_timerAry[26];

signals:
    void accelerationChanged(short acceleration);
    void timerChanged(AccelerationTimerData* timer);
};

class AccelerationViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(short allAcceleration READ allAcceleration NOTIFY allAccelerationChanged)
    Q_PROPERTY(short interpolationAcceleration READ interpolationAcceleration NOTIFY interpolationAccelerationChanged)

public:
    explicit AccelerationViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~AccelerationViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();

    Q_INVOKABLE int writeAcceleration(long hrAddr, short value);
    Q_INVOKABLE int writeAllAcceleration(short value);
    Q_INVOKABLE int writeInterpolationAcceleration(short value);
    Q_INVOKABLE QObject* getData(int axisIndex);

    void setAllAcceleration(short allAcceleration);
    void setInterpolationAcceleration(short interpolationAcceleration);

    short allAcceleration() const { return m_allAcceleration; }
    short interpolationAcceleration() const { return m_interpolationAcceleration; }

private:
    ModbusManager           *m_modbusManager;
    AccelerationData        *m_dataList[8];
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();

    short                   m_allAcceleration;
    short                   m_interpolationAcceleration;
    const QList<int>        AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onFinished();

signals:
    void allAccelerationChanged(short allAcceleration);
    void interpolationAccelerationChanged(short interpolationAcceleration);
};


#endif // ACCELERATIONVIEWMODEL_H
