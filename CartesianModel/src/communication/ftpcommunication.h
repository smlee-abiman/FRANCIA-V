#ifndef FTPCOMMUNICATION_H
#define FTPCOMMUNICATION_H

#include <QObject>
#include <QDebug>
#include "corelibfiletransferprotocol.h"
#include "corelibsdcontrol.h"

class FtpCommunication : public QObject
{
    Q_OBJECT
private:
    explicit FtpCommunication(CoreLibFtpSettings *ftpSettings, QObject *parent = 0);
    ~FtpCommunication() = default;

    static FtpCommunication* instance;
public:
    static FtpCommunication* create_instance(CoreLibFtpSettings *ftpSettings, QObject *parent = 0);
    static FtpCommunication* get_instance();
    static void destroy_instance();
    CoreLibFileTransferProtocol* getCoreLibFtpInstance() { return mCoreLibFtp; }

private:
    CoreLibFtpSettings          *mCoreLibFtpSettings;
    CoreLibFileTransferProtocol *mCoreLibFtp;

private slots:
    void onFtpCommandStarted(int id);
    void onFtpCommandFinished(int id, bool error, QString errorStr);
    void onFtpRawCommandReply(int replyCode, const QString & detail);
    void onFtpStateChanged(int status);

signals:
    void ftpCommandStarted(int id);
    void ftpCommandFinished(int id, bool error, QString errorStr);
    void ftpRawCommandReply(int replyCode, const QString & detail);
    void ftpStateChanged(int status);
};

#endif // FTPCOMMUNICATION_H
