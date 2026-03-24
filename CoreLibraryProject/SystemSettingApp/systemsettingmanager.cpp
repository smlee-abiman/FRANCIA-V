#include "systemsettingmanager.h"
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include "corelibusbcontrol.h"
#include "corelibtouchcontrol.h"
#include <QMap>
#include <QProcess>
#include <QDebug>
#include "corelibsdcontrol.h"

using namespace std;

bool nameLessThan(const stRemoteDirFileInfo &d1, const stRemoteDirFileInfo &d2)
{
    QString d1Name;
    QString d2Name;

    if(d1.parent.length() != 0) {
        d1Name = d1.parent + " " + d1.name;  // space is smaller than [0-9|a-z|A-Z|etc]
    } else {
        d1Name = d1.name;
    }

    if(d2.parent.length() != 0) {
        d2Name = d2.parent + " " + d2.name;  // space is smaller than [0-9|a-z|A-Z|etc]
    } else {
        d2Name = d2.name;
    }

    return d1Name < d2Name;
}

SystemSettingManager::SystemSettingManager(QObject *parent) : QObject(parent)
{
    CoreLibSdControl sdControl;
    QString userName = sdControl.getPendantData("FTP", "ID", "defaultUser").toString();  // default user name
    QString password = sdControl.getPendantData("FTP", "Password", "defaultPass").toString();  // default user pass
    QString ipAddr = sdControl.getControllerData("System", "IPAddress", "defaultIP").toString();  // default IP
    qint16 portNo = sdControl.getPendantData("FTP", "PortNo", 21).toInt();

    CoreLibFtpSettings *ftpSettings = new CoreLibFtpSettings(userName, password, ipAddr, portNo);
    m_coreLibFtp = new CoreLibFileTransferProtocol(ftpSettings);

    connect(m_coreLibFtp, SIGNAL(ftpCommandStarted(int)), this, SLOT(onFtpCommandStarted(int)));
    connect(m_coreLibFtp, SIGNAL(ftpCommandFinished(int, bool, QString)), this, SLOT(onFtpCommandFinished(int, bool, QString)));
    connect(m_coreLibFtp, SIGNAL(ftpRawCommandReply(int, const QString &)), this, SLOT(onFtpRawCommandReply(int, const QString &)));
    connect(m_coreLibFtp, SIGNAL(ftpStateChanged(int)), this, SLOT(onFtpStateChanged(int)));

    connectToFtp();
}

SystemSettingManager::~SystemSettingManager()
{
    disconnect(m_coreLibFtp, SIGNAL(ftpStateChanged(int)), this, SLOT(onFtpStateChanged(int)));
    disconnect(m_coreLibFtp, SIGNAL(ftpRawCommandReply(int, const QString &)), this, SLOT(onFtpRawCommandReply(int, const QString &)));
    disconnect(m_coreLibFtp, SIGNAL(ftpCommandFinished(int, bool, QString)), this, SLOT(onFtpCommandFinished(int, bool, QString)));
    disconnect(m_coreLibFtp, SIGNAL(ftpCommandStarted(int)), this, SLOT(onFtpCommandStarted(int)));
    m_coreLibFtp->execCloseCommand();
}

void SystemSettingManager::onFtpStateChanged(int ftpStatus)
{
    mFtpStatus = ftpStatus;
}

QString SystemSettingManager::selectdTopDirName() const
{
    return m_selectedTopDirName;
}

bool SystemSettingManager::isUSBConnected() const
{
    return m_isUSBConnected;
}

void SystemSettingManager::setSelectdTopDirName(QString topDirname)
{
    m_selectedTopDirName = topDirname;
    emit selectdTopDirNameChanged();
}

void SystemSettingManager::setIsUSBConnected(bool isUSBConnected)
{
    m_isUSBConnected = isUSBConnected;
    emit isUSBConnectedChanged();
}

