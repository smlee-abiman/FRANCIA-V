#include "axispointinfo.h"
#include "resourcemanager.h"

const quint16 POSITION_TOP_HR_ADDR          = 2608;
const quint16 POINT_DATA_SIZE               = 24;
const int POINT_FIX_START                   = 1;
const int POINT_FIX_END                     = 80;
const int POINT_STANDARD_PALLETIZE_START    = 201;
const int POINT_STANDARD_PALLETIZE_END      = 208;
const int POINT_FREE_PALLETIZE_START        = 211;
const int POINT_FREE_PALLETIZE_END          = 218;
const int POINT_START                       = 1;
const int POINT_END                         = 999;

AxisPointInfo::AxisPointInfo(ResourceManager *resource, QObject *parent) : QObject(parent),
    m_resource(resource)
{


}

AxisPointInfo* AxisPointInfo::create_instance(ResourceManager *resource, QObject *parent)
{
    if (m_instance == nullptr)
        m_instance = new AxisPointInfo(resource, parent);
    return m_instance;
}

AxisPointInfo* AxisPointInfo::get_instance()
{
    return m_instance;
}

void AxisPointInfo::init()
{
}

bool AxisPointInfo::isValidPoint(int pointNo)
{
    return (pointNo >= POINT_START) && (pointNo <= POINT_END);
}

int AxisPointInfo::getStartFixPointNo()
{
    return POINT_FIX_START;
}

int AxisPointInfo::getEndFixPointNo()
{
    return POINT_FIX_END;
}

int AxisPointInfo::getStartStandardPalletizePointNo()
{
    return POINT_STANDARD_PALLETIZE_START;
}

int AxisPointInfo::getEndStandardPalletizePointNo()
{
    return POINT_STANDARD_PALLETIZE_END;
}

int AxisPointInfo::getStartFreePalletizePointNo()
{
    return POINT_FREE_PALLETIZE_START;
}

int AxisPointInfo::getEndFreePalletizePointNo()
{
    return POINT_FREE_PALLETIZE_END;
}

int AxisPointInfo::getStandardPalletizePointNo(int palletizeNo)
{
    return POINT_STANDARD_PALLETIZE_START + palletizeNo - 1;
}

int AxisPointInfo::getFreePalletizePointNo(int palletizeNo)
{
    return POINT_FREE_PALLETIZE_START + palletizeNo - 1;
}

int AxisPointInfo::getPalletizeNo(int pointNo)
{
    int ret = 0;
    if ((pointNo >= POINT_STANDARD_PALLETIZE_START) && (pointNo <= POINT_STANDARD_PALLETIZE_END)) {
        ret = pointNo - POINT_STANDARD_PALLETIZE_START + 1;
    } else if ((pointNo >= POINT_FREE_PALLETIZE_START) && (pointNo <= POINT_FREE_PALLETIZE_END)) {
        ret = pointNo - POINT_FREE_PALLETIZE_START + 9;
    }
    return ret;
}

AxisPointInfo::PointType AxisPointInfo::getPointType(int pointNo)
{
    PointType ret = PointType::UnKnown;
    if (pointNo < POINT_FIX_START) {
        ret = PointType::UnKnown;
    } else if (pointNo <= POINT_FIX_END) {
        ret = PointType::Fix;
    } else if (pointNo <= 100) {
        ret = PointType::UnKnown;
    } else if (pointNo < POINT_STANDARD_PALLETIZE_START) {
        ret = PointType::Arbitrary;
    } else if (pointNo <= POINT_STANDARD_PALLETIZE_END) {
        ret = PointType::StandardPalletize;
    } else if (pointNo < POINT_FREE_PALLETIZE_START) {
        ret = PointType::UnKnown;
    } else if (pointNo <= POINT_FREE_PALLETIZE_END) {
        ret = PointType::FreePalletize;
    } else if (pointNo <= 255) {
        ret = PointType::UnKnown;
    } else if (pointNo <= 999) {
        ret = PointType::Arbitrary;
    }
    return ret;
}

int AxisPointInfo::formatArbitraryPointPosition()
{
    return m_resource->getPenSetIntValue("16", 414, 0);
}

int AxisPointInfo::formatArbitraryPointSpeed()
{
    return m_resource->getPenSetIntValue("16", 414, 1);
}

quint16 AxisPointInfo::getPointPositionAddress(int pointNo, int axis)
{
    return POSITION_TOP_HR_ADDR + (static_cast<quint16>(pointNo) - 1) * 3 * 8 + static_cast<quint16>(axis) * 3;
}

quint16 AxisPointInfo::getPointSpeedAddress(int pointNo, int axis)
{
    return getPointPositionAddress(pointNo, axis) + 2;
}

quint16 AxisPointInfo::positionTopHrAddr() const
{
    return POSITION_TOP_HR_ADDR;
}

quint16 AxisPointInfo::pointDataSize() const
{
    return POINT_DATA_SIZE;
}

QString AxisPointInfo::getPointName(int pointNo)
{
    int labelIndex = 0;
    if ((pointNo >= POINT_FIX_START) && (pointNo <= POINT_FIX_END)) {
        labelIndex = 29 + pointNo;
    } else if ((pointNo >= POINT_STANDARD_PALLETIZE_START) && (pointNo <= POINT_STANDARD_PALLETIZE_END)) {
        labelIndex = 110 + pointNo - POINT_STANDARD_PALLETIZE_START;
    } else {
        return "";
    }
    return m_resource->label(36, labelIndex);
}

AxisPointInfo* AxisPointInfo::m_instance = nullptr;
