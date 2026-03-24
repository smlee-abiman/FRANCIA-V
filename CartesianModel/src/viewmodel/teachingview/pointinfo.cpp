#include "pointinfo.h"
#include <QDebug>

PointInfo::PointInfo(int numAxis, QObject *parent)
 : QObject(parent)
{
    for (int i = 0; i < numAxis; i++) {
        PointData *data = new PointData(this);
        m_positionList.append(data);
        data = new PointData(this);
        m_sppedList.append(data);
    }
}

PointInfo::PointInfo(QObject *parent) : QObject(parent)
{
}

void PointInfo::setLabelName(const QString &name)
{
    if (m_labelName != name) {
        m_labelName = name;
        emit labelNameChanged(m_labelName);
    }
}

void PointInfo::setLabelColor(const QString &color)
{
    if (m_labelColor != color) {
        m_labelColor = color;
        emit labelColorChanged(m_labelColor);
    }
}

void PointInfo::setAddress(int axis, int positionAddress, int speedAddress)
{
    PointData *pointData;
    if (axis < m_positionList.length()) {
        pointData = static_cast<PointData *>(m_positionList.at(axis));
        pointData->setAddress(positionAddress);
    }
    if (axis < m_sppedList.length()) {
        pointData = static_cast<PointData *>(m_sppedList.at(axis));
        pointData->setAddress(speedAddress);
    }
}

void PointInfo::setDataSize(quint16 size)
{
    m_dataSize = size;
}

void PointInfo::setStyle(int axis, int positionStyle, int speedStyle)
{
    PointData *pointData;
    if (axis < m_positionList.length()) {
        pointData = static_cast<PointData *>(m_positionList.at(axis));
        pointData->setStyle(positionStyle);
    }
    if (axis < m_sppedList.length()) {
        pointData = static_cast<PointData *>(m_sppedList.at(axis));
        pointData->setStyle(speedStyle);
    }
}

void PointInfo::setPosition(int axis, int position)
{
    if (axis < m_positionList.length()) {
        PointData *pointData = static_cast<PointData *>(m_positionList.at(axis));
        pointData->setValue(position);
    }
}

void PointInfo::setPosition(int axis, QString position)
{
    if (axis < m_positionList.length()) {
        PointData *pointData = static_cast<PointData *>(m_positionList.at(axis));
        pointData->setValueStr(position);
    }
}

int PointInfo::getPosition(int axis)
{
    int ret = 0;
    if (axis < m_positionList.length()) {
        auto pointData = static_cast<PointData *>(m_positionList.at(axis));
        ret = pointData->value();
    }
    return ret;
}

void PointInfo::setSpeed(int axis, int spped)
{
    if (axis < m_sppedList.length()) {
        PointData *pointData = static_cast<PointData *>(m_sppedList.at(axis));
        pointData->setValue(spped);
    }
}

int PointInfo::getSpeed(int axis)
{
    int ret = 0;
    if (axis < m_sppedList.length()) {
        auto pointData = static_cast<PointData *>(m_sppedList.at(axis));
        ret = pointData->value();
    }
    return ret;
}

int PointInfo::getPositionAddress(int axis, int &address)
{
    if (axis >= m_positionList.length()) {
        return -1;
    }
    PointData *pointData = static_cast<PointData *>(m_positionList.at(axis));
    address = pointData->address();
    return 0;
}

int PointInfo::getSpeedAddress(int axis, int &address)
{
    if (axis >= m_sppedList.length()) {
        return -1;
    }
    PointData *pointData = static_cast<PointData *>(m_sppedList.at(axis));
    address = pointData->address();
    return 0;
}

int PointInfo::positionAddress(int axis)
{
    if (axis >= m_positionList.length()) {
        return -1;
    }
    PointData *pointData = static_cast<PointData *>(m_positionList.at(axis));
    return pointData->address();
}

int PointInfo::speedAddress(int axis)
{
    if (axis >= m_sppedList.length()) {
        return -1;
    }
    PointData *pointData = static_cast<PointData *>(m_sppedList.at(axis));
    return pointData->address();
}

void PointInfo::setPositionList(QList<QObject *>& list)
{
    if (m_positionList != list) {
        m_positionList = list;
        emit positionListChanged(m_positionList);
    }
}

void PointInfo::setSppedList(QList<QObject *>& list)
{
    if (m_sppedList != list) {
        m_sppedList = list;
        emit sppedListChanged(m_sppedList);
    }
}
