#include <QCoreApplication>
#include <QDebug>
#include <QFile>

#ifdef __linux__
#    include <systemd/sd-daemon.h>
#endif

#include "platformhelpers.h"

PlatformHelpers::PlatformHelpers(QObject *parent) : QObject(parent) {}

Q_INVOKABLE void PlatformHelpers::notifyGuiReady()
{
#ifdef __linux__
    int r = sd_notifyf(0, "READY=1\nMAINPID=%ld",
                       static_cast<long>(QCoreApplication::applicationPid()));
    if (r < 0) {
        qDebug() << "failed to notify systemd: code =" << r;
    }
#endif
}
