#ifndef CORELIBJOGCONTROL_H
#define CORELIBJOGCONTROL_H

#include "corelibdrivermanager.h"
#ifdef Q_OS_LINUX
#include <assert.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#endif // Q_OS_LINUX
#ifdef CORELIB_UNIT_TEST
#include "corelibunittestmockmethod.h"
#define READ1 mMockMethod._mock_read1
#define READ2 mMockMethod._mock_read2
#define READ4 mMockMethod._mock_read4
#define READ_EVNET mMockMethod._mock_read_event
#define LSEEK mMockMethod._mock_lseek
//#define WRITE mMockMethod._mock_close
#else // CORELIB_UNIT_TEST
#define READ1 read
#define READ2 read
#define READ4 read
#define READ_EVNET read
#define LSEEK lseek
//#define CLOSE close
#endif // CORELIB_UNIT_TEST

#define USE_POLLING_JOG_COUNTER
#define EMULATED_JOG_OPERATION_AMOUNT   4

typedef struct {
    QString jogCounter;
    QString jogDirection;
} stJogInfo;

class CoreLibJogControl : public CoreLibDriverManager
{
public:
    enum JogRotationDirection
    {
        JogRotationDirectionCW = 0,
        JogRotationDirectionCCW,
    };
    CoreLibJogControl();
    ~CoreLibJogControl();
    int readJogCounterEvent(QString *jogCounter);
    int getJogCounter(QString *jogCounter);
    int getJogInfo(stJogInfo *jogInfo);
#ifdef REAL_TARGET
#else // REAL_TARGET
    void incrementEmulatedJogCount() {
        mEmulatedJogCount = mEmulatedJogCount + EMULATED_JOG_OPERATION_AMOUNT;
        mEmulatedJogRotationDirection = JogRotationDirectionCW;
    }
    void decrementEmulatedJogCount() {
        mEmulatedJogCount = mEmulatedJogCount - EMULATED_JOG_OPERATION_AMOUNT;
        mEmulatedJogRotationDirection = JogRotationDirectionCCW;
    }
#endif // REAL_TARGET

private:
    ushort jogCounterMask(int jogCounter);
    int getJogStatus(ushort *qepSts);
    int getJogRotateDirection();
    int    mPreJogCounter;
    uchar  mJogRotationDirection;
    bool mIsLimitExceedCwdirFlag;
    bool mIsLimitExceedCCwdirFlag;
#ifdef REAL_TARGET
#else // REAL_TARGET
    ushort mEmulatedJogCount = 0;
    uchar  mEmulatedJogRotationDirection = 0;
#endif // REAL_TARGET
};

#endif // CORELIBJOGCONTROL_H
