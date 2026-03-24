#ifndef CORELIBDRIVERMANAGER_H
#define CORELIBDRIVERMANAGER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <QtGlobal>
#include <QBitArray>
#ifdef CORELIB_UNIT_TEST
#include "corelibunittestmockmethod.h"
#define OPEN mMockMethod._mock_open
#define CLOSE mMockMethod._mock_close
#define GET_DEV_FILE_PATH mMockMethod._mock_get_dev_file
#else // CORELIB_UNIT_TEST
#define OPEN open
#define CLOSE close
#define GET_DEV_FILE_PATH getDevFilePath
#endif // CORELIB_UNIT_TEST

class CoreLibDriverManager : public QObject
{
    Q_OBJECT
public:
    explicit CoreLibDriverManager(QObject *parent = nullptr);
    ~CoreLibDriverManager();
    void notifyJogCounterInfo(QString jogCounterInfo);
    enum DriverControlType {
        DriverControlTypeDisplayBrightness,     // Brightness
        DriverControlTypeGpioLedBlue,           // Led Blue
        DriverControlTypeGpioLedGreen,          // Led Green
        DriverControlTypeGpioLedRed,            // Led Red
        DriverControlTypeGpioBuzzer,            // buzzer
        DriverControlTypeGpioInputDataBuf,      // input buffer
        DriverControlTypeJogQPosCntControl,     // JogQPosCntControl
        DriverControlTypeJogQepStsControl,      // JogQepStsControl
        DriverControlTypeJogInputEvent,         // JogInputEvent
        DriverControlTypeWatchDogTimerClock,    // WatchDogTimer-Clock
        DriverControlTypeWatchDogTimerInh,      // WatchDogTimer-INH
        DriverControlTypeMax
    };

    bool openDevice(DriverControlType drvType);
    void closeDevice(DriverControlType drvType);

    enum LedType {
        LedTypeBlue,
        LedTypeGreen,
        LedTypeRed
    };
#ifdef CORELIB_UNIT_TEST
    CoreLibUnitTestMockMethod& getUnitTestMockMethod()
    {
        return mMockMethod;
    }
#endif // CORELIB_UNIT_TEST

private:
#ifdef CORELIB_RELEASE_TEST
    const QString displayBrightnessDevPath = "/tmp/class/backlight/backlight/brightness";
    const QString gpioLedBlueDevPath = "/tmp/class/leds/blue/brightness";
    const QString gpioLedGreenDevPath = "/tmp/class/leds/green/brightness";
    const QString gpioLedRedDevPath = "/tmp/class/leds/red/brightness";
    const QString gpioInputDataBufDevPath = "/tmp/devices/platform/itz3000-misc/data_buffer";
    const QString gpioBuzzerCtrlDevPath = "/tmp/devices/platform/itz3000-misc/buzzer_control";
    const QString JogQPosCntControlDevPath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";
    const QString JogQEpStsControlDevPath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qepsts_control";
    const QString JogInputEventDevPath = "/tmp/input/event1";
    const QString WatchDogTimerClkDevPath = "/tmp/watchdog";
    const QString WatchDogTimerInhDevPath = "/tmp/class/leds/wdt-inh/brightness";
#else
    const QString displayBrightnessDevPath = "/sys/class/backlight/backlight/brightness";
    const QString gpioLedBlueDevPath = "/sys/class/leds/blue/brightness";
    const QString gpioLedGreenDevPath = "/sys/class/leds/green/brightness";
    const QString gpioLedRedDevPath = "/sys/class/leds/red/brightness";
    const QString gpioInputDataBufDevPath = "/sys/devices/platform/itz3000-misc/data_buffer";
    const QString gpioBuzzerCtrlDevPath = "/sys/devices/platform/itz3000-misc/buzzer_control";
    const QString JogQPosCntControlDevPath = "/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";
    const QString JogQEpStsControlDevPath = "/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qepsts_control";
    const QString JogInputEventDevPath = "/dev/input/event1";
    const QString WatchDogTimerClkDevPath = "/dev/watchdog1";
    const QString WatchDogTimerInhDevPath = "/sys/class/leds/wdt-inh/brightness";
#endif

    bool judgeUseSystemCall(DriverControlType drvType);
    int judgeDevFileAccessMode(DriverControlType drvType);
    QIODevice::OpenMode judgeDevFileOpenMode(DriverControlType drvType);
    QString getDevFilePath(DriverControlType drvType);

protected:
    QFile *mDevFile[DriverControlTypeMax];
    int mDevFd[DriverControlTypeMax];
#ifdef CORELIB_UNIT_TEST
    CoreLibUnitTestMockMethod mMockMethod;
#endif // CORELIB_UNIT_TEST

signals:
    void updateJogCounterInfo(QString jogCounterInfo);
};

#endif // CORELIBDRIVERMANAGER_H
