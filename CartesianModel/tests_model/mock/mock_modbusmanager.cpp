#include "mock_modbusmanager.h"

ModbusManager* ModbusManager::m_self = nullptr;

ModbusManager::ModbusManager(){

}

ModbusManager* ModbusManager::get_instance(){
    if (m_self == nullptr) {
        m_self = new ModbusManager();
    }
    return  m_self;
}

void ModbusManager::instanceRenew()
{
    if (m_self != nullptr) {
        delete m_self;
        m_self = new ModbusManager();
    }
}

bool ModbusManager::getReceiveDataDI(quint16 key) {
    if (m_receiveDataMapDI.contains(key)) {
        return m_receiveDataMapDI.take(key);
    }
    return false;
}

bool ModbusManager::getReceiveDataDO(quint16 key) {
    if (m_receiveDataMapDO.contains(key)) {
        return m_receiveDataMapDO.take(key);
    }
    return false;
}

int ModbusManager::getReceiveDataHR(quint16 key) {
    if (m_receiveDataMapHR.contains(key)) {
        return m_receiveDataMapHR.take(key);
    }
    return 0;
}

int ModbusManager::getReceiveDataRG(quint16 key) {
    Q_UNUSED(key)

    return m_getReceiveDataRGReturnValue;
}

int ModbusManager::getLongReceiveDataRG(quint16 key) {
    if (m_longReceiveDataRG.contains(key)) {
        return m_longReceiveDataRG.take(key);
    }
    return 0;
}
int ModbusManager::getLongReceiveDataHR(quint16 key) {
    if (m_longReceiveDataHR.contains(key)) {
        return m_longReceiveDataHR.take(key);
    }
    return 0;
}
