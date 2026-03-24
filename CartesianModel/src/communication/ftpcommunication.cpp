#include "ftpcommunication.h"

FtpCommunication::FtpCommunication(CoreLibFtpSettings *ftpSettings, QObject *parent)
    : QObject(parent)
{
    this->mCoreLibFtpSettings = ftpSettings;
    qDebug() << "userName:" << mCoreLibFtpSettings->getUserName()
             << "password:" << mCoreLibFtpSettings->getPassword()
             << "hostAddress:" << mCoreLibFtpSettings->getHostAddress()
             << "portNo:" << mCoreLibFtpSettings->getPortNo();
    mCoreLibFtp = new CoreLibFileTransferProtocol(ftpSettings);
    connect(mCoreLibFtp, SIGNAL(ftpCommandStarted(int)), this, SLOT(onFtpCommandStarted(int)), Qt::DirectConnection);
    connect(mCoreLibFtp, SIGNAL(ftpCommandFinished(int, bool, QString)), this, SLOT(onFtpCommandFinished(int, bool, QString)), Qt::DirectConnection);
    connect(mCoreLibFtp, SIGNAL(ftpRawCommandReply(int, const QString &)), this, SLOT(onFtpRawCommandReply(int, const QString &)), Qt::DirectConnection);
    connect(mCoreLibFtp, SIGNAL(ftpStateChanged(int)), this, SLOT(onFtpStateChanged(int)), Qt::DirectConnection);
}

FtpCommunication* FtpCommunication::create_instance(CoreLibFtpSettings *ftpSettings, QObject *parent) {
    if(!instance){
        instance = new FtpCommunication(ftpSettings, parent);
    }
    return instance;
}

FtpCommunication* FtpCommunication::get_instance() {
    return instance;
}

void FtpCommunication::destroy_instance() {
    delete instance;
    instance = nullptr;
}

void FtpCommunication::onFtpCommandStarted(int id)
{
    emit ftpCommandStarted(id);
}

void FtpCommunication::onFtpCommandFinished(int id, bool error, QString errorStr)
{
    emit ftpCommandFinished(id, error, errorStr);
}

void FtpCommunication::onFtpRawCommandReply(int replyCode, const QString & detail)
{
    emit ftpRawCommandReply(replyCode, detail);
}

void FtpCommunication::onFtpStateChanged(int status)
{
    emit ftpStateChanged(status);
}

FtpCommunication* FtpCommunication::instance = nullptr;
