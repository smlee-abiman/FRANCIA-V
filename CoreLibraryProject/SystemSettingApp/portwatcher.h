#ifndef PORTWATCHER_H
#define PORTWATCHER_H

#include <QThread>
#include <QTimer>
#include "corelibgpiocontrol.h"

class PortWatcher : public QThread
{
    Q_OBJECT
public:
    explicit PortWatcher(QObject *parent = 0);
    void run();

#ifdef REAL_TARGET
#else
    void emulatedKey01() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey01();
        }
    }

    void emulatedKey02() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey02();
        }
    }

    void emulatedKey03() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey03();
        }
    }

    void emulatedKey04() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey04();
        }
    }

    void emulatedKey05() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey05();
        }
    }

    void emulatedKey06() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey06();
        }
    }

    void emulatedKey07() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey07();
        }
    }

    void emulatedKey08() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey08();
        }
    }

    void emulatedKey09() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey09();
        }
    }

    void emulatedKey10() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey10();
        }
    }

    void emulatedKey11() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey11();
        }
    }

    void emulatedKey12() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey12();
        }
    }

    void emulatedKey13() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey13();
        }
    }

    void emulatedKey14() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey14();
        }
    }

    void emulatedKey15() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey15();
        }
    }

    void emulatedKey16() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey16();
        }
    }

    void emulatedKey17() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey17();
        }
    }

    void emulatedKey18() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKey18();
        }
    }

    void emulatedKeyAllOff() {
        if(gpioControl != nullptr) {
            gpioControl->emulateKeyAllOff();
        }
    }

    void emulatedJogDialSwStatus(bool jogDialSwStatus) {
        if (!gpioControl) { return; }
        qDebug() << "emulatedJogDialSwStatus() value:" << jogDialSwStatus;
        jogDialSwStatus ? gpioControl->emulateJogDialSwOn() :
                          gpioControl->emulateJogDialSwOff();
    }

    void emulatedDeadmanSwStatus(bool deadmanSwStatus) {
        if (!gpioControl) { return; }
        qDebug() << "emulatedDeadmanSwStatus() value:" << deadmanSwStatus;
        deadmanSwStatus ? gpioControl->emulateDeadManSwOn() :
                          gpioControl->emulateDeadManSwOff();
    }
#endif

signals:
    void updatePortInfo(QBitArray portInfo);

private slots:
    void timerHit();

private:
    CoreLibGpioControl *gpioControl = nullptr;
    const uchar mHoldInputDataBufCount = 6;
};

#endif // PORTWATCHER_H
