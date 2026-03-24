#include "ftpmanager.h"
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
#include <QProcess>
#endif // ENABLE_COMMUNICATION_DUAL_PORT
#include <QTimer>
#include <QEventLoop>

FtpManager::FtpManager(QObject *parent) :
    QObject(parent),
    m_ftpCommunicationThread(FtpCommunicationThread::create_instance()),
    mGetFile{nullptr, nullptr, nullptr, nullptr, nullptr},
    mTimerId(0)
{
#ifdef ENABLE_FTP_COM_ASYNC   // invalid sync ftp com
#else // ENABLE_FTP_COM_ASYNC
    CoreLibSdControl sdControl;
    QString userName = sdControl.getPendantData("FTP", "ID", "defaultUser").toString();  // default user name
    QString password = sdControl.getPendantData("FTP", "Password", "defaultPass").toString();  // default user pass
    QString ipAddr = sdControl.getControllerData("System", "IPAddress", "defaultIP").toString();  // default IP
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
#ifdef REAL_TARGET
    int execCode = QProcess::execute("ping", QStringList() << "-c" << "1" << ipAddr);
    if (execCode != 0){
        ipAddr = sdControl.getControllerData("System", "IPAddress2", "defaultIP").toString();  // default IP
    }
#endif // REAL_TARGET
#endif // ENABLE_COMMUNICATION_DUAL_PORT
#ifdef USE_FTP_COM_SECOND_PORT
    ipAddr = sdControl.getControllerData("System", "IPAddress2", "defaultIP").toString();
#endif // USE_FTP_COM_SECOND_PORT
    qint16 portNo = sdControl.getPendantData("FTP", "PortNo", 21).toInt();
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
#ifdef REAL_TARGET
    qInfo() << "execCode:" << execCode << "userName:" << userName << "password:" << password << "ipAddr:" << ipAddr << "portNo:" << portNo;
#endif // REAL_TARGET
#endif // ENABLE_COMMUNICATION_DUAL_PORT

#ifdef USE_FTP_COM_SECOND_PORT
    qDebug() << "FtpManager::FtpManager userName:" << userName << "password:" << password << "ipAddr:" << ipAddr << "portNo:" << portNo;
#endif // USE_FTP_COM_SECOND_PORT

    mCoreLibFtpSettings = new CoreLibFtpSettings(userName, password, ipAddr, portNo);
    mFtpCommunication = FtpCommunication::create_instance(mCoreLibFtpSettings);
    connect(mFtpCommunication, SIGNAL(ftpCommandStarted(int)), this, SLOT(onFtpCommandStarted(int)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpCommandFinished(int, bool, QString)), this, SLOT(onFtpCommandFinished(int, bool, QString)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpRawCommandReply(int, const QString &)), this, SLOT(onFtpRawCommandReply(int, const QString &)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpStateChanged(int)), this, SLOT(onFtpStateChanged(int)), Qt::DirectConnection);

    if (mFtpCommunication){
        m_coreLibFtp = mFtpCommunication->getCoreLibFtpInstance();
    }
#endif // ENABLE_FTP_COM_ASYNC
}

void FtpManager::startFtpCommunicationThread()
{
    qDebug() << "FtpManager::startFtpCommunicationThread:";
    if ((m_ftpCommunicationThread != nullptr) && (m_ftpCommunicationThread->isRunning())) {
        qWarning() << "ftp communication thread already running";
        return;
    }
    if (!m_ftpCommunicationThread) { m_ftpCommunicationThread = FtpCommunicationThread::create_instance();}
    CoreLibSdControl sdControl;
    QString userName = sdControl.getPendantData("FTP", "ID", "defaultUser").toString();  // default user name
    QString password = sdControl.getPendantData("FTP", "Password", "defaultPass").toString();  // default user pass
    QString ipAddr = sdControl.getControllerData("System", "IPAddress", "defaultIP").toString();  // default IP
    qint16 portNo = sdControl.getPendantData("FTP", "PortNo", 21).toInt();
    m_ftpCommunicationThread->setFtpInfo(userName, password, ipAddr, portNo);
    m_ftpCommunicationThread->setIntervalMiliSec(FTP_COM_THREAD_CYCLE_MONITOR_TIME);
    if (m_ftpCommunicationThread){
        m_coreLibFtp = m_ftpCommunicationThread->getCoreLibFtpInstance();
    }
    m_ftpCommunicationThread->start();
}

FtpManager* FtpManager::create_instance(QObject *parent) {
    if(!instance){
        instance = new FtpManager(parent);
    }
    return instance;
}

FtpManager* FtpManager::get_instance() {
    return instance;
}

void FtpManager::destroy_instance() {
    delete instance;
    instance = nullptr;
}

