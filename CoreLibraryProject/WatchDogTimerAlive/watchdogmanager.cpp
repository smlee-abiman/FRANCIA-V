#include "watchdogmanager.h"

const QString WatchDogManager::DEVDIR = QString(qgetenv("TT_DEVICE_DIR")).trimmed();
const bool WatchDogManager::OFFLINE = WatchDogManager::DEVDIR.length() > 0;

WatchDogManager::WatchDogManager(QObject *parent) :
    QObject(parent)
{
}

WatchDogManager::~WatchDogManager()
{
#ifdef REAL_TARGET
    closeDevice(DriverControlType::DriverControlTypeWatchDogTimerInh);
    closeDevice(DriverControlType::DriverControlTypeWatchDogTimerClock);
#endif // REAL_TARGET
}

bool WatchDogManager::openDevice(DriverControlType drvType)
{
    if (drvType >= DriverControlTypeMax){
        qDebug() << "drvType is not support" << drvType;
        return false;
    }
    QString devFilePath = GET_DEV_FILE_PATH(drvType);
    if (devFilePath == ""){
        qDebug() << "devFilePath is nothing";
        return false;
    }
    if (judgeUseSystemCall(drvType)){
        int flags = judgeDevFileAccessMode(drvType);
        if (flags < 0){
            qDebug() << "option flags " << flags;
            return false;
        }
        mDevFd[drvType] = OPEN(devFilePath.toUtf8().data(), flags);
        if (mDevFd[drvType] < 0){
            if (drvType != DriverControlTypeWatchDogTimerClock){
                qDebug() << "dev file open error " << mDevFd[drvType] << "errno: "<< errno << "drvType:" << drvType;
            }
            return false;
        }
    } else {
        if (mDevFile[drvType] == nullptr){
            mDevFile[drvType] = new QFile();
            if (mDevFile[drvType] == nullptr){
                qDebug() << "Qfile create failed " << drvType;
                return false;
            }
        }
        mDevFile[drvType]->setFileName(devFilePath);
        if( !mDevFile[drvType]->exists() ){
            qDebug() << "dev file not exists " << devFilePath;
            return false;
        }
        if (!mDevFile[drvType]->open(judgeDevFileOpenMode(drvType))){
            qDebug() << "dev file open error " << errno << "drvType:" << drvType;
            return false;
        }
    }
    return true;
}

void WatchDogManager::closeDevice(DriverControlType drvType)
{
    if (mDevFd[drvType] >= 0){
        CLOSE(mDevFd[drvType]);
        mDevFd[drvType] = -1;
    }
    if (!mDevFile[drvType]){
        return;
    }
    mDevFile[drvType]->close();
}

bool WatchDogManager::judgeUseSystemCall(DriverControlType drvType)
{
    switch(drvType){
    case DriverControlTypeWatchDogTimerClock:
        return true;
    default:
        return false;
    }
}

int WatchDogManager::judgeDevFileAccessMode(DriverControlType drvType)
{
    switch(drvType){
    case DriverControlTypeWatchDogTimerClock:
        return O_WRONLY;
    default:
        return -1;
    }
}

QIODevice::OpenMode WatchDogManager::judgeDevFileOpenMode(DriverControlType drvType)
{
    switch(drvType){
    case DriverControlTypeWatchDogTimerInh:
        return QIODevice::WriteOnly;
    default:
        return QIODevice::ReadOnly;
    }
}

QString WatchDogManager::getDevFilePath(DriverControlType drvType)
{
    switch(drvType){
    case DriverControlTypeWatchDogTimerClock:
        return WatchDogTimerClkDevPath;
    case DriverControlTypeWatchDogTimerInh:
        return WatchDogTimerInhDevPath;
    default:
        return "";
    }
}

bool WatchDogManager::enableWdt(bool isEnable)
{
    if (isEnable){
        QString wdtCtrl = (isEnable == true ? "1" : "0");
        DriverControlType drvType = DriverControlTypeWatchDogTimerInh;
        QTextStream stream(mDevFile[drvType]);
        stream << wdtCtrl;
        stream.flush();
    }
    return true;
}

int WatchDogManager::controlWDT(unsigned int cmd)
{
#ifdef REAL_TARGET
    int result = 0;
    DriverControlType drvType = DriverControlTypeWatchDogTimerClock;
    if (mDevFd[drvType] <= 0){
        qDebug("%s:%d mDevFd[%d], drvType[%d]", __func__, __LINE__, mDevFd[drvType], drvType);
        return -1;
    }
    int arg = 0;
    switch(cmd){
    case WDIOC_KEEPALIVE:
        arg = 0;
        break;
    case WDIOC_SETTIMEOUT:
        arg = WDT_TIMEOUT_NUM;
        break;
    default:
        return -1;
    }
    result = ioctl(mDevFd[drvType], cmd, &arg);
    if (result < 0){
        qDebug("%s:%d result[%d] devfd[%d] 0x%08x errno[%d]", __func__, __LINE__, result, mDevFd[drvType], cmd, errno);
    }
    return result;
#else
    Q_UNUSED(cmd);
    return true;
#endif // REAL_TARGET
}

bool WatchDogManager::initWDT()
{
#ifdef REAL_TARGET
    bool openResult = false;
    int result = 0;

    openResult = openDevice(DriverControlType::DriverControlTypeWatchDogTimerClock);
    if (openResult == false){
        //qDebug("wdt-clock openDevice failed ret[%d]\n", openResult);
        return false;
    }
    openResult = openDevice(DriverControlType::DriverControlTypeWatchDogTimerInh);
    if (openResult == false){
        //qDebug("wdt-inh openDevice failed ret[%d]\n", openResult);
        closeDevice(DriverControlType::DriverControlTypeWatchDogTimerClock);
        return false;
    }
    result = controlWDT(WDIOC_SETTIMEOUT);
    if (result < 0){
        qDebug("controlWDT failed ret[%d]\n", result);
        goto error;
    }
    result = controlWDT(WDIOC_KEEPALIVE);
    if (result < 0){
        qDebug("controlWDT failed ret[%d]\n", result);
        goto error;
    }
    // enable watchdog
    openResult = enableWdt(true);
    if (openResult == false){
        qDebug("wdt-inh controlWdt failed ret[%d]\n", openResult);
        goto error;
    }
    return true;
error:
    closeDevice(DriverControlType::DriverControlTypeWatchDogTimerInh);
    closeDevice(DriverControlType::DriverControlTypeWatchDogTimerClock);
    return false;
#else
    qDebug("initWDT");
    return true;
#endif // REAL_TARGET
}

int WatchDogManager::aliveWDT()
{
    int result = 0;
#ifdef REAL_TARGET
    result = controlWDT(WDIOC_KEEPALIVE);
    if (result < 0){
        qDebug("controlWDT failed ret[%d]\n", result);
        return result;
    }
#endif // REAL_TARGET
    qDebug("aliveWDT");
    return result;
}

void WatchDogManager::wdtProcess()
{
    if (mIsInitWdt == false){
        mIsInitWdt = initWDT();
        //qDebug("%s:%d mIsInitWdt[%d]", __func__, __LINE__, mIsInitWdt);
    } else {
        aliveWDT();
    }
}

void WatchDogManager::timerStart(int timeOut)
{
    if (!mIsTimerStatus) {
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void WatchDogManager::timerStop()
{
    if (mIsTimerStatus) {
        killTimer(mTimerId);
        mIsTimerStatus = false;
    }
}

void WatchDogManager::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent) return;
    if(timerEvent->timerId() == mTimerId){
        wdtProcess();
    }
}
