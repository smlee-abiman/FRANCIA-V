#include "communicationthread.h"
#include <QEventLoop>
#include <QMutexLocker>
#include "commonviewmodel.h"
#include <QDebug>

const int JOG_KEY_RELEASE_COMMAND_SEND_COUNT = 2;

Q_LOGGING_CATEGORY(sharedcontrol, "sharedcontrol")

CommunicationThread::CommunicationThread(QObject* parent) : QThread(parent)
{
    initialize();
}

void CommunicationThread::initialize()
{
    m_modbusInitInfo.aplInfo = "";
    m_modbusInitInfo.portNum = 0;
    m_modbusInitInfo.serverAddress = 0;
    m_modbusInitInfo.serverIPAddress = "";
    m_intervalMiliSec = 0;
    m_phykeymanager = new PhyKeyManager();
    makeJogKeyReleaseCommand();
    connect(m_phykeymanager, SIGNAL(updateJogCounterInfo(int)),
        this, SLOT(onUpdateJogCounterInfo(int)), Qt::DirectConnection);
    connect(m_phykeymanager, SIGNAL(updateOperableSwInfo(bool)),
        this, SLOT(onUpdateOperableSwInfo(bool)));
    connect(m_phykeymanager, SIGNAL(updateReleaseOperableSwInfo(bool)),
        this, SLOT(onUpdateReleaseOperableSwInfo(bool)));
}

void CommunicationThread::setModbusInfo(QString aplInfo, int portNo, int serverAddress, QString ipAddr, int timeout, int retry)
{
    m_modbusInitInfo.aplInfo = aplInfo;
    m_modbusInitInfo.portNum = portNo;
    m_modbusInitInfo.serverAddress = serverAddress;
    m_modbusInitInfo.serverIPAddress = ipAddr;
    m_modbusInitInfo.timeout = timeout;
    m_modbusInitInfo.retry = retry;
}

void CommunicationThread::setIntervalMiliSec(int msec)
{
    m_intervalMiliSec = msec;
}

void CommunicationThread::setRequestPage(Enums::Pages page)
{
    m_requestPage = page;
}

void CommunicationThread::onUpdateJogCounterInfo(int jogCounter)
{
    QList<int> writeData;
    writeData.append(jogCounter);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister, 0x075F, 0x0001, writeData);      // HR1887
    setJogModbusCommand(command);
}

void CommunicationThread::onUpdateOperableSwInfo(bool operableSwInfo)
{
    if (operableSwInfo){
        writeModbusDO(OPERABLE_SW_STATUS_DO, operableSwInfo);
    }
}
void CommunicationThread::onUpdateReleaseOperableSwInfo(bool operableSwReleaseInfo)
{
    if (!operableSwReleaseInfo){
        writeModbusDO(OPERABLE_SW_STATUS_DO, operableSwReleaseInfo);
    }
}

void CommunicationThread::setModbusWriteCommand(stModBusCommunicationData &command)
{
    QMutexLocker locker(&m_modbusWriteCommandListMutex);
    m_modbusWriteCommandList.append(command);
}

void CommunicationThread::setOneShotReadCommand(stModBusCommunicationData &command)
{
    QMutexLocker locker(&m_oneShotReadCommandListMutex);
    m_oneShotReadCommandList.append(command);
}

void CommunicationThread::setOneShotModbusCaller(OneShotReadModBusCaller caller)
{
    oneShotReadCaller = caller;
}
void CommunicationThread::setOneShotModbusCommand(stModBusCommunicationData &command)
{
    QMutexLocker locker(&m_oneShotModbusCommandListMutex);
    m_oneShotModbusCommandList.append(command);
}

void CommunicationThread::setPageReadCommandList(Enums::Pages page, stModBusCommunicationData &command)
{
    QMutexLocker locker(&m_pageReadCommandListMutex);
    m_pageReadCommandList[page].append(command);
}

void CommunicationThread::resetPageReadCommandList(Enums::Pages page)
{
    QMutexLocker locker(&m_pageReadCommandListMutex);
    m_pageReadCommandList[page].clear();
}

void CommunicationThread::setDelayCommandList(Enums::Pages page, stModBusCommunicationData &command)
{
    QMutexLocker locker(&m_delayCommandListMutex);

    stDelayModbusDataSet dataSet;
    dataSet.commandList.append(command);

    m_delayCommandList[page].append(dataSet);
}

