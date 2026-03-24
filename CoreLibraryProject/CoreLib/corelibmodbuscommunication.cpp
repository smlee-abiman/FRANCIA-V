#include "corelibmodbuscommunication.h"
#include <QDebug>
#include <QEventLoop>
#ifdef CORELIB_UNIT_TEST
#define MODBUS_CONNECT_DEVICE mMockMethod._mock_modbus_connect
#define MODBUS_SEND_READ_REQUEST mMockMethod._mock_modbus_send_read_request
#define MODBUS_SEND_WRITE_REQUEST mMockMethod._mock_modbus_send_write_request
#else // CORELIB_UNIT_TEST
#define MODBUS_CONNECT_DEVICE mModBusDevice->connectDevice
#define MODBUS_SEND_READ_REQUEST mModBusDevice->sendReadRequest
#define MODBUS_SEND_WRITE_REQUEST mModBusDevice->sendWriteRequest
#endif // CORELIB_UNIT_TEST

CoreLibModBusCommunication::CoreLibModBusCommunication(stModBusInitInfo &modBusInitInfo)
    : mModBusDevice(nullptr),
      mIsBitMemory(false),
      mModBusErrCount(0),
      mModBusErrCountFile(nullptr)
#ifdef REAL_TARGET
#else // REAL_TARGET
      ,mQDir(nullptr)
#endif // REAL_TARGET
{
    mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    if (mModBusDevice){
        mModBusDevice->disconnectDevice();
        delete mModBusDevice;
        mModBusDevice = nullptr;
    }
#ifdef REAL_TARGET
#else // REAL_TARGET
    if (!mQDir){
        mQDir = new QDir();
    }
#endif // REAL_TARGET
    mModBusDevice = new QModbusTcpClient();
    mServerUrl = modBusInitInfo.serverIPAddress+":"+QString::number(modBusInitInfo.portNum);
    mModBusServerAddress = modBusInitInfo.serverAddress;
    mAplInfo = modBusInitInfo.aplInfo;
    mModBusTimeout = modBusInitInfo.timeout;
    mModBusRetryCnt = modBusInitInfo.retry;
    readyModBusErrorCountFile(modBusInitInfo.aplInfo);
    //connect(mModBusDevice, &QModbusClient::stateChanged,
    //    this, &CoreLibModBusCommunication::onStateChanged);
}

CoreLibModBusCommunication::~CoreLibModBusCommunication()
{
    if (mModBusErrCountFile){
        mModBusErrCountFile->close();
        delete mModBusErrCountFile;
        mModBusErrCountFile = nullptr;
    }
    if (mModBusDevice){
        disconnect(mModBusDevice, &QModbusClient::stateChanged, this, &CoreLibModBusCommunication::onStateChanged);
        mModBusDevice->disconnectDevice();
        delete mModBusDevice;
        mModBusDevice = nullptr;
    }
#ifdef REAL_TARGET
#else // REAL_TARGET
    if (mQDir){
        delete mQDir;
        mQDir = nullptr;
    }
#endif // REAL_TARGET
}

#ifdef GET_POINT_MEMORY_HEADER_INFO_FROM_CORELIB
int CoreLibModBusCommunication::getPointHeaderSize()
{
    bool ret = false;
    int pointMemSize = 0;
    stModBusCommunicationData modBusCommunicationData;
    QModbusDataUnit dataUnit;
    // sync modbus connect
    ret = syncConnectModBusDevice();
    if (ret == false){
        qCritical() << "syncConnectModBusDevice failed";
        return 0;
    }
    // read point mermory header
    modBusCommunicationData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusCommunicationData.readAddress = mPointMemoryDataHeaderAddress;
    modBusCommunicationData.comSize = mPointMemoryDataHeaderSize;
    ret = syncReadModBusDataUnit(&modBusCommunicationData, &dataUnit);
    if (ret == false){
        qCritical() << "syncReadModBusDataUnit failed";
        return 0;
    }
    for (int i = 0; i < modBusCommunicationData.comSize; i++){
        qDebug() << "readBuf:" << modBusCommunicationData.readBuf[i];
    }
    pointMemSize = parsePointMemorySize(dataUnit);
    // sync modbus disconnect
    ret = syncDisconnectModBusDevice();
    if (ret == false){
        qCritical() << "syncDisconnectModBusDevice failed";
        return 0;
    }
    return pointMemSize;
}
#endif // GET_POINT_MEMORY_HEADER_INFO_FROM_CORELIB

void CoreLibModBusCommunication::connectModBusDevice()
{
    if(mModBusDevice == nullptr)
        return;

    QUrl url = QUrl::fromUserInput(mServerUrl);
    if(mModBusDevice->state() != QModbusDevice::ConnectedState) {
        // Set host and port
        mModBusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        mModBusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

        // Set timeout and retry count
        mModBusDevice->setTimeout(mModBusTimeout);
        mModBusDevice->setNumberOfRetries(mModBusRetryCnt);

        // Connect
        if(!MODBUS_CONNECT_DEVICE()) {
            qInfo() << "Connect failed.";
        } else {
            qInfo() << "connectModBusDevice Connect success.";
            connect(mModBusDevice, &QModbusClient::stateChanged, this, &CoreLibModBusCommunication::onStateChanged);
        }
    }
}

bool CoreLibModBusCommunication::syncConnectModBusDevice()
{
    bool ret = false;
#ifdef CORELIB_UNIT_TEST
#else // CORELIB_UNIT_TEST
    uchar syncConnectRetry = 0;
#endif // CORELIB_UNIT_TEST
    if(mModBusDevice == nullptr)
        return false;

    QUrl url = QUrl::fromUserInput(mServerUrl);
    if(mModBusDevice->state() != QModbusDevice::ConnectedState) {
        // Set host and port
        mModBusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        mModBusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

        // Set timeout and retry count
        mModBusDevice->setTimeout(mModBusTimeout);
        mModBusDevice->setNumberOfRetries(mModBusRetryCnt);

        // Connect
        if(!MODBUS_CONNECT_DEVICE()) {
            qInfo() << "Connect failed.";
        } else {
            qInfo() << "syncConnectModBusDevice Connect success.";
#ifdef CORELIB_UNIT_TEST
#else // CORELIB_UNIT_TEST
            connect(mModBusDevice, &QModbusClient::stateChanged, this, &CoreLibModBusCommunication::onStateChanged);
            for (syncConnectRetry = 0; syncConnectRetry < mMaxModBusConnectTimeOutCnt; syncConnectRetry++){
                QEventLoop loop;
                QTimer::singleShot(mModBusConnectTickCnt, &loop, SLOT(quit()));
                loop.exec();
                if(m_modBusConnectState == QModbusDevice::ConnectedState){
                    qInfo() << "modBusConnectState." << m_modBusConnectState;
                    ret = true;
                    break;
                }
            }
            //qDebug() << "syncConnectRetry." << syncConnectRetry;
#endif // CORELIB_UNIT_TEST
        }
    }
    return ret;
}

