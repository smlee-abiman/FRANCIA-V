#include "testandsetmanager.h"
#include <QNetworkInterface>

#define cJOGDIAL_SW_BITNo   (18)    // bit18: ジョグダイヤルスイッチ入力
#define cDEADMAN_SW_BITNo   (22)    // bit22: 動作可能スイッチ入力

TestAndSetManager::TestAndSetManager(QObject *parent) : QObject(parent)
{

}

QString TestAndSetManager::getEthernetInterfaceName()
{
    QString interfaceName = "eth0";  // default interface name
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    for(int i = 0; i < interfaceList.count(); i++) {
        QNetworkInterface interface = interfaceList.at(i);
        if((interface.flags() & QNetworkInterface::IsLoopBack) != QNetworkInterface::IsLoopBack) {
            interfaceName = interface.name();
            qDebug() << "Find not loopback interface name = " << interfaceName;
            break;
        }
    }
    return interfaceName;
}

bool TestAndSetManager::isExecuteInputTest() const
{
    return mIsExecuteInputTest;
}

void TestAndSetManager::setIsExecuteInputTest(bool isExecute)
{
    mIsExecuteInputTest = isExecute;
    emit isExecuteInputTestChanged();
}

bool TestAndSetManager::isExecuteOutputTest() const
{
    return mIsExecuteOutputTest;
}

void TestAndSetManager::setIsExecuteOutputTest(bool isExecute)
{
    mIsExecuteOutputTest = isExecute;
    emit isExecuteOutputTestChanged();
}

int TestAndSetManager::backLightValue() const
{
    return mBackLightValue;
}

void TestAndSetManager::setBackLightValue(int value)
{
    mBackLightValue = value;
    emit backLightValueChanged();
}

bool TestAndSetManager::isPressedKey01() const
{
    return mIsPressedKey01;
}

void TestAndSetManager::setIsPressedKey01(bool isPressed)
{
    mIsPressedKey01 = isPressed;
    emit isPressedKey01Changed();
}

bool TestAndSetManager::isPressedKey02() const
{
    return mIsPressedKey02;
}

void TestAndSetManager::setIsPressedKey02(bool isPressed)
{
    mIsPressedKey02 = isPressed;
    emit isPressedKey02Changed();
}

bool TestAndSetManager::isPressedKey03() const
{
    return mIsPressedKey03;
}

void TestAndSetManager::setIsPressedKey03(bool isPressed)
{
    mIsPressedKey03 = isPressed;
    emit isPressedKey03Changed();
}

bool TestAndSetManager::isPressedKey04() const
{
    return mIsPressedKey04;
}

void TestAndSetManager::setIsPressedKey04(bool isPressed)
{
    mIsPressedKey04 = isPressed;
    emit isPressedKey04Changed();
}

bool TestAndSetManager::isPressedKey05() const
{
    return mIsPressedKey05;
}

void TestAndSetManager::setIsPressedKey05(bool isPressed)
{
    mIsPressedKey05 = isPressed;
    emit isPressedKey05Changed();
}

bool TestAndSetManager::isPressedKey06() const
{
    return mIsPressedKey06;
}

void TestAndSetManager::setIsPressedKey06(bool isPressed)
{
    mIsPressedKey06 = isPressed;
    emit isPressedKey06Changed();
}

bool TestAndSetManager::isPressedKey07() const
{
    return mIsPressedKey07;
}

void TestAndSetManager::setIsPressedKey07(bool isPressed)
{
    mIsPressedKey07 = isPressed;
    emit isPressedKey07Changed();
}

bool TestAndSetManager::isPressedKey08() const
{
    return mIsPressedKey08;
}

void TestAndSetManager::setIsPressedKey08(bool isPressed)
{
    mIsPressedKey08 = isPressed;
    emit isPressedKey08Changed();
}

bool TestAndSetManager::isPressedKey09() const
{
    return mIsPressedKey09;
}

void TestAndSetManager::setIsPressedKey09(bool isPressed)
{
    mIsPressedKey09 = isPressed;
    emit isPressedKey09Changed();
}

