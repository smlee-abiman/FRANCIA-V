#ifndef EXTERNALMEMORYVIEWMODEL_H
#define EXTERNALMEMORYVIEWMODEL_H

#include <QObject>
#include <QDirIterator>
#include <QTimerEvent>
#include <QFileInfo>
#include "viewmodelbase.h"
#include "enums.h"
#include "ftpmanager.h"
#include "usbmanager.h"
#include "commonviewmodel.h"
#include "transferfilethread.h"
#include "programfilemanager.h"

#define EXT_MEM_BACKUP_MAX_NUM                 99
#define EXT_MEM_COPY_TIMER_INTERVAL           100
#define EXT_MEM_ASYNC_SINGLE_TIMER_VALUE      100

#define EXT_MEM_VARIABLE_CONTROLLER_BACKUP_MAX      32
#define EXT_MEM_NCP_BACKUP_MAX                      99
#define EXT_MEM_COND_BACKUP_MAX                    999
#define EXT_MEM_MACHINE_BACKUP_MAX                  32
#define EXT_MEM_NCP_FILE_MAX                        99

const QList<QString> WRITE_FROM_USB_TARGET_FOLDER_LIST = {
    "penupdate",
    "controllerupdate",
    "NCP",
    "backup",
    "pen_msg",
    "images"
};

const QList<QString> WRITE_FROM_USB_TARGET_FILE_LIST = {
    "ChangeEthernetSpeed",
};

const QList<QString> WRITE_FROM_USB_LOGO_IMAGE_PEN_UPDATE_LIST = {
    "logo1.png",
};

const QList<QString> WRITE_FROM_USB_COMMON_LOGO_IMAGE_PEN_UPDATE_LIST = {
    "logo2.png",
};

const QList<QString> WRITE_FROM_USB_APP_BINARY_PEN_UPDATE_LIST = {
    "pendant-gui",
};

const QList<QString> WRITE_FROM_USB_SYSTEM_SET_APP_BINARY_PEN_UPDATE_LIST = {
    "SystemSettingApp",
};

const QList<QString> WRITE_FROM_USB_CORE_LIBRARY_PEN_UPDATE_LIST = {
    "libCoreLib.so",
    "libCoreLib.so.1",
    "libCoreLib.so.1.0",
    "libCoreLib.so.1.0.0",
};

const QList<QString> FTP_PUT_FIXED_CONTROLLER_UPDATE_LIST = {
    "TRC1000C.dsb",
    "TRC1000C.dop",
    "motc.dat",
    "motIPL.dat",
};

const QList<QString> FTP_PUT_VARIABLE_CONTROLLER_UPDATE_LIST = {
    "machine.dat",
};

const QList<QString> FTP_PUT_NCP_LIST = {
    "NCP.NCP",
};

const QList<QString> FTP_PUT_FIXED_DAILY_BACKUP_LIST = {
    "MOTINIT.DAT",
    "SETETC.DAT",
    "IONAME.io",
    "NOWDATA.DAT",
    "NCSTEP.NCP",
};

const QList<QString> FTP_PUT_VARIABLE_DAILY_COND_BACKUP_LIST = {
    "COND.CND",
};

const QList<QString> FTP_PUT_VARIABLE_DAILY_MACHINE_BACKUP_LIST = {
    "machine.dat",
};

const QList<QString> FTP_READ_ONLY_BACKUP_LIST = {
    "ALMHIS1.DAT",
    "ALMHIS2.DAT",
    "OPE2HIS.DAT",
    "IOHIS.DAT",
    "DANDORI.CSV",
};

const QList<QString> WRITE_FROM_USB_PEN_MSG_LIST = {
    "all_msg.txt",
    "ALM_msg.txt",
    "counter_msg.txt",
    "symbol_msg.csv",
    "ERR_msg.txt",
    "HELP_msg.txt",
    "IO_msg.txt",
    "manual_msg.txt",
    "Mkey_msg.txt",
    "mode_msg.txt",
    "OPE_msg.txt",
    "point_msg.txt",
};