void CoreLibModBusCommunication::disconnectModBusDevice()
{
    if(mModBusDevice == nullptr)
        return;

    if(mModBusDevice->state() == QModbusDevice::ConnectedState) {
        // Disconnect
        qInfo() << "ModBus disConnect";
        mModBusDevice->disconnectDevice();
    }
}

bool CoreLibModBusCommunication::syncDisconnectModBusDevice()
{
    bool ret = false;
    uchar syncDisConnectRetry = 0;
    if(mModBusDevice == nullptr)
        return false;

    if(mModBusDevice->state() == QModbusDevice::ConnectedState) {
        // Disconnect
        qInfo() << "ModBus disConnect";
        mModBusDevice->disconnectDevice();
        connect(mModBusDevice, &QModbusClient::stateChanged, this, &CoreLibModBusCommunication::onStateChanged);
        for (syncDisConnectRetry = 0; syncDisConnectRetry < mMaxModBusDisConnectTimeOutCnt; syncDisConnectRetry++){
            QEventLoop loop;
            QTimer::singleShot(mModBusDisConnectTickCnt, &loop, SLOT(quit()));
            loop.exec();
            if(m_modBusConnectState == QModbusDevice::UnconnectedState){
                qInfo() << "modBusConnectState." << m_modBusConnectState;
                ret = true;
                break;
            }
        }
        //qDebug() << "syncDisConnectRetry." << syncDisConnectRetry;
    }
    return ret;
}

QModbusReply* CoreLibModBusCommunication::modBusRawRequest(QModbusRequest& request)
{
    if (mModBusDevice == nullptr){
        return nullptr;
    }
    return mModBusDevice->sendRawRequest(request, mModBusServerAddress);
}

QModbusReply* CoreLibModBusCommunication::modBusReadRequest()
{
    if (mModBusDevice == nullptr){
        return nullptr;
    }
    QModbusDataUnit *readModBusDataUnit = mModBusDataMgr->getModBudDataUnit(CoreLibModBusDataManager::ModbusRequestTypeRead);
    return MODBUS_SEND_READ_REQUEST(*readModBusDataUnit, mModBusServerAddress);
}

QModbusReply* CoreLibModBusCommunication::modBusWriteRequest()
{
    if (mModBusDevice == nullptr){
        return nullptr;
    }
    QModbusDataUnit *writeModBusDataUnit = mModBusDataMgr->getModBudDataUnit(CoreLibModBusDataManager::ModbusRequestTypeWrite);
    return MODBUS_SEND_WRITE_REQUEST(*writeModBusDataUnit, mModBusServerAddress);
}

QModbusReply* CoreLibModBusCommunication::modBusReadWriteRequest()
{
    if (mModBusDevice == nullptr){
        return nullptr;
    }
    QModbusDataUnit *readModBusDataUnit = mModBusDataMgr->getModBudDataUnit(CoreLibModBusDataManager::ModbusRequestTypeRead);
    QModbusDataUnit *writeModBusDataUnit = mModBusDataMgr->getModBudDataUnit(CoreLibModBusDataManager::ModbusRequestTypeWrite);
    return mModBusDevice->sendReadWriteRequest(*readModBusDataUnit, *writeModBusDataUnit, mModBusServerAddress);
}

void CoreLibModBusCommunication::startModBusCommunication()
{
    connectModBusDevice();
}

bool CoreLibModBusCommunication::syncStartModBusCommunication()
{
    bool ret = syncConnectModBusDevice();
    return ret;
}

void CoreLibModBusCommunication::stopModBusCommunication()
{
    disconnectModBusDevice();
}

bool CoreLibModBusCommunication::syncStopModBusCommunication()
{
    bool ret = syncDisconnectModBusDevice();
    return ret;
}

bool CoreLibModBusCommunication::rawReadRequestModBusData(stModBusCommunicationData &modBusCommunicationData)
{
    bool ret = true;
    QModbusReply* reply = nullptr;
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    QModbusRequest request(modBusCommunicationData.funcCode, modBusCommunicationData.readByteArray);

    reply = modBusRawRequest(request);
    if (!reply){
        qCritical() << "modBusRawRequest error.";
        goto err_exit;
    }
    if(!reply->isFinished()) {
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusRawReadReady);
    } else {
        delete reply;
    }
    mModBusDataMgr->setModBusComData(modBusCommunicationData);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    return ret;
}

bool CoreLibModBusCommunication::rawWriteRequestModBusData(stModBusCommunicationData &modBusCommunicationData)
{
    bool ret = true;
    QModbusReply* reply = nullptr;
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    QModbusRequest request(modBusCommunicationData.funcCode, modBusCommunicationData.writeByteArray);
    reply = modBusRawRequest(request);
    if (!reply){
        qCritical() << "modBusRawRequest error.";
        goto err_exit;
    }
    if(!reply->isFinished()) {
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusRawWriteReady);
    } else {
        delete reply;
    }
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    return ret;
}

bool CoreLibModBusCommunication::readModBusData(stModBusCommunicationData &modBusCommunicationData)
{
    bool ret = true;
    QModbusReply* reply = nullptr;

    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    mIsBitMemory = checkModBusBitMemory(modBusCommunicationData);
    if (checkModBusCommunicationSize(modBusCommunicationData) == false){
        qCritical() << "read ModBus Size Err" << "funcCode:" << modBusCommunicationData.funcCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeRead) == false){
        qCritical() << "readyModBusDataUnit error.";
        goto err_exit;
    }
    reply = modBusReadRequest();
    if (!reply){
        qCritical() << "modBusReadRequest error.";
        goto err_exit;
    }
    if(!reply->isFinished()) {
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusReadReady);
    } else {
        delete reply;
    }
    mModBusDataMgr->setModBusComData(modBusCommunicationData);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    return ret;

}
bool CoreLibModBusCommunication::writeModBusData(stModBusCommunicationData &modBusCommunicationData)
{
    bool ret = true;
    QModbusReply* reply = nullptr;
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    if (checkModBusCommunicationSize(modBusCommunicationData) == false){
        qCritical() << "write ModBus Size Err" << "funcCode:" << modBusCommunicationData.funcCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeWrite) == false){
        qCritical() << "readyModBusDataUnit error.";
        goto err_exit;
    }
    reply = modBusWriteRequest();
    if (!reply){
        qCritical() << "modBusWriteRequest error.";
        goto err_exit;
    }
    if(!reply->isFinished()) {
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusWriteReady);
    } else {
        delete reply;
    }
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    return ret;
}

bool CoreLibModBusCommunication::readWriteModBusData(stModBusCommunicationData &modBusCommunicationData)
{
    bool ret = true;
    QModbusReply* reply = nullptr;
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    if (checkModBusCommunicationSize(modBusCommunicationData) == false){
        qCritical() << "read ModBus Size Err" << "funcCode:" << modBusCommunicationData.funcCode;
        goto err_exit;
    }
    if (checkModBusWriteCommunicationSize(modBusCommunicationData) == false){
        qCritical() << "write ModBus Size Err" << "funcCode:" << modBusCommunicationData.writeFuncCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeRead) == false){
        qCritical() << "readyModBusDataUnit read error.";
        goto err_exit;
    }
    if (readyModBusDataUnit(modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeReadWrite) == false){
        qCritical() << "readyModBusDataUnit write error.";
        goto err_exit;
    }
    reply = modBusReadWriteRequest();
    if (!reply){
        qCritical() << "modBusReadWriteRequest error.";
        goto err_exit;
    }
    if(!reply->isFinished()) {
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusReadWriteReady);
    } else {
        delete reply;
    }
    mModBusDataMgr->setModBusComData(modBusCommunicationData);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    return ret;
}

void CoreLibModBusCommunication::onStateChanged(int state)
{
    m_modBusConnectState = state;
    emit modBusComConnectStatusChanged(state);
}

void CoreLibModBusCommunication::modBusReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        qCritical() << "reply nullptr ";
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
        return;
    }
    if (reply->error() == QModbusDevice::NoError) {
        m_modBusReadState = reply->error();
        qDebug("%s:%d m_modBusReadState[%d] mIsBitMemory[%d]", __func__, __LINE__, m_modBusReadState, mIsBitMemory);
        emit modBusComReadStatusChanged(m_modBusReadState);
        QModbusDataUnit unit = reply->result();
        if (mIsBitMemory){
            for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                mModBusDataMgr->getModBusComData()->readBuf[i / 16] |= (unit.value(i) << (i % 16));
            }
            emit mModBusDataMgr->modBusMemDataChanged(*(mModBusDataMgr->getModBusComData()));
        } else {
            for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                mModBusDataMgr->getModBusComData()->readBuf[i] = unit.value(i);
            }
            emit mModBusDataMgr->modBusMemDataChanged(*(mModBusDataMgr->getModBusComData()));
        }
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        qCritical() << "status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    } else {
        qCritical() << "status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    }
    reply->deleteLater();
}

void CoreLibModBusCommunication::modBusAnyReadyForSync()
{
    mModbusResult = qobject_cast<QModbusReply *>(sender());
    emit modBusReplyArrivedForSync();
}

void CoreLibModBusCommunication::modBusWriteReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        qCritical() << "reply nullptr ";
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
        return;
    }
    if (reply->error() == QModbusDevice::NoError) {
        //qDebug() << "reply status" << reply->error() ;
        m_modBusWriteState = reply->error();
        emit modBusComWriteStatusChanged(m_modBusWriteState);
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    } else {
        if (reply->error() == QModbusDevice::ProtocolError){
            qCritical() << "reply exceptionCode " << reply->rawResult().exceptionCode();
            mModBusErrCount++;
            updateModBusErrCount();
            m_modBusWriteState = reply->error();
            emit modBusComWriteStatusChanged(m_modBusWriteState);
        } else {
            qCritical() << "reply status" << reply->error() ;
            mModBusErrCount++;
            updateModBusErrCount();
            m_modBusWriteState = reply->error();
            emit modBusComWriteStatusChanged(m_modBusWriteState);
        }
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    }
    reply->deleteLater();
}

void CoreLibModBusCommunication::modBusReadWriteReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        qCritical() << "reply nullptr ";
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
        return;
    }
    if (reply->error() == QModbusDevice::NoError) {
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        QModbusDataUnit unit = reply->result();
        if (mIsBitMemory){
            for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                mModBusDataMgr->getModBusComData()->readBuf[i / 16] |= (unit.value(i) << (i % 16));
            }
            emit mModBusDataMgr->modBusMemDataChanged(*(mModBusDataMgr->getModBusComData()));
        } else {
            for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
                mModBusDataMgr->getModBusComData()->readBuf[i] = unit.value(i);
            }
            emit mModBusDataMgr->modBusMemDataChanged(*(mModBusDataMgr->getModBusComData()));
        }
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        qCritical() << "status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    } else {
        qCritical() << "status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    }
    reply->deleteLater();
}

void CoreLibModBusCommunication::modBusRawReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        qCritical() << "reply nullptr ";
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
        return;
    }
    if (reply->error() == QModbusDevice::NoError) {
        QModbusResponse response = reply->rawResult();
        qDebug() << "response size " << response.size();
        mModBusDataMgr->getModBusComData()->readByteArray.clear();
        mModBusDataMgr->getModBusComData()->readByteArray.append(response.data());
        emit mModBusDataMgr->modBusMemDataChanged(*(mModBusDataMgr->getModBusComData()));
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    } else {
        qCritical() << "status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    }
}
void CoreLibModBusCommunication::modBusRawWriteReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        qCritical() << "modBusRawWriteReady reply nullptr ";
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
        return;
    }
    if (reply->error() == QModbusDevice::NoError) {
        qDebug() << "modBusRawWriteReady reply status" << reply->error() ;
        m_modBusWriteState = reply->error();
        emit modBusComWriteStatusChanged(m_modBusWriteState);
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    } else {
        qCritical() << "modBusRawWriteReady reply status" << reply->error() ;
        m_modBusWriteState = reply->error();
        emit modBusComWriteStatusChanged(m_modBusWriteState);
        mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    }
}

