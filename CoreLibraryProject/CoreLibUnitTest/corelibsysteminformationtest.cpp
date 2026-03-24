#include "corelibsysteminformationtest.h"

CoreLibSystemInformationTestSub::CoreLibSystemInformationTestSub(QObject *parent)
    : QThread(parent)
{
    mSystemInformation = new CorelibSystemInformation();
}

CoreLibSystemInformationTestSub::~CoreLibSystemInformationTestSub()
{
    if (mSystemInformation){
        delete mSystemInformation;
        mSystemInformation = nullptr;
    }
}

#ifdef CORELIB_UNIT_TEST
#else
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemInformationTest001</testname>
    <category1>CorelibSystemInformation</category1>
    <category2>getCoreAppVersion()</category2>
    <category3>CoreLibVersion-get</category3>
    <case>
        1)getCoreAppVersion()を実行する
    </case>
    <check>
        1)getCoreAppVersion()の戻り値が"1.0.0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemInformationTestBase, SystemInformationTest001)
{
    QString coreLibVersion = "";

    CorelibSystemInformation* systemInfo = testSub->getSystemControlInstance();
    if (!systemInfo){
        qDebug("systemInfo is Null[%p]", systemInfo);
    }
    // get core lib version
    coreLibVersion = systemInfo->getCoreAppVersion();
    EXPECT_EQ(coreLibVersion, "1.0.0");
}
#endif // CORELIB_UNIT_TEST