void FtpManager::folderSelected(int index)
{
    (void)index;
}

void FtpManager::setFtpTimeOut(ResourceManager* resourceManager)
{
    if (!resourceManager){
        qCritical("%s:%d resourceManager is invalid", __func__, __LINE__);
        return;
    }
    bool is_ok;
    QString ftpTimeout = resourceManager->getPenSetStringValue("1", 25, 0);
    qDebug() << "ftpTimeout:" << ftpTimeout;
    mFtpResourceTimeOut = ftpTimeout.toInt(&is_ok, 10);
    if (!is_ok){
        qDebug() << "failed is_ok:" << is_ok << "ftpTimeout:" << ftpTimeout;
        mFtpResourceTimeOut = 0;
        return;
    }
    if((mFtpResourceTimeOut > 0 && mFtpResourceTimeOut < 10) || mFtpResourceTimeOut > 999){
        mFtpResourceTimeOut = 180;
    }
    qDebug() << "mFtpResourceTimeOut:" << mFtpResourceTimeOut;
    return;
}

void FtpManager::requestFtpListCommand(QString targetPath)
{
    if (!m_coreLibFtp){
        return;
    }
    mFtpListArgMap.clear();
    mFtpMdtmArgMap.clear();
    mRemoteFileList.clear();
    mRemoteFileNameList.clear();

    execFtpCommand(Enums::FtpCommand::FtpCommandCWD, nullptr, targetPath);
    mFirstListId = execFtpCommand(Enums::FtpCommand::FtpCommandLIST, nullptr, "");
    stFtpListArgument arg;
    arg.controllerDirPath = targetPath;
    arg.parent = "";
    mFtpListArgMap.insert(mFirstListId, arg);
}

int FtpManager::execNoopCommand()
{
    const QString commandName = "NOOP";
    QStringList argument;
    int rawCommandId = m_coreLibFtp->execRawCommand(commandName, argument);
    return rawCommandId;
}

int FtpManager::execPwdCommand()
{
    const QString commandName = "PWD";
    QStringList argument;
    int rawCommandId = m_coreLibFtp->execRawCommand(commandName, argument);
    return rawCommandId;
}

int FtpManager::execXpwdCommand()
{
    const QString commandName = "XPWD";
    QStringList argument;
    int rawCommandId = m_coreLibFtp->execRawCommand(commandName, argument);
    return rawCommandId;
}

int FtpManager::execAlloCommand(int byteNum)
{
    const QString commandName = "ALLO";
    QStringList argument;
    argument << QString::number(byteNum);
    int rawCommandId = m_coreLibFtp->execRawCommand(commandName, argument);
    return rawCommandId;
}

int FtpManager::execSizeCommand(QString targetPath)
{
    const QString commandName = "SIZE";
    QStringList argument;
    argument << targetPath;
    int rawCommandId = m_coreLibFtp->execRawCommand(commandName, argument);
    return rawCommandId;
}

void FtpManager::parseRemoteFileList(QList<QUrlInfo> *fileList, stFtpListArgument &arg)
{
    if (!fileList){
        qDebug("%s:%d fileList invalid", __func__, __LINE__);
        return;
    }
    for(int i = 0; i < fileList->count(); i++) {
        const QUrlInfo &urlInfo = fileList->at(i);
        stRemoteDirFileInfo info;
        info.name = urlInfo.name();
        info.parent = arg.parent;
        info.lastModified = urlInfo.lastModified();
        info.size = urlInfo.size();
        info.isDir = urlInfo.isDir();
        info.path = arg.controllerDirPath;
        qDebug() << "index:" << i << "name:" << info.name << "parent:" << info.parent
                 << "lastModified:" << info.lastModified << "size:" << info.size
                 << "isDir:" << info.isDir << "path:" << info.path;
        mRemoteFileList << info;
    }
    std::sort(mRemoteFileList.begin(), mRemoteFileList.end(), [] (stRemoteDirFileInfo &left, stRemoteDirFileInfo &right) {
        return left.name < right.name;
    });
    mFtpListArgMap.clear();
    // status update
    setFtpStatus((int)Enums::FtpStatus::FtpStatusExeListCommand);
}