const QList<QString> WRITE_FROM_USB_PEN_SET_LIST = {
    "pen_set.csv",
};

const QList<QString> WRITE_FROM_USB_REQUIRED_POWER_RESTART_FILE_LIST = {
    "TRC1000C.dsb","TRC1000C.dop","motc.dat","motIPL.dat","MOTINIT.DAT",
    "SETETC.DAT","IONAME.io","NOWDATA.DAT","machine01.dat","machine02.dat",
    "machine03.dat","machine04.dat","machine05.dat","machine06.dat","machine07.dat",
    "machine08.dat","machine09.dat","machine10.dat","machine11.dat","machine12.dat",
    "machine13.dat","machine14.dat","machine15.dat","machine16.dat","machine17.dat",
    "machine18.dat","machine19.dat","machine20.dat","machine21.dat","machine22.dat",
    "machine23.dat","machine24.dat","machine25.dat","machine26.dat","machine27.dat",
    "machine28.dat","machine29.dat","machine30.dat","machine31.dat","machine32.dat",
    "ChangeEthernetSpeed",
};

const QList<QString> FTP_FOLDER_BULK_TRANSFER_NOT_APPLICABLE_LIST = {
    "NCP",
};

class ExternalMemoryViewFileList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString fileParent READ fileParent WRITE setFileParent NOTIFY fileParentChanged)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    //Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged)
    Q_PROPERTY(bool fileUrlDir READ fileUrlDir WRITE setFileUrlDir NOTIFY fileUrlDirChanged)
    Q_PROPERTY(qint64 fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged)
    Q_PROPERTY(QDateTime fileLastModified READ fileLastModified WRITE setFileLastModified NOTIFY fileLastModifiedChanged)

public:
    explicit ExternalMemoryViewFileList(QObject *parent = nullptr);

    void setFileName(QString fileName);
    void setFileParent(QString fileParent);
    void setFilePath(QString filePath);
    void setFileUrlDir(bool fileUrlDir);
    void setFileSize(qint64 fileSize);
    void setFileLastModified(QDateTime fileLastModified);

    QString fileName() const { return m_fileName; }
    QString fileParent() const { return m_fileParent; }
    QString filePath() const { return m_filePath; }
    bool fileUrlDir() const { return m_fileUrlDir; }
    qint64 fileSize() const { return m_fileSize; }
    QDateTime fileLastModified() const { return m_fileLastModified; }

private:
    QString m_fileName = "";
    QString m_fileParent = "";
    QString m_filePath = "";
    bool m_fileUrlDir = false;
    qint64 m_fileSize = 0;
    QDateTime m_fileLastModified;

signals:
    void fileNameChanged(QString fileName);
    void fileParentChanged(QString fileParent);
    void filePathChanged(QString filePath);
    void fileUrlDirChanged(bool fileUrlDir);
    void fileSizeChanged(qint64 fileSize);
    void fileLastModifiedChanged(QDateTime fileLastModified);
};

class ExternalMemoryViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> extFtpControllerFileList READ extFtpControllerFileList WRITE setExtFtpControllerFileList NOTIFY extFtpControllerFileListChanged)
    Q_PROPERTY(QList<QObject *> extUsbMemoryFileList READ extUsbMemoryFileList WRITE setExtUsbMemoryFileList NOTIFY extUsbMemoryFileListChanged)
    Q_PROPERTY(QList<QObject *> extUsbMemoryChildDirFileList READ extUsbMemoryChildDirFileList WRITE setExtUsbMemoryChildDirFileList NOTIFY extUsbMemoryChildDirFileListChanged)
    Q_PROPERTY(int remoteDirFileCnt READ remoteDirFileCnt WRITE setRemoteDirFileCnt NOTIFY remoteDirFileCntChanged)
    Q_PROPERTY(int usbDirFileCnt READ usbDirFileCnt WRITE setUsbDirFileCnt NOTIFY usbDirFileCntChanged)
    Q_PROPERTY(int usbChildDirFileCnt READ usbChildDirFileCnt WRITE setUsbChildDirFileCnt NOTIFY usbChildDirFileCntChanged)
    Q_PROPERTY(bool isUSBConnected READ isUSBConnected WRITE setIsUSBConnected NOTIFY isUSBConnectedChanged)
    Q_PROPERTY(bool isAlreadyUsbFileList READ isAlreadyUsbFileList WRITE setIsAlreadyUsbFileList NOTIFY isAlreadyUsbFileListChanged)
    Q_PROPERTY(bool isAlreadyUsbChildFileList READ isAlreadyUsbChildFileList WRITE setIsAlreadyUsbChildFileList NOTIFY isAlreadyUsbChildFileListChanged)
    Q_PROPERTY(bool processingResult MEMBER m_processingResult NOTIFY processingResultChanged)
    Q_PROPERTY(bool processingTimeout MEMBER m_processingTimeout NOTIFY processingTimeoutChanged)
    Q_PROPERTY(QString errCode READ errCode WRITE setErrCode NOTIFY errCodeChanged)
    Q_PROPERTY(int curTabIndex READ curTabIndex WRITE setCurTabIndex NOTIFY curTabIndexChanged)
    Q_PROPERTY(bool isUsbWriteProtected MEMBER m_isUsbWriteProtected NOTIFY isUsbWriteProtectedChanged)
    Q_PROPERTY(bool firstUsbConnected READ firstUsbConnected WRITE setFirstUsbConnected NOTIFY firstUsbConnectedChanged)
    Q_PROPERTY(bool requestPowerRestart MEMBER m_requestPowerRestart NOTIFY requestPowerRestartChanged)
    Q_PROPERTY(bool isNeedUpdateSetupData READ isNeedUpdateSetupData WRITE setIsNeedUpdateSetupData NOTIFY isNeedUpdateSetupDataChanged)
    Q_PROPERTY(bool isReOrderSetupDataListFinish MEMBER m_isReOrderSetupDataListFinish NOTIFY isReOrderSetupDataListFinishChanged)
    Q_PROPERTY(bool isReOrderSetupDataFinish MEMBER m_isReOrderSetupDataFinish NOTIFY isReOrderSetupDataFinishChanged)
    Q_PROPERTY(bool isNcpFileUpdate MEMBER m_isNcpFileUpdate NOTIFY isNcpFileUpdateChanged)
    Q_PROPERTY(bool isFtpFileAcquired MEMBER m_isFtpFileAcquired NOTIFY isFtpFileAcquiredChanged)
    Q_PROPERTY(bool isModbusConnect MEMBER m_isModbusConnect NOTIFY isModbusConnectChanged)
    Q_PROPERTY(int complieState MEMBER m_complieState NOTIFY complieStateChanged)
    Q_PROPERTY(int compareDiscordFactor MEMBER m_compareDiscordFactor NOTIFY compareDiscordFactorChanged)
    Q_PROPERTY(bool isRequestFtpFileInfo MEMBER m_isRequestFtpFileInfo NOTIFY isRequestFtpFileInfoChanged)