void SystemSettingManager::startSystemSetting()
{
    mUsbDetectThread = new UsbDetectThread();
    connect(mUsbDetectThread, SIGNAL(usbMounted(QString)), this, SLOT(onUsbMounted(QString)));
    connect(mUsbDetectThread, SIGNAL(usbUnmounted()), this, SLOT(onUsbUnmounted()));
    mUsbDetectThread->start();
}

void SystemSettingManager::endSystemSetting()
{
    mUsbDetectThread->quit();
    mUsbDetectThread->wait();
    disconnect(mUsbDetectThread, SIGNAL(usbMounted(QString)), this, SLOT(onUsbMounted(QString)));
    disconnect(mUsbDetectThread, SIGNAL(usbUnmounted()), this, SLOT(onUsbUnmounted()));
    delete mUsbDetectThread;
}

void SystemSettingManager::onUsbMounted(QString usbMountPath)
{
    mUsbRootPath = usbMountPath + "/tietech";
    m_folderListModel->clearDirInfoList();
    m_fileListModel->beginReset();
    m_fileListModel->clearFileInfoList();
    m_fileListModel->endReset();
    m_folderListModel->updateDirList(mUsbRootPath, 0);
    setIsUSBConnected(true);
}

void SystemSettingManager::onUsbUnmounted()
{
    m_folderListModel->clearDirInfoList();
    m_fileListModel->beginReset();
    m_fileListModel->clearFileInfoList();
    m_fileListModel->endReset();
    setIsUSBConnected(false);
}

QString SystemSettingManager::copyDirection() const
{
    QString direction = "";

    switch(m_copyDirection) {
    case CopyDirection::InitDirection:
    case CopyDirection::UsbToController:
        direction = ">>>>>";
        break;

    case CopyDirection::ControllerToUsb:
        direction = "<<<<<";
        break;
    }

    return direction;
}

bool SystemSettingManager::isCopyToUSB() const
{
    return m_isCopyToUSB;
}

void SystemSettingManager::setCopyDirection(QString dummy)
{
    (void) dummy; // unused param

    switch(m_copyDirection) {
    case CopyDirection::InitDirection:
    case CopyDirection::UsbToController:
        m_copyDirection = CopyDirection::ControllerToUsb;
        setIsCopyToUSB(true);
        break;

    case CopyDirection::ControllerToUsb:
        m_copyDirection = CopyDirection::UsbToController;
        setIsCopyToUSB(false);
        break;
    }

    emit copyDirectionChanged();
}

void SystemSettingManager::setIsCopyToUSB(bool isCopyToUSB)
{
    m_isCopyToUSB = isCopyToUSB;
    emit isCopyToUSBChanged();
}

QString SystemSettingManager::getFromAppName()
{
    return mFromAppName;
}

void SystemSettingManager::setFromAppName(QString fromAppName)
{
    mFromAppName = fromAppName;
}

void SystemSettingManager::registerFolderListModel(FolderListModel* folderListModel)
{
    m_folderListModel = folderListModel;
}

void SystemSettingManager::registerFileListModel(FileListModel* fileListModel)
{
    m_fileListModel = fileListModel;
}

void SystemSettingManager::registerProxyModel(QSortFilterProxyModel* proxyModel)
{
    m_proxyModel = proxyModel;
}