bool TestAndSetManager::isPressedKey10() const
{
    return mIsPressedKey10;
}

void TestAndSetManager::setIsPressedKey10(bool isPressed)
{
    mIsPressedKey10 = isPressed;
    emit isPressedKey10Changed();
}

bool TestAndSetManager::isPressedKey11() const
{
    return mIsPressedKey11;
}

void TestAndSetManager::setIsPressedKey11(bool isPressed)
{
    mIsPressedKey11 = isPressed;
    emit isPressedKey11Changed();
}

bool TestAndSetManager::isPressedKey12() const
{
    return mIsPressedKey12;
}

void TestAndSetManager::setIsPressedKey12(bool isPressed)
{
    mIsPressedKey12 = isPressed;
    emit isPressedKey12Changed();
}

bool TestAndSetManager::isPressedKey13() const
{
    return mIsPressedKey13;
}

void TestAndSetManager::setIsPressedKey13(bool isPressed)
{
    mIsPressedKey13 = isPressed;
    emit isPressedKey13Changed();
}

bool TestAndSetManager::isPressedKey14() const
{
    return mIsPressedKey14;
}

void TestAndSetManager::setIsPressedKey14(bool isPressed)
{
    mIsPressedKey14 = isPressed;
    emit isPressedKey14Changed();
}

bool TestAndSetManager::isPressedKey15() const
{
    return mIsPressedKey15;
}

void TestAndSetManager::setIsPressedKey15(bool isPressed)
{
    mIsPressedKey15 = isPressed;
    emit isPressedKey15Changed();
}

bool TestAndSetManager::isPressedKey16() const
{
    return mIsPressedKey16;
}

void TestAndSetManager::setIsPressedKey16(bool isPressed)
{
    mIsPressedKey16 = isPressed;
    emit isPressedKey16Changed();
}

bool TestAndSetManager::isPressedKey17() const
{
    return mIsPressedKey17;
}

void TestAndSetManager::setIsPressedKey17(bool isPressed)
{
    mIsPressedKey17 = isPressed;
    emit isPressedKey17Changed();
}

bool TestAndSetManager::isPressedKey18() const
{
    return mIsPressedKey18;
}

void TestAndSetManager::setIsPressedKey18(bool isPressed)
{
    mIsPressedKey18 = isPressed;
    emit isPressedKey18Changed();
}

bool TestAndSetManager::isSelect01() const
{
    return mIsSelect01;
}

void TestAndSetManager::setIsSelect01(bool isSelect01)
{
    mIsSelect01 = isSelect01;
    emit isSelect01Changed();
}

bool TestAndSetManager::isSelect02() const
{
    return mIsSelect02;
}

void TestAndSetManager::setIsSelect02(bool isSelect02)
{
    mIsSelect02 = isSelect02;
    emit isSelect02Changed();
}

bool TestAndSetManager::isSelect03() const
{
    return mIsSelect03;
}

void TestAndSetManager::setIsSelect03(bool isSelect03)
{
    mIsSelect03 = isSelect03;
    emit isSelect03Changed();
}

void TestAndSetManager::setJogDialSwStatus(bool jogDialSwStatus)
{
    if (mJogDialSwStatus == jogDialSwStatus) { return; }
    mJogDialSwStatus = jogDialSwStatus;
    emit jogDialSwStatusChanged(mJogDialSwStatus);
}

void TestAndSetManager::updateJogDialSwStatus(const QBitArray& inputDataBuf)
{
    if (inputDataBuf.size() <= cJOGDIAL_SW_BITNo) { return; }
    bool value = inputDataBuf.at(cJOGDIAL_SW_BITNo);
    // qDebug() << "updateJogDialSwStatus():" << value; // for debug.
    setJogDialSwStatus(!value);
}

void TestAndSetManager::setDeadmanSwStatus(bool deadmanSwStatus)
{
    if (mDeadmanSwStatus == deadmanSwStatus) { return; }
    mDeadmanSwStatus = deadmanSwStatus;
    emit deadmanSwStatusChanged(mDeadmanSwStatus);
}

