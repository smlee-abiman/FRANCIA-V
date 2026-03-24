#include "usbmanager.h"

UsbManager::UsbManager(QObject *parent) : QThread(parent)
{

}

UsbManager* UsbManager::create_instance(QObject *parent) {
    if(!instance){
        instance = new UsbManager(parent);
    }
    return instance;
}

UsbManager* UsbManager::get_instance() {
    return instance;
}

void UsbManager::destroy_instance() {
    delete instance;
    instance = nullptr;
}

void UsbManager::terminate()
{
    if(mUsbControl){
        delete mUsbControl;
        mUsbControl = nullptr;
    }
}
void UsbManager::initialize()
{
    if (!mUsbControl){
        mUsbControl = new CoreLibUsbControl();
    }
}
void UsbManager::run()
{
    if (!mUsbControl){
        qWarning("%s:%d mUsbControl is invalid", __func__, __LINE__);
        return;
    }

#ifdef REAL_TARGET
    if(mUsbControl->initMonitor(&mUdm) != 0) {
        qWarning("%s:%d initMonitor failed", __func__, __LINE__);
    }
#endif // REAL_TARGET
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);

    timer.setInterval(100);  // 100ms (0.1s)
    timer.start();

    exec();  // start event loop
#ifdef REAL_TARGET
    mUsbControl->deinitMonitor(&mUdm);
#endif // REAL_TARGET
    timer.stop();
}

void UsbManager::timerHit()
{
    if (!mUsbControl){
        qWarning("%s:%d mUsbControl is invalid", __func__, __LINE__);
        return;
    }
#ifdef REAL_TARGET
    int result = mUsbControl->pollMonitor(&mUdm);
    //qDebug("%s:%d pollMonitor result[%d]", __func__, __LINE__, result);
#endif // REAL_TARGET
}

QFileInfoList UsbManager::getFolderFileList(QString path, QDir::SortFlags sort, bool isSort)
{
    QFileInfoList result;
    if (!mUsbControl){
        qWarning("%s:%d mUsbControl is invalid", __func__, __LINE__);
        return result;
    }
    return mUsbControl->getFolderFileList(path, sort, isSort);
}

QFileInfoList UsbManager::getFolderList(QString path)
{
    QFileInfoList result;
    if (!mUsbControl){
        qWarning("%s:%d mUsbControl is invalid", __func__, __LINE__);
        return result;
    }
    return mUsbControl->getFolderList(path);
}

bool UsbManager::copyFile(QString srcFilePath, QString dstFilePath, bool isOverWrite)
{
    bool result = false;
    if (!mUsbControl){
        qWarning("%s:%d mUsbControl is invalid", __func__, __LINE__);
        return result;
    }
    return mUsbControl->copyFile(srcFilePath, dstFilePath, isOverWrite);
}

bool UsbManager::createFolder(QString folderPath)
{
    bool result = false;
    if (!mUsbControl){
        qWarning("%s:%d mUsbControl is invalid", __func__, __LINE__);
        return result;
    }
    return mUsbControl->createFolder(folderPath);
}

bool UsbManager::getCurUsbMountStatus()
{
    bool result = false;
    if (!mUsbControl){
        qWarning("%s:%d mUsbControl is invalid", __func__, __LINE__);
        return result;
    }

#ifdef Q_OS_LINUX
    QString usbMountPath = mUsbControl->getMountPathUseSysCall();
#else
    QString usbMountPath = mUsbControl->getMountPath();
#endif // Q_OS_LINUX

    //qDebug() << "UsbManager::getCurUsbMountStatus usbMountPath:" << usbMountPath;
    if(usbMountPath.length() == 0) {
        mMountPath = "";
        usbMountState = UsbMountState::UnMounted;
        if(oldUsbMountState != usbMountState) {
            emit usbUnmounted();
            oldUsbMountState = usbMountState;
        }
        return false;
    } else {
        mMountPath = usbMountPath;
        usbMountState = UsbMountState::Mounted;
        if(oldUsbMountState != usbMountState) {
            emit usbMounted(usbMountPath);
            oldUsbMountState = usbMountState;
        }
        return true;
    }
}

void UsbManager::checkUsbMountStatus()
{
    if (!mUsbControl){
        qWarning("%s:%d mUsbControl is invalid", __func__, __LINE__);
        return;
    }

#ifdef Q_OS_LINUX
    QString usbMountPath = mUsbControl->getMountPathUseSysCall();
#else
    QString usbMountPath = mUsbControl->getMountPath();
#endif // Q_OS_LINUX
    //qDebug() << "UsbManager::checkUsbMountStatus usbMountPath:" << usbMountPath;
    if(usbMountPath.length() == 0) {
        usbMountState = UsbMountState::UnMounted;
        if(oldUsbMountState != usbMountState) {
            mMountPath = "";
            emit usbUnmounted();
            oldUsbMountState = usbMountState;
        }
    } else {
        usbMountState = UsbMountState::Mounted;
        if(oldUsbMountState != usbMountState) {
            mMountPath = usbMountPath;
            emit usbMounted(usbMountPath);
            oldUsbMountState = usbMountState;
        }
    }
}

void UsbManager::timerStart(int timeOut)
{
    if (!mIsTimerStatus) {
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void UsbManager::timerStop()
{
    if (mIsTimerStatus) {
        killTimer(mTimerId);
        mTimerCnt = 0;
        mIsTimerStatus = false;
    }
}

void UsbManager::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent) return;
    if(timerEvent->timerId() == mTimerId){
        checkUsbMountStatus();
    }
}

UsbManager* UsbManager::instance = nullptr;