public:
    enum CopyDirectionStatus {
        CopyDirectionInit   = 0,
        CopyDirectionToUsb,
        CopyDirectionFromUsb,
    };
    enum FtpTransMode {
        FtpTransModeGet     = 0,
        FtpTransModePut,
    };
    enum LogoType {
        LogoTypeInvalid = -1,
        LogoType1     = 0,  // logo1
        LogoType2,          // logo2
    };

    explicit ExternalMemoryViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    explicit ExternalMemoryViewModel(QObject *object = nullptr);
    ~ExternalMemoryViewModel();
    Q_INVOKABLE void startFtpCommunication();
    Q_INVOKABLE void stopFtpCommunication();
    Q_INVOKABLE void startUsbMonitoring();
    Q_INVOKABLE void stopUsbMonitoring();
    Q_INVOKABLE void getFtpFileList();
    Q_INVOKABLE int getFtpRemoteFileListCount();
    Q_INVOKABLE int getUsbDirFileListCount();
    Q_INVOKABLE int writeToUsb(int listIndex, int subListIndex);
    Q_INVOKABLE int writeFromUsb(int listIndex, int subListIndex);
    Q_INVOKABLE void getUsbDirFileList();
    Q_INVOKABLE void getChildDirFileList(int mode, int childDirIndex, int listIndex);
    Q_INVOKABLE int createBackupFolder();
    Q_INVOKABLE int writeInBulkToUsb();
    Q_INVOKABLE void clearUsbChildDirFileList(int usbChildDirfileCnt);
    Q_INVOKABLE void startModbusMonitoring();
    Q_INVOKABLE void stopModbusMonitoring();
    Q_INVOKABLE void clearFtpControllerViewFileList(int ftpCtrlDirfileCnt);

    void setExtFtpControllerFileList(QList<QObject *> extFtpControllerFileList);
    QList<QObject *> extFtpControllerFileList() const { return m_extFtpControllerFileList; }
    void setExtUsbMemoryFileList(QList<QObject *> extUsbMemoryFileList);
    QList<QObject *> extUsbMemoryFileList() const { return m_extUsbMemoryFileList; }
    void setExtUsbMemoryChildDirFileList(QList<QObject *> extUsbMemoryChildDirFileList);
    QList<QObject *> extUsbMemoryChildDirFileList() const { return m_extUsbMemoryChildDirFileList; }
    void setRemoteDirFileCnt(int remoteDirFileCnt);
    int remoteDirFileCnt() const { return m_remoteDirFileCnt; }
    void setUsbDirFileCnt(int usbDirFileCnt);
    int usbDirFileCnt() const { return m_usbDirFileCnt; }
    void setUsbChildDirFileCnt(int usbChildDirFileCnt);
    int usbChildDirFileCnt() const { return m_usbChildDirFileCnt; }
    void setIsUSBConnected(bool isUSBConnected);
    bool isUSBConnected() const { return m_isUSBConnected; }
    void setIsAlreadyUsbFileList(bool isAlreadyUsbFileList);
    bool isAlreadyUsbFileList() const { return m_isAlreadyUsbFileList; }
    void setIsAlreadyUsbChildFileList(bool isAlreadyUsbChildFileList);
    bool isAlreadyUsbChildFileList() const { return m_isAlreadyUsbChildFileList; }

    Q_INVOKABLE QString getFileFolderListDispColor() { return m_extMemFileFolderListDispRGB; }
    Q_INVOKABLE QString getSelectedFileFolderListDispColor() { return m_extMemSelectedFileFolderListDispRGB; }
    Q_INVOKABLE QString getCommandPressColor() { return m_extMemCommandPressRGB; }
    Q_INVOKABLE void timerStop();
    Q_INVOKABLE bool downloadImageData(QString targetDirName);

    void setErrCode(QString errCode);
    QString errCode() const { return m_errCode; }
    void setCurTabIndex(int curTabIndex);
    int curTabIndex() const { return m_curTabIndex; }
    void setFirstUsbConnected(bool firstUsbConnected);
    bool firstUsbConnected() const { return m_firstUsbConnected; }
    Q_INVOKABLE bool checkReadOnlyFile(QString targetFileName);
    void setIsNeedUpdateSetupData(bool isNeedUpdateSetupData);
    bool isNeedUpdateSetupData() const { return m_isNeedUpdateSetupData; }