void TestAndSetManager::updateDeadmanSwStatus(const QBitArray& inputDataBuf)
{
    if (inputDataBuf.size() <= cDEADMAN_SW_BITNo) { return; }
    bool value = inputDataBuf.at(cDEADMAN_SW_BITNo);
    // qDebug() << "updateDeadmanSwStatus():" << value; // for debug.
    setDeadmanSwStatus(!value);
}

QString TestAndSetManager::macAddress() const
{
    return mMacAddress;
}

void TestAndSetManager::setMacAddress(QString macAddress)
{
    mMacAddress = macAddress;
    emit macAddressChanged();
}

QString TestAndSetManager::ipAddress() const
{
    return mIpAddress;
}

void TestAndSetManager::setIpAddress(QString ipAddress)
{
    mIpAddress = ipAddress;
    emit ipAddressChanged();
}

QString TestAndSetManager::subnetMask() const
{
    return mSubnetMask;
}

void TestAndSetManager::setSubnetMask(QString subnetMask)
{
    mSubnetMask = subnetMask;
    emit subnetMaskChanged();
}

QString TestAndSetManager::serialNo() const
{
    return mSerialNo;
}

void TestAndSetManager::setSerialNo(QString serialNo)
{
    mSerialNo = serialNo;
    emit serialNoChanged();
}

int TestAndSetManager::modbusErrorCount() const
{
    return mModbusErrorCount;
}

void TestAndSetManager::setModbusErrorCount(int errorCount)
{
    mModbusErrorCount = errorCount;
    emit modbusErrorCountChanged();
}

QString TestAndSetManager::jogCounter() const
{
    return mJogCounter;
}

void TestAndSetManager::setJogCounter(QString jogCounter)
{
    mJogCounter = jogCounter;
    emit jogCounterChanged();
}

void TestAndSetManager::loadBackLightSetting()
{
    QString backLightStr;
    bool result = mDisplayControl.getLcdBackLightBrightness(&backLightStr);
    if(result) {
        setBackLightValue(backLightStr.toInt());
    } else {
        // mBackLightValue initial value is 8
    }
}

void TestAndSetManager::loadSerialNo()
{
    QString serialNo = "";
    serialNo = mSdControl.getPendantData("System", "SerialNo", "defaultSerialNo").toString();  // default serialNo
    setSerialNo(serialNo);
}

void TestAndSetManager::loadNetworkSetting()
{
    bool result = false;
    QString ipAddr = "";
    QString subnetMask = "";
    QString macAddr = "";

    result = mCoreLibSystemControl.getNetworkSetting(&ipAddr, &subnetMask);
    if(!result) {
        ipAddr = mSdControl.getPendantData("System", "IPAddress", "defaultIP").toString();  // default IP address
        subnetMask = mSdControl.getPendantData("System", "SubnetMask", "defaultSubnet").toString();  // default subnetmask
    }

    QString interfaceName = getEthernetInterfaceName();
    result = mCoreLibSystemControl.getMACAddress(&macAddr, interfaceName);
    if(!result) {
        macAddr = mSdControl.getPendantData("System", "MACAddress", "defaultMAC").toString();  // default MAC address
    }

    setIpAddress(ipAddr);
    setSubnetMask(subnetMask);
    setMacAddress(macAddr);
}

void TestAndSetManager::startTestAndSet()
{
    mDisplayControl.openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeDisplayBrightness);
    loadNetworkSetting();
    loadSerialNo();
    loadBackLightSetting();

    portWatcher = new PortWatcher();
    connect(portWatcher, SIGNAL(updatePortInfo(QBitArray)), this, SLOT(onUpdatePortInfo(QBitArray)));
    portWatcher->start();

    jogWatcher = new JogWatcher();
    connect(jogWatcher, SIGNAL(updateJogInfo(QString)), this, SLOT(onUpdateJogInfo(QString)));
    jogWatcher->start();

    int modbusErrorCount = loadModbusErrorCountFile(mFromAppName);
    setModbusErrorCount(modbusErrorCount);

}

