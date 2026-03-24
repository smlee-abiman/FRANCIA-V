#include "corelibmodbusdatamanager.h"
#include <QDebug>

CoreLibModBusDataManager::CoreLibModBusDataManager(QObject *parent)
    : QObject(parent)
{
    mMutexReadModBus = new QMutex(QMutex::Recursive);
    mMutexWriteModBus = new QMutex(QMutex::Recursive);
}

CoreLibModBusDataManager::~CoreLibModBusDataManager()
{
    if (mMutexReadModBus){
        mMutexReadModBus->unlock();
        delete mMutexReadModBus;
        mMutexReadModBus = nullptr;
    }
    if (mMutexWriteModBus){
        mMutexWriteModBus->unlock();
        delete mMutexWriteModBus;
        mMutexWriteModBus = nullptr;
    }
}

QModbusDataUnit::RegisterType CoreLibModBusDataManager::getRegisterType(QModbusPdu::FunctionCode funcCode)
{
    switch(funcCode){
    case QModbusPdu::FunctionCode::ReadCoils:
        return QModbusDataUnit::RegisterType::Coils;
    case QModbusPdu::FunctionCode::ReadDiscreteInputs:
        return QModbusDataUnit::RegisterType::DiscreteInputs;
    case QModbusPdu::FunctionCode::ReadHoldingRegisters:
        return QModbusDataUnit::RegisterType::HoldingRegisters;
    case QModbusPdu::FunctionCode::ReadInputRegisters:
        return QModbusDataUnit::RegisterType::InputRegisters;
    case QModbusPdu::FunctionCode::WriteSingleCoil:
    case QModbusPdu::FunctionCode::WriteMultipleCoils:
        return QModbusDataUnit::RegisterType::Coils;
    case QModbusPdu::FunctionCode::WriteSingleRegister:
    case QModbusPdu::FunctionCode::WriteMultipleRegisters:
        return QModbusDataUnit::RegisterType::HoldingRegisters;
    default:
        qCritical("Not supported FuncCode: 0x%02x", funcCode);
        return QModbusDataUnit::RegisterType::Invalid;
    }
}

QModbusDataUnit* CoreLibModBusDataManager::getModBudDataUnit(ModbusRequestType reqType)
{
    if (reqType == ModbusRequestType::ModbusRequestTypeRead){
        return &mReadModBusDataUnit;
    } else {
        return &mWriteModBusDataUnit;
    }
}
#if 0
void CoreLibModBusDataManager::getModBusMemoryData(int address, size_t size, char* dest)
{
    memcpy(dest, &mModBusMemoryData[address], size);
}
void CoreLibModBusDataManager::setModBusMemoryData(ushort address, char* val, size_t size)
{
    memcpy(&mModBusMemoryData[address], val, size);

    emit dataChanged(address, size);
}
#endif

void CoreLibModBusDataManager::lockModBusMutex(ModbusRequestType reqType)
{
    switch(reqType){
    case ModbusRequestTypeRead:
        mMutexReadModBus->lock();
        break;
    case ModbusRequestTypeWrite:
        mMutexWriteModBus->lock();
        break;
    case ModbusRequestTypeReadWrite:
        mMutexReadModBus->lock();
        mMutexWriteModBus->lock();
        break;
    default:
        break;
    }
}
void CoreLibModBusDataManager::unlockModBusMutex(ModbusRequestType reqType)
{
    switch(reqType){
    case ModbusRequestTypeRead:
        mMutexReadModBus->unlock();
        break;
    case ModbusRequestTypeWrite:
        mMutexWriteModBus->unlock();
        break;
    case ModbusRequestTypeReadWrite:
        mMutexWriteModBus->unlock();
        mMutexReadModBus->unlock();
        break;
    default:
        break;
    }
}

QBitArray* CoreLibModBusDataManager::getModBusBitMemoryData()
{
    return &m_modBusBitMemoryData;
}

stModBusCommunicationData* CoreLibModBusDataManager::getModBusComData()
{
    return &m_modBusMemData;
}

void CoreLibModBusDataManager::setModBusComData(stModBusCommunicationData &modBusMemData)
{
    m_modBusMemData = modBusMemData;
}

