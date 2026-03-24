#ifndef USBDETECTTHREAD_H
#define USBDETECTTHREAD_H

#include <QThread>
#include <QTimer>

class UsbDetectThread : public QThread
{
    Q_OBJECT
public:
    explicit UsbDetectThread(QObject *parent = 0);

    void run();

signals:
    void usbMounted(QString usbMountPath);
    void usbUnmounted();

private:
    enum UsbMountState {
        Init = 0,
        Mounted,
        UnMounted
    };
    UsbMountState usbMountState = UsbMountState::Init;
    UsbMountState oldUsbMountState = UsbMountState::Init;

private slots:
    void timerHit();
};

#endif // USBDETECTTHREAD_H