void CommunicationThread::setDelayCommandList(Enums::Pages page, stModBusCommunicationData &writeCommand, stModBusCommunicationData &readCommand)
{
    QMutexLocker locker(&m_delayCommandListMutex);

    stDelayModbusDataSet dataSet;
    dataSet.commandList.append(writeCommand);
    dataSet.commandList.append(readCommand);

    m_delayCommandList[page].append(dataSet);
}

void CommunicationThread::setJogModbusCommand(stModBusCommunicationData &command)
{
    QMutexLocker locker(&m_jogCommandMutex);
    m_jogCommand = command;
    m_jogCommandIsWrote = false;
}

void CommunicationThread::removePageReadCommandList(Enums::Pages page, int index)
{
    QMutexLocker locker(&m_pageReadCommandListMutex);
    if (index < 0 || index >= m_pageReadCommandList[page].count()){
        qCritical("%s:%d index[%d] is invalid pageList count[%d]",
            __func__, __LINE__, index, m_pageReadCommandList[page].count());
        return;
    }
    m_pageReadCommandList[page].removeAt(index);
}

void CommunicationThread::setCommonPageReadCommandList(stModBusCommunicationData &command)
{
    QMutexLocker locker(&m_commonPageReadCommandListMutex);
    m_commonPageReadCommandList.append(command);
}

void CommunicationThread::resetCommonPageReadCommandList()
{
    QMutexLocker locker(&m_commonPageReadCommandListMutex);
    m_commonPageReadCommandList.clear();
}

PhyKeyManager* CommunicationThread::getPhyKeyManagerInstance()
{
    return m_phykeymanager;
}

void CommunicationThread::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()), Qt::DirectConnection);

    m_modbus = new CoreLibModBusCommunication(m_modbusInitInfo);
    connect(m_modbus, SIGNAL(modBusComConnectStatusChanged(int)), this, SLOT(onModBusComConnectStatusChanged(int)), Qt::DirectConnection);
    connect(m_modbus, SIGNAL(modBusComReadStatusChanged(int)), this, SLOT(onModBusComReadStatusChanged(int)), Qt::DirectConnection);

    m_modbus->startModBusCommunication();  // connect controller

    int loopCnt = 0;
    while(m_modBusComConnectStatus != QModbusDevice::ConnectedState && loopCnt < 300){
        QEventLoop loop;
        QTimer::singleShot( 100, &loop, SLOT( quit() ) );
        loop.exec();
        loopCnt++;
    }
    qInfo() << "CommunicationThread::run loopCnt:" << loopCnt;
    if (m_phykeymanager){ m_phykeymanager->initSelectSwitchInfo(); }
    timer.setInterval(m_intervalMiliSec);
    timer.start();

    exec();  // start event loop

    timer.stop();

    m_modbus->stopModBusCommunication();  // disconnect controller

    disconnect(m_modbus, SIGNAL(modBusComReadStatusChanged(int)), this, SLOT(onModBusComReadStatusChanged(int)));
    disconnect(m_modbus, SIGNAL(modBusComConnectStatusChanged(int)), this, SLOT(onModBusComConnectStatusChanged(int)));
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    if (m_phykeymanager){
        delete m_phykeymanager;
        m_phykeymanager = nullptr;
    }
    delete m_modbus;
}

bool CommunicationThread::syncReadCommandList(QList<stModBusCommunicationData> readCommandList){

    for(int i = 0; i < readCommandList.size(); i++) {
        stModBusCommunicationData &command = readCommandList[i];
        bool ret = m_modbus->syncReadModBusData(&command);
        if(ret) {
            feed(command);
        } else {
            qCritical() << "syncReadCommandList read failed." << "funcCode:" << command.funcCode << "readAddress:" << command.readAddress << "comSize:" << command.comSize << "requestPage:" << m_requestPage;
            return false;
        }
    }

    return true;
}

bool CommunicationThread::syncOneShotReadCommandList(QList<stModBusCommunicationData> oneShotReadCommandList){

    for(int i = 0; i < oneShotReadCommandList.size(); i++) {
        stModBusCommunicationData &command = oneShotReadCommandList[i];
        bool ret = m_modbus->syncReadModBusData(&command);
        if(ret) {
            feed(command);
            oneShotReadFinished(command);
        } else {
            qCritical() << "syncOneShotReadCommandList read failed." << "funcCode:" << command.funcCode << "readAddress:" << command.readAddress << "comSize:" << command.comSize << "requestPage:" << m_requestPage;
            return false;
        }
    }

    return true;
}

