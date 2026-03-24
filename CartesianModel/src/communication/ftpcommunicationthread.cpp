#include "ftpcommunicationthread.h"
#include <QEventLoop>
#include <QMutexLocker>

FtpCommunicationThread::FtpCommunicationThread(QObject* parent) : QThread(parent)
{

}

void FtpCommunicationThread::setFtpInfo(QString userName, QString password, QString ipAddr, qint16 portNo)
{
    mCoreLibFtpSettings = new CoreLibFtpSettings(userName, password, ipAddr, portNo);
    mFtpCommunication = FtpCommunication::create_instance(mCoreLibFtpSettings);
    //connect(mFtpCommunication, SIGNAL(ftpCommandStarted(int)), this, SLOT(onFtpCommandStarted(int)), Qt::DirectConnection);
    //connect(mFtpCommunication, SIGNAL(ftpCommandFinished(int, bool, QString)), this, SLOT(onFtpCommandFinished(int, bool, QString)), Qt::DirectConnection);
    //connect(mFtpCommunication, SIGNAL(ftpRawCommandReply(int, const QString &)), this, SLOT(onFtpRawCommandReply(int, const QString &)), Qt::DirectConnection);
    //connect(mFtpCommunication, SIGNAL(ftpStateChanged(int)), this, SLOT(onFtpStateChanged(int)), Qt::DirectConnection);
    m_coreLibFtp = mFtpCommunication->getCoreLibFtpInstance();
}

void FtpCommunicationThread::setIntervalMiliSec(int msec)
{
    m_intervalMiliSec = msec;
}

void FtpCommunicationThread::startFtpCommunication()
{
    if (!m_coreLibFtp){
        qDebug("%s:%d m_coreLibFtp[%p] is invalid", __func__, __LINE__, m_coreLibFtp);    
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

void FtpCommunicationThread::run()
{
    connect(mFtpCommunication, SIGNAL(ftpCommandStarted(int)), this, SLOT(onFtpCommandStarted(int)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpCommandFinished(int, bool, QString)), this, SLOT(onFtpCommandFinished(int, bool, QString)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpRawCommandReply(int, const QString &)), this, SLOT(onFtpRawCommandReply(int, const QString &)), Qt::DirectConnection);
    connect(mFtpCommunication, SIGNAL(ftpStateChanged(int)), this, SLOT(onFtpStateChanged(int)), Qt::DirectConnection);

    startFtpCommunication();  // connect QFtp (controller) and login

    int loopCnt = 0;
    while(m_ftpStatus != (int)QFtp::Connected && loopCnt < 300){
        QEventLoop loop;
        QTimer::singleShot( 100, &loop, SLOT( quit() ) );
        loop.exec();
        loopCnt++;
    }
    qDebug() << "FtpCommunicationThread::run loopCnt:" << loopCnt;
}

void FtpCommunicationThread::onFtpCommandStarted(int id)
{
    qDebug("%s:%d id[%d]", __func__, __LINE__, id);
    (void)id;
}

void FtpCommunicationThread::onFtpCommandFinished(int id, bool error, QString errorStr)
{
    qDebug() << "id:" << id << "error:" << error << "errorStr:" << errorStr << "currentCommand:" << m_coreLibFtp->getCurrentCommand();
    (void)error;
    (void)errorStr;

}

void FtpCommunicationThread::onFtpRawCommandReply(int replyCode, const QString & detail)
{
    qDebug() << "currentRawCommand:" << m_coreLibFtp->currentRawCommandName() << "replyCode:" << replyCode << "detail:" << detail;
}

void FtpCommunicationThread::onFtpStateChanged(int status)
{
    qDebug("%s:%d status[%d]", __func__, __LINE__, status);
    m_ftpStatus = status;
}

FtpCommunicationThread* FtpCommunicationThread::instance = nullptr;