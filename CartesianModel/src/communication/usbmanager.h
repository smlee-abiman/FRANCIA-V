#ifndef USBMANAGER_H
#define USBMANAGER_H

#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QTimerEvent>
#include "corelibusbcontrol.h"

struct stUsbDirFileInfo {
    QString name;
    QString parent;
    QString path;
    QDateTime lastModified;
    qint64 size;
    bool isDir;
};

class UsbManager : public QThread
{
    Q_OBJECT
private:
    explicit UsbManager(QObject *parent = 0);
    ~UsbManager() = default;

    static UsbManager* instance;

public:
    static UsbManager* create_instance(QObject *parent = 0);
    static UsbManager* get_instance();
    static void destroy_instance();

    void run();
    QFileInfoList getFolderFileList(QString path, QDir::SortFlags sort, bool isSort = false);
    QFileInfoList getFolderList(QString path);
    bool copyFile(QString srcFilePath, QString dstFilePath, bool isOverWrite);
    bool createFolder(QString folderPath);
    bool getCurUsbMountStatus();
    void timerStart(int timeOut);
    void timerStop();
    QString mountPath() const { return mMountPath; }
    void terminate();
    void initialize();
private:
    enum UsbMountState {
        Init = 0,
        Mounted,
        UnMounted
    };
    UsbMountState usbMountState = UsbMountState::Init;
    UsbMountState oldUsbMountState = UsbMountState::Init;
    int     mTimerId;
    bool    mIsTimerStatus = false;
    int     mTimerCnt = 0;
    QString mMountPath = "";
#ifdef REAL_TARGET
    stUdevMonitor mUdm;
#endif // REAL_TARGET
    CoreLibUsbControl *mUsbControl = nullptr;

    void checkUsbMountStatus();

protected:
    void timerEvent(QTimerEvent *timerEvent);

private slots:
    void timerHit();

signals:
    void usbMounted(QString usbMountPath);
    void usbUnmounted();

};

#endif // USBMANAGER_H