private:
    QList<QObject *>            m_extFtpControllerFileList;
    QList<QObject *>            m_extUsbMemoryFileList;
    QList<QObject *>            m_extUsbMemoryChildDirFileList;
    FtpManager                  *mFtpManager;
    UsbManager                  *mUsbManager;
    CommonViewModel             *m_commonView = nullptr;
    TransferFileThread          *mTransferFileThread = nullptr;
	ProgramFileManager          *mProgramFileManager;
    QList<stRemoteDirFileInfo>  mRemoteDirFileInfo;
    QList<stRemoteDirFileInfo>  mUsbDirFileInfo;
    QList<stRemoteDirFileInfo>  mUsbChildDirFileInfo;
    int                         m_remoteDirFileCnt = 0;
    int                         m_usbDirFileCnt = 0;
    int                         m_usbChildDirFileCnt = 0;
    bool                        mIsUsbMonitor = false;
    bool                        m_isUSBConnected = false;
    bool                        m_isAlreadyUsbFileList = false;
    bool                        m_isAlreadyUsbChildFileList = false;
    QString                     mUsbRootPath = "";
    QString                     mUsbMountPath = "";
    QFile                       *mGetFile = nullptr;
    QFile                       *mPutFile = nullptr;
    bool                        m_processingResult = false;
    bool                        m_processingTimeout = false;
    int                         mTimerId;
    bool                        mIsTimerStatus = false;
    int                         mTimerCnt = 0;
    int                         mUsbDirection = 0;
    QString                     mCurCopyFile = "";
    int                         m_ftpListIndex = -1;
    int                         m_preFtpListIndex = -1;
    QString                     m_extMemFileFolderListDispRGB;
    QString                     m_extMemSelectedFileFolderListDispRGB;
    QString                     m_extMemCommandPressRGB;
    QString                     m_errCode = "";
    QMutex                      *mMutexFtpPut;
    QMutex                      *mMutexFtpGet;
    QMutex                      *mMutexCursorOpe;
    bool                        mIsMutexFtpPut = false;
    bool                        mIsMutexFtpGet = false;
    bool                        mIsMutexCursorOpe = false;
    stRemoteDirFileInfo         mUpdateInfo;
    int                         m_curTabIndex = -1;
    QString                     mCurUsbFolder = "";
    bool                        m_isUsbWriteProtected = false;
    bool                        m_firstUsbConnected = false;
    int                         mBackupDataCnt = 0;
    QMutex                      m_usbBackupFtpListMutex;
    bool                        mIsTimerTriggerCheck = false;
    int                         mWriteTargetFromUsb = -1;
    int                         mTargetListNum = -1;
    int                         mCondStartIndex = 0;
    int                         mMachineStartIndex = 0;
    QString                     mTargetDirName = "";
    bool                        m_requestPowerRestart = false;
    bool                        m_isNotifyFtpGetSize = false;
    qint64                      mCurCopyFileSize = 0;
    bool                        mIsMatchFileSize = false;
    bool                        m_isNeedUpdateSetupData = false;
    bool                        m_isReOrderSetupDataListFinish = false;
    bool                        m_isReOrderSetupDataFinish = false;
    QString                     mCurSrcFilePath = "";
    bool                        mIsPowerRestart = false;
    QString                     mTargetDirPath = "";
    QString                     mSrcDirPath= "";
    QString                     mTargetFileName = "";
    bool                        mRemoveRenameFile = false;
    int                         m_curTransListIndex = -1;
    bool                        m_isNcpFileUpdate = false;
    bool                        m_isFtpFileAcquired = false;
    int                         mExtMemCopyTimeOutCnt = 0;
    bool                        m_isModbusConnect = true;
    QString                     mModbusDisconnectDate = "";
    QString                     mModbusDisconnectTime = "";
	int                         m_complieState = 0;
    int                         m_compareDiscordFactor = -1;
    bool                        mUndoNcpFileName = false;
    bool                        m_isRequestFtpFileInfo = false;
    bool                        m_isRequestGetDandoriData = false;

    const QString mImagesDirPath                    = "/opt/pendant-gui/images/";
    const QString mCommonImagesDirPath              = "/opt/pendant-gui/images/00_COMMON/";
    const QString mAplExistDirPath                  = "/opt/pendant-gui/bin/";
    const QString mSysSetAplExistDirPath            = "/opt/SystemSettingApp/bin/";
    const QString mCoreLibExistDirPath              = "/opt/SystemSettingApp/CoreLib/";
    const QString mPenMsgExistDirPath               = "/opt/pendant-gui/settings/msg_folder/";
    const QString mPenSetExistDirPath               = "/opt/pendant-gui/settings/set_folder/";
    const QString mNcpFileExistDirPath              = "/opt/pendant-gui/ftp/";
    const QString mImageFileExistDirPath            = "/opt/pendant-gui/images";
    const QString mRenameNcpFileName                = "NCSTEP.NCP";
    const QString mRenameLogo1FileName              = "splash.png";
    const QString mRenameLogo2FileName              = "logo.png";
    const QString mComSpeedSetFileExistDirPath      = "/opt/";

    void initFtpControllerViewFileList();
    void initUsbDirFileList();
    void initUsbChildDirFileList(int usbChildDirfileCnt);
    void initSettingPenSetData();
    int copyToController(QString copyFileName, QString dstFilePath, QString srcFilePath, int targetListIndex = -1);
    int copyToUsb(QString copyFileName, QString dstFilePath, QString srcFilePath, int listIndex, qint64 fileSize);
    void getFtpChildDirFileList(int childDirIndex, int listIndex);
    void getUsbChildDirFileList(int childDirIndex, int listIndex);
    void timerStart(int timeOut);
    void lockFtpPutMutex();
    void unlockFtpPutMutex();
    void lockFtpGetMutex();
    void unlockFtpGetMutex();
    void lockCursorOpeMutex();
    void unlockCursorOpeMutex();
    QString getUsbMountPath() const;
    QString decideFileNum(QFileInfoList &fileInfo, QString curDate);
    void nextWriteInBulkToUsb(int curListIndex);
    void downloadFromUsb(QString targetDirName);
    bool downloadToSd(const QString dstPath, QString srcPath, QString fileName);
    bool downloadDirToSd(const QString dstPath, QString srcPath);
    bool copyDir(const QString dstPath, QString srcPath,bool isOverWrite);
    bool copySubDir(const QString dstPath, QString srcPath);
    void asyncDownloadToSd(const QString dstPath, QString srcPath, QString fileName);
    void downloadToController(const QString dstPath, QString srcPath, QString fileName);
    void downloadBkDateData(QString targetDirName, int startIndex = 0);
    void terminateFtpTransProcess(int ftpTransMode);
    qint64 getFileSize(const QString& filePath);
    void downloadControllerUpdateData(QString targetDirName, int startIndex = 0);
    void downloadNcpData(QString targetDirName, int startIndex = 0);
    void checkTargetDir(QString& targetDirName);
    void checkTargetFile(QString& targetFileName);
    QString renameNcpFile(QString copyFileName, QString srcFilePath);
    bool downloadPenUpdateData(QString targetDirName, int startIndex = 0);
    bool downloadPenMsgData(QString targetDirName, int startIndex = 0);