void SystemSettingManager::folderSelected(int index)
{
    m_folderListModel->updateSelected(index);

    auto selectedDirInfo = m_folderListModel->getSelectedDirInfo();
    if(selectedDirInfo == nullptr) {
        // not selected.
        return;
    }
    auto& dirInfo = *selectedDirInfo;

    mLocalFileList.clear();

    // get file and directory list on selected top directory (USB)
    QDir::Filters filters = QDir::Filter::Dirs | QDir::Filter::Files | QDir::Filter::NoDotAndDotDot;
    QStringList nameFilters;
    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
    QDirIterator it(dirInfo.dirPath(), nameFilters, filters, flags);
    while(it.hasNext()) {
        QFileInfo fileInfo(it.next());
        stRemoteDirFileInfo localFileInfo;
        localFileInfo.name = fileInfo.fileName();
        localFileInfo.path = fileInfo.filePath();
        localFileInfo.size = fileInfo.size();
        localFileInfo.isDir = fileInfo.isDir();
        localFileInfo.parent = "";
        localFileInfo.lastModified = fileInfo.lastModified();
        mLocalFileList << localFileInfo;
    }

    // get file list on selected child directory (USB)
    filters = QDir::Filter::Files | QDir::Filter::NoDotAndDotDot;
    for(int i = 0; i < mLocalFileList.count(); i++) {
        stRemoteDirFileInfo info = mLocalFileList.at(i);
        if(!info.isDir) {
            continue;
        }
        QDirIterator childIt(info.path, nameFilters, filters, flags);
        while(childIt.hasNext()) {
            QFileInfo fileInfo(childIt.next());
            stRemoteDirFileInfo localFileInfo;
            localFileInfo.name = fileInfo.fileName();
            localFileInfo.path = fileInfo.filePath();
            localFileInfo.size = fileInfo.size();
            localFileInfo.isDir = fileInfo.isDir();
            localFileInfo.parent = info.name;
            localFileInfo.lastModified = fileInfo.lastModified();
            mLocalFileList << localFileInfo;
        }
    }

    // get file and directory list on selected directory (Controller)
    requestFtpListCommand(convertControllerPath(dirInfo.dirPath()));

    QDir topDir(dirInfo.dirPath());
    for(int i = 0; i < dirInfo.depth(); i++) {
        topDir.cdUp();
    }

    setSelectdTopDirName(topDir.dirName());
}

void SystemSettingManager::connectToFtp(int ftpState)
{
    switch (ftpState) {
    case QFtp::State::Closing:
    case QFtp::State::Unconnected:
        m_coreLibFtp->execConnectCommand();
        // fallthrough.
    case QFtp::State::HostLookup:
    case QFtp::State::Connecting:
    case QFtp::State::Connected:
        m_coreLibFtp->execLoginCommand();
        break;
    default:
    //case QFtp::State::LoggedIn:
        break;
    }
}

void SystemSettingManager::requestFtpListCommand(QString targetPath)
{
    mFtpListArgMap.clear();
    mFtpMdtmArgMap.clear();
    mRemoteFileList.clear();
    connectToFtp(mFtpStatus);
    m_coreLibFtp->execCdCommand(targetPath);
    mFirstListId = m_coreLibFtp->execListCommand("");  // get file and directory list on target top directory
    stFtpListArgument arg;
    arg.controllerDirPath = targetPath;
    arg.parent = "";
    mFtpListArgMap.insert(mFirstListId, arg);
}

QString SystemSettingManager::convertControllerPath(QString usbPath)
{
    QString controllerPath = usbPath;
    controllerPath.remove(0, mUsbRootPath.length() + 1 + 11);  // +1 is separator(/) length. delete "<usb_mount_path>/tietech/<SyyyyMMdd-x>"
    return controllerPath + "/";
}

void SystemSettingManager::onFtpCommandStarted(int id)
{
    if(m_coreLibFtp->getCurrentCommand() == QFtp::Command::RawCommand) {
        if(m_coreLibFtp->currentRawCommandName() == "MDTM") {
            stFtpMdtmArgument arg = mFtpMdtmArgMap.take(id);
            mMdtmTargetName = arg.name;
            mMdtmTargetParent = arg.parent;
            mMdtmIsPut = arg.isPut;
        }
    } else if(m_coreLibFtp->getCurrentCommand() == QFtp::Command::Get) {
        m_fileListModel->beginReset();
        stFtpGetArgument arg = mFtpGetArgMap[id];
        int index = arg.selectedIndex;
        FileInfo *selectedFile = m_fileListModel->getFileInfo(index);
        selectedFile->setNowCopying(true);
        m_fileListModel->endReset();
    } else if(m_coreLibFtp->getCurrentCommand() == QFtp::Command::Put) {
        m_fileListModel->beginReset();
        stFtpPutArgument arg = mFtpPutArgMap[id];
        int index = arg.selectedIndex;
        FileInfo *selectedFile = m_fileListModel->getFileInfo(index);
        selectedFile->setNowCopying(true);
        m_fileListModel->endReset();
    }
}

