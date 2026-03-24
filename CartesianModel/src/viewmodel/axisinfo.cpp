#include "axisinfo.h"
#include "axisdata.h"

AxisInfo::AxisInfo(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < Axis::AxisNum; i++)
        m_axisDataList.append(new AxisData(this));
}

int AxisInfo::setDatas(Axis axis, bool home, bool over, bool brake, int origin, qint32 position, qint16 encoder, qint16 torque)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatas(home, over, brake, origin, position, encoder, torque);
    return 0;
}

int AxisInfo::setDatas(Axis axis, bool home, bool over, bool brake, int origin, QString position, QString encoder, QString torque)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatas(home, over, brake, origin, position, encoder, torque);
    return 0;
}

int AxisInfo::setDatas(Axis axis, bool home, bool over, bool brake, bool arrival, QString position, QString encoder, QString torque, QString speed)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatas(home, over, brake, arrival, position, encoder, torque, speed);
    return 0;
}

int AxisInfo::setDatas(Axis axis, bool home, bool over, bool brake, int origin, QString originPercent, QString position, QString encoder, QString torque)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatas(home, over, brake, origin, originPercent, position, encoder, torque);
    return 0;
}

int AxisInfo::setDatas(Axis axis, QString position, QString startPos, QString startSpeed,
    QString pointCount, QString pointValue, QString pitch, bool direction)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatas(position, startPos, startSpeed, pointCount, pointValue, pitch, direction);
    return 0;
}

int AxisInfo::setStandarStackDatas(Axis axis, QString startPos, QString startSpeed, QString pointCount, QString pointValue, QString pitch, bool direction)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatas(startPos, startSpeed, pointCount, pointValue, pitch, direction);
    return 0;
}

int AxisInfo::setDatas(Axis axis, QString position, QString correctValue, QString speed, QString pointCount, QString pointValue)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatas(position, correctValue, speed, pointCount, pointValue);
    return 0;
}

int AxisInfo::setOriginDatas(Axis axis, bool home, bool over, bool brake, int origin, QString originPercent, QString encoder, QString torque)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setorgDatas(home, over, brake, origin, originPercent, encoder, torque);
    return 0;
}

int AxisInfo::setFreeStackDatas(Axis axis, QString correctValue, QString speed, QString pointCount, QString pointValue)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatas(correctValue, speed, pointCount, pointValue);
    return 0;
}

int AxisInfo::setDatasRT(Axis axis, QString speed, QString deviation, QString torque, QString loadRate, QString resurrectionResistorRate)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatasRT(speed, deviation, torque, loadRate, resurrectionResistorRate);
    return 0;
}

int AxisInfo::setDatasPeak(Axis axis, QString speedPeak, QString deviationPeak, QString torquePeak)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatasPeak(speedPeak, deviationPeak, torquePeak);
    return 0;
}

int AxisInfo::setDatasMin(Axis axis, QString speedMin, QString deviationMin, QString torqueMin)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatasMin(speedMin, deviationMin, torqueMin);
    return 0;
}

int AxisInfo::setPosition(Axis axis, const QString &position)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setPosition(position);
    return 0;
}

int AxisInfo::setSpeed(Axis axis, const QString &speed)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setSpeed(speed);
    return 0;
}

int AxisInfo::setEncoder(Axis axis, const QString &encoder)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setEncoder(encoder);
    return 0;
}

int AxisInfo::setTorque(Axis axis, const QString &torque)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setTorque(torque);
    return 0;
}

int AxisInfo::setOriginPercent(Axis axis, const QString &originPercent)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setOriginPercent(originPercent);
    return 0;
}

int AxisInfo::setServo(Axis axis, bool servo)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setServo(servo);
    return 0;
}

int AxisInfo::setMaxPosition(Axis axis, const QString &max)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setMaxPosition(max);
    return 0;
}

int AxisInfo::setDatasWaveform(Axis axis, int speed, int torque)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setDatasWaveform(speed, torque);
    return 0;
}

int AxisInfo::setHome(Axis axis, int home)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setHome(home);
    return 0;
}

int AxisInfo::setOver(Axis axis, int over)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setOver(over);
    return 0;
}

int AxisInfo::setBrake(Axis axis, int brake)
{
    if (axis >= m_axisDataList.length())
        return -1;
    AxisData *data = static_cast<AxisData *>(m_axisDataList.at(axis));
    data->setBrake(brake);
    return 0;
}


void AxisInfo::setAxisDataList(QList<QObject *> list)
{
    if (m_axisDataList != list) {
        m_axisDataList = list;
        emit axisDataListChanged(m_axisDataList);
    }
}

AxisInfo* AxisInfo::instance = nullptr;