//    bool downloadImageData(QString targetDirName);
    bool downloadPenSetData(QString targetDirName, int startIndex = 0);
    QString getCopyDirPath(QString targetFileName, bool *pIsRename, int *pLogoType);
    bool checkDispFilePenUpdate(QString targetFileName);
    bool checkDispFileContUpdate(QString targetFileName);
    bool checkDispFileNcp(QString targetFileName);
    bool checkDispFilePenMsg(QString targetFileName);
    bool checkDispTarget(QString parentDirName, QString targetFileName);
    bool checkDispTargetDir(QString targetDirName);
    bool checkDispTargetFile(QString targetFileName);
    bool checkCondFile(QString curFileName);
    QString renameLogoFile(QString copyFileName, QString srcFilePath, int logoType);
    bool removeRenameFile(QString fileName, QString srcFilePath);
    bool checkEffectivePowerRestart(QString targetFileName);
    void singleTimerStart(int timeInterval);
    void writeInBulkFromUsb(QString targetDirName);
    int writeIndividualFromUsb(
        ExternalMemoryViewFileList *extUsbMemoryFileList, ExternalMemoryViewFileList *extUsbMemoryChildDirFileList,
        int listIndex, bool isChildDir = false);
    void updateRemoteDirFileInfo(ExternalMemoryViewFileList *updateUsbMemoryFileList, QString dstDirPath = "/");
    void checkFtpTimeoutProcess();
    void checkFtpGetProcess();
    void checkFtpPutProcess();
    void startAsyncFileTrasnfer();
    void stopAsyncFileTrasnfer();