void SystemSettingManager::onFtpRawCommandReply(int replyCode, const QString & detail)
{
    (void) replyCode; // unused param

    if(m_coreLibFtp->currentRawCommandName() == "MDTM") {
        if(!mMdtmIsPut) {
            QString utcTimeStr = convertISOFormat(detail);
            QDateTime dateTime = QDateTime::fromString(utcTimeStr, Qt::ISODate);
            if(dateTime.isValid()) {
                dateTime.setTimeSpec(Qt::UTC);
                QDateTime localTime = dateTime.toLocalTime();
                for(int i = 0; i < mRemoteFileList.count(); i++) {
                    stRemoteDirFileInfo &info = mRemoteFileList[i];
                    if((info.parent.compare(mMdtmTargetParent) == 0) && (info.name.compare(mMdtmTargetName) == 0)) {
                        info.lastModified.swap(localTime);
                        break;
                    }
                }
            }
        }
    }
}

QString SystemSettingManager::convertISOFormat(QString dateTimeStr)
{
    if((dateTimeStr == nullptr) || (dateTimeStr.length() != 14)) {
        return dateTimeStr;
    }

    QString isoFormatStr = "";
    isoFormatStr.append(dateTimeStr.mid(0, 4));  // yyyy
    isoFormatStr.append("-");
    isoFormatStr.append(dateTimeStr.mid(4, 2));  // MM
    isoFormatStr.append("-");
    isoFormatStr.append(dateTimeStr.mid(6, 2));  // dd
    isoFormatStr.append("T");
    isoFormatStr.append(dateTimeStr.mid(8, 2));  // HH
    isoFormatStr.append(":");
    isoFormatStr.append(dateTimeStr.mid(10, 2));  // mm
    isoFormatStr.append(":");
    isoFormatStr.append(dateTimeStr.mid(12, 2));  // ss
    isoFormatStr.append("Z");  // Z (UTC)

    return isoFormatStr;
}

