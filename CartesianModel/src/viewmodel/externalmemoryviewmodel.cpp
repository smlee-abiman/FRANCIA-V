#include "externalmemoryviewmodel.h"
#include <QMutex>
#include <QTimer>

ExternalMemoryViewFileList::ExternalMemoryViewFileList(QObject *parent):QObject(parent)
{

}

void ExternalMemoryViewFileList::setFileName(QString fileName)
{
    if(m_fileName != fileName) {
        m_fileName = fileName;
        emit fileNameChanged(m_fileName);
    }
}

void ExternalMemoryViewFileList::setFileParent(QString fileParent)
{
    if(m_fileParent != fileParent) {
        m_fileParent = fileParent;
        emit fileParentChanged(m_fileParent);
    }
}

void ExternalMemoryViewFileList::setFilePath(QString filePath)
{
    if(m_filePath != filePath) {
        m_filePath = filePath;
        emit filePathChanged(m_filePath);
    }
}

void ExternalMemoryViewFileList::setFileUrlDir(bool fileUrlDir)
{
    if(m_fileUrlDir!= fileUrlDir) {
        m_fileUrlDir = fileUrlDir;
        emit fileUrlDirChanged(m_fileUrlDir);
    }
}

void ExternalMemoryViewFileList::setFileSize(qint64 fileSize)
{
    if(m_fileSize != fileSize) {
        m_fileSize = fileSize;
        emit fileSizeChanged(m_fileSize);
    }
}

void ExternalMemoryViewFileList::setFileLastModified(QDateTime fileLastModified)
{
    if(m_fileLastModified != fileLastModified) {
        m_fileLastModified = fileLastModified;
        emit fileLastModifiedChanged(m_fileLastModified);
    }
}

ExternalMemoryViewModel::ExternalMemoryViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm),
      m_commonView(commonView),
      mTimerId(0)
{
    mFtpManager = FtpManager::get_instance();
    mUsbManager = UsbManager::get_instance();
	mProgramFileManager = ProgramFileManager::get_instance();
    mTransferFileThread = TransferFileThread::get_instance();
    initSettingPenSetData();
    mMutexFtpPut = new QMutex(QMutex::Recursive);
    mMutexFtpGet = new QMutex(QMutex::Recursive);
    mMutexCursorOpe = new QMutex(QMutex::Recursive);
    if (mFtpManager) {
        int ftpTimeOut = mFtpManager->getFtpTimeOut();
        if(ftpTimeOut <= 0){
            mExtMemCopyTimeOutCnt = 0;
        } else {
            mExtMemCopyTimeOutCnt = ftpTimeOut * (1000 / EXT_MEM_COPY_TIMER_INTERVAL);        // ftpTimeOut*10(by 100msインターバル)
        }
        connect(mFtpManager, SIGNAL(ftpRemoteFileInfoChanged(bool)), this, SLOT(onFtpRemoteFileInfoChanged(bool)), Qt::DirectConnection);
    }
    qDebug("%s:%d mExtMemCopyTimeOutCnt[%d]", __func__, __LINE__, mExtMemCopyTimeOutCnt);
    if (mTransferFileThread) { mTransferFileThread->start(); }
}

ExternalMemoryViewModel::ExternalMemoryViewModel(QObject *object)
    : ViewModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

ExternalMemoryViewModel::~ExternalMemoryViewModel()
{
    if (mTransferFileThread) {
        mTransferFileThread->quit();
        mTransferFileThread->wait();
    }
    if (mFtpManager) { disconnect(mFtpManager, SIGNAL(ftpRemoteFileInfoChanged(bool)), this, SLOT(onFtpRemoteFileInfoChanged(bool))); }
    delete mMutexCursorOpe;
    mMutexCursorOpe = nullptr;
    delete mMutexFtpGet;
    mMutexFtpGet = nullptr;
    delete mMutexFtpPut;
    mMutexFtpPut = nullptr;
    stopUsbMonitoring();
    if (mGetFile){
        mGetFile->close();
        delete mGetFile;
        mGetFile = nullptr;
    }
    if (mPutFile){
        mPutFile->close();
        delete mPutFile;
        mPutFile = nullptr;
    }
    qDeleteAll(m_extFtpControllerFileList);
    qDeleteAll(m_extUsbMemoryFileList);
    qDeleteAll(m_extUsbMemoryChildDirFileList);
}

void ExternalMemoryViewModel::onActivate()
{
    startUsbMonitoring();
}

void ExternalMemoryViewModel::onDeactivate()
{
    timerStop();
    stopUsbMonitoring();
    setIsAlreadyUsbFileList(false);
    setIsAlreadyUsbChildFileList(false);
}

void ExternalMemoryViewModel::startModbusMonitoring()
{
    if(m_communication) {
        connect(m_communication, SIGNAL(modBusComConnectStatusChanged(int)),
            this, SLOT(onModBusComConnectStatusChanged(int)));
        if (m_isModbusConnect == false){
            emit isModbusConnectChanged(m_isModbusConnect);
        }
        //qDebug() << "ExternalMemoryViewModel::startModbusMonitoring m_isModbusConnect:" << m_isModbusConnect;
    }
}

void ExternalMemoryViewModel::stopModbusMonitoring()
{
    if(m_communication) {
        disconnect(m_communication, SIGNAL(modBusComConnectStatusChanged(int)),
            this, SLOT(onModBusComConnectStatusChanged(int)));
    }
}

void ExternalMemoryViewModel::setExtFtpControllerFileList(QList<QObject *> extFtpControllerFileList)
{
    m_extFtpControllerFileList = extFtpControllerFileList;
    emit extFtpControllerFileListChanged(m_extFtpControllerFileList);
}

void ExternalMemoryViewModel::setExtUsbMemoryFileList(QList<QObject *> extUsbMemoryFileList)
{
    m_extUsbMemoryFileList = extUsbMemoryFileList;
    emit extUsbMemoryFileListChanged(m_extUsbMemoryFileList);
}

void ExternalMemoryViewModel::setExtUsbMemoryChildDirFileList(QList<QObject *> extUsbMemoryChildDirFileList)
{
    m_extUsbMemoryChildDirFileList = extUsbMemoryChildDirFileList;
    emit extUsbMemoryChildDirFileListChanged(m_extUsbMemoryChildDirFileList);
}

void ExternalMemoryViewModel::initFtpControllerViewFileList()
{
    qDebug("%s:%d m_remoteDirFileCnt[%d] m_extFtpControllerFileList count[%d]", __func__, __LINE__, m_remoteDirFileCnt, m_extFtpControllerFileList.count());
    m_extFtpControllerFileList.clear();
    for(int i = 0; i < m_remoteDirFileCnt; i++){
        //qDebug() << "index:" << i << "name:" << mRemoteDirFileInfo.at(i).name << "parent:" << mRemoteDirFileInfo.at(i).parent
        //         << "lastModified:" << mRemoteDirFileInfo.at(i).lastModified << "size:" << mRemoteDirFileInfo.at(i).size
        //         << "isDir:" << mRemoteDirFileInfo.at(i).isDir << "path:" << mRemoteDirFileInfo.at(i).path;
        ExternalMemoryViewFileList *extFtpControllerFileList
            = new ExternalMemoryViewFileList();
        extFtpControllerFileList->setFileName(mRemoteDirFileInfo.at(i).name);
        extFtpControllerFileList->setFileParent(mRemoteDirFileInfo.at(i).parent);
        extFtpControllerFileList->setFilePath(mRemoteDirFileInfo.at(i).path);
        extFtpControllerFileList->setFileUrlDir(mRemoteDirFileInfo.at(i).isDir);
        extFtpControllerFileList->setFileSize(mRemoteDirFileInfo.at(i).size);
        extFtpControllerFileList->setFileLastModified(mRemoteDirFileInfo.at(i).lastModified);
        m_extFtpControllerFileList.append(extFtpControllerFileList);
    }
    setExtFtpControllerFileList(m_extFtpControllerFileList);
}

void ExternalMemoryViewModel::initUsbDirFileList()
{
    qDebug("%s:%d m_usbDirFileCnt[%d] m_extUsbMemoryFileList count[%d]", __func__, __LINE__, m_usbDirFileCnt, m_extUsbMemoryFileList.count());
    m_extUsbMemoryFileList.clear();
    for(int i = 0; i < m_usbDirFileCnt; i++){
        ExternalMemoryViewFileList *extUsbMemoryFileList
            = new ExternalMemoryViewFileList();
        extUsbMemoryFileList->setFileName(mUsbDirFileInfo.at(i).name);
        extUsbMemoryFileList->setFileParent("");
        extUsbMemoryFileList->setFilePath(mUsbDirFileInfo.at(i).path);
        extUsbMemoryFileList->setFileUrlDir(mUsbDirFileInfo.at(i).isDir);
        extUsbMemoryFileList->setFileSize(mUsbDirFileInfo.at(i).size);
        extUsbMemoryFileList->setFileLastModified(mUsbDirFileInfo.at(i).lastModified);
        m_extUsbMemoryFileList.append(extUsbMemoryFileList);
    }
    setExtUsbMemoryFileList(m_extUsbMemoryFileList);
    setIsAlreadyUsbFileList(true);
}

void ExternalMemoryViewModel::initUsbChildDirFileList(int usbChildDirfileCnt)
{
    qDebug("%s:%d usbChildDirfileCnt[%d]", __func__, __LINE__, usbChildDirfileCnt);
    m_extUsbMemoryChildDirFileList.clear();
    QString parentDir = "";
    for(int i = 0; i < usbChildDirfileCnt; i++){
        ExternalMemoryViewFileList *extUsbMemoryChildDirFileList
            = new ExternalMemoryViewFileList();
        extUsbMemoryChildDirFileList->setFileName(mUsbChildDirFileInfo.at(i).name);
        extUsbMemoryChildDirFileList->setFileParent(mUsbChildDirFileInfo.at(i).parent);
        extUsbMemoryChildDirFileList->setFilePath(mUsbChildDirFileInfo.at(i).path);
        extUsbMemoryChildDirFileList->setFileUrlDir(mUsbChildDirFileInfo.at(i).isDir);
        extUsbMemoryChildDirFileList->setFileSize(mUsbChildDirFileInfo.at(i).size);
        extUsbMemoryChildDirFileList->setFileLastModified(mUsbChildDirFileInfo.at(i).lastModified);
        m_extUsbMemoryChildDirFileList.append(extUsbMemoryChildDirFileList);
        if(i == 0) { parentDir = mUsbChildDirFileInfo.at(i).parent; }
    }
    int childDirFileCnt = m_extUsbMemoryChildDirFileList.count();
    qDebug() << "ExternalMemoryViewModel::initUsbChildDirFileList parentDir:" << parentDir << "childDirFileCnt:" << childDirFileCnt;
    if (parentDir == "NCP" && childDirFileCnt > EXT_MEM_NCP_FILE_MAX){
        std::sort(m_extUsbMemoryChildDirFileList.begin(), m_extUsbMemoryChildDirFileList.end(), [] (QObject *left, QObject *right) {
            auto *l = static_cast<ExternalMemoryViewFileList *>(left);
            auto *r = static_cast<ExternalMemoryViewFileList *>(right);
            if(l->fileLastModified() == r->fileLastModified()) {                    // ファイル更新時間が同じ場合
                return l->fileName() > r->fileName();                               // ファイル名順で並び替える
            } else {
                return l->fileLastModified() > r->fileLastModified();               // ファイル更新時間で並び替える
            }
        });
        for(int j = 0; j < (childDirFileCnt - EXT_MEM_NCP_FILE_MAX); j++){
            m_extUsbMemoryChildDirFileList.removeLast();
            usbChildDirfileCnt--;
        }
        //qDebug() << "m_extUsbMemoryChildDirFileList count:" << m_extUsbMemoryChildDirFileList.count() << "usbChildDirfileCnt:" << usbChildDirfileCnt;
    } else {
        std::sort(m_extUsbMemoryChildDirFileList.begin(), m_extUsbMemoryChildDirFileList.end(), [] (QObject *left, QObject *right) {
            auto *l = static_cast<ExternalMemoryViewFileList *>(left);
            auto *r = static_cast<ExternalMemoryViewFileList *>(right);
            return r->fileName() > l->fileName();                                   // ファイル名順で並び替える
        });
    }
    setIsAlreadyUsbChildFileList(false);
    setUsbChildDirFileCnt(usbChildDirfileCnt);
    setExtUsbMemoryChildDirFileList(m_extUsbMemoryChildDirFileList);
    setIsAlreadyUsbChildFileList(true);
}

void ExternalMemoryViewModel::clearUsbChildDirFileList(int usbChildDirfileCnt)
{
    m_extUsbMemoryChildDirFileList.clear();
    for(int i = 0; i < 7; i++){
        ExternalMemoryViewFileList *extUsbMemoryChildDirFileList
            = new ExternalMemoryViewFileList();
        extUsbMemoryChildDirFileList->setFileName("");
        m_extUsbMemoryChildDirFileList.append(extUsbMemoryChildDirFileList);
    }
    setUsbChildDirFileCnt(usbChildDirfileCnt);
    setExtUsbMemoryChildDirFileList(m_extUsbMemoryChildDirFileList);
}

void ExternalMemoryViewModel::clearFtpControllerViewFileList(int ftpCtrlDirfileCnt)
{
    m_extFtpControllerFileList.clear();
    for(int i = 0; i < 7; i++){
        ExternalMemoryViewFileList *extFtpControllerFileList
            = new ExternalMemoryViewFileList();
        extFtpControllerFileList->setFileName("");
        m_extFtpControllerFileList.append(extFtpControllerFileList);
    }
    setRemoteDirFileCnt(ftpCtrlDirfileCnt);
    setExtFtpControllerFileList(m_extFtpControllerFileList);
}

void ExternalMemoryViewModel::initSettingPenSetData()
{
    m_extMemFileFolderListDispRGB           = m_resource->dataColor(26, 21);
    m_extMemSelectedFileFolderListDispRGB   = m_resource->dataColor(26, 22);
    m_extMemCommandPressRGB                 = m_resource->dataColor(26, 23);
#if 0
    qDebug() << "m_extMemFileFolderListDispRGB:" << m_extMemFileFolderListDispRGB;
    qDebug() << "m_extMemSelectedFileFolderListDispRGB:" << m_extMemSelectedFileFolderListDispRGB;
    qDebug() << "m_extMemCommandPressRGB:" << m_extMemCommandPressRGB;
#endif
}

