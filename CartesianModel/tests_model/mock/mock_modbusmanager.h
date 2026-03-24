#ifndef MODBUSMANAGER_H
#define MODBUSMANAGER_H

#include <QDateTime>
#include <QDebug>

class ModbusManager{
public:
    static ModbusManager* get_instance();
    static void instanceRenew();
    bool getReceiveDataDI(quint16 key);
    bool getReceiveDataDO(quint16 key);
    int getReceiveDataHR(quint16 key);
    int getReceiveDataRG(quint16 key);
    int getLongReceiveDataRG(quint16 key);
    int getLongReceiveDataHR(quint16 key);

    int m_getReceiveDataRGReturnValue = 0;

public:
    QMap<quint16, bool> m_receiveDataMapDI;
    QMap<quint16, bool> m_receiveDataMapDO;
    QMap<quint16, int>  m_receiveDataMapHR;
    QMap<quint16, int>  m_longReceiveDataRG;
    QMap<quint16, int>  m_longReceiveDataHR;

private:
    ModbusManager();
    static ModbusManager* m_self;

};

#endif // MODBUSMANAGER_H
