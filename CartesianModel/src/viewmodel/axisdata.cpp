#include "axisdata.h"
#include <QDebug>

AxisData::AxisData(QObject *parent) : QObject(parent)
{
}

bool AxisData::operator ==(AxisData &dat)
{
    if (dat.home() != m_home) return false;
    if (dat.over() != m_over) return false;
    if (dat.brake() != m_brake) return false;
    if (dat.origin() != m_origin) return false;
    if (dat.position() != m_position) return false;
    if (dat.encoder() != m_encoder) return false;
    if (dat.torque() != m_torque) return false;
    if (dat.servo() != m_servo) return false;
    if (dat.arrival() != m_arrival) return false;
    if (dat.speed() != m_speed) return false;
    if (dat.originPercent() != m_originPercent) return false;
    if (dat.startPos() != m_startPos) return false;
    if (dat.startSpeed() != m_startSpeed) return false;
    if (dat.pointCount() != m_pointCount) return false;
    if (dat.pointValue() != m_pointValue) return false;
    if (dat.pitch() != m_pitch) return false;
    if (dat.direction() != m_direction) return false;
    if (dat.correctValue() != m_correctValue) return false;
    if (dat.deviation() != m_deviation) return false;
    if (dat.speedPeak() != m_speedPeak) return false;
    if (dat.torquePeak() != m_torquePeak) return false;
    if (dat.deviationPeak() != m_deviationPeak) return false;
    if (dat.speedValue() != m_speedValue) return false;
    if (dat.torqueValue() != m_torqueValue) return false;
    return true;
}

AxisData &AxisData::operator =(AxisData &dat)
{
    qDebug() << "a";
    setDatas(dat.home(), dat.over(), dat.brake(), dat.origin(), dat.position(), dat.encoder(), dat.torque());
    setServo(dat.servo());
    return *this;
}

AxisData &AxisData::operator =(AxisData dat)
{
    qDebug() << "a";
    setDatas(dat.home(), dat.over(), dat.brake(), dat.origin(), dat.position(), dat.encoder(), dat.torque());
    return *this;
}

void AxisData::setDatas(bool home, bool over, bool brake, int origin, QString position, QString encoder, QString torque)
{
    setHome(home);
    setOver(over);
    setBrake(brake);
    setOrigin(origin);
    setPosition(position);
    setEncoder(encoder);
    setTorque(torque);
}


void AxisData::setDatas(bool home, bool over, bool brake, int origin, qint32 position, qint16 encoder, qint16 torque)
{
    setHome(home);
    setOver(over);
    setBrake(brake);
    setOrigin(origin);
    setPosition(QString::number(position));
    setEncoder(QString::number(encoder));
    setTorque(QString::number(torque));
}

void AxisData::setDatas(bool home, bool over, bool brake, bool arrival, QString position, QString encoder, QString torque, QString speed)
{
    setHome(home);
    setOver(over);
    setBrake(brake);
    setArrival(arrival);
    setPosition(position);
    setEncoder(encoder);
    setTorque(torque);
    setSpeed(speed);
}

void AxisData::setDatas(bool home, bool over, bool brake, int origin, QString originPercent, QString position, QString encoder, QString torque)
{
    setHome(home);
    setOver(over);
    setBrake(brake);
    setOrigin(origin);
    setOriginPercent(originPercent);
    setPosition(position);
    setEncoder(encoder);
    setTorque(torque);
}

void AxisData::setDatas(QString position, QString startPos, QString startSpeed,
    QString pointCount, QString pointValue, QString pitch, bool direction)
{
    setPosition(position);
    setStartPos(startPos);
    setStartSpeed(startSpeed);
    setPointCount(pointCount);
    setPointValue(pointValue);
    setPitch(pitch);
    setDirection(direction);
}

void AxisData::setDatas(QString startPos, QString startSpeed, QString pointCount, QString pointValue, QString pitch, bool direction)
{
    setStartPos(startPos);
    setStartSpeed(startSpeed);
    setPointCount(pointCount);
    setPointValue(pointValue);
    setPitch(pitch);
    setDirection(direction);
}

void AxisData::setDatas(QString position, QString correctValue, QString speed, QString pointCount, QString pointValue)
{
    setPosition(position);
    setCorrectValue(correctValue);
    setSpeed(speed);
    setPointCount(pointCount);
    setPointValue(pointValue);
}

void AxisData::setDatas(QString correctValue, QString speed, QString pointCount, QString pointValue)
{
    setCorrectValue(correctValue);
    setSpeed(speed);
    setPointCount(pointCount);
    setPointValue(pointValue);
}

void AxisData::setDatasRT(QString speed, QString deviation, QString torque, QString loadRate, QString resurrectionResistorRate)
{
    setSpeed(speed);
    setDeviation(deviation);
    setTorque(torque);
    setLoadRate(loadRate);
    setResurrectionResistorRate(resurrectionResistorRate);
}

void AxisData::setDatasPeak(QString speedPeak, QString deviationPeak, QString torquePeak)
{
    setSpeedPeak(speedPeak);
    setDeviationPeak(deviationPeak);
    setTorquePeak(torquePeak);
}

void AxisData::setDatasMin(QString speedMin, QString deviationMin, QString torqueMin)
{
    setSpeedMin(speedMin);
    setDeviationMin(deviationMin);
    setTorqueMin(torqueMin);
}

void AxisData::setorgDatas(bool home, bool over, bool brake, int origin, QString originPercent, QString encoder, QString torque)
{
    setHome(home);
    setOver(over);
    setBrake(brake);
    setOrigin(origin);
    setOriginPercent(originPercent);
    setEncoder(encoder);
    setTorque(torque);
}

void AxisData::setDatasWaveform(int speed, int torque)
{
    setSpeedValue(speed);
    setTorqueValue(torque);
}
