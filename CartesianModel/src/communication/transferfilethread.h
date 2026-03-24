#ifndef TRANSFERFILETHREAD_H
#define TRANSFERFILETHREAD_H

#include <QThread>
#include <QTimer>
#include "enums.h"

#define FILE_TRANSFER_PROCESS_INTERVAL      100
#define FILE_TRANSFER_PROCESS_TIMEOUT_CNT   100

typedef struct {
    //int     timerMode;
    int     writeTargetFromUsb;
    QString srcFilePath;
    QString dstFilePath;
    QString targetDirName;
    QString targetFileName;
    bool    isOverWrite;
    int     curListIndex;
    int     totalListNum;
} stFileTransInfo;

class TransferFileThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool fileTransComplete MEMBER m_fileTransComplete NOTIFY fileTransCompleteChanged)
    Q_PROPERTY(bool isCopying READ isCopying WRITE setIsCopying NOTIFY isCopyingChanged)
private:
    explicit TransferFileThread(QObject *parent = 0);
    ~TransferFileThread() = default;

    static TransferFileThread* instance;
    bool    m_fileTransComplete = false;
    stFileTransInfo m_fileTransInfo;
    bool    mIsCopying = false;
    int     mCurTimeCnt = 0;
    bool    mIsThreadOpeEnable = false;

public:
    //enum TransTimerMode {
    //    TransTimerModeSingle = 0,
    //    TransTimerModePeriodic,
    //};
    static TransferFileThread* create_instance(QObject *parent = 0);
    static TransferFileThread* get_instance();
    static void destroy_instance();
    void setFileTransInfo(stFileTransInfo &transInfo);
    void run();
    bool isCopying() const { return mIsCopying; }
    void setIsCopying(bool isCopying);
    void setThreadOpeEnable(bool isThreadOpeEnable) { mIsThreadOpeEnable = isThreadOpeEnable; };

private slots:
    void onTimeout();

signals:
    void fileTransCompleteChanged(bool fileTransComplete);
    void isCopyingChanged(bool isCopying);
};

#endif // TRANSFERFILETHREAD_H