void SystemSettingManager::onFtpCommandFinished(int id, bool error, QString errorStr)
{
    bool isUpdateFileListModel = false;

    if(error) {
        qDebug() << "FTP command error: " << errorStr;
        if (m_coreLibFtp->getCurrentCommand() == QFtp::Command::Cd) {
            // The folder does not exist in the controller.
            // Update the file list with the folder structure on the USB side.
            isUpdateFileListModel = true;
        } else {
            return;
        }
    }

    if(m_coreLibFtp->getCurrentCommand() == QFtp::Command::Get) {
        stFtpGetArgument arg = mFtpGetArgMap.take(id);
        QFile *file = arg.getFile;
        file->close();
        QDateTime timestamp = arg.timestamp;
        QString command = "touch -t " + timestamp.toString("yyyyMMddHHmm.ss") + " " + file->fileName();
        QProcess::execute(command);
        delete file;

        m_fileListModel->beginReset();
        int index = arg.selectedIndex;
        FileInfo *selectedFile = m_fileListModel->getFileInfo(index);
        selectedFile->setNowCopying(false);
        m_fileListModel->updateSelected(arg.selectedIndex);
        m_fileListModel->endReset();
    }

    if(m_coreLibFtp->getCurrentCommand() == QFtp::Command::Put) {
        stFtpPutArgument arg = mFtpPutArgMap.take(id);
        QFile *file = arg.putFile;
        file->close();
        delete file;

        m_fileListModel->beginReset();
        int index = arg.selectedIndex;
        FileInfo *selectedFile = m_fileListModel->getFileInfo(index);
        selectedFile->setNowCopying(false);
        m_fileListModel->updateSelected(arg.selectedIndex);
        m_fileListModel->endReset();
    }

    if (m_coreLibFtp->getCurrentCommand() == QFtp::Command::List) {
        // get file and directory list.
        stFtpListArgument arg = mFtpListArgMap.take(id);
        QList<QUrlInfo> *fileList = m_coreLibFtp->getFileList();
        bool isTopDir = (mFirstListId == id);
        for(int i = 0; i < fileList->count(); i++) {
            const QUrlInfo &urlInfo = fileList->at(i);
            if(urlInfo.isDir() && !isTopDir) continue;  // skip directory.
            stRemoteDirFileInfo info;
            info.name = urlInfo.name();
            info.parent = arg.parent;
            info.lastModified = urlInfo.lastModified();
            info.size = urlInfo.size();
            info.isDir = urlInfo.isDir();
            info.path = arg.controllerDirPath;
            mRemoteFileList << info;
        }

        isUpdateFileListModel = true;
        if (mFirstListId == id) {
            // request LIST command. target is child directory.
            mFtpListArgMap.clear();
            for(int i = 0; i < mRemoteFileList.count(); i++) {
                stRemoteDirFileInfo info = mRemoteFileList.at(i);
                if((info.parent.length() == 0) && (info.isDir)) {
                    mLastListId = m_coreLibFtp->execListCommand(info.name);
                    stFtpListArgument listArg;
                    listArg.parent = info.name;
                    listArg.controllerDirPath = info.path + "/" + info.name;
                    mFtpListArgMap.insert(mLastListId, listArg);
                    isUpdateFileListModel = false;
                }
            }
        } else if (mLastListId == id) {
            // request MDTM command. target is all file.
            for(int i = 0; i < mRemoteFileList.count(); i++) {
                stRemoteDirFileInfo info = mRemoteFileList.at(i);
                if(info.isDir) continue;  // skip directory
                m_coreLibFtp->execCdCommand(info.path);
                mLastMdtmId = execMdtmCommand("", info.name, info.parent, false);
                isUpdateFileListModel = false;
            }
        } else {
            isUpdateFileListModel = false;
        }
    }

    if ((m_coreLibFtp->getCurrentCommand() == QFtp::Command::RawCommand) &&
        (m_coreLibFtp->currentRawCommandName() == "MDTM")) {
        mMdtmTargetName = "";
        mMdtmTargetParent = "";
        mMdtmIsPut = false;
        isUpdateFileListModel = (mLastMdtmId == id);
    }

    if (isUpdateFileListModel) {
        // done collect remote file list.
        // update file list model.
        m_fileListModel->beginReset();
        m_fileListModel->clearFileInfoList();
        for(int i = 0; i < mLocalFileList.count(); i++) {
            stRemoteDirFileInfo localFileInfo = mLocalFileList.at(i);
            if(localFileInfo.parent.length() != 0) continue;  // skip child
            if(localFileInfo.isDir) {
                m_fileListModel->addUsbFileInfo(localFileInfo.name, localFileInfo.isDir, localFileInfo.parent, false, localFileInfo.path, localFileInfo.lastModified, localFileInfo.size, isHasDiffChildFiles(localFileInfo.name));
            } else {
                m_fileListModel->addUsbFileInfo(localFileInfo.name, localFileInfo.isDir, localFileInfo.parent, false, localFileInfo.path, localFileInfo.lastModified, localFileInfo.size, isHasDiffFile(localFileInfo, false));
            }
        }

        for(int i = 0; i < mRemoteFileList.count(); i++) {
            stRemoteDirFileInfo remoteFileInfo = mRemoteFileList.at(i);
            if(remoteFileInfo.parent.length() != 0) continue;  // skip child
            if(remoteFileInfo.isDir) {
                m_fileListModel->addControllerFileInfo(remoteFileInfo.name, remoteFileInfo.isDir, remoteFileInfo.parent, false, remoteFileInfo.path, remoteFileInfo.lastModified, remoteFileInfo.size, isHasDiffChildFiles(remoteFileInfo.name));
            } else {
                m_fileListModel->addControllerFileInfo(remoteFileInfo.name, remoteFileInfo.isDir, remoteFileInfo.parent, false, remoteFileInfo.path, remoteFileInfo.lastModified, remoteFileInfo.size, isHasDiffFile(remoteFileInfo, true));
            }
        }
        m_fileListModel->endReset();
    }
}

