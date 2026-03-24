#include "areasetpointinfo.h"

AreaSetPointInfo::AreaSetPointInfo()
{

}

void AreaSetPointInfo::setPointSetting(quint8 value)
{
    if (m_pointSetting != value) {
        m_pointSetting = value;
    }
}

void AreaSetPointInfo::setAxis(quint8 axis)
{
    if (m_axis != axis) {
        m_axis = axis;
    }
}

void AreaSetPointInfo::setPointNoMax(quint8 no)
{
    if (m_pointNoMax != no) {
        m_pointNoMax = no;
    }
}

void AreaSetPointInfo::setPointNoMin(quint8 no)
{
    if (m_pointNoMin != no) {
        m_pointNoMin = no;
    }
}

void AreaSetPointInfo::setMaxHrAddr(quint16 addr)
{
    if (m_maxHrAddr != addr) {
        m_maxHrAddr = addr;
    }
}

void AreaSetPointInfo::setMinHrAddr(quint16 addr)
{
    if (m_minHrAddr != addr) {
        m_minHrAddr = addr;
    }
}
