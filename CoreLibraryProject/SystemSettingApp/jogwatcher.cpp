#include "jogwatcher.h"

JogWatcher::JogWatcher(QObject *parent) 
    : QThread(parent),
      jogControl(nullptr)  
{
    jogControl = new CoreLibJogControl();
}

JogWatcher::~JogWatcher()
{
    if (jogControl){
#ifdef USE_POLLING_JOG_COUNTER
        jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
#else // USE_POLLING_JOG_COUNTER
        jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
#endif // USE_POLLING_JOG_COUNTER
        delete jogControl;
        jogControl = nullptr;
    }
}

#ifdef REAL_TARGET
void JogWatcher::run()
{
    bool openResult = false;
#ifdef USE_POLLING_JOG_COUNTER
    QTimer timer;
#else // USE_POLLING_JOG_COUNTER
    int ret = 0;
#endif // USE_POLLING_JOG_COUNTER
    if (!jogControl){
        return;
    }
    openResult = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQepStsControl);
    if (openResult == false){
        qDebug("Jog QepStsControl openDevice failed ret[%d]\n", openResult);
        return;
    }
    // jog dev open
#ifdef USE_POLLING_JOG_COUNTER
    openResult = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
#else // USE_POLLING_JOG_COUNTER
    openResult = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
#endif // USE_POLLING_JOG_COUNTER
    if (openResult == false){
        qDebug("Jog openDevice failed ret[%d]\n", openResult);
        return;
    }
#ifdef USE_POLLING_JOG_COUNTER
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
    timer.setInterval(100);
    timer.start();
    exec();  // start event loop
    timer.stop();
#else // USE_POLLING_JOG_COUNTER
    while(QThread::currentThread()->isInterruptionRequested() != true) {
        ret = jogControl->readJogCounterEvent(&mJogCounter);
        if (ret < 0){
            qDebug("readJogCounterEvent failed ret[%d]\n", ret);
            break;
        }
        qDebug() << "mJogCounter: " << mJogCounter;
        emit updateJogInfo(mJogCounter);
    }
    qDebug("isInterruptionRequested status:%d", QThread::currentThread()->isInterruptionRequested());
#endif // USE_POLLING_JOG_COUNTER
}
#else // REAL_TARGET
void JogWatcher::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
    timer.setInterval(25);
    timer.start();
    exec();  // start event loop
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()));
}
#endif // REAL_TARGET

#ifdef REAL_TARGET
#ifdef USE_POLLING_JOG_COUNTER
void JogWatcher::timerHit()
{
    int result = 0;
    result = jogControl->getJogCounter(&mJogCounter);
    if (result < 0){
        qDebug() << "result: " << result;
    }
    //qDebug() << "mJogCounter: " << mJogCounter;
    emit updateJogInfo(mJogCounter);
}
#endif // USE_POLLING_JOG_COUNTER
#else // REAL_TARGET
void JogWatcher::timerHit()
{
    int ret = 0;

    ret = jogControl->readJogCounterEvent(&mJogCounter);
    if (ret < 0){
        qDebug("readJogCounterEvent failed ret[%d]\n", ret);
        return;
    }
    //qDebug() << "mJogCounter: " << mJogCounter;
    emit updateJogInfo(mJogCounter);
}
#endif // REAL_TARGET
