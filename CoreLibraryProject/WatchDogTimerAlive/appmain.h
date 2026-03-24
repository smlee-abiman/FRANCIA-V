#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include <QCoreApplication>
#include <QTimer>
#include "watchdogmanager.h"

class AppMain : public QObject
{
    Q_OBJECT


public:
    explicit AppMain(QObject *parent, QCoreApplication *coreApp);
    ~AppMain();

private:
    QCoreApplication *app;

   WatchDogManager *m_wdtMgr;
   QTimer m_timer;

public slots:
    void run();
    void onTimeout();
};

#endif // APPMAIN_H