#ifdef ENABLE_ASYNC_COPY_TO_SD_FROM_USB
    void asyncDownloadPenUpdateData(QString targetDirName, int curIndex, bool isFirst = false);
    void asyncDownloadPenMsgData(QString targetDirName, int curIndex, bool isFirst = false);
    void initFileTransInfo(stFileTransInfo &transInfo);
#endif // ENABLE_ASYNC_COPY_TO_SD_FROM_USB
    void reConnectCommunication(bool status);
    bool undoNcpFileName(QString fileName, QString srcFilePath);

protected:
    void timerEvent(QTimerEvent *timerEvent) override;
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onUsbMounted(QString usbMountPath);
    void onUsbUnmounted();
    void onFtpStatusChanged(int ftpStatus);
    void onFtpListIndexChanged(int ftpListIndex);
    void onFtpErrorStrNotify(QString errorStr);
    void onUpdateSingleTimer();
    void onFileTransCompleteChanged(bool fileTransComplete);
    void onFtpRemoteFileInfoChanged(bool ftpRemoteFileInfo);
    void onModBusComConnectStatusChanged(int modBusComConnectStatus);
    void onSecondsChanged();
	void onCompileResultChanged(bool result);

public slots:
    void onFtpGetFinished(const QString &filePath);
    void onFtpExtMemFinishNotify(bool isToExtMem);
    void onFtpFileSizeNotify(int fileSize);

signals:
    void extFtpControllerFileListChanged(QList<QObject *> extFtpControllerFileList);
    void extUsbMemoryFileListChanged(QList<QObject *> extUsbMemoryFileList);
    void extUsbMemoryChildDirFileListChanged(QList<QObject *> extUsbMemoryChildDirFileList);
    void remoteDirFileCntChanged(int remoteDirFileCnt);
    void usbDirFileCntChanged(int usbDirFileCnt);
    void usbChildDirFileCntChanged(int usbChildDirFileCnt);
    void isUSBConnectedChanged(bool isUSBConnected);
    void isAlreadyUsbFileListChanged(bool isAlreadyUsbFileList);
    void isAlreadyUsbChildFileListChanged(bool isAlreadyUsbChildFileList);
    void processingResultChanged(bool processingResult);
    void processingTimeoutChanged(bool processingTimeout);
    void errCodeChanged(QString errCode);
    void curTabIndexChanged(int curTabIndex);
    void isUsbWriteProtectedChanged(bool isUsbWriteProtected);
    void firstUsbConnectedChanged(bool firstUsbConnected);
    void requestPowerRestartChanged(bool requestPowerRestart);
    void isNeedUpdateSetupDataChanged(bool isNeedUpdateSetupData);
    void isReOrderSetupDataListFinishChanged(bool isReOrderSetupDataListFinish);
    void isReOrderSetupDataFinishChanged(bool isReOrderSetupDataFinish);
    void isNcpFileUpdateChanged(bool isNcpFileUpdate);
    void isFtpFileAcquiredChanged(bool isFtpFileAcquired);
    void isModbusConnectChanged(bool isModbusConnect);
	void complieStateChanged(int state);
    void compareDiscordFactorChanged(int compareDiscordFactor);
    void isRequestFtpFileInfoChanged(bool isRequestFtpFileInfo);
};

#endif // EXTERNALMEMORYVIEWMODEL_H
