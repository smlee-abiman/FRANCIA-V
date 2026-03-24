#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include <QTimerEvent>
#include "enums.h"
#include "ftpcommunication.h"
#include "corelibsdcontrol.h"
#include "appenvironment.h"
#include "ftpcommunicationthread.h"
#include "resourcemanager.h"

struct stRemoteDirFileInfo {
    QString name;
    QString parent;
    QString path;
    QDateTime lastModified;
    qint64 size;
    bool isDir;
};

struct stFtpGetArgument {
    QFile *getFile;
    int selectedIndex;
    QDateTime timestamp;
    bool isToExtMem;
    int fileInfo;
};

struct stFtpPutArgument {
    QFile *putFile;
    int selectedIndex;
    int targetListNum;
};

const QList<QString> STARTUP_FTP_FILE_NAME_LIST = {
    "DANDORI.CSV",
    "ALMHIS1.DAT",
    "ALMHIS2.DAT",
    "OPE2HIS.DAT",
    "IOHIS.DAT",
    "NCSTEP.NCP",
};

#define FTP_CONNETCTED_TIMEOUT      300
#define FTP_LOGIN_TIMEOUT           300
#define FTP_GET_LIST_TIMEOUT       2000
#define FTP_GET_FILE_TIMEOUT       2000
#define FTP_COM_THREAD_CYCLE_MONITOR_TIME   50

class FtpManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ftpStatus READ ftpStatus WRITE setFtpStatus NOTIFY ftpStatusChanged)
    Q_PROPERTY(int ftpListIndex READ ftpListIndex WRITE setFtpListIndex NOTIFY ftpListIndexChanged)
    Q_PROPERTY(bool isStartupFtpStatus READ isStartupFtpStatus WRITE setIsStartupFtpStatus NOTIFY isStartupFtpStatusChanged)
    Q_PROPERTY(bool isDeterNoopCmd READ isDeterNoopCmd WRITE setIsDeterNoopCmd NOTIFY isDeterNoopCmdChanged)
    Q_PROPERTY(int ftpFileInfo MEMBER m_ftpFileInfo NOTIFY ftpFileInfoChanged)
    Q_PROPERTY(bool ftpRemoteFileInfo MEMBER m_ftpRemoteFileInfo NOTIFY ftpRemoteFileInfoChanged)
private:
    explicit FtpManager(QObject *parent = 0);
    ~FtpManager() = default;

    static FtpManager* instance;
public:
    enum FtpFileInfo {
        FTP_FILE_INVALID = -1,
        FTP_DANDORI_CSV = 0,
        FTP_ALM_HIS1_DAT,
        FTP_ALM_HIS2_DAT,
        FTP_OPE_HIS_DAT,
        FTP_IO_HIS_DAT,
        FTP_NCSTEP_NCP,
        FTP_FILE_INFO_MAX,
    };
    static FtpManager* create_instance(QObject *parent = 0);
    static FtpManager* get_instance();
    static void destroy_instance();

    Q_INVOKABLE void folderSelected(int index);
    Q_INVOKABLE void startFtpCommunication();
    Q_INVOKABLE void stopFtpCommunication();
    Q_INVOKABLE int execFtpCommand(Enums::FtpCommand ftpCommad, QIODevice *ioDevFile, QString targetPath, int byteNum = 0);
    Q_INVOKABLE int getFtpConnectStatus() { return m_ftpStatus; }
    Q_INVOKABLE void changeDirFtpCommunication(QString targetPath);
    Q_INVOKABLE void getFileListFtpCommunication();
    Q_INVOKABLE void asyncStartupFtpProcess();
    Q_INVOKABLE void timerStart(int timeOut);
    Q_INVOKABLE void timerStop();
    Q_INVOKABLE void restartFtpCommunication();
    void requestFtpListCommand(QString targetPath);
    void getFtpFileInfo(FtpManager::FtpFileInfo fileInfo);
    QFile* getFtpFileData(FtpManager::FtpFileInfo fileInfo);
    QString getFtpFilePath(FtpManager::FtpFileInfo fileInfo);

    void setFtpStatus(int ftpStatus);
    int ftpStatus() const { return m_ftpStatus;}

    QList<stRemoteDirFileInfo> getRemoteFileInfo() { return mRemoteFileList; }
    void setFtpGetArgment(int cmdId, stFtpGetArgument &ftpGetArg);
    void setFtpPutArgment(int cmdId, stFtpPutArgument &ftpPutArg);

    void setFtpListIndex(int ftpListIndex);
    int ftpListIndex() const { return m_ftpListIndex;}
    void startFtpLogin();
    void startupFtpProcess();

    void setIsStartupFtpStatus(bool isStartupFtpStatus);
    bool isStartupFtpStatus() const { return m_isStartupFtpStatus;}
    void updateRemoteFileInfo(stRemoteDirFileInfo &updateInfo);
    void updateRemoteFileInfo(int opeMode, stRemoteDirFileInfo &updateInfo);

    void setIsDeterNoopCmd(bool isDeterNoopCmd);
    bool isDeterNoopCmd() const { return m_isDeterNoopCmd; }
    int getFtpTimeOut() { return mFtpResourceTimeOut; };
    void setFtpTimeOut(ResourceManager* resourceManager);

    void setIsStartupFtpGetFileList(bool isStartupFtpGetFileList);
    bool isStartupFtpGetFileList() { return mIsStartupFtpGetFileList; }
    void startFtpCommunicationThread();
    void createFtpCommunication();
    void terminate();
    void sortRemoteFile();