void CoreLibModBusCommunication::setModBusDataUnit(
        QModbusDataUnit &memoryDataUnit, stModBusCommunicationData &modBusCommunicationData)
{
    if (modBusCommunicationData.funcCode == QModbusPdu::FunctionCode::WriteMultipleCoils){
        // bit memory
        ushort comUnitCnt = (modBusCommunicationData.comSize / 16 == 0 ? 1 :
                   (modBusCommunicationData.comSize % 16 == 0 ? modBusCommunicationData.comSize / 16 : \
                    modBusCommunicationData.comSize / 16 + 1));
        ushort setValueCnt = (modBusCommunicationData.comSize < 16 ? modBusCommunicationData.comSize : 16);
        for (ushort i = 0; i < comUnitCnt; i++){
            for (ushort j = 0; j < setValueCnt; j++){
                ushort mask =  MODBUS_BIT_SHIFT_MASK_NUM(j % 16);
                memoryDataUnit.setValue(i * 16 + j, ((modBusCommunicationData.writeBuf[i] & ~mask) >> (j % 16)));
            }
        }
    } else {
        for (ushort i = 0; i < modBusCommunicationData.comSize; i++){
            memoryDataUnit.setValue(i, modBusCommunicationData.writeBuf[i]);
        }
    }
}

void CoreLibModBusCommunication::setModBusDataUnitForReadWrite(
    QModbusDataUnit &memoryDataUnit, stModBusCommunicationData &modBusCommunicationData)
{
    if (modBusCommunicationData.writeFuncCode == QModbusPdu::FunctionCode::WriteMultipleCoils){
        // bit memory
        ushort comUnitCnt = (modBusCommunicationData.comSize / 16 == 0 ? 1 :
                   (modBusCommunicationData.comSize % 16 == 0 ? modBusCommunicationData.comSize / 16 : \
                    modBusCommunicationData.comSize / 16 + 1));
        ushort setValueCnt = (modBusCommunicationData.comSize < 16 ? modBusCommunicationData.comSize : 16);
        for (ushort i = 0; i < comUnitCnt; i++){
            for (ushort j = 0; j < setValueCnt; j++){
                ushort mask =  MODBUS_BIT_SHIFT_MASK_NUM(j % 16);
                memoryDataUnit.setValue(i * 16 + j, ((modBusCommunicationData.writeBuf[i] & ~mask) >> (j % 16)));
            }
        }
    } else {
        for (ushort i = 0; i < modBusCommunicationData.comSize; i++){
            memoryDataUnit.setValue(i, modBusCommunicationData.writeBuf[i]);
        }
    }
}

bool CoreLibModBusCommunication::readyModBusDataUnit(
        stModBusCommunicationData &modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestType requestType)
{
    bool ret = true;
    QModbusDataUnit::RegisterType regType = QModbusDataUnit::RegisterType::Invalid;
    // judge register type from funcCode
    if (requestType == CoreLibModBusDataManager::ModbusRequestTypeReadWrite){
        regType = mModBusDataMgr->getRegisterType(modBusCommunicationData.writeFuncCode);
    } else {
        regType = mModBusDataMgr->getRegisterType(modBusCommunicationData.funcCode);
    }
    switch(requestType){
    case CoreLibModBusDataManager::ModbusRequestTypeRead:{
        QModbusDataUnit *readModBusDataUnit = mModBusDataMgr->getModBudDataUnit(CoreLibModBusDataManager::ModbusRequestTypeRead);
        if (regType == QModbusDataUnit::RegisterType::DiscreteInputs
            || regType == QModbusDataUnit::RegisterType::Coils
            || regType == QModbusDataUnit::RegisterType::InputRegisters
            || regType == QModbusDataUnit::RegisterType::HoldingRegisters){
            // set modbus data unit
            *readModBusDataUnit =
                QModbusDataUnit(regType, modBusCommunicationData.readAddress, modBusCommunicationData.comSize);
        } else {
            qCritical("requestType[%d] invalid regType:%d", requestType, regType);
            ret = false;
        }
        break;
    }
    case CoreLibModBusDataManager::ModbusRequestTypeWrite:{
        QModbusDataUnit *writeModBusDataUnit = mModBusDataMgr->getModBudDataUnit(CoreLibModBusDataManager::ModbusRequestTypeWrite);
        if (regType == QModbusDataUnit::RegisterType::Coils
            || regType == QModbusDataUnit::RegisterType::HoldingRegisters){
            *writeModBusDataUnit =
                QModbusDataUnit(regType, modBusCommunicationData.writeAddress, modBusCommunicationData.comSize);
        } else {
            qCritical("requestType[%d] invalid regType:%d", requestType, regType);
            ret = false;
        }
        setModBusDataUnit(*writeModBusDataUnit, modBusCommunicationData);
        break;
    }
    case CoreLibModBusDataManager::ModbusRequestTypeReadWrite:{
        QModbusDataUnit *writeModBusDataUnit = mModBusDataMgr->getModBudDataUnit(CoreLibModBusDataManager::ModbusRequestTypeWrite);
        if (regType == QModbusDataUnit::RegisterType::Coils){
            *writeModBusDataUnit =
                QModbusDataUnit(regType, modBusCommunicationData.writeAddress, modBusCommunicationData.writeComSize);
        } else if (regType == QModbusDataUnit::RegisterType::HoldingRegisters) {
            *writeModBusDataUnit =
                QModbusDataUnit(regType, modBusCommunicationData.writeAddress, modBusCommunicationData.writeComSize);
        } else {
            qCritical("requestType[%d] invalid regType:%d", requestType, regType);
            ret = false;
        }
        setModBusDataUnitForReadWrite(*writeModBusDataUnit, modBusCommunicationData);
        break;
    }
    default:
        qCritical() << "Not supported Request Type: " << requestType;
        ret = false;
        break;
    }
    return ret;
}

bool CoreLibModBusCommunication::checkModBusCommunicationSize(stModBusCommunicationData &modBusCommunicationData)
{
    return MODBUS_COMSIZE_CHECK(modBusCommunicationData.funcCode, modBusCommunicationData.comSize);
}

bool CoreLibModBusCommunication::checkModBusWriteCommunicationSize(stModBusCommunicationData &modBusCommunicationData)
{
    return MODBUS_COMSIZE_CHECK(modBusCommunicationData.writeFuncCode, modBusCommunicationData.writeComSize);
}

