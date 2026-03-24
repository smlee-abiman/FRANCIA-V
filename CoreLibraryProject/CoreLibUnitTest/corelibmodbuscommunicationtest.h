#ifndef CORELIBMODBUSCOMMUNICATIONTEST_H
#define CORELIBMODBUSCOMMUNICATIONTEST_H

#include <cstring>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QThread>
#include "corelibmodbuscommunication.h"


using namespace testing;

class CoreLibModBusCommunicationTestSub : public QThread
{
    Q_OBJECT
public:
    explicit CoreLibModBusCommunicationTestSub(QObject *parent = nullptr);
    ~CoreLibModBusCommunicationTestSub();
    stModBusInitInfo& getModBusInitInfo(){
        return mModBusInitInfo;
    }
    CoreLibModBusCommunication* getModBusCom(){
        return mModBusCom;
    }
    stModBusCommunicationData* getModBusReadComData(){
        return &mModBusReadComData;
    }
    stModBusCommunicationData getModBusReadComData2(){
        return mModBusReadComData;
    }
    void connetStatusChangedFunc();
    int getModBusState();
    void connetStatusChangedEventLoop();
    bool connetReadStatusChangedFunc();
    bool connetReadWriteStatusChangedFunc();
    void connetReadStatusChangedEventLoop();

private:
    stModBusInitInfo mModBusInitInfo;
    CoreLibModBusCommunication *mModBusCom;
    int modBusState;
    stModBusCommunicationData mModBusReadComData;
    bool mIsBitMemory;

public slots:
    void modBusConnectStatusChanged(int state);
    void modBusReadStatusChanged(int state);
    void modBusBitMemoryDataChanged(QBitArray bitMemoryData);
    void modBusMemoryDataChanged(stModBusCommunicationData modBusMemData);

signals:
    void modBusConnectStatusChangedNotify();
};

class CoreLibModBusCommunicationTestBase : public ::testing::Test
{

public:
    CoreLibModBusCommunicationTestSub *testSub;
    virtual void SetUp(){
        testSub = new CoreLibModBusCommunicationTestSub();
    }
    virtual void TearDown(){
        delete testSub;
    }
    static void SetUpTestCase(){
    }
    static void TearDownTestCase(){
    }
    CoreLibModBusCommunicationTestSub* getinstace(){
        return testSub;
    }

private:


};

#endif // CORELIBMODBUSCOMMUNICATIONTEST_H