bool SystemSettingManager::isHasDiffChildFiles(QString parent)
{
    bool hasDiff = false;

    QList<stRemoteDirFileInfo> usbChildList;
    for(int i = 0; i < mLocalFileList.count(); i++) {
        stRemoteDirFileInfo info = mLocalFileList.at(i);
        if((info.parent.compare(parent) == 0) && (!info.isDir)) {
            usbChildList << info;
        }
    }

    QList<stRemoteDirFileInfo> controllerChildList;
    for(int i = 0; i < mRemoteFileList.count(); i++) {
        stRemoteDirFileInfo info = mRemoteFileList.at(i);
        if((info.parent.compare(parent) == 0) && (!info.isDir)) {
            controllerChildList << info;
        }
    }

    // check file count
    if(usbChildList.count() != controllerChildList.count()) {
        hasDiff = true;
        return hasDiff;
    }

    std::sort(usbChildList.begin(), usbChildList.end(), nameLessThan);
    std::sort(controllerChildList.begin(), controllerChildList.end(), nameLessThan);

    for(int i = 0; i < usbChildList.count(); i++) {
        stRemoteDirFileInfo usbFile = usbChildList.at(i);
        stRemoteDirFileInfo controllerFile = controllerChildList.at(i);
        // check file name
        if(usbFile.name.compare(controllerFile.name) != 0) {
            hasDiff = true;
            break;
        }
        // check file size
        if(usbFile.size != controllerFile.size) {
            hasDiff = true;
            break;
        }
        // check file lastModified
        if(usbFile.lastModified != controllerFile.lastModified) {
            hasDiff = true;
            break;
        }
    }

    return hasDiff;
}

bool SystemSettingManager::isHasDiffFile(stRemoteDirFileInfo checkFile, bool isTargetLocal)
{
    bool isHasDiff = true;

    if(isTargetLocal) {
        for(int i = 0; i < mLocalFileList.count(); i++) {
            stRemoteDirFileInfo info = mLocalFileList.at(i);
            if((info.parent.compare(checkFile.parent) != 0) || (info.name.compare(checkFile.name) != 0)) {
                continue;
            }
            if(info.size == checkFile.size) isHasDiff = false;
            if(info.lastModified == checkFile.lastModified) isHasDiff = false;
            break;
        }
    } else {
        for(int i = 0; i < mRemoteFileList.count(); i++) {
            stRemoteDirFileInfo info = mRemoteFileList.at(i);
            if((info.parent.compare(checkFile.parent) != 0) || (info.name.compare(checkFile.name) != 0)) {
                continue;
            }
            if(info.size == checkFile.size) isHasDiff = false;
            if(info.lastModified == checkFile.lastModified) isHasDiff = false;
            break;
        }
    }

    return isHasDiff;
}

QModelIndex SystemSettingManager::toFileListModelIndex(int index)
{
    auto proxyModelIndex = m_proxyModel->index(index, 0);
    return m_proxyModel->mapToSource(proxyModelIndex);
}

int SystemSettingManager::execMdtmCommand(QString updateLastModified, QString targetName, QString parent, bool isPut)
{
    const QString commandName = "MDTM";
    QStringList argument;
    if(updateLastModified.length() == 0) {
        argument << targetName;
    } else {
        argument << updateLastModified << targetName;
    }
    int rawCommandId = m_coreLibFtp->execRawCommand(commandName, argument);
    stFtpMdtmArgument arg;
    arg.name = QFile(targetName).fileName();
    arg.parent = parent;
    arg.isPut = isPut;
    mFtpMdtmArgMap.insert(rawCommandId, arg);
    return rawCommandId;
}