private:
    struct stFtpListArgument {
        QString controllerDirPath;
        QString parent;
    };
    struct stFtpMdtmArgument {
        QString parent;
        QString name;
        bool isPut;
    };
    CoreLibFtpSettings              *mCoreLibFtpSettings = nullptr;
    CoreLibFileTransferProtocol     *m_coreLibFtp = nullptr;
    FtpCommunication                *mFtpCommunication = nullptr;
    FtpCommunicationThread          *m_ftpCommunicationThread;  // Instance for data handling worker running on the other thread.
    QMap<int, stFtpListArgument>    mFtpListArgMap;
    QMap<int, stFtpMdtmArgument>    mFtpMdtmArgMap;
    QMap<int, stFtpGetArgument>     mFtpGetArgMap;
    QMap<int, stFtpPutArgument>     mFtpPutArgMap;
    QList<stRemoteDirFileInfo>      mRemoteFileList;
    QList<QString>                  mRemoteFileNameList;
    int                             mFirstListId = -1;
    int                             mLastListId = -1;
    int                             m_ftpStatus = 0;
    QFile                           *mGetFile[FtpFileInfo::FTP_FILE_INFO_MAX];
    QString                         mTargetPath = "";
    int                             m_ftpListIndex = -1;
    bool                            mIsStartupFtpGetFileList = false;
    bool                            m_isStartupFtpStatus = false;
    bool                            m_isDeterNoopCmd = false;
    int                             mTimerId;
    bool                            mIsTimerStatus = false;
    int                             mTimerCnt = 0;
    bool                            mIsGetFtpList = false;
    bool                            m_isGetFinNotify = false;
    int                             m_ftpFileInfo = -1;
    int                             mFtpResourceTimeOut = 0;                         // 秒単位
    bool                            m_ftpRemoteFileInfo = false;

    int execNoopCommand();
    void parseRemoteFileList(QList<QUrlInfo> *fileList, stFtpListArgument &arg);
    int execPwdCommand();
    int execXpwdCommand();
    int execAlloCommand(int byteNum);
    int execSizeCommand(QString targetPath);
    void checkStartupFtpProcess();

protected:
    void timerEvent(QTimerEvent *timerEvent);

private slots:
    void onFtpCommandStarted(int id);
    void onFtpCommandFinished(int id, bool error, QString errorStr);
    void onFtpRawCommandReply(int replyCode, const QString & detail);
    void onFtpStateChanged(int status);

signals:
    void ftpStatusChanged(int ftpStatus);
    void ftpListIndexChanged(int ftpListIndex);
    void ftpCurDirNotify(QString curDir);
    void ftpFileSizeNotify(int fileSize);
    void ftpDirSizeNotify(int dirSize);
    void ftpAlloResultNotify(bool result);
    void isStartupFtpStatusChanged(bool isStartupFtpStatus);
    void ftpErrorStrNotify(QString errorStr);
    void ftpGetFinished(const QString &filePath);
    void ftpExtMemFinishNotify(bool isToExtMem);
    void isDeterNoopCmdChanged(bool isDeterNoopCmd);
    void ftpFileInfoChanged(int ftpFileInfo);
    void ftpRemoteFileInfoChanged(bool ftpRemoteFileInfo);
    void isStartupFtpGetFileListChanged(bool isStartupFtpGetFileList);
};

#endif // FTPMANAGER_H