void FtpManager::updateRemoteFileInfo(stRemoteDirFileInfo &updateInfo)
{
    qDebug("%s:%d mRemoteFileList count[%d]", __func__, __LINE__, mRemoteFileList.count());
    qDebug() << "name:" << updateInfo.name << "parent:" << updateInfo.parent
             << "lastModified:" << updateInfo.lastModified << "size:" << updateInfo.size
             << "isDir:" << updateInfo.isDir << "path:" << updateInfo.path << "remoteCnt:" << mRemoteFileList.count();
    int i = 0;
    for(i = 0; i < mRemoteFileList.count(); i++){
        if (mRemoteFileList.at(i).name == updateInfo.name){
            mRemoteFileList.replace(i, updateInfo);
            break;
        } else {
            if (i == mRemoteFileList.count() - 1){
                mRemoteFileList.append(updateInfo);
            }
        }
    }
    std::sort(mRemoteFileList.begin(), mRemoteFileList.end(), [] (stRemoteDirFileInfo &left, stRemoteDirFileInfo &right) {
        return left.name < right.name;
    });
    qDebug("%s:%d mRemoteFileList count[%d] index[%d]", __func__, __LINE__, mRemoteFileList.count(), i);
}

void FtpManager::updateRemoteFileInfo(int opeMode, stRemoteDirFileInfo &updateInfo)
{
    if (opeMode < 0 || opeMode > 3){
        qDebug("%s:%d opeMode[%d] is invalid", __func__, __LINE__, opeMode);
        return;
    }
    qDebug() << "updateRemoteFileInfo name:" << updateInfo.name << "parent:" << updateInfo.parent
             << "lastModified:" << updateInfo.lastModified << "size:" << updateInfo.size
             << "isDir:" << updateInfo.isDir << "path:" << updateInfo.path << "remoteCnt:" << mRemoteFileList.count();
    if (opeMode == 0 || opeMode == 3){
        mRemoteFileList << updateInfo;
        mRemoteFileNameList << updateInfo.name;
    } else if (opeMode == 1){   // Save
        if(mRemoteFileNameList.contains(updateInfo.name)){
            return;
        } else {
            mRemoteFileList << updateInfo;
            mRemoteFileNameList << updateInfo.name;
        }
    } else if (opeMode == 2){   // Erase
        if(mRemoteFileNameList.contains(updateInfo.name)){
            for(int i = 0; i < mRemoteFileList.count(); i++){
                if(mRemoteFileList.at(i).name == updateInfo.name){
                    mRemoteFileList.removeAt(i);
                    break;
                }
            }
            for(int i = 0; i < mRemoteFileNameList.count(); i++){
                if(mRemoteFileNameList.at(i) == updateInfo.name){
                    mRemoteFileNameList.removeAt(i);
                    break;
                }
            }
        } else {
            return;
        }
    }
}

void FtpManager::sortRemoteFile()
{
    qDebug("%s:%d mRemoteFileList count[%d] mRemoteFileNameList count[%d]", __func__, __LINE__, mRemoteFileList.count(), mRemoteFileNameList.count());
    std::sort(mRemoteFileList.begin(), mRemoteFileList.end(), [] (stRemoteDirFileInfo &left, stRemoteDirFileInfo &right) {
        return left.name < right.name;
    });
    if (mRemoteFileNameList.count() > 0){
        std::sort(mRemoteFileNameList.begin(), mRemoteFileNameList.end(), [] (QString &leftName, QString &rightName) {
            return leftName < rightName;
        });
    }
    return;
}

void FtpManager::startFtpCommunication()
{
    if (!m_coreLibFtp){
        return;
    }
    qDebug("%s:%d m_ftpStatus[%d]", __func__, __LINE__, m_ftpStatus);
    switch((QFtp::State)m_ftpStatus){
    case QFtp::Unconnected:
        m_coreLibFtp->execConnectCommand();
        m_coreLibFtp->execLoginCommand();
        break;
    case QFtp::Connected:
        m_coreLibFtp->execLoginCommand();
        break;
    default:
        break;
    }
}

