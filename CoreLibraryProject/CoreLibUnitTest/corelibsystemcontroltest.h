#ifndef CORELIBSYSTEMCONTROLTEST_H
#define CORELIBSYSTEMCONTROLTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <QObject>
#include <QThread>
#include <QTextCodec>
#include <QByteArray>
#include <QTextStream>
#include <QFile>
#include "corelibsystemcontrol.h"
#include "corelibunittestmockmethod.h"

#define PATHNAME_MAX_SIZE   512

using namespace testing;

class CoreLibSystemControlTestSub : public QThread
{
    Q_OBJECT
public:
    explicit CoreLibSystemControlTestSub(QObject *parent = nullptr);
    ~CoreLibSystemControlTestSub();
    CoreLibSystemControl* getSystemControlInstance(){
        return mSystemControl;
    }
    int createEthServiceFile();
    int deleteEthServiceFile();
    bool getMACAddress(QString *macAddress, QString ifrNameStr = "eth0");

private:
    CoreLibSystemControl *mSystemControl;

};

class CoreLibSystemControlTestBase : public ::testing::Test
{
public:
    CoreLibSystemControlTestSub *testSub;
    virtual void SetUp(){
        testSub = new CoreLibSystemControlTestSub();
    }
    virtual void TearDown(){
        delete testSub;
    }
    static void SetUpTestCase(){
    }
    static void TearDownTestCase(){
    }
    CoreLibSystemControlTestSub* getinstace(){
        return testSub;
    }

private:
};

#endif // CORELIBSYSTEMCONTROLTEST_H
