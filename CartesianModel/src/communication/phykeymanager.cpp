#include "phykeymanager.h"

#define ONLY_ONCE_PHYSICAL_MEMBLEM_KEY_NO_ISSUE
const qint64 PROC_JOG_DIAL_TIME_THRESHOLD = 25;

PhyKeyManager::PhyKeyManager(QObject *parent)
    : /*QThread(parent)*/QObject(parent),
      mJogControl(nullptr),
      mGpioControl(nullptr),
      mSelectSwStatus(SelectSwitchStatusInvalid),
      mPreSelectSwStatus(SelectSwitchStatusInvalid),
      mStartSelectSwStatus(SelectSwitchStatusInvalid),
      mIsTouchSelectSw(false),
      mIsPreOperableSw(false),
      mIsOperableSw(false),
      mJogOpeMode(JogOperationModeInValid),
      mPreJogCounter(0),
      mScrollPreJogCounter(0),
      mJogCounter(0),
      mPreJogDialSwStatus(false),
      mPreMemblemSelectSwStatus(MemblemSelectSwitchStatusInvalid),
      mMemblemSelectSwStatus(MemblemSelectSwitchStatusInvalid),
      mPreSelectSwInfo{false,false,false},
      mSelectSwInfo{false,false,false},
      mPreMemblemSwInfo{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
      mUpdateJogInfo{0, 0},
      mMutexJogCounterUpdate(nullptr),
      mHisOperableSw{false,false,false},
      mHisJogDialSw{false,false,false}
{
    initialize();
}

PhyKeyManager::~PhyKeyManager()
{
    delete mMutexJogCounterUpdate;
    mMutexJogCounterUpdate = nullptr;
    if (mJogControl){
        mJogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
        mJogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQepStsControl);
        delete mJogControl;
        mJogControl = nullptr;
    }
    if (mGpioControl){
#ifdef REAL_TARGET
        mGpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeWatchDogTimerInh);
        mGpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeWatchDogTimerClock);
#endif // REAL_TARGET
        mGpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
        delete mGpioControl;
        mGpioControl = nullptr;
    }
    delete m_elapsedTimer;
}

void PhyKeyManager::initialize()
{
    mMutexJogCounterUpdate = new QMutex(QMutex::Recursive);
    mJogControl = new CoreLibJogControl();
    mGpioControl = new CoreLibGpioControl();
    initJogInfo();
    initGpioInfo();
    initMemblemSwitchInfo();

    m_elapsedTimer = new QElapsedTimer();
    m_elapsedTimer->start();

    mGpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
}

void PhyKeyManager::initJogInfo()
{
    stJogInfo jogInfo;
#ifdef REAL_TARGET
    bool ret = false;
    int result = 0;
    if (!mJogControl){
        return;
    }
    ret = mJogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQepStsControl);
    if (ret == false){
        qDebug("Jog QepStsControl openDevice failed ret[%d]\n", ret);
        return;
    }
    // jog dev open
    ret = mJogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    if (ret == false){
        qDebug("Jog openDevice failed ret[%d]\n", ret);
        return;
    }
    result = mJogControl->getJogInfo(&jogInfo);
    if (result < 0){
        qDebug() << "getJogInfo failed result: " << result;
        return;
    }
    ret = analyzeJogInfo(jogInfo);
    if (!ret){
        qDebug() << "analyzeJogInfo failed ret: " << ret;
        return;
    }
    mPreJogCounter = mJogCounter;
    mScrollPreJogCounter = mJogCounter;
    mPreJogDirection = mJogDirection;
#else // REAL_TARGET
    (void)jogInfo;
#endif // REAL_TARGET
}

void PhyKeyManager::initGpioInfo()
{
#ifdef REAL_TARGET
    bool openResult = false;
    if (!mGpioControl) {
        return;
    }
    openResult = mGpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    if (openResult == false){
        qDebug("gpio input buf openDevice failed ret[%d]\n", openResult);
        return;
    }
#endif // REAL_TARGET
}

bool PhyKeyManager::initLedDevice()
{
#ifdef REAL_TARGET
    bool openResult = false;
    if (!mGpioControl) {
        return false;
    }
    openResult = mGpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    if (openResult == false){
        qDebug("gpio blue led openDevice failed ret[%d]\n", openResult);
        return openResult;
    }
    openResult = mGpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    if (openResult == false){
        qDebug("gpio red led openDevice failed ret[%d]\n", openResult);
        return openResult;
    }
    openResult = mGpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
    if (openResult == false){
        qDebug("gpio green openDevice failed ret[%d]\n", openResult);
        return openResult;
    }
    return openResult;
#else
    return true;
#endif // REAL_TARGET
}

void PhyKeyManager::deinitLedDevice()
{
#ifdef REAL_TARGET
    if (!mGpioControl) {
        return;
    }
    mGpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    mGpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    mGpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
#endif // REAL_TARGET
}

void PhyKeyManager::monitorByComThreadTriger()
{
    int result = 0;
    QString jogCounter = "";
    QString jogDirection = "";
    stJogInfo jogInfo = {"0", "0"};
    bool ret;
    int i = 0;

    if (!mJogControl || !mGpioControl){
        return;
    }
    QBitArray bitArray[mHoldInputDataBufCount];
    for (i = 0; i < mHoldInputDataBufCount; i++){
        bitArray[i].resize(32);
    }
    result = mGpioControl->getInputDataBuffer(bitArray, mHoldInputDataBufCount);
    if (result < 0) {
        qDebug() << "getInputDataBuffer result: " << result;
        goto jog_monitor;
    }
    // init switch info
    initCooperationMemblemSw();//初始化轴操作按键(3~12)
    analyzeSwitchInfo(bitArray);//解析按键信息

    notifySwitchInfo();// switch info emit

jog_monitor:
    result = mJogControl->getJogInfo(&jogInfo);
    if (result < 0){
        //qDebug() << "result: " << result;
        qDebug("%s:%d result[%d]", __func__, __LINE__, result);
        return;
    }
    ret = analyzeJogInfo(jogInfo);
    if (!ret){
        qDebug() << "ret: " << ret;
    }
    notifyJogInfo();
}

void PhyKeyManager::monitorByTimerTriger()
{
    int result = 0;
    QString jogCounter = "";
    QString jogDirection = "";
    stJogInfo jogInfo = {"0", "0"};
    bool ret;
    int i = 0;

    if (!mJogControl || !mGpioControl){
        return;
    }
    QBitArray bitArray[mHoldInputDataBufCount];
    for (i = 0; i < mHoldInputDataBufCount; i++){
        bitArray[i].resize(32);
    }
    result = mGpioControl->getInputDataBuffer(bitArray, mHoldInputDataBufCount);
    if (result < 0) {
        qDebug() << "getInputDataBuffer result: " << result;
        goto jog_monitor;
    }
    // init switch info
    initCooperationMemblemSw();
    analyzeSwitchInfo(bitArray);
    // switch info emit
    notifySwitchInfo();
    if (mHisCount >= PHY_KEY_CHATTERING_PREVENTION_NUM){
        mHisCount = 0;
    }

jog_monitor:
    if (m_elapsedTimer->elapsed() >= PROC_JOG_DIAL_TIME_THRESHOLD) {
        result = mJogControl->getJogInfo(&jogInfo);
        if (result < 0){
            //qDebug() << "result: " << result;
            qDebug("%s:%d result[%d]", __func__, __LINE__, result);
            return;
        }
        ret = analyzeJogInfo(jogInfo);
        if (!ret){
            qDebug() << "ret: " << ret;
        }

        notifyJogInfo();
        m_elapsedTimer->restart();
    }
}

void PhyKeyManager::initMemblemSwitchInfo()
{
    mMemblemSwInfo.key01 = false;
    mMemblemSwInfo.key02 = false;
    mMemblemSwInfo.key03 = false;
    mMemblemSwInfo.key04 = false;
    mMemblemSwInfo.key05 = false;
    mMemblemSwInfo.key06 = false;
    mMemblemSwInfo.key07 = false;
    mMemblemSwInfo.key08 = false;
    mMemblemSwInfo.key09 = false;
    mMemblemSwInfo.key10 = false;
    mMemblemSwInfo.key11 = false;
    mMemblemSwInfo.key12 = false;
    mMemblemSwInfo.key13 = false;
    mMemblemSwInfo.key14 = false;
    mMemblemSwInfo.key15 = false;
    mMemblemSwInfo.key16 = false;
    mMemblemSwInfo.key17 = false;
    mMemblemSwInfo.key18 = false;
    initHisMemblemSwitchInfo();
}

void PhyKeyManager::initCooperationMemblemSw()
{
    mMemblemSwInfo.key03 = false;
    mMemblemSwInfo.key04 = false;
    mMemblemSwInfo.key05 = false;
    mMemblemSwInfo.key06 = false;
    mMemblemSwInfo.key07 = false;
    mMemblemSwInfo.key08 = false;
    mMemblemSwInfo.key09 = false;
    mMemblemSwInfo.key10 = false;
    mMemblemSwInfo.key11 = false;
    mMemblemSwInfo.key12 = false;
}

void PhyKeyManager::initHisMemblemSwitchInfo()
{
    for(int i = 0; i < PHY_KEY_CHATTERING_PREVENTION_NUM; i++){
        mHisMemblemSwInfo[i].key01 = false;
        mHisMemblemSwInfo[i].key02 = false;
        mHisMemblemSwInfo[i].key03 = false;
        mHisMemblemSwInfo[i].key04 = false;
        mHisMemblemSwInfo[i].key05 = false;
        mHisMemblemSwInfo[i].key06 = false;
        mHisMemblemSwInfo[i].key07 = false;
        mHisMemblemSwInfo[i].key08 = false;
        mHisMemblemSwInfo[i].key09 = false;
        mHisMemblemSwInfo[i].key10 = false;
        mHisMemblemSwInfo[i].key11 = false;
        mHisMemblemSwInfo[i].key12 = false;
        mHisMemblemSwInfo[i].key13 = false;
        mHisMemblemSwInfo[i].key14 = false;
        mHisMemblemSwInfo[i].key15 = false;
        mHisMemblemSwInfo[i].key16 = false;
        mHisMemblemSwInfo[i].key17 = false;
        mHisMemblemSwInfo[i].key18 = false;
    }
}

