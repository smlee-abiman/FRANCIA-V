#include "corelibusbcontrol.h"
#include <QStorageInfo>
#ifdef Q_OS_LINUX
#include <string.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#endif // Q_OS_LINUX

CoreLibUsbControl::CoreLibUsbControl()
{

}

CoreLibUsbControl::~CoreLibUsbControl()
{

}

QString CoreLibUsbControl::getMountPath()
{
    QString usbMountPath = "";
    QString usbFileSystemType = "vfat";  // Basic USB file system type

    foreach(const QStorageInfo &storageInfo, QStorageInfo::mountedVolumes()) {
        if(usbFileSystemType.compare(storageInfo.fileSystemType()) == 0) {
            usbMountPath = storageInfo.rootPath();
            break;
        }
    }

    return usbMountPath;
}

QFileInfoList CoreLibUsbControl::getFolderFileList(QString path, QDir::SortFlags sort, bool isSort)
{
    QDir dir(path);
    if (isSort){
        return dir.entryInfoList((QDir::Files | QDir::Dirs | QDir::NoDot | QDir::NoDotDot), sort);
    } else {
        return dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
    }
}

QFileInfoList CoreLibUsbControl::getFolderList(QString path)
{
    QDir dir(path);
    return dir.entryInfoList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
}

#ifdef Q_OS_LINUX
QString CoreLibUsbControl::getMountPathUseSysCall()
{
    QString usbMountPath = "";
    struct mntent *ent;
    FILE *aFile;

    aFile = setmntent("/proc/mounts", "r");
    if (aFile == NULL) {
        qCritical("%s:%d aFile[%p]", __func__, __LINE__, aFile);
        return usbMountPath;
    }
    while (NULL != (ent = getmntent(aFile))) {
        //qDebug("%-14s %s type[%s] mnt_opts[%s] mnt_freq[%d] mnt_passno[%d]",
        //    ent->mnt_dir, ent->mnt_fsname, ent->mnt_type, ent->mnt_opts, ent->mnt_freq, ent->mnt_passno);
        if(!strcmp((const char *)ent->mnt_type, (const char *)"vfat")){
            //qDebug("%s:%d mnt_type[%s] is match", __func__, __LINE__, ent->mnt_type);
            break;
        }
    }
    if (ent){
        //qDebug("%-14s %s type[%s] mnt_opts[%s] mnt_freq[%d] mnt_passno[%d]",
        //    ent->mnt_dir, ent->mnt_fsname, ent->mnt_type, ent->mnt_opts, ent->mnt_freq, ent->mnt_passno);
        int namelen = strlen(ent->mnt_dir);
        usbMountPath = QString::fromUtf8((const char *)ent->mnt_dir,namelen);
    }
    endmntent(aFile);
    //qDebug() << "getMountPathUseSysCall usbMountPath:" << usbMountPath
    //         << "mSysName:" << mSysName << "mPreSysName:" << mPreSysName << "mSysMountCmdStatus:" << mSysMountCmdStatus;
    return usbMountPath;
}

QString CoreLibUsbControl::getMountPathAndSysPathUseSysCall(QString *sysPath)
{
    QString usbMountPath = getMountPathUseSysCall();
    if (sysPath){
        qDebug() << "getMountPathAndSysPathUseSysCall usbMountPath:" << usbMountPath
            << "mSysName:" << mSysName << "mPreSysName:" << mPreSysName << "mSysMountCmdStatus:" << mSysMountCmdStatus;
        if (usbMountPath == "" && mSysName != "" && (mPreSysName == mSysName)){
            *sysPath = "/run/media/" + mSysName;
        }
    } else {
        qDebug("%s:%d sysPath[%p] is null", __func__, __LINE__, sysPath);
    }
    qDebug() << "getMountPathAndSysPathUseSysCall usbMountPath:" << usbMountPath;
    return usbMountPath;
}

void CoreLibUsbControl::execUsbMount(QString sysName, bool isAsync)
{
    if(sysName == ""){
        qWarning() << "execUsbMount devName is invalid:" << sysName << "isAsync:" << isAsync;
        return;
    }
    QString devName = "";
    QString mntCommandString = "";
    if(isAsync){
        devName = mDevBasePath + sysName;
        mntCommandString = "mount -o " + mAsyncOption + " " + devName + " " + mMountBasePath + sysName;
        qDebug() << "execUsbMount mntCommandString:" << mntCommandString << "mSysMountCmdStatus:" << mSysMountCmdStatus;
        QByteArray str_array = mntCommandString.toLocal8Bit();
        const char *command = str_array.constData();
        qDebug("%s:%d execUsbMount command[%s]", __func__, __LINE__, command);
        mSysMountCmdStatus = system(command);
        qDebug("%s:%d execUsbMount mSysMountCmdStatus[%d]", __func__, __LINE__, mSysMountCmdStatus);
    }
}

