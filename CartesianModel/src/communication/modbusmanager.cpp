#include "modbusmanager.h"
#include "corelibsdcontrol.h"
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
#include <QProcess>
#endif // ENABLE_COMMUNICATION_DUAL_PORT

ModbusManager::ModbusManager(QObject *parent) 
    : QObject(parent),
      m_communication(CommunicationThread::create_instance()),
      m_modbusCommunication(ModbusCommunication::create_instance())
{

}

ModbusManager* ModbusManager::create_instance(QObject *parent) {
    if(!instance){
        instance = new ModbusManager(parent);
    }
    return instance;
}

ModbusManager* ModbusManager::get_instance() {
    return instance;
}

void ModbusManager::destroy_instance() {
    delete instance;
    instance = nullptr;
}

/// Starts communication thread.
void ModbusManager::startCommunication(QString applicationName)
{
    if ((m_communication != nullptr) && (m_communication->isRunning())) {
        qWarning() << "communication thread already running";
        return;
    }

    CoreLibSdControl sdControl;
    QString aplInfo = applicationName;
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
    int portNum = 0;
    int serverAddress = 0;
    int execCode = 0;
#endif // ENABLE_COMMUNICATION_DUAL_PORT
    QString serverIPAddress = sdControl.getControllerData("System", "IPAddress", "127.0.0.1").toString();  // default IP
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
#ifdef REAL_TARGET
    execCode = QProcess::execute("ping", QStringList() << "-c" << "1" << serverIPAddress);
#else
#ifdef Q_OS_LINUX
    execCode = QProcess::execute("ping", QStringList() << "-c" << "1" << serverIPAddress);
#else // Q_OS_LINUX
    execCode = QProcess::execute("ping", QStringList() << "-n" << "1" << serverIPAddress);
#endif // Q_OS_LINUX
#endif // REAL_TARGET
    if (execCode == 0){
        portNum = sdControl.getPendantData("Modbus", "PortNo", 502).toInt();  // default port number
        serverAddress = sdControl.getPendantData("Modbus", "ServerAddress", 1).toInt();  // default server address
    } else {
        serverIPAddress = sdControl.getControllerData("System", "IPAddress2", "127.0.0.1").toString();  // default IP
        portNum = sdControl.getPendantData("Modbus2", "PortNo", 502).toInt();  // default port number
        serverAddress = sdControl.getPendantData("Modbus2", "ServerAddress", 1).toInt();  // default server address
    }
#ifdef REAL_TARGET
    qInfo() << "execCode:" << execCode << "serverIPAddress:" << serverIPAddress << "portNum:" << portNum << "serverAddress:" << serverAddress;
#endif // REAL_TARGET
#else // ENABLE_COMMUNICATION_DUAL_PORT
    int portNum = sdControl.getPendantData("Modbus", "PortNo", 502).toInt();  // default port number
    int serverAddress = sdControl.getPendantData("Modbus", "ServerAddress", 1).toInt();  // default server address
#endif // ENABLE_COMMUNICATION_DUAL_PORT

    if (!m_communication) { m_communication = CommunicationThread::create_instance();}
    connect(m_communication, &CommunicationThread::feed, this, &ModbusManager::update, Qt::QueuedConnection);
    connect(m_communication, &CommunicationThread::timeout, this, &ModbusManager::onTimeout, Qt::QueuedConnection);
    connect(m_communication, &CommunicationThread::timeoutError, this, &ModbusManager::onTimeoutError, Qt::DirectConnection);
    connect(m_communication, &CommunicationThread::replyAbortedError, this, &ModbusManager::onReplyAbortedError, Qt::DirectConnection);
    connect(m_communication,SIGNAL(linkStateChange(bool)),this,SLOT(onlinkStateChange(bool)));
	
    //qDebug() << "portNum:" << portNum << "serverAddress:" << serverAddress << "serverIPAddress:" << serverIPAddress;
    m_communication->setModbusInfo(aplInfo, portNum, serverAddress, serverIPAddress, m_timeout, m_retry);
    m_communication->setIntervalMiliSec(COM_THREAD_CYCLE_MONITOR_TIME);
    m_communication->start();
    m_communication->timerStart(2);
#if 0
    stModBusInitInfo modbusInitInfo;
    modbusInitInfo.aplInfo = aplInfo;
    modbusInitInfo.portNum = portNum;
    modbusInitInfo.serverAddress = serverAddress;
    modbusInitInfo.serverIPAddress = serverIPAddress;
    if (m_modbusCommunication) { m_modbusCommunication->modbusCommunicationInit(modbusInitInfo); }
#endif
}

void ModbusManager::stopCommunication()
{
    if (!m_communication->isRunning())
        return;

    m_communication->timerStop();
    m_communication->quit();
    m_communication->wait();
    CommunicationThread::destroy_instance();
    m_communication = nullptr; // should be deleted by the worker thread
    m_modbusCommunication->modbusCommunicationTerminate();
    ModbusCommunication::destroy_instance();
    m_modbusCommunication = nullptr;
}

void ModbusManager::startModbusCommunication(QString applicationName)
{
    stModBusInitInfo modbusInitInfo;
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
    int portNum = 0;
    int serverAddress = 0;
    int execCode = 0;
#endif // ENABLE_COMMUNICATION_DUAL_PORT
    CoreLibSdControl sdControl;
    QString aplInfo = applicationName;
    QString serverIPAddress = sdControl.getControllerData("System", "IPAddress", "127.0.0.1").toString();  // default IP
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
#ifdef REAL_TARGET
    execCode = QProcess::execute("ping", QStringList() << "-c" << "1" << serverIPAddress);
#else
#ifdef Q_OS_LINUX
    execCode = QProcess::execute("ping", QStringList() << "-c" << "1" << serverIPAddress);
#else // Q_OS_LINUX
    execCode = QProcess::execute("ping", QStringList() << "-n" << "1" << serverIPAddress);
#endif // Q_OS_LINUX
#endif // REAL_TARGET
    if (execCode == 0){
        portNum = sdControl.getPendantData("Modbus", "PortNo", 502).toInt();  // default port number
        serverAddress = sdControl.getPendantData("Modbus", "ServerAddress", 1).toInt();  // default server address
    } else {
        serverIPAddress = sdControl.getControllerData("System", "IPAddress2", "127.0.0.1").toString();  // default IP
        portNum = sdControl.getPendantData("Modbus2", "PortNo", 502).toInt();  // default port number
        serverAddress = sdControl.getPendantData("Modbus2", "ServerAddress", 1).toInt();  // default server address
    }
#ifdef REAL_TARGET
    qInfo() << "execCode:" << execCode << "serverIPAddress:" << serverIPAddress << "portNum:" << portNum << "serverAddress:" << serverAddress;
#endif // REAL_TARGET
#else // ENABLE_COMMUNICATION_DUAL_PORT
    int portNum = sdControl.getPendantData("Modbus", "PortNo", 502).toInt();  // default port number
    int serverAddress = sdControl.getPendantData("Modbus", "ServerAddress", 1).toInt();  // default server address
#endif // ENABLE_COMMUNICATION_DUAL_PORT

    modbusInitInfo.aplInfo = aplInfo;
    modbusInitInfo.portNum = portNum;
    modbusInitInfo.serverAddress = serverAddress;
    modbusInitInfo.serverIPAddress = serverIPAddress;
    modbusInitInfo.timeout = m_timeout;
    modbusInitInfo.retry = m_retry;
    if (m_modbusCommunication) {
        m_modbusCommunication->modbusCommunicationInit(modbusInitInfo);
        m_modbusCommunication->syncModbusCommunicationStart();
    }
}

void ModbusManager::stopModbusCommunication()
{
    if (m_modbusCommunication) {
        m_modbusCommunication->syncModbusCommunicationStop();
    }
}

void ModbusManager::setTimeoutAndRetry(ResourceManager* resourceManager)
{
    bool is_ok;

    if (!resourceManager){
        qCritical("%s:%d resourceManager is invalid", __func__, __LINE__);
        return;
    }

    QString s_timeout = resourceManager->getPenSetStringValue("1", 17, 0);
    QString s_retry = resourceManager->getPenSetStringValue("1", 18, 0);
    qInfo() << "timeout:" << s_timeout;
    qInfo() << "retry:" << s_retry;
    int timeout = s_timeout.toInt(&is_ok, 10);
    if (!is_ok){
        qDebug() << "failed is_ok:" << is_ok << "timeout:" << s_timeout;
        return;
    }
    int retry = s_retry.toInt(&is_ok, 10);
    if (!is_ok){
        qDebug() << "failed is_ok:" << is_ok << "retry:" << s_retry;
        return;
    }

    m_timeout = timeout;
    m_retry = retry;
}

void ModbusManager::update(stModBusCommunicationData command)
{
    switch(command.funcCode) {
    case QModbusPdu::FunctionCode::ReadCoils:
        parseModbusCommandHex01(command);
        break;
    case QModbusPdu::FunctionCode::ReadDiscreteInputs:
        parseModbusCommandHex02(command);
        if(m_isInitial) {
            if((!getReceiveDataDI(31))
                && (!getReceiveDataDI(28))
                && (!getReceiveDataDI(30))) {
                emit updateModbusManualPressed();
            }
            m_isInitial = false;
        }
        break;
    case QModbusPdu::FunctionCode::ReadHoldingRegisters:
        parseModbusCommandHex03(command);
        break;
    case QModbusPdu::FunctionCode::ReadInputRegisters:
        if (command.isSummaryBulkMode){
            parseSummaryModbusCommandHex04(command);//解析画面125words数据
        } else {
            parseModbusCommandHex04(command);
        }
#ifdef REAL_TARGET
        if(!m_isFirstReadCommonModbus
            && command.readAddress == COMMON_MODBUS_ADDR_TOP_HR){
            //qDebug("%s:%d funcCode[0x%02x] readAddress[%d] comSize[%d]", __func__, __LINE__, command.funcCode, command.readAddress, command.comSize);
            m_isFirstReadCommonModbus = true;
            emit isFirstReadCommonModbusChanged(m_isFirstReadCommonModbus);
        }
#endif // REAL_TARGET
        break;
    case QModbusPdu::FunctionCode::WriteSingleCoil:
        break;
    case QModbusPdu::FunctionCode::WriteSingleRegister:
        if(command.writeAddress == AXIS_NUM_READY_ADDR_HR){
            QList<int> writeData;
            writeData.clear();
            writeData.append(command.writeBuf[0]);
            m_isAxisWriteReady = false;
            emit endModbusAxisWriteReady(writeData);
        }
        break;
    case QModbusPdu::FunctionCode::WriteMultipleCoils:
        if(command.writeAddress == MAIN_SCREEN_ID_TOP_DO){
            int mainScreenId = 0;

            for(int i = 0; i < MAIN_SCREEN_ID_BIT_NUM; i++){
                mainScreenId += (command.writeBuf[i] << i);
            }

            m_isMainScreenWrote = true;
            m_mainScreenId = mainScreenId;
        }
        else if(command.writeAddress == SUB_SCREEN_ID_TOP_DO){
            int subScreenId = 0;

            for(int i = 0; i < SUB_SCREEN_ID_BIT_NUM; i++){
                subScreenId += (command.writeBuf[i] << i);
            }

            m_isSubScreenWrote = true;
            m_subScreenId = subScreenId;
        }
        if(m_isMainScreenWrote == true && m_isSubScreenWrote == true){
            m_isMainScreenWrote = false;
            m_isSubScreenWrote = false;
            m_isPageChanging = false;
            emit endModbusPageChange(m_mainScreenId, m_subScreenId);
        }
        break;
    case QModbusPdu::FunctionCode::WriteMultipleRegisters:{
        if(command.writeAddress == SUMMARY_BULK_READY_ADDR_TOP_HR){
            QList<int> writeData;
            writeData.clear();
            writeData.append(command.writeBuf[0]);
            writeData.append(command.writeBuf[1]);
            writeData.append(command.writeBuf[2]);
            m_isPageWriteReady = false;
            emit endModbusPageWriteReady(writeData);
        }
        break;
    }
    default:
        break;
    }
}

void ModbusManager::parseModbusCommandHex01(stModBusCommunicationData &command)
{
    quint32 baseAddress = command.readAddress;
    bool isComSizeRemainder = false;
    int comUnitCnt = (command.comSize / 16 == 0 ? 1 : \
        (command.comSize % 16 == 0 ? command.comSize / 16 : command.comSize / 16 + 1));     // bit unit from comSize(word base)
    quint32 addrDO;
    int i = 0, j = 0;

    isComSizeRemainder = (command.comSize % 16 == 0 ? false : true);
    for (i = 0; i < comUnitCnt; i++){
        if (isComSizeRemainder){
            if (i == comUnitCnt - 1){
                for (j = 0; j < command.comSize % 16; j++){
                    addrDO = baseAddress + (i * 16 + j);
                    m_receiveDataMapDO[addrDO] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                }
            } else {
                for (j = 0; j < 16; j++){
                    addrDO = baseAddress + (i * 16 + j);
                    m_receiveDataMapDO[addrDO] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                }
            }
        } else {
            for (j = 0; j < (command.comSize)/comUnitCnt; j++){
                addrDO = baseAddress + (i * 16 + j);
                m_receiveDataMapDO[addrDO] = ((command.readBuf[i % 16] & (1 << j)) >> j);
            }
        }
    }
}

void ModbusManager::parseModbusCommandHex02(stModBusCommunicationData &command)
{
    quint32 baseAddress = command.readAddress;
    int mapIndex = 0;
    if (command.isBulkMode){
        mapIndex = getBulkModbusMapIndex(command);
        if (mapIndex < 0){
            qDebug("%s:%d funcCode[0x%02x] readAddress[%d] comSize[%d]",
                __func__, __LINE__, command.funcCode, command.readAddress, command.comSize);
            return;
        }
    }
    bool isComSizeRemainder = false;
    int comUnitCnt = (command.comSize / 16 == 0 ? 1 : \
        (command.comSize % 16 == 0 ? command.comSize / 16 : command.comSize / 16 + 1));     // bit unit from comSize(word base)
    quint32 addrDI;
    int i = 0, j = 0;

    isComSizeRemainder = (command.comSize % 16 == 0 ? false : true);
    for (i = 0; i < comUnitCnt; i++){
        if (isComSizeRemainder){
            if (i == comUnitCnt - 1){
                for (j = 0; j < command.comSize % 16; j++){
                    //addrDI = baseAddress + (i * 16 + j);
                    addrDI = ((command.isBulkMode == true) ? BULK_ADDR_COVERSION_MAP_DI.at(i * 16 + mapIndex + j).value(1) : \
                                                             baseAddress + (i * 16 + j));
                    m_receiveDataMapDI[addrDI] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                    if(addrDI != baseAddress + (i * 16 + j)){
                        m_receiveDataMapDI[baseAddress + (i * 16 + j)] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                    }
                }
            } else {
                for (j = 0; j < 16; j++){
                    //addrDI = baseAddress + (i * 16 + j);
                    addrDI = ((command.isBulkMode == true) ? BULK_ADDR_COVERSION_MAP_DI.at(i * 16 + mapIndex + j).value(1) : \
                                                             baseAddress + (i * 16 + j));
                    m_receiveDataMapDI[addrDI] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                    if(addrDI != baseAddress + (i * 16 + j)){
                        m_receiveDataMapDI[baseAddress + (i * 16 + j)] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                    }
                }
            }
        } else {
            for (j = 0; j < (command.comSize)/comUnitCnt; j++){
                //addrDI = baseAddress + (i * 16 + j);
                addrDI = ((command.isBulkMode == true) ? BULK_ADDR_COVERSION_MAP_DI.at(i * 16 + j).value(1) : \
                                                             baseAddress + (i * 16 + j));
                m_receiveDataMapDI[addrDI] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                if(addrDI != baseAddress + (i * 16 + j)){
                    m_receiveDataMapDI[baseAddress + (i * 16 + j)] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                }
            }
        }
    }
}

void ModbusManager::parseModbusCommandHex03(stModBusCommunicationData &command)
{
    quint32 baseAddress = command.readAddress;
    for(int i = 0; i < command.comSize; i++) {
        quint32 addrHR = baseAddress + i;
        m_receiveDataMapHR[addrHR] = command.readBuf[i];
    }
}