void FtpManager::createFtpCommunication()
{
    CoreLibSdControl sdControl;
    QString userName = sdControl.getPendantData("FTP", "ID", "defaultUser").toString();  // default user name
    QString password = sdControl.getPendantData("FTP", "Password", "defaultPass").toString();  // default user pass
    QString ipAddr = sdControl.getControllerData("System", "IPAddress", "defaultIP").toString();  // default IP
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
#ifdef REAL_TARGET
    int execCode = QProcess::execute("ping", QStringList() << "-c" << "1" << ipAddr);
    if (execCode != 0){
        ipAddr = sdControl.getControllerData("System", "IPAddress2", "defaultIP").toString();  // default IP
    }
#endif // REAL_TARGET
#endif // ENABLE_COMMUNICATION_DUAL_PORT
#ifdef USE_FTP_COM_SECOND_PORT
    ipAddr = sdControl.getControllerData("System", "IPAddress2", "defaultIP").toString();
#endif // USE_FTP_COM_SECOND_PORT
    qint16 portNo = sdControl.getPendantData("FTP", "PortNo", 21).toInt();
#ifdef ENABLE_COMMUNICATION_DUAL_PORT
#ifdef REAL_TARGET
    qInfo() << "execCode:" << execCode << "userName:" << userName << "password:" << password << "ipAddr:" << ipAddr << "portNo:" << portNo;
#endif // REAL_TARGET
#endif // ENABLE_COMMUNICATION_DUAL_PORT

#ifdef USE_FTP_COM_SECOND_PORT
    qDebug() << "FtpManager::FtpManager userName:" << userName << "password:" << password << "ipAddr:" << ipAddr << "portNo:" << portNo;
#endif // USE_FTP_COM_SECOND_PORT

    mCoreLibFtpSettings = new CoreLibFtpSettings(userName, password, ipAddr, portNo);
    mFtpCommunication = FtpCommunication::create_instance(mCoreLibFtpSettings);
    connect(mFtpCommunication, SIGNAL(ftpCommandStarted(int)), this, SLOT(onFtpCommandStarted(int)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpCommandFinished(int, bool, QString)), this, SLOT(onFtpCommandFinished(int, bool, QString)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpRawCommandReply(int, const QString &)), this, SLOT(onFtpRawCommandReply(int, const QString &)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpStateChanged(int)), this, SLOT(onFtpStateChanged(int)), Qt::DirectConnection);

    if (mFtpCommunication){
        m_coreLibFtp = mFtpCommunication->getCoreLibFtpInstance();
    }
}

void FtpManager::terminate()
{
    disconnect(mFtpCommunication, SIGNAL(ftpStateChanged(int)), this, SLOT(onFtpStateChanged(int)));
    disconnect(mFtpCommunication, SIGNAL(ftpRawCommandReply(int, const QString &)), this, SLOT(onFtpRawCommandReply(int, const QString &)));
    disconnect(mFtpCommunication, SIGNAL(ftpCommandFinished(int, bool, QString)), this, SLOT(onFtpCommandFinished(int, bool, QString)));
    disconnect(mFtpCommunication, SIGNAL(ftpCommandStarted(int)), this, SLOT(onFtpCommandStarted(int)));
    if(mCoreLibFtpSettings){
        delete mCoreLibFtpSettings;
        mCoreLibFtpSettings = nullptr;
    }
    FtpCommunication::destroy_instance();
    mFtpCommunication = nullptr;
    m_coreLibFtp = nullptr;
}

void FtpManager::restartFtpCommunication()
{
    qDebug("%s:%d m_ftpStatus[%d]", __func__, __LINE__, m_ftpStatus);
    terminate();
    createFtpCommunication();
    m_coreLibFtp->execConnectCommand();
    m_coreLibFtp->execLoginCommand();
}

void FtpManager::stopFtpCommunication()
{
    if (!m_coreLibFtp){
        return;
    }
    m_coreLibFtp->execCloseCommand();
}

int FtpManager::execFtpCommand(Enums::FtpCommand ftpCommad, QIODevice *ioDevFile, QString targetPath, int byteNum)
{
    if (!m_coreLibFtp){
        return -1;
    }
    int result = 0;
    switch(ftpCommad){
    case Enums::FtpCommand::FtpCommandUSER:
    case Enums::FtpCommand::FtpCommandPASS:
    case Enums::FtpCommand::FtpCommandMDTM:
        break;
    case Enums::FtpCommand::FtpCommandCWD:
        result = m_coreLibFtp->execCdCommand(targetPath);
        break;
    case Enums::FtpCommand::FtpCommandLIST:
        result = m_coreLibFtp->execListCommand(targetPath);
        break;
    case Enums::FtpCommand::FtpCommandRETR:
        result = m_coreLibFtp->execGetCommand(ioDevFile, targetPath);
        break;
    case Enums::FtpCommand::FtpCommandSTOR:
        result = m_coreLibFtp->execPutCommand(ioDevFile, targetPath);
        break;
    case Enums::FtpCommand::FtpCommandPORT:
        result = m_coreLibFtp->execTransferModeCommand(CoreLibFileTransferProtocol::FtpTransMode::FtpTransModeActive);
        break;
    case Enums::FtpCommand::FtpCommandPASV:
        result = m_coreLibFtp->execTransferModeCommand(CoreLibFileTransferProtocol::FtpTransMode::FtpTransModePassive);
        break;
    case Enums::FtpCommand::FtpCommandNOOP:
        result = execNoopCommand();
        break;
    case Enums::FtpCommand::FtpCommandPWD:
        result = execPwdCommand();
        break;
    case Enums::FtpCommand::FtpCommandXPWD:
        result = execXpwdCommand();
        break;
    case Enums::FtpCommand::FtpCommandALLO:
        result = execAlloCommand(byteNum);
        break;
    case Enums::FtpCommand::FtpCommandSIZE:
        result = execSizeCommand(targetPath);
        break;
    default:
        break;
    }
    if (ftpCommad != Enums::FtpCommand::FtpCommandNOOP) { qDebug("%s:%d result[%d] ftpCommad[%d]", __func__, __LINE__, result, ftpCommad); }
    return result;
}

