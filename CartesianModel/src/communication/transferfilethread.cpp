#include "transferfilethread.h"
#include "corelibsdcontrol.h"
#include <QDebug>

TransferFileThread::TransferFileThread(QObject *parent) : QThread(parent)
{

}

TransferFileThread* TransferFileThread::create_instance(QObject *parent)
{
    if(!instance){
        instance = new TransferFileThread(parent);
    }
    return instance;
}

TransferFileThread* TransferFileThread::get_instance()
{
    return instance;
}

void TransferFileThread::destroy_instance()
{
    delete instance;
    instance = nullptr;
}

void TransferFileThread::setFileTransInfo(stFileTransInfo &transInfo)
{
    m_fileTransInfo = transInfo;
}

void TransferFileThread::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()), Qt::DirectConnection);

    timer.setInterval(FILE_TRANSFER_PROCESS_INTERVAL);
    timer.start();

    exec();  // start event loop
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void TransferFileThread::setIsCopying(bool isCopying)
{
    if (mIsCopying == isCopying)
        return;

    mIsCopying = isCopying;
    emit isCopyingChanged(mIsCopying);
}

void TransferFileThread::onTimeout()
{
    // qDebug() << "TransferFileThread::onUpdateSingleTimer writeTargetFromUsb:" << m_fileTransInfo.writeTargetFromUsb
    //          << "srcFilePath:" << m_fileTransInfo.srcFilePath << "dstFilePath:" << m_fileTransInfo.dstFilePath
    //          << "targetDirName:" << m_fileTransInfo.targetDirName << "targetFileName:" << m_fileTransInfo.targetFileName
    //          << "isOverWrite:" << m_fileTransInfo.isOverWrite
    //          << "curListIndex:" << m_fileTransInfo.curListIndex << "totalListNum:" << m_fileTransInfo.totalListNum << "mCurTimeCnt:" << mCurTimeCnt;
    if (!mIsThreadOpeEnable){
        //qDebug("%s:%d mIsThreadOpeEnable[%d]", __func__, __LINE__, mIsThreadOpeEnable);
        return;
    }
    if (mCurTimeCnt >= FILE_TRANSFER_PROCESS_TIMEOUT_CNT){
        mCurTimeCnt = 0;
        m_fileTransComplete = false;
        emit fileTransCompleteChanged(m_fileTransComplete);
        return;
    } else {
        if (mIsCopying){
            //qDebug("%s:%d mIsCopying[%d]", __func__, __LINE__, mIsCopying);
            mCurTimeCnt++;
            return;
        } else {
            if(m_fileTransInfo.curListIndex >= m_fileTransInfo.totalListNum){
                mCurTimeCnt = 0;
                mIsCopying = false;
                m_fileTransComplete = true;
                emit fileTransCompleteChanged(m_fileTransComplete);
                return;
            }
        }
    }
    CoreLibSdControl coreLibSdCtrl;
    bool ret = false;
    switch(m_fileTransInfo.writeTargetFromUsb){
        case (int)Enums::UsbDownloadType::PenUpdate:
        case (int)Enums::UsbDownloadType::PenMsgData:
        case (int)Enums::UsbDownloadType::NcpData:
        case (int)Enums::UsbDownloadType::ComSpeedSetting:{
            mIsCopying = true;
            ret = coreLibSdCtrl.copyFile(m_fileTransInfo.srcFilePath + m_fileTransInfo.targetFileName,
                m_fileTransInfo.dstFilePath + m_fileTransInfo.targetFileName, m_fileTransInfo.isOverWrite);
            if (!ret){
                qWarning() << "TransferFileThread::onTimeout copyFile failed:" << ret;
                qWarning() << "failed srcFilePath:" << m_fileTransInfo.srcFilePath
                           << "dstFilePath:" << m_fileTransInfo.dstFilePath << "targetFileName:" << m_fileTransInfo.targetFileName;
                mCurTimeCnt = 0;
                m_fileTransComplete = false;
                emit fileTransCompleteChanged(m_fileTransComplete);
            } else {

            }
            break;
        }
        default:
            break;
    }
    mCurTimeCnt = 0;
    mIsCopying = false;
    m_fileTransComplete = true;
    emit fileTransCompleteChanged(m_fileTransComplete);
}

TransferFileThread* TransferFileThread::instance = nullptr;