void ModbusManager::parseModbusCommandHex04(stModBusCommunicationData &command)
{
    quint32 baseAddress = command.readAddress;
    int mapIndex = 0;
    if (command.isBulkMode){
        mapIndex = getBulkModbusMapIndex(command);
        if (mapIndex < 0){
            qDebug("%s:%d funcCode[0x%02x] readAddress[%d] comSize[%d]",
                __func__, __LINE__, command.funcCode, command.readAddress, command.comSize);
            return;
        }
    }
    for(int i = mapIndex; i < command.comSize; i++) {
        //quint32 addrRG = baseAddress + i;
        quint32 addrRG = ((command.isBulkMode == true) ? BULK_ADDR_COVERSION_MAP_SR.at(i).value(1) : \
                                                         baseAddress + i);
        m_receiveDataMapRG[addrRG] = command.readBuf[i];
        if(addrRG != baseAddress + i){
            m_receiveDataMapRG[baseAddress + i] = command.readBuf[i];
        }
    }
}

int ModbusManager::getBulkModbusMapIndex(stModBusCommunicationData &command)
{
    int mapIndex = -1;
    switch(command.funcCode) {
    case QModbusPdu::FunctionCode::ReadCoils:               // 0x01
        break;
    case QModbusPdu::FunctionCode::ReadDiscreteInputs:      // 0x02
        for(int i = 0; i < BULK_ADDR_COVERSION_MAP_DI.count(); i++){
            if(BULK_ADDR_COVERSION_MAP_DI.at(i).value(0) == command.readAddress){
                mapIndex = i;
                break;
            }
        }
        break;
    case QModbusPdu::FunctionCode::ReadHoldingRegisters:    // 0x03
        break;
    case QModbusPdu::FunctionCode::ReadInputRegisters:      // 0x04
        for(int i = 0; i < BULK_ADDR_COVERSION_MAP_SR.count(); i++){
            if(BULK_ADDR_COVERSION_MAP_SR.at(i).value(0) == command.readAddress){
                mapIndex = i;
                break;
            }
        }
        break;
    case QModbusPdu::FunctionCode::WriteSingleCoil:
        break;
    case QModbusPdu::FunctionCode::WriteSingleRegister:
        break;
    case QModbusPdu::FunctionCode::WriteMultipleCoils:
        break;
    case QModbusPdu::FunctionCode::WriteMultipleRegisters:
        break;
    default:
        break;
    }
    return mapIndex;
}

bool ModbusManager::getReceiveDataDI(quint16 key) {
    return m_receiveDataMapDI.value(key).toBool();
}

bool ModbusManager::getReceiveDataDO(quint16 key) {
    return m_receiveDataMapDO.value(key).toBool();
}

int ModbusManager::getReceiveDataHR(quint16 key) {
    return m_receiveDataMapHR.value(key).toInt();
}

int ModbusManager::getLongReceiveDataHR(quint16 key) {
    int low = m_receiveDataMapHR.value(key).toInt();
    int high = m_receiveDataMapHR.value(key + 1).toInt();
    return (high << 16) + low;
}

int ModbusManager::getReceiveDataRG(quint16 key) {
    return m_receiveDataMapRG.value(key).toInt();
}

short ModbusManager::getWordReceiveDataRG(quint16 key) {
    short result = m_receiveDataMapRG.value(key).toInt();
    return result;
}

int ModbusManager::getLongReceiveDataRG(quint16 key) {
    int low = m_receiveDataMapRG.value(key).toInt();
    int high = m_receiveDataMapRG.value(key + 1).toInt();
    return (high << 16) + low;
}

int ModbusManager::getUserLevel()
{
    int userLevel = (int)((m_receiveDataMapRG.value(2722).toInt() & 0x0F00) >> 8); // SR2722
    return userLevel;
}

int ModbusManager::getLanguage() {
    int language = (int)((m_receiveDataMapRG.value(2722).toInt() & 0xF000) >> 12); // SR2722
    return language;
}

