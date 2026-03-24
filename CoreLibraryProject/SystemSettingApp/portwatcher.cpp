#include "portwatcher.h"

PortWatcher::PortWatcher(QObject *parent) : QThread(parent)
{

}

void PortWatcher::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);

    gpioControl = new CoreLibGpioControl();
#ifdef REAL_TARGET
    bool openResult = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    qDebug("openResult = %d", openResult);
    if(openResult){
#else
#endif
        timer.setInterval(25);
        timer.start();

        exec();  // start event loop

        timer.stop();
#ifdef REAL_TARGET
        gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    }
#else
#endif
    delete gpioControl;
    gpioControl = nullptr;
}

void PortWatcher::timerHit()
{
    QBitArray bitArray[mHoldInputDataBufCount];
    for (int i= 0; i < mHoldInputDataBufCount; i++){
        bitArray[i].resize(32);
    }
    //qDebug("call getKeyInfo");
    int result = gpioControl->getInputDataBuffer(bitArray, mHoldInputDataBufCount);
    //qDebug("getKeyInfo result = %d", result);
    (void)result;

    for(int i = 0; i < mHoldInputDataBufCount; i++) {
        for (int j = 0; j < 32; j++){
            bool value = bitArray[i].at(j);
            //qDebug("InputDataBuf[%d]: %d", j, value);
            (void)value;
        }
        emit updatePortInfo(bitArray[i]);
    }
}
