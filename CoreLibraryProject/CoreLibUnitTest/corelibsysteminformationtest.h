#ifndef CORELIBSYSTEMINFORMATIONTEST_H
#define CORELIBSYSTEMINFORMATIONTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <QObject>
#include <QThread>
#include "corelibsysteminformation.h"

using namespace testing;

class CoreLibSystemInformationTestSub : public QThread
{
    Q_OBJECT
public:
    explicit CoreLibSystemInformationTestSub(QObject *parent = nullptr);
    ~CoreLibSystemInformationTestSub();
    CorelibSystemInformation* getSystemControlInstance(){
        return mSystemInformation;
    }

private:
    CorelibSystemInformation *mSystemInformation;

};

class CoreLibSystemInformationTestBase : public ::testing::Test
{
public:
    CoreLibSystemInformationTestSub *testSub;
    virtual void SetUp(){
        testSub = new CoreLibSystemInformationTestSub();
    }
    virtual void TearDown(){
        delete testSub;
    }
    static void SetUpTestCase(){
    }
    static void TearDownTestCase(){
    }
    CoreLibSystemInformationTestSub* getinstace(){
        return testSub;
    }

private:
};

#endif // CORELIBSYSTEMINFORMATIONTEST_H