void ExternalMemoryViewModel::startFtpCommunication()
{
    if (!mFtpManager) {
        qCritical("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    mFtpManager->startFtpCommunication();
}

void ExternalMemoryViewModel::stopFtpCommunication()
{
    if (!mFtpManager) {
        qCritical("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    mFtpManager->stopFtpCommunication();
}

void ExternalMemoryViewModel::startUsbMonitoring()
{
    if (!mUsbManager){
        qCritical("%s:%d mUsbManager is invalid", __func__, __LINE__);
        return;
    }
    bool usbMountStatus = mUsbManager->getCurUsbMountStatus();
    //qDebug("%s:%d usbMountStatus[%d] m_curTabIndex[%d] m_firstUsbConnected[%d]", __func__, __LINE__, usbMountStatus, m_curTabIndex, m_firstUsbConnected);
    if (m_curTabIndex == 1){    // USBに書き込み画面
        if (usbMountStatus){
            setFirstUsbConnected(usbMountStatus);
        } else {
            setIsUSBConnected(usbMountStatus);
        }
    } else {
        setIsUSBConnected(usbMountStatus);
    }
    if (!mIsUsbMonitor){
#if 1
        connect(mUsbManager, SIGNAL(usbMounted(QString)), this, SLOT(onUsbMounted(QString)));
        connect(mUsbManager, SIGNAL(usbUnmounted()), this, SLOT(onUsbUnmounted()));
        mUsbManager->timerStart(100);
        mIsUsbMonitor = true;
#else
        connect(mUsbManager, SIGNAL(usbMounted(QString)), this, SLOT(onUsbMounted(QString)));
        connect(mUsbManager, SIGNAL(usbUnmounted()), this, SLOT(onUsbUnmounted()));
        mUsbManager->start();
        mIsUsbMonitor = true;
#endif
    }
}

void ExternalMemoryViewModel::stopUsbMonitoring()
{
    if (mIsUsbMonitor){
#if 1
        mUsbManager->timerStop();
#else
        mUsbManager->quit();
        mUsbManager->wait();
#endif
        disconnect(mUsbManager, SIGNAL(usbMounted(QString)), this, SLOT(onUsbMounted(QString)));
        disconnect(mUsbManager, SIGNAL(usbUnmounted()), this, SLOT(onUsbUnmounted()));
        mIsUsbMonitor = false;
    }
}

QString ExternalMemoryViewModel::getUsbMountPath() const
{
    if (!mUsbManager) {
        return "";
    }
    return mUsbManager->mountPath();
}

void ExternalMemoryViewModel::onUsbMounted(QString usbMountPath)
{
    (void)usbMountPath;
    mUsbRootPath = getUsbMountPath() + "/";
    setIsUSBConnected(true);
}

void ExternalMemoryViewModel::onUsbUnmounted()
{
    setIsUSBConnected(false);
}

void ExternalMemoryViewModel::getFtpFileList()
{
    if (!mFtpManager) {
        qCritical("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    mFtpManager->requestFtpListCommand("/");
}

int ExternalMemoryViewModel::getFtpRemoteFileListCount()
{
    int result = 0;
    if (!mFtpManager){
        return -1;
    }
    mRemoteDirFileInfo = mFtpManager->getRemoteFileInfo();
    result = mRemoteDirFileInfo.count();
    if (result <= 0){
        qDebug("%s:%d mRemoteDirFileInfo count[%d] is nothing", __func__, __LINE__, mRemoteDirFileInfo.count());
        return result;
    }
    setRemoteDirFileCnt(result);
    if (result > 0){
        initFtpControllerViewFileList();
    }
    return result;
}

int ExternalMemoryViewModel::getUsbDirFileListCount()
{
    int fileDirCnt = 0;
    if (!mUsbManager){
        return -1;
    }
    fileDirCnt = mUsbDirFileInfo.count();
    setUsbDirFileCnt(fileDirCnt);
    if (fileDirCnt > 0){
        initUsbDirFileList();
    }
    qDebug("%s:%d m_usbDirFileCnt[%d] fileDirCnt[%d]", __func__, __LINE__, m_usbDirFileCnt, fileDirCnt);
    return fileDirCnt;
}

int ExternalMemoryViewModel::writeToUsb(int listIndex, int subListIndex)
{
    int result = 0;
    (void)subListIndex;
    mUsbRootPath = getUsbMountPath() + "/";
    QString dstDirPath = mUsbRootPath;
    QString srcDirPath = "/";
    ExternalMemoryViewFileList *extFtpControllerFileList;
    bool isDir = false;
    qint64 fileSize = 0;
    qDebug("%s:%d listIndex[%d] subListIndex[%d] mCurUsbFolder count[%d]", __func__, __LINE__, listIndex, subListIndex, mCurUsbFolder.length());
    if (m_isUsbWriteProtected == true || mCurUsbFolder.length() == 0){
        qCritical("%s:%d Cannot write USB [%d] mCurUsbFolder count[%d]", __func__, __LINE__, m_isUsbWriteProtected, mCurUsbFolder.length());
        return -1;
    }
    dstDirPath = dstDirPath + mCurUsbFolder + "/";
    qDebug() << "ExternalMemoryViewModel::writeToUsb dstDirPath:" << dstDirPath << "mCurUsbFolder:" << mCurUsbFolder;
    lockFtpGetMutex();
    //NOOPコマンド抑止
    mFtpManager->setIsDeterNoopCmd(true);
    if (m_extFtpControllerFileList.count() <= 0 || listIndex >= m_extFtpControllerFileList.count()){
        qCritical("%s:%d m_externalMemoryViewFileList count[%d] is invalid or listIndex[%d] is invalid",
            __func__, __LINE__, m_extFtpControllerFileList.count(), listIndex);
        result = -1;
        goto process_error;
    }
    extFtpControllerFileList = static_cast<ExternalMemoryViewFileList *>(m_extFtpControllerFileList.at(listIndex));
    isDir = extFtpControllerFileList->fileUrlDir();
    fileSize = extFtpControllerFileList->fileSize();
    if (isDir){
        // TODO
        qCritical("%s:%d isDir[%d] is invalid", __func__, __LINE__, isDir);
        goto process_error;
    } else {
        result = copyToUsb(
                    extFtpControllerFileList->fileName(),
                    dstDirPath, srcDirPath, listIndex, fileSize);
        if (result < 0){
            qCritical("%s:%d copyToUsb result[%d]", __func__, __LINE__, result);
            goto process_error;
        }
    }
    return result;
process_error:
    //m_processingResult = false;
    //emit processingResultChanged(m_processingResult);
    mFtpManager->setIsDeterNoopCmd(false);
    unlockFtpGetMutex();
    return result;
}

int ExternalMemoryViewModel::writeFromUsb(int listIndex, int subListIndex)
{
    int result = 0;
    QString dstDirPath = "/";
    mUsbRootPath = getUsbMountPath() + "/";
    QString srcDirPath = mUsbRootPath;
    ExternalMemoryViewFileList *extUsbMemoryFileList = nullptr;
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList = nullptr;
    bool isDir = false;

    qDebug("%s:%d listIndex[%d] subListIndex[%d]", __func__, __LINE__, listIndex, subListIndex);
    lockFtpPutMutex();
    mFtpManager->setIsDeterNoopCmd(true);
    if (m_extUsbMemoryFileList.count() <= 0 || listIndex >= m_extUsbMemoryFileList.count()){
        qCritical("%s:%d m_extUsbMemoryFileList count[%d] is invalid or listIndex[%d] is invalid",
            __func__, __LINE__, m_extUsbMemoryFileList.count(), listIndex);
        result = -1;
        goto process_error;
    }
    mIsPowerRestart = false;
    mTargetDirName = "";
    extUsbMemoryFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryFileList.at(listIndex));
    isDir = extUsbMemoryFileList->fileUrlDir();
    if (isDir){
        if (subListIndex < 0){
            QString targetDirName = extUsbMemoryFileList->fileName();
            writeInBulkFromUsb(targetDirName);
        } else {
            if (m_extUsbMemoryChildDirFileList.count() <= 0 || subListIndex >= m_extUsbMemoryChildDirFileList.count()){
                qCritical("%s:%d m_extUsbMemoryChildDirFileList count[%d] is invalid or subListIndex[%d] is invalid",
                    __func__, __LINE__, m_extUsbMemoryChildDirFileList.count(), subListIndex);
                result = -1;
                goto process_error;
            }
            extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(subListIndex));
            bool isChildDir = extUsbMemoryChildDirFileList->fileUrlDir();
            if (isChildDir){
                // TODO
                qCritical("%s:%d isChildDir[%d] is invalid", __func__, __LINE__, isChildDir);
                result = -1;
                goto process_error;
            } else {
                result = writeIndividualFromUsb(extUsbMemoryFileList, extUsbMemoryChildDirFileList, subListIndex, true);
                if (result < 0){
                    goto process_error;
                } else {
                    goto process_success;
                }
            }
        }
    } else {
        result = writeIndividualFromUsb(extUsbMemoryFileList, extUsbMemoryChildDirFileList, listIndex);
        if (result < 0){
            goto process_error;
        } else {
            goto process_success;
        }
    }
    return result;
process_error:
    //m_processingResult = false;
    //emit processingResultChanged(m_processingResult);
    mIsPowerRestart = false;
    mWriteTargetFromUsb = -1;
    mFtpManager->setIsDeterNoopCmd(false);
    unlockFtpPutMutex();
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    return result;
process_success:
    qDebug("%s:%d process_success result[%d]", __func__, __LINE__, result);
    return result;
}

void ExternalMemoryViewModel::getUsbDirFileList()
{
    if (!mUsbManager){
        return;
    }
    QFileInfoList fileInfo;
    mUsbMountPath = getUsbMountPath();
    qDebug() << "getUsbDirFileList mUsbMountPath:" << mUsbMountPath << "mUsbMountPath length:" << mUsbMountPath.length();
    if (mUsbMountPath.length() == 0){
        mUsbDirFileInfo.clear();
        setIsAlreadyUsbFileList(false);
    } else {
        fileInfo = mUsbManager->getFolderFileList(mUsbMountPath, QDir::DirsFirst, true);
        qDebug("%s:%d fileInfo count[%d]", __func__, __LINE__, fileInfo.count());
        mUsbDirFileInfo.clear();
        for(int i = 0; i < fileInfo.count(); i++){
            //qDebug() << "fileName:" << fileInfo.at(i).fileName() << "isDir:" << fileInfo.at(i).isDir()
            //         << "path:" << fileInfo.at(i).filePath() << "isDir:" << fileInfo.at(i).isDir()
            //         << "lastModified:" << fileInfo.at(i).lastModified() << "size:" << fileInfo.at(i).size();
            stRemoteDirFileInfo dirFileInfo;
            dirFileInfo.name = fileInfo.at(i).fileName();
            dirFileInfo.parent = "";
            dirFileInfo.path = fileInfo.at(i).filePath();
            dirFileInfo.isDir = fileInfo.at(i).isDir();
            dirFileInfo.lastModified = fileInfo.at(i).lastModified();
            dirFileInfo.size = fileInfo.at(i).size();
            bool isDisp = false;
            if (dirFileInfo.isDir){
                isDisp = checkDispTargetDir(dirFileInfo.name);
            } else {
                isDisp = checkDispTargetFile(dirFileInfo.name);
            }
            if (isDisp) { mUsbDirFileInfo.append(dirFileInfo); }
        }
        qDebug("%s:%d fileInfo count[%d] mUsbDirFileInfo count[%d]", __func__, __LINE__, fileInfo.count(), mUsbDirFileInfo.count());
    }
}

void ExternalMemoryViewModel::getFtpChildDirFileList(int childDirIndex, int listIndex)
{
    (void)childDirIndex;
    (void)listIndex;
}

void ExternalMemoryViewModel::getUsbChildDirFileList(int childDirIndex, int listIndex)
{
    qDebug("%s:%d childDirIndex[%d] listIndex[%d]", __func__, __LINE__, childDirIndex, listIndex);
    mUsbChildDirFileInfo.clear();
    QDir::Filters filters = QDir::Filter::Files | QDir::Filter::NoDotAndDotDot;
    QStringList nameFilters;
    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
    if (childDirIndex == -1){       // file select
        int usbChildDirfileCnt = mUsbChildDirFileInfo.count();
        setUsbChildDirFileCnt(usbChildDirfileCnt);
        setIsAlreadyUsbChildFileList(false);
    } else if(childDirIndex == 0){ // top dir
        if (listIndex < 0 || listIndex >= mUsbDirFileInfo.count()){
            qCritical("%s:%d listIndex[%d] is invalid", __func__, __LINE__, listIndex);
            return;
        }
        if (!mUsbDirFileInfo.at(listIndex).isDir){
            qCritical("%s:%d targe listIndex[%d]is not dir[%d]", __func__, __LINE__, listIndex, mUsbDirFileInfo.at(listIndex).isDir);
            return;
        }
        QDirIterator childIt(mUsbDirFileInfo.at(listIndex).path, nameFilters, filters, flags);
        while(childIt.hasNext()) {
            QFileInfo fileInfo(childIt.next());
            stRemoteDirFileInfo localFileInfo;
            localFileInfo.name = fileInfo.fileName();
            localFileInfo.path = fileInfo.filePath();
            localFileInfo.size = fileInfo.size();
            localFileInfo.isDir = fileInfo.isDir();
            localFileInfo.parent = mUsbDirFileInfo.at(listIndex).name;
            localFileInfo.lastModified = fileInfo.lastModified();
            //qDebug() << "getUsbChildDirFileList name:" << localFileInfo.name << "parent:" << localFileInfo.parent
            //        << "lastModified:" << localFileInfo.lastModified << "size:" << localFileInfo.size
            //        << "isDir:" << localFileInfo.isDir << "path:" << localFileInfo.path;
            bool isDisp = checkDispTarget(localFileInfo.parent, localFileInfo.name);
            if (isDisp) { mUsbChildDirFileInfo << localFileInfo; }
            else { continue; }
        }
        int usbChildDirfileCnt = mUsbChildDirFileInfo.count();
        if (usbChildDirfileCnt > 0){
            initUsbChildDirFileList(usbChildDirfileCnt);
        } else {
            clearUsbChildDirFileList(0);
        }
    } else {
        // TODO
    }
}

void ExternalMemoryViewModel::getChildDirFileList(int mode, int childDirIndex, int listIndex)
{
    lockCursorOpeMutex();
    if (mode == 0){
        getFtpChildDirFileList(childDirIndex, listIndex);
    } else {
        getUsbChildDirFileList(childDirIndex, listIndex);
    }
    unlockCursorOpeMutex();
}

bool ExternalMemoryViewModel::checkDispFilePenUpdate(QString targetFileName)
{
    bool isFindTargetFile = false;
    for(int i = 0; i < WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    if (isFindTargetFile) { goto find_target_file; }
    for(int i = 0; i < WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    if (isFindTargetFile) { goto find_target_file; }
    for(int i = 0; i < WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    if (isFindTargetFile) { goto find_target_file; }
    for(int i = 0; i < WRITE_FROM_USB_PEN_SET_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_PEN_SET_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    if (isFindTargetFile) { goto find_target_file; }
    for(int i = 0; i < WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    if (isFindTargetFile) { goto find_target_file; }
    for(int i = 0; i < WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    if (isFindTargetFile) { goto find_target_file; }
    return isFindTargetFile;
find_target_file:
    return isFindTargetFile;
}

bool ExternalMemoryViewModel::checkDispFileContUpdate(QString targetFileName)
{
    bool isFindTargetFile = false;
    QString frontsWith = "";
    QString machineIndexStr = "";
    bool is_ok;
    int machineIndex = -1;
    for(int i = 0; i < FTP_PUT_FIXED_CONTROLLER_UPDATE_LIST.count(); i++){
        if(targetFileName == FTP_PUT_FIXED_CONTROLLER_UPDATE_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    if (isFindTargetFile) { goto find_target_file; }
    frontsWith = targetFileName.mid(0, 7);
    machineIndexStr = targetFileName.mid(7, 2);
    machineIndex = machineIndexStr.toInt(&is_ok, 10);
    if (!is_ok) {
        qDebug() << "machineIndexStr is not num :" << machineIndexStr;
        machineIndex = -1;
    }
    qDebug() << "ExternalMemoryViewModel::checkDispFileContUpdate targetFileName length:" << targetFileName.length()
             << "frontsWith:" << frontsWith << "endsWith:" << targetFileName.endsWith(".NCP") << "machineIndexStr:" << machineIndexStr << "machineIndex:" << machineIndex;
    if (targetFileName.length() == 13 && (frontsWith == "machine") && targetFileName.endsWith(".dat") && (machineIndex >= 1 && machineIndex <= 32)){
        isFindTargetFile = true;
    }
    if (isFindTargetFile) { goto find_target_file; }
    qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
find_target_file:
    qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
}

bool ExternalMemoryViewModel::checkDispFileNcp(QString targetFileName)
{
    bool isFindTargetFile = false;
    QString frontsWith = targetFileName.mid(0, 3);
    QString ncpIndexStr = targetFileName.mid(3, 2);
#if 1
    qDebug() << "ExternalMemoryViewModel::checkDispFileNcp targetFileName:" << targetFileName;
    if (targetFileName.endsWith(".NCP")){
        isFindTargetFile = true;
    }
#else
    bool is_ok;
    int ncpIndex = ncpIndexStr.toInt(&is_ok, 10);
    if (!is_ok) {
        //qDebug() << "ncpIndexStr is not num :" << ncpIndexStr;
        ncpIndex = -1;
    }
    //qDebug() << "ExternalMemoryViewModel::checkDispFileNcp targetFileName length:" << targetFileName.length()
    //    << "frontsWith:" << frontsWith << "endsWith:" << targetFileName.endsWith(".NCP") << "ncpIndexStr:" << ncpIndexStr << "ncpIndex:" << ncpIndex;
    if (targetFileName.length() == 9 && (frontsWith == "NCP") && targetFileName.endsWith(".NCP") && (ncpIndex >= 1 && ncpIndex <= 99)){
        isFindTargetFile = true;
    }
#endif
    if (isFindTargetFile) { goto find_target_file; }
    qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
find_target_file:
    qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
}

bool ExternalMemoryViewModel::checkDispFilePenMsg(QString targetFileName)
{
    bool isFindTargetFile = false;
    qDebug() << "ExternalMemoryViewModel::checkDispFilePenMsg targetFileName:" << targetFileName;
    for(int i = 0; i < WRITE_FROM_USB_PEN_MSG_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_PEN_MSG_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    if (isFindTargetFile) { goto find_target_file; }
    qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
find_target_file:
    qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
}

bool ExternalMemoryViewModel::checkDispTarget(QString parentDirName, QString targetFileName)
{
    bool isFindTargetDir = false;
    bool isFindTargetFile = false;
    int i = 0;
    qDebug() << "ExternalMemoryViewModel::checkDispTarget parentDirName:" << parentDirName << "targetFileName:" << targetFileName;
    for(i = 0; i < WRITE_FROM_USB_TARGET_FOLDER_LIST.count(); i++){
        if(parentDirName == WRITE_FROM_USB_TARGET_FOLDER_LIST.at(i)){
            isFindTargetDir = true;
            break;
        }
    }
    if (isFindTargetDir) {
        switch(i){
        case (int)Enums::UsbDownloadType::PenUpdate:
            isFindTargetFile = checkDispFilePenUpdate(targetFileName);
            break;
        case (int)Enums::UsbDownloadType::ControllerUpdate:{
            isFindTargetFile = checkDispFileContUpdate(targetFileName);
            break;
        }
        case (int)Enums::UsbDownloadType::NcpData:{
            isFindTargetFile = checkDispFileNcp(targetFileName);
            break;
        }
        case (int)Enums::UsbDownloadType::PenMsgData:
            isFindTargetFile = checkDispFilePenMsg(targetFileName);
            break;
        default:
            break;
        }
    } else {
        if (i >= WRITE_FROM_USB_TARGET_FOLDER_LIST.count()
            && parentDirName.length() == 14 && parentDirName.contains("backup")){
            for(int i = 0; i < FTP_PUT_FIXED_DAILY_BACKUP_LIST.count(); i++){
                if(targetFileName == FTP_PUT_FIXED_DAILY_BACKUP_LIST.at(i)){
                    isFindTargetFile = true;
                    break;
                }
            }
            if (isFindTargetFile) { goto find_target_file; }
            for(int i = 0; i < FTP_READ_ONLY_BACKUP_LIST.count(); i++){
                if(targetFileName == FTP_READ_ONLY_BACKUP_LIST.at(i)){
                    isFindTargetFile = true;
                    break;
                }
            }
            if (isFindTargetFile) { goto find_target_file; }
            QString frontsWith = "";
            bool is_ok;
            frontsWith = targetFileName.mid(0, 4);
            QString condIndexStr = targetFileName.mid(4, 4);
            int condIndex = condIndexStr.toInt(&is_ok, 10);
            if (!is_ok) {
                //qDebug() << "condIndexStr is not num :" << condIndexStr;
                condIndex = -1;
            }
            //qDebug() << "ExternalMemoryViewModel::checkDispTarget COND targetFileName length:" << targetFileName.length()
            //    << "frontsWith:" << frontsWith << "endsWith:" << targetFileName.endsWith(".NCP") << "condIndexStr:" << condIndexStr << "condIndex:" << condIndex;
            if (targetFileName.length() == 12 && (frontsWith == "COND") && targetFileName.endsWith(".CND") && (condIndex >= 1 && condIndex <= 999)){
                isFindTargetFile = true;
            }
            if (isFindTargetFile) { goto find_target_file; }
            frontsWith = targetFileName.mid(0, 7);
            QString machineIndexStr = targetFileName.mid(7, 2);
            int machineIndex = machineIndexStr.toInt(&is_ok, 10);
            if (!is_ok) {
                //qDebug() << "machineIndexStr is not num :" << machineIndexStr;
                machineIndex = -1;
            }
            //qDebug() << "ExternalMemoryViewModel::checkDispTarget machine targetFileName length:" << targetFileName.length()
            //    << "frontsWith:" << frontsWith << "endsWith:" << targetFileName.endsWith(".NCP") << "machineIndexStr:" << machineIndexStr << "machineIndex:" << machineIndex;
            if (targetFileName.length() == 13 && (frontsWith == "machine") && targetFileName.endsWith(".dat") && (machineIndex >= 1 && machineIndex <= 32)){
                isFindTargetFile = true;
            }
            if (isFindTargetFile) { goto find_target_file; }
        } else {
            return false;
        }
    }
    qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
find_target_file:
    qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
}

bool ExternalMemoryViewModel::checkDispTargetDir(QString targetDirName)
{
    bool isFindTargetDir = false;
    int i = 0;
    //qDebug() << "ExternalMemoryViewModel::checkDispTargetDir targetDirName:" << targetDirName;
    for(i = 0; i < WRITE_FROM_USB_TARGET_FOLDER_LIST.count(); i++){
        if(targetDirName == WRITE_FROM_USB_TARGET_FOLDER_LIST.at(i)){
            isFindTargetDir = true;
            break;
        }
    }
    if (isFindTargetDir) {
        goto find_target_dir;
    } else {
        if (i >= WRITE_FROM_USB_TARGET_FOLDER_LIST.count()
            && targetDirName.length() == 14 && targetDirName.contains("backup")){
            isFindTargetDir = true;
            goto find_target_dir;
        }
    }
    //qDebug("%s:%d isFindTargetDir[%d]", __func__, __LINE__, isFindTargetDir);
    return isFindTargetDir;
find_target_dir:
    //qDebug("%s:%d isFindTargetDir[%d]", __func__, __LINE__, isFindTargetDir);
    return isFindTargetDir;
}

bool ExternalMemoryViewModel::checkDispTargetFile(QString targetFileName)
{
    bool isFindTargetFile = false;
    int i = 0;
    //qDebug() << "ExternalMemoryViewModel::checkDispTargetFile targetFileName:" << targetFileName;
    for(i = 0; i < WRITE_FROM_USB_TARGET_FILE_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_TARGET_FILE_LIST.at(i)){
            isFindTargetFile = true;
            break;
        }
    }
    //qDebug("%s:%d isFindTargetFile[%d]", __func__, __LINE__, isFindTargetFile);
    return isFindTargetFile;
}

bool ExternalMemoryViewModel::checkReadOnlyFile(QString targetFileName)
{
    qDebug() << "ExternalMemoryViewModel::checkReadOnlyFile targetFileName:" << targetFileName;
    bool isReadOnly = false;
    for(int i = 0; i < FTP_READ_ONLY_BACKUP_LIST.count(); i++){
        if(targetFileName == FTP_READ_ONLY_BACKUP_LIST.at(i)){
            isReadOnly = true;
            break;
        }
    }
    if (!isReadOnly){
        for(int i = 0; i < FTP_FOLDER_BULK_TRANSFER_NOT_APPLICABLE_LIST.count(); i++){
            if(targetFileName == FTP_FOLDER_BULK_TRANSFER_NOT_APPLICABLE_LIST.at(i)){
                isReadOnly = true;
                break;
            }
        }
    }
    if (!isReadOnly){
        if (targetFileName.length() == 14 && targetFileName.contains("backup")){
            isReadOnly = true;
        }
    }
    qDebug("%s:%d isReadOnly[%d]", __func__, __LINE__, isReadOnly);
    return isReadOnly;
}

bool ExternalMemoryViewModel::checkCondFile(QString curFileName)
{
    bool isCondFile = false;
    qDebug() << "ExternalMemoryViewModel::checkCondFile curFileName:" << curFileName;
    if (curFileName == ""){
        qCritical() << "curFileName is invalid:"<< curFileName;
        return false;
    }
    if (curFileName.length() == 12){
        QString frontsWith = curFileName.mid(0, 4);
        if (frontsWith == "COND" && curFileName.endsWith(".CND")){
            isCondFile = true;
        } else {
            isCondFile = false;
        }
    } else {
        isCondFile = false;
    }
    qDebug("%s:%d isCondFile[%d]", __func__, __LINE__, isCondFile);
    return isCondFile;
}

int ExternalMemoryViewModel::createBackupFolder()
{
    if (!m_commonView || !mUsbManager) return -1;
    int result = 0;
    QString dstDirPath = getUsbMountPath();
    QString folderName = "backup";
    QString curDateTime = m_commonView->curDate();
    QString adjDateTime = curDateTime.mid(2, 2) + curDateTime.mid(5, 2) + curDateTime.mid(8, 2);
    folderName = folderName + adjDateTime;
    qDebug() << "ExternalMemoryViewModel::createBackupFolder folderName:" << folderName
             << "curDateTime:" << curDateTime << "dstDirPath:" << dstDirPath << "adjDateTime:" << adjDateTime;
    QFileInfoList fileInfo = mUsbManager->getFolderList(dstDirPath);
    QString targetFileNum = decideFileNum(fileInfo, adjDateTime);
    if (targetFileNum == ""){
        m_isUsbWriteProtected = true;
        emit isUsbWriteProtectedChanged(m_isUsbWriteProtected);
        mCurUsbFolder = "";
        result = -1;
    } else {
        m_isUsbWriteProtected = false;
        emit isUsbWriteProtectedChanged(m_isUsbWriteProtected);
        folderName = folderName + targetFileNum;
        mUsbManager->createFolder(dstDirPath + "/" + folderName);
        mCurUsbFolder = folderName;
    }
    qDebug() << "ExternalMemoryViewModel::createBackupFolder mCurUsbFolder:" << mCurUsbFolder;
    return result;
}

QString ExternalMemoryViewModel::decideFileNum(QFileInfoList &fileInfo, QString curDate)
{
    QString ret = "";
    QList<QString> backupFolderList;
    QList<QString> backupFolderDateList;
    QList<QString> backupFolderNumList;
    backupFolderList.clear();
    backupFolderDateList.clear();
    backupFolderNumList.clear();
    for(int i = 0; i < fileInfo.count(); i++){
        QString prefix = (fileInfo.at(i).fileName()).mid(0, 6);
        QString date = (fileInfo.at(i).fileName()).mid(6, 6);
        QString num = (fileInfo.at(i).fileName()).mid(12, 2);
        //qDebug() << "ExternalMemoryViewModel::decideFileNum prefix:" << prefix << "date:" << date << "num:" << num;
        if((fileInfo.at(i).fileName()).length() == 14 && prefix == "backup"){
            backupFolderList.append(fileInfo.at(i).fileName());
            if (date == curDate){
                backupFolderNumList.append(num);
            }
        }
    }
    if (backupFolderNumList.count() >= EXT_MEM_BACKUP_MAX_NUM){
        qCritical() << "File number exceeds maximum:" << backupFolderNumList.count();
        return ret;
    }
    int curFileNumber = 0;
    int nextFileNumber = 0;
    int startNumber = 1;
    int freeNumber = 1;
    bool is_ok = false;

    std::sort(backupFolderNumList.begin(), backupFolderNumList.end());
    for (int i = 0; i < backupFolderNumList.count(); i++){
        //qDebug() << "backupFolderNumList num:" << backupFolderNumList.at(i);
        curFileNumber = backupFolderNumList.at(i).toInt(&is_ok, 10);
        if (!is_ok){
            qDebug() << "ExternalMemoryViewModel::decideFileNum not is num:" << backupFolderNumList.at(i);
            continue;
        }
        if (startNumber != curFileNumber){
            freeNumber = startNumber;
            break;
        } else {
            if (i < backupFolderNumList.count() - 1){
                nextFileNumber = backupFolderNumList.at(i + 1).toInt(&is_ok, 10);
                if (!is_ok){
                    qDebug() << "not is num:" << backupFolderNumList.at(i + 1);
                    continue;
                }
                if (startNumber + 1 == nextFileNumber){
                    startNumber++;
                    continue;
                } else {
                    freeNumber = startNumber + 1;
                    break;
                }
            } else {
                freeNumber = curFileNumber + 1;
            }
        }
    }
    ret = (freeNumber < 10 ? "0" + QString::number(freeNumber, 10) : \
            QString::number(freeNumber, 10));
    qDebug() << "ExternalMemoryViewModel::decideFileNum freeNumber:" << freeNumber << "ret:" << ret;
    return ret;
}

int ExternalMemoryViewModel::writeInBulkToUsb()
{
    if (mCurUsbFolder.length() == 0 || m_remoteDirFileCnt <= 0){
        qCritical("%s:%d mCurUsbFolder length[%d] remote fileCnt[%d]", __func__, __LINE__, mCurUsbFolder.length(), m_remoteDirFileCnt);
        return -1;
    }
    mUsbRootPath = getUsbMountPath() + "/";
    QString dstDirPath = mUsbRootPath + mCurUsbFolder + "/";
    QString srcDirPath = "/";
    qDebug() << "ExternalMemoryViewModel::writeInBulkToUsb dstDirPath:" << dstDirPath << "m_remoteDirFileCnt:" << m_remoteDirFileCnt;
    lockFtpGetMutex();
    mFtpManager->setIsDeterNoopCmd(true);
    mBackupDataCnt = m_remoteDirFileCnt;
    QString copyFileName = mRemoteDirFileInfo.at(mBackupDataCnt - 1).name;
    qint64 fileSize = mRemoteDirFileInfo.at(mBackupDataCnt - 1).size;
    mGetFile = new QFile(dstDirPath + copyFileName);
#if 1
    if(mGetFile->exists()) {
        // target already exists in USB
        mGetFile->remove();
    }
#endif
    mGetFile->open(QFile::WriteOnly);
    int result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandCWD, nullptr, srcDirPath);
    result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandRETR, mGetFile, copyFileName);        // Ftp get
    stFtpGetArgument arg;
    arg.getFile = mGetFile;
    arg.selectedIndex = mBackupDataCnt - 1;
    arg.isToExtMem = true;
    mFtpManager->setFtpGetArgment(result, arg);
    mUsbDirection = (int)CopyDirectionStatus::CopyDirectionToUsb;
    mCurCopyFile = copyFileName;
    mCurCopyFileSize = fileSize;
    mWriteTargetFromUsb = (int)Enums::UsbDownloadType::BackupDateData;
    connect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
    connect(mFtpManager, SIGNAL(ftpListIndexChanged(int)), this, SLOT(onFtpListIndexChanged(int)));
    connect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
    connect(mFtpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)), Qt::QueuedConnection);
    connect(mFtpManager, SIGNAL(ftpExtMemFinishNotify(bool)), this, SLOT(onFtpExtMemFinishNotify(bool)), Qt::QueuedConnection);
    connect(mFtpManager, SIGNAL(ftpFileSizeNotify(int)), this, SLOT(onFtpFileSizeNotify(int)), Qt::QueuedConnection);
    timerStart(EXT_MEM_COPY_TIMER_INTERVAL);
    return 0;
}

void ExternalMemoryViewModel::nextWriteInBulkToUsb(int curListIndex)
{
    if (mCurUsbFolder.length() == 0 || m_remoteDirFileCnt <= 0 || curListIndex < 0){
        qCritical("%s:%d mCurUsbFolder length[%d] remote fileCnt[%d] curListIndex[%d] is invalid",
            __func__, __LINE__, mCurUsbFolder.length(), m_remoteDirFileCnt, curListIndex);
        return;
    }
    QString dstDirPath = mUsbRootPath + mCurUsbFolder + "/";
    QString srcDirPath = "/";
    QString copyFileName = mRemoteDirFileInfo.at(curListIndex).name;
    qint64 fileSize = mRemoteDirFileInfo.at(curListIndex).size;
    lockFtpGetMutex();
    mGetFile = new QFile(dstDirPath + copyFileName);
#if 1
    if(mGetFile->exists()) {
        // target already exists in USB
        mGetFile->remove();
    }
#endif
    mGetFile->open(QFile::WriteOnly);
    int result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandCWD, nullptr, srcDirPath);
    result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandRETR, mGetFile, copyFileName);        // Ftp get
    stFtpGetArgument arg;
    arg.getFile = mGetFile;
    arg.selectedIndex = curListIndex;
    arg.isToExtMem = true;
    mFtpManager->setFtpGetArgment(result, arg);
    mUsbDirection = (int)CopyDirectionStatus::CopyDirectionToUsb;
    mCurCopyFile = copyFileName;
    mCurCopyFileSize = fileSize;
    connect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
    connect(mFtpManager, SIGNAL(ftpListIndexChanged(int)), this, SLOT(onFtpListIndexChanged(int)));
    connect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
    connect(mFtpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)), Qt::QueuedConnection);
    connect(mFtpManager, SIGNAL(ftpExtMemFinishNotify(bool)), this, SLOT(onFtpExtMemFinishNotify(bool)), Qt::QueuedConnection);
    connect(mFtpManager, SIGNAL(ftpFileSizeNotify(int)), this, SLOT(onFtpFileSizeNotify(int)), Qt::QueuedConnection);
    timerStart(EXT_MEM_COPY_TIMER_INTERVAL);
}

void ExternalMemoryViewModel::writeInBulkFromUsb(QString targetDirName)
{
    checkTargetDir(targetDirName);
    qDebug() << "extUsbMemoryFileList mWriteTargetFromUsb:" << mWriteTargetFromUsb << "targetDirName:" << targetDirName;
    switch(mWriteTargetFromUsb){
    case (int)Enums::UsbDownloadType::ControllerUpdate:{
        if (!m_isModbusConnect) { goto process_error; }
        mIsPowerRestart = true;
        downloadFromUsb(targetDirName);
    }
        break;
    case (int)Enums::UsbDownloadType::PenUpdate:
    case (int)Enums::UsbDownloadType::PenMsgData:
        mIsPowerRestart = true;
        downloadFromUsb(targetDirName);
        break;
    case (int)Enums::UsbDownloadType::ImageData:
        mIsPowerRestart = true;
        downloadFromUsb(targetDirName);
        break;
    default:
        goto process_error;
    }
    return;
process_error:
    mIsPowerRestart = false;
    mWriteTargetFromUsb = -1;
    mFtpManager->setIsDeterNoopCmd(false);
    unlockFtpPutMutex();
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    return;
}

void ExternalMemoryViewModel::updateRemoteDirFileInfo(ExternalMemoryViewFileList *updateUsbMemoryFileList, QString dstDirPath)
{
    if (!updateUsbMemoryFileList) { return; }
    mUpdateInfo.name = updateUsbMemoryFileList->fileName();
    mUpdateInfo.parent = updateUsbMemoryFileList->fileParent();
    mUpdateInfo.isDir = updateUsbMemoryFileList->fileUrlDir();
    mUpdateInfo.path = dstDirPath;
    mUpdateInfo.lastModified = updateUsbMemoryFileList->fileLastModified();
    mUpdateInfo.size = updateUsbMemoryFileList->fileSize();
}

int ExternalMemoryViewModel::writeIndividualFromUsb(
    ExternalMemoryViewFileList *extUsbMemoryFileList, ExternalMemoryViewFileList *extUsbMemoryChildDirFileList,
    int listIndex, bool isChildDir)
{
    QString dstDirPath = "/";
    QString srcDirPath = mUsbRootPath;
    ExternalMemoryViewFileList *updateUsbMemoryFileList = nullptr;
    QString targetDirName = "";
    QString targetFileName = "";
    if (isChildDir){
        if (!extUsbMemoryFileList || !extUsbMemoryChildDirFileList){
            qCritical() << "extUsbMemoryFileList or extUsbMemoryChildDirFileList is invalid:";
            return -1;
        }
        updateUsbMemoryFileList = extUsbMemoryChildDirFileList;
        srcDirPath = srcDirPath + extUsbMemoryFileList->fileName() + "/";
        targetDirName = extUsbMemoryFileList->fileName();
        targetFileName = extUsbMemoryChildDirFileList->fileName();
    } else {
        if (!extUsbMemoryFileList){
            qCritical() << "extUsbMemoryFileList is invalid:";
            return -1;
        }
        updateUsbMemoryFileList = extUsbMemoryFileList;
        targetFileName = extUsbMemoryFileList->fileName();
    }
    if (!updateUsbMemoryFileList) { return -1; }
    updateRemoteDirFileInfo(updateUsbMemoryFileList);
    int result = 0;
    if (targetDirName == ""){
        checkTargetFile(targetFileName);
    } else {
        checkTargetDir(targetDirName);
    }
    mTargetListNum = 1;
    qDebug() << "extUsbMemoryFileList mWriteTargetFromUsb:" << mWriteTargetFromUsb;
    switch(mWriteTargetFromUsb){
    case (int)Enums::UsbDownloadType::NcpData:{
        if (!m_isModbusConnect) { return -1; }
        QString fileName = renameNcpFile(targetFileName, srcDirPath);
        if (fileName == ""){
            qCritical() << "ncp file invalid:" << targetFileName;
            result = -1;
        } else {
            result = copyToController(fileName, dstDirPath, srcDirPath, listIndex);
            if (result < 0){
                qDebug("%s:%d copyToController result[%d]", __func__, __LINE__, result);
                qDebug() << "writeIndividualFromUsb targetFileName:" << targetFileName;
                if (targetFileName != mRenameNcpFileName){
                    removeRenameFile(fileName, srcDirPath);
                } else {
                    QString tmpNcpFileName = mRenameNcpFileName;
                    removeRenameFile(tmpNcpFileName, srcDirPath);
                    undoNcpFileName(fileName, srcDirPath);
                }
            } else {
                asyncDownloadToSd(mNcpFileExistDirPath, srcDirPath, fileName);
            }
        }
        break;
    }
    case (int)Enums::UsbDownloadType::PenUpdate:{
        bool isRename = false;
        int logoType = (int)LogoType::LogoTypeInvalid;
        QString targetDirPath = getCopyDirPath(targetFileName, &isRename, &logoType);
        if (targetDirPath == ""){
            qCritical() << "PenUpdate targetDirPath is invalid:";
            result = -1;
        } else {
            if (isRename) { targetFileName = renameLogoFile(targetFileName, srcDirPath, logoType); }
            mIsPowerRestart = true;
            if (isRename) { mRemoveRenameFile = true; }
            asyncDownloadToSd(targetDirPath, srcDirPath, targetFileName);
        }
        break;
    }
    case (int)Enums::UsbDownloadType::PenMsgData:{
        mIsPowerRestart = true;
        asyncDownloadToSd(mPenMsgExistDirPath, srcDirPath, targetFileName);
        break;
    }
    case (int)Enums::UsbDownloadType::BackupDateData:
    case (int)Enums::UsbDownloadType::ControllerUpdate:{
        if (!m_isModbusConnect) { return -1; }
        mIsPowerRestart = checkEffectivePowerRestart(targetFileName);
        result = copyToController(targetFileName, dstDirPath, srcDirPath, listIndex);
        if (result < 0){
            qCritical("%s:%d copyToController result[%d]", __func__, __LINE__, result);
        }
        break;
    }
    case (int)Enums::UsbDownloadType::ComSpeedSetting:{
        //mIsPowerRestart = checkEffectivePowerRestart(targetFileName);
        mIsPowerRestart = true;
        asyncDownloadToSd(mComSpeedSetFileExistDirPath, srcDirPath, targetFileName);
        break;
    }
    default:{
        bool isRename = false;
        int logoType = (int)LogoType::LogoTypeInvalid;
        QString targetDirPath = getCopyDirPath(targetFileName, &isRename, &logoType);
        if (targetDirPath == "") {  // to controller(FTP)
            if (!m_isModbusConnect) { return -1; }
            mIsPowerRestart = checkEffectivePowerRestart(targetFileName);
            mTargetListNum = 1;
            result = copyToController(targetFileName, dstDirPath, srcDirPath, listIndex);
            if (result < 0){
                qCritical("%s:%d copyToController result[%d]", __func__, __LINE__, result);
            }
        } else {
            if (isRename) { targetFileName = renameLogoFile(targetFileName, srcDirPath, logoType); }
            mIsPowerRestart = true;
            if (isRename) { mRemoveRenameFile = true; }
            asyncDownloadToSd(targetDirPath, srcDirPath, targetFileName);
        }
        break;
    }
    }
    return result;
}

bool ExternalMemoryViewModel::downloadToSd(const QString dstPath, QString srcPath, QString fileName)
{
    qDebug() << "ExternalMemoryViewModel::downloadToSd dstPath:" << dstPath << "srcPath:" << srcPath << "fileName:" << fileName;
    CoreLibSdControl coreLibSdCtrl;
    bool ret = false;
    ret = coreLibSdCtrl.copyFile(srcPath + fileName, dstPath + fileName, true);
    if (!ret){
        qCritical() << "downloadToSd copyFile failed:" << ret;
    }
    return ret;
}

bool ExternalMemoryViewModel::downloadDirToSd(const QString dstPath, QString srcPath)
{
    qDebug() << "ExternalMemoryViewModel::downloadDirToSd dstPath:" << dstPath << "srcPath:" << srcPath;
    bool ret = false;
    ret = copyDir(dstPath,srcPath,true);
    if (!ret){
        qCritical() << "downloadDirToSd copyDir failed:" << ret;
    }
    return ret;
}

bool ExternalMemoryViewModel::copyDir(const QString dstPath, QString srcPath, bool isOverWrite)
{
    QDir scrDir(srcPath);
    QDir dstDir(dstPath);
    QFileInfoList fileInfoList = scrDir.entryInfoList();
    if(!scrDir.exists()){
        return false;
    }
    if(isOverWrite){
        dstDir.removeRecursively();
        dstDir.mkpath(dstPath);
    }
    foreach(QFileInfo fileinfo, fileInfoList){
        if(fileinfo.isDir() && (fileinfo.fileName() != "..")){
            dstDir.mkdir(fileinfo.fileName());
            bool ret =  copySubDir(dstPath + "/" + fileinfo.fileName(),fileinfo.filePath());
            if(!ret){
                return false;
            }
        }
//        else {
//            if(!QFile::copy(fileinfo.filePath(), dstPath + "/" + fileinfo.fileName())){
//                qDebug() << "main error fileinfo filePath::::::::::::::::" << fileinfo.filePath() << "main error fileinfo dstPath" << dstPath + "/" + fileinfo.fileName();
//                return false;
//            }
//               return true;
//        }
    }
    return true;
}

bool ExternalMemoryViewModel::copySubDir(const QString dstPath, QString srcPath)
{
    QDir scrDir(srcPath);
    QDir dstDir(dstPath);
    QFileInfoList fileInfoList = scrDir.entryInfoList();

    foreach(QFileInfo fileinfo, fileInfoList){
        if(fileinfo.isDir()){
            continue;
        } else {
            if(!QFile::copy(fileinfo.filePath(), dstPath + "/" + fileinfo.fileName())){
                qCritical() << "sub error fileinfo name" << fileinfo.fileName() << "sub error fileinfo filePath::::::::::::::::" << fileinfo.filePath();
                return false;
            }
        }
    }
    return true;
}

void ExternalMemoryViewModel::asyncDownloadToSd(const QString dstPath, QString srcPath, QString fileName)
{
    mTargetDirPath = dstPath;
    mSrcDirPath = srcPath;
    mTargetFileName = fileName;
    singleTimerStart(EXT_MEM_ASYNC_SINGLE_TIMER_VALUE);
}

void ExternalMemoryViewModel::downloadToController(const QString dstPath, QString srcPath, QString fileName)
{
    qDebug() << "ExternalMemoryViewModel::downloadToController dstPath:" << dstPath << "srcPath:" << srcPath << "fileName:" << fileName;
    int result = 0;
    result = copyToController(fileName, dstPath, srcPath);
    if (result < 0){
        qCritical() << "downloadToController copyToController failed:" << result;
    }
}

void ExternalMemoryViewModel::downloadBkDateData(QString targetDirName, int startIndex)
{
    bool isFind = false;
    mTargetListNum = m_extUsbMemoryChildDirFileList.count();
    int targetListIndex = 0;
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList;
    QString srcDirPath = "";
    QString targetFileName = "";
    int i = 0;
    qDebug() << "ExternalMemoryViewModel::downloadBkDateData startIndex:" << startIndex << "targetDirName:" << targetDirName;
    for(i = startIndex; i < mTargetListNum; i++){
        isFind = false;
        extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(i));
        targetFileName = extUsbMemoryChildDirFileList->fileName();
        if (!isFind){
            for(int j = 0; j < FTP_PUT_FIXED_DAILY_BACKUP_LIST.count(); j++){
                if (targetFileName == FTP_PUT_FIXED_DAILY_BACKUP_LIST.at(j)){
                    srcDirPath = mUsbRootPath + targetDirName + "/";
                    isFind = true;
                    targetListIndex = i;
                    break;
                }
            }
            if (isFind){ break; }
        }
        if (!isFind){
            if (targetFileName.contains("COND")){
                for(int j = mCondStartIndex; j < EXT_MEM_COND_BACKUP_MAX; j++){
                    QString fileIndex = (j < 10) ? "000" + QString::number(j) : ((j < 100) ? "00" + QString::number(j) : ((j < 1000) ? "0" + QString::number(j) : QString::number(j)));
                    QString orgFileName = FTP_PUT_VARIABLE_DAILY_COND_BACKUP_LIST.at(0);
                    orgFileName.insert(4, (const QString)fileIndex);
                    if (targetFileName == orgFileName){
                        srcDirPath = mUsbRootPath + targetDirName + "/";
                        isFind = true;
                        mCondStartIndex = (j <= EXT_MEM_COND_BACKUP_MAX - 1) ? j : EXT_MEM_COND_BACKUP_MAX;
                        targetListIndex = i;
                        break;
                    }
                }
                if (isFind){ break; }
            }
        }
        if (!isFind){
            if (targetFileName.contains("machine")){
                for(int j = mMachineStartIndex; j < EXT_MEM_MACHINE_BACKUP_MAX; j++){
                    QString fileIndex = (j < 10) ? "0" + QString::number(j) : QString::number(j);
                    QString orgFileName = FTP_PUT_VARIABLE_DAILY_MACHINE_BACKUP_LIST.at(0);
                    orgFileName.insert(7, (const QString)fileIndex);
                    if (targetFileName == orgFileName){
                        srcDirPath = mUsbRootPath + targetDirName + "/";
                        isFind = true;
                        mMachineStartIndex = (j <= EXT_MEM_MACHINE_BACKUP_MAX - 1) ? j : EXT_MEM_MACHINE_BACKUP_MAX;
                        targetListIndex = i;
                        break;
                    }
                }
                if (isFind){ break; }
            }
        }
    }
    if (isFind){
        int result = 0;
        result = copyToController(targetFileName, "/", srcDirPath, targetListIndex);
        if (result < 0){
            qCritical() << "ExternalMemoryViewModel::downloadBkDateData copyToController failed:" << result;
            goto process_error;
        }
    } else {
        if (i >= (mTargetListNum - 1)){
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
            m_processingResult = true;
            emit processingResultChanged(m_processingResult);
            mFtpManager->setIsDeterNoopCmd(false);
            m_preFtpListIndex = -1;
            m_ftpListIndex = -1;
            mWriteTargetFromUsb = -1;
        } else {
            qCritical("%s:%d mTargetListNum[%d] i[%d]", __func__, __LINE__, mTargetListNum, i);
            goto process_error;
        }
    }
    return;
process_error:
    qCritical() << "downloadBkDateData process_error isFind:" << isFind << "targetListIndex:" << targetListIndex << "mTargetListNum:" << mTargetListNum << "i:" << i;
    terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    mFtpManager->setIsDeterNoopCmd(false);
    m_preFtpListIndex = -1;
    m_ftpListIndex = -1;
    mWriteTargetFromUsb = -1;
}

void ExternalMemoryViewModel::downloadControllerUpdateData(QString targetDirName, int startIndex)
{
    bool isFind = false;
    mTargetListNum = m_extUsbMemoryChildDirFileList.count();
    int targetListIndex = 0;
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList;
    QString srcDirPath = "";
    QString targetFileName = "";
    int i = 0;
    qDebug() << "ExternalMemoryViewModel::downloadControllerUpdateData startIndex:" << startIndex << "targetDirName:" << targetDirName;
    for(i = startIndex; i < mTargetListNum; i++){
        isFind = false;
        extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(i));
        targetFileName = extUsbMemoryChildDirFileList->fileName();
        if (!isFind){
            for(int j = 0; j < FTP_PUT_FIXED_CONTROLLER_UPDATE_LIST.count(); j++){
                if (targetFileName == FTP_PUT_FIXED_CONTROLLER_UPDATE_LIST.at(j)){
                    srcDirPath = mUsbRootPath + targetDirName + "/";
                    isFind = true;
                    targetListIndex = i;
                    break;
                }
            }
            if (isFind){ break; }
        }
        if (!isFind){
            for(int j = 1; j <= EXT_MEM_VARIABLE_CONTROLLER_BACKUP_MAX; j++){
                QString fileIndex = (j < 10) ? "0" + QString::number(j) : QString::number(j);
                QString orgFileName = FTP_PUT_VARIABLE_CONTROLLER_UPDATE_LIST.at(0);
                orgFileName.insert(7, (const QString)fileIndex);
                if (targetFileName == orgFileName){
                    srcDirPath = mUsbRootPath + targetDirName + "/";
                    isFind = true;
                    targetListIndex = i;
                    break;
                }
            }
            if (isFind){ break; }
        }
    }
    if (isFind){
        int result = 0;
        result = copyToController(targetFileName, "/", srcDirPath, targetListIndex);
        if (result < 0){
            qCritical() << "ExternalMemoryViewModel::downloadControllerUpdateData copyToController failed:" << result;
            goto process_error;
        }
    } else {
        if (i >= (mTargetListNum - 1)){
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
            m_processingResult = true;
            emit processingResultChanged(m_processingResult);
            mFtpManager->setIsDeterNoopCmd(false);
            if(mIsPowerRestart) {
                m_requestPowerRestart = true;
                emit requestPowerRestartChanged(m_requestPowerRestart);
                mIsPowerRestart = false;
#ifdef REAL_TARGET
                int ret = system("sync");
                (void)ret;
#endif // REAL_TARGET
            }
            m_preFtpListIndex = -1;
            m_ftpListIndex = -1;
            mWriteTargetFromUsb = -1;
        } else {
            qCritical("%s:%d mTargetListNum[%d] i[%d]", __func__, __LINE__, mTargetListNum, i);
            goto process_error;
        }
    }
    return;
process_error:
    qCritical() << "downloadControllerUpdateData process_error isFind:" << isFind << "targetListIndex:" << targetListIndex << "mTargetListNum:" << mTargetListNum << "i:" << i;
    terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    mFtpManager->setIsDeterNoopCmd(false);
    m_preFtpListIndex = -1;
    m_ftpListIndex = -1;
    mWriteTargetFromUsb = -1;
}

void ExternalMemoryViewModel::downloadNcpData(QString targetDirName, int startIndex)
{
    bool isFind = false;
    mTargetListNum = m_extUsbMemoryChildDirFileList.count();
    int targetListIndex = 0;
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList;
    QString srcDirPath = "";
    QString targetFileName = "";
    int i = 0;
    qDebug() << "ExternalMemoryViewModel::downloadNcpData startIndex:" << startIndex << "targetDirName:" << targetDirName;
    for(i = startIndex; i < mTargetListNum; i++){
        isFind = false;
        extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(i));
        targetFileName = extUsbMemoryChildDirFileList->fileName();
        if (!isFind){
            for(int j = 1; j < EXT_MEM_NCP_BACKUP_MAX; j++){
                QString fileIndex = (j < 10) ? "0" + QString::number(j) : QString::number(j);
                QString orgFileName = FTP_PUT_NCP_LIST.at(0);
                orgFileName.insert(3, (const QString)fileIndex);
                if (targetFileName == orgFileName){
                    srcDirPath = mUsbRootPath + targetDirName + "/";
                    isFind = true;
                    targetListIndex = i;
                    break;
                }
            }
        }
    }
    if (isFind){
        int result = 0;
        result = copyToController(targetFileName, "/", srcDirPath, targetListIndex);
        if (result < 0){
            qCritical() << "ExternalMemoryViewModel::downloadNcpData copyToController failed:" << result;
            goto process_error;
        }
    } else {
        if (i >= (mTargetListNum - 1)){
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
            m_processingResult = true;
            emit processingResultChanged(m_processingResult);
            mFtpManager->setIsDeterNoopCmd(false);
            m_preFtpListIndex = -1;
            m_ftpListIndex = -1;
            mWriteTargetFromUsb = -1;
        } else {
            qCritical("%s:%d mTargetListNum[%d] i[%d]", __func__, __LINE__, mTargetListNum, i);
            goto process_error;
        }
    }
    return;
process_error:
    qCritical() << "downloadNcpData process_error isFind:" << isFind << "targetListIndex:" << targetListIndex << "mTargetListNum:" << mTargetListNum << "i:" << i;
    terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    mFtpManager->setIsDeterNoopCmd(false);
    m_preFtpListIndex = -1;
    m_ftpListIndex = -1;
    mWriteTargetFromUsb = -1;
}

bool ExternalMemoryViewModel::downloadPenUpdateData(QString targetDirName, int startIndex)
{
    bool ret = true;
    bool isFind = false;
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList;
    int i = 0;
    QString targetFileName = "";
    QFile::Permissions permissions =
        (QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
         QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ExeUser |
         QFileDevice::ReadGroup | QFileDevice::ExeGroup |
         QFileDevice::ReadOther | QFileDevice::ExeOther);
    for(i = startIndex; i < m_extUsbMemoryChildDirFileList.count(); i++){
        isFind = false;
        extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(i));
        targetFileName = extUsbMemoryChildDirFileList->fileName();
        if (!isFind){
            for(int j = 0; j < WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST.count(); j++){
                if (targetFileName == WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST.at(j)){
                    // download to sd (~/images)
                    QString srcDirPath = mUsbRootPath + targetDirName + "/";
                    targetFileName = renameLogoFile(targetFileName, srcDirPath, (int)LogoType::LogoType1);
                    if (targetFileName == ""){
                        goto process_error;
                    }
                    ret = downloadToSd(mImagesDirPath, srcDirPath, targetFileName);
                    if(!ret){
                        removeRenameFile(targetFileName, srcDirPath);
                        goto process_error;
                    } else {
                        removeRenameFile(targetFileName, srcDirPath);
                    }
                    isFind = true;
                    break;
                }
            }
        }
        if (!isFind){
            for(int j = 0; j < WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST.count(); j++){
                if (targetFileName == WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST.at(j)){
                    // download to sd (~/images/00_COMMON)
                    QString srcDirPath = mUsbRootPath + targetDirName + "/";
                    targetFileName = renameLogoFile(targetFileName, srcDirPath, (int)LogoType::LogoType2);
                    if (targetFileName == ""){
                        goto process_error;
                    }
                    ret = downloadToSd(mCommonImagesDirPath, srcDirPath, targetFileName);
                    if(!ret){
                        removeRenameFile(targetFileName, srcDirPath);
                        goto process_error;
                    } else {
                        removeRenameFile(targetFileName, srcDirPath);
                    }
                    isFind = true;
                    break;
                }
            }
        }
        if (!isFind){
            for(int j = 0; j < WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST.count(); j++){
                if (targetFileName == WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST.at(j)){
                    // download to sd (~/bin)
                    QString srcDirPath = mUsbRootPath + targetDirName + "/";
                    ret = downloadToSd(mAplExistDirPath, srcDirPath, targetFileName);
                    if(!ret){
                        goto process_error;
                    }
                    isFind = true;
                    break;
                }
            }
        }
        if (!isFind){
            for(int j = 0; j < WRITE_FROM_USB_PEN_SET_LIST.count(); j++){
                if (targetFileName == WRITE_FROM_USB_PEN_SET_LIST.at(j)){
                    // download to sd (~/settings/set_folder)
                    QString srcDirPath = mUsbRootPath + targetDirName + "/";
                    ret = downloadToSd(mPenSetExistDirPath, srcDirPath, targetFileName);
                    QFile dstFile(mPenSetExistDirPath + targetFileName);
                    dstFile.setPermissions(permissions);
                    if(!ret){
                        goto process_error;
                    }
                    isFind = true;
                    break;
                }
            }
        }
        if (!isFind){
            for(int j = 0; j < WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST.count(); j++){
                if (targetFileName == WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST.at(j)){
                    // download to sd (/opt/SystemSettingApp/bin)
                    QString srcDirPath = mUsbRootPath + targetDirName + "/";
                    ret = downloadToSd(mSysSetAplExistDirPath, srcDirPath, targetFileName);
                    if(!ret){
                        goto process_error;
                    }
                    isFind = true;
                    break;
                }
            }
        }
        if (!isFind){
            for(int j = 0; j < WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST.count(); j++){
                if (targetFileName == WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST.at(j)){
                    // download to sd (/opt/SystemSettingApp/CoreLib)
                    QString srcDirPath = mUsbRootPath + targetDirName + "/";
                    ret = downloadToSd(mCoreLibExistDirPath, srcDirPath, targetFileName);
                    if(!ret){
                        goto process_error;
                    }
                    isFind = true;
                    break;
                }
            }
        }
    }
    return ret;
process_error:
    qCritical() << "downloadPenUpdateData failed targetFileName:" << targetFileName << "index:" << i;
    mWriteTargetFromUsb = -1;
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    return ret;
}
#ifdef ENABLE_ASYNC_COPY_TO_SD_FROM_USB
void ExternalMemoryViewModel::asyncDownloadPenUpdateData(QString targetDirName, int curIndex, bool isFirst)
{
    stFileTransInfo transInfo;
    initFileTransInfo(transInfo);
    bool isFind = false;
    QString targetFileName = "";
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList;
    extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(curIndex));
    targetFileName = extUsbMemoryChildDirFileList->fileName();
    if (!isFind){
        for(int j = 0; j < WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST.count(); j++){
            if (targetFileName == WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST.at(j)){
                // download to sd (~/images)
                QString srcDirPath = mUsbRootPath + targetDirName + "/";
                targetFileName = renameLogoFile(targetFileName, srcDirPath, (int)LogoType::LogoType1);
                if (targetFileName == ""){
                    goto process_error;
                }
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = srcDirPath;
                transInfo.dstFilePath = mImagesDirPath;
                transInfo.targetDirName = targetDirName;
                transInfo.targetFileName = targetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = curIndex;
                transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
                mTargetFileName = targetFileName;
                mSrcDirPath = srcDirPath;
                mRemoveRenameFile = true;
                mTransferFileThread->setFileTransInfo(transInfo);
                mTransferFileThread->setIsCopying(false);
                if (isFirst) { startAsyncFileTrasnfer(); }
                isFind = true;
                break;
            }
        }
    }
    if (!isFind){
        for(int j = 0; j < WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST.count(); j++){
            if (targetFileName == WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST.at(j)){
                // download to sd (~/images/00_COMMON)
                QString srcDirPath = mUsbRootPath + targetDirName + "/";
                targetFileName = renameLogoFile(targetFileName, srcDirPath, (int)LogoType::LogoType2);
                if (targetFileName == ""){
                    goto process_error;
                }
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = srcDirPath;
                transInfo.dstFilePath = mCommonImagesDirPath;
                transInfo.targetDirName = targetDirName;
                transInfo.targetFileName = targetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = curIndex;
                transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
                mTargetFileName = targetFileName;
                mSrcDirPath = srcDirPath;
                mRemoveRenameFile = true;
                mTransferFileThread->setFileTransInfo(transInfo);
                mTransferFileThread->setIsCopying(false);
                if (isFirst) { startAsyncFileTrasnfer(); }
                isFind = true;
                break;
            }
        }
    }
    if (!isFind){
        for(int j = 0; j < WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST.count(); j++){
            if (targetFileName == WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST.at(j)){
                // download to sd (~/bin)
                QString srcDirPath = mUsbRootPath + targetDirName + "/";
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = srcDirPath;
                transInfo.dstFilePath = mAplExistDirPath;
                transInfo.targetDirName = targetDirName;
                transInfo.targetFileName = targetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = curIndex;
                transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
                mTargetFileName = targetFileName;
                mSrcDirPath = srcDirPath;
                mRemoveRenameFile = false;
                mTransferFileThread->setFileTransInfo(transInfo);
                mTransferFileThread->setIsCopying(false);
                if (isFirst) { startAsyncFileTrasnfer(); }
                isFind = true;
                break;
            }
        }
    }
    if (!isFind){
        for(int j = 0; j < WRITE_FROM_USB_PEN_SET_LIST.count(); j++){
            if (targetFileName == WRITE_FROM_USB_PEN_SET_LIST.at(j)){
                // download to sd (~/settings/set_folder)
                QString srcDirPath = mUsbRootPath + targetDirName + "/";
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = srcDirPath;
                transInfo.dstFilePath = mPenSetExistDirPath;
                transInfo.targetDirName = targetDirName;
                transInfo.targetFileName = targetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = curIndex;
                transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
                mTargetFileName = targetFileName;
                mSrcDirPath = srcDirPath;
                mRemoveRenameFile = false;
                mTransferFileThread->setFileTransInfo(transInfo);
                mTransferFileThread->setIsCopying(false);
                if (isFirst) { startAsyncFileTrasnfer(); }
                isFind = true;
                break;
            }
        }
    }
    if (!isFind){
        for(int j = 0; j < WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST.count(); j++){
            if (targetFileName == WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST.at(j)){
                // download to sd (/opt/SystemSettingApp/bin)
                QString srcDirPath = mUsbRootPath + targetDirName + "/";
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = srcDirPath;
                transInfo.dstFilePath = mSysSetAplExistDirPath;
                transInfo.targetDirName = targetDirName;
                transInfo.targetFileName = targetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = curIndex;
                transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
                mTargetFileName = targetFileName;
                mSrcDirPath = srcDirPath;
                mRemoveRenameFile = false;
                mTransferFileThread->setFileTransInfo(transInfo);
                mTransferFileThread->setIsCopying(false);
                if (isFirst) { startAsyncFileTrasnfer(); }
                isFind = true;
                break;
            }
        }
    }
    if (!isFind){
        for(int j = 0; j < WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST.count(); j++){
            if (targetFileName == WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST.at(j)){
                // download to sd (/opt/SystemSettingApp/CoreLib)
                QString srcDirPath = mUsbRootPath + targetDirName + "/";
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = srcDirPath;
                transInfo.dstFilePath = mCoreLibExistDirPath;
                transInfo.targetDirName = targetDirName;
                transInfo.targetFileName = targetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = curIndex;
                transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
                mTargetFileName = targetFileName;
                mSrcDirPath = srcDirPath;
                mRemoveRenameFile = false;
                mTransferFileThread->setFileTransInfo(transInfo);
                mTransferFileThread->setIsCopying(false);
                if (isFirst) { startAsyncFileTrasnfer(); }
                isFind = true;
                break;
            }
        }
    }
    if (!isFind){
        transInfo.curListIndex = curIndex;
        transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
        mRemoveRenameFile = false;
        mTransferFileThread->setFileTransInfo(transInfo);
        mTransferFileThread->setIsCopying(false);
        if (isFirst) { startAsyncFileTrasnfer(); }
    }
    return;