void SystemSettingManager::folderOpen(QString folderPath, int depth)
{
    m_folderListModel->updateDirList(folderPath, depth + 1);
}

void SystemSettingManager::onFileSelected(int index)
{
    auto fileListModelIndex = toFileListModelIndex(index);
    m_fileListModel->beginReset();
    m_fileListModel->updateSelected(fileListModelIndex);
    m_fileListModel->endReset();
}

void SystemSettingManager::onChildFolderOpened(int index)
{
    auto fileListModelIndex = toFileListModelIndex(index);
    FileInfo *fileInfo = m_fileListModel->getFileInfo(fileListModelIndex);
    QString parentName = fileInfo->name();
    bool isRemoved = false;

    m_fileListModel->beginReset();
    isRemoved = m_fileListModel->removeChildFolder(parentName);

    if(isRemoved) {
        // child folder is already opened. to Close.
        m_fileListModel->endReset();
        return;
    }

    for(int i = 0; i < mLocalFileList.count(); i++) {
        stRemoteDirFileInfo info = mLocalFileList.at(i);
        if(info.parent.compare(parentName) != 0) continue;
        if(info.isDir) {
            m_fileListModel->addUsbFileInfo(info.name, info.isDir, info.parent, false, info.path, info.lastModified, info.size, isHasDiffChildFiles(info.name));
        } else {
            m_fileListModel->addUsbFileInfo(info.name, info.isDir, info.parent, false, info.path, info.lastModified, info.size, isHasDiffFile(info, false));
        }
    }

    for(int i = 0; i < mRemoteFileList.count(); i++) {
        stRemoteDirFileInfo info = mRemoteFileList.at(i);
        if(info.parent.compare(parentName) != 0) continue;
        if(info.isDir) {
            m_fileListModel->addControllerFileInfo(info.name, info.isDir, info.parent, false, info.path, info.lastModified, info.size, isHasDiffChildFiles(info.name));
        } else {
            m_fileListModel->addControllerFileInfo(info.name, info.isDir, info.parent, false, info.path, info.lastModified, info.size, isHasDiffFile(info, true));
        }
    }

    m_fileListModel->endReset();
}

void SystemSettingManager::onCalibrationClicked()
{
    CoreLibTouchControl touchControl;
    touchControl.executeCaribration(getFromAppName());
}

void SystemSettingManager::onExecuteClicked(QString currentPath)
{
    QList<int> selectedIndexList = m_fileListModel->getSelectedIndex();
    if(selectedIndexList.count() == 0) {
        return;
    }

    mFtpGetArgMap.clear();
    mFtpPutArgMap.clear();

    connectToFtp(mFtpStatus);
    m_fileListModel->beginReset();
    for(int i = 0; i < selectedIndexList.count(); i++) {
        FileInfo* selectedFile = m_fileListModel->getFileInfo(selectedIndexList[i]);
        if(isCopyToUSB()) {
            QString dstDirPath;
            if(selectedFile->parent().length() == 0) {
                dstDirPath = currentPath;
            } else {
                dstDirPath = currentPath + "/" + selectedFile->parent();
            }
            QString srcDirPath = convertControllerPath(dstDirPath);
            getFile = new QFile(dstDirPath + "/" + selectedFile->name());
            if(getFile->exists()) {
                // target already exists in USB
                getFile->remove();
            }
            getFile->open(QFile::WriteOnly);
            m_coreLibFtp->execCdCommand(srcDirPath);
            int id = m_coreLibFtp->execGetCommand(getFile, selectedFile->name());
            stFtpGetArgument arg;
            arg.getFile = getFile;
            arg.selectedIndex = selectedIndexList[i];
            arg.timestamp = selectedFile->controllerLastModified();
            mFtpGetArgMap.insert(id, arg);
            m_fileListModel->updateUsbLastModified(selectedIndexList[i], selectedFile->controllerLastModified());
        } else {
            QString srcDirPath;
            if(selectedFile->parent().length() == 0) {
                srcDirPath = currentPath;
            } else {
                srcDirPath = currentPath + "/" + selectedFile->parent();
            }
            putFile = new QFile(srcDirPath + "/" + selectedFile->name());
            putFile->open(QFile::ReadOnly);
            QString dstDirPath = convertControllerPath(srcDirPath);
            m_coreLibFtp->execCdCommand(dstDirPath);
            if(selectedFile->controllerLastModified().isValid()) {
                // target already exists in controller
                m_coreLibFtp->execRemoveCommand(selectedFile->name());
            }
            int id = m_coreLibFtp->execPutCommand(putFile, selectedFile->name());
            stFtpPutArgument arg;
            arg.putFile = putFile;
            arg.selectedIndex = selectedIndexList[i];
            mFtpPutArgMap.insert(id, arg);
            execMdtmCommand(selectedFile->usbLastModified().toUTC().toString("yyyyMMddHHmmss"), selectedFile->name(), selectedFile->parent(), true);
            stControllerInfo controllerInfo;
            controllerInfo.filePath = dstDirPath + "/" + selectedFile->name();
            controllerInfo.lastModified = selectedFile->usbLastModified();
            controllerInfo.size = selectedFile->usbSize();
            m_fileListModel->updateControllerInfo(selectedIndexList[i],  controllerInfo);
        }
    }
    m_fileListModel->endReset();
}

