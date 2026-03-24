#ifndef SYSTEMSETTINGMANAGER_H
#define SYSTEMSETTINGMANAGER_H

#include <QObject>
#include <QFile>
#include "folderlistmodel.h"
#include "filelistmodel.h"
#include "usbdetectthread.h"
#include "corelibfiletransferprotocol.h"
#include <QSortFilterProxyModel>

struct stRemoteDirFileInfo {
    QString name;
    QString parent;
    QString path;
    QDateTime lastModified;
    qint64 size;
    bool isDir;
};

class SystemSettingManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString selectdTopDirName READ selectdTopDirName WRITE setSelectdTopDirName NOTIFY selectdTopDirNameChanged)
    Q_PROPERTY(bool isUSBConnected READ isUSBConnected WRITE setIsUSBConnected NOTIFY isUSBConnectedChanged)
    Q_PROPERTY(QString copyDirection READ copyDirection WRITE setCopyDirection NOTIFY copyDirectionChanged)
    Q_PROPERTY(bool isCopyToUSB READ isCopyToUSB WRITE setIsCopyToUSB NOTIFY isCopyToUSBChanged)
public:
    explicit SystemSettingManager(QObject *parent = nullptr);
    virtual ~SystemSettingManager();
    void startSystemSetting();
    void endSystemSetting();
    void registerFolderListModel(FolderListModel* folderListModel);
    void registerFileListModel(FileListModel* fileListModel);
    void registerProxyModel(QSortFilterProxyModel* proxyModel);
    QString selectdTopDirName() const;
    bool isUSBConnected() const;
    QString copyDirection() const;
    bool isCopyToUSB() const;
    void setSelectdTopDirName(QString);
    void setIsUSBConnected(bool);
    void setCopyDirection(QString);
    void setIsCopyToUSB(bool);
    QString getFromAppName();
    void setFromAppName(QString fromAppName);

signals:
    void folderCreateFailed();
    void selectdTopDirNameChanged();
    void isUSBConnectedChanged();
    void copyDirectionChanged();
    void isCopyToUSBChanged();

public slots:
    void folderSelected(int index);
    void folderOpen(QString folderPath, int index);
    void onFileSelected(int index);
    void onChildFolderOpened(int index);
    void onCalibrationClicked();
    void onExecuteClicked(QString currentPath);
    void onDirectionClicked();
    void onCreateFolderClicked();
    void onEraceFolderClicked();
    void onDisplaySelectClicked();

private:
    enum CopyDirection {
        InitDirection = 0,
        UsbToController,
        ControllerToUsb
    };
    struct stFtpListArgument {
        QString controllerDirPath;
        QString parent;
    };
    struct stFtpMdtmArgument {
        QString parent;
        QString name;
        bool isPut;
    };
    struct stFtpGetArgument {
        QFile *getFile;
        int selectedIndex;
        QDateTime timestamp;
    };
    struct stFtpPutArgument {
        QFile *putFile;
        int selectedIndex;
    };
    CopyDirection m_copyDirection = CopyDirection::InitDirection;
    bool m_isCopyToUSB = false;
    FolderListModel* m_folderListModel;
    FileListModel* m_fileListModel;
    QSortFilterProxyModel* m_proxyModel;
    QString m_selectedTopDirName = "";
    bool m_isUSBConnected = false;
    UsbDetectThread *mUsbDetectThread;
    CoreLibFileTransferProtocol *m_coreLibFtp;
    QMap<int, stFtpGetArgument> mFtpGetArgMap;
    QMap<int, stFtpPutArgument> mFtpPutArgMap;
    QFile *getFile = nullptr;
    QFile *putFile = nullptr;
    QMap<int, stFtpListArgument> mFtpListArgMap;
    QMap<int, stFtpMdtmArgument> mFtpMdtmArgMap;
    QString mMdtmTargetName = "";
    QString mMdtmTargetParent = "";
    bool mMdtmIsPut = false;
    QList<stRemoteDirFileInfo> mRemoteFileList;
    QList<stRemoteDirFileInfo> mLocalFileList;
    int mFirstListId = -1;
    int mLastListId = -1;
    int mLastMdtmId = -1;
    QString mUsbRootPath = "";
    QString mFromAppName = "";
    int mFtpStatus = -1;

private slots:
    void onUsbMounted(QString usbMountPath);
    void onUsbUnmounted();
    void onFtpStateChanged(int ftpStatus);
    void onFtpCommandStarted(int id);
    void onFtpCommandFinished(int id, bool error, QString errorStr);
    void onFtpRawCommandReply(int replyCode, const QString & detail);

private:
     QString convertControllerPath(QString usbPath);
     void connectToFtp(int ftpState = QFtp::State::Unconnected);
     void requestFtpListCommand(QString targetPath);
     QString convertISOFormat(QString dateTimeStr);
     int execMdtmCommand(QString updateLastModified, QString targetName, QString parent, bool isPut);
     bool isHasDiffChildFiles(QString parent);
     bool isHasDiffFile(stRemoteDirFileInfo checkFile, bool isTargetLocal);
     QModelIndex toFileListModelIndex(int index);
};

#endif // SYSTEMSETTINGMANAGER_H