int ModbusManager::getModbusConnectStatus() {
    if(m_communication != nullptr) {
        return m_communication->getModbusConnectStatus();
    }
    return (int)QModbusDevice::UnconnectedState;
}

int ModbusManager::getModbusComConnectStatus()
{
    if (m_modbusCommunication != nullptr){
        return m_modbusCommunication->getModbusConnectStatus();
    }
    return (int)QModbusDevice::UnconnectedState;
}

void ModbusManager::onTimeout(int communicationAbnormalStatus)
{
    emit communicationAbnormalStatusChanged(communicationAbnormalStatus);
}

void ModbusManager::setCommunicationTimeoutError(bool communicationTimeoutError)
{
    if(m_communicationTimeoutError != communicationTimeoutError){
        m_communicationTimeoutError = communicationTimeoutError;
        emit communicationTimeoutErrorChanged(m_communicationTimeoutError);
    }
}

void ModbusManager::onTimeoutError()
{
    setCommunicationTimeoutError(true);
}

void ModbusManager::reConnect() {
    if(m_communication != nullptr) {
        m_communication->reConnect();
    }
}

void ModbusManager::setRequestPage(Enums::Pages page)
{
    m_requestPage = page;
    if(m_communication != nullptr) m_communication->setRequestPage(m_requestPage);
}

void ModbusManager::updatePageCommand(Enums::Pages page, int subInfo)
{
    if (page < Enums::Pages::InvalidPage || page >= Enums::Pages::pageMax){
        qDebug("%s:%d page[%d] is invalid", __func__, __LINE__, page);
        return;
    }
    bool isWriteReady = true;
    if (SUMMARY_BULK_ADDR_COMMAND_INFO.at(page).value(0) == 0
        && SUMMARY_BULK_ADDR_COMMAND_INFO.at(page).value(1) == 0
        && SUMMARY_BULK_ADDR_COMMAND_INFO.at(page).value(2) == 0){
        isWriteReady = false;
    }
    if(m_communication != nullptr) {
        //qDebug("%s:%d m_requestPage[%d] page[%d]", __func__, __LINE__, m_requestPage, page);
        if (m_requestPage <= Enums::Pages::InvalidPage) {
            m_communication->invalidCommonModbusCmd();
        } else {
            m_communication->resetPageReadCommandList(m_requestPage);
        }
        setRequestPage(page);
        QList<int> writeData;
        writeData.clear();
        // MODBUSページごとコマンド書込み準備
        if (isWriteReady){
            for(int i = 0; i < SUMMARY_BULK_READY_ADDR_SIZE; i++){
                if (i == 1 && subInfo > 0){
                    writeData.append((subInfo - 1));
                } else {
                    writeData.append((int)SUMMARY_BULK_ADDR_COMMAND_INFO.at(page).value(i));
                }
            }
            stModBusCommunicationData writeCommand =
                ModbusCommandGenerator::makeModbusWriteCommand(
                    QModbusPdu::FunctionCode::WriteMultipleRegisters, SUMMARY_BULK_READY_ADDR_TOP_HR, SUMMARY_BULK_READY_ADDR_SIZE, writeData);
            m_communication->setModbusWriteCommand(writeCommand);
        }
        stModBusCommunicationData readCommand =
            ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadInputRegisters,
                (quint16)(SUMMARY_BULK_ADDR_COMMAND_INFO.at(page).value(3)),
                (quint16)(SUMMARY_BULK_ADDR_COMMAND_INFO.at(page).value(4))
            );
        readCommand.isSummaryBulkMode = true;
        m_communication->setPageReadCommandList(m_requestPage, readCommand);
    }
}

