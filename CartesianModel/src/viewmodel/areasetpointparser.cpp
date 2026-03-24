#include "areasetpointparser.h"
#include "areasetpointinfo.h"

const quint16 INVALID_ADDR = 0xFFFF;
const QHash<int, QList<quint16>> MAX_MIN_ADDR_TABLE = {
// PointNo. 軸1                軸2             軸3             軸4              軸5             軸6                 軸7                  軸8
    {253,   {8656,            8659,           8662,           8665,           8668,           8671,               8674,               8677        } },    // 最大移動値
    {231,   {8128,            8131,           8134,           INVALID_ADDR,   INVALID_ADDR,   INVALID_ADDR,       INVALID_ADDR,       INVALID_ADDR} },    // 取出側最小値
    {221,   {7888,            7891,           7894,           7897,           7900,           INVALID_ADDR,       INVALID_ADDR,       INVALID_ADDR} },    // 取出側最大値/下降最大値
    {223,   {INVALID_ADDR,    INVALID_ADDR,   INVALID_ADDR,   7945,           7948,           INVALID_ADDR,       INVALID_ADDR,       INVALID_ADDR} },    // 取出側最大値
    {232,   {8152,            8155,           8158,           INVALID_ADDR,   INVALID_ADDR,   INVALID_ADDR,       INVALID_ADDR,       INVALID_ADDR} },    // 開放側最小値
    {222,   {7912,            7915,           7918,           7921,           7924,           INVALID_ADDR,       INVALID_ADDR,       INVALID_ADDR} },    // 開放側最大値
};

const quint8 INVALID_POINT = 0xFF;

AreaSetPointParser::AreaSetPointParser(QObject *parent) : QObject(parent)
{

}

void AreaSetPointParser::parse(AreaSetPointInfo *info, quint8 pointSetting, quint8 axis)
{
    info->setAxis(axis);
    info->setPointSetting(pointSetting);
    info->setPointNoMax(parsePointMaxSetting(pointSetting));
    info->setMaxHrAddr(parseHrAddr(axis, info->pointNoMax()));
    info->setPointNoMin(parsePointMinSetting(pointSetting));
    info->setMinHrAddr(parseHrAddr(axis, info->pointNoMin()));
}

quint16 AreaSetPointParser::parseHrAddr(quint8 axis, quint8 pointNo)
{
    quint16 ret = INVALID_ADDR;
    if (MAX_MIN_ADDR_TABLE.contains(pointNo)) {
        QList<quint16> list = MAX_MIN_ADDR_TABLE.value(pointNo);
        if (axis < list.length())
            ret = list.at(axis);
    }
    return ret;
}

quint8 AreaSetPointParser::parsePointMaxSetting(quint8 point)
{
    quint8 pointMaxSetting = getPointMaxSetting(point);
    const QList<quint8> POINT_NO_MAX_TABLE = {
        219,
        220,
        221,
        222,
        223,
        224,
        225,
        226,
        227,
        228,
        229,
        253,
        INVALID_POINT,
    };

    if (pointMaxSetting >= POINT_NO_MAX_TABLE.length())
        return INVALID_POINT;

    return POINT_NO_MAX_TABLE.at(pointMaxSetting);
}

quint8 AreaSetPointParser::parsePointMinSetting(quint8 point)
{
    quint8 pointMinSetting = getPointMinSetting(point);
    const QList<quint8> POINT_NO_MIN_TABLE = {
        230,
        231,
        232,
        233,
        234,
        235,
        236,
        237,
        238,
        239,
        240,
        INVALID_POINT,
        INVALID_POINT,
    };

    if (pointMinSetting >= POINT_NO_MIN_TABLE.length())
        return INVALID_POINT;

    return POINT_NO_MIN_TABLE.at(pointMinSetting);
}

int AreaSetPointParser::getMaxValue(AreaSetPointInfo *info)
{
    int ret = 0;
    int pointMaxSetting = getPointMaxSetting(info->pointSetting());
    if (pointMaxSetting == 0x0C) {
        ret = 80000000;
    } else {
        quint16 maxHrAddr = info->maxHrAddr();
        if (maxHrAddr != INVALID_ADDR)
            ret = m_modbus->getLongReceiveDataHR(maxHrAddr);
    }
//    qDebug() << __PRETTY_FUNCTION__ << "pointMaxSetting:" <<  QString::number(info->pointSetting(), 16) << "maxHrAddr:" << info->maxHrAddr() << "value:" << ret;
    return ret;
}

int AreaSetPointParser::getMinValue(AreaSetPointInfo *info)
{
    int ret = 0;
    int pointMinSetting = getPointMinSetting(info->pointSetting());
    if (pointMinSetting == 0x0B) {
        ret = 0;
    } else if (pointMinSetting == 0x0C) {
        ret = -80000000;
    } else {
        quint16 minHrAddr = info->minHrAddr();
        if (minHrAddr != INVALID_ADDR)
            ret = m_modbus->getLongReceiveDataHR(minHrAddr);
    }
//    qDebug() << __PRETTY_FUNCTION__ << "pointMinSetting:" <<  QString::number(info->pointSetting(), 16) << "minHrAddr:" << info->minHrAddr() << "value:" << ret;
    return ret;
}

QList<quint16> AreaSetPointParser::addressList()
{
    QList<quint16> addressList;
    QHashIterator<int, QList<quint16>> i(MAX_MIN_ADDR_TABLE);
    while (i.hasNext()) {
        i.next();
        QList<quint16> list = i.value();
        for (quint16 addr : list) {
            if (addr == INVALID_ADDR)
                continue;
            addressList.append(addr);
        }
    }
    return addressList;
}