bool CoreLibModBusCommunication::checkModBusBitMemory(stModBusCommunicationData &modBusCommunicationData)
{
    QModbusPdu::FunctionCode funcCode = modBusCommunicationData.funcCode;

    return checkModBusBitMemory(funcCode);
}

bool CoreLibModBusCommunication::checkModBusBitMemory(QModbusPdu::FunctionCode funcCode)
{
    if (funcCode == QModbusPdu::FunctionCode::ReadCoils ||
        funcCode == QModbusPdu::FunctionCode::ReadDiscreteInputs ||
        funcCode == QModbusPdu::FunctionCode::WriteSingleCoil ||
        funcCode == QModbusPdu::FunctionCode::WriteMultipleCoils){
        return true;
    } else {
        return false;
    }
}

ulong CoreLibModBusCommunication::getModBusErrorCount()
{
    return mModBusErrCount;
}

void CoreLibModBusCommunication::updateModBusErrCount()
{
    if (!mModBusErrCountFile){
        qCritical() << "mModBusErrCountFile is null:" << mModBusErrCountFilePath;
        return;
    }
    if( !mModBusErrCountFile->exists() ){
        qCritical() << "write file not exists " << mModBusErrCountFilePath;
        return;
    }
    if (mModBusErrCount >= MODBUS_ERR_COUNT_MAX(MODBUS_ERR_COUNT_STR_SIZE)){
        mModBusErrCount = MODBUS_ERR_COUNT_MAX(MODBUS_ERR_COUNT_STR_SIZE);
    }
    QString src;
    src.clear();
    uchar digit = GET_MODBUS_ERR_COUNT_DIGIT(mModBusErrCount);
    for (uchar i = MODBUS_ERR_COUNT_STR_SIZE - digit; i > 0; i--){
        src.append("0");
    }
    src.append(QString::number(mModBusErrCount));
    QTextStream ts(mModBusErrCountFile);
    ts.seek(0);
    ts << src;
    ts.flush();
}

void CoreLibModBusCommunication::readyModBusErrorCountFile(QString &aplInfo)
{
    bool is_ok = false;
#ifdef Q_OS_LINUX
    int ret = 0;
    mModBusBasePath = mHomePath + "/" + aplInfo + "/" + "modbus";
    QString commandString = "mkdir -p " + mModBusBasePath;
    qDebug() << "commandString:" << commandString << "mModBusBasePath :" << mModBusBasePath;
    QByteArray str_arr = commandString.toLocal8Bit();
    const char *command = str_arr.constData();
    qDebug("command[%s]", command);
    ret = system(command);
    if (ret < 0){
        qDebug() << "mkdir failed.";
#else
    bool ret = false;
    mModBusAplPath = mHomePath + "/" + aplInfo;
    qDebug() << "mModBusAplPath :" << mModBusAplPath;
    ret = mQDir->mkdir(mModBusAplPath);
    if (!ret){
        qDebug() << "QDir mkdir failed:" << ret << "mModBusAplPath" << mModBusAplPath;
        return;
    }
    mModBusBasePath = mModBusAplPath + "/" + "modbus";
    qDebug() << "mModBusBasePath :" << mModBusBasePath;
    ret = mQDir->mkdir(mModBusBasePath);
    if (!ret){
        qDebug() << "QDir mkdir failed:" << ret << "mModBusBasePath" << mModBusBasePath;
#endif
    } else {
        mModBusErrCountFilePath = mModBusBasePath + "/" + mModBusErrCountFileName;
        mModBusErrCountFile = new QFile();
        mModBusErrCountFile->setFileName(mModBusErrCountFilePath);
        if (!mModBusErrCountFile->open(QIODevice::ReadWrite)){
            qDebug() << "open failed." << mModBusErrCountFilePath;
        } else {
            QTextStream ts(mModBusErrCountFile);
            mModBusErrCountInitValue = ts.readLine(0);
            mModBusErrCount = mModBusErrCountInitValue.toInt(&is_ok, 10);
            if (!is_ok){
                qDebug("mModBusErrCountInitValue is not num");
                mModBusErrCount = 0;
            }
            qDebug("mModBusErrCount[%lu]", mModBusErrCount);
        }
    }
}

#ifdef GET_POINT_MEMORY_HEADER_INFO_FROM_CORELIB
void CoreLibModBusCommunication::readPointMemoryHeaderData()
{
    stModBusCommunicationData modBusCommunicationData;
    modBusCommunicationData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusCommunicationData.readAddress = mPointMemoryDataHeaderAddress;
    modBusCommunicationData.comSize = mPointMemoryDataHeaderSize;
    // read ModBus PointMemoryDataHeader
    readModBusData(modBusCommunicationData);
}

int CoreLibModBusCommunication::parsePointMemorySize(QModbusDataUnit &dataUnit)
{
    int pointMemorySizeInfo = 0;
    short pointMemoryInfo = 0;
    char pointMemoryHeaderDataInfo = 0;
    if (dataUnit.valueCount() <= 0 || dataUnit.valueCount() > 1){
        qCritical() << "point memory header data count invalid:" << dataUnit.valueCount();
        return 0;
    }
    for (uint i = 0; i < dataUnit.valueCount(); i++){
        qDebug() << "value:" << dataUnit.value(i);
        qDebug() << "value(str):" << QString::number(dataUnit.value(i));
        bool isOk;
        pointMemoryInfo = QString::number(dataUnit.value(i)).toInt(&isOk, 16);
        qDebug("pointMemoryInfo[0x%04x]", pointMemoryInfo);
        for (uint j = 0; j < APL_INDEX_INFO_MAX; j++){
            pointMemoryHeaderDataInfo = GET_POINT_MEMORY_INFO(j, pointMemoryInfo);
            if (pointMemoryHeaderDataInfo < 0){
                // init point memory size
                qDebug() << "index invalid:" << pointMemoryHeaderDataInfo;
                mMemoryDataMgr->initPointMemoryData(0);
                return 0;
            }
            pointMemorySizeInfo += getPointMemorySize(pointMemoryHeaderDataInfo);
        }
        if (pointMemorySizeInfo > mPointMemoryDataMaxLimitSize){
            qDebug() << "point memory max limit:" << pointMemorySizeInfo;
            pointMemorySizeInfo = mPointMemoryDataMaxLimitSize;
        }
    }
    // init point memory
    mMemoryDataMgr->initPointMemoryData(pointMemorySizeInfo);
    return pointMemorySizeInfo;
}

int CoreLibModBusCommunication::getPointMemorySize(short headerInfo)
{
    return POINT_HEADER_INFO_TO_SIZE(headerInfo);
}
#endif // GET_POINT_MEMORY_HEADER_INFO_FROM_CORELIB

bool CoreLibModBusCommunication::syncReadModBusData(stModBusCommunicationData* modBusCommunicationData)
{
    bool ret = true;
    QModbusReply* reply = nullptr;

    if (!modBusCommunicationData){
        qCritical("syncReadModBusData:modBusCommunicationData is Null");
        return false;
    }
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    mIsBitMemory = checkModBusBitMemory(*modBusCommunicationData);
    /*
    if (mIsBitMemory){
        qDebug() << "This function is only for non-BitType";
        goto err_exit;
    }
    */
    if (checkModBusCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "read ModBus Size Err" << "funcCode:" << modBusCommunicationData->funcCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeRead) == false){
        qCritical() << "readyModBusDataUnit error.";
        goto err_exit;
    }
    reply = modBusReadRequest();
    if (!reply){
        qCritical() << "modBusReadRequest error." << "readAddress:" << modBusCommunicationData->readAddress << "comSize:" << modBusCommunicationData->comSize;
        goto err_exit;
    }
    if(!reply->isFinished()) {
        QEventLoop loop;
        connect(this, &CoreLibModBusCommunication::modBusReplyArrivedForSync, &loop, &QEventLoop::quit);
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusAnyReadyForSync);
        loop.exec();
        disconnect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusAnyReadyForSync);
        disconnect(this, &CoreLibModBusCommunication::modBusReplyArrivedForSync, &loop, &QEventLoop::quit);
        reply = mModbusResult;
    }

    if (!reply){
        qCritical() << "reply nullptr ";
        goto err_exit;
    }

    if (reply->error() == QModbusDevice::NoError) {
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        QModbusDataUnit unit = reply->result();
        for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
            if (mIsBitMemory) {
                modBusCommunicationData->readBuf[i / 16] |= (unit.value(i) << (i % 16));
            } else {
                modBusCommunicationData->readBuf[i] = unit.value(i);
            }
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
#ifdef REAL_TARGET
        qCritical() << "syncReadModBusData:ProtocolError status " << reply->error() << "readAddress:" << modBusCommunicationData->readAddress << "comSize:" << modBusCommunicationData->comSize;
#endif // REAL_TARGET
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
    } else {
        qCritical() << "syncReadModBusData:status " << reply->error() << "readAddress:" << modBusCommunicationData->readAddress << "comSize:" << modBusCommunicationData->comSize;
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        ret = false;
    }

    delete reply;

    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    return ret;
}

