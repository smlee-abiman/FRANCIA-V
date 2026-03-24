#include "modbuscommunication.h"

ModbusCommunication::ModbusCommunication(QObject *parent)
    : QObject(parent),
      mModbusCom(nullptr),
      mIsModbusConnect(false),
      mModBusComConnectStatus(0)
{
    initialize();
}

void ModbusCommunication::modbusCommunicationTerminate()
{
    if (mIsModbusConnect) {
        mModbusCom->stopModBusCommunication();
        mIsModbusConnect = false;
    }
    delete mModbusCom;
}

void ModbusCommunication::modbusCommunicationInit(stModBusInitInfo& modbusInitInfo)
{
    mModbusInitInfo = modbusInitInfo;
    if (!mModbusCom){
        mModbusCom = new CoreLibModBusCommunication(mModbusInitInfo);
    }
}

void ModbusCommunication::modbusCommunicationStart()
{
    if (!mModbusCom){
        return;
    }
    connect(mModbusCom, SIGNAL(modBusComConnectStatusChanged(int)),
        this, SLOT(onModBusComConnectStatusChanged(int)), Qt::QueuedConnection);
    mModbusCom->startModBusCommunication();
}

void ModbusCommunication::syncModbusCommunicationStart()
{
    if (!mModbusCom){
        return;
    }
    connect(mModbusCom, SIGNAL(modBusComConnectStatusChanged(int)),
        this, SLOT(onModBusComConnectStatusChanged(int)), Qt::QueuedConnection);
    mModbusCom->syncStartModBusCommunication();
}

void ModbusCommunication::syncModbusCommunicationStop()
{
    if (!mModbusCom){
        return;
    }
    mModbusCom->syncStopModBusCommunication();
    disconnect(mModbusCom, SIGNAL(modBusComConnectStatusChanged(int)),
        this, SLOT(onModBusComConnectStatusChanged(int)));
}

void ModbusCommunication::modbusCommunicationStop()
{
    if (!mModbusCom){
        return;
    }
    if (mIsModbusConnect){
        mModbusCom->stopModBusCommunication();
    }
}

int ModbusCommunication::getModbusConnectStatus()
{
    return mModBusComConnectStatus;
}

bool ModbusCommunication::writeModbusData(stModBusCommunicationData &command)
{
    bool ret = false;

    if (!mModbusCom){
        qDebug() << "mModbusCom is Null";
        return false;
    }
    if(mModBusComConnectStatus == QModbusDevice::ConnectedState) {
        ret = mModbusCom->syncWriteModBusData(&command);
        if (ret){
            return ret;
        } else {
            qDebug() << "syncWriteModBusData failed:" << ret;
        }
    }
    else{
#ifndef REAL_TARGET
        ret = mModbusCom->syncWriteModBusDebugData(&command);
        if (ret){
            return ret;
        } else {
            qDebug() << "syncWriteModBusDebugData failed:" << ret;
        }
#else
        if(mModBusComConnectStatus != QModbusDevice::ConnectedState) {
            qDebug() << "mModBusComConnectStatus not connected" << mModBusComConnectStatus;
            return false;
        }
#endif
    }
    return ret;
}
bool ModbusCommunication::readModbusData(stModBusCommunicationData &command)
{
    bool ret = false;

    if (!mModbusCom){
        qDebug() << "mModbusCom is Null";
        return false;
    }
    if(mModBusComConnectStatus == QModbusDevice::ConnectedState) {

        ret = mModbusCom->syncReadModBusData(&command);
        if (ret){
            return ret;
        } else {
            qDebug() << "syncReadModBusData failed:" << ret;
        }
    }
    else{
#ifndef REAL_TARGET
        ret = mModbusCom->syncReadModBusDebugData(&command);
        if (ret){
            return ret;
        } else {
            qDebug() << "syncReadModBusDebugData failed:" << ret;
        }
#else
        if(mModBusComConnectStatus != QModbusDevice::ConnectedState) {
            qDebug() << "mModBusComConnectStatus not connected" << mModBusComConnectStatus;
            return false;
        }
#endif
    }
    return ret;
}

bool ModbusCommunication::modbusWriteDO(int doNumber, bool isOn)
{
    bool ret = true;
    bool is_ok = false;
    int writeAddress;
    QString result = QString::number( doNumber, 16 );

    writeAddress = result.toInt(&is_ok, 16);
    if (!is_ok){
        qDebug("%s:%d result is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return false;
    }
    QList<int> writeData;
    if(isOn) {
        writeData.append(0xFF);
    } else {
        writeData.append(0x00);
    }
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteSingleCoil, writeAddress, 0x0001, writeData);
    ret = writeModbusData(command);
    if (!ret){
        qDebug("%s:%d writeModbusData failed ret[%d]", __func__, __LINE__, ret);
    }
    return ret;
}

bool ModbusCommunication::asyncModbusWriteDO(int doNumber, uchar anyValue, int write_size)
{
    bool ret = false, is_ok = false;
    int writeAddress;
    QString result = QString::number( doNumber, 16 );

    if (!mModbusCom){
        qDebug() << "mModbusCom is Null";
        return false;
    }
    writeAddress = result.toInt(&is_ok, 16);
    if (!is_ok){
        qDebug("%s:%d result is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return is_ok;
    }
    QList<int> writeData;
    for(int i = 0; i < write_size; i++){
        writeData.append((int)((anyValue >> i) & 0x01));
    }
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, writeAddress, write_size, writeData);
    ret = mModbusCom->writeModBusData(command);
    if (!ret){
        qDebug("%s:%d writeModBusData ret[%d]", __func__, __LINE__, ret);
    }
    return ret;
}