void PhyKeyManager::initHisEachMemblemSwitchInfo(int keyNo)
{
    //qDebug("%s:%d keyNo[%d]", __func__, __LINE__, keyNo);
    for(int i = 0; i < PHY_KEY_CHATTERING_PREVENTION_NUM; i++){
        switch((MemblemKeyNo)keyNo){
        case MemblemKeyNo::MemblemKey01:
            mHisMemblemSwInfo[i].key01 = false;
            break;
        case MemblemKeyNo::MemblemKey02:
            mHisMemblemSwInfo[i].key02 = false;
            break;
        case MemblemKeyNo::MemblemKey03:
            mHisMemblemSwInfo[i].key03 = false;
            break;
        case MemblemKeyNo::MemblemKey04:
            mHisMemblemSwInfo[i].key04 = false;
            break;
        case MemblemKeyNo::MemblemKey05:
            mHisMemblemSwInfo[i].key05 = false;
            break;
        case MemblemKeyNo::MemblemKey06:
            mHisMemblemSwInfo[i].key06 = false;
            break;
        case MemblemKeyNo::MemblemKey07:
            mHisMemblemSwInfo[i].key07 = false;
            break;
        case MemblemKeyNo::MemblemKey08:
            mHisMemblemSwInfo[i].key08 = false;
            break;
        case MemblemKeyNo::MemblemKey09:
            mHisMemblemSwInfo[i].key09 = false;
            break;
        case MemblemKeyNo::MemblemKey10:
            mHisMemblemSwInfo[i].key10 = false;
            break;
        case MemblemKeyNo::MemblemKey11:
            mHisMemblemSwInfo[i].key11 = false;
            break;
        case MemblemKeyNo::MemblemKey12:
            mHisMemblemSwInfo[i].key12 = false;
            break;
        case MemblemKeyNo::MemblemKey13:
            mHisMemblemSwInfo[i].key13 = false;
            break;
        case MemblemKeyNo::MemblemKey14:
            mHisMemblemSwInfo[i].key14 = false;
            break;
        case MemblemKeyNo::MemblemKey15:
            mHisMemblemSwInfo[i].key15 = false;
            break;
        case MemblemKeyNo::MemblemKey16:
            mHisMemblemSwInfo[i].key16 = false;
            break;
        case MemblemKeyNo::MemblemKey17:
            mHisMemblemSwInfo[i].key17 = false;
            break;
        case MemblemKeyNo::MemblemKey18:
            mHisMemblemSwInfo[i].key18 = false;
            break;
        default:
            break;
        }
    }
}

void PhyKeyManager::initHisOperableSwitchInfo()
{
    for(int i = 0; i < PHY_KEY_CHATTERING_PREVENTION_NUM; i++){
        mHisOperableSw[i] = false;
    }
}

void PhyKeyManager::initHisJogDialSwitchInfo()
{
    for(int i = 0; i < PHY_KEY_CHATTERING_PREVENTION_NUM; i++){
        mHisJogDialSw[i] = false;
    }
}

void PhyKeyManager::initSelectSwitchInfo()
{
#ifdef REAL_TARGET
    int result = 0;
    int i = 0, j = 0;
    QBitArray bitArray[mHoldInputDataBufCount];

    if (!mGpioControl){
        qDebug("%s:%d mGpioControl is Null", __func__, __LINE__);
        return;
    }
    for (i = 0; i < mHoldInputDataBufCount; i++){
        bitArray[i].resize(32);
    }
    result = mGpioControl->getInputDataBuffer(bitArray, mHoldInputDataBufCount);
    if (result < 0) {
        qDebug() << "getInputDataBuffer result: " << result;
        return;
    }
    static uchar bitIncrement = 0;
    for(i = 0; i < mHoldInputDataBufCount; i++) {
        for (j = 31; j >= 0; j--){
            if (j < mSelectSwAuto || j > mSelectSwManual){
                continue;
            }
            bool value = bitArray[i].at(j);
            if (j == mSelectSwAuto){
                mSelectSwInfo.select01 = value;
                bitIncrement++;
            } else if (j == mSelectSwOrigin){
                mSelectSwInfo.select02 = value;
                bitIncrement++;
            } else if (j == mSelectSwManual) {
                mSelectSwInfo.select03 = value;
                bitIncrement++;
            }
            if (bitIncrement >= 3){
                if (!mSelectSwInfo.select01 && !mSelectSwInfo.select02 && mSelectSwInfo.select03){
                    mPreSelectSwStatus = SelectSwitchStatusAuto;
                } else if (mSelectSwInfo.select01 && !mSelectSwInfo.select02 && !mSelectSwInfo.select03){
                    mPreSelectSwStatus = SelectSwitchStatusOrigin;
                } else if (mSelectSwInfo.select01 && !mSelectSwInfo.select02 && mSelectSwInfo.select03){
                    mPreSelectSwStatus = SelectSwitchStatusManual;
                } else if (!mSelectSwInfo.select01 && !mSelectSwInfo.select02 && !mSelectSwInfo.select03){
                    mPreSelectSwStatus = SelectSwitchStatusOff;
                }
                bitIncrement = 0;
                goto end;
            }
        }
    }
end:
    qInfo("%s:%d mPreSelectSwStatus[%d]", __func__, __LINE__, mPreSelectSwStatus);
    mStartSelectSwStatus = mPreSelectSwStatus;
    setSelectSwMode(mPreSelectSwStatus);
#endif // REAL_TARGET
}

void PhyKeyManager::setCurrentBuzzerState(bool isonoff)
{
    m_currentBuzzerState = isonoff;
    emit currentBuzzerStateChanged();
}

void PhyKeyManager::setCurrentBuzzerIndex(quint8 type)
{
    switch (type) {
    case 0:
        m_currentBuzzerIndex = 0;
        m_currentBuzzerPitch = 40;
        emit currentBuzzerIndexChanged(m_currentBuzzerIndex);
        emit currentBuzzerPitchChanged();
        break;
    case 1:
        m_currentBuzzerIndex = 1;
        m_currentBuzzerPitch = 50;
        emit currentBuzzerIndexChanged(m_currentBuzzerIndex);
        emit currentBuzzerPitchChanged();
        break;
    case 2:
        m_currentBuzzerIndex = 2;
        m_currentBuzzerPitch = 62;
        emit currentBuzzerIndexChanged(m_currentBuzzerIndex);
        emit currentBuzzerPitchChanged();
        break;
    case 3:
        m_currentBuzzerIndex = 3;
        m_currentBuzzerPitch = 69;
        emit currentBuzzerIndexChanged(m_currentBuzzerIndex);
        emit currentBuzzerPitchChanged();
        break;
    case 4:
        m_currentBuzzerIndex = 4;
        m_currentBuzzerPitch = 75;
        emit currentBuzzerIndexChanged(m_currentBuzzerIndex);
        emit currentBuzzerPitchChanged();
        break;
    default:
        break;
    }
}

bool PhyKeyManager::analyzeJogInfo(stJogInfo &jogInfo)
{
    bool ret = true;

    // jogcounter change to num
    mJogCounter = (jogInfo.jogCounter).toInt(&ret, 10);
    if (!ret){
        qDebug() << "failed is_ok:" << ret << "jogCounter:" << jogInfo.jogCounter;
        return ret;
    }
    mJogDirection = (uchar)(jogInfo.jogDirection).toInt(&ret, 10);
    if (!ret){
        qDebug() << "failed is_ok:" << ret << "jogDirection:" << jogInfo.jogDirection;
        return ret;
    }
    return ret;
}

void PhyKeyManager::initResourceManager(ResourceManager* resourceManager)
{
    m_resource = resourceManager;
    m_calibrateLongPress = true;
}

void PhyKeyManager::initJogSetting(ResourceManager* resourceManager)
{
    setJogCounterThreshold(resourceManager);
    setJogMagnification(resourceManager);
    setJogPulseThreshold(resourceManager);
}

void PhyKeyManager::setJogCounterThreshold(ResourceManager* resourceManager)
{
    bool is_ok;
    int jogCounterThresh = 0;

    if (!resourceManager){
        qCritical("%s:%d resourceManager is invalid", __func__, __LINE__);
        return;
    }
    QString jogThreshold = resourceManager->getPenSetStringValue("1", 16, 0);
    qDebug() << "jogThreshold:" << jogThreshold;
    jogCounterThresh = jogThreshold.toInt(&is_ok, 10);
    if (!is_ok){
        qDebug() << "failed is_ok:" << is_ok << "jogThreshold:" << jogThreshold;
        return;
    }
    mJogCounterThreshold = jogCounterThresh;
}

void PhyKeyManager::setJogMagnification(ResourceManager* resourceManager)
{
    if (!resourceManager){
        qCritical("%s:%d resourceManager is invalid", __func__, __LINE__);
        return;
    }
    const int PEN_SET_JOG_MAGNIFICATION_TOP_ROW = 194;
    for (int i = 0; i < 3; i++) {
        stJogMagnificationInfo info;
        info.magnification = resourceManager->getPenSetIntValue("6", PEN_SET_JOG_MAGNIFICATION_TOP_ROW + i, 0);
        info.division = resourceManager->getPenSetIntValue("6", PEN_SET_JOG_MAGNIFICATION_TOP_ROW + i, 1);
        info.multiplication = resourceManager->getPenSetIntValue("6", PEN_SET_JOG_MAGNIFICATION_TOP_ROW + i, 2);
        m_jogMagnificationInfo.append(info);
    }
}

