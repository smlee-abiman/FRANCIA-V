#ifndef CORELIBJOGCONTROLTEST_H
#define CORELIBJOGCONTROLTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QThread>
#include "corelibjogcontrol.h"
#include "corelibunittestmockmethod.h"
#include <unistd.h>

using namespace testing;

class CoreLibJogControlTestSub : public QThread
{
    Q_OBJECT
public:
    explicit CoreLibJogControlTestSub(QObject *parent = nullptr);
    ~CoreLibJogControlTestSub();
    CoreLibJogControl* getJogControlInstance(){
        return mJogControl;
    }

private:
    CoreLibJogControl *mJogControl;

};

class CoreLibJogControlTestBase : public ::testing::Test
{
public:
    CoreLibJogControlTestSub *testSub;
    virtual void SetUp(){
        testSub = new CoreLibJogControlTestSub();
    }
    virtual void TearDown(){
        delete testSub;
    }
    static void SetUpTestCase(){
    }
    static void TearDownTestCase(){
    }
    CoreLibJogControlTestSub* getinstace(){
        return testSub;
    }

private:

};

#endif // CORELIBJOGCONTROLTEST_H
