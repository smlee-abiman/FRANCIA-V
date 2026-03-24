#include "pointdata.h"

PointData::PointData(QObject *parent) : QObject(parent)
{

}

void PointData::setValue(int value)
{
    if (m_value != value) {
        m_value = value;
        emit valueChanged(m_value);
    }
}

void PointData::setStyle(int style)
{
    if (m_style != style) {
        m_style = style;
        emit styleChanged(m_style);
    }
}

void PointData::setAddress(int address)
{
    if (m_address != address) {
        m_address = address;
        emit addressChanged(m_address);
    }
}

void PointData::setValueStr(QString valueStr)
{
    if (m_valueStr != valueStr) {
        m_valueStr = valueStr;
        emit valueStrChanged(m_valueStr);
    }
}
