#include "corelibdrivermanager.h"

CoreLibDriverManager::CoreLibDriverManager(QObject *parent) :
    QObject(parent),
    mDevFile{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    mDevFd{-1, -1, -1, -1, -1, -1, -1, -1, -1}
{
}

CoreLibDriverManager::~CoreLibDriverManager()
{
    for (int i = 0; i < DriverControlTypeMax; i++){
        if (mDevFile[i]){
            if (mDevFile[i]->isOpen()){
                mDevFile[i]->close();
            }
            delete mDevFile[i];
            mDevFile[i] = nullptr;
        }
        if (mDevFd[i] >= 0){
            CLOSE(mDevFd[i]);
            mDevFd[i] = -1;
        }
    }
}

bool CoreLibDriverManager::openDevice(DriverControlType drvType)
{
    if (drvType >= DriverControlTypeMax){
        qCritical() << "CoreLibDriverManager::openDevice drvType is not support" << drvType;
        return false;
    }
    QString devFilePath = GET_DEV_FILE_PATH(drvType);
    if (devFilePath == ""){
        qCritical() << "CoreLibDriverManager::openDevice devFilePath is nothing";
        return false;
    }
    if (judgeUseSystemCall(drvType)){
        int flags = judgeDevFileAccessMode(drvType);
        if (flags < 0){
            qCritical() << "CoreLibDriverManager::openDevice option flags " << flags;
            return false;
        }
        mDevFd[drvType] = OPEN(devFilePath.toUtf8().data(), flags);
        if (mDevFd[drvType] < 0){
            if (drvType != DriverControlTypeWatchDogTimerClock){
                qCritical() << "CoreLibDriverManager::openDevice dev file open error " << mDevFd[drvType] << "errno: "<< errno << "drvType:" << drvType;
            }
            return false;
        }
    } else {
        if (mDevFile[drvType] == nullptr){
            mDevFile[drvType] = new QFile();
            if (mDevFile[drvType] == nullptr){
                qCritical() << "CoreLibDriverManager::openDevice Qfile create failed " << drvType;
                return false;
            }
        }
        mDevFile[drvType]->setFileName(devFilePath);
        if( !mDevFile[drvType]->exists() ){
            qCritical() << "CoreLibDriverManager::openDevice dev file not exists " << devFilePath;
            return false;
        }
        if (!mDevFile[drvType]->open(judgeDevFileOpenMode(drvType))){
            qCritical() << "CoreLibDriverManager::openDevice dev file open error " << errno << "drvType:" << drvType;
            return false;
        }
    }
    return true;
}

void CoreLibDriverManager::closeDevice(DriverControlType drvType)
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

bool CoreLibDriverManager::judgeUseSystemCall(DriverControlType drvType)
{
    switch(drvType){
    case DriverControlTypeGpioBuzzer:
    case DriverControlTypeGpioInputDataBuf:
    case DriverControlTypeJogQPosCntControl:
    case DriverControlTypeJogQepStsControl:
    case DriverControlTypeJogInputEvent:
    case DriverControlTypeWatchDogTimerClock:
        return true;
    default:
        return false;
    }
}

int CoreLibDriverManager::judgeDevFileAccessMode(DriverControlType drvType)
{
    switch(drvType){
    case DriverControlTypeGpioInputDataBuf:
    case DriverControlTypeJogQPosCntControl:
    case DriverControlTypeJogQepStsControl:
        return O_RDONLY;
    case DriverControlTypeGpioBuzzer:
    case DriverControlTypeJogInputEvent:
        return O_RDWR;
    case DriverControlTypeWatchDogTimerClock:
        return O_WRONLY;
    default:
        return -1;
    }
}

QIODevice::OpenMode CoreLibDriverManager::judgeDevFileOpenMode(DriverControlType drvType)
{
    switch(drvType){
    case DriverControlTypeDisplayBrightness:
        return QIODevice::ReadWrite;
    case DriverControlTypeGpioLedBlue:
        return QIODevice::WriteOnly;
    case DriverControlTypeGpioLedGreen:
        return QIODevice::WriteOnly;
    case DriverControlTypeGpioLedRed:
        return QIODevice::WriteOnly;
    case DriverControlTypeWatchDogTimerInh:
        return QIODevice::WriteOnly;
    default:
        return QIODevice::ReadOnly;
    }
}

QString CoreLibDriverManager::getDevFilePath(DriverControlType drvType)
{
    switch(drvType){
    case DriverControlTypeDisplayBrightness:
        return displayBrightnessDevPath;
    case DriverControlTypeGpioLedBlue:
        return gpioLedBlueDevPath;
    case DriverControlTypeGpioLedGreen:
        return gpioLedGreenDevPath;
    case DriverControlTypeGpioLedRed:
        return gpioLedRedDevPath;
    case DriverControlTypeGpioInputDataBuf:
        return gpioInputDataBufDevPath;
    case DriverControlTypeGpioBuzzer:
        return gpioBuzzerCtrlDevPath;
    case DriverControlTypeJogQPosCntControl:
        return JogQPosCntControlDevPath;
    case DriverControlTypeJogQepStsControl:
        return JogQEpStsControlDevPath;
    case DriverControlTypeJogInputEvent:
        return JogInputEventDevPath;
    case DriverControlTypeWatchDogTimerClock:
        return WatchDogTimerClkDevPath;
    case DriverControlTypeWatchDogTimerInh:
        return WatchDogTimerInhDevPath;
    default:
        return "";
    }
}

void CoreLibDriverManager::notifyJogCounterInfo(QString jogCounterInfo)
{
    qDebug() << "jogCounterInfo:" << jogCounterInfo;
    emit updateJogCounterInfo(jogCounterInfo);
}