bool CoreLibModBusCommunication::syncReadModBusDataUnit(
        stModBusCommunicationData* modBusCommunicationData, QModbusDataUnit *dataUnit)
{
    bool ret = true;
    QModbusReply* reply = nullptr;
    if (!dataUnit || !modBusCommunicationData){
        qCritical("syncReadModBusDataUnit:modBusCommunicationData or dataUnit is Null");
        return false;
    }
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    mIsBitMemory = checkModBusBitMemory(*modBusCommunicationData);
    if (mIsBitMemory){
        qCritical() << "This function is only for non-BitType";
        goto err_exit;
    }
    if (checkModBusCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "read ModBus Size Err" << "funcCode:" << modBusCommunicationData->funcCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeRead) == false){
        qCritical() << "readyModBusDataUnit error.";
        goto err_exit;
    }
    reply = modBusReadRequest();
    if (!reply){
        qCritical() << "modBusReadRequest error.";
        goto err_exit;
    }

    if(!reply->isFinished()) {
        QEventLoop loop;
        connect(this, &CoreLibModBusCommunication::modBusReplyArrivedForSync, &loop, &QEventLoop::quit);
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusAnyReadyForSync);
        loop.exec();
        disconnect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusAnyReadyForSync);
        disconnect(this, &CoreLibModBusCommunication::modBusReplyArrivedForSync, &loop, &QEventLoop::quit);
        reply = mModbusResult;
    }

    if (!reply){
        qCritical() << "reply nullptr ";
        goto err_exit;
    }
    if (reply->error() == QModbusDevice::NoError) {
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        *dataUnit = reply->result();
        for (int i = 0, total = int(dataUnit->valueCount()); i < total; ++i) {
            if (mIsBitMemory) {
                modBusCommunicationData->readBuf[i / 16] |= (dataUnit->value(i) << (i % 16));
            } else {
                modBusCommunicationData->readBuf[i] = dataUnit->value(i);
            }
        }
        ret = true;
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        qCritical() << "status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        ret = false;
    } else {
        qCritical() << "status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        ret = false;
    }
    delete reply;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    return ret;
}

bool CoreLibModBusCommunication::syncWriteModBusData(stModBusCommunicationData* modBusCommunicationData)
{
    bool ret = true;
    QModbusReply* reply = nullptr;

    if (!modBusCommunicationData){
        qCritical("syncWriteModBusData:modBusCommunicationData is Null");
        return false;
    }
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    if (checkModBusCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "write ModBus Size Err" << "funcCode:" << modBusCommunicationData->funcCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeWrite) == false){
        qCritical() << "readyModBusDataUnit error.";
        goto err_exit;
    }
    reply = modBusWriteRequest();
    if (!reply){
        qCritical() << "modBusWriteRequest error.";
        goto err_exit;
    }

    if (!reply->isFinished()) {
        QEventLoop loop;
        connect(this, &CoreLibModBusCommunication::modBusReplyArrivedForSync, &loop, &QEventLoop::quit);
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusAnyReadyForSync);
        loop.exec();
        disconnect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusAnyReadyForSync);
        disconnect(this, &CoreLibModBusCommunication::modBusReplyArrivedForSync, &loop, &QEventLoop::quit);
        reply = mModbusResult;
    }

    if (!reply){
        qCritical() << "reply nullptr ";
        goto err_exit;
    }

    if (reply->error() == QModbusDevice::NoError) {
        //qDebug() << "reply status" << reply->error() ;
        m_modBusWriteState = reply->error();
        emit modBusComWriteStatusChanged(m_modBusWriteState);
    } else {
        if (reply->error() == QModbusDevice::ProtocolError){
            qCritical() << "reply exceptionCode " << reply->rawResult().exceptionCode();
            mModBusErrCount++;
            updateModBusErrCount();
            m_modBusWriteState = reply->error();
            emit modBusComWriteStatusChanged(m_modBusWriteState);
        } else {
            qCritical() << "syncWriteModBusData: reply status" << reply->error() ;
            mModBusErrCount++;
            updateModBusErrCount();
            m_modBusWriteState = reply->error();
            emit modBusComWriteStatusChanged(m_modBusWriteState);
            ret = false;
        }
    }
    delete reply;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    return ret;
}

