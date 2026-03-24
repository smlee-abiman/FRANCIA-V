#ifndef AREASETPOINTPARSER_H
#define AREASETPOINTPARSER_H

#include <QObject>
#include "modbusmanager.h"

class AreaSetPointInfo;

class AreaSetPointParser : public QObject
{
    Q_OBJECT
public:
    explicit AreaSetPointParser(QObject *parent = nullptr);
    void parse(AreaSetPointInfo *info, quint8 pointSetting, quint8 axis);
    quint8 parsePointMaxSetting(quint8 point);
    quint8 parsePointMinSetting(quint8 point);
    quint16 parseHrAddr(quint8 axis, quint8 pointNo);
    int getMaxValue(AreaSetPointInfo *info);
    int getMinValue(AreaSetPointInfo *info);
    inline quint8 getPointMaxSetting(quint8 pointSetting) { return ((pointSetting >> 4) & 0x0F); }
    inline quint8 getPointMinSetting(quint8 pointSetting) { return (pointSetting & 0x0F); }

    QList<quint16> addressList();

private:
    ModbusManager *m_modbus = ModbusManager::get_instance();
};

#endif // AREASETPOINTPARSER_H
