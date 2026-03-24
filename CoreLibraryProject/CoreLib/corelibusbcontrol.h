#ifndef CORELIBUSBCONTROL_H
#define CORELIBUSBCONTROL_H

#include "corelibexternalmemorymanger.h"
#include <QDir>
#ifdef REAL_TARGET
#include <libudev.h>
#endif // REAL_TARGET

#ifdef REAL_TARGET
//----------------------------------------------------------------------------------------------------
typedef struct{
    udev            *hotplug;
    udev_monitor    *monitor;
    char            node[128];
}stUdevMonitor;
#endif // REAL_TARGET

class CoreLibUsbControl : public CoreLibExternalMemoryManger
{
public:
    CoreLibUsbControl();
    ~CoreLibUsbControl();
    QString getMountPath();
    QFileInfoList getFolderFileList(QString path, QDir::SortFlags sort, bool isSort = false);
    QFileInfoList getFolderList(QString path);
#ifdef Q_OS_LINUX
    QString getMountPathUseSysCall();
    QString getMountPathAndSysPathUseSysCall(QString *sysPath);
    void execUsbMount(QString sysName, bool isAsync = true);
#ifdef REAL_TARGET
    int     initMonitor(stUdevMonitor* pUdm);
    void    deinitMonitor(stUdevMonitor* pUdm);
    int     getDevices(stUdevMonitor* pUdm, int needPrint);
    int     pollMonitor(stUdevMonitor* pUdm);
#endif // REAL_TARGET
#endif // Q_OS_LINUX
private:
    QString mSysName = "";
    QString mPreSysName = "";
    const QString mDevBasePath = "/dev/";
    const QString mMountBasePath = "/run/media/";
    const QString mAsyncOption = "async,flush";
    const QString mSyncOption = "";
    int mSysMountCmdStatus = -1;
};

#endif // CORELIBUSBCONTROL_H