bool CommunicationThread::syncWriteCommandList(QList<stModBusCommunicationData> writeCommandList){

    if(writeCommandList.size() > 0) {
        for(int i = 0; i < writeCommandList.size(); i++) {
            stModBusCommunicationData &command = writeCommandList[i];
            bool ret = m_modbus->syncWriteModBusData(&command);
            if(ret) {
                feed(command);
            } else {
                qCritical() << "write failed." << "funcCode:" << command.funcCode << "comSize:" << command.comSize << "writeAddress" << command.writeAddress << "requestPage:" << m_requestPage;
                return false;
            }
        }
    }


    return true;
}

bool CommunicationThread::syncOneShotModBusCommandList(QList<stModBusCommunicationData> oneShotModbusCommandList){

    for(int i = 0; i < oneShotModbusCommandList.size(); i++) {
        stModBusCommunicationData &command = oneShotModbusCommandList[i];
        bool ret = false;
        switch(command.funcCode) {
        case QModbusPdu::FunctionCode::ReadCoils:
        case QModbusPdu::FunctionCode::ReadDiscreteInputs:
        case QModbusPdu::FunctionCode::ReadHoldingRegisters:
        case QModbusPdu::FunctionCode::ReadInputRegisters:
            ret = m_modbus->syncReadModBusData(&command);
            if(ret) {
                feed(command);
                oneShotReadFinished(command);
                oneShotReadFinishedIndividualSignal(command, oneShotReadCaller);
            } else {
                qCritical() << "syncOneShotModBusCommandList read failed.";
                return false;
            }
            break;

        case QModbusPdu::FunctionCode::WriteSingleCoil:
        case QModbusPdu::FunctionCode::WriteSingleRegister:
        case QModbusPdu::FunctionCode::WriteMultipleCoils:
        case QModbusPdu::FunctionCode::WriteMultipleRegisters:
            ret = m_modbus->syncWriteModBusData(&command);
            if(ret) {
                feed(command);
            } else {
                qCritical() << "syncOneShotModBusCommandList write failed.";
                return false;
            }
            break;

        default:
            qCritical() << "syncOneShotModBusCommandList not supported function code:" << command.funcCode;
            break;
        }
    }


    return true;
}

bool CommunicationThread::syncReadDebugCommandList(QList<stModBusCommunicationData> readCommandList){

    for(int i = 0; i < readCommandList.size(); i++) {
        stModBusCommunicationData &command = readCommandList[i];
        bool ret = m_modbus->syncReadModBusDebugData(&command);
        if(ret) {
            feed(command);
        } else {
            qCritical() << "syncReadDebugCommandList read failed.";
            return false;
        }
    }

    return true;
}

bool CommunicationThread::syncOneShotReadDebugCommandList(QList<stModBusCommunicationData> oneShotReadCommandList){

    for(int i = 0; i < oneShotReadCommandList.size(); i++) {
        stModBusCommunicationData &command = oneShotReadCommandList[i];
        bool ret = m_modbus->syncReadModBusDebugData(&command);
        if(ret) {
            feed(command);
            oneShotReadFinished(command);
        } else {
            qCritical() << "syncOneShotReadDebugCommandList read failed.";
            return false;
        }
    }


    return true;
}

bool CommunicationThread::syncWriteDebugCommandList(QList<stModBusCommunicationData> writeCommandList){

    if(writeCommandList.size() > 0) {
        for(int i = 0; i < writeCommandList.size(); i++) {
            stModBusCommunicationData &command = writeCommandList[i];
            bool ret = m_modbus->syncWriteModBusDebugData(&command);
            if(ret) {
                feed(command);
            } else {
                qCritical() << "write failed.";
                return false;
            }
        }
    }


    return true;
}

bool CommunicationThread::syncOneShotModBusDebugCommandList(QList<stModBusCommunicationData> oneShotModbusCommandList){

    for(int i = 0; i < oneShotModbusCommandList.size(); i++) {
        stModBusCommunicationData &command = oneShotModbusCommandList[i];
        bool ret = false;
        switch(command.funcCode) {
        case QModbusPdu::FunctionCode::ReadCoils:
        case QModbusPdu::FunctionCode::ReadDiscreteInputs:
        case QModbusPdu::FunctionCode::ReadHoldingRegisters:
        case QModbusPdu::FunctionCode::ReadInputRegisters:
            ret = m_modbus->syncReadModBusDebugData(&command);
            if(ret) {
                feed(command);
                oneShotReadFinished(command);
                oneShotReadFinishedIndividualSignal(command, oneShotReadCaller);
            } else {
                qCritical() << "syncOneShotModBusDebugCommandList read failed.";
                return false;
            }
            break;

        case QModbusPdu::FunctionCode::WriteSingleCoil:
        case QModbusPdu::FunctionCode::WriteSingleRegister:
        case QModbusPdu::FunctionCode::WriteMultipleCoils:
        case QModbusPdu::FunctionCode::WriteMultipleRegisters:
            ret = m_modbus->syncWriteModBusDebugData(&command);
            if(ret) {
                feed(command);
            } else {
                qCritical() << "write failed.";
                return false;
            }
            break;

        default:
            qCritical() << "not supported function code:" << command.funcCode;
            break;
        }
    }


    return true;
}

