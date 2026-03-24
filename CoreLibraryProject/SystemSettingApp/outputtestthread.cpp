#include "outputtestthread.h"

OutputTestThread::OutputTestThread(QObject *parent) : QThread(parent)
{
}

void OutputTestThread::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);

    gpioCtl.openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
    gpioCtl.openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
    gpioCtl.openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    gpioCtl.openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);

    timer.setInterval(500);  // 500ms (0.5s)
    timer.start();

    exec();  // start event loop

    timer.stop();
    buzzerOff();
    lightLEDOff();

    lightLEDState = LightLedState::LedNone;
    buzzerState = BuzzerState::BuzzerNone;

    gpioCtl.closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
    gpioCtl.closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    gpioCtl.closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    gpioCtl.closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
}

void OutputTestThread::timerHit()
{
    stBuzzerOutput buzzer;
    buzzer.onTime = 500;
    buzzer.offTime = 0;
    buzzer.repeat = 1;

    switch(buzzerState) {
    case BuzzerState::BuzzerNone:
    case BuzzerState::CC:
        buzzer.pitch = 39;
        gpioCtl.controlBuzzer(true, buzzer);
        buzzerState = BuzzerState::C;
        break;

    case BuzzerState::C:
        buzzer.pitch = 41;
        gpioCtl.controlBuzzer(true, buzzer);
        buzzerState = BuzzerState::D;
        break;

    case BuzzerState::D:
        buzzer.pitch = 43;
        gpioCtl.controlBuzzer(true, buzzer);
        buzzerState = BuzzerState::E;
        break;

    case BuzzerState::E:
        buzzer.pitch = 44;
        gpioCtl.controlBuzzer(true, buzzer);
        buzzerState = BuzzerState::F;
        break;

    case BuzzerState::F:
        buzzer.pitch = 46;
        gpioCtl.controlBuzzer(true, buzzer);
        buzzerState = BuzzerState::G;
        break;

    case BuzzerState::G:
        buzzer.pitch = 48;
        gpioCtl.controlBuzzer(true, buzzer);
        buzzerState = BuzzerState::A;
        break;

    case BuzzerState::A:
        buzzer.pitch = 50;
        gpioCtl.controlBuzzer(true, buzzer);
        buzzerState = BuzzerState::B;
        break;

    case BuzzerState::B:
        buzzer.pitch = 51;
        gpioCtl.controlBuzzer(true, buzzer);
        buzzerState = BuzzerState::CC;
        break;
    }

    switch(lightLEDState) {
    case LightLedState::LedNone:
    case LightLedState::Red:
        lightLEDGreen();
        lightLEDState = LightLedState::Green;
        break;

    case LightLedState::Green:
        lightLEDBlue();
        lightLEDState = LightLedState::Blue;
        break;

    case LightLedState::Blue:
        lightLEDRed();
        lightLEDState = LightLedState::Red;
        break;
    }
}

void OutputTestThread::buzzerOff()
{
    stBuzzerOutput buzzer;
    buzzer.onTime = 0;
    buzzer.offTime = 0;
    buzzer.repeat = 0;
    buzzer.pitch = 0;
    gpioCtl.controlBuzzer(false, buzzer);
}

void OutputTestThread::lightLEDGreen()
{
    gpioCtl.controlLed(255, CoreLibDriverManager::LedType::LedTypeGreen);
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeBlue);
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeRed);
}

void OutputTestThread::lightLEDBlue()
{
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeGreen);
    gpioCtl.controlLed(255, CoreLibDriverManager::LedType::LedTypeBlue);
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeRed);
}

void OutputTestThread::lightLEDRed()
{
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeGreen);
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeBlue);
    gpioCtl.controlLed(255, CoreLibDriverManager::LedType::LedTypeRed);
}

void OutputTestThread::lightLEDOff()
{
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeGreen);
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeBlue);
    gpioCtl.controlLed(0, CoreLibDriverManager::LedType::LedTypeRed);
}