void ModbusManager::parseSummaryModbusCommandHex04(stModBusCommunicationData &command)
{
    quint32 baseAddress = command.readAddress;
    //qDebug("%s:%d m_requestPage[%d]", __func__, __LINE__, m_requestPage);
    QList<short> commandDIList = SUMMARY_BULK_ADDR_DI_COVERSION_SR_LIST.at(m_requestPage);
    QList<short> commandDOList = SUMMARY_BULK_ADDR_DO_COVERSION_SR_LIST.at(m_requestPage);
    int curCheckDICnt = 0;
    int curCheckDOCnt = 0;
    stModBusCommunicationData data;
    for(int i = 0; i < command.comSize; i++) {
        int targetDIAddr = commandDIList.value(curCheckDICnt);
        int targetDIIndex = targetDIAddr - command.readAddress;
        int targetDOAddr = commandDOList.value(curCheckDOCnt);
        int targetDOIndex = targetDOAddr - command.readAddress;
        int DIPattern = SUMMARY_BULK_DI_PATTERN_INFO.at(m_requestPage).value(curCheckDICnt);
        int DOPattern = SUMMARY_BULK_DO_PATTERN_INFO.at(m_requestPage).value(curCheckDOCnt);
        int targetDISize = 0;
        int DIBitUnit = 0;
        QList<QList<short>> addrDIList;
        int targetDOSize = 0;
        int DOBitUnit = 0;
        QList<QList<short>> addrDOList;

        if (DIPattern >= 0){
            targetDISize = SUMMARY_BULK_DI_PATTERN_TO_WORD_SIZE(DIPattern);
            DIBitUnit = SUMMARY_BULK_DI_BIT_UNIT(DIPattern);
            addrDIList = SUMMARY_BULK_ADDR_DI_LIST.at(DIPattern);
        }
        if (DOPattern >= 0){
            targetDOSize = SUMMARY_BULK_DO_PATTERN_TO_WORD_SIZE(DOPattern);
            DOBitUnit = SUMMARY_BULK_DO_BIT_UNIT(DOPattern);
            addrDOList = SUMMARY_BULK_ADDR_DO_LIST.at(DOPattern);
        }
        if ((DIPattern >= 0 && targetDIIndex >= 0) && i >= targetDIIndex && i < targetDIIndex + targetDISize){
            //qDebug("%s:%d i[%d] targetDIIndex[%d] targetDISize[%d]", __func__, __LINE__, i, targetDIIndex, targetDISize);
            data.isBulkMode = false;
            data.readAddress = 0;       // DI addr get from table
            data.comSize = (DIBitUnit != addrDIList.at(i - targetDIIndex).count() ?
                            (ushort)(addrDIList.at(i - targetDIIndex).count()) : (ushort)DIBitUnit);
            data.readBuf[0] = command.readBuf[i];
            parseSummaryModbusCommandHex02(data, DIPattern, i - targetDIIndex);
            if (i == (targetDIIndex + targetDISize - 1)){
                curCheckDICnt++;
            }
            quint32 addrRG = baseAddress + i;
            m_receiveDataMapRG[addrRG] = command.readBuf[i];
            if(addrRG != baseAddress + i){
                m_receiveDataMapRG[baseAddress + i] = command.readBuf[i];
            }
        } else if ((DOPattern >= 0 && targetDOIndex >= 0) && i >= targetDOIndex && i < targetDOIndex + targetDOSize){
            //qDebug("%s:%d i[%d] targetDOIndex[%d] targetDOSize[%d]", __func__, __LINE__, i, targetDOIndex, targetDOSize);
            data.isBulkMode = false;
            data.readAddress = 0;   // DO addr get from table
            data.comSize = (DOBitUnit != addrDOList.at(i - targetDOIndex).count() ?
                            (ushort)(addrDOList.at(i - targetDOIndex).count()) : (ushort)DOBitUnit);
            data.readBuf[0] = command.readBuf[i];
            parseSummaryModbusCommandHex01(data, DOPattern, i - targetDOIndex);
            if (i == (targetDOIndex + targetDOSize - 1)){
                curCheckDOCnt++;
            }
            quint32 addrRG = baseAddress + i;
            m_receiveDataMapRG[addrRG] = command.readBuf[i];
            if(addrRG != baseAddress + i){
                m_receiveDataMapRG[baseAddress + i] = command.readBuf[i];
            }
        } else {
            quint32 addrRG = (command.isBulkMode == true) ? BULK_ADDR_COVERSION_MAP_SR.at(i).value(1) : baseAddress + i;
            m_receiveDataMapRG[addrRG] = command.readBuf[i];
            if(addrRG != baseAddress + i){
                m_receiveDataMapRG[baseAddress + i] = command.readBuf[i];
            }
        }
    }
}//解析每个画面125words批量读的数据