void CommunicationThread::appendRetryCommandList(QList<stModBusCommunicationData> *destList, QList<stModBusCommunicationRetryData> *srcList)
{
    for(int i = 0; i < srcList->count(); i++){
        destList->append(srcList->at(i).modbusData);
    }
}

void CommunicationThread::checkRetryCommandList(QList<stModBusCommunicationRetryData> *commandList)
{
    int i = 0;
    while(i < commandList->count()){
        stModBusCommunicationRetryData command = commandList->at(i);

        command.retryCount++;

        if(m_commandRetryMax <= command.retryCount){
            commandList->removeAt(i);
        }
        else{
            commandList->replace(i, command);
            i++;
        }
    }
}

void CommunicationThread::appendNewRetryCommandList(QList<stModBusCommunicationRetryData> *destList, QList<stModBusCommunicationData> *srcList)
{
    for(int i = 0; i < srcList->count(); i++){
        stModBusCommunicationRetryData retryCommand;
        retryCommand.modbusData = srcList->at(i);
        retryCommand.retryCount = 0;
        destList->append(retryCommand);
    }
}

void CommunicationThread::onTimeout()
{
    // phykey monitor
#if 1   // 2ms周期で物理キー監視
#else
    m_phykeymanager->monitorByComThreadTriger();
#endif

    QList<stModBusCommunicationData> readCommandList;
    QList<stModBusCommunicationData> oneShotReadCommandList;
    QList<stModBusCommunicationData> writeCommandList;
    QList<stModBusCommunicationData> oneShotModbusCommandList;
    QList<stModBusCommunicationData> retryWriteCommandList;
    QList<stModBusCommunicationData> retryOneShotReadCommandList;
    QList<stModBusCommunicationData> retryOneShotModbusCommandList;

    if(m_modBusComConnectStatus == QModbusDevice::ConnectedState) {
        appendRetryCommandList(&retryWriteCommandList, &m_retryWriteCommandList);
        appendRetryCommandList(&retryOneShotReadCommandList, &m_retryOneShotReadCommandList);
        appendRetryCommandList(&retryOneShotModbusCommandList, &m_retryOneShotModbusCommandList);

        appendDelayCommonModbusCommand(&oneShotModbusCommandList);
        appendCommonModbusCommand(&readCommandList);
        appendPageByCommonModbusCommand(&readCommandList);
        appendPageByPageModbusCommand(&readCommandList);
        appendOneShotReadCommand(&oneShotReadCommandList);
        appendJogModbusCommand(&writeCommandList);
        appendWriteModbusCommand(&writeCommandList);
        appendOneShotModbusCommand(&oneShotModbusCommandList);

        // WriteCommandを先に（ページ変更Modbus通信をページ情報読込より先にする為）
        //-->retry
        if(syncWriteCommandList(retryWriteCommandList)){
            m_retryWriteCommandList.clear();
        }else{
            // retry count check
            checkRetryCommandList(&m_retryWriteCommandList);
            checkRetryCommandList(&m_retryOneShotReadCommandList);
            checkRetryCommandList(&m_retryOneShotModbusCommandList);
            // append retryList
            appendNewRetryCommandList(&m_retryWriteCommandList, &writeCommandList);
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncWriteCommandList failed. (retryWriteCommandList) count[%d]", retryWriteCommandList.count());
            goto onTimeoutEnd;
        }
        //<--retry

        if(!syncWriteCommandList(writeCommandList)){
            // append retryList
            appendNewRetryCommandList(&m_retryWriteCommandList, &writeCommandList);
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncWriteCommandList failed. (writeCommandList) count[%d]", writeCommandList.count());
            goto onTimeoutEnd;
        }

        // ページ情報・共通情報読込
        if(!syncReadCommandList(readCommandList)){
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncReadCommandList failed mModbusTimeOutCnt[%d] m_requestPage[%d]", mModbusTimeOutCnt, m_requestPage);
            if(judgeComStatusNotify()){
                emit modBusComConnectStatusChanged((int)QModbusDevice::UnconnectedState);
            }
            if(judgeSeriousComStatus()){
                goto onReplyAbortedError;
            }
            goto onTimeoutEnd;
        }

        //-->retry
        if(syncOneShotReadCommandList(retryOneShotReadCommandList)){
            m_retryOneShotReadCommandList.clear();
        }else{
            // retry count check
            checkRetryCommandList(&m_retryOneShotReadCommandList);
            checkRetryCommandList(&m_retryOneShotModbusCommandList);
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncOneShotReadCommandList failed. (retryOneShotReadCommandList)");
            goto onTimeoutEnd;
        }

        if(syncOneShotModBusCommandList(retryOneShotModbusCommandList)){
            m_retryOneShotModbusCommandList.clear();
        }else{
            // retry count check
            checkRetryCommandList(&m_retryOneShotModbusCommandList);
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncOneShotModBusCommandList failed. (retryOneShotModbusCommandList)");
            goto onTimeoutEnd;
        }
        //<--retry

        if(!syncOneShotReadCommandList(oneShotReadCommandList)){
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncOneShotReadCommandList failed. (oneShotReadCommandList)");
            goto onTimeoutEnd;
        }

        if(!syncOneShotModBusCommandList(oneShotModbusCommandList)){
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncOneShotModBusCommandList failed. (oneShotModbusCommandList)");
            goto onTimeoutEnd;
        }

        m_connectRetryCount = 0;
    }
    else{
        if(m_connectRetryCount < connectRetry){
            m_modbus->startModBusCommunication();  // connect controller
            m_connectRetryCount++;
        }
        else if(retryDlgShowed == false){
            goto onTimeoutError;
        }
#if false
#ifndef REAL_TARGET
        appendRetryCommandList(&retryWriteCommandList, &m_retryWriteCommandList);
        appendRetryCommandList(&retryOneShotReadCommandList, &m_retryOneShotReadCommandList);
        appendRetryCommandList(&retryOneShotModbusCommandList, &m_retryOneShotModbusCommandList);

        appendCommonModbusCommand(&readCommandList);
        appendPageByCommonModbusCommand(&readCommandList);
        appendPageByPageModbusCommand(&readCommandList);
        appendOneShotReadCommand(&oneShotReadCommandList);
        appendWriteModbusCommand(&writeCommandList);
        appendOneShotModbusCommand(&oneShotModbusCommandList);

        // WriteCommandを先に（ページ変更Modbus通信をページ情報読込より先にする為）
        //-->retry
        if(syncWriteDebugCommandList(retryWriteCommandList)){
            m_retryWriteCommandList.clear();
        }
        else{
            // retry count check
            checkRetryCommandList(&m_retryWriteCommandList);
            checkRetryCommandList(&m_retryOneShotReadCommandList);
            checkRetryCommandList(&m_retryOneShotModbusCommandList);
            // append retryList
            appendNewRetryCommandList(&m_retryWriteCommandList, &writeCommandList);
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncWriteDebugCommandList failed. (retryWriteCommandList)");
            goto onTimeoutEnd;
        }
        //<--retry

        if(!syncWriteDebugCommandList(writeCommandList)){
            // append retryList
            appendNewRetryCommandList(&m_retryWriteCommandList, &writeCommandList);
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncWriteDebugCommandList failed. (writeCommandList)");
            goto onTimeoutEnd;
        }

        // ページ情報・共通情報読込
        if(!syncReadDebugCommandList(readCommandList)){
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncReadDebugCommandList failed");
            goto onTimeoutEnd;
        }

        //-->retry
        if(syncOneShotReadDebugCommandList(retryOneShotReadCommandList)){
            m_retryOneShotReadCommandList.clear();
        }
        else{
            // retry count check
            checkRetryCommandList(&m_retryOneShotReadCommandList);
            checkRetryCommandList(&m_retryOneShotModbusCommandList);
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncOneShotReadDebugCommandList failed. (retryOneShotReadCommandList)");
            goto onTimeoutEnd;
        }

        if(syncOneShotModBusDebugCommandList(retryOneShotModbusCommandList)){
            m_retryOneShotModbusCommandList.clear();
        }
        else{
            // retry count check
            checkRetryCommandList(&m_retryOneShotModbusCommandList);
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncOneShotModBusDebugCommandList failed. (retryOneShotModbusCommandList)");
            goto onTimeoutEnd;
        }
        //<--retry

        if(!syncOneShotReadDebugCommandList(oneShotReadCommandList)){
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotReadCommandList, &oneShotReadCommandList);
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncOneShotReadDebugCommandList failed. (oneShotReadCommandList)");
            goto onTimeoutEnd;
        }

        if(!syncOneShotModBusDebugCommandList(oneShotModbusCommandList)){
            // append retryList
            appendNewRetryCommandList(&m_retryOneShotModbusCommandList, &oneShotModbusCommandList);
            mModbusTimeOutCnt++;
            qCritical("syncOneShotModBusDebugCommandList failed. (oneShotModbusCommandList)");
            goto onTimeoutEnd;
        }
#endif // REAL_TARGET
#endif
    }

    finish();
	updateLinkLed(false);
    judgeModbusConnectState(false);
    return;

