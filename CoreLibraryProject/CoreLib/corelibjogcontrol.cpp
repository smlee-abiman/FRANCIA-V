#include "corelibjogcontrol.h"

CoreLibJogControl::CoreLibJogControl()
    :mPreJogCounter(0),
     mJogRotationDirection(0),
     mIsLimitExceedCwdirFlag(false),
     mIsLimitExceedCCwdirFlag(false)
{

}

CoreLibJogControl::~CoreLibJogControl()
{

}

#ifdef REAL_TARGET
int CoreLibJogControl::readJogCounterEvent(QString *jogCounter)
{
    int ret = 0;
    ushort jogCounterMaskNum = 0;
    struct input_event event;

    memset(&event, 0, sizeof(struct input_event));
    ret = READ_EVNET(mDevFd[DriverControlTypeJogInputEvent], &event, sizeof(event));
    if (ret != sizeof(event)){
        qCritical() << "dev file read error " << ret << "errno: "<< errno << "sizeof(event):" << sizeof(event);
        return -1;
    }
    if (event.type != 0){
        ret = getJogRotateDirection();
        if (ret < 0){
            qCritical("getJogRotateDirection failed ret[%d]\n", ret);
            return ret;
        }
        qDebug("tv_sec: %lu, type: %04x, code: %04x, value: %08x\n", event.time.tv_sec, event.type, event.code, event.value);
        jogCounterMaskNum = jogCounterMask((int)event.value);
        qDebug("readJogCounterEvent:jogCounterMaskNum[%d]\n", jogCounterMaskNum);
        *jogCounter = QString::number(jogCounterMaskNum);
    }
    return ret;
}

int CoreLibJogControl::getJogCounter(QString *jogCounter)
{
    int qposCnt = 0;
    ushort jogCounterMaskNum = 0;
    int ret = -1;

    ret = LSEEK(mDevFd[DriverControlTypeJogQPosCntControl], 0, SEEK_SET);
    if (ret < 0){
        qCritical("%s:%d dev file seek error ret[%d] errno[%d]", __func__, __LINE__, ret, errno);
        return ret;
    }
    ret = READ4(mDevFd[DriverControlTypeJogQPosCntControl], &qposCnt, 4);
    if (ret < 0){
        qCritical() << "dev file read error " << ret << "errno: "<< errno;
        return ret;
    }
    //qDebug("qposCnt[0x%08x]", qposCnt);
    ret = getJogRotateDirection();
    if (ret < 0){
        qCritical("getJogRotateDirection failed ret[%d]\n", ret);
        return ret;
    }
    jogCounterMaskNum = jogCounterMask(qposCnt);
    *jogCounter = QString::number(jogCounterMaskNum);
    //qDebug() << "jogCounter:" << *jogCounter;
    return ret;
}
int CoreLibJogControl::getJogInfo(stJogInfo *jogInfo)
{
    int ret = 0;
    if (!jogInfo){
        return ret;
    }
    ret = getJogCounter(&jogInfo->jogCounter);
    if (ret < 0){
        qCritical("%s:%d getJogCounter failed[%d]", __func__, __LINE__, ret);
        return ret;
    }
    jogInfo->jogDirection = QString::number(mJogRotationDirection);
    return ret;
}
#else
#ifdef Q_OS_LINUX
int CoreLibJogControl::readJogCounterEvent(QString *jogCounter)
{
    int ret = 0;
#ifdef CORELIB_UNIT_TEST
    ushort jogCounterMaskNum = 0;
    struct input_event event;

    memset(&event, 0, sizeof(struct input_event));
    ret = READ_EVNET(mDevFd[DriverControlTypeJogInputEvent], &event, sizeof(event));
    if (ret != sizeof(event)){
        qCritical() << "dev file read error " << ret << "errno: "<< errno << "sizeof(event):" << sizeof(event);
        return -1;
    }
    if (event.type != 0){
        ret = getJogRotateDirection();
        if (ret < 0){
            qCritical("getJogRotateDirection failed ret[%d]\n", ret);
            return ret;
        }
        qDebug("tv_sec: %lu, type: %04x, code: %04x, value: %08x\n", event.time.tv_sec, event.type, event.code, event.value);
        jogCounterMaskNum = jogCounterMask((int)event.value);
        qDebug("readJogCounterEvent:jogCounterMaskNum[%d]\n", jogCounterMaskNum);
        *jogCounter = QString::number(jogCounterMaskNum);
    }
#else
    *jogCounter = QString::number(mEmulatedJogCount);
#endif // CORELIB_UNIT_TEST
    return ret;
}