void FtpManager::getFtpFileInfo(FtpManager::FtpFileInfo fileInfo)
{
    if (fileInfo < FtpManager::FtpFileInfo::FTP_DANDORI_CSV
        || fileInfo >= FtpManager::FtpFileInfo::FTP_FILE_INFO_MAX){
        return;
    }

    for(int i = 0; i < mRemoteFileList.count(); i++){
        stRemoteDirFileInfo info = mRemoteFileList.at(i);
        if (info.name == STARTUP_FTP_FILE_NAME_LIST.at((int)fileInfo)){
            mGetFile[(int)fileInfo] = new QFile(AppEnvironment::getFtpFolderPath() + "/" + STARTUP_FTP_FILE_NAME_LIST.at((int)fileInfo));
            if (mGetFile[(int)fileInfo]->exists()){
                //qDebug() << "remove file:" << STARTUP_FTP_FILE_NAME_LIST.at((int)fileInfo);
                mGetFile[(int)fileInfo]->remove();
            }
            mGetFile[(int)fileInfo]->open(QIODevice::ReadWrite);
            int id = execFtpCommand(Enums::FtpCommand::FtpCommandRETR, mGetFile[(int)fileInfo], info.name);
            stFtpGetArgument arg;
            arg.getFile = mGetFile[(int)fileInfo];
            arg.selectedIndex = i;
            if (fileInfo == FtpManager::FtpFileInfo::FTP_NCSTEP_NCP
                || fileInfo == FtpManager::FtpFileInfo::FTP_DANDORI_CSV
                || fileInfo == FtpManager::FtpFileInfo::FTP_ALM_HIS1_DAT
                || fileInfo == FtpManager::FtpFileInfo::FTP_ALM_HIS2_DAT){
                arg.fileInfo = (int)fileInfo;
            } else {
                arg.fileInfo = (int)FtpManager::FtpFileInfo::FTP_FILE_INVALID;
            }
            mFtpGetArgMap.insert(id, arg);
            break;
        }
    }
}

QFile* FtpManager::getFtpFileData(FtpManager::FtpFileInfo fileInfo)
{
    if (fileInfo < FtpManager::FtpFileInfo::FTP_DANDORI_CSV
        || fileInfo >= FtpManager::FtpFileInfo::FTP_FILE_INFO_MAX){
        return nullptr;
    }
    return mGetFile[fileInfo];
}

QString FtpManager::getFtpFilePath(FtpManager::FtpFileInfo fileInfo)
{
    QString filePath = "";
    if (fileInfo < FtpManager::FtpFileInfo::FTP_DANDORI_CSV
        || fileInfo >= FtpManager::FtpFileInfo::FTP_FILE_INFO_MAX){
        return filePath;
    }
    filePath = AppEnvironment::getFtpFolderPath() + "/" + STARTUP_FTP_FILE_NAME_LIST.at((int)fileInfo);
    return filePath;
}

void FtpManager::changeDirFtpCommunication(QString targetPath)
{
    int result = execFtpCommand(Enums::FtpCommand::FtpCommandCWD, nullptr, targetPath);
    if (result < 0){
        qDebug("%s:%d execFtpCommand result[%d]", __func__, __LINE__, result);
        mTargetPath = "";
    } else {
        mTargetPath = targetPath;
    }
}

void FtpManager::getFileListFtpCommunication()
{
    mFtpListArgMap.clear();
    int id = execFtpCommand(Enums::FtpCommand::FtpCommandLIST, nullptr, "");
    stFtpListArgument arg;
    arg.controllerDirPath = mTargetPath;
    arg.parent = "";
    mFtpListArgMap.insert(id, arg);
}

void FtpManager::setFtpStatus(int ftpStatus)
{
//    if (m_ftpStatus == ftpStatus){
//        return;
//    }
    m_ftpStatus = ftpStatus;
    emit ftpStatusChanged(m_ftpStatus);
}

void FtpManager::setFtpListIndex(int ftpListIndex)
{
//    if (m_ftpListIndex == ftpListIndex){
//        return;
//    }
    m_ftpListIndex = ftpListIndex;
    emit ftpListIndexChanged(m_ftpListIndex);
}

void FtpManager::setFtpGetArgment(int cmdId, stFtpGetArgument &ftpGetArg)
{
    //qDebug("%s:%d cmdId[%d]", __func__, __LINE__, cmdId);
    mFtpGetArgMap.insert(cmdId, ftpGetArg);
}