void TestAndSetManager::endTestAndSet()
{
    jogWatcher->quit();
    jogWatcher->wait();
    disconnect(jogWatcher, SIGNAL(updateJogInfo(QString)), this, SLOT(onUpdateJogInfo(QString)));
    delete jogWatcher;
    jogWatcher = nullptr;
    portWatcher->quit();
    portWatcher->wait();
    disconnect(portWatcher, SIGNAL(updatePortInfo(QBitArray)), this, SLOT(onUpdatePortInfo(QBitArray)));
    delete portWatcher;
    portWatcher = nullptr;

    mDisplayControl.closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeDisplayBrightness);
}

void TestAndSetManager::onInputTestExecute()
{
    if(isExecuteInputTest()) {
        setIsExecuteInputTest(false);
    } else {
        setIsExecuteInputTest(true);
    }
}

void TestAndSetManager::outputTestExecute()
{
    if(isExecuteOutputTest()) {
        outputTestThread.quit();
        setIsExecuteOutputTest(false);
    } else {
        outputTestThread.start();
        setIsExecuteOutputTest(true);
    }
}

void TestAndSetManager::onBackLightCountDown()
{
    int backLight = backLightValue();
    if(backLight > 0) {
        int newValue = backLight - 1;
        setBackLightValue(newValue);

        mDisplayControl.setLcdBackLightBrightness(QString::number(newValue));
    }
}

void TestAndSetManager::onBackLightCountUp()
{
    int backLight = backLightValue();
    if(backLight < 8) {
        int newValue = backLight + 1;
        setBackLightValue(newValue);

        mDisplayControl.setLcdBackLightBrightness(QString::number(newValue));
    }
}

void TestAndSetManager::onUpdatePortInfo(QBitArray inputDataBuf)
{
    mInputDataBuffer.push_back(inputDataBuf);
    mInputKeyList.push_back(CoreLibInputKey::fromInputDataBuffer(inputDataBuf));
    int dataCount = mInputDataBuffer.count();
    if(dataCount < 6) {
        return;
    }

    updateMembraneKey(&mInputKeyList);

    SelectSwInfo selectSwInfo[dataCount];

    for(int i = 0; i < dataCount; i++) {
        for (int j = 0; j < 32; j++){
            bool value = mInputDataBuffer[i].at(j);
            switch (j) {
            case 19:
                selectSwInfo[i].select01 = value;
                break;
            case 20:
                selectSwInfo[i].select02 = value;
                break;
            case 21:
                selectSwInfo[i].select03 = value;
                break;
            }
        }
    }

    bool executeInputTest = isExecuteInputTest();

    for(int i = dataCount - 1; i > 1; i--) {
        SelectSwInfo latest = selectSwInfo[i];
        SelectSwInfo old = selectSwInfo[i-1];
        SelectSwInfo oldOld = selectSwInfo[i-2];

        if((latest.select01 && old.select01 && oldOld.select01)
            && (!latest.select02 && !old.select02 && !oldOld.select02)
            && (!latest.select03 && !old.select03 && !oldOld.select03)) {
            setIsSelect02(false);
            if( !executeInputTest ) setIsSelect01(true);
            if( !executeInputTest ) setIsSelect03(true);
            break;
        }
        if((latest.select01 && old.select01 && oldOld.select01)
            && (!latest.select02 && !old.select02 && !oldOld.select02)
            && (latest.select03 && old.select03 && oldOld.select03)) {
            if( !executeInputTest ) setIsSelect02(true);
            setIsSelect03(false);
            if( !executeInputTest ) setIsSelect01(true);
            break;
        }
        if((!latest.select01 && !old.select01 && !oldOld.select01)
            && (!latest.select02 && !old.select02 && !oldOld.select02)
            && (latest.select03 && old.select03 && oldOld.select03)) {
            if( !executeInputTest ) setIsSelect03(true);
            if( !executeInputTest ) setIsSelect02(true);
            setIsSelect01(false);
            break;
        }
    }

    updateJogDialSwStatus(inputDataBuf);
    updateDeadmanSwStatus(inputDataBuf);

    mInputDataBuffer.clear();
    mInputKeyList.clear();
}