void PhyKeyManager::setJogPulseThreshold(ResourceManager *resourceManager)
{
    if (!resourceManager){
        qCritical("%s:%d resourceManager is invalid", __func__, __LINE__);
        return;
    }
    int threshold;
    const int PEN_SET_JOG_PULSE_THRESHOLD_TOP_ROW = 228;
    for (int i = 0; i < 3; i++) {
        threshold = resourceManager->getPenSetIntValue("6", PEN_SET_JOG_PULSE_THRESHOLD_TOP_ROW + i, 0);
        m_jogPulseThreshold.append(threshold);
    }
}

int PhyKeyManager::getJogCounterThreshold()
{
    qDebug("%s:%d mJogCounterThreshold[%d]", __func__, __LINE__, mJogCounterThreshold);
    return mJogCounterThreshold;
}

void PhyKeyManager::writeJogMagnification(int jogMagnificationIndex)
{
    int magnification = 0;
    //qDebug("%s:%d jogMagnification[%d]", __func__, __LINE__, jogMagnificationIndex);
    switch (jogMagnificationIndex){
    case Enums::JogMagnification1:
    case Enums::JogMagnification10:
    case Enums::JogMagnification100:
    {
        auto jogMagnificationInfo = m_jogMagnificationInfo.at(jogMagnificationIndex);
        magnification = jogMagnificationInfo.multiplication;
        m_jogCounterDivision = jogMagnificationInfo.division;
        setJogMagnificationSetting(static_cast<Enums::JogMagnification>(jogMagnificationIndex));
        setJogDialSpeed(Enums::JogDialSpeedUnknown);
        // reset jogCounter info
        resetJogCounterInfo();
    }
        break;
    case Enums::JogMagnification1000:
    {
        magnification = 1;
        m_jogCounterDivision = 1;
        setJogMagnificationSetting(static_cast<Enums::JogMagnification>(jogMagnificationIndex));
        // reset jogCounter info
        resetJogCounterInfo();
    }
        break;
    default:
        break;
    }
    setJogMagnification(magnification);
}

void PhyKeyManager::writeJogDefaultValue()
{
    if (!mJogControl){
        qCritical("%s:%d mJogControl is invalid", __func__, __LINE__);
        return;
    }
    stJogInfo jogInfo;
    bool is_ok = false;
    int ret = 0;
    int jogCounter = 0;

    ret = mJogControl->getJogInfo(&jogInfo);
    if (ret != 0){
        qCritical("%s:%d getJogInfo failed[%d]", __func__, __LINE__, ret);
        return;
    }
    qDebug() << "writeJogDefaultValue jogCounter:" << jogInfo.jogCounter;
    mJogCounterInfo.jogCounter = (jogInfo.jogCounter).toInt(&is_ok, 10);
    if (!is_ok){
        qCritical() << "failed is_ok:" << ret << "jogCounter:" << jogInfo.jogCounter;
        return;
    }
    mJogCounterInfo.jogDirection = (jogInfo.jogDirection).toInt(&is_ok, 10);
    if (!is_ok){
        qCritical() << "failed is_ok:" << ret << "jogCounter:" << jogInfo.jogDirection;
        return;
    }
    jogCounter = mJogCounterInfo.jogCounter * (int)mJogDefaultMagnification;
    qInfo("%s:%d jogCounter[%d]", __func__, __LINE__, jogCounter);
    emit updateJogCounterInfo(jogCounter);
}

void PhyKeyManager::setJogMagnification(int jogMagnification)
{
    m_jogMagnification = jogMagnification;
}

void PhyKeyManager::setJogMagnificationSetting(Enums::JogMagnification jogMagnification)
{
    if (m_jogMagnificationSetting == jogMagnification)
        return;

    m_jogMagnificationSetting = jogMagnification;
    emit jogMagnificationSettingChanged();
}

void PhyKeyManager::updateJogDialSpeed()
{
    int speed = Enums::JogDialSpeedUnknown;
    int diff = abs(mJogCounter - mScrollPreJogCounter);
    for (int threshold : m_jogPulseThreshold) {
        if (diff < threshold)
            break;
        speed++;
    }
    setJogDialSpeed(static_cast<Enums::JogDialSpeed>(speed));
}

void PhyKeyManager::setJogDialSpeed(Enums::JogDialSpeed speed)
{
    if (m_jogDialSpeed != speed) {
        m_jogDialSpeed = speed;
        jogDialSpeedChanged(m_jogDialSpeed);
    }
}

void PhyKeyManager::judgeJogScrollNotify()
{
    mCurJogCounter = mJogCounter;
    if (mPreJogDirection != mJogDirection){
        mPreJogDirection = mJogDirection;
        if (mIsPassOriginFlag) {mIsPassOriginFlag = false;}
    }
    if (mJogDirection == CoreLibJogControl::JogRotationDirection::JogRotationDirectionCW){
        if (mIsPassOriginFlag){
            mCurJogCounter += 0x10000;
            if (abs(mCurJogCounter - mScrollPreJogCounter) >= mJogCounterThreshold){
                mScrollPreJogCounter = mJogCounter;
                //qDebug() << "jogScrollDown:::::::true";
                onUpdateJogScrollDown(true);
            }
        } else {
            if (abs(mCurJogCounter - mScrollPreJogCounter) >= mJogCounterThreshold){
                mScrollPreJogCounter = mJogCounter;
                //qDebug() << "jogScrollDown:::::::false";
                onUpdateJogScrollDown(true);
            }
        }
    } else {
        if (mIsPassOriginFlag){
            mCurJogCounter -= 0x10000;
            if (abs(mScrollPreJogCounter - mCurJogCounter) >= mJogCounterThreshold){
                mScrollPreJogCounter = mJogCounter;
                qDebug() << "jogScrollup:::::::true";
                //onUpdateJogScrollUp(true);
            }
        } else {
            if (abs(mCurJogCounter - mScrollPreJogCounter) >= mJogCounterThreshold){
                mScrollPreJogCounter = mJogCounter;
                //qDebug() << "onUpdateJogScrollUp:::::::false";
                onUpdateJogScrollUp(true);
            }
        }
    }
}

void PhyKeyManager::analyzeSwitchInfo(QBitArray *bitArray)
{
    int i = 0, j = 0;
    if (!bitArray){
        return;
    }
    for(i = 0; i < mHoldInputDataBufCount; i++) {
        for (j = 31; j >= 0; j--){
            bool value = bitArray[i].at(j);
            //qDebug("InputDataBuf[%d]: %d", j, value);
            analyzeCooperationMemblemSw(j, value);          // check coopertaion memblemSw
            analyzeModBusCooperationMemblemSw(j, value);    // check modbus coopertaion memblemSw
            analyzeDeadManSw(j, value, i);                  // check deadManSw
            analyzeSelectSw(j, value);                      // check seletcSw
            analyzeJogDialSw(j, value, i);                  // check jogdialSw
        }
    }
    mHisCount++;
}

void PhyKeyManager::analyzeDeadManSw(int bitIndex, bool value, int bufIndexCount)
{
    if (mHisCount >= PHY_KEY_CHATTERING_PREVENTION_NUM){
        qDebug("%s:%d mHisCount:%d", __func__, __LINE__, mHisCount);
        return;
    }
    if (bitIndex != mDeadManSwBitNo){
        return;
    }
    if(bufIndexCount % 2){
        if (!value) { mHisOperableSw[mHisCount] = true; }
        else { mHisOperableSw[mHisCount] = false; }
        if (mHisOperableSw[0] && mHisOperableSw[1] && mHisOperableSw[2]){
            mIsOperableSw = true;
        } else {
            if (!(!mHisOperableSw[0] && !mHisOperableSw[1] && !mHisOperableSw[2])){
                //qDebug("%s:%d OperableSw %d %d %d", __func__, __LINE__, mHisOperableSw[0], mHisOperableSw[1], mHisOperableSw[2]);
            }
            if ((!mHisOperableSw[0] && !mHisOperableSw[1] && !mHisOperableSw[2])){
                mIsOperableSw = false;
            }
        }
        //mJogOpeMode = ((mIsOperableSw == true) ? JogOperationModeValid : JogOperationModeInValid);
    }
}

void PhyKeyManager::analyzeSelectSw(int bitIndex, bool value)
{
    static uchar bitIncrement = 0;
    if (bitIndex < mSelectSwAuto || bitIndex > mSelectSwManual){
        return;
    }
    if (bitIndex == mSelectSwAuto){
        mSelectSwInfo.select01 = value;
        bitIncrement++;
    } else if (bitIndex == mSelectSwOrigin){
        mSelectSwInfo.select02 = value;
        bitIncrement++;
    } else if (bitIndex == mSelectSwManual) {
        mSelectSwInfo.select03 = value;
        bitIncrement++;
    }
    if (bitIncrement >= 3){
        decideSelectSw(mSelectSwInfo);
        bitIncrement = 0;
    }
}

void PhyKeyManager::analyzeJogDialSw(int bitIndex, bool value, int bufIndexCount)
{
    if (mHisCount >= PHY_KEY_CHATTERING_PREVENTION_NUM){
        qDebug("%s:%d mHisCount:%d", __func__, __LINE__, mHisCount);
        return;
    }
    if (bitIndex != mJogDialSwBitNo){
        return;
    }
    if(bufIndexCount % 2){
        if (!value) { mHisJogDialSw[mHisCount] = true; }
        else { mHisJogDialSw[mHisCount] = false; }
        if (mHisJogDialSw[0] && mHisJogDialSw[1] && mHisJogDialSw[2]){
            mJogDialSwStatus = true;
        } else {
            if (!(!mHisJogDialSw[0] && !mHisJogDialSw[1] && !mHisJogDialSw[2])){
                //qDebug("%s:%d JogDialSw %d %d %d", __func__, __LINE__, mHisJogDialSw[0], mHisJogDialSw[1], mHisJogDialSw[2]);
            }
            if ((!mHisJogDialSw[0] && !mHisJogDialSw[1] && !mHisJogDialSw[2])){
                mJogDialSwStatus = false;
            }
        }
        if (mJogDialSwStatus){
            mJogDialSwMonitorCounter++;
        }
    }
    mJogOpeMode = ((mJogDialSwStatus == true) ? JogOperationModeValid : JogOperationModeInValid);
}

