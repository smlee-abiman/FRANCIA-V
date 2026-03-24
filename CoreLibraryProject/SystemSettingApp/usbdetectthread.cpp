#include "usbdetectthread.h"
#include "corelibusbcontrol.h"

UsbDetectThread::UsbDetectThread(QObject *parent) : QThread(parent)
{

}

void UsbDetectThread::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);

    timer.setInterval(500);  // 500ms (0.5s)
    timer.start();

    exec();  // start event loop

    timer.stop();
}

void UsbDetectThread::timerHit()
{
    CoreLibUsbControl usbControl;
    QString usbMountPath = usbControl.getMountPath();

    if(usbMountPath.length() == 0) {
        usbMountState = UsbMountState::UnMounted;
        if(oldUsbMountState != usbMountState) {
            emit usbUnmounted();
            oldUsbMountState = usbMountState;
        }
    } else {
        usbMountState = UsbMountState::Mounted;
        if(oldUsbMountState != usbMountState) {
            emit usbMounted(usbMountPath);
            oldUsbMountState = usbMountState;
        }
    }
}