void TestAndSetManager::onUpdateJogInfo(QString jogCounterInfo)
{
    // qDebug() << "onUpdateJogInfo: jogCounterInfo :" << jogCounterInfo;   // for debug.
    setJogCounter(jogCounterInfo);
}

void TestAndSetManager::updateMembraneKey(QList<CoreLibInputKey> *pInputKeyList)
{
    if((pInputKeyList == nullptr) || (pInputKeyList->size() == 0)) {
        return;
    }

    QList<int> sel0KeyNumberList = getConfirmedInputKeyNumber(pInputKeyList, CoreLibInputKey::KeySelect::Select0);
    QList<int> sel1KeyNumberList = getConfirmedInputKeyNumber(pInputKeyList, CoreLibInputKey::KeySelect::Select1);

    QList<int> confirmedInputKeyNumberList;
    confirmedInputKeyNumberList.append(sel0KeyNumberList);
    confirmedInputKeyNumberList.append(sel1KeyNumberList);

    AllKeyInfo allKeyInfo{ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
    bool executeInputTest = isExecuteInputTest();
    for(int i = 0; i < confirmedInputKeyNumberList.count(); i++) {
        int keyNumber = confirmedInputKeyNumberList.at(i);
        switch(keyNumber) {
        case INPUT_KEY_01:
            allKeyInfo.key01 = false;
            break;
        case INPUT_KEY_02:
            allKeyInfo.key02 = false;
            break;
        case INPUT_KEY_03:
            allKeyInfo.key03 = false;
            break;
        case INPUT_KEY_04:
            allKeyInfo.key04 = false;
            break;
        case INPUT_KEY_05:
            allKeyInfo.key05 = false;
            break;
        case INPUT_KEY_06:
            allKeyInfo.key06 = false;
            break;
        case INPUT_KEY_07:
            allKeyInfo.key07 = false;
            break;
        case INPUT_KEY_08:
            allKeyInfo.key08 = false;
            break;
        case INPUT_KEY_09:
            allKeyInfo.key09 = false;
            break;
        case INPUT_KEY_10:
            allKeyInfo.key10 = false;
            break;
        case INPUT_KEY_11:
            allKeyInfo.key11 = false;
            break;
        case INPUT_KEY_12:
            allKeyInfo.key12 = false;
            break;
        case INPUT_KEY_13:
            allKeyInfo.key13 = false;
            break;
        case INPUT_KEY_14:
            allKeyInfo.key14 = false;
            break;
        case INPUT_KEY_15:
            allKeyInfo.key15 = false;
            break;
        case INPUT_KEY_16:
            allKeyInfo.key16 = false;
            break;
        case INPUT_KEY_17:
            allKeyInfo.key17 = false;
            break;
        case INPUT_KEY_18:
            allKeyInfo.key18 = false;
            break;
        }
    }

    if(( !executeInputTest ) || ( !allKeyInfo.key01 )) setIsPressedKey01(allKeyInfo.key01);
    if(( !executeInputTest ) || ( !allKeyInfo.key02 )) setIsPressedKey02(allKeyInfo.key02);
    if(( !executeInputTest ) || ( !allKeyInfo.key03 )) setIsPressedKey03(allKeyInfo.key03);
    if(( !executeInputTest ) || ( !allKeyInfo.key04 )) setIsPressedKey04(allKeyInfo.key04);
    if(( !executeInputTest ) || ( !allKeyInfo.key05 )) setIsPressedKey05(allKeyInfo.key05);
    if(( !executeInputTest ) || ( !allKeyInfo.key06 )) setIsPressedKey06(allKeyInfo.key06);
    if(( !executeInputTest ) || ( !allKeyInfo.key07 )) setIsPressedKey07(allKeyInfo.key07);
    if(( !executeInputTest ) || ( !allKeyInfo.key08 )) setIsPressedKey08(allKeyInfo.key08);
    if(( !executeInputTest ) || ( !allKeyInfo.key09 )) setIsPressedKey09(allKeyInfo.key09);
    if(( !executeInputTest ) || ( !allKeyInfo.key10 )) setIsPressedKey10(allKeyInfo.key10);
    if(( !executeInputTest ) || ( !allKeyInfo.key11 )) setIsPressedKey11(allKeyInfo.key11);
    if(( !executeInputTest ) || ( !allKeyInfo.key12 )) setIsPressedKey12(allKeyInfo.key12);
    if(( !executeInputTest ) || ( !allKeyInfo.key13 )) setIsPressedKey13(allKeyInfo.key13);
    if(( !executeInputTest ) || ( !allKeyInfo.key14 )) setIsPressedKey14(allKeyInfo.key14);
    if(( !executeInputTest ) || ( !allKeyInfo.key15 )) setIsPressedKey15(allKeyInfo.key15);
    if(( !executeInputTest ) || ( !allKeyInfo.key16 )) setIsPressedKey16(allKeyInfo.key16);
    if(( !executeInputTest ) || ( !allKeyInfo.key17 )) setIsPressedKey17(allKeyInfo.key17);
    if(( !executeInputTest ) || ( !allKeyInfo.key18 )) setIsPressedKey18(allKeyInfo.key18);
}

QList<int> TestAndSetManager::getConfirmedInputKeyNumber(QList<CoreLibInputKey> *pInputKeyList, CoreLibInputKey::KeySelect select)
{
    QList<int> confirmedKeyNumberList;

    if((pInputKeyList == nullptr) || (pInputKeyList->size() == 0)) {
        return confirmedKeyNumberList;
    }

    for(int i = 0; i < pInputKeyList->size(); i++) {
        CoreLibInputKey inputKey = pInputKeyList->at(i);
        if(inputKey.getKeySelect() == select) {
            if(confirmedKeyNumberList.isEmpty()) {
                confirmedKeyNumberList = inputKey.getSwitchOnKeyList();
            } else {
                QList<int> oldSwitchOnList = inputKey.getSwitchOnKeyList();
                for(int i = 0; i < confirmedKeyNumberList.count(); i++) {
                    int keyNumber = confirmedKeyNumberList.at(i);
                    if(!oldSwitchOnList.contains(keyNumber)) {
                        confirmedKeyNumberList.removeAt(i);
                        i--;
                    }
                }
            }
        }
    }

    return confirmedKeyNumberList;
}

void TestAndSetManager::onRetClicked(QString ipAddr, QString subnetMask)
{
    QString interfaceName = getEthernetInterfaceName();

    if ((interfaceName.length() != 0) && (!mMacAddress.isEmpty())) {
        // found Ethernet interface
        mCoreLibSystemControl.setMACAddress(mMacAddress, interfaceName);
    }

    if (!ipAddr.isEmpty() && !subnetMask.isEmpty()) {
        mCoreLibSystemControl.setNetworkSetting(ipAddr, subnetMask);
    }

    mCoreLibSystemControl.setBackLightBrightnessValue();
}

QString TestAndSetManager::getFromAppName()
{
    return mFromAppName;
}

void TestAndSetManager::setFromAppName(QString fromAppName)
{
    mFromAppName = fromAppName;
}

int TestAndSetManager::loadModbusErrorCountFile(QString appName)
{
    int errorCount = 0;
#ifdef REAL_TARGET
    const QString userHomePath = "/home/root";
#else // REAL_TARGET
    const QString userHomePath = QDir::homePath();
#endif // REAL_TARGET
    QString modbusErrorCountFilePath = userHomePath + "/" + appName + "/modbus/modbus_err_count";
    QFile modbusErrorCountFile(modbusErrorCountFilePath);
    if(!modbusErrorCountFile.exists()) {
        return errorCount;
    }
    if(!modbusErrorCountFile.open(QFile::ReadOnly)) {
        return errorCount;
    }
    bool convertSuccess = false;
    QTextStream stream(&modbusErrorCountFile);
    stream.seek(0);
    QString line = stream.readLine();
    errorCount = line.toInt(&convertSuccess, 10);
    if(!convertSuccess) {
        errorCount = 0;
    }
    modbusErrorCountFile.close();
    return errorCount;
}