process_error:
    qCritical() << "asyncDownloadPenUpdateData failed targetFileName:" << targetFileName;
    mWriteTargetFromUsb = -1;
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    return;
}

void ExternalMemoryViewModel::asyncDownloadPenMsgData(QString targetDirName, int curIndex, bool isFirst)
{
    stFileTransInfo transInfo;
    initFileTransInfo(transInfo);
    bool isFind = false;
    QString targetFileName = "";
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList;
    extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(curIndex));
    targetFileName = extUsbMemoryChildDirFileList->fileName();
    if (!isFind){
        for(int j = 0; j < WRITE_FROM_USB_PEN_MSG_LIST.count(); j++){
            if (targetFileName == WRITE_FROM_USB_PEN_MSG_LIST.at(j)){
                // download to sd (~/settings/msg_folder)
                QString srcDirPath = mUsbRootPath + targetDirName + "/";
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = srcDirPath;
                transInfo.dstFilePath = mPenMsgExistDirPath;
                transInfo.targetDirName = targetDirName;
                transInfo.targetFileName = targetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = curIndex;
                transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
                mTargetFileName = targetFileName;
                mSrcDirPath = srcDirPath;
                mRemoveRenameFile = false;
                mTransferFileThread->setFileTransInfo(transInfo);
                mTransferFileThread->setIsCopying(false);
                if (isFirst) { startAsyncFileTrasnfer(); }
                isFind = true;
                break;
            }
        }
    }
    if (!isFind){
        transInfo.curListIndex = curIndex;
        transInfo.totalListNum = m_extUsbMemoryChildDirFileList.count();
        mRemoveRenameFile = false;
        mTransferFileThread->setFileTransInfo(transInfo);
        mTransferFileThread->setIsCopying(false);
        if (isFirst) { startAsyncFileTrasnfer(); }
    }
    return;
}

