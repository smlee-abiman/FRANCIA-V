#ifndef OUTPUTTESTTHREAD_H
#define OUTPUTTESTTHREAD_H

#include <QThread>
#include <QTimer>
#include "corelib.h"

class OutputTestThread : public QThread
{
    Q_OBJECT
public:
    explicit OutputTestThread(QObject *parent = 0);

    void run();

private:
    void buzzerOff();
    void lightLEDGreen();
    void lightLEDBlue();
    void lightLEDRed();
    void lightLEDOff();

private:
    enum LightLedState {
        LedNone,
        Green,
        Blue,
        Red
    };
    enum BuzzerState {
        BuzzerNone,
        C,  // Do
        D,  // Re
        E,  // Mi
        F,  // Fa
        G,  // Sol
        A,  // La
        B,  // Si
        CC  // Do
    };
    CoreLibGpioControl gpioCtl;
    LightLedState lightLEDState = LightLedState::LedNone;
    BuzzerState buzzerState = BuzzerState::BuzzerNone;

signals:
    void hoge();

private slots:
    void timerHit();
};

#endif // OUTPUTTESTTHREAD_H