void PhyKeyManager::analyzeCooperationMemblemSw(int bitIndex, bool value)
{
    if (!(bitIndex >= mMemblemKIN1SwBitNo && bitIndex <= mMemblemKIN5SwBitNo) &&
        !(bitIndex >= mMemblemSEL1SwBitNo && bitIndex <= mMemblemSEL0SwBitNo)){
        return;
    }
    if (mHisCount >= PHY_KEY_CHATTERING_PREVENTION_NUM){
        qDebug("%s:%d mHisCount:%d", __func__, __LINE__, mHisCount);
        return;
    }
    if (bitIndex == mMemblemSEL1SwBitNo){    // 偶数
        if (!value){
            mMemblemSelectSwStatus = MemblemSelectSwitchStatusEven;
        }
    } else if (bitIndex == mMemblemSEL0SwBitNo){ // 奇数
        if (!value){
            mMemblemSelectSwStatus = MemblemSelectSwitchStatusOdd;
        }
    }
    if (mMemblemSelectSwStatus == MemblemSelectSwitchStatusEven){
        if (bitIndex == mMemblemKIN1SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key04 = true; }
            else { mHisMemblemSwInfo[mHisCount].key04 = false; }
        } else if (bitIndex == mMemblemKIN2SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key06 = true; }
            else { mHisMemblemSwInfo[mHisCount].key06 = false; }
        } else if (bitIndex == mMemblemKIN3SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key08 = true; }
            else { mHisMemblemSwInfo[mHisCount].key08 = false; }
        } else if (bitIndex == mMemblemKIN4SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key10 = true; }
            else { mHisMemblemSwInfo[mHisCount].key10 = false; }
        } else if (bitIndex == mMemblemKIN5SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key12 = true; }
            else { mHisMemblemSwInfo[mHisCount].key12 = false; }
        }
        // memblem key history status check(History 3 times all true and ON)
        if (bitIndex == mMemblemKIN1SwBitNo){
            if (mHisMemblemSwInfo[0].key04 && mHisMemblemSwInfo[1].key04 && mHisMemblemSwInfo[2].key04){
                mMemblemSwInfo.key04 = true;                // KEY04-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key04 && !mHisMemblemSwInfo[1].key04 && !mHisMemblemSwInfo[2].key04)){
                    //qDebug("%s:%d key04 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key04, mHisMemblemSwInfo[1].key04, mHisMemblemSwInfo[2].key04);
                }
            }
        } else if (bitIndex == mMemblemKIN2SwBitNo){
            if (mHisMemblemSwInfo[0].key06 && mHisMemblemSwInfo[1].key06 && mHisMemblemSwInfo[2].key06){
                mMemblemSwInfo.key06 = true;                // KEY06-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key06 && !mHisMemblemSwInfo[1].key06 && !mHisMemblemSwInfo[2].key06)){
                    //qDebug("%s:%d key06 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key06, mHisMemblemSwInfo[1].key06, mHisMemblemSwInfo[2].key06);
                }
            }
        } else if (bitIndex == mMemblemKIN3SwBitNo){
            if (mHisMemblemSwInfo[0].key08 && mHisMemblemSwInfo[1].key08 && mHisMemblemSwInfo[2].key08){
                mMemblemSwInfo.key08 = true;                // KEY08-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key08 && !mHisMemblemSwInfo[1].key08 && !mHisMemblemSwInfo[2].key08)){
                    //qDebug("%s:%d key08 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key08, mHisMemblemSwInfo[1].key08, mHisMemblemSwInfo[2].key08);
                }
            }
        } else if (bitIndex == mMemblemKIN4SwBitNo){
            if (mHisMemblemSwInfo[0].key10 && mHisMemblemSwInfo[1].key10 && mHisMemblemSwInfo[2].key10){
                mMemblemSwInfo.key10 = true;                // KEY08-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key10 && !mHisMemblemSwInfo[1].key10 && !mHisMemblemSwInfo[2].key10)){
                    //qDebug("%s:%d key10 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key10, mHisMemblemSwInfo[1].key10, mHisMemblemSwInfo[2].key10);
                }
            }
        } else if (bitIndex == mMemblemKIN5SwBitNo){
            if (mHisMemblemSwInfo[0].key12 && mHisMemblemSwInfo[1].key12 && mHisMemblemSwInfo[2].key12){
                mMemblemSwInfo.key12 = true;                // KEY12-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key12 && !mHisMemblemSwInfo[1].key12 && !mHisMemblemSwInfo[2].key12)){
                    //qDebug("%s:%d key12 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key12, mHisMemblemSwInfo[1].key12, mHisMemblemSwInfo[2].key12);
                }
            }
        }
    } else if (mMemblemSelectSwStatus == MemblemSelectSwitchStatusOdd){
        if (bitIndex == mMemblemKIN1SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key03 = true; }
            else { mHisMemblemSwInfo[mHisCount].key03 = false; }
        } else if (bitIndex == mMemblemKIN2SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key05 = true; }
            else { mHisMemblemSwInfo[mHisCount].key05 = false; }
        } else if (bitIndex == mMemblemKIN3SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key07 = true; }
            else { mHisMemblemSwInfo[mHisCount].key07 = false; }
        } else if (bitIndex == mMemblemKIN4SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key09 = true; }
            else { mHisMemblemSwInfo[mHisCount].key09 = false; }
        } else if (bitIndex == mMemblemKIN5SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key11 = true; }
            else { mHisMemblemSwInfo[mHisCount].key11 = false; }
        }
        // memblem key history status check(History 3 times all true and ON)
        if (bitIndex == mMemblemKIN1SwBitNo){
            if (mHisMemblemSwInfo[0].key03 && mHisMemblemSwInfo[1].key03 && mHisMemblemSwInfo[2].key03){
                mMemblemSwInfo.key03 = true;                // KEY03-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key03 && !mHisMemblemSwInfo[1].key03 && !mHisMemblemSwInfo[2].key03)){
                    //qDebug("%s:%d key03 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key03, mHisMemblemSwInfo[1].key03, mHisMemblemSwInfo[2].key03);
                }
            }
        } else if (bitIndex == mMemblemKIN2SwBitNo){
            if (mHisMemblemSwInfo[0].key05 && mHisMemblemSwInfo[1].key05 && mHisMemblemSwInfo[2].key05){
                mMemblemSwInfo.key05 = true;                // KEY05-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key05 && !mHisMemblemSwInfo[1].key05 && !mHisMemblemSwInfo[2].key05)){
                    //qDebug("%s:%d key05 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key05, mHisMemblemSwInfo[1].key05, mHisMemblemSwInfo[2].key05);
                }
            }
        } else if (bitIndex == mMemblemKIN3SwBitNo){
            if (mHisMemblemSwInfo[0].key07 && mHisMemblemSwInfo[1].key07 && mHisMemblemSwInfo[2].key07){
                mMemblemSwInfo.key07 = true;                // KEY07-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key07 && !mHisMemblemSwInfo[1].key07 && !mHisMemblemSwInfo[2].key07)){
                    //qDebug("%s:%d key07 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key07, mHisMemblemSwInfo[1].key07, mHisMemblemSwInfo[2].key07);
                }
            }
        } else if (bitIndex == mMemblemKIN4SwBitNo){
            if (mHisMemblemSwInfo[0].key09 && mHisMemblemSwInfo[1].key09 && mHisMemblemSwInfo[2].key09){
                mMemblemSwInfo.key09 = true;                // KEY09-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key09 && !mHisMemblemSwInfo[1].key09 && !mHisMemblemSwInfo[2].key09)){
                    //qDebug("%s:%d key09 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key09, mHisMemblemSwInfo[1].key09, mHisMemblemSwInfo[2].key09);
                }
            }
        } else if (bitIndex == mMemblemKIN5SwBitNo){
            if (mHisMemblemSwInfo[0].key11 && mHisMemblemSwInfo[1].key11 && mHisMemblemSwInfo[2].key11){
                mMemblemSwInfo.key11 = true;                // KEY11-ON
            } else {
                if (!(!mHisMemblemSwInfo[0].key11 && !mHisMemblemSwInfo[1].key11 && !mHisMemblemSwInfo[2].key11)){
                    //qDebug("%s:%d key11 %d %d %d", __func__, __LINE__, mHisMemblemSwInfo[0].key11, mHisMemblemSwInfo[1].key11, mHisMemblemSwInfo[2].key11);
                }
            }
        }
    }
}