void ExternalMemoryViewModel::initFileTransInfo(stFileTransInfo &transInfo)
{
    transInfo.writeTargetFromUsb = -1;
    transInfo.srcFilePath = "";
    transInfo.dstFilePath = "";
    transInfo.targetDirName = "";
    transInfo.targetFileName = "";
    transInfo.isOverWrite = false;
}
#endif // ENABLE_ASYNC_COPY_TO_SD_FROM_USB

bool ExternalMemoryViewModel::downloadPenMsgData(QString targetDirName, int startIndex)
{
    bool ret = true;
    bool isFind = false;
    int i = 0;
    QString targetFileName = "";
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList;
    for(i = startIndex; i < m_extUsbMemoryChildDirFileList.count(); i++){
        isFind = false;
        extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(i));
        targetFileName = extUsbMemoryChildDirFileList->fileName();
        if (!isFind){
            for(int j = 0; j < WRITE_FROM_USB_PEN_MSG_LIST.count(); j++){
                if (targetFileName == WRITE_FROM_USB_PEN_MSG_LIST.at(j)){
                    // download to sd (~/settings/msg_folder)
                    QString srcDirPath = mUsbRootPath + targetDirName + "/";
                    ret = downloadToSd(mPenMsgExistDirPath, srcDirPath, targetFileName);
                    if(!ret){
                        goto process_error;
                    }
                    isFind = true;
                    break;
                }
            }
        }
    }
    return ret;
