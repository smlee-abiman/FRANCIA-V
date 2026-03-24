#ifndef PENDANTAPPLICATION_H
#define PENDANTAPPLICATION_H

#ifdef SPLASH_SCREEN_VALID
#include <QApplication>
#define mInheritApplication   QApplication
#else // SPLASH_SCREEN_VALID
#include <QGuiApplication>
#define mInheritApplication   QGuiApplication
#endif // SPLASH_SCREEN_VALID

#include <QtCore>
#include <QEvent>
#include <QMutex>

#include "phykeymanager.h"
#include "communicationthread.h"

class PendantApplication final : public mInheritApplication
{
    Q_OBJECT
//------------------------------------------------------
// static member.
//------------------------------------------------------
public:
    static PendantApplication* initializeInstance(int &argc, char **argv) {
        static PendantApplication instanse(argc, argv);
        if (!m_instance) {
            m_instance = &instanse;
        }
        return m_instance;
    }
    static PendantApplication* getInstance() {
        return m_instance;
    }

private:
    static PendantApplication* m_instance;

//------------------------------------------------------
// instance member.
//------------------------------------------------------
public:
    virtual ~PendantApplication() override;

    // note: already started monitoring, start monitoring again.
    void startOperationMonitoring(const int);
    void startPhyKeyMonitoring(PhyKeyManager&);
    void startCommunicationMonitoring(CommunicationThread& communication);

signals:
    void operationStarted();
    void operationStopped();

private:
    explicit PendantApplication(int &, char **);

    bool onOperated();
    bool notify(QObject *, QEvent *) override;
    bool operationMonitor(QEvent*);
    void connectPhyKeyManager();
    void disconnectPhyKeyManager();
    void connectCommunicationThread();
    void disconnectCommunicationThread();

private slots:
    void onOperationMonitorTimeout();
    void onPhyKeySwChanged();
    void onPhyKeySwStatusChanged(bool swStatus);
    void onPhyKeySwMemblemChanged(PhyKeyManager::MemblemSwInfo);
    void onPhyKeySwSelectChanged(PhyKeyManager::SelectSwitchStatus);
    void onFinished();

private:
    QTimer* m_operationMonitoringTimer;
    QMutex m_mutex;
    PhyKeyManager* m_phyKeyManager;
    CommunicationThread* m_communication;
};

#endif // PENDANTAPPLICATION_H
