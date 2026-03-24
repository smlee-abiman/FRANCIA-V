#ifndef CORELIBFILETRANSFERPROTOCOLTEST_H
#define CORELIBFILETRANSFERPROTOCOLTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QObject>
#include <QString>
#include <QDebug>
#include "corelibfiletransferprotocol.h"

using namespace testing;

class CoreLibFileTransferProtocolTestBase : public ::testing::Test
{

public:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }
    static void SetUpTestCase(){
    }
    static void TearDownTestCase(){
    }
protected:

private:

};

#endif // CORELIBFILETRANSFERPROTOCOLTEST_H