process_error:
    qCritical() << "downloadPenMsgData failed targetFileName:" << targetFileName << "index:" << i;
    mWriteTargetFromUsb = -1;
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    return ret;
}

bool ExternalMemoryViewModel::downloadImageData(QString targetDirName)
{
    bool ret = true;
    QString srcDirPath = mUsbRootPath + targetDirName;
    QDir folderDir(srcDirPath);
    QFileInfoList fileList = folderDir.entryInfoList();
    foreach (const QFileInfo &fileInfo, fileList) {
        if(fileInfo.isDir()){
            QDir subFolderDir(fileInfo.filePath());
            QFileInfoList subFileList = subFolderDir.entryInfoList();
            foreach (const QFileInfo &fileInfo, subFileList) {
                if(!fileInfo.isDir()){
                    QString dst = mImageFileExistDirPath + "/" + subFolderDir.dirName() + "/" + fileInfo.fileName();
                    QFile file(dst);
                    if(file.exists()){
                        QFile::remove(dst);
                    }
                    bool ret1 = QFile::copy(fileInfo.filePath(), dst);
                    if(!ret1){
                       ret = false;
                    }
//                    qDebug() << "1111<<<<<<<<<<<<<<<" << fileInfo.filePath() << ":::dst:::" << dst; // 输出文件路径
                }
            }
        }else {
            QString dst = mImageFileExistDirPath + "/" + fileInfo.fileName();
            QFile file(dst);
            if(file.exists()){
                QFile::remove(dst);
            }
            bool ret1 = QFile::copy(fileInfo.filePath(), dst);
            if(!ret1){
               ret = false;
            }
//            qDebug() << "<<<<<<<<<<<<<<<" << fileInfo.filePath() << ":::dst:::" << dst; // 输出文件路径
        }
    }
//    QString srcDirPath = mUsbRootPath + targetDirName;
//    ret = downloadDirToSd(mImageFileExistDirPath, srcDirPath);
    return ret;
}

bool ExternalMemoryViewModel::downloadPenSetData(QString targetDirName, int startIndex)
{
    bool ret = true;
    bool isFind = false;
    int i = 0;
    QString targetFileName = "";
    ExternalMemoryViewFileList *extUsbMemoryChildDirFileList;
    for(i = startIndex; i < m_extUsbMemoryChildDirFileList.count(); i++){
        isFind = false;
        extUsbMemoryChildDirFileList = static_cast<ExternalMemoryViewFileList *>(m_extUsbMemoryChildDirFileList.at(i));
        targetFileName = extUsbMemoryChildDirFileList->fileName();
        if (!isFind){
            for(int j = 0; j < WRITE_FROM_USB_PEN_SET_LIST.count(); j++){
                if (targetFileName == WRITE_FROM_USB_PEN_SET_LIST.at(j)){
                    // download to sd (~/settings/set_folder)
                    QString srcDirPath = mUsbRootPath + targetDirName + "/";
                    ret = downloadToSd(mPenSetExistDirPath, srcDirPath, targetFileName);
                    if(!ret){
                        goto process_error;
                    }
                    isFind = true;
                    break;
                }
            }
        }
    }
    return ret;
process_error:
    qCritical() << "downloadPenSetData process_error targetFileName:" << targetFileName << "index:" << i;
    mWriteTargetFromUsb = -1;
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    return ret;
}