onTimeoutError:
    m_modbus->stopModBusCommunication();  // disconnect controller
    if(connectRetry <= m_connectRetryCount && retryDlgShowed == false){
        timeout(m_modBusComConnectStatus);
        retryDlgShowed = true;
    }
    finish();
onTimeoutEnd:
    timeoutError();
	updateLinkLed(true);
    judgeModbusConnectState(true);
    return;
onReplyAbortedError:
    emit replyAbortedError();
	updateLinkLed(true);
    judgeModbusConnectState(true);
    return;
}

void CommunicationThread::appendCommonModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    if(commandList == nullptr) return;
    if (mIsCommonModbusCmd) {
        commandList->append(ModbusCommandGenerator::makeModbusReadCommand(QModbusPdu::FunctionCode::ReadInputRegisters, 0x0A8C, 0x003D, false, true));   // Summary Bulk(SR2700, 61)
    }
}

void CommunicationThread::appendPageByPageModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    if(commandList == nullptr) return;
    QMutexLocker locker(&m_pageReadCommandListMutex);
    commandList->append(m_pageReadCommandList[m_requestPage]);
}

void CommunicationThread::appendOneShotReadCommand(QList<stModBusCommunicationData> *commandList)
{
    if(commandList == nullptr) return;
    QMutexLocker locker(&m_oneShotReadCommandListMutex);
    while(m_oneShotReadCommandList.size() > 0) {
        commandList->append(m_oneShotReadCommandList.takeFirst());
    }
}