bool CoreLibModBusCommunication::syncReadWriteModBusData(stModBusCommunicationData* modBusCommunicationData)
{
    bool ret = true;
    QModbusReply* reply = nullptr;
    if (!modBusCommunicationData){
        qCritical("syncWriteModBusData:modBusCommunicationData is Null");
        return false;
    }
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    if (checkModBusCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "read ModBus Size Err" << "funcCode:" << modBusCommunicationData->funcCode;
        goto err_exit;
    }
    if (checkModBusWriteCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "write ModBus Size Err" << "funcCode:" << modBusCommunicationData->writeFuncCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeRead) == false){
        qCritical() << "readyModBusDataUnit read error.";
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeReadWrite) == false){
        qCritical() << "readyModBusDataUnit write error.";
        goto err_exit;
    }
    reply = modBusReadWriteRequest();
    if (!reply){
        qCritical() << "modBusReadWriteRequest error.";
        goto err_exit;
    }
    if (!reply->isFinished()) {
        QEventLoop loop;
        connect(this, &CoreLibModBusCommunication::modBusReplyArrivedForSync, &loop, &QEventLoop::quit);
        connect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusAnyReadyForSync);
        loop.exec();
        disconnect(reply, &QModbusReply::finished, this, &CoreLibModBusCommunication::modBusAnyReadyForSync);
        disconnect(this, &CoreLibModBusCommunication::modBusReplyArrivedForSync, &loop, &QEventLoop::quit);
        reply = mModbusResult;
    }
    if (!reply){
        qCritical() << "reply nullptr ";
        goto err_exit;
    }

    if (reply->error() == QModbusDevice::NoError) {
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        QModbusDataUnit unit = reply->result();
        for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
            modBusCommunicationData->readBuf[i] = unit.value(i);
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        qCritical() << "syncReadWriteModBusData:ProtocolError status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
    } else {
        qCritical() << "syncReadWriteModBusData:status " << reply->error();
        mModBusErrCount++;
        updateModBusErrCount();
        m_modBusReadState = reply->error();
        emit modBusComReadStatusChanged(m_modBusReadState);
        ret = false;
    }
    delete reply;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    return ret;
}

bool CoreLibModBusCommunication::syncReadModBusDebugData(stModBusCommunicationData* modBusCommunicationData)
{
    bool ret = true;

    if (!modBusCommunicationData){
        qCritical("syncReadModBusData:modBusCommunicationData is Null");
        return false;
    }
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    mIsBitMemory = checkModBusBitMemory(*modBusCommunicationData);

    if (checkModBusCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "read ModBus Size Err" << "funcCode:" << modBusCommunicationData->funcCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeRead) == false){
        qCritical() << "readyModBusDataUnit error.";
        goto err_exit;
    }

    if(mIsBitMemory){
        QByteArray dataList = mMemoryDataMgr->getModBusBitData(modBusCommunicationData->funcCode, modBusCommunicationData->readAddress, modBusCommunicationData->comSize);

        for(int i = 0; i < dataList.count(); i+=2){
            ushort data = 0;
            uchar c = 0;
            c = (uchar)dataList.at(i);
            data += c << 8;
            if(i+1 < dataList.count()){
                c = (uchar)dataList.at(i+1);
                data += c;
            }

            ushort reverse = 0;

            for (int i = 0; i < 16; i++){
                int shift = (15 - i*2);

                if(0 < shift){
                    reverse += ((data & (1 << i)) << shift);
                }
                else{
                    reverse += ((data & (1 << i)) >> -shift);
                }

            }

            modBusCommunicationData->readBuf[i/2] = reverse;
        }
    }
    else{
        QByteArray dataList = mMemoryDataMgr->getModBusData(modBusCommunicationData->funcCode, modBusCommunicationData->readAddress, modBusCommunicationData->comSize);

        for(int i = 0; i < dataList.count(); i+=2){
            ushort data = 0;
            uchar c = 0;
            c = (uchar)dataList.at(i);
            data += c << 8;
            if(i+1 < dataList.count()){
                c = (uchar)dataList.at(i+1);
                data += c;
            }

            modBusCommunicationData->readBuf[i/2] = data;
        }
    }

    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeRead);
    return ret;
}