QString ExternalMemoryViewModel::getCopyDirPath(QString targetFileName, bool *pIsRename, int *pLogoType)
{
    qDebug() << "ExternalMemoryViewModel::getCopyDirPath targetFileName:" << targetFileName;
    if (!pIsRename || !pLogoType) {
        qCritical() << "ExternalMemoryViewModel::getCopyDirPath arg invalid:";
        return "";
    }
    QString dstDirPath = "";
    bool isFind = false;
    for(int i = 0; i < WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST.count(); i++){
        if (targetFileName == WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST.at(i)){
            dstDirPath = mImagesDirPath;
            isFind = true;
            *pIsRename = true;
            *pLogoType = (int)LogoType::LogoType1;
            break;
        }
    }
    if (!isFind){
        for(int i = 0; i < WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST.count(); i++){
            if (targetFileName == WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST.at(i)){
                dstDirPath = mCommonImagesDirPath;
                isFind = true;
                *pIsRename = true;
                *pLogoType = (int)LogoType::LogoType2;
                break;
            }
        }
    }
    if (!isFind){
        for(int i = 0; i < WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST.count(); i++){
            if (targetFileName == WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST.at(i)){
                dstDirPath = mAplExistDirPath;
                isFind = true;
                *pIsRename = false;
                *pLogoType = (int)LogoType::LogoTypeInvalid;
                break;
            }
        }
    }
    if (!isFind){
        for(int i = 0; i < WRITE_FROM_USB_PEN_MSG_LIST.count(); i++){
            if (targetFileName == WRITE_FROM_USB_PEN_MSG_LIST.at(i)){
                dstDirPath = mPenMsgExistDirPath;
                isFind = true;
                *pIsRename = false;
                *pLogoType = (int)LogoType::LogoTypeInvalid;
                break;
            }
        }
    }
    if (!isFind){
        for(int i = 0; i < WRITE_FROM_USB_PEN_SET_LIST.count(); i++){
            if (targetFileName == WRITE_FROM_USB_PEN_SET_LIST.at(i)){
                dstDirPath = mPenSetExistDirPath;
                isFind = true;
                *pIsRename = false;
                *pLogoType = (int)LogoType::LogoTypeInvalid;
                break;
            }
        }
    }
    if (!isFind){
        for(int i = 0; i < WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST.count(); i++){
            if (targetFileName == WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST.at(i)){
                dstDirPath = mSysSetAplExistDirPath;
                isFind = true;
                *pIsRename = false;
                *pLogoType = (int)LogoType::LogoTypeInvalid;
                break;
            }
        }
    }
    if (!isFind){
        for(int i = 0; i < WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST.count(); i++){
            if (targetFileName == WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST.at(i)){
                dstDirPath = mCoreLibExistDirPath;
                isFind = true;
                *pIsRename = false;
                *pLogoType = (int)LogoType::LogoTypeInvalid;
                break;
            }
        }
    }
    qDebug("%s:%d pIsRename[%d] pLogoType[%d]", __func__, __LINE__, *pIsRename, *pLogoType);
    return dstDirPath;
}

void ExternalMemoryViewModel::terminateFtpTransProcess(int ftpTransMode)
{
    qDebug("%s:%d mUsbDirection[%d] ftpTransMode[%d]", __func__, __LINE__, mUsbDirection, ftpTransMode);
    switch(ftpTransMode){
    case (int)FtpTransMode::FtpTransModePut:
        if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionFromUsb){
            mTimerCnt = 0;
            mIsTimerTriggerCheck = false;
            timerStop();
            disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
            disconnect(mFtpManager, SIGNAL(ftpListIndexChanged(int)), this, SLOT(onFtpListIndexChanged(int)));
            disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
//            mFtpManager->setIsDeterNoopCmd(false);
            if(checkCondFile(mCurCopyFile)){
                m_isNeedUpdateSetupData = true;
                setIsNeedUpdateSetupData(m_isNeedUpdateSetupData);
            }
            // Ftp-List再取り寄せ
            m_isFtpFileAcquired = false;
            emit isFtpFileAcquiredChanged(m_isFtpFileAcquired);
            if (mWriteTargetFromUsb != (int)Enums::UsbDownloadType::ControllerUpdate){
                m_isRequestGetDandoriData = true;
                getFtpFileList();
                m_isRequestFtpFileInfo = true;
                emit isRequestFtpFileInfoChanged(m_isRequestFtpFileInfo);
            }
#ifdef REAL_TARGET
            int ret = system("sync");
            (void)ret;
#endif // REAL_TARGET
            unlockFtpPutMutex();
        }
        break;
    case (int)FtpTransMode::FtpTransModeGet:
        if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionToUsb){
            mTimerCnt = 0;
            mIsTimerTriggerCheck = false;
            timerStop();
            disconnect(mFtpManager, SIGNAL(ftpExtMemFinishNotify(bool)), this, SLOT(onFtpExtMemFinishNotify(bool)));
            disconnect(mFtpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)));
            disconnect(mFtpManager, SIGNAL(ftpListIndexChanged(int)), this, SLOT(onFtpListIndexChanged(int)));
            disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
            disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
            disconnect(mFtpManager, SIGNAL(ftpFileSizeNotify(int)), this, SLOT(onFtpFileSizeNotify(int)));
#ifdef REAL_TARGET
            int ret = system("sync");
            (void)ret;
#endif // REAL_TARGET
            unlockFtpGetMutex();
        }
        break;
    default:
        break;
    }
}

void ExternalMemoryViewModel::downloadFromUsb(QString targetDirName)
{
    bool ret = true;
    switch(mWriteTargetFromUsb){
    case (int)Enums::UsbDownloadType::PenUpdate:
        mTargetDirName = targetDirName;
        asyncDownloadToSd("", "", "");
        break;
    case (int)Enums::UsbDownloadType::ControllerUpdate:
        mTargetDirName = targetDirName;
        downloadControllerUpdateData(targetDirName);
        break;
    case (int)Enums::UsbDownloadType::NcpData:
        mTargetDirName = targetDirName;
        downloadNcpData(targetDirName);
        break;
    case (int)Enums::UsbDownloadType::PenMsgData:
        mTargetDirName = targetDirName;
        asyncDownloadToSd("", "", "");
        break;
    case (int)Enums::UsbDownloadType::PenSetData:
        mTargetDirName = targetDirName;
        ret = downloadPenSetData(targetDirName);
        if (ret == true){
            qDebug("%s:%d downloadPenSetData success ret[%d]", __func__, __LINE__, ret);
            goto sd_process_success;
        }
        break;
    case (int)Enums::UsbDownloadType::BackupDateData:
        mTargetDirName = targetDirName;
        downloadBkDateData(targetDirName);
        break;
     case (int)Enums::UsbDownloadType::ImageData:
        mTargetDirName = targetDirName;
        asyncDownloadToSd("", "", "");
        break;
    default:
        qCritical() << "ExternalMemoryViewModel::downloadFromUsb mWriteTargetFromUsb:" << mWriteTargetFromUsb << " is invalid";
        break;
    }
    return;
sd_process_success:
    mWriteTargetFromUsb = -1;
    m_requestPowerRestart = true;
    emit requestPowerRestartChanged(m_requestPowerRestart);
    return;
}

void ExternalMemoryViewModel::checkTargetDir(QString& targetDirName)
{
    int i = 0;
    qDebug() << "ExternalMemoryViewModel::checkTargetDir targetDirName:" << targetDirName;
    for(i = 0; i < WRITE_FROM_USB_TARGET_FOLDER_LIST.count(); i++){
        if(i == 5){
            mWriteTargetFromUsb = 7;
            break;
        } else if(targetDirName == WRITE_FROM_USB_TARGET_FOLDER_LIST.at(i)){
            mWriteTargetFromUsb = i;
            break;
        }
    }
    if (i >= WRITE_FROM_USB_TARGET_FOLDER_LIST.count()){
        if(targetDirName.length() == 14 && targetDirName.contains("backup")){
            mWriteTargetFromUsb = (int)Enums::UsbDownloadType::BackupDateData;
        }
    }
}

void ExternalMemoryViewModel::checkTargetFile(QString& targetFileName)
{
    int i = 0;
    qDebug() << "ExternalMemoryViewModel::checkTargetFile targetFileName:" << targetFileName;
    for(i = 0; i < WRITE_FROM_USB_TARGET_FILE_LIST.count(); i++){
        if(targetFileName == WRITE_FROM_USB_TARGET_FILE_LIST.at(i)){
            break;
        }
    }
    if (i < WRITE_FROM_USB_TARGET_FILE_LIST.count()){
        mWriteTargetFromUsb = (int)Enums::UsbDownloadType::ComSpeedSetting;
    }
}

QString ExternalMemoryViewModel::renameNcpFile(QString copyFileName, QString srcFilePath)
{
    QString ret = "";
    if (copyFileName != mRenameNcpFileName){
        QFile file(srcFilePath + mRenameNcpFileName);
        if (file.exists()){
            qDebug() << srcFilePath + mRenameNcpFileName << " is exists:";
            QString renameFileName = mRenameNcpFileName + ".tmp";
            bool result = file.rename(srcFilePath + renameFileName);
            (void)result;
            mUndoNcpFileName = true;
            mRemoveRenameFile = false;
        } else {
            mRemoveRenameFile = true;
        }
        if (!QFile::copy((srcFilePath + copyFileName), (srcFilePath + mRenameNcpFileName))) {
            qCritical() << "rename failed:" << srcFilePath + copyFileName << "to" << srcFilePath + mRenameNcpFileName;
            return ret;
        }
#ifdef REAL_TARGET
        int result = system("sync");
        (void)result;
#endif // REAL_TARGET
    }
    ret = mRenameNcpFileName;
    qDebug() << "ExternalMemoryViewModel::renameNcpFile ret:" << ret << "copyFileName:" << copyFileName << "srcFilePath:" << srcFilePath;
    return ret;
}

QString ExternalMemoryViewModel::renameLogoFile(QString copyFileName, QString srcFilePath, int logoType)
{
    if (logoType < (int)LogoType::LogoType1 || logoType > (int)LogoType::LogoType2){
        qCritical() << "ExternalMemoryViewModel::renameLogoFile logoType is invalid:" << logoType;
        return "";
    }
    QString ret = "";
    QString renameFile = (logoType == (int)LogoType::LogoType1) ? mRenameLogo1FileName : mRenameLogo2FileName;
    QFile file(srcFilePath + renameFile);
    if (file.exists()){
        qDebug() << srcFilePath + renameFile << " is exists:";
        file.remove();
    }
    if (!QFile::copy((srcFilePath + copyFileName), (srcFilePath + renameFile))) {
        qCritical() << "rename failed:" << srcFilePath + copyFileName << "to" << srcFilePath + renameFile;
        return ret;
    }
    ret = renameFile;
    qDebug() << "ExternalMemoryViewModel::renameLogoFile ret:" << ret << "copyFileName:" << copyFileName << "srcFilePath:" << srcFilePath << "logoType:" << logoType;
    return ret;
}

bool ExternalMemoryViewModel::removeRenameFile(QString fileName, QString srcFilePath)
{
    qDebug() << "removeRenameFile srcFilePath:" << srcFilePath << "fileName:" << fileName;
    bool ret = QFile::remove(srcFilePath + fileName);
    if (!ret){
        qCritical() << "removeRenameFile failed:" << srcFilePath + fileName;
    }
    return ret;
}

bool ExternalMemoryViewModel::undoNcpFileName(QString fileName, QString srcFilePath)
{
    qDebug() << "undoNcpFileName srcFilePath:" << srcFilePath << "fileName:" << fileName;
    bool ret = true;
    QFile file(srcFilePath + fileName + ".tmp");
    if (file.exists()){
        ret = file.rename(srcFilePath + mRenameNcpFileName);
    }
    return ret;
}

bool ExternalMemoryViewModel::checkEffectivePowerRestart(QString targetFileName)
{
    qDebug() << "checkEffectivePowerRestart targetFileName:" << targetFileName;
    bool isPowerRestart = false;
    for(int i = 0; i < WRITE_FROM_USB_REQUIRED_POWER_RESTART_FILE_LIST.count(); i++){
        if (targetFileName == WRITE_FROM_USB_REQUIRED_POWER_RESTART_FILE_LIST.at(i)){
            isPowerRestart = true;
            break;
        }
    }
    return isPowerRestart;
}

void ExternalMemoryViewModel::setRemoteDirFileCnt(int remoteDirFileCnt)
{
    if (m_remoteDirFileCnt == remoteDirFileCnt){
        return;
    }
    m_remoteDirFileCnt = remoteDirFileCnt;
    emit remoteDirFileCntChanged(m_remoteDirFileCnt);
}

void ExternalMemoryViewModel::setUsbDirFileCnt(int usbDirFileCnt)
{
    if (m_usbDirFileCnt == usbDirFileCnt){
        return;
    }
    m_usbDirFileCnt = usbDirFileCnt;
    emit usbDirFileCntChanged(m_usbDirFileCnt);
}

void ExternalMemoryViewModel::setUsbChildDirFileCnt(int usbChildDirFileCnt)
{
    if (m_usbChildDirFileCnt == usbChildDirFileCnt){
        return;
    }
    m_usbChildDirFileCnt = usbChildDirFileCnt;
    emit usbChildDirFileCntChanged(m_usbChildDirFileCnt);
}

void ExternalMemoryViewModel::setIsUSBConnected(bool isUSBConnected)
{
    m_isUSBConnected = isUSBConnected;
    emit isUSBConnectedChanged(m_isUSBConnected);
}

void ExternalMemoryViewModel::setFirstUsbConnected(bool firstUsbConnected)
{
    if (m_firstUsbConnected == firstUsbConnected){
        return;
    }
    m_firstUsbConnected = firstUsbConnected;
    emit firstUsbConnectedChanged(m_firstUsbConnected);
}

void ExternalMemoryViewModel::setIsAlreadyUsbFileList(bool isAlreadyUsbFileList)
{
    m_isAlreadyUsbFileList = isAlreadyUsbFileList;
    emit isAlreadyUsbFileListChanged(m_isAlreadyUsbFileList);
}

void ExternalMemoryViewModel::setIsAlreadyUsbChildFileList(bool isAlreadyUsbChildFileList)
{
    m_isAlreadyUsbChildFileList = isAlreadyUsbChildFileList;
    emit isAlreadyUsbChildFileListChanged(m_isAlreadyUsbChildFileList);
}

void ExternalMemoryViewModel::setErrCode(QString errCode)
{
    if (m_errCode == errCode){
        return;
    }
    m_errCode = errCode;
    emit errCodeChanged(m_errCode);
}

void ExternalMemoryViewModel::setCurTabIndex(int curTabIndex)
{
    if (m_curTabIndex == curTabIndex){
        return;
    }
    m_curTabIndex = curTabIndex;
    emit curTabIndexChanged(m_curTabIndex);
}

void ExternalMemoryViewModel::setIsNeedUpdateSetupData(bool isNeedUpdateSetupData)
{
    if (m_isNeedUpdateSetupData == isNeedUpdateSetupData){
        return;
    }
    m_isNeedUpdateSetupData = isNeedUpdateSetupData;
    emit isNeedUpdateSetupDataChanged(m_isNeedUpdateSetupData);
}

qint64 ExternalMemoryViewModel::getFileSize(const QString& filePath)
{
    QFile file(filePath);
    qint64 fileSize = file.size();
    return fileSize;
}

int ExternalMemoryViewModel::copyToController(QString copyFileName, QString dstFilePath, QString srcFilePath, int targetListIndex)
{
    qDebug() << "copyToController copyFileName:" << copyFileName << "dstFilePath:" << dstFilePath << "srcFilePath:" << srcFilePath << "targetListIndex:" << targetListIndex;
    if (!m_isUSBConnected || !mFtpManager){
        qCritical("%s:%d usb is not connect m_isUSBConnected[%d] or mFtpManager[%p] is null",
            __func__, __LINE__, m_isUSBConnected, mFtpManager);
        return -1;
    }
    mPutFile = new QFile(srcFilePath + copyFileName);
    if (!mPutFile->exists()){
        qCritical() << "srcFilePath:" << srcFilePath + "/" << "copyFileName:" << copyFileName << " is not exist";
        return -1;
    }
    mPutFile->open(QFile::ReadOnly);
    int targetFileSize = (int)(getFileSize(srcFilePath + copyFileName));
    // FTP(ALLO)
    int result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandALLO, nullptr, nullptr, targetFileSize);
    result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandCWD, nullptr, dstFilePath);
    result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandSTOR, mPutFile, copyFileName);        // Ftp put
    stFtpPutArgument arg;
    arg.putFile = mPutFile;
    arg.selectedIndex = targetListIndex;
    arg.targetListNum = mTargetListNum;
    mFtpManager->setFtpPutArgment(result, arg);
    mUsbDirection = (int)CopyDirectionStatus::CopyDirectionFromUsb;
    mCurCopyFile = copyFileName;
    mCurSrcFilePath = srcFilePath;
    connect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
    connect(mFtpManager, SIGNAL(ftpListIndexChanged(int)), this, SLOT(onFtpListIndexChanged(int)));
    connect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
    timerStart(EXT_MEM_COPY_TIMER_INTERVAL);
    return result;
}