void SystemSettingManager::onDirectionClicked()
{
    m_fileListModel->beginReset();
    m_fileListModel->resetSelectState();
    m_fileListModel->endReset();
    setCopyDirection("dummy");
}

void SystemSettingManager::onCreateFolderClicked()
{
    CoreLibUsbControl usbControl;
    QDateTime dt = QDateTime::currentDateTime();
    for(int folderIndex = 1; folderIndex <= 9; folderIndex++) {
        QString folderName = "S" + dt.toString("yyyyMMdd") + "-" + QString::number(folderIndex);
        QString dirPath = mUsbRootPath + "/" + folderName;
        QDir createDir(dirPath);
        if(!createDir.exists()) {
            if(usbControl.createFolder(dirPath)) {
                m_folderListModel->addDirInfo(createDir.dirName(), dirPath, 0, false);
                m_folderListModel->sortDirList();
            }
            return;
        }
    }

    // All folder exist.
    emit folderCreateFailed();
}

void SystemSettingManager::onEraceFolderClicked()
{
    DirInfo *dirInfo = m_folderListModel->getSelectedDirInfo();
    if(dirInfo == nullptr) {
        // not selected.
        return;
    }

    m_folderListModel->clearSelectedDirInfo();

    CoreLibUsbControl usbControl;
    if(dirInfo->depth() == 0) {
        // top folder (Syyyymmdd-x)
        if(usbControl.deleteFolder(dirInfo->dirPath())) {
            m_folderListModel->removeDirInfo(*dirInfo);
            m_fileListModel->beginReset();
            m_fileListModel->clearFileInfoList();
            m_fileListModel->endReset();
        }
    } else {
        // sub folder
        QDir targetDir(dirInfo->dirPath());
        for(int i = 0; i < dirInfo->depth(); i++) {
            targetDir.cdUp();
        }

        if(usbControl.deleteFolder(targetDir.path())) {
            m_folderListModel->removeDirInfo(DirInfo(targetDir.dirName(), targetDir.path(), 0, false));
            m_fileListModel->beginReset();
            m_fileListModel->clearFileInfoList();
            m_fileListModel->endReset();
        }
    }
    m_folderListModel->sortDirList();
}

void SystemSettingManager::onDisplaySelectClicked()
{
    m_proxyModel->setFilterRole(m_fileListModel->HasDiffRole);
    if(m_proxyModel->filterRegExp().isEmpty()) {
        m_proxyModel->setFilterRegExp("true");
    } else {
        m_proxyModel->setFilterRegExp("");
    }
}
