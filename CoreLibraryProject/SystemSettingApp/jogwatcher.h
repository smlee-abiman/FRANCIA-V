#ifndef JOGWATCHER_H
#define JOGWATCHER_H

#include <QThread>
#include <QString>
#include <QDebug>
#include "corelibjogcontrol.h"
#ifdef REAL_TARGET
#ifdef USE_POLLING_JOG_COUNTER
#include <QTimer>
#endif // USE_POLLING_JOG_COUNTER
#else // REAL_TARGET
#include <QTimer>
#endif // REAL_TARGET

class JogWatcher : public QThread
{
    Q_OBJECT
public:
    explicit JogWatcher(QObject *parent = 0);
    ~JogWatcher();
    void run();

#ifdef REAL_TARGET
#else
    void incrementEmulatedJogCount() {
        if(jogControl != nullptr) {
            jogControl->incrementEmulatedJogCount();
        }
    }

    void decrementEmulatedJogCount() {
        if(jogControl != nullptr) {
            jogControl->decrementEmulatedJogCount();
        }
    }
#endif

signals:
    void updateJogInfo(QString jogInfo);

#ifdef REAL_TARGET
#ifdef USE_POLLING_JOG_COUNTER
private slots:
    void timerHit();
#endif // USE_POLLING_JOG_COUNTER
#else // REAL_TARGET
private slots:
    void timerHit();
#endif // REAL_TARGET

private:
    CoreLibJogControl *jogControl = nullptr;
    QString mJogCounter = "0";
};

#endif // JOGWATCHER_H
