#include "corelibfiletransferprotocol.h"

QString CoreLibFileTransferProtocol::qFtpStateToString(const int state)
{
    const QList<QString> qFtpStateStrings = {
        "QFtp::State::Unconnected",
        "QFtp::State::HostLookup",
        "QFtp::State::Connecting",
        "QFtp::State::Connected",
        "QFtp::State::LoggedIn",
        "QFtp::State::Closing"
    };
    return (state < qFtpStateStrings.size()) ? qFtpStateStrings[state]
                                             : "unknown QFtp::State";
}

QString CoreLibFileTransferProtocol::qFtpCommandToString(const int command)
{
    const QList<QString> qFtpCommandStrings = {
        "QFtp::Command::None",
        "QFtp::Command::SetTransferMode",
        "QFtp::Command::SetProxy",
        "QFtp::Command::ConnectToHost",
        "QFtp::Command::Login",
        "QFtp::Command::Close",
        "QFtp::Command::List",
        "QFtp::Command::Cd",
        "QFtp::Command::Get",
        "QFtp::Command::Put",
        "QFtp::Command::Remove",
        "QFtp::Command::Mkdir",
        "QFtp::Command::Rmdir",
        "QFtp::Command::Rename",
        "QFtp::Command::RawCommand",
    };
    return (command < qFtpCommandStrings.size()) ? qFtpCommandStrings[command]
                                                 : "unknown QFtp::Command";
}

CoreLibFileTransferProtocol::CoreLibFileTransferProtocol(CoreLibFtpSettings *ftpSettings, QObject *parent) : QObject(parent)
{
    this->ftpSettings = ftpSettings;
    ftp = new QFtp(parent);
    connect(ftp, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(onListInfo(const QUrlInfo &)));
    connect(ftp, SIGNAL(commandStarted(int)), this, SLOT(onCommandStarted(int)));
    connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(onCommandFinished(int, bool)));
    connect(ftp, SIGNAL(rawCommandReply(int, const QString &)), this, SLOT(onRawCommandReply(int, const QString &)));
    connect(ftp, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));
}

CoreLibFileTransferProtocol::~CoreLibFileTransferProtocol()
{
    if(ftp != nullptr) {
        disconnect(ftp, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(onListInfo(const QUrlInfo &)));
        disconnect(ftp, SIGNAL(commandStarted(int)), this, SLOT(onCommandStarted(int)));
        disconnect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(onCommandFinished(int, bool)));
        disconnect(ftp, SIGNAL(rawCommandReply(int, const QString &)), this, SLOT(onRawCommandReply(int, const QString &)));
        delete ftp;
        ftp = nullptr;
    }
}

void CoreLibFileTransferProtocol::onCommandStarted(int id)
{
    if(getCurrentCommand() == QFtp::RawCommand) {
        mCurrentRawCommandName = mRawCommandNameMap.take(id);
    }
    if(getCurrentCommand() == QFtp::List) {
        remoteFileInfoList.clear();
    }
    emit ftpCommandStarted(id);
}

void CoreLibFileTransferProtocol::onCommandFinished(int id, bool error)
{
    emit ftpCommandFinished(id, error, (error) ? ftp->errorString() : "");

    if(getCurrentCommand() == QFtp::RawCommand) {
        mCurrentRawCommandName = "";
    }
}

void CoreLibFileTransferProtocol::onRawCommandReply(int replyCode, const QString & detail)
{
    emit ftpRawCommandReply(replyCode, detail);
}

void CoreLibFileTransferProtocol::onStateChanged(int status)
{
    qInfo("%s:%d ftp status:[%d](%s)", __func__, __LINE__, status, qPrintable(CoreLibFileTransferProtocol::qFtpStateToString(status)));
    emit ftpStateChanged(status);
}

int CoreLibFileTransferProtocol::execConnectCommand()
{
    return ftp->connectToHost(ftpSettings->getHostAddress(), ftpSettings->getPortNo());
}

int CoreLibFileTransferProtocol::execLoginCommand()
{
    return ftp->login(ftpSettings->getUserName(), ftpSettings->getPassword());
}

int CoreLibFileTransferProtocol::execListCommand(QString targetPath)
{
    return ftp->list(targetPath);
}

void CoreLibFileTransferProtocol::onListInfo(const QUrlInfo &listInfo)
{
    remoteFileInfoList.push_back(listInfo);
}

QList<QUrlInfo>* CoreLibFileTransferProtocol::getFileList()
{
    return &remoteFileInfoList;
}

int CoreLibFileTransferProtocol::execGetCommand(QIODevice *dstFile, QString remoteSrcFilePath)
{
    return ftp->get(remoteSrcFilePath, dstFile);
}

int CoreLibFileTransferProtocol::execPutCommand(QIODevice *srcFile, QString remoteDstFilePath)
{
    return ftp->put(srcFile, remoteDstFilePath);
}

int CoreLibFileTransferProtocol::execRawCommand(QString rawCommandName, QStringList argument)
{
    QString command;
    int argCount = argument.count();
    if(argCount == 0) {
        command = rawCommandName;
    } else {
        command += rawCommandName;
        for(int i = 0; i < argCount; i++) {
            command += " ";
            command += argument.at(i);
        }
    }

    int id = ftp->rawCommand(command);
    mRawCommandNameMap.insert(id, rawCommandName);

    return id;
}

int CoreLibFileTransferProtocol::execCloseCommand()
{
    return ftp->close();
}

QFtp::Command CoreLibFileTransferProtocol::getCurrentCommand()
{
    return ftp->currentCommand();
}

QString CoreLibFileTransferProtocol::currentRawCommandName()
{
    return mCurrentRawCommandName;
}

int CoreLibFileTransferProtocol::execCdCommand(QString dirPath)
{
    return ftp->cd(dirPath);
}

int CoreLibFileTransferProtocol::execRemoveCommand(QString fileName)
{
    return ftp->remove(fileName);
}

int CoreLibFileTransferProtocol::execTransferModeCommand(FtpTransMode transMode)
{
    return ftp->setTransferMode((QFtp::TransferMode)transMode);
}