int ExternalMemoryViewModel::copyToUsb(QString copyFileName, QString dstFilePath, QString srcFilePath, int listIndex, qint64 fileSize)
{
    qDebug() << "copyToUsb copyFileName:" << copyFileName << "dstFilePath:" << dstFilePath << "srcFilePath:" << srcFilePath << "fileSize" << fileSize;
    if ((!m_isUSBConnected && !m_firstUsbConnected) || !mFtpManager){
        qCritical("%s:%d usb is not connect m_isUSBConnected[%d] m_firstUsbConnected[%d] or mFtpManager[%p] or is null",
            __func__, __LINE__, m_isUSBConnected, m_firstUsbConnected, mFtpManager);
        return -1;
    }
    mGetFile = new QFile(dstFilePath + copyFileName);
    if(mGetFile->exists()) {
        // target already exists in USB
        mGetFile->remove();
    }
    mGetFile->open(QFile::WriteOnly);
    int result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandCWD, nullptr, srcFilePath);
    //qDebug("%s:%d CWD[%d]", __func__, __LINE__, result);
    result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandRETR, mGetFile, copyFileName);        // Ftp get
    //qDebug("%s:%d RETR[%d]", __func__, __LINE__, result);
    stFtpGetArgument arg;
    arg.getFile = mGetFile;
    arg.selectedIndex = listIndex;
    arg.isToExtMem = true;
    mFtpManager->setFtpGetArgment(result, arg);
    mUsbDirection = (int)CopyDirectionStatus::CopyDirectionToUsb;
    mCurCopyFile = copyFileName;
    mCurCopyFileSize = fileSize;
    connect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
    connect(mFtpManager, SIGNAL(ftpListIndexChanged(int)), this, SLOT(onFtpListIndexChanged(int)));
    connect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
    connect(mFtpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)), Qt::QueuedConnection);
    connect(mFtpManager, SIGNAL(ftpExtMemFinishNotify(bool)), this, SLOT(onFtpExtMemFinishNotify(bool)), Qt::QueuedConnection);
    connect(mFtpManager, SIGNAL(ftpFileSizeNotify(int)), this, SLOT(onFtpFileSizeNotify(int)), Qt::QueuedConnection);
    timerStart(EXT_MEM_COPY_TIMER_INTERVAL);
    return result;
}

void ExternalMemoryViewModel::checkFtpTimeoutProcess()
{
    if (m_ftpListIndex <= 0){
        if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionToUsb){
#ifdef REAL_TARGET
            int ret = system("sync");
            (void)ret;
#endif // REAL_TARGET
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
            m_preFtpListIndex = -1;
            m_ftpListIndex = -1;
            mWriteTargetFromUsb = -1;
            m_processingResult = false;
            emit processingResultChanged(m_processingResult);
            mFtpManager->setIsDeterNoopCmd(false);
        } else {
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
            m_preFtpListIndex = -1;
            m_ftpListIndex = -1;
            mWriteTargetFromUsb = -1;
            m_processingResult = false;
            emit processingResultChanged(m_processingResult);
            mFtpManager->setIsDeterNoopCmd(false);
        }
    } else {
        if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionToUsb){
#ifdef REAL_TARGET
            int ret = system("sync");
            (void)ret;
#endif // REAL_TARGET
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
            m_preFtpListIndex = m_ftpListIndex;
            nextWriteInBulkToUsb(m_ftpListIndex - 1);
        } else {
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
            m_preFtpListIndex = m_ftpListIndex;
            if (m_ftpListIndex < mTargetListNum - 1) {
                if (mWriteTargetFromUsb == (int)Enums::UsbDownloadType::BackupDateData){
                    downloadBkDateData(mTargetDirName, (m_ftpListIndex + 1));
                } else if (mWriteTargetFromUsb == (int)Enums::UsbDownloadType::ControllerUpdate){
                    downloadControllerUpdateData(mTargetDirName, (m_ftpListIndex + 1));
                }
            }
        }
    }
}

void ExternalMemoryViewModel::checkFtpGetProcess()
{
    QFile usbFile(mUsbRootPath + mCurUsbFolder + "/" + mCurCopyFile);
    ExternalMemoryViewFileList *extFtpControllerFileList = static_cast<ExternalMemoryViewFileList *>(m_extFtpControllerFileList.at(m_ftpListIndex));
    qint64 fileSize = extFtpControllerFileList->fileSize();
    if (mWriteTargetFromUsb >= 0){
        if (usbFile.size() >= fileSize){
#ifdef REAL_TARGET
            int ret = system("sync");
            (void)ret;
#endif // REAL_TARGET
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
            m_preFtpListIndex = m_ftpListIndex;
            nextWriteInBulkToUsb(m_ftpListIndex - 1);
        }
    } else {
        if (m_isNotifyFtpGetSize){
            if (mIsMatchFileSize){
                if (usbFile.size() == fileSize){
#ifdef REAL_TARGET
                    int ret = system("sync");
                    (void)ret;
#endif // REAL_TARGET
                    terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
                    m_preFtpListIndex = -1;
                    m_ftpListIndex = -1;
                    mWriteTargetFromUsb = -1;
                    m_isNotifyFtpGetSize = false;
                    mIsMatchFileSize = false;
                    m_processingResult = true;
                    emit processingResultChanged(m_processingResult);
                    mFtpManager->setIsDeterNoopCmd(false);
                } else {
#ifdef REAL_TARGET
                    int ret = system("sync");
                    (void)ret;
#endif // REAL_TARGET
                    terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
                    m_preFtpListIndex = -1;
                    m_ftpListIndex = -1;
                    mWriteTargetFromUsb = -1;
                    m_isNotifyFtpGetSize = false;
                    mIsMatchFileSize = false;
                    if (usbFile.exists()) { usbFile.remove(); }
#ifdef REAL_TARGET
                    ret = system("sync");
#endif // REAL_TARGET
                    m_processingResult = false;
                    emit processingResultChanged(m_processingResult);
                    mFtpManager->setIsDeterNoopCmd(false);
                }
            } else {
#ifdef REAL_TARGET
                int ret = system("sync");
                (void)ret;
#endif // REAL_TARGET
                terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
                m_preFtpListIndex = -1;
                m_ftpListIndex = -1;
                mWriteTargetFromUsb = -1;
                m_isNotifyFtpGetSize = false;
                mIsMatchFileSize = false;
                if (usbFile.exists()) { usbFile.remove(); }
#ifdef REAL_TARGET
                ret = system("sync");
#endif // REAL_TARGET
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
                mFtpManager->setIsDeterNoopCmd(false);
            }
        }
    }  
}

void ExternalMemoryViewModel::checkFtpPutProcess()
{
    if (m_ftpListIndex < mTargetListNum - 1) {
        if (m_preFtpListIndex != m_ftpListIndex){
            terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
            m_preFtpListIndex = m_ftpListIndex;
            if (mWriteTargetFromUsb == (int)Enums::UsbDownloadType::BackupDateData){
                downloadBkDateData(mTargetDirName, (m_ftpListIndex + 1));
            } else if (mWriteTargetFromUsb == (int)Enums::UsbDownloadType::ControllerUpdate){
                downloadControllerUpdateData(mTargetDirName, (m_ftpListIndex + 1));
            }
        }
    } else {
        terminateFtpTransProcess((int)FtpTransMode::FtpTransModePut);
        m_preFtpListIndex = -1;
        m_ftpListIndex = -1;
        mWriteTargetFromUsb = -1;
        m_processingResult = true;
        emit processingResultChanged(m_processingResult);
        mFtpManager->setIsDeterNoopCmd(false);
        if(mCurCopyFile == mRenameNcpFileName){
            m_complieState = 1;
            emit complieStateChanged(m_complieState);
            connect(mProgramFileManager, SIGNAL(compileResultChanged(bool)), this, SLOT(onCompileResultChanged(bool)));
            mProgramFileManager->requestCompile();
        }
        if(mIsPowerRestart) {
            m_requestPowerRestart = true;
            emit requestPowerRestartChanged(m_requestPowerRestart);
            mIsPowerRestart = false;
#ifdef REAL_TARGET
            int ret = system("sync");
            (void)ret;
#endif // REAL_TARGET
        }
    }
}

void ExternalMemoryViewModel::startAsyncFileTrasnfer()
{
    if (!mTransferFileThread){
        qCritical("%s:%d mTransferFileThread is invalid", __func__, __LINE__);
        return;
    }
    mTransferFileThread->setThreadOpeEnable(true);
    connect(mTransferFileThread, SIGNAL(fileTransCompleteChanged(bool)), this, SLOT(onFileTransCompleteChanged(bool)), Qt::DirectConnection);
}

void ExternalMemoryViewModel::stopAsyncFileTrasnfer()
{
    if (!mTransferFileThread){
        qCritical("%s:%d mTransferFileThread is invalid", __func__, __LINE__);
        return;
    }
    mTransferFileThread->setThreadOpeEnable(false);
    disconnect(mTransferFileThread, SIGNAL(fileTransCompleteChanged(bool)), this, SLOT(onFileTransCompleteChanged(bool)));
}

void ExternalMemoryViewModel::singleTimerStart(int timeInterval)
{
    //qDebug("%s:%d timeInterval[%d]", __func__, __LINE__, timeInterval);
    QTimer::singleShot(timeInterval, this, &ExternalMemoryViewModel::onUpdateSingleTimer);
}

void ExternalMemoryViewModel::timerStart(int timeOut)
{
    if (!mIsTimerStatus) {
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void ExternalMemoryViewModel::timerStop()
{
    if (mIsTimerStatus) {
        killTimer(mTimerId);
        mTimerCnt = 0;
        mIsTimerStatus = false;
    }
}

void ExternalMemoryViewModel::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent || !mFtpManager) return;
    if(timerEvent->timerId() == mTimerId){
        if (mExtMemCopyTimeOutCnt > 0){
            mTimerCnt++;
            if (mTimerCnt >= mExtMemCopyTimeOutCnt){
                checkFtpTimeoutProcess();
            } else {
                if (mIsTimerTriggerCheck){
                    checkFtpGetProcess();
                } else if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionFromUsb
                    && mFtpManager->getFtpConnectStatus() == (int)Enums::FtpStatus::FtpStatusPutFinished){
                    checkFtpPutProcess();
                }
            }
        } else {
            // タイムアウト設定なし
            if (mIsTimerTriggerCheck){
                checkFtpGetProcess();
            } else if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionFromUsb
                && mFtpManager->getFtpConnectStatus() == (int)Enums::FtpStatus::FtpStatusPutFinished){
                checkFtpPutProcess();
            }
        }
    }
    return;
}

void ExternalMemoryViewModel::lockFtpPutMutex()
{
    if (mMutexFtpPut){
        mMutexFtpPut->lock();
        mIsMutexFtpPut = true;
    }
}

void ExternalMemoryViewModel::unlockFtpPutMutex()
{
    if (mMutexFtpPut){
        if (mIsMutexFtpPut) {
            mMutexFtpPut->unlock();
        }
        mIsMutexFtpPut = false;
    }
}

void ExternalMemoryViewModel::lockFtpGetMutex()
{
    if (mMutexFtpGet){
        mMutexFtpGet->lock();
        mIsMutexFtpGet = true;
    }
}

void ExternalMemoryViewModel::unlockFtpGetMutex()
{
    if (mMutexFtpGet){
        if (mIsMutexFtpGet) {
            mMutexFtpGet->unlock();
        }
        mIsMutexFtpGet = false;
    }
}

void ExternalMemoryViewModel::lockCursorOpeMutex()
{
    if (mMutexCursorOpe){
        mMutexCursorOpe->lock();
        mIsMutexCursorOpe = true;
    }
}

void ExternalMemoryViewModel::unlockCursorOpeMutex()
{
    if (mMutexCursorOpe){
        if (mIsMutexCursorOpe) {
            mMutexCursorOpe->unlock();
        }
        mIsMutexCursorOpe = false;
    }
}

void ExternalMemoryViewModel::onFtpStatusChanged(int ftpStatus)
{
    //qDebug("ExternalMemoryViewModel %s:%d ftpStatus[%d]", __func__, __LINE__, ftpStatus);
    if (ftpStatus == (int)Enums::FtpStatus::FtpStatusGetFinished){
//        timerStop();
//        m_processingResult = true;
//        emit processingResultChanged(m_processingResult);
    } else if ((int)Enums::FtpStatus::FtpStatusPutFinished){
        //qDebug() << "ExternalMemoryViewModel::onFtpStatusChanged mCurCopyFile:" << mCurCopyFile << "mCurSrcFilePath:"
        //    << mCurSrcFilePath << "mWriteTargetFromUsb:" << mWriteTargetFromUsb << "mUndoNcpFileName:" << mUndoNcpFileName;
        if (mCurCopyFile == mRenameNcpFileName && (mWriteTargetFromUsb != -1 && mWriteTargetFromUsb != (int)Enums::UsbDownloadType::NcpData)
            && mUndoNcpFileName != true && (mWriteTargetFromUsb != (int)Enums::UsbDownloadType::ImageData && mWriteTargetFromUsb != (int)Enums::UsbDownloadType::BackupDateData)){
            removeRenameFile(mCurCopyFile, mCurSrcFilePath);
        }
    }
//    disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
}

void ExternalMemoryViewModel::onCompileResultChanged(bool result)
{
    if(result){
        m_complieState = 2;
        emit complieStateChanged(m_complieState);
    }else {
        m_complieState = 0;
        emit complieStateChanged(m_complieState);
    }
    disconnect(mProgramFileManager, SIGNAL(compileResultChanged(bool)), this, SLOT(onCompileResultChanged(bool)));
}

void ExternalMemoryViewModel::onFtpListIndexChanged(int ftpListIndex)
{
    m_ftpListIndex = ftpListIndex;
}

void ExternalMemoryViewModel::onFtpErrorStrNotify(QString errorStr)
{
    qCritical() << "onFtpErrorStrNotify errorStr:" << errorStr;
    qCritical("%s:%d ftp process error mTimerCnt[%d] mUsbDirection[%d] m_preFtpListIndex[%d] m_ftpListIndex[%d] mTargetListNum[%d]",
        __func__, __LINE__, mTimerCnt, mUsbDirection, m_preFtpListIndex, m_ftpListIndex, mTargetListNum);
    timerStop();
    if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionToUsb){
        disconnect(mFtpManager, SIGNAL(ftpFileSizeNotify(int)), this, SLOT(onFtpFileSizeNotify(int)));
        disconnect(mFtpManager, SIGNAL(ftpExtMemFinishNotify(bool)), this, SLOT(onFtpExtMemFinishNotify(bool)));
        disconnect(mFtpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)));
        disconnect(mFtpManager, SIGNAL(ftpListIndexChanged(int)), this, SLOT(onFtpListIndexChanged(int)));
    } else {
        disconnect(mFtpManager, SIGNAL(ftpListIndexChanged(int)), this, SLOT(onFtpListIndexChanged(int)));
    }
    disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
    disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
    if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionToUsb){
        unlockFtpGetMutex();
    } else {
        unlockFtpPutMutex();
    }
    if (mUsbDirection == (int)CopyDirectionStatus::CopyDirectionToUsb){
        m_ftpListIndex = -1;
        mWriteTargetFromUsb = -1;
        m_processingResult = false;
        emit processingResultChanged(m_processingResult);
        mFtpManager->setIsDeterNoopCmd(false);
    } else {
        if (mUndoNcpFileName){
            QString tmpNcpFileName = mRenameNcpFileName;
            removeRenameFile(tmpNcpFileName, mSrcDirPath);
            undoNcpFileName(mTargetFileName, mSrcDirPath);
            mUndoNcpFileName = false;
        }
        // if (mTargetListNum > 1){
        //     if (m_ftpListIndex >= (mTargetListNum - 1)){
        //         m_ftpListIndex = -1;
        //         mWriteTargetFromUsb = -1;
        //         m_processingResult = false;
        //         emit processingResultChanged(m_processingResult);
        //         mFtpManager->setIsDeterNoopCmd(false);
        //     } else {
        //         if (mWriteTargetFromUsb == (int)Enums::UsbDownloadType::BackupDateData){
        //             downloadBkDateData(mTargetDirName, (m_ftpListIndex + 1));
        //         } else if (mWriteTargetFromUsb == (int)Enums::UsbDownloadType::ControllerUpdate){
        //             downloadControllerUpdateData(mTargetDirName, (m_ftpListIndex + 1));
        //         }
        //     }
        // } else {
            m_ftpListIndex = -1;
            mWriteTargetFromUsb = -1;
            m_processingResult = false;
            emit processingResultChanged(m_processingResult);
            mFtpManager->setIsDeterNoopCmd(false);
        // }
    }

}