void CommunicationThread::appendWriteModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    if(commandList == nullptr) return;
    QMutexLocker locker(&m_modbusWriteCommandListMutex);
    while(m_modbusWriteCommandList.size() > 0) {
        commandList->append(m_modbusWriteCommandList.takeFirst());
    }
}

void CommunicationThread::appendOneShotModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    if(commandList == nullptr) return;
    QMutexLocker locker(&m_oneShotModbusCommandListMutex);
    while(m_oneShotModbusCommandList.size() > 0) {
        commandList->append(m_oneShotModbusCommandList.takeFirst());
    }
}

void CommunicationThread::appendPageByCommonModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    if(commandList == nullptr) return;
    QMutexLocker locker(&m_commonPageReadCommandListMutex);
    commandList->append(m_commonPageReadCommandList);
}

void CommunicationThread::appendDelayCommonModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    if(commandList == nullptr) return;
    QMutexLocker locker(&m_delayCommandListMutex);

    if(m_delayCommandList.isEmpty() == true) return;

    if(m_delayCommandList.contains(m_requestPage) == true){
        while(m_delayCommandList[m_requestPage].isEmpty() == false){
            stDelayModbusDataSet dataSet = m_delayCommandList[m_requestPage].first();

            commandList->append(dataSet.commandList);

            m_delayCommandList[m_requestPage].removeFirst();
        }

        if(m_delayCommandList[m_requestPage].isEmpty() == true){
            m_delayCommandList.remove(m_requestPage);
        }
    }
    else{
        Enums::Pages key = m_delayCommandList.firstKey();
        if(m_delayCommandList[key].isEmpty() == false){
            stDelayModbusDataSet dataSet = m_delayCommandList[key].first();

            commandList->append(dataSet.commandList);

            m_delayCommandList[key].removeFirst();
        }

        if(m_delayCommandList[key].isEmpty() == true){
            m_delayCommandList.remove(key);
        }
    }
}

