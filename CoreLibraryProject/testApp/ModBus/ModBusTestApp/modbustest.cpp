#include "modbustest.h"

ModBusTest::ModBusTest(QObject *parent)
    : QObject(parent),
      mModBusInitInfo{"robot1", "127.0.0.1", 502, 1, 450, 1}
{
    mCoreLibModBus = new CoreLibModBusCommunication(mModBusInitInfo);
}

void ModBusTest::modBusReadConnect()
{
    bool isBitMemory = mCoreLibModBus->checkModBusBitMemory(mModBusComData);
    mCoreLibModBus->readModBusData(mModBusComData);
    CoreLibModBusDataManager *mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    if (isBitMemory){
        connect(mModBusDataMgr, SIGNAL(modBusBitMemDataChanged(QBitArray)),
            this, SLOT(modBusBitMemoryDataChanged(QBitArray)));
    } else {
        connect(mModBusDataMgr, SIGNAL(modBusMemDataChanged(stModBusCommunicationData)),
            this, SLOT(modBusMemoryDataChanged(stModBusCommunicationData)));
    }
    connect(mCoreLibModBus, SIGNAL(modBusComReadStatusChanged(int)),
        this, SLOT(modBusReadStatusChanged(int)));
}

void ModBusTest::connectSlot()
{
    mCoreLibModBus->startModBusCommunication();
    connect(mCoreLibModBus, SIGNAL(modBusComConnectStatusChanged(int)),
        this, SLOT(modBusConnectStatusChanged(int)));
}

void ModBusTest::syncConnectSlot()
{
    bool ret = mCoreLibModBus->syncStartModBusCommunication();
    qDebug() << "syncStartModBusCommunication() ret:" << ret;
}

void ModBusTest::readBitSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::ReadCoils;
    mModBusComData.readAddress = 0x0000;
    mModBusComData.comSize = 4;
    mCoreLibModBus->readModBusData(mModBusComData);
    modBusReadConnect();
}

void ModBusTest::readByteSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    mModBusComData.readAddress = 0x02A0;
    mModBusComData.comSize = 8;
    mCoreLibModBus->readModBusData(mModBusComData);
    modBusReadConnect();
}

void ModBusTest::readWordSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    mModBusComData.readAddress = 0x04A0;
    mModBusComData.comSize = 32;
    mCoreLibModBus->readModBusData(mModBusComData);
    modBusReadConnect();
}

void ModBusTest::readLongSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    mModBusComData.readAddress = 0x2C80;
    mModBusComData.comSize = 64;
    mCoreLibModBus->readModBusData(mModBusComData);
    modBusReadConnect();
}

void ModBusTest::readStringSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    mModBusComData.readAddress = 0x3280;
    mModBusComData.comSize = 6;
    mCoreLibModBus->readModBusData(mModBusComData);
    modBusReadConnect();
}

void ModBusTest::writeBitSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::WriteSingleCoil;
    mModBusComData.writeAddress = 0x0000;
    mModBusComData.comSize = 4;
    for (int i = 0; i < mModBusComData.comSize; i++){
        mModBusComData.writeBuf[i] = 0;
    }
    mCoreLibModBus->writeModBusData(mModBusComData);
    readBitSlot();
}

void ModBusTest::writeByteSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    mModBusComData.writeAddress = 0x02A0;
    mModBusComData.comSize = 8;
    for (int i = 0; i < mModBusComData.comSize; i++){
        mModBusComData.writeBuf[i] = 0;
    }
    mCoreLibModBus->writeModBusData(mModBusComData);
    readByteSlot();
}

void ModBusTest::writeWordSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    mModBusComData.writeAddress = 0x04A0;
    mModBusComData.comSize = 32;
    for (int i = 0; i < mModBusComData.comSize; i++){
        mModBusComData.writeBuf[i] = 0;
    }
    mCoreLibModBus->writeModBusData(mModBusComData);
    readWordSlot();
}

void ModBusTest::writeLongSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    mModBusComData.writeAddress = 0x2C80;
    mModBusComData.comSize = 64;
    for (int i = 0; i < mModBusComData.comSize; i++){
        mModBusComData.writeBuf[i] = 0;
    }
    mCoreLibModBus->writeModBusData(mModBusComData);
    readLongSlot();
}

void ModBusTest::writeStringSlot()
{
    mModBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    mModBusComData.writeAddress = 0x3280;
    mModBusComData.comSize = 6;
    for (int i = 0; i < mModBusComData.comSize; i++){
        mModBusComData.writeBuf[i] = 0;
    }
    mCoreLibModBus->writeModBusData(mModBusComData);
    readStringSlot();
}


void ModBusTest::disconnectSlot()
{
    mCoreLibModBus->stopModBusCommunication();
    connect(mCoreLibModBus, SIGNAL(modBusComConnectStatusChanged(int)),
        this, SLOT(modBusConnectStatusChanged(int)));
}

void ModBusTest::syncDisconnectSlot()
{
    bool ret = mCoreLibModBus->syncStopModBusCommunication();
    qDebug() << "syncStopModBusCommunication() ret:" << ret;
}

void ModBusTest::initializeSlot()
{
#ifdef GET_POINT_MEMORY_HEADER_INFO_FROM_CORELIB
    mCoreLibModBus->getPointHeaderSize();
#endif // GET_POINT_MEMORY_HEADER_INFO_FROM_CORELIB
}

void ModBusTest::modBusConnectStatusChanged(int state)
{
    qDebug() << "modBusConnectStatusChanged() state:" << state;
    disconnect(mCoreLibModBus, SIGNAL(modBusComConnectStatusChanged(int)),
        this, SLOT(modBusConnectStatusChanged(int)));
}

void ModBusTest::modBusReadStatusChanged(int state)
{
    qDebug() << "modBusReadStatusChanged() state:" << state;
    disconnect(mCoreLibModBus, SIGNAL(modBusComReadStatusChanged(int)),
        this, SLOT(modBusReadStatusChanged(int)));

}
void ModBusTest::modBusBitMemoryDataChanged(QBitArray bitMemoryData)
{
    int size = bitMemoryData.size();
    mModBusComData.bitReadBuf.resize(size);
    for (int i = 0; i < size; i++){
        mModBusComData.bitReadBuf[i] = bitMemoryData[i];
        //qDebug() << "bitReadBuf: " << mModBusComData.bitReadBuf[i];
        bool value = mModBusComData.bitReadBuf.at(i);
        qDebug("bitReadBuf:[%d]", value);
    }
    CoreLibModBusDataManager *mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    disconnect(mModBusDataMgr, SIGNAL(modBusBitMemDataChanged(QBitArray)),
        this, SLOT(modBusBitMemoryDataChanged(QBitArray)));
}
void ModBusTest::modBusMemoryDataChanged(stModBusCommunicationData modBusMemData)
{
    qDebug() << "address: " << modBusMemData.readAddress;
    qDebug() << "funcCode: " << modBusMemData.funcCode;
    qDebug() << "comSize: " << modBusMemData.comSize;

    int size = (int)modBusMemData.comSize;
    for (int i = 0; i < size; i++){
        qDebug("readData[%d]=[0x%04x]", i, modBusMemData.readBuf[i]);
    }
    CoreLibModBusDataManager *mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    disconnect(mModBusDataMgr, SIGNAL(modBusMemDataChanged(stModBusCommunicationData)),
        this, SLOT(modBusMemoryDataChanged(stModBusCommunicationData)));
}
