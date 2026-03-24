#ifndef CORELIBSDCONTROLTESTBASE_H
#define CORELIBSDCONTROLTESTBASE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <QObject>
#include <QThread>
#include <QTextCodec>
#include <QByteArray>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <sys/stat.h>
#include "corelibsdcontrol.h"
#include "corelibunittestmockmethod.h"

using namespace testing;

class CoreLibSdControlTestSub : public QThread
{
    Q_OBJECT
public:
    explicit CoreLibSdControlTestSub(QObject *parent = nullptr);
    ~CoreLibSdControlTestSub();
    CoreLibSdControl* getSdControlInstance(){
        return mSdControl;
    }
    void createTestDir();
    void createTestDir(int num);
    void createTestFile(int num);
    void deleteTestDir();
    QDateTime getCurrentDateTime();

private:
    CoreLibSdControl *mSdControl;
    const QString mTestDirPath = QDir::homePath() + "/" + "itz3000-gui/settings/test_folder";
};

class CoreLibSdControlTestBase : public ::testing::Test
{
public:
    CoreLibSdControlTestSub *testSub;
    virtual void SetUp(){
        testSub = new CoreLibSdControlTestSub();
    }
    virtual void TearDown(){
        delete testSub;
    }
    static void SetUpTestCase(){
    }
    static void TearDownTestCase(){
    }
    CoreLibSdControlTestSub* getinstace(){
        return testSub;
    }

private:
};

#endif // CORELIBSDCONTROLTESTBASE_H
