#ifndef CORELIBFILETRANSFERPROTOCOL_H
#define CORELIBFILETRANSFERPROTOCOL_H

#include <QObject>
#include <QtFtp/QFtp>
#include <QMap>
#include <QDebug>

class CoreLibFtpSettings
{
public:
    CoreLibFtpSettings(const QString userName, const QString password, const QString hostAddress, quint16 portNo = 21)
    {
        this->userName = userName;
        this->password = password;
        this->hostAddress = hostAddress;
        this->portNo = portNo;
    }
    QString getUserName(){ return userName; };
    QString getPassword(){ return password; };
    QString getHostAddress(){ return hostAddress; };
    quint16 getPortNo(){ return portNo; };

private:
    QString userName;
    QString password;
    QString hostAddress;
    quint16 portNo;
};

class CoreLibFileTransferProtocol : public QObject
{
    Q_OBJECT
public:
    // for debug.
    static QString qFtpStateToString(const int state);
    static QString qFtpCommandToString(const int command);

public:
    enum FtpTransMode {
        FtpTransModeActive = QFtp::Active,
        FtpTransModePassive = QFtp::Passive
    };
    enum FtpTransType {
        FtpTransTypeBinary = QFtp::Binary,
        FtpTransTypeAscii = QFtp::Ascii
    };
    explicit CoreLibFileTransferProtocol(CoreLibFtpSettings *ftpSettings, QObject *parent = nullptr);
    ~CoreLibFileTransferProtocol();
    int execListCommand(QString targetPath);
    QList<QUrlInfo>* getFileList();
    int execGetCommand(QIODevice *dstFile, QString remoteSrcFilePath);
    int execPutCommand(QIODevice *srcFile, QString remoteDstFilePath);
    int execRawCommand(QString rawCommandName, QStringList argument);
    int execConnectCommand();
    int execLoginCommand();
    int execCloseCommand();
    QFtp::Command getCurrentCommand();
    QString currentRawCommandName();
    int execCdCommand(QString dirPath);
    int execRemoveCommand(QString fileName);
    int execTransferModeCommand(FtpTransMode transMode);    // PORT,PASV

private:
    CoreLibFtpSettings *ftpSettings = nullptr;
    QFtp* ftp = nullptr;
    QList<QUrlInfo> remoteFileInfoList;
    QMap<int, QString> mRawCommandNameMap;
    QString mCurrentRawCommandName;

signals:
    void ftpCommandStarted(int id);
    void ftpCommandFinished(int id, bool error, QString errorStr);
    void ftpRawCommandReply(int replyCode, const QString & detail);
    void ftpStateChanged(int status);

private slots:
    void onListInfo(const QUrlInfo &);
    void onCommandStarted(int);
    void onCommandFinished(int, bool);
    void onRawCommandReply(int, const QString &);
    void onStateChanged(int);

};

#endif // CORELIBFILETRANSFERPROTOCOL_H