void PhyKeyManager::analyzeModBusCooperationMemblemSw(int bitIndex, bool value)
{
    if (mHisCount >= PHY_KEY_CHATTERING_PREVENTION_NUM){
        qDebug("%s:%d mHisCount:%d", __func__, __LINE__, mHisCount);
        return;
    }
    if (bitIndex == mMemblemSEL1SwBitNo){    // 偶数
        if (!value){
            mMemblemSelectSwStatus = MemblemSelectSwitchStatusEven;
        }
    } else if (bitIndex == mMemblemSEL0SwBitNo){ // 奇数
        if (!value){
            mMemblemSelectSwStatus = MemblemSelectSwitchStatusOdd;
        }
    }
    if (mMemblemSelectSwStatus == MemblemSelectSwitchStatusEven){
        if (bitIndex == mMemblemKIN0SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key02 = true; }
            else { mHisMemblemSwInfo[mHisCount].key02 = false; }
        }
        if (bitIndex == mMemblemKIN1SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key04 = true; }
            else { mHisMemblemSwInfo[mHisCount].key04 = false; }
        }
        if (bitIndex == mMemblemKIN2SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key06 = true; }
            else { mHisMemblemSwInfo[mHisCount].key06 = false; }
        }
        if (bitIndex == mMemblemKIN3SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key08 = true; }
            else { mHisMemblemSwInfo[mHisCount].key08 = false; }
        }
        if (bitIndex == mMemblemKIN4SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key10 = true; }
            else { mHisMemblemSwInfo[mHisCount].key10 = false; }
        }
        if (bitIndex == mMemblemKIN5SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key12 = true; }
            else { mHisMemblemSwInfo[mHisCount].key12 = false; }
        }
        if (bitIndex == mMemblemKIN6SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key14 = true; }
            else { mHisMemblemSwInfo[mHisCount].key14 = false; }
        }
        if (bitIndex == mMemblemKIN7SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key16 = true; }
            else { mHisMemblemSwInfo[mHisCount].key16 = false; }
        }
        if (bitIndex == mMemblemKIN8SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key18 = true; }
            else { mHisMemblemSwInfo[mHisCount].key18 = false; }
        }
        if (bitIndex == mMemblemKIN0SwBitNo){
            if (mHisMemblemSwInfo[0].key02 && mHisMemblemSwInfo[1].key02 && mHisMemblemSwInfo[2].key02){
                mMemblemSwInfo.key02 = true;                            // KEY02-ON
            } else {
                if (!mHisMemblemSwInfo[0].key02 && !mHisMemblemSwInfo[1].key02 && !mHisMemblemSwInfo[2].key02){
                    mMemblemSwInfo.key02 = false;                       // KEY02-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN1SwBitNo){
            if (mHisMemblemSwInfo[0].key04 && mHisMemblemSwInfo[1].key04 && mHisMemblemSwInfo[2].key04){
                mMemblemSwInfo.key04 = true;                            // KEY04-ON
            } else {
                if (!mHisMemblemSwInfo[0].key04 && !mHisMemblemSwInfo[1].key04 && !mHisMemblemSwInfo[2].key04){
                    mMemblemSwInfo.key04 = false;                       // KEY04-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN2SwBitNo){
            if (mHisMemblemSwInfo[0].key06 && mHisMemblemSwInfo[1].key06 && mHisMemblemSwInfo[2].key06){
                mMemblemSwInfo.key06 = true;                            // KEY06-ON
            } else {
                if (!mHisMemblemSwInfo[0].key06 && !mHisMemblemSwInfo[1].key06 && !mHisMemblemSwInfo[2].key06){
                    mMemblemSwInfo.key06 = false;                       // KEY06-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN3SwBitNo){
            if (mHisMemblemSwInfo[0].key08 && mHisMemblemSwInfo[1].key08 && mHisMemblemSwInfo[2].key08){
                mMemblemSwInfo.key08 = true;                            // KEY08-ON
            } else {
                if (!mHisMemblemSwInfo[0].key08 && !mHisMemblemSwInfo[1].key08 && !mHisMemblemSwInfo[2].key08){
                    mMemblemSwInfo.key08 = false;                       // KEY08-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN4SwBitNo){
            if (mHisMemblemSwInfo[0].key10 && mHisMemblemSwInfo[1].key10 && mHisMemblemSwInfo[2].key10){
                mMemblemSwInfo.key10 = true;                            // KEY10-ON
            } else {
                if (!mHisMemblemSwInfo[0].key10 && !mHisMemblemSwInfo[1].key10 && !mHisMemblemSwInfo[2].key10){
                    mMemblemSwInfo.key10 = false;                       // KEY10-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN5SwBitNo){
            if (mHisMemblemSwInfo[0].key12 && mHisMemblemSwInfo[1].key12 && mHisMemblemSwInfo[2].key12){
                mMemblemSwInfo.key12 = true;                            // KEY12-ON
            } else {
                if (!mHisMemblemSwInfo[0].key12 && !mHisMemblemSwInfo[1].key12 && !mHisMemblemSwInfo[2].key12){
                    mMemblemSwInfo.key12 = false;                       // KEY12-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN6SwBitNo){
            if (mHisMemblemSwInfo[0].key14 && mHisMemblemSwInfo[1].key14 && mHisMemblemSwInfo[2].key14){
                mMemblemSwInfo.key14 = true;                            // KEY14-ON
            } else {
                if (!mHisMemblemSwInfo[0].key14 && !mHisMemblemSwInfo[1].key14 && !mHisMemblemSwInfo[2].key14){
                    mMemblemSwInfo.key14 = false;                       // KEY14-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN7SwBitNo){
            if (mHisMemblemSwInfo[0].key16 && mHisMemblemSwInfo[1].key16 && mHisMemblemSwInfo[2].key16){
                mMemblemSwInfo.key16 = true;                            // KEY16-ON
            } else {
                if (!mHisMemblemSwInfo[0].key16 && !mHisMemblemSwInfo[1].key16 && !mHisMemblemSwInfo[2].key16){
                    mMemblemSwInfo.key16 = false;                       // KEY16-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN8SwBitNo){
            if (mHisMemblemSwInfo[0].key18 && mHisMemblemSwInfo[1].key18 && mHisMemblemSwInfo[2].key18){
                mMemblemSwInfo.key18 = true;                            // KEY18-ON
            } else {
                if (!mHisMemblemSwInfo[0].key18 && !mHisMemblemSwInfo[1].key18 && !mHisMemblemSwInfo[2].key18){
                    mMemblemSwInfo.key18 = false;                       // KEY18-OFF
                }
            }
        }
    } else if (mMemblemSelectSwStatus == MemblemSelectSwitchStatusOdd){
        if (bitIndex == mMemblemKIN0SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key01 = true; }
            else { mHisMemblemSwInfo[mHisCount].key01 = false; }
        }
        if (bitIndex == mMemblemKIN1SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key03 = true; }
            else { mHisMemblemSwInfo[mHisCount].key03 = false; }
        }
        if (bitIndex == mMemblemKIN2SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key05 = true; }
            else { mHisMemblemSwInfo[mHisCount].key05 = false; }
        }
        if (bitIndex == mMemblemKIN3SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key07 = true; }
            else { mHisMemblemSwInfo[mHisCount].key07 = false; }
        }
        if (bitIndex == mMemblemKIN4SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key09 = true; }
            else { mHisMemblemSwInfo[mHisCount].key09 = false; }
        }
        if (bitIndex == mMemblemKIN5SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key11 = true; }
            else { mHisMemblemSwInfo[mHisCount].key11 = false; }
        }
        if (bitIndex == mMemblemKIN6SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key13 = true; }
            else { mHisMemblemSwInfo[mHisCount].key13 = false; }
        }
        if (bitIndex == mMemblemKIN7SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key15 = true; }
            else { mHisMemblemSwInfo[mHisCount].key15 = false; }
        }
        if (bitIndex == mMemblemKIN8SwBitNo){
            if (!value) { mHisMemblemSwInfo[mHisCount].key17 = true; }
            else { mHisMemblemSwInfo[mHisCount].key17 = false; }
        }
        if (bitIndex == mMemblemKIN0SwBitNo){
            if (mHisMemblemSwInfo[0].key01 && mHisMemblemSwInfo[1].key01 && mHisMemblemSwInfo[2].key01){
                mMemblemSwInfo.key01 = true;                            // KEY01-ON
            } else {
                if (!mHisMemblemSwInfo[0].key01 && !mHisMemblemSwInfo[1].key01 && !mHisMemblemSwInfo[2].key01){
                    mMemblemSwInfo.key01 = false;                       // KEY01-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN1SwBitNo){
            if (mHisMemblemSwInfo[0].key03 && mHisMemblemSwInfo[1].key03 && mHisMemblemSwInfo[2].key03){
                mMemblemSwInfo.key03 = true;                            // KEY03-ON
            } else {
                if (!mHisMemblemSwInfo[0].key03 && !mHisMemblemSwInfo[1].key03 && !mHisMemblemSwInfo[2].key03){
                    mMemblemSwInfo.key03 = false;                       // KEY03-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN2SwBitNo){
            if (mHisMemblemSwInfo[0].key05 && mHisMemblemSwInfo[1].key05 && mHisMemblemSwInfo[2].key05){
                mMemblemSwInfo.key05 = true;                            // KEY05-ON
            } else {
                if (!mHisMemblemSwInfo[0].key05 && !mHisMemblemSwInfo[1].key05 && !mHisMemblemSwInfo[2].key05){
                    mMemblemSwInfo.key05 = false;                       // KEY05-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN3SwBitNo){
            if (mHisMemblemSwInfo[0].key07 && mHisMemblemSwInfo[1].key07 && mHisMemblemSwInfo[2].key07){
                mMemblemSwInfo.key07 = true;                            // KEY07-ON
            } else {
                if (!mHisMemblemSwInfo[0].key07 && !mHisMemblemSwInfo[1].key07 && !mHisMemblemSwInfo[2].key07){
                    mMemblemSwInfo.key07 = false;                       // KEY07-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN4SwBitNo){
            if (mHisMemblemSwInfo[0].key09 && mHisMemblemSwInfo[1].key09 && mHisMemblemSwInfo[2].key09){
                mMemblemSwInfo.key09 = true;                            // KEY09-ON
            } else {
                if (!mHisMemblemSwInfo[0].key09 && !mHisMemblemSwInfo[1].key09 && !mHisMemblemSwInfo[2].key09){
                    mMemblemSwInfo.key09 = false;                       // KEY09-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN5SwBitNo){
            if (mHisMemblemSwInfo[0].key11 && mHisMemblemSwInfo[1].key11 && mHisMemblemSwInfo[2].key11){
                mMemblemSwInfo.key11 = true;                            // KEY11-ON
            } else {
                if (!mHisMemblemSwInfo[0].key11 && !mHisMemblemSwInfo[1].key11 && !mHisMemblemSwInfo[2].key11){
                    mMemblemSwInfo.key11 = false;                       // KEY11-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN6SwBitNo){
            if (mHisMemblemSwInfo[0].key13 && mHisMemblemSwInfo[1].key13 && mHisMemblemSwInfo[2].key13){
                mMemblemSwInfo.key13 = true;                            // KEY13-ON
            } else {
                if (!mHisMemblemSwInfo[0].key13 && !mHisMemblemSwInfo[1].key13 && !mHisMemblemSwInfo[2].key13){
                    mMemblemSwInfo.key13 = false;                       // KEY13-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN7SwBitNo){
            if (mHisMemblemSwInfo[0].key15 && mHisMemblemSwInfo[1].key15 && mHisMemblemSwInfo[2].key15){
                mMemblemSwInfo.key15 = true;                            // KEY15-ON
            } else {
                if (!mHisMemblemSwInfo[0].key15 && !mHisMemblemSwInfo[1].key15 && !mHisMemblemSwInfo[2].key15){
                    mMemblemSwInfo.key15 = false;                       // KEY15-OFF
                }
            }
        }
        if (bitIndex == mMemblemKIN8SwBitNo){
            if (mHisMemblemSwInfo[0].key17 && mHisMemblemSwInfo[1].key17 && mHisMemblemSwInfo[2].key17){
                mMemblemSwInfo.key17 = true;                            // KEY17-ON
            } else {
                if (!mHisMemblemSwInfo[0].key17 && !mHisMemblemSwInfo[1].key17 && !mHisMemblemSwInfo[2].key17){
                    mMemblemSwInfo.key17 = false;                       // KEY17-OFF
                }
            }
        }
    }
}

void PhyKeyManager::decideSelectSw(SelectSwInfo selectSwInfo)
{
    if (!selectSwInfo.select01 && !selectSwInfo.select02 && selectSwInfo.select03){
        mSelectSwStatus = SelectSwitchStatusAuto;
    } else if (selectSwInfo.select01 && !selectSwInfo.select02 && !selectSwInfo.select03){
        //mSelectSwStatus = SelectSwitchStatusSetting;
        mSelectSwStatus = SelectSwitchStatusOrigin;
    } else if (selectSwInfo.select01 && !selectSwInfo.select02 && selectSwInfo.select03){
        mSelectSwStatus = SelectSwitchStatusManual;
    }
}

bool PhyKeyManager::judgeUpdateMemblemSw()
{
    bool ret = false;
    bool multiPress = false;
    if (mPreMemblemSwInfo.key03 != mMemblemSwInfo.key03){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key04 != mMemblemSwInfo.key04){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key05 != mMemblemSwInfo.key05){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key06 != mMemblemSwInfo.key06){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key07 != mMemblemSwInfo.key07){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key08 != mMemblemSwInfo.key08){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key09 != mMemblemSwInfo.key09){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key10 != mMemblemSwInfo.key10){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key11 != mMemblemSwInfo.key11){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (mPreMemblemSwInfo.key12 != mMemblemSwInfo.key12){
        if (ret) { multiPress = true; }
        ret = true;
    }
    if (multiPress){
        ret = false;
    }
    return ret;
}

bool PhyKeyManager::judgeUpdateModBusMemblemSw(bool *iSMemblemRelease)
{
    bool ret = false;
    if (!iSMemblemRelease){
        qDebug("%s:%d iSMemblemRelease[%p]", __func__, __LINE__, iSMemblemRelease);
        return ret;
    }
#ifdef ONLY_ONCE_PHYSICAL_MEMBLEM_KEY_NO_ISSUE
    if ((!mPreMemblemSwInfo.key01 && mMemblemSwInfo.key01) || (!mPreMemblemSwInfo.key02 && mMemblemSwInfo.key02) ||
        (!mPreMemblemSwInfo.key03 && mMemblemSwInfo.key03) || (!mPreMemblemSwInfo.key04 && mMemblemSwInfo.key04) ||
        (!mPreMemblemSwInfo.key05 && mMemblemSwInfo.key05) || (!mPreMemblemSwInfo.key06 && mMemblemSwInfo.key06) ||
        (!mPreMemblemSwInfo.key07 && mMemblemSwInfo.key07) || (!mPreMemblemSwInfo.key08 && mMemblemSwInfo.key08) ||
        (!mPreMemblemSwInfo.key09 && mMemblemSwInfo.key09) || (!mPreMemblemSwInfo.key10 && mMemblemSwInfo.key10) ||
        (!mPreMemblemSwInfo.key11 && mMemblemSwInfo.key11) || (!mPreMemblemSwInfo.key12 && mMemblemSwInfo.key12) ||
        (!mPreMemblemSwInfo.key13 && mMemblemSwInfo.key13) || (!mPreMemblemSwInfo.key14 && mMemblemSwInfo.key14) ||
        (!mPreMemblemSwInfo.key15 && mMemblemSwInfo.key15) || (!mPreMemblemSwInfo.key16 && mMemblemSwInfo.key16) ||
        (!mPreMemblemSwInfo.key17 && mMemblemSwInfo.key17) || (!mPreMemblemSwInfo.key18 && mMemblemSwInfo.key18)){
        ret = true;
    }
#else // ONLY_ONCE_PHYSICAL_MEMBLEM_KEY_NO_ISSUE
    if (mMemblemSwInfo.key01 || mMemblemSwInfo.key02 || mMemblemSwInfo.key03 ||
        mMemblemSwInfo.key04 || mMemblemSwInfo.key05 || mMemblemSwInfo.key06 ||
        mMemblemSwInfo.key07 || mMemblemSwInfo.key08 || mMemblemSwInfo.key09 ||
        mMemblemSwInfo.key10 || mMemblemSwInfo.key11 || mMemblemSwInfo.key12 ||
        mMemblemSwInfo.key13 || mMemblemSwInfo.key14 || mMemblemSwInfo.key15 ||
        mMemblemSwInfo.key16 || mMemblemSwInfo.key17 || mMemblemSwInfo.key18){
        ret = true;
    }
#endif // ONLY_ONCE_PHYSICAL_MEMBLEM_KEY_NO_ISSUE
    if ((mPreMemblemSwInfo.key01 && !mMemblemSwInfo.key01) || (mPreMemblemSwInfo.key02 && !mMemblemSwInfo.key02) ||
        (mPreMemblemSwInfo.key03 && !mMemblemSwInfo.key03) || (mPreMemblemSwInfo.key04 && !mMemblemSwInfo.key04) ||
        (mPreMemblemSwInfo.key05 && !mMemblemSwInfo.key05) || (mPreMemblemSwInfo.key06 && !mMemblemSwInfo.key06) ||
        (mPreMemblemSwInfo.key07 && !mMemblemSwInfo.key07) || (mPreMemblemSwInfo.key08 && !mMemblemSwInfo.key08) ||
        (mPreMemblemSwInfo.key09 && !mMemblemSwInfo.key09) || (mPreMemblemSwInfo.key10 && !mMemblemSwInfo.key10) ||
        (mPreMemblemSwInfo.key11 && !mMemblemSwInfo.key11) || (mPreMemblemSwInfo.key12 && !mMemblemSwInfo.key12) ||
        (mPreMemblemSwInfo.key13 && !mMemblemSwInfo.key13) || (mPreMemblemSwInfo.key14 && !mMemblemSwInfo.key14) ||
        (mPreMemblemSwInfo.key15 && !mMemblemSwInfo.key15) || (mPreMemblemSwInfo.key16 && !mMemblemSwInfo.key16) ||
        (mPreMemblemSwInfo.key17 && !mMemblemSwInfo.key17) || (mPreMemblemSwInfo.key18 && !mMemblemSwInfo.key18)){
        ret = true;
        *iSMemblemRelease = true;
    }
    return ret;
}

void PhyKeyManager::notifySwitchInfo()
{
    bool iSmemblemRelease = false;
    bool iSOperableRelease = false;

    if (judgeUpdateModBusOperableSw(&iSOperableRelease)){
        onUpdateModBusOperableSwInfo(mIsOperableSw);
        if (iSOperableRelease){
            notifyReleaseOperableSwInfo();
        }
    } else {
        if (iSOperableRelease){
            notifyReleaseOperableSwInfo();
        }
    }// modbus deadman SW

    if (mIsPreOperableSw != mIsOperableSw){
        mIsPreOperableSw = mIsOperableSw;
        onUpdateDeadmanSwInfo(mIsOperableSw);
    }// deadman SW

    if (mPreSelectSwStatus != mSelectSwStatus){
        mPreSelectSwStatus = mSelectSwStatus;
        if (!mIsTouchSelectSw){
            onUpdateSelectSwInfo(mSelectSwStatus);
        }
    }// select Sw

    if (mPreJogDialSwStatus != mJogDialSwStatus){
        mPreJogDialSwStatus = mJogDialSwStatus;
        m_jogDialSwLongPressInvalid = false;
        mJogDialSwMonitorCounter = 0;
        if (mJogDialSwStatus){
            if (mJogOpeMode == JogOperationModeInValid){
                onUpdateJogDialSwInvalidInfo(mJogDialSwStatus);
            } else {
                onUpdateJogDialSwValidInfo(mJogDialSwStatus);
            }
        } else {
            onUpdateJogDialSwRelease(mJogDialSwStatus);
        }
    }// jog dial Sw

    if (mJogDialSwStatus &&
        (!m_jogDialSwLongPressInvalid && mJogDialSwMonitorCounter >= mJogDialSwJudgeLongPressCnt)){
        onUpdateJogDialSwLongPress(mJogDialSwStatus);
    }// jog dial sw long press

    // memblem Sw
#if 1
#else
    if (judgeUpdateMemblemSw()){
        mPreMemblemSwInfo = mMemblemSwInfo;
        onUpdateMemblemSwInfo(mMemblemSwInfo);
    }
#endif
    // modbus memblem Sw
    if (judgeUpdateModBusMemblemSw(&iSmemblemRelease)){
        onUpdateModBusMemblemSwInfo(mMemblemSwInfo);
        if (iSmemblemRelease) {
            notifyReleaseMemblemSwInfo();
        }
        mPreMemblemSwInfo = mMemblemSwInfo;
    }
    if (mHisCount >= PHY_KEY_CHATTERING_PREVENTION_NUM){
        checkCalibrateLongPress();
    }
}

void PhyKeyManager::notifyReleaseMemblemSwInfo()
{
    int releaseMemblemInfo = 0;
    if (mPreMemblemSwInfo.key01 && !mMemblemSwInfo.key01){
        releaseMemblemInfo |= (1 << 0);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey01);
    }
    if (mPreMemblemSwInfo.key02 && !mMemblemSwInfo.key02){
        releaseMemblemInfo |= (1 << 1);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey02);
    }
    if (mPreMemblemSwInfo.key03 && !mMemblemSwInfo.key03){
        releaseMemblemInfo |= (1 << 2);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey03);
    }
    if (mPreMemblemSwInfo.key04 && !mMemblemSwInfo.key04){
        releaseMemblemInfo |= (1 << 3);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey04);
    }
    if (mPreMemblemSwInfo.key05 && !mMemblemSwInfo.key05){
        releaseMemblemInfo |= (1 << 4);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey05);
    }
    if (mPreMemblemSwInfo.key06 && !mMemblemSwInfo.key06){
        releaseMemblemInfo |= (1 << 5);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey06);
    }
    if (mPreMemblemSwInfo.key07 && !mMemblemSwInfo.key07){
        releaseMemblemInfo |= (1 << 6);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey07);
    }
    if (mPreMemblemSwInfo.key08 && !mMemblemSwInfo.key08){
        releaseMemblemInfo |= (1 << 7);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey08);
    }
    if (mPreMemblemSwInfo.key09 && !mMemblemSwInfo.key09){
        releaseMemblemInfo |= (1 << 8);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey09);
    }
    if (mPreMemblemSwInfo.key10 && !mMemblemSwInfo.key10){
        releaseMemblemInfo |= (1 << 9);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey10);
    }
    if (mPreMemblemSwInfo.key11 && !mMemblemSwInfo.key11){
        releaseMemblemInfo |= (1 << 10);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey11);
    }
    if (mPreMemblemSwInfo.key12 && !mMemblemSwInfo.key12){
        releaseMemblemInfo |= (1 << 11);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey12);
    }
    if (mPreMemblemSwInfo.key13 && !mMemblemSwInfo.key13){
        releaseMemblemInfo |= (1 << 12);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey13);
    }
    if (mPreMemblemSwInfo.key14 && !mMemblemSwInfo.key14){
        releaseMemblemInfo |= (1 << 13);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey14);
    }
    if (mPreMemblemSwInfo.key15 && !mMemblemSwInfo.key15){
        releaseMemblemInfo |= (1 << 14);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey15);
    }
    if (mPreMemblemSwInfo.key16 && !mMemblemSwInfo.key16){
        releaseMemblemInfo |= (1 << 15);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey16);
    }
    if (mPreMemblemSwInfo.key17 && !mMemblemSwInfo.key17){
        releaseMemblemInfo |= (1 << 16);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey17);
    }
    if (mPreMemblemSwInfo.key18 && !mMemblemSwInfo.key18){
        releaseMemblemInfo |= (1 << 17);
        initHisEachMemblemSwitchInfo((int)MemblemKeyNo::MemblemKey18);
    }
    if (releaseMemblemInfo != 0){
        emit updateReleaseMemblemSwInfo(releaseMemblemInfo);
    }
}

void PhyKeyManager::notifyJogInfo()
{
    if(mStepOperate && mSelectSwStatus == SelectSwitchStatusAuto){
        judgeJogScrollNotify();
    }
    if(mSelectSwStatus != SelectSwitchStatusManual){
        return ;
    }
    if (m_jogMagnificationSetting == Enums::JogMagnification1000) {
        updateJogDialSpeed();
        judgeJogScrollNotify();
        if (mPreJogCounter != mJogCounter){
            mUpdateJogInfo.jogDirection = mJogDirection;
            mUpdateJogInfo.jogCounter = mJogCounter;
            mPreJogCounter = mJogCounter;
            setJogCounter(mJogCounter);
        }
    } else {
        judgeJogScrollNotify();
        if (mPreJogCounter != mJogCounter){
            mUpdateJogInfo.jogDirection = mJogDirection;
            mUpdateJogInfo.jogCounter = mJogCounter;
            onUpdateJogCounterInfo(mUpdateJogInfo);
        }
        mCurJogCounter = mJogCounter;
        mPreJogCounter = mJogCounter;
    }
}

void PhyKeyManager::setTouchSelectStatus(bool isTouchSelectStatus)
{
    mIsTouchSelectSw = isTouchSelectStatus;
}

void PhyKeyManager::setStepOperate(bool isonoff)
{
    mStepOperate = isonoff;
}

void PhyKeyManager::onUpdateJogDialSwInvalidInfo(bool jogDialSwitchInvalid)
{
    setJogDialSwitchInvalid(jogDialSwitchInvalid);
    emit jogDialSwitchInvalidChanged(m_jogDialSwitchInvalid);
}

void PhyKeyManager::onUpdateJogDialSwValidInfo(bool jogDialSwitchValid)
{
    setJogDialSwitchValid(jogDialSwitchValid);
    emit jogDialSwitchValidChanged(m_jogDialSwitchValid);
}

void PhyKeyManager::onUpdateJogDialSwRelease(bool jogDialSwitchRelease)
{
    m_jogDialSwitchRelease = jogDialSwitchRelease;
    emit jogDialSwitchReleaseChanged();
    initHisJogDialSwitchInfo();
}

void PhyKeyManager::onUpdateJogScrollDown(bool jogScrollDown)
{
    setJogScrollDown(jogScrollDown);
    emit jogScrollDownChanged(m_jogScrollDown);
}

void PhyKeyManager::onUpdateJogScrollUp(bool jogScrollUp)
{
    setJogScrollUp(jogScrollUp);
    emit jogScrollUpChanged(m_jogScrollUp);
}

void PhyKeyManager::onUpdateMemblemSwInfo(PhyKeyManager::MemblemSwInfo memblemSwInfo)
{
    setMemblemSwKey03Press(memblemSwInfo.key03);
    if (memblemSwInfo.key03) { emit memblemSwKey03PressChanged(memblemSwInfo.key03);}
    setMemblemSwKey04Press(memblemSwInfo.key04);
    if (memblemSwInfo.key04) { emit memblemSwKey04PressChanged(memblemSwInfo.key04);}
    setMemblemSwKey05Press(memblemSwInfo.key05);
    if (memblemSwInfo.key05) { emit memblemSwKey05PressChanged(memblemSwInfo.key05);}
    setMemblemSwKey06Press(memblemSwInfo.key06);
    if (memblemSwInfo.key06) { emit memblemSwKey06PressChanged(memblemSwInfo.key06);}
    setMemblemSwKey07Press(memblemSwInfo.key07);
    if (memblemSwInfo.key07) { emit memblemSwKey07PressChanged(memblemSwInfo.key07);}
    setMemblemSwKey08Press(memblemSwInfo.key08);
    if (memblemSwInfo.key08) { emit memblemSwKey08PressChanged(memblemSwInfo.key08);}
    setMemblemSwKey09Press(memblemSwInfo.key09);
    if (memblemSwInfo.key09) { emit memblemSwKey09PressChanged(memblemSwInfo.key09);}
    setMemblemSwKey10Press(memblemSwInfo.key10);
    if (memblemSwInfo.key10) { emit memblemSwKey10PressChanged(memblemSwInfo.key10);}
    setMemblemSwKey11Press(memblemSwInfo.key11);
    if (memblemSwInfo.key11) { emit memblemSwKey11PressChanged(memblemSwInfo.key11);}
    setMemblemSwKey12Press(memblemSwInfo.key12);
    if (memblemSwInfo.key12) { emit memblemSwKey12PressChanged(memblemSwInfo.key12);}
}

void PhyKeyManager::onUpdateModBusMemblemSwInfo(PhyKeyManager::MemblemSwInfo memblemSwInfo)
{
    emit updateMemblemSwInfo(memblemSwInfo);
    setMemblemSwKey01Press(memblemSwInfo.key01);
    setMemblemSwKey02Press(memblemSwInfo.key02);
    setMemblemSwKey03Press(memblemSwInfo.key03);
    setMemblemSwKey04Press(memblemSwInfo.key04);
    setMemblemSwKey05Press(memblemSwInfo.key05);
    setMemblemSwKey06Press(memblemSwInfo.key06);
    setMemblemSwKey07Press(memblemSwInfo.key07);
    setMemblemSwKey08Press(memblemSwInfo.key08);
    setMemblemSwKey09Press(memblemSwInfo.key09);
    setMemblemSwKey10Press(memblemSwInfo.key10);
    setMemblemSwKey11Press(memblemSwInfo.key11);
    setMemblemSwKey12Press(memblemSwInfo.key12);
    setMemblemSwKey13Press(memblemSwInfo.key13);
    setMemblemSwKey14Press(memblemSwInfo.key14);
    setMemblemSwKey15Press(memblemSwInfo.key15);
    setMemblemSwKey16Press(memblemSwInfo.key16);
    setMemblemSwKey17Press(memblemSwInfo.key17);
    setMemblemSwKey18Press(memblemSwInfo.key18);
}

void PhyKeyManager::onUpdateSelectSwInfo(PhyKeyManager::SelectSwitchStatus selectSwStatus)
{
    setSelectSwMode((int)selectSwStatus);
    emit updateSelectSwInfo(selectSwStatus);
}

void PhyKeyManager::onUpdateJogDialSwLongPress(bool jogDialSwitchLongPress)
{
    setJogDialSwitchLongPress(jogDialSwitchLongPress);
    emit jogDialSwitchLongPressChanged(m_jogDialSwitchLongPress);
    mJogDialSwMonitorCounter = 0;
    m_jogDialSwLongPressInvalid = true;
    initHisJogDialSwitchInfo();
}

void PhyKeyManager::onUpdateDeadmanSwInfo(bool operableSwStatus)
{
    setOperableSwStatus(operableSwStatus);
}

bool PhyKeyManager::judgeUpdateModBusOperableSw(bool *iSOperableSwRelease)
{
    bool ret = false;
    if (!iSOperableSwRelease){
        qDebug("%s:%d iSOperableSwRelease[%p]", __func__, __LINE__, iSOperableSwRelease);
        return ret;
    }
    if (!mIsPreOperableSw && mIsOperableSw){
        //qDebug("%s:%d mIsPreOperableSw[%d] mIsOperableSw[%d]", __func__, __LINE__, mIsPreOperableSw, mIsOperableSw);
        ret = true;
    }
    if (mIsPreOperableSw && !mIsOperableSw){
#if 1
#else
        ret = true;
#endif
        *iSOperableSwRelease = true;
    }
    return ret;
}

void PhyKeyManager::onUpdateModBusOperableSwInfo(bool operableSwStatus)
{
    emit updateOperableSwInfo(operableSwStatus);
}

void PhyKeyManager::notifyReleaseOperableSwInfo()
{
    emit updateReleaseOperableSwInfo(mIsOperableSw);
    initHisOperableSwitchInfo();
}

bool PhyKeyManager::holdJogCounter()
{
    bool updated = false;
    int jogMagnification = (m_jogMagnification == 0 ? mJogDefaultMagnification : m_jogMagnification);
    int mod = mJogCounterInfo.jogCounter % mJogCounterThreshold;
    if ((mJogCounterInfo.jogCounter >= mPreJogCounterInfo.jogCounter && ((mJogCounterInfo.jogCounter - mPreJogCounterInfo.jogCounter) < 60000)) ||
        (mPreJogCounterInfo.jogCounter >= mJogCounterInfo.jogCounter && ((mPreJogCounterInfo.jogCounter - mJogCounterInfo.jogCounter) > 60000))) {
        mJogCounterInfo.jogCounter -= mod;
    } else {
        mJogCounterInfo.jogCounter += mod;
    }
    int jogCounterDiff = mJogCounterInfo.jogCounter - mPreJogCounterInfo.jogCounter;
    int jogPassJogOriginOffset = getJogPassJogOriginOffset(jogCounterDiff);
    if (abs(jogCounterDiff) >= mJogCounterThreshold) {
        mInternalJogCounter += (jogCounterDiff + jogPassJogOriginOffset) * jogMagnification / m_jogCounterDivision;
        updated = true;
    }
    //qDebug("%s:%d mInternalJogCounter[%d]", __func__, __LINE__, mInternalJogCounter);
    return updated;
}

int PhyKeyManager::getJogPassJogOriginOffset(int jogCounterDiff)
{
    int jogPassJogOriginOffset = 0;
    if (jogCounterDiff >= 60000) {
        jogPassJogOriginOffset = -(USHRT_MAX + 1);
    } else if (jogCounterDiff <= -60000) {
        jogPassJogOriginOffset = USHRT_MAX + 1;
    }
    return jogPassJogOriginOffset;
}

void PhyKeyManager::resetJogCounterInfo()
{
    mPreJogCounterInfo = getCurJogCounterInfo();
    //mInternalJogCounter = 0;
}

void PhyKeyManager::onUpdateJogCounterInfo(stUpdateJogInfo updateJogInfo)
{
    mJogCounterInfo = updateJogInfo;
    if (m_jogMagnification != 0){
        if (mMutexJogCounterUpdate) { mMutexJogCounterUpdate->lock();}
        bool updated = holdJogCounter();
        //qDebug("%s:%d mInternalJogCounter[%d]", __func__, __LINE__, mInternalJogCounter, updated);
        if (updated) {
            emit updateJogCounterInfo(mInternalJogCounter);
            mPreJogCounterInfo = mJogCounterInfo;
        }
        if (mMutexJogCounterUpdate) { mMutexJogCounterUpdate->unlock();}
    }
}

#ifdef REAL_TARGET
#else // REAL_TARGET
void PhyKeyManager::setEmulatedDeadManSwStatus(bool isDeadManSwitch)
{
    if (!mGpioControl){
        return;
    }
    if (isDeadManSwitch){
        mGpioControl->emulateDeadManSwOn();
    } else {
        mGpioControl->emulateDeadManSwOff();
    }
}
void PhyKeyManager::setEmulatedMemblemSwStatus(int memblemKeySwitchNo)
{
    if (!mGpioControl){
        return;
    }
    switch(memblemKeySwitchNo){
    case PhyKeyManager::MemblemKeyNo::MemblemKeyAllOff:
        mGpioControl->emulateKeyAllOff();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey01:
        mGpioControl->emulateKey01();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey02:
        mGpioControl->emulateKey02();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey03:
        mGpioControl->emulateKey03();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey04:
        mGpioControl->emulateKey04();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey05:
        mGpioControl->emulateKey05();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey06:
        mGpioControl->emulateKey06();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey07:
        mGpioControl->emulateKey07();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey08:
        mGpioControl->emulateKey08();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey09:
        mGpioControl->emulateKey09();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey10:
        mGpioControl->emulateKey10();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey11:
        mGpioControl->emulateKey11();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey12:
        mGpioControl->emulateKey12();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey13:
        mGpioControl->emulateKey13();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey14:
        mGpioControl->emulateKey14();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey15:
        mGpioControl->emulateKey15();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey16:
        mGpioControl->emulateKey16();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey17:
        mGpioControl->emulateKey17();
        break;
    case PhyKeyManager::MemblemKeyNo::MemblemKey18:
        mGpioControl->emulateKey18();
        break;
    default:
        qDebug("%s:%d invalid memblemKey:%d", __func__, __LINE__, memblemKeySwitchNo);
        break;
    }
}
void PhyKeyManager::setEmulatedJogDialSwStatus(bool isJogDialSwitch)
{
    if (!mGpioControl){
        return;
    }
    if (isJogDialSwitch){
        mGpioControl->emulateJogDialSwOn();
    } else {
        mGpioControl->emulateJogDialSwOff();
    }
}
void PhyKeyManager::incrementEmulatedJogCount()
{
    if (!mJogControl){
        return;
    }
    mJogControl->incrementEmulatedJogCount();
}
void PhyKeyManager::decrementEmulatedJogCount()
{
    if (!mJogControl){
        return;
    }
    mJogControl->decrementEmulatedJogCount();
}

void PhyKeyManager::setEmulatedSelectSwStatus(int selectSwStatus)
{
    if (!mGpioControl){
        return;
    }
    switch(selectSwStatus){
    case PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusOff:
        mGpioControl->emulateSelectSwOff();
        break;
    case PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusAuto:
        mGpioControl->emulateSelectSwAuto();
        break;
    case PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusManual:
        mGpioControl->emulateSelectSwManual();
        break;
    case PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusOrigin:
        mGpioControl->emulateSelectSwOrigin();
        break;
    case PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusSetting:
        mGpioControl->emulateSelectSwMenu();
        break;
    default:
        qDebug("%s:%d invalid selectSwStatus:%d", __func__, __LINE__, selectSwStatus);
    }
}
#endif // REAL_TARGET

void PhyKeyManager::ctrlLed(bool isOnOff, int ledType)
{
#ifdef REAL_TARGET
    if (!mGpioControl) return;
    switch(ledType){
    case CoreLibDriverManager::LedType::LedTypeBlue:
        mGpioControl->controlLed(isOnOff, CoreLibDriverManager::LedType::LedTypeBlue);
        break;
    case CoreLibDriverManager::LedType::LedTypeGreen:
        mGpioControl->controlLed(isOnOff, CoreLibDriverManager::LedType::LedTypeGreen);
        break;
    case CoreLibDriverManager::LedType::LedTypeRed:
        mGpioControl->controlLed(isOnOff, CoreLibDriverManager::LedType::LedTypeRed);
        break;
    default:
        break;
    }
#else // REAL_TARGET
    (void)isOnOff;
    (void)ledType;
#endif // REAL_TARGET
}

void PhyKeyManager::ctrlBuzzer()
{
    stBuzzerOutput buzzer;
    buzzer.onTime = 500;
    buzzer.offTime = 0;
    buzzer.repeat = 1;
    buzzer.pitch = m_currentBuzzerPitch;
    mGpioControl->controlBuzzer(m_currentBuzzerState,buzzer);
}

void PhyKeyManager::checkCalibrateLongPress(){
    if(m_calibrateLongPress == true){
        //qDebug("%s:%d m_calibrateLongPress:%d", __func__, __LINE__, m_calibrateLongPress);
        //qDebug("%s:%d mMemblemSwInfo.key01:%d mMemblemSwInfo.key18:%d", __func__, __LINE__, mMemblemSwInfo.key01, mMemblemSwInfo.key18);
        if(mMemblemSwInfo.key01 && mMemblemSwInfo.key18){
            if(m_calibrateLongPressCnt <= m_calibrateLongPressCounter){
                m_resource->changeApplication("TSLIB_CALIBFILE=/etc/pointercal ts_calibrate", "", "/opt/pendant-gui/bin/pendant-gui", "-L info -D restart ");

            }
            m_calibrateLongPressCounter++;
        }
        else{
            m_calibrateLongPress = false;
        }
    }
}



