#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include <QObject>
#include <QFile>
#include <fcntl.h>
#include <QTimerEvent>
#include <QDebug>
#include <unistd.h>
#ifdef REAL_TARGET
#include <sys/ioctl.h>
#endif // REAL_TARGET

#define OPEN open
#define CLOSE close
#define GET_DEV_FILE_PATH getDevFilePath

#define WDIOC_KEEPALIVE 0x80045705
#define WDIOC_SETTIMEOUT 0xc0045706
#define WDT_TIMEOUT_NUM 5               // seconds

class WatchDogManager : public QObject
{
public:
    static const QString DEVDIR;
    static const bool OFFLINE;
    explicit WatchDogManager(QObject *parent = nullptr);
    ~WatchDogManager();

    enum DriverControlType {
        DriverControlTypeWatchDogTimerClock,    // WatchDogTimer-Clock
        DriverControlTypeWatchDogTimerInh,      // WatchDogTimer-INH
        DriverControlTypeMax
    };

    void timerStart(int timeOut);
    void timerStop();

    void wdtProcess();

private:
    int     mTimerId;
    bool    mIsTimerStatus = false;
    bool    mIsInitWdt = false;

    const QString WatchDogTimerClkDevPath = DEVDIR + "/dev/watchdog1";
    const QString WatchDogTimerInhDevPath = DEVDIR + "/sys/class/leds/wdt-inh/brightness";

    bool openDevice(DriverControlType drvType);
    void closeDevice(DriverControlType drvType);
    bool judgeUseSystemCall(DriverControlType drvType);
    int judgeDevFileAccessMode(DriverControlType drvType);
    QIODevice::OpenMode judgeDevFileOpenMode(DriverControlType drvType);
    QString getDevFilePath(DriverControlType drvType);

    bool enableWdt(bool isEnable);
    int controlWDT(unsigned int cmd);

    bool initWDT();
    int aliveWDT();

protected:
    QFile *mDevFile[DriverControlTypeMax];
    int mDevFd[DriverControlTypeMax];

    void timerEvent(QTimerEvent *timerEvent);
};

#endif // WATCHDOGMANAGER_H