bool CoreLibModBusCommunication::syncWriteModBusDebugData(stModBusCommunicationData* modBusCommunicationData)
{
    bool ret = true;

    if (!modBusCommunicationData){
        qCritical("syncWriteModBusData:modBusCommunicationData is Null");
        return false;
    }
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    bool isBitMemory = checkModBusBitMemory(*modBusCommunicationData);
    if (checkModBusCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "write ModBus Size Err" << "funcCode:" << modBusCommunicationData->funcCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeWrite) == false){
        qCritical() << "readyModBusDataUnit error.";
        goto err_exit;
    }

    if(isBitMemory){
        QByteArray data;
        int size = modBusCommunicationData->comSize;
        int comUnitCnt = (size / 16 == 0 ? 1 : \
            (size % 16 == 0 ? size / 16 : size / 16 + 1));     // bit unit from comSize(word base)
        bool isComSizeRemainder = (size % 16 == 0 ? false : true);
        for(int i = 0; i < comUnitCnt; i++){
            if (isComSizeRemainder){
                if (i == comUnitCnt - 1){
                    if(8 < size % 16){
                        ushort src = modBusCommunicationData->writeBuf[i];
                        ushort reverse = 0;

                        for (int i = 0; i < 16; i++){
                            int shift = (15 - i*2);

                            if(0 < shift){
                                reverse += ((src & (1 << i)) << shift);
                            }
                            else{
                                reverse += ((src & (1 << i)) >> -shift);
                            }
                        }

                        uchar data1 = (reverse >> 8) & 0xff;
                        uchar data2 = (reverse) & 0xff;

                        data.append(data1);
                        data.append(data2);
                    }
                    else{
                        ushort src = modBusCommunicationData->writeBuf[i];
                        ushort reverse = 0;

                        for (int i = 0; i < 8; i++){
                            int shift = (7 - i*2);

                            if(0 < shift){
                                reverse += ((src & (1 << i)) << shift);
                            }
                            else{
                                reverse += ((src & (1 << i)) >> -shift);
                            }
                        }

                        uchar data1 = (reverse) & 0xff;
                        data.append(data1);

                    }
                } else {
                    ushort src = modBusCommunicationData->writeBuf[i];
                    ushort reverse = 0;

                    for (int i = 0; i < 16; i++){
                        int shift = (15 - i*2);

                        if(0 < shift){
                            reverse += ((src & (1 << i)) << shift);
                        }
                        else{
                            reverse += ((src & (1 << i)) >> -shift);
                        }
                    }

                    uchar data1 = (reverse >> 8) & 0xff;
                    uchar data2 = (reverse) & 0xff;

                    data.append(data1);
                    data.append(data2);
                }
            } else {
                ushort src = modBusCommunicationData->writeBuf[i];
                ushort reverse = 0;

                for (int i = 0; i < 16; i++){
                    int shift = (15 - i*2);

                    if(0 < shift){
                        reverse += ((src & (1 << i)) << shift);
                    }
                    else{
                        reverse += ((src & (1 << i)) >> -shift);
                    }
                }

                uchar data1 = (reverse >> 8) & 0xff;
                uchar data2 = (reverse) & 0xff;

                data.append(data1);
                data.append(data2);
            }
        }
        mMemoryDataMgr->setModBusBitData(modBusCommunicationData->writeAddress, data.count(), data);
    }
    else{
        QByteArray data;
        for(int i = 0; i < modBusCommunicationData->comSize; i++){
            uchar data1 = (modBusCommunicationData->writeBuf[i] >> 8) & 0xff;
            uchar data2 = (modBusCommunicationData->writeBuf[i]) & 0xff;

            data.append(data1);
            data.append(data2);
        }
        mMemoryDataMgr->setModBusData(modBusCommunicationData->writeAddress, modBusCommunicationData->comSize, data);
    }

    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);

    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeWrite);
    return ret;
}

bool CoreLibModBusCommunication::syncReadWriteModBusDebugData(stModBusCommunicationData* modBusCommunicationData)
{
    bool ret = true;

    if (!modBusCommunicationData){
        qCritical("syncWriteModBusData:modBusCommunicationData is Null");
        return false;
    }
    mModBusDataMgr->lockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    bool isBitMemory = checkModBusBitMemory(modBusCommunicationData->writeFuncCode);
    if (checkModBusCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "read ModBus Size Err" << "funcCode:" << modBusCommunicationData->funcCode;
        goto err_exit;
    }
    if (checkModBusWriteCommunicationSize(*modBusCommunicationData) == false){
        qCritical() << "write ModBus Size Err" << "funcCode:" << modBusCommunicationData->writeFuncCode;
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeRead) == false){
        qCritical() << "readyModBusDataUnit read error.";
        goto err_exit;
    }
    if (readyModBusDataUnit(*modBusCommunicationData, CoreLibModBusDataManager::ModbusRequestTypeReadWrite) == false){
        qCritical() << "readyModBusDataUnit write error.";
        goto err_exit;
    }
    if(isBitMemory){
        QByteArray data;
        int size = modBusCommunicationData->comSize;
        int comUnitCnt = (size / 16 == 0 ? 1 : \
            (size % 16 == 0 ? size / 16 : size / 16 + 1));     // bit unit from comSize(word base)
        bool isComSizeRemainder = (size % 16 == 0 ? false : true);
        for(int i = 0; i < comUnitCnt; i++){
            if (isComSizeRemainder){
                if (i == comUnitCnt - 1){
                    uchar data1 = (modBusCommunicationData->writeBuf[i] >> 8) & 0xff;
                    data.append(data1);
                    if(8 < size % 16){
                        uchar data2 = (modBusCommunicationData->writeBuf[i]) & 0xff;
                        data.append(data2);
                    }
                } else {
                    uchar data1 = (modBusCommunicationData->writeBuf[i] >> 8) & 0xff;
                    uchar data2 = (modBusCommunicationData->writeBuf[i]) & 0xff;

                    data.append(data1);
                    data.append(data2);
                }
            } else {
                uchar data1 = (modBusCommunicationData->writeBuf[i] >> 8) & 0xff;
                uchar data2 = (modBusCommunicationData->writeBuf[i]) & 0xff;

                data.append(data1);
                data.append(data2);
            }
        }
        mMemoryDataMgr->setModBusBitData(modBusCommunicationData->writeAddress, comUnitCnt, data);
    }
    else{
        QByteArray data;
        for(int i = 0; i < modBusCommunicationData->comSize; i++){
            uchar data1 = (modBusCommunicationData->writeBuf[i] >> 8) & 0xff;
            uchar data2 = (modBusCommunicationData->writeBuf[i]) & 0xff;

            data.append(data1);
            data.append(data2);
        }
        mMemoryDataMgr->setModBusData(modBusCommunicationData->writeAddress, modBusCommunicationData->comSize, data);
    }

    mIsBitMemory = checkModBusBitMemory(modBusCommunicationData->funcCode);
    if(mIsBitMemory){
        QByteArray dataList = mMemoryDataMgr->getModBusBitData(modBusCommunicationData->funcCode, modBusCommunicationData->readAddress, modBusCommunicationData->comSize);

        for(int i = 0; i < dataList.count(); i+=2){
            ushort data = 0;
            uchar c = 0;
            c = (uchar)dataList.at(i);
            data += c << 8;
            if(i+1 < dataList.count()){
                c = (uchar)dataList.at(i+1);
                data += c;
            }

            modBusCommunicationData->readBuf[i] = data;
        }
    }
    else{
        QByteArray dataList = mMemoryDataMgr->getModBusData(modBusCommunicationData->funcCode, modBusCommunicationData->readAddress, modBusCommunicationData->comSize);

        for(int i = 0; i < dataList.count(); i+=2){
            ushort data = 0;
            uchar c = 0;
            c = (uchar)dataList.at(i);
            data += c << 8;
            if(i+1 < dataList.count()){
                c = (uchar)dataList.at(i+1);
                data += c;
            }

            modBusCommunicationData->readBuf[i] = data;
        }
    }

    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    return ret;
err_exit:
    ret = false;
    mModBusDataMgr->unlockModBusMutex(mModBusDataMgr->ModbusRequestType::ModbusRequestTypeReadWrite);
    return ret;
}