int CoreLibJogControl::getJogCounter(QString *jogCounter)
{
#ifdef CORELIB_UNIT_TEST
    int qposCnt = 0;
    ushort jogCounterMaskNum = 0;
    int ret = -1;

    ret = LSEEK(mDevFd[DriverControlTypeJogQPosCntControl], 0, SEEK_SET);
    if (ret < 0){
        qCritical("%s:%d dev file seek error ret[%d] errno[%d]", __func__, __LINE__, ret, errno);
        return ret;
    }
    ret = READ4(mDevFd[DriverControlTypeJogQPosCntControl], &qposCnt, 4);
    if (ret < 0){
        qCritical() << "dev file read error " << ret << "errno: "<< errno;
        return ret;
    }
    qDebug("qposCnt[0x%08x]", qposCnt);
    ret = getJogRotateDirection();
    if (ret < 0){
        qCritical("getJogRotateDirection failed ret[%d]\n", ret);
        return ret;
    }
    jogCounterMaskNum = jogCounterMask(qposCnt);
    *jogCounter = QString::number(jogCounterMaskNum);
#else // CORELIB_UNIT_TEST
    int ret = 0;
    *jogCounter = QString::number(mEmulatedJogCount);
#endif // CORELIB_UNIT_TEST
    qDebug() << "jogCounter:" << *jogCounter;
    return ret;
}
int CoreLibJogControl::getJogInfo(stJogInfo *jogInfo)
{
    int ret = 0;
    if (!jogInfo){
        return ret;
    }
#ifdef CORELIB_UNIT_TEST
    ret = getJogCounter(&jogInfo->jogCounter);
    if (ret < 0){
        qCritical("%s:%d getJogCounter failed[%d]", __func__, __LINE__, ret);
        return ret;
    }
    jogInfo->jogDirection = QString::number(mJogRotationDirection);
#else // CORELIB_UNIT_TEST
    jogInfo->jogCounter = QString::number(mEmulatedJogCount);
    jogInfo->jogDirection = QString::number(mEmulatedJogRotationDirection);
#endif // CORELIB_UNIT_TEST
    return ret;
}
#else
int CoreLibJogControl::readJogCounterEvent(QString *jogCounter)
{
    int ret = 0;
    *jogCounter = QString::number(mEmulatedJogCount);
    return ret;
}

int CoreLibJogControl::getJogCounter(QString *jogCounter)
{
    int ret = 0;
    *jogCounter = QString::number(mEmulatedJogCount);
    return ret;
}
int CoreLibJogControl::getJogInfo(stJogInfo *jogInfo)
{
    int ret = 0;
    if (!jogInfo){
        return ret;
    }
    jogInfo->jogCounter = QString::number(mEmulatedJogCount);
    jogInfo->jogDirection = QString::number(mEmulatedJogRotationDirection);
    return ret;
}
#endif // Q_OS_LINUX
#endif // REAL_TARGET

ushort CoreLibJogControl::jogCounterMask(int jogCounter)
{
    ushort jogCounterMaskNum = 0;
    uint curJogCounter = 0;

    curJogCounter = (uint)jogCounter;
    if (mJogRotationDirection == JogRotationDirectionCW){
        if (mIsLimitExceedCCwdirFlag){
            if (curJogCounter > 0x0000FFFF && curJogCounter < 0xFFFF0000) {
                curJogCounter = 0x00000000;
            } else {
                mIsLimitExceedCCwdirFlag = false;
            }
        } else {
            if (curJogCounter > 0x0000FFFF && curJogCounter < 0xFFFF0000){
                curJogCounter = 0x0000FFFF;
                mIsLimitExceedCwdirFlag = true;
            }
        }
    } else {
        if (mIsLimitExceedCwdirFlag) {
            if (curJogCounter > 0x0000FFFF) {
                curJogCounter = 0x0000FFFF;
            } else {
                mIsLimitExceedCwdirFlag = false;
            }
        } else {
            if (curJogCounter > 0x0000FFFF && curJogCounter < 0xFFFF0000){
                curJogCounter = 0x00000000;
                mIsLimitExceedCCwdirFlag = true;
            }
        }
    }
    jogCounterMaskNum = (ushort)(curJogCounter & 0x0000FFFF);
    //qDebug("jogCounterMaskNum[0x%04x]", jogCounterMaskNum);
    return jogCounterMaskNum;
}

int CoreLibJogControl::getJogStatus(ushort *qepSts)
{
    int ret = -1;
    if (!qepSts){
        return ret;
    }
    ret = LSEEK(mDevFd[DriverControlTypeJogQepStsControl], 0, SEEK_SET);
    if (ret < 0){
        qCritical("%s:%d dev file seek error ret[%d] errno[%d]", __func__, __LINE__, ret, errno);
        return ret;
    }
    ret = READ2(mDevFd[DriverControlTypeJogQepStsControl], qepSts, 2);
    if (ret < 0){
        qCritical() << "dev file read error " << ret << "errno: "<< errno;
        return ret;
    }
    //qDebug("qepSts[0x%04x]", *qepSts);
    return ret;
}

int CoreLibJogControl::getJogRotateDirection()
{
    int ret = -1;
    ushort qepSts = 0;

    // Jog Status
    ret = getJogStatus(&qepSts);
    if (ret < 0){
        qCritical("%s:%d getJogStatus failed[%d]", __func__, __LINE__, ret);
        return ret;
    }
    mJogRotationDirection = (((qepSts & 0x0020) == 0x0020) ? JogRotationDirectionCW : JogRotationDirectionCCW);
    //qDebug("mJogRotationDirection[%d]", mJogRotationDirection);
    return 0;
}
