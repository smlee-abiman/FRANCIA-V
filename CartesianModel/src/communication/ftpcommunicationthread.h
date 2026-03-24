#ifndef FTPCOMMUNICATIONTHREAD_H
#define FTPCOMMUNICATIONTHREAD_H

#include <QThread>
#include <QTimer>
#include <QLoggingCategory>
#include <QList>
#include "enums.h"
#include <QMutex>
#include <QTimerEvent>
#include "ftpcommunication.h"

class FtpCommunicationThread : public QThread
{
    Q_OBJECT
public:
    explicit FtpCommunicationThread(QObject *parent = 0);
    ~FtpCommunicationThread() = default;

    static FtpCommunicationThread* instance;

    static FtpCommunicationThread* create_instance(QObject *parent = 0) {
        if(!instance){
            instance = new FtpCommunicationThread(parent);
        }
        return instance;
    }

    static FtpCommunicationThread* get_instance() {
        return instance;
    }

    static void destroy_instance() {
        delete instance;
        instance = nullptr;
    }
    void setFtpInfo(QString userName, QString password, QString ipAddr, qint16 portNo);
    void setIntervalMiliSec(int msec);
    CoreLibFileTransferProtocol* getCoreLibFtpInstance() { return mFtpCommunication->getCoreLibFtpInstance(); }
    void run();

private:
    CoreLibFtpSettings              *mCoreLibFtpSettings;
    CoreLibFileTransferProtocol     *m_coreLibFtp;
    FtpCommunication                *mFtpCommunication;
    int                             m_intervalMiliSec;
    int                             m_ftpStatus = 0;

    void startFtpCommunication();

private slots:
    void onFtpCommandStarted(int id);
    void onFtpCommandFinished(int id, bool error, QString errorStr);
    void onFtpRawCommandReply(int replyCode, const QString & detail);
    void onFtpStateChanged(int status);

signals:

};

#endif // FTPCOMMUNICATIONTHREAD_H