void FtpManager::setFtpPutArgment(int cmdId, stFtpPutArgument &ftpPutArg)
{
    //qDebug("%s:%d cmdId[%d]", __func__, __LINE__, cmdId);
    mFtpPutArgMap.insert(cmdId, ftpPutArg);
}

void FtpManager::startFtpLogin()
{
    if (!m_coreLibFtp){
        return;
    }
    //qDebug("%s:%d m_ftpStatus[%d]", __func__, __LINE__, m_ftpStatus);
    m_coreLibFtp->execLoginCommand();
}

void FtpManager::setIsStartupFtpStatus(bool isStartupFtpStatus)
{
    if (m_isStartupFtpStatus == isStartupFtpStatus){
        return;
    }
    m_isStartupFtpStatus = isStartupFtpStatus;
    emit isStartupFtpStatusChanged(m_isStartupFtpStatus);
}

void FtpManager::startupFtpProcess()
{
    int ftpLoopCnt = 0;
    while (getFtpConnectStatus() != QFtp::Connected && ftpLoopCnt < FTP_CONNETCTED_TIMEOUT){
        QEventLoop loop;
        QTimer::singleShot( 10, &loop, SLOT( quit() ) );
        loop.exec();
        ftpLoopCnt++;
    }
    //qDebug("%s:%d FTP_CONNETCTED: ftpLoopCnt[%d] m_ftpStatus[%d]", __func__, __LINE__, ftpLoopCnt, getFtpConnectStatus());
    ftpLoopCnt = 0;
    if (getFtpConnectStatus() >= QFtp::Connected && getFtpConnectStatus() != QFtp::Closing){
        startFtpLogin();
        while (getFtpConnectStatus() != QFtp::LoggedIn && ftpLoopCnt < FTP_LOGIN_TIMEOUT){
            QEventLoop loop;
            QTimer::singleShot( 10, &loop, SLOT( quit() ) );
            loop.exec();
            ftpLoopCnt++;
        }
        //qDebug("%s:%d FTP_LOGIN: ftpLoopCnt[%d] ftpStatus[%d]", __func__, __LINE__, ftpLoopCnt, getFtpConnectStatus());
        ftpLoopCnt = 0;
    }
    if (getFtpConnectStatus() >= QFtp::LoggedIn && getFtpConnectStatus() != QFtp::Closing){
        requestFtpListCommand("/");
        while (getFtpConnectStatus() < Enums::FtpStatus::FtpStatusExeListCommand && ftpLoopCnt < FTP_GET_LIST_TIMEOUT){
            QEventLoop loop;
            QTimer::singleShot( 10, &loop, SLOT( quit() ) );
            loop.exec();
            ftpLoopCnt++;
        }
        //qDebug("%s:%d FTP_GET_LIST: ftpLoopCnt[%d] ftpStatus[%d]", __func__, __LINE__, ftpLoopCnt, getFtpConnectStatus());
        ftpLoopCnt = 0;
    }
    if (getFtpConnectStatus() >= Enums::FtpStatus::FtpStatusExeListCommand){
        getFtpFileInfo(FtpManager::FtpFileInfo::FTP_DANDORI_CSV);
        getFtpFileInfo(FtpManager::FtpFileInfo::FTP_ALM_HIS1_DAT);
        getFtpFileInfo(FtpManager::FtpFileInfo::FTP_ALM_HIS2_DAT);
        getFtpFileInfo(FtpManager::FtpFileInfo::FTP_NCSTEP_NCP);
        while (getFtpConnectStatus() < Enums::FtpStatus::FtpStatusGetFinished && ftpLoopCnt < FTP_GET_FILE_TIMEOUT){
            QEventLoop loop;
            QTimer::singleShot( 10, &loop, SLOT( quit() ) );
            loop.exec();
            ftpLoopCnt++;
        }
        //qDebug("%s:%d FTP_GET_FILE: ftpLoopCnt[%d] ftpStatus[%d]", __func__, __LINE__, ftpLoopCnt, getFtpConnectStatus());
        ftpLoopCnt = 0;
    }
    setIsStartupFtpStatus(true);
}

void FtpManager::asyncStartupFtpProcess()
{
    qDebug("%s:%d m_ftpStatus[%d]", __func__, __LINE__, getFtpConnectStatus());
    requestFtpListCommand("/");
    timerStart(25);
}

void FtpManager::setIsDeterNoopCmd(bool isDeterNoopCmd)
{
    if (m_isDeterNoopCmd == isDeterNoopCmd)
        return;

    m_isDeterNoopCmd = isDeterNoopCmd;
    emit isDeterNoopCmdChanged(m_isDeterNoopCmd);
}

void FtpManager::setIsStartupFtpGetFileList(bool isStartupFtpGetFileList)
{
    if (mIsStartupFtpGetFileList == isStartupFtpGetFileList)
        return;

    mIsStartupFtpGetFileList = isStartupFtpGetFileList;
    emit isStartupFtpGetFileListChanged(mIsStartupFtpGetFileList);
}

void FtpManager::checkStartupFtpProcess()
{
    //qDebug("%s:%d m_ftpStatus[%d] mTimerCnt[%d]", __func__, __LINE__, getFtpConnectStatus(), mTimerCnt);
    if (mTimerCnt >= 1000){
        if (getFtpConnectStatus() < Enums::FtpStatus::FtpStatusExeListCommand){
            qCritical("%s:%d m_ftpStatus[%d] mTimerCnt[%d] get ftpList failed", __func__, __LINE__, getFtpConnectStatus(), mTimerCnt);
            timerStop();
            mTimerCnt = 0;
        } else {
            qCritical("%s:%d m_ftpStatus[%d] mTimerCnt[%d] TimeOut", __func__, __LINE__, getFtpConnectStatus(), mTimerCnt);
            timerStop();
            mTimerCnt = 0;
        }
    } else {
        if (getFtpConnectStatus() >= Enums::FtpStatus::FtpStatusExeListCommand){
            if (getFtpConnectStatus() == Enums::FtpStatus::FtpStatusExeListCommand && !mIsGetFtpList){
                qDebug("%s:%d m_ftpStatus[%d] mTimerCnt[%d] get ftpList success", __func__, __LINE__, getFtpConnectStatus(), mTimerCnt);
                timerStop();
                mTimerCnt = 0;
                mIsGetFtpList = true;
                getFtpFileInfo(FtpManager::FtpFileInfo::FTP_DANDORI_CSV);
                getFtpFileInfo(FtpManager::FtpFileInfo::FTP_ALM_HIS1_DAT);
                getFtpFileInfo(FtpManager::FtpFileInfo::FTP_ALM_HIS2_DAT);
                getFtpFileInfo(FtpManager::FtpFileInfo::FTP_NCSTEP_NCP);
                timerStart(25);
            } else {
                //qDebug("%s:%d m_ftpStatus[%d] mTimerCnt[%d] mIsStartupFtpGetFileList[%d] getting ftpdata", __func__, __LINE__, getFtpConnectStatus(), mTimerCnt, mIsStartupFtpGetFileList);
                if (getFtpConnectStatus() >= Enums::FtpStatus::FtpStatusGetFinished
                    && mIsStartupFtpGetFileList){
                    qInfo("%s:%d m_ftpStatus[%d] mTimerCnt[%d] mIsStartupFtpGetFileList[%d] get ftpdata success", __func__, __LINE__, getFtpConnectStatus(), mTimerCnt, mIsStartupFtpGetFileList);
                    timerStop();
                    mTimerCnt = 0;
                    setIsStartupFtpStatus(true);
                }
            }
        }
    }
}

void FtpManager::onFtpCommandStarted(int id)
{
    //qDebug("%s:%d id[%d]", __func__, __LINE__, id);
    (void)id;
}