void ModbusManager::parseSummaryModbusCommandHex02(stModBusCommunicationData &command, int DIPattern, int DIListIndex)
{
    QList<QList<short>> addrDIList = SUMMARY_BULK_ADDR_DI_LIST.at(DIPattern);
    bool isComSizeRemainder = false;
    int comUnitCnt = (command.comSize / 16 == 0 ? 1 : \
        (command.comSize % 16 == 0 ? command.comSize / 16 : command.comSize / 16 + 1));     // bit unit from comSize(word base)
    quint32 addrDI;
    int i = 0, j = 0;

    isComSizeRemainder = (command.comSize % 16 == 0 ? false : true);
    for (i = 0; i < comUnitCnt; i++){
        if (isComSizeRemainder){
            if (i == comUnitCnt - 1){
                for (j = 0; j < command.comSize % 16; j++){
                    addrDI = addrDIList.at(DIListIndex).value(j);
                    m_receiveDataMapDI[addrDI] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                }
            } else {
                for (j = 0; j < 16; j++){
                    addrDI = addrDIList.at(DIListIndex).value(j);
                    m_receiveDataMapDI[addrDI] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                }
            }
        } else {
            for (j = 0; j < (command.comSize)/comUnitCnt; j++){
                addrDI = addrDIList.at(DIListIndex).value(j);
                m_receiveDataMapDI[addrDI] = ((command.readBuf[i % 16] & (1 << j)) >> j);
            }
        }
    }
}

void ModbusManager::parseSummaryModbusCommandHex01(stModBusCommunicationData &command, int DOPattern, int DOListIndex)
{
    QList<QList<short>> addrDOList = SUMMARY_BULK_ADDR_DO_LIST.at(DOPattern);
    bool isComSizeRemainder = false;
    int comUnitCnt = (command.comSize / 16 == 0 ? 1 : \
        (command.comSize % 16 == 0 ? command.comSize / 16 : command.comSize / 16 + 1));     // bit unit from comSize(word base)
    quint32 addrDO;
    int i = 0, j = 0;

    isComSizeRemainder = (command.comSize % 16 == 0 ? false : true);
    for (i = 0; i < comUnitCnt; i++){
        if (isComSizeRemainder){
            if (i == comUnitCnt - 1){
                for (j = 0; j < command.comSize % 16; j++){
                    addrDO = addrDOList.at(DOListIndex).value(j);
                    m_receiveDataMapDO[addrDO] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                }
            } else {
                for (j = 0; j < 16; j++){
                    addrDO = addrDOList.at(DOListIndex).value(j);
                    m_receiveDataMapDO[addrDO] = ((command.readBuf[i % 16] & (1 << j)) >> j);
                }
            }
        } else {
            for (j = 0; j < (command.comSize)/comUnitCnt; j++){
                addrDO = addrDOList.at(DOListIndex).value(j);
                m_receiveDataMapDO[addrDO] = ((command.readBuf[i % 16] & (1 << j)) >> j);
            }
        }
    }
}

void ModbusManager::startIsPageChanging()
{
    m_isMainScreenWrote = false;
    m_isSubScreenWrote = false;
    m_isPageChanging = true;
    m_isPageWriteReady = true;
}

void ModbusManager::startIsAxisChanging()
{
    m_isAxisWriteReady = true;
}

void ModbusManager::setCommunicationReplyAbortedError(bool communicationReplyAbortedError)
{
    if(m_communicationReplyAbortedError != communicationReplyAbortedError){
        m_communicationReplyAbortedError = communicationReplyAbortedError;

        emit communicationReplyAbortedErrorChanged(m_communicationReplyAbortedError);
    }
}

void ModbusManager::onReplyAbortedError()
{
    setCommunicationReplyAbortedError(true);
}

void ModbusManager::onlinkStateChange(bool isonoff)
{
    if(isonoff){
        emit linkchange(true);
    }else {
        emit linkchange(false);
    }
}

ModbusManager* ModbusManager::instance = nullptr;
