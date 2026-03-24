#include "areasetdata.h"

AreaSetData::AreaSetData(QObject *parent) : QObject(parent)
{

}

void AreaSetData::setValue(int value)
{
    if (m_value != value) {
        m_value = value;
        emit valueChanged(m_value);
    }
}

void AreaSetData::setEnable(int enable)
{
    if (m_enable != enable) {
        m_enable = enable;
        emit enableChanged(m_enable);
    }
}