void ExternalMemoryViewModel::onFtpGetFinished(const QString &filePath)
{
    //qDebug() << "ExternalMemoryViewModel::onFtpGetFinished filePath:" << filePath;
    (void)filePath;
}

void ExternalMemoryViewModel::onFtpExtMemFinishNotify(bool isToExtMem)
{
    if (isToExtMem){
        int curFtpListIndex = m_ftpListIndex;
        if (curFtpListIndex > 0 && m_preFtpListIndex != curFtpListIndex){
            QFile usbFile(mUsbRootPath + mCurUsbFolder + "/" + mCurCopyFile);
            if (usbFile.exists()){
                if (mWriteTargetFromUsb >= 0){
#ifdef REAL_TARGET
                    int ret = system("sync");
                    (void)ret;
#endif // REAL_TARGET
                    terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
                    m_preFtpListIndex = curFtpListIndex;
                    nextWriteInBulkToUsb(curFtpListIndex - 1);
                } else {
#ifdef REAL_TARGET
                    int ret = system("sync");
                    (void)ret;
#endif // REAL_TARGET
                    terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
                    m_preFtpListIndex = -1;
                    m_ftpListIndex = -1;
                    mWriteTargetFromUsb = -1;
                    m_isNotifyFtpGetSize = false;
                    mIsMatchFileSize = false;
                    m_processingResult = true;
                    emit processingResultChanged(m_processingResult);
                    mFtpManager->setIsDeterNoopCmd(false);
                }
            }
        } else {
            if (curFtpListIndex <= 0){
#ifdef REAL_TARGET
                int ret = system("sync");
                (void)ret;
#endif // REAL_TARGET
                terminateFtpTransProcess((int)FtpTransMode::FtpTransModeGet);
                m_preFtpListIndex = -1;
                m_ftpListIndex = -1;
                mWriteTargetFromUsb = -1;
                m_isNotifyFtpGetSize = false;
                m_processingResult = true;
                emit processingResultChanged(m_processingResult);
                mFtpManager->setIsDeterNoopCmd(false);
            }
        }
    }
}

void ExternalMemoryViewModel::onFtpFileSizeNotify(int fileSize)
{
    //qDebug("%s:%d fileSize[%d]", __func__, __LINE__, fileSize);
    m_isNotifyFtpGetSize = true;
    mIsMatchFileSize = (mCurCopyFileSize == fileSize);
    //qDebug("%s:%d mIsMatchFileSize[%d]", __func__, __LINE__, mIsMatchFileSize);
}

void ExternalMemoryViewModel::onUpdateSingleTimer()
{
    qDebug() << "onUpdateSingleTimer mWriteTargetFromUsb:" << mWriteTargetFromUsb << "mTargetDirName:" << mTargetDirName;
    qDebug() << "onUpdateSingleTimer mTargetDirPath:" << mTargetDirPath << "mSrcDirPath:" << mSrcDirPath << "mTargetFileName:" << mTargetFileName;
#ifdef ENABLE_ASYNC_COPY_TO_SD_FROM_USB
    if(!mTransferFileThread){
        qCritical() << "ExternalMemoryViewModel::onUpdateSingleTimer mTransferFileThread is invalid:";
        return;
    }
#endif // ENABLE_ASYNC_COPY_TO_SD_FROM_USB
    CoreLibSdControl coreLibSdCtrl;
    bool ret = false;
    QFile::Permissions permissions =
        (QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
         QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ExeUser |
         QFileDevice::ReadGroup | QFileDevice::ExeGroup |
         QFileDevice::ReadOther | QFileDevice::ExeOther);
#ifdef ENABLE_ASYNC_COPY_TO_SD_FROM_USB
    stFileTransInfo transInfo;
    initFileTransInfo(transInfo);
#endif // ENABLE_ASYNC_COPY_TO_SD_FROM_USB

    switch(mWriteTargetFromUsb){
        case (int)Enums::UsbDownloadType::PenUpdate:{
#ifdef ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            if (mTargetDirName == ""){
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = mSrcDirPath;
                transInfo.dstFilePath = mTargetDirPath;
                transInfo.targetDirName = mTargetDirName;
                transInfo.targetFileName = mTargetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = 0;
                transInfo.totalListNum = 1;
                mTransferFileThread->setFileTransInfo(transInfo);
                startAsyncFileTrasnfer();
            } else {
                m_curTransListIndex = 0;
                asyncDownloadPenUpdateData(mTargetDirName, m_curTransListIndex, true);
            }
#else // ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            if (mTargetDirName == ""){
                ret = coreLibSdCtrl.copyFile(mSrcDirPath + mTargetFileName, mTargetDirPath + mTargetFileName, true);
                if (!ret){
                    qCritical() << "ExternalMemoryViewModel::onUpdateSingleTimer copyFile failed:" << ret;
                    goto process_error;
                } else {
                    if(mRemoveRenameFile){
                        removeRenameFile(mTargetFileName, mSrcDirPath);
                        mRemoveRenameFile = false;
                    }
                    QFile dstFile(mTargetDirPath + mTargetFileName);
                    dstFile.setPermissions(permissions);
                    goto sd_process_success;
                }
            } else {
                ret = downloadPenUpdateData(mTargetDirName);
                if (ret == true){
                    goto sd_process_success;
                } else {
                    goto process_error;
                }
            }
#endif // ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            break;
        }
        case (int)Enums::UsbDownloadType::PenMsgData:{
#ifdef ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            if (mTargetDirName == ""){
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = mSrcDirPath;
                transInfo.dstFilePath = mTargetDirPath;
                transInfo.targetDirName = mTargetDirName;
                transInfo.targetFileName = mTargetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = 0;
                transInfo.totalListNum = 1;
                mTransferFileThread->setFileTransInfo(transInfo);
                startAsyncFileTrasnfer();
            } else {
                m_curTransListIndex = 0;
                asyncDownloadPenMsgData(mTargetDirName, m_curTransListIndex, true);
            }
#else // ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            if (mTargetDirName == ""){
                ret = coreLibSdCtrl.copyFile(mSrcDirPath + mTargetFileName, mTargetDirPath + mTargetFileName, true);
                if (!ret){
                    qCritical() << "ExternalMemoryViewModel::onUpdateSingleTimer copyFile failed:" << ret;
                    goto process_error;
                } else {
                    if(mRemoveRenameFile){
                        removeRenameFile(mTargetFileName, mSrcDirPath);
                        mRemoveRenameFile = false;
                    }
                    QFile dstFile(mTargetDirPath + mTargetFileName);
                    dstFile.setPermissions(permissions);
                    goto sd_process_success;
                }
            } else {
                ret = downloadPenMsgData(mTargetDirName);
                if (ret == true){
                    goto sd_process_success;
                } else {
                    goto process_error;
                }
            }
#endif // ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            break;
        }
        case (int)Enums::UsbDownloadType::NcpData:{
#ifdef ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            if (mTargetDirName == ""){
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = mSrcDirPath;
                transInfo.dstFilePath = mTargetDirPath;
                transInfo.targetDirName = mTargetDirName;
                transInfo.targetFileName = mTargetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = 0;
                transInfo.totalListNum = 1;
                mTransferFileThread->setFileTransInfo(transInfo);
                startAsyncFileTrasnfer();
            }
#else   // ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            if (mTargetDirName == ""){
                ret = coreLibSdCtrl.copyFile(mSrcDirPath + mTargetFileName, mTargetDirPath + mTargetFileName, true);
                if (!ret){
                    qCritical() << "ExternalMemoryViewModel::onUpdateSingleTimer copyFile failed:" << ret;
                    goto process_error;
                } else {
                    if(mRemoveRenameFile){
                        removeRenameFile(mTargetFileName, mSrcDirPath);
                        mRemoveRenameFile = false;
                    }
                    goto sd_process_success;
                }
            }
#endif // ENABLE_ASYNC_COPY_TO_SD_FROM_USB
            break;
        }
        case (int)Enums::UsbDownloadType::ImageData:{
            ret =  downloadImageData(mTargetDirName);
            if (ret == true){
                goto sd_process_success;
            } else {
                goto process_error;
            }
            break;
        }
        case (int)Enums::UsbDownloadType::ComSpeedSetting:{
            if (mTargetDirName == ""){
                transInfo.writeTargetFromUsb = mWriteTargetFromUsb;
                transInfo.srcFilePath = mSrcDirPath;
                transInfo.dstFilePath = mTargetDirPath;
                transInfo.targetDirName = mTargetDirName;
                transInfo.targetFileName = mTargetFileName;
                transInfo.isOverWrite = true;
                transInfo.curListIndex = 0;
                transInfo.totalListNum = 1;
                mTransferFileThread->setFileTransInfo(transInfo);
                startAsyncFileTrasnfer();
            }
            break;
        }
        default:{
            ret = coreLibSdCtrl.copyFile(mSrcDirPath + mTargetFileName, mTargetDirPath + mTargetFileName, true);
            if (!ret){
                qCritical() << "ExternalMemoryViewModel::onUpdateSingleTimer copyFile failed:" << ret;
                goto process_error;
            } else {
                if(mRemoveRenameFile){
                    if (mUndoNcpFileName != true){
                        removeRenameFile(mTargetFileName, mSrcDirPath);
                    }
                    mRemoveRenameFile = false;
                }
                QFile dstFile(mTargetDirPath + mTargetFileName);
                dstFile.setPermissions(permissions);
                goto sd_process_success;
            }
            break;
        }
    }
    return;
process_error:
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    return;
sd_process_success:
    mWriteTargetFromUsb = -1;
    mFtpManager->setIsDeterNoopCmd(false);
    m_processingResult = true;
    emit processingResultChanged(m_processingResult);
    if(mIsPowerRestart) {
        m_requestPowerRestart = true;
        emit requestPowerRestartChanged(m_requestPowerRestart);
        mIsPowerRestart = false;
#ifdef REAL_TARGET
        int ret = system("sync");
        (void)ret;
#endif // REAL_TARGET
    }
    return;
}

void ExternalMemoryViewModel::onFileTransCompleteChanged(bool fileTransComplete)
{
    QFile::Permissions permissions =
        (QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
         QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ExeUser |
         QFileDevice::ReadGroup | QFileDevice::ExeGroup |
         QFileDevice::ReadOther | QFileDevice::ExeOther);
    //qDebug() << "ExternalMemoryViewModel::onFileTransCompleteChanged fileTransComplete:" << fileTransComplete;
    if (fileTransComplete){
        if (mTargetDirName == ""){
            stopAsyncFileTrasnfer();
            goto trans_process_success;
        } else {
            if (m_curTransListIndex >= m_extUsbMemoryChildDirFileList.count() - 1){
                stopAsyncFileTrasnfer();
                mTransferFileThread->setIsCopying(false);
                goto trans_process_success;
            }
            if(mRemoveRenameFile){
                if (mUndoNcpFileName != true){
                    removeRenameFile(mTargetFileName, mSrcDirPath);
                }
                mRemoveRenameFile = false;
            } else {
                if (mUndoNcpFileName){
                    QString tmpNcpFileName = mRenameNcpFileName;
                    removeRenameFile(tmpNcpFileName, mSrcDirPath);
                    undoNcpFileName(mTargetFileName, mSrcDirPath);
                    mUndoNcpFileName = false;
                }
            }
            QFile dstFile(mTargetDirPath + mTargetFileName);
            dstFile.setPermissions(permissions);
            m_curTransListIndex++;
            switch(mWriteTargetFromUsb){
                case (int)Enums::UsbDownloadType::PenUpdate:{
                    asyncDownloadPenUpdateData(mTargetDirName, m_curTransListIndex);
                    break;
                }
                case (int)Enums::UsbDownloadType::PenMsgData:{
                    asyncDownloadPenMsgData(mTargetDirName, m_curTransListIndex);
                    break;
                }
                default:
                    break;
            }
        }
    } else {
        stopAsyncFileTrasnfer();
        mTransferFileThread->setIsCopying(false);
        mWriteTargetFromUsb = -1;
        mFtpManager->setIsDeterNoopCmd(false);
        m_processingResult = false;
        emit processingResultChanged(m_processingResult);
    }
    return;
trans_process_success:
    QString tmpNcpFileName = mRenameNcpFileName;
    if(mRemoveRenameFile){
        if (mUndoNcpFileName != true){
            removeRenameFile(mTargetFileName, mSrcDirPath);
        }
        mRemoveRenameFile = false;
        if (mUndoNcpFileName){
            removeRenameFile(tmpNcpFileName, mSrcDirPath);
            undoNcpFileName(mTargetFileName, mSrcDirPath);
            mUndoNcpFileName = false;
        }
    } else {
        if (mUndoNcpFileName){
            removeRenameFile(tmpNcpFileName, mSrcDirPath);
            undoNcpFileName(mTargetFileName, mSrcDirPath);
            mUndoNcpFileName = false;
        }
    }
    QFile dstFile(mTargetDirPath + mTargetFileName);
    dstFile.setPermissions(permissions);
    if(mWriteTargetFromUsb == (int)Enums::UsbDownloadType::NcpData){
        m_isNcpFileUpdate = true;
        emit isNcpFileUpdateChanged(m_isNcpFileUpdate);
    } else {
        m_processingResult = true;
        emit processingResultChanged(m_processingResult);
    }
    mWriteTargetFromUsb = -1;
    mFtpManager->setIsDeterNoopCmd(false);
    if(mIsPowerRestart) {
        m_requestPowerRestart = true;
        emit requestPowerRestartChanged(m_requestPowerRestart);
        mIsPowerRestart = false;
#ifdef REAL_TARGET
        int ret = system("sync");
        (void)ret;
#endif // REAL_TARGET
    } else {
#ifdef REAL_TARGET
        int ret = system("sync");
        (void)ret;
#endif // REAL_TARGET
    }
    m_isNcpFileUpdate = false;
    return;
}

void ExternalMemoryViewModel::reConnectCommunication(bool status)
{
    switch(m_curTabIndex){
    case 1: // USB-Export
        if (status){
            getFtpRemoteFileListCount();
        } else {
            clearFtpControllerViewFileList(0);
        }
        break;
    case 2: // USB-Import
        // NOP
        break;
    default:
        break;
    }
}

void ExternalMemoryViewModel::onFtpRemoteFileInfoChanged(bool ftpRemoteFileInfo)
{
    m_isFtpFileAcquired = ftpRemoteFileInfo;
    emit isFtpFileAcquiredChanged(m_isFtpFileAcquired);
    qDebug("ExternalMemoryViewModel::%s:%d ftpRemoteFileInfo[%d] m_isRequestGetDandoriData[%d]",
        __func__, __LINE__, ftpRemoteFileInfo, m_isRequestGetDandoriData);
    if (ftpRemoteFileInfo){
        if (m_isRequestGetDandoriData){
            if(mFtpManager) { mFtpManager->getFtpFileInfo(FtpManager::FtpFileInfo::FTP_DANDORI_CSV); }
            m_isRequestGetDandoriData = false;
        }
    }
}

void ExternalMemoryViewModel::onModBusComConnectStatusChanged(int modBusComConnectStatus)
{
    //qDebug() << "ExternalMemoryViewModel::onModBusComConnectStatusChanged m_isModbusConnect:" << m_isModbusConnect << "modBusComConnectStatus:" << modBusComConnectStatus;
    if (m_isModbusConnect && modBusComConnectStatus != (int)QModbusDevice::ConnectedState){
        m_isModbusConnect = false;
        emit isModbusConnectChanged(false);
        reConnectCommunication(false);
        if (m_commonView){
            mModbusDisconnectDate = m_commonView->curDate();
            mModbusDisconnectTime = m_commonView->curTime();
            qCritical() << "onModBusComConnectStatusChanged mModbusDisconnectTime:" << mModbusDisconnectTime << "mModbusDisconnectTime:" << mModbusDisconnectTime;
            connect(m_commonView, SIGNAL(secondsChanged()), this, SLOT(onSecondsChanged()), Qt::DirectConnection);
        }
    } else if (!m_isModbusConnect && modBusComConnectStatus == (int)QModbusDevice::ConnectedState){
#if 1   // 時刻更新通知で実施
#else
        if (mFtpManager){
            mFtpManager->restartFtpCommunication();
        }
        m_isModbusConnect = true;
        emit isModbusConnectChanged(true);
        reConnectCommunication(true);
#endif
    }
}

void ExternalMemoryViewModel::onSecondsChanged()
{
    qCritical() << "ExternalMemoryViewModel::onSecondsChanged mModbusDisconnectDate:" << mModbusDisconnectDate << "mModbusDisconnectTime:" << mModbusDisconnectTime
             << "curTime:" << m_commonView->curTime();
    disconnect(m_commonView, SIGNAL(secondsChanged()), this, SLOT(onSecondsChanged()));
    if (mFtpManager){
        mFtpManager->restartFtpCommunication();
    }
    m_isModbusConnect = true;
    emit isModbusConnectChanged(true);
    reConnectCommunication(true);
    if (m_communication) { m_communication->initIndividualPageModbusTimeOutVariable(); }
}