void CommunicationThread::appendJogModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    QMutexLocker locker(&m_jogCommandMutex);
    if (m_jogCommandIsWrote == true) {
        if (m_jogKeyReleaseCommandSendRequired) {
            if (m_jogKeyReleaseCommandSendCounter >= JOG_KEY_RELEASE_COMMAND_SEND_COUNT) {
                commandList->append(m_jogReleaseCommand);
                m_jogKeyReleaseCommandSendRequired = false;
                m_jogKeyReleaseCommandSendCounter = 0;
            } else {
                m_jogKeyReleaseCommandSendCounter++;
            }
        }
    } else {
        commandList->append(m_jogCommand);
        if (m_jogCommand.writeAddress == MEMBLEM_SW_STATUS_DO) {
            m_jogKeyReleaseCommandSendRequired = true;
            m_jogKeyReleaseCommandSendCounter = 0;
        }
        m_jogCommandIsWrote = true;
    }
}

void CommunicationThread::onModBusComConnectStatusChanged(int state)
{
    qInfo() << "CommunicationThread::onModBusComConnectStatusChanged: state = " << state;
    m_modBusComConnectStatus = state;
    if (state == QModbusDevice::ConnectedState) {
        mModbusTimeOutCnt = 0;
        mIndividualPageModbusTimeOutCnt = 0;
        mIsIndividualPageModbusTimeOutNotify = false;
    }
    emit modBusComConnectStatusChanged(m_modBusComConnectStatus);
}

void CommunicationThread::onModBusComReadStatusChanged(int modBusReadState)
{
    //qDebug() << "CommunicationThread::onModBusComReadStatusChanged: modBusReadState = " << modBusReadState;
    m_modBusReadState = modBusReadState;
}

void CommunicationThread::writeModbusDO(int doNumber, bool isOn)
{
    bool is_ok = false;
    int writeAddress;
    QString result = QString::number( doNumber, 16 );

    writeAddress = result.toInt(&is_ok, 16);
    if (!is_ok){
        qCritical("%s:%d result is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return;
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
    setModbusWriteCommand(command);
}

void CommunicationThread::writeModbusDO(int doNumber, bool isOn, uchar anyValue)
{
    bool is_ok = false;
    int writeAddress;
    QString result = QString::number( doNumber, 16 );

    writeAddress = result.toInt(&is_ok, 16);
    if (!is_ok){
        qCritical("%s:%d result is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return;
    }
    QList<int> writeData;
    if(isOn) {
        writeData.append((int)anyValue);
    } else {
        writeData.append(0x00);
    }
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteSingleCoil, writeAddress, 0x0001, writeData);
    setModbusWriteCommand(command);
}

void CommunicationThread::writeModbusDO(int doNumber, uchar anyValue, int write_size)
{
    bool is_ok = false;
    int writeAddress;
    QString result = QString::number( doNumber, 16 );

    writeAddress = result.toInt(&is_ok, 16);
    if (!is_ok){
        qCritical("%s:%d result is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return;
    }
    QList<int> writeData;
    for(int i = 0; i < write_size; i++){
        writeData.append((int)((anyValue >> i) & 0x01));
    }
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, writeAddress, write_size, writeData);
    setModbusWriteCommand(command);
}

bool CommunicationThread::writeModbusHR(int HrNumber, int *write_data, int write_size)
{
    bool is_ok = false;
    int writeAddress;
    QString hrNumberstr = QString::number( HrNumber, 16 );

    if (!write_data){
        qCritical("%s:%d write_data is invalid num write_size[%d]", __func__, __LINE__, write_size);
        return false;
    }
    writeAddress = hrNumberstr.toInt(&is_ok, 16);
    //qDebug("%s:%d HrNumber[%d] writeAddress[0x%04x]", __func__, __LINE__, HrNumber, writeAddress);
    if (!is_ok){
        qCritical("%s:%d hrNumberstr is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return false;
    }
    QList<int> writeData;
    for (int i = 0; i < write_size; i++){
        writeData.append(write_data[i]);
    }
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, writeAddress, write_size, writeData);
    setModbusWriteCommand(command);
    return true;
}
bool CommunicationThread::writeModbusHR(int HrNumber, QString writeDataStr)
{
    bool is_ok = false;
    int writeAddress = 0, writeSize = 0;

    if (writeDataStr == ""){
        qCritical() << "writeDataStr is Empty:" << writeDataStr;
        return false;
    }
    QString str = QString::number( HrNumber, 16 );
    writeAddress = str.toInt(&is_ok, 16);
    if (!is_ok){
        qCritical("%s:%d str is not num is_ok[%d]", __func__, __LINE__, is_ok);
        return false;
    }
    writeSize = writeDataStr.length();
    if (!is_ok){
        qCritical("%s:%d writeData is not num is_ok[%d]", __func__, __LINE__, is_ok);
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
                qCritical("%s:%d writeDataStr is not num is_ok[%d]", __func__, __LINE__, is_ok);
                continue;
            }
        }
        //qDebug() << "writeDataAscii" << writeDataAscii;
        writeData.append(writeDataAscii);
    }
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleRegisters, writeAddress, writeSize, writeData);
    setModbusWriteCommand(command);
    return true;
}