bool ModbusCommunication::modbusWriteHR(int HrNumber, int write_data)
{
    bool ret = false, is_ok = false;
    int writeAddress = 0;
    QString result = QString::number( HrNumber, 16 );

    writeAddress = result.toInt(&is_ok, 16);
    //qDebug("%s:%d HrNumber[%d] write_data[%d] writeAddress[0x%04x]", __func__, __LINE__, HrNumber, write_data, writeAddress);
    if (!is_ok){
        qDebug("%s:%d result is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return is_ok;
    }
    QList<int> writeData;
    writeData.append(write_data);
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteSingleRegister, writeAddress, 0x0001, writeData);
    ret = writeModbusData(command);
    if (!ret){
        qDebug("%s:%d writeModbusData failed ret[%d]", __func__, __LINE__, ret);
    }
    return ret;
}
bool ModbusCommunication::modbusWriteHR(int HrNumber, int *write_data, int write_size)
{
    bool is_ok = false, ret = false;
    int writeAddress = 0;
    QString result = QString::number( HrNumber, 16 );

    if (!write_data){
        return false;
    }
    writeAddress = result.toInt(&is_ok, 16);
    //qDebug("%s:%d HrNumber[%d] writeAddress[0x%04x]", __func__, __LINE__, HrNumber, writeAddress);
    if (!is_ok){
        qDebug("%s:%d result is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return false;
    }
    QList<int> writeData;
    for (int i = 0; i < write_size; i++){
        writeData.append(write_data[i]);
    }
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, writeAddress, write_size, writeData);
    ret = writeModbusData(command);
    if (!ret){
        qDebug("%s:%d writeModbusData failed ret[%d]", __func__, __LINE__, ret);
    }
    return ret;
}
bool ModbusCommunication::modbusWriteHR(int HrNumber, QString writeDataStr)
{
    bool is_ok = false, ret = false;;
    int writeAddress = 0, writeSize = 0;

    if (writeDataStr == ""){
        qDebug() << "writeDataStr is Empty:" << writeDataStr;
        return false;
    }
    QString str = QString::number( HrNumber, 16 );
    writeAddress = str.toInt(&is_ok, 16);
    if (!is_ok){
        qDebug("%s:%d str is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return false;
    }
    writeSize = writeDataStr.length();
    if (!is_ok){
        qDebug("%s:%d writeData is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return false;
    }
    QList<int> writeData;
    int writeDataAscii = 0;
    for(int i = 0; i < writeSize; i++){
        if (writeDataStr.at(i).unicode() == 0x20){
            writeDataAscii = 0x00;
        } else {
            writeDataAscii = QString::number(writeDataStr.at(i).unicode(), 16).toInt(&is_ok, 16);
            if (!is_ok){
                qDebug("%s:%d writeDataStr is not num is_ok[%d]", __func__, __LINE__, is_ok);
                continue;
            }
        }
        //qDebug() << "writeDataAscii" << writeDataAscii;
        writeData.append(writeDataAscii);
    }
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleRegisters, writeAddress, writeSize, writeData);
    ret = writeModbusData(command);
    if (!ret){
        qDebug("%s:%d writeModbusData failed ret[%d]", __func__, __LINE__, ret);
    }
    return ret;
}
bool ModbusCommunication::modbusReadDI()
{
    bool ret = true;
    return ret;
}
bool ModbusCommunication::modbusReadHR(int HrNumber, int read_size, stModBusCommunicationData* modbusComData)
{
    bool is_ok = false, ret = false;
    int readAddress = 0;

    if (!modbusComData){
        qDebug("%s:%d modbusComData is null", __func__, __LINE__);
        return false;
    }
    QString str = QString::number( HrNumber, 16 );
    readAddress = str.toInt(&is_ok, 16);
    if (!is_ok){
        qDebug("%s:%d str is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return false;
    }
    *modbusComData =
            ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadHoldingRegisters, readAddress, read_size);
    ret = readModbusData(*modbusComData);
    if (!ret){
        qDebug("%s:%d readModbusData failed ret[%d]", __func__, __LINE__, ret);
    }
    return ret;
}
bool ModbusCommunication::modbusReadRG(int RgNumber, int read_size, stModBusCommunicationData* modbusComData)
{
    bool is_ok = false, ret = false;
    int readAddress = 0;

    if (!modbusComData){
        qDebug("%s:%d modbusComData is null", __func__, __LINE__);
        return false;
    }
    QString str = QString::number( RgNumber, 16 );
    readAddress = str.toInt(&is_ok, 16);
    if (!is_ok){
        qDebug("%s:%d str is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return false;
    }
    *modbusComData =
            ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadInputRegisters, readAddress, read_size);
    ret = readModbusData(*modbusComData);
    if (!ret){
        qDebug("%s:%d readModbusData failed ret[%d]", __func__, __LINE__, ret);
    }
    return ret;
}

void ModbusCommunication::onModBusComConnectStatusChanged(int state)
{
    qDebug() << "ModbusCommunication::onModBusComConnectStatusChanged: state = " << state;
    mModBusComConnectStatus = state;
    if(mModBusComConnectStatus == QModbusDevice::ConnectedState){
        mIsModbusConnect = true;
    }
}

void ModbusCommunication::initialize()
{
    mModbusInitInfo.aplInfo = "";
    mModbusInitInfo.portNum = 0;
    mModbusInitInfo.serverAddress = 0;
    mModbusInitInfo.serverIPAddress = "";
}

ModbusCommunication* ModbusCommunication::instance = nullptr;