void FtpManager::onFtpCommandFinished(int id, bool error, QString errorStr)
{
    if (m_coreLibFtp->getCurrentCommand() != QFtp::Command::RawCommand) {
        qDebug() << "id:" << id << "error:" << error << "errorStr:" << errorStr << "mFirstListId:" << mFirstListId << "currentCommand:" << m_coreLibFtp->getCurrentCommand();
    }
    (void)error;
    (void)errorStr;
    if(m_coreLibFtp->getCurrentCommand() == QFtp::Command::Get) {
        if (!mIsStartupFtpGetFileList){
            stFtpGetArgument arg = mFtpGetArgMap.take(id);
            QFile *file = arg.getFile;
            file->close();
            delete file;
            setFtpListIndex(arg.selectedIndex);
            //qDebug("%s:%d FTP_GET_FILE: id[%d] mFtpGetArgMap count[%d]", __func__, __LINE__, id, mFtpGetArgMap.count());
            if (mFtpGetArgMap.count() <= 0){
                setFtpStatus((int)Enums::FtpStatus::FtpStatusGetFinished);
                setIsStartupFtpGetFileList(true);
            }
            if(arg.fileInfo >= 0){
                m_ftpFileInfo = arg.fileInfo;
                emit ftpFileInfoChanged(m_ftpFileInfo);
            }
        } else {
            qDebug() << "QFtp::Command::Get" << "id:" << id << "error:" << error << "errorStr:" << errorStr << "mFirstListId:" << mFirstListId << "currentCommand:" << m_coreLibFtp->getCurrentCommand();
            stFtpGetArgument arg = mFtpGetArgMap.take(id);
            QFile *file = arg.getFile;
            bool isToExtMem = arg.isToExtMem;
            QString fileName = file->fileName();
            file->close();
            delete file;
            setFtpListIndex(arg.selectedIndex);
            if (error) {
                emit ftpErrorStrNotify(errorStr);
            }
            setFtpStatus((int)Enums::FtpStatus::FtpStatusGetFinished);
            emit ftpGetFinished(fileName);
            if (isToExtMem) { emit ftpExtMemFinishNotify(isToExtMem); }
            if (arg.fileInfo == (int)FtpManager::FtpFileInfo::FTP_NCSTEP_NCP
                || arg.fileInfo == (int)FtpManager::FtpFileInfo::FTP_DANDORI_CSV){
                m_ftpFileInfo = arg.fileInfo;
                emit ftpFileInfoChanged(m_ftpFileInfo);
            }
        }
    }
    if(m_coreLibFtp->getCurrentCommand() == QFtp::Command::Put) {
        qDebug() << "QFtp::Command::Put" << "id:" << id << "error:" << error << "errorStr:" << errorStr << "mFirstListId:" << mFirstListId << "currentCommand:" << m_coreLibFtp->getCurrentCommand();
        stFtpPutArgument arg = mFtpPutArgMap.take(id);
        QFile *file = arg.putFile;
        file->close();
        delete file;
        setFtpListIndex(arg.selectedIndex);
        if (error){
            emit ftpErrorStrNotify(errorStr);
        } else {
            setFtpStatus((int)Enums::FtpStatus::FtpStatusPutFinished);
        }
    }

    if(mFirstListId == id) {
        // top directory
        // get file and directory list
        stFtpListArgument arg = mFtpListArgMap.take(id);
        QList<QUrlInfo> *fileList = m_coreLibFtp->getFileList();
        parseRemoteFileList(fileList, arg);
        emit ftpRemoteFileInfoChanged(true);
    } else {
        if(m_coreLibFtp->getCurrentCommand() == QFtp::Command::List) {
            stFtpListArgument arg = mFtpListArgMap.take(id);
            QList<QUrlInfo> *fileList = m_coreLibFtp->getFileList();
            parseRemoteFileList(fileList, arg);
            emit ftpRemoteFileInfoChanged(true);
        }
    }
}

void FtpManager::onFtpRawCommandReply(int replyCode, const QString & detail)
{
    if (m_coreLibFtp->currentRawCommandName() != "NOOP") {
        qDebug() << "currentRawCommand:" << m_coreLibFtp->currentRawCommandName() << "replyCode:" << replyCode << "detail:" << detail;
    }
    if (m_coreLibFtp->currentRawCommandName() == "PWD"
        || m_coreLibFtp->currentRawCommandName() == "XPWD"){
        QStringList extractStrList = detail.split(" is");
        //qDebug() << "extractStrList[0]:" << extractStrList[0] << "extractStrList[1]:" << extractStrList[1];
        emit ftpCurDirNotify(extractStrList[0]);
    } else if (m_coreLibFtp->currentRawCommandName() == "SIZE"){
        bool is_ok = false;
        int size = detail.toInt(&is_ok, 10);
        if (!is_ok){
            qDebug() << "detail is not num:" << detail;
            return;
        }
        if (replyCode == 213){
            emit ftpFileSizeNotify(size);
        } else if (replyCode == 212){
            emit ftpDirSizeNotify(size);
        }
    } else if (m_coreLibFtp->currentRawCommandName() == "ALLO"){
        if (replyCode == 200 && detail == "Command successful"){
            emit ftpAlloResultNotify(true);
        } else {
            emit ftpAlloResultNotify(false);
        }
    }
}

void FtpManager::onFtpStateChanged(int status)
{
    //qDebug("%s:%d status[%d]", __func__, __LINE__, status);
    setFtpStatus(status);
#ifdef ENABLE_FTP_COM_ASYNC
#else // ENABLE_FTP_COM_ASYNC
    if (m_ftpStatus >= (int)QFtp::LoggedIn && m_ftpStatus != (int)QFtp::Closing
        && !m_isStartupFtpStatus){
        asyncStartupFtpProcess();
        setIsStartupFtpStatus(true);
    }
#endif // ENABLE_FTP_COM_ASYNC
}

void FtpManager::timerStart(int timeOut)
{
    if (!mIsTimerStatus) {
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void FtpManager::timerStop()
{
    if (mIsTimerStatus) {
        killTimer(mTimerId);
        mTimerCnt = 0;
        mIsTimerStatus = false;
    }
}

void FtpManager::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent) return;
    if(timerEvent->timerId() == mTimerId){
        mTimerCnt++;
        checkStartupFtpProcess();
    }
}

FtpManager* FtpManager::instance = nullptr;