void CommunicationThread::writeModbusHR(quint16 addr, int value)
{
    QList<int> writeData;
    writeData.append(value);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, addr, 0x0001, writeData);
    setModbusWriteCommand(command);
}

void CommunicationThread::readModbusDI(CommunicationThread::OneShotReadModBusCaller caller, quint16 addr, quint16 size)
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadDiscreteInputs, addr, size);
    setOneShotModbusCommand(caller, command);
}

void CommunicationThread::readModbusHR(CommunicationThread::OneShotReadModBusCaller caller, quint16 addr, quint16 size)
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, addr, size);
    setOneShotModbusCommand(caller, command);
}

void CommunicationThread::setOneShotModbusCommand(CommunicationThread::OneShotReadModBusCaller caller, stModBusCommunicationData &data)
{
    setOneShotModbusCaller(caller);
    setOneShotModbusCommand(data);
}

int CommunicationThread::getModbusConnectStatus()
{
    return m_modBusComConnectStatus;
}

void CommunicationThread::reConnect() {
    m_connectRetryCount = 0;
    retryDlgShowed = false;
}

void CommunicationThread::invalidCommonModbusCmd()
{
    mIsCommonModbusCmd = false;
}

void CommunicationThread::timerStart(int timeOut)
{
    if (!mIsTimerStatus) {
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void CommunicationThread::timerStop()
{
    if (mIsTimerStatus) {
        killTimer(mTimerId);
        mIsTimerStatus = false;
    }
}

void CommunicationThread::updateLinkLed(bool isonoff)
{
    if((isonoff && !m_greenLedState) || (!isonoff && m_greenLedState)){
        m_phykeymanager->ctrlLed(isonoff,CoreLibDriverManager::LedType::LedTypeGreen);
        m_greenLedState = isonoff;
		emit linkStateChange(m_greenLedState);
    }else {
        return;
    }
}

void CommunicationThread::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent) return;
    if(timerEvent->timerId() == mTimerId){
        // phykey monitor
        m_phykeymanager->monitorByTimerTriger();
    }
}

void CommunicationThread::makeJogKeyReleaseCommand()
{
    QList<int> writeData = {1, 1, 1, 1, 1, 1, 1, 1};
    m_jogReleaseCommand = ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, MEMBLEM_SW_RELEASE_DO, MEMBLEM_SW_RELEASE_BIT_NUM, writeData);
}

bool CommunicationThread::judgeComStatusNotify()
{
    bool ret = false;
    if ((m_requestPage == Enums::Pages::SetupChgExportUsb || m_requestPage == Enums::Pages::SetupChgImportUsb)
        && m_modBusReadState == QModbusDevice::TimeoutError){
        mIndividualPageModbusTimeOutCnt++;
    }
    if (mIndividualPageModbusTimeOutCnt >= JUDGE_NOTIFY_TIMEOUT_COUNT
        && !mIsIndividualPageModbusTimeOutNotify){
        qCritical("%s:%d m_modBusReadState[%d] mIndividualPageModbusTimeOutCnt[%d]", __func__, __LINE__,
            m_modBusReadState, mIndividualPageModbusTimeOutCnt);
        ret = true;
        mIndividualPageModbusTimeOutCnt = 0;
        mIsIndividualPageModbusTimeOutNotify = true;
    }
    return ret;
}

bool CommunicationThread::judgeSeriousComStatus()
{
    bool ret = false;
    ret = judgeSeriousSubjectScreen();
    if (ret){
        qCritical("%s:%d m_modBusReadState[%d] m_requestPage[%d]", __func__, __LINE__,
            m_modBusReadState, m_requestPage);
        if (m_modBusReadState != QModbusDevice::ReplyAbortedError){
            ret = false;
        }
    }
    return ret;
}

bool CommunicationThread::judgeSeriousSubjectScreen()
{
    if (m_requestPage == Enums::Pages::TeachingProgram){
        return true;
    } else {
        return false;
    }
}

void CommunicationThread::judgeModbusConnectState(bool isonoff)
{
    if((!m_modbusConnectState && isonoff) || (m_modbusConnectState && !isonoff)){
        m_modbusConnectState = isonoff;
        emit linkStateChange(m_modbusConnectState);
    }else {
        return;
    }
}

CommunicationThread* CommunicationThread::instance = nullptr;