#ifdef REAL_TARGET
int CoreLibUsbControl::initMonitor(stUdevMonitor* pUdm)
{
    if (!pUdm) {
        qWarning("%s:%d pUdm[%p] is invalid", __func__, __LINE__, pUdm);
        return -1;
    }
    pUdm->hotplug = udev_new();
    if ( pUdm->hotplug == NULL ) { qCritical("%s:%d cannot create udev object", __func__, __LINE__); return -1; }
    // create the udev monitor
    pUdm->monitor = udev_monitor_new_from_netlink(pUdm->hotplug, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(pUdm->monitor, "block", NULL);

    // start receiving hotplug events
    udev_monitor_enable_receiving(pUdm->monitor);
    return 0;
}

void CoreLibUsbControl::deinitMonitor(stUdevMonitor* pUdm)
{
    if (!pUdm) {
        qWarning("%s:%d pUdm[%p] is invalid", __func__, __LINE__, pUdm);
        return;
    }
    // destroy the udev monitor
    udev_monitor_unref(pUdm->monitor);
    // destroy the udev object
    udev_unref(pUdm->hotplug);
    return;
}

int CoreLibUsbControl::getDevices(stUdevMonitor* pUdm, int needPrint)
{
    if (!pUdm) {
        qWarning("%s:%d pUdm[%p] is invalid", __func__, __LINE__, pUdm);
        return -1;
    }
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;

    // Create a list of the devices in the 'block' subsystem.
    enumerate = udev_enumerate_new(pUdm->hotplug);
    udev_enumerate_add_match_subsystem(enumerate, "block");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);

    int hasDevice = 0;
    // Enumerate device list
    udev_list_entry_foreach(dev_list_entry, devices) {
        const char *path, *node;
        // Get the filename of the /sys entry for the device and create a udev_device object (dev) representing it
        path = udev_list_entry_get_name(dev_list_entry);
        struct udev_device* dev = udev_device_new_from_syspath(pUdm->hotplug, path);

        // usb_device_get_devnode() returns the path to the device node itself in /dev.
        node = udev_device_get_devnode(dev);
        if ( needPrint == 1 ) qInfo("%s:%d Device Node Path: %s", __func__, __LINE__, node);

        // To get information about the USB device, need parent USB device tree
        dev = udev_device_get_parent_with_subsystem_devtype(dev,"block","disk");
        if ( dev == NULL && (strcmp(node, "/dev/sda") != 0)) {
            qDebug("%s:%d Unable to find parent usb device. node[%s]", __func__, __LINE__, node);
            return 0;
        }
        udev_device_unref(dev);
        if(strcmp(node, "/dev/sda") == 0){
            hasDevice = 1;
            qDebug("%s:%d . hasDevice[%d]", __func__, __LINE__, hasDevice);
            break;
        }
    }
    // Free the enumerator object
    udev_enumerate_unref(enumerate);
    return hasDevice;
}

int CoreLibUsbControl::pollMonitor(stUdevMonitor* pUdm)
{
    if (!pUdm) {
        qWarning("%s:%d pUdm[%p] is invalid", __func__, __LINE__, pUdm);
        return -1;
    }
    pollfd items[1];
    // Get the file descriptor (fd) for the monitor
    items[0].fd = udev_monitor_get_fd(pUdm->monitor);
    items[0].events = POLLIN;
    items[0].revents = 0;

    // Poll event with file descriptor
    if ( poll(items, 1, 100) <= 0 ) return 1;
    // receive the relevant device
    udev_device* dev = udev_monitor_receive_device(pUdm->monitor);
    if ( dev == NULL ) return 1;

    const char* action = udev_device_get_action(dev);
    const char* node = udev_device_get_devnode(dev);
    if ( strcmp(action,"remove") == 0 && strcmp(pUdm->node,node) == 0 ) {
        qDebug("%s:%d Device removed", __func__, __LINE__);
        mSysName = "";
        return 0;
    }
    qDebug("%s:%d Hotplug[%s]:",__func__, __LINE__, action);
    qDebug("\tNode: %s", node);
    qDebug("\tSubsystem: %s", udev_device_get_subsystem(dev));
    qDebug("\tDevtype: %s", udev_device_get_devtype(dev));
    qDebug("\tSysname: %s", udev_device_get_sysname(dev));

    int sysNameLen = strlen(udev_device_get_sysname(dev));
    QString usbSysName = QString::fromUtf8((const char *)udev_device_get_sysname(dev), sysNameLen);
    int nodeLen = strlen(node);
    QString nodeName = QString::fromUtf8((const char *)node, nodeLen);
    if ((strcmp(udev_device_get_subsystem(dev),"block") == 0)
        && ((strcmp(udev_device_get_devtype(dev),"disk") == 0) || (strcmp(udev_device_get_devtype(dev),"partition") == 0))
        && (nodeName.contains("sda"))){
        if ((strcmp(action,"add") == 0)){
            mSysName = usbSysName;
            qDebug() << "add mSysName:" << mSysName << "mPreSysName:" << mPreSysName << "mSysMountCmdStatus:" << mSysMountCmdStatus;
            if (mSysName == mPreSysName){
                execUsbMount(mSysName);
            } else {
                if (mSysMountCmdStatus > 0 && mSysMountCmdStatus != 0){
                    execUsbMount(mSysName);
                }
            }
        } else if (strcmp(action,"change") == 0){
            mSysName = usbSysName;
            qDebug() << "change mSysName:" << mSysName << "mPreSysName:" << mPreSysName << "mSysMountCmdStatus:" << mSysMountCmdStatus;
        } else if (strcmp(action,"remove") == 0){
            qDebug() << "remove mSysName:" << mSysName;
            if (mSysName == usbSysName){
                mPreSysName = usbSysName;
            }
            mSysName = "";
            mSysMountCmdStatus = -1;
            qDebug() << "remove mSysName:" << mSysName << "mPreSysName:" << mPreSysName;
        }
    } else if (strcmp(action,"remove") == 0){
        if (mSysName == usbSysName){
            mPreSysName = usbSysName;
        }
        mSysName = "";
        qDebug() << "remove-1 mSysName:" << mSysName << "mPreSysName:" << mPreSysName;
    }
    // destroy the relevant device
    udev_device_unref(dev);
    // clear the revents
    items[0].revents = 0;
    return 1;
}
#endif // REAL_TARGET
#endif // Q_OS_LINUX