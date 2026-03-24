#ifndef CORELIBGPIOCONTROLTEST_H
#define CORELIBGPIOCONTROLTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <QObject>
#include <QThread>
#include <QBitArray>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "corelibgpiocontrol.h"
#include "corelibunittestmockmethod.h"

using namespace testing;

class CoreLibGpioControlTestSub : public QThread
{
    Q_OBJECT
public:
    explicit CoreLibGpioControlTestSub(QObject *parent = nullptr);
    ~CoreLibGpioControlTestSub();
    CoreLibGpioControl* getGpioControlInstance(){
        return mGpioControl;
    }
#ifdef CORELIB_UNIT_TEST
    int createLedDevFile(CoreLibDriverManager::LedType);
    bool confirmLedDevFile(bool, CoreLibDriverManager::LedType);
#endif // CORELIB_UNIT_TEST

private:
    CoreLibGpioControl *mGpioControl;

};

class CoreLibGpioControlTestBase : public ::testing::Test
{
public:
    CoreLibGpioControlTestSub *testSub;
    virtual void SetUp(){
        testSub = new CoreLibGpioControlTestSub();
    }
    virtual void TearDown(){
        delete testSub;
    }
    static void SetUpTestCase(){
    }
    static void TearDownTestCase(){
    }
    CoreLibGpioControlTestSub* getinstace(){
        return testSub;
    }

private:

};

#endif // CORELIBGPIOCONTROLTEST_H
