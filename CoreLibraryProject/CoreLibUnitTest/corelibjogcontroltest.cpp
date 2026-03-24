#include "corelibjogcontroltest.h"
#include "corelibunittestmockmethod.h"
#include <QDebug>

CoreLibJogControlTestSub::CoreLibJogControlTestSub(QObject *parent)
    : QThread(parent)
{
    mJogControl = new CoreLibJogControl();
}

CoreLibJogControlTestSub::~CoreLibJogControlTestSub()
{
    if (mJogControl){
        delete mJogControl;
        mJogControl = nullptr;
    }
}

#ifdef CORELIB_UNIT_TEST
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest001</testname>
    <category1>CoreLibJogControl</category1>
    <category2>openDevice()</category2>
    <category3>JOGドライバイベント通知用デバイスファイルオープン</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest001)
{
    QString devFilePath = "/tmp/input/event1";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    EXPECT_EQ(true, ret);
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest002</testname>
    <category1>CoreLibJogControl</category1>
    <category2>readJogCounterEvent()</category2>
    <category3>JOGドライバイベント通知用デバイスファイル読み出し</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)event情報にJOGカウンタ値(0x000000004=4)を設定しておく
        3)openDevice()を実行する
        4)readJogCounterEvent()を実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"4"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest002)
{
    struct input_event event = {{0, 0}, 4, 0, 0x00000004};
    int result = 0;
    ushort eQpSts = 0x00A0;
    QString jogCounter = "";
    QString devFilePath = "/tmp/input/event1";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read_event(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(event), Return(24)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    EXPECT_EQ(true, ret);
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "result:" << result;
    EXPECT_EQ(jogCounter, "4");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest003</testname>
    <category1>CoreLibJogControl</category1>
    <category2>readJogCounterEvent()</category2>
    <category3>JOGドライバイベント通知用デバイスファイル読み出し</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)event情報にJOGカウンタ値(0x000010000=65536)を設定しておく
        3)回転方向をCWに設定しておく(0x0020)
        4)openDevice()を実行する
        5)readJogCounterEvent()を実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"65535"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest003)
{
    struct input_event event = {{0, 0}, 4, 0, 0x00010000};
    int result = 0;
    ushort eQpSts = 0x0020;
    QString jogCounter = "";
    QString devFilePath = "/tmp/input/event1";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read_event(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(event), Return(24)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    EXPECT_EQ(true, ret);
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "result:" << result;
    EXPECT_EQ(jogCounter, "65535");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest004</testname>
    <category1>CoreLibJogControl</category1>
    <category2>readJogCounterEvent()</category2>
    <category3>JOGドライバイベント通知用デバイスファイル読み出し</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)event情報にJOGカウンタ値(0x000010000=65536)を設定しておく
        3)回転方向をCCWに設定しておく(0x0000)
        4)openDevice()を実行する
        5)readJogCounterEvent()を実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"0"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest004)
{
    struct input_event event = {{0, 0}, 4, 0, 0x00010000};
    int result = 0;
    ushort eQpSts = 0x0000;
    QString jogCounter = "";
    QString devFilePath = "/tmp/input/event1";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read_event(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(event), Return(24)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    EXPECT_EQ(true, ret);
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "result:" << result;
    qDebug() << "jogCounter:" << jogCounter;
    EXPECT_EQ(jogCounter, "0");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest005</testname>
    <category1>CoreLibJogControl</category1>
    <category2>readJogCounterEvent()</category2>
    <category3>JOGドライバイベント通知用デバイスファイル読み出し</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)event情報にJOGカウンタ値(0x000010000=65536)を設定しておく
        3)event2情報にJOGカウンタ値(0x000010004=65540)を設定しておく
        4)回転方向をCWに設定しておく(0x0020)
        5)openDevice()を実行する
        6)readJogCounterEvent()を2回実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"65535"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest005)
{
    struct input_event event = {{0, 0}, 4, 0, 0x00010000};
    struct input_event event2 = {{0, 0}, 4, 0, 0x00010004};
    int result = 0;
    ushort eQpSts = 0x0020;
    QString jogCounter = "";
    QString devFilePath = "/tmp/input/event1";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read_event(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(event), Return(24)))
                    .WillOnce(DoAll(SetArgPointee<1>(event2), Return(24)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    EXPECT_EQ(true, ret);
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "jogCounter:" << jogCounter;
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "result:" << result;
    qDebug() << "jogCounter:" << jogCounter;
    EXPECT_EQ(jogCounter, "65535");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest006</testname>
    <category1>CoreLibJogControl</category1>
    <category2>readJogCounterEvent()</category2>
    <category3>JOGドライバイベント通知用デバイスファイル読み出し</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)event情報にJOGカウンタ値(0x0000FFFF=65535)を設定しておく
        3)event2情報にJOGカウンタ値(0x000010000=65536)を設定しておく
        4)回転方向をCCWに設定しておく(0x0000)
        5)openDevice()を実行する
        6)readJogCounterEvent()を2回実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"0"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest006)
{
    struct input_event event = {{0, 0}, 4, 0, 0x0000FFFF};
    struct input_event event2 = {{0, 0}, 4, 0, 0x00010000};
    int result = 0;
    ushort eQpSts = 0x0000;
    QString jogCounter = "";
    QString devFilePath = "/tmp/input/event1";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read_event(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(event), Return(24)))
                    .WillOnce(DoAll(SetArgPointee<1>(event2), Return(24)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    EXPECT_EQ(true, ret);
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "jogCounter:" << jogCounter;
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "result:" << result;
    qDebug() << "jogCounter:" << jogCounter;
    EXPECT_EQ(jogCounter, "0");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>JogControlTest007</testname>
    <category1>CoreLibJogControl</category1>
    <category2>readJogCounterEvent()</category2>
    <category3>JOGドライバイベント通知用デバイスファイル読み出し</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)event情報にJOGカウンタ値(0x0000FFFF=65535)を設定しておく
        3)read_eventの戻り値を22に設定しておく
        4)openDevice()を実行する
        5)readJogCounterEvent()を実行する
    </case>
    <check>
        1)readJogCounterEventの戻り値が-1であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest007)
{
    int result = 0;
    struct input_event event = {{0, 0}, 4, 0, 0x0000FFFF};
    QString jogCounter = "";
    QString devFilePath = "/tmp/input/event1";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read_event(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(event), Return(22)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    EXPECT_EQ(true, ret);
    result = jogControl->readJogCounterEvent(&jogCounter);
    EXPECT_LT(result, 0);
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>JogControlTest008</testname>
    <category1>CoreLibJogControl</category1>
    <category2>readJogCounterEvent()</category2>
    <category3>JOGドライバイベント通知用デバイスファイル読み出し</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)event情報にJOGカウンタ値(0x0000FFFF=65535),event_type=4を設定しておく
        3)event2情報にJOGカウンタ値(0x0000FFFB=65531),event_type=0を設定しておく
        4)openDevice()を実行する
        5)readJogCounterEvent()を実行する
    </case>
    <check>
        1)JOGカウント文字列が"65535"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest008)
{
    int result = 0;
    struct input_event event = {{0, 0}, 4, 0, 0x0000FFFF};
    struct input_event event2 = {{0, 0}, 0, 0, 0x0000FFFB};
    ushort eQpSts = 0x0000;
    QString jogCounter = "";
    QString devFilePath = "/tmp/input/event1";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read_event(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(event), Return(24)))
                    .WillOnce(DoAll(SetArgPointee<1>(event2), Return(24)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    EXPECT_EQ(true, ret);
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "jogCounter:" << jogCounter;
    result = jogControl->readJogCounterEvent(&jogCounter);
    qDebug() << "result:" << result;
    qDebug() << "jogCounter:" << jogCounter;
    EXPECT_EQ(jogCounter, "65535");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest009</testname>
    <category1>CoreLibJogControl</category1>
    <category2>openDevice()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control"を生成しないでおく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest009)
{
    QString devFilePath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";
    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    EXPECT_EQ(true, ret);
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest010</testname>
    <category1>CoreLibJogControl</category1>
    <category2>getJogCounter()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control"を生成しないでおく
        2)qPosCnt情報にJOGカウンタ値(0x000000004=4)を設定しておく
        3)openDevice()を実行する
        4)getJogCounter()を実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"4"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest010)
{
    int result = 0;
    ushort eQpSts = 0x00A0;
    int qPosCnt = 0x000000004;
    QString jogCounter = "";
    QString devFilePath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(qPosCnt), Return(4)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    EXPECT_EQ(true, ret);
    result = jogControl->getJogCounter(&jogCounter);
    qDebug() << "result:" << result;
    EXPECT_EQ(jogCounter, "4");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest011</testname>
    <category1>CoreLibJogControl</category1>
    <category2>getJogCounter()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control"を生成しないでおく
        2)qPosCnt情報にJOGカウンタ値(0x000010000=65536)を設定しておく
        3)回転方向をCWに設定しておく(0x0020)
        4)openDevice()を実行する
        5)getJogCounter()を実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"65535"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest011)
{
    int result = 0;
    ushort eQpSts = 0x0020;
    int qPosCnt = 0x000010000;
    QString jogCounter = "";
    QString devFilePath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(qPosCnt), Return(4)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    EXPECT_EQ(true, ret);
    result = jogControl->getJogCounter(&jogCounter);
    qDebug() << "result:" << result;
    EXPECT_EQ(jogCounter, "65535");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest012</testname>
    <category1>CoreLibJogControl</category1>
    <category2>getJogCounter()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control"を生成しないでおく
        2)qPosCnt情報にJOGカウンタ値(0x000010000=65536)を設定しておく
        3)回転方向をCCWに設定しておく(0x0000)
        4)openDevice()を実行する
        5)getJogCounter()を実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"0"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest012)
{
    int result = 0;
    ushort eQpSts = 0x0000;
    int qPosCnt = 0x000010000;
    QString jogCounter = "";
    QString devFilePath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(qPosCnt), Return(4)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    EXPECT_EQ(true, ret);
    result = jogControl->getJogCounter(&jogCounter);
    qDebug() << "result:" << result;
    qDebug() << "jogCounter:" << jogCounter;
    EXPECT_EQ(jogCounter, "0");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest013</testname>
    <category1>CoreLibJogControl</category1>
    <category2>getJogCounter()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control"を生成しないでおく
        2)qPosCnt情報にJOGカウンタ値(0x000010000=65536)を設定しておく
        3)qPosCnt2情報にJOGカウンタ値(0x000010004=65540)を設定しておく
        4)回転方向をCWに設定しておく(0x0020)
        5)openDevice()を実行する
        6)getJogCounter()を2回実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"65535"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest013)
{
    int result = 0;
    ushort eQpSts = 0x0020;
    int qPosCnt = 0x000010000;
    int qPosCnt2 = 0x000010004;
    QString jogCounter = "";
    QString devFilePath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(4))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read4(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(qPosCnt), Return(4)))
                    .WillOnce(DoAll(SetArgPointee<1>(qPosCnt2), Return(4)));

    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    EXPECT_EQ(true, ret);
    result = jogControl->getJogCounter(&jogCounter);
    qDebug() << "jogCounter:" << jogCounter;
    result = jogControl->getJogCounter(&jogCounter);
    qDebug() << "result:" << result;
    qDebug() << "jogCounter:" << jogCounter;
    EXPECT_EQ(jogCounter, "65535");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest014</testname>
    <category1>CoreLibJogControl</category1>
    <category2>getJogCounter()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control"を生成しないでおく
        2)qPosCnt情報にJOGカウンタ値(0x0000FFFF=65535)を設定しておく
        3)qPosCnt2情報にJOGカウンタ値(0x000010000=65536)を設定しておく
        4)回転方向をCWに設定しておく(0x0000)
        5)openDevice()を実行する
        6)getJogCounter()を2回実行する
    </case>
    <check>
        1)JOGカウンタ文字列が"0"であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest014)
{
    int result = 0;
    ushort eQpSts = 0x0000;
    int qPosCnt = 0x0000FFFF;
    int qPosCnt2 = 0x00010000;
    QString jogCounter = "";
    QString devFilePath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(4))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read4(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(qPosCnt), Return(4)))
                    .WillOnce(DoAll(SetArgPointee<1>(qPosCnt2), Return(4)));

    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read2(_,_,_))
                    .Times(AtLeast(2))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)))
                    .WillOnce(DoAll(SetArgPointee<1>(eQpSts), Return(2)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    EXPECT_EQ(true, ret);
    result = jogControl->getJogCounter(&jogCounter);
    qDebug() << "jogCounter:" << jogCounter;
    result = jogControl->getJogCounter(&jogCounter);
    qDebug() << "result:" << result;
    qDebug() << "jogCounter:" << jogCounter;
    EXPECT_EQ(jogCounter, "0");
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest015</testname>
    <category1>CoreLibJogControl</category1>
    <category2>getJogCounter()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control"を生成しないでおく
        2)qPosCnt情報にJOGカウンタ値(0x0000FFFF=65535)を設定しておく
        3)read4の戻り値を-1に設定しておく
        4)openDevice()を実行する
        5)getJogCounter()を実行する
    </case>
    <check>
        1)getJogCounterの戻り値が-1であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest015)
{
    int result = 0;
    int qPosCnt = 0x0000FFFF;
    QString jogCounter = "";
    QString devFilePath = "/tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control";

    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_read4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<1>(qPosCnt), Return(-1)));
    EXPECT_CALL(testSub->getJogControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    EXPECT_EQ(true, ret);
    result = jogControl->getJogCounter(&jogCounter);
    EXPECT_LT(result, 0);
    jogControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
}
#else
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest016</testname>
    <category1>CoreLibJogControl</category1>
    <category2>openDevice()</category2>
    <category3>JOGドライバイベント通知用デバイスファイルオープン</category3>
    <case>
        1)"/dev/input/event1"を生成しておく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest016)
{
    int result = 0;
    result = system("mkdir -p /tmp/input");
    qDebug() << "result:" << result;
    result = system("touch /tmp/input/event1");
    qDebug() << "result:" << result;
    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogInputEvent);
    result = system("rm /tmp/input/event1");
    EXPECT_EQ(ret, true);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest017</testname>
    <category1>CoreLibJogControl</category1>
    <category2>openDevice()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/dev/input/event1"を生成しないでおく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest017)
{
    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>JogControlTest018</testname>
    <category1>CoreLibJogControl</category1>
    <category2>openDevice()</category2>
    <category3>JOGドライバカウンタ値取得用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control"を生成しておく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibJogControlTestBase, JogControlTest018)
{
    int result = 0;
    result = system("mkdir -p /tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/");
    qDebug() << "result:" << result;
    result = system("touch /tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control");
    qDebug() << "result:" << result;
    CoreLibJogControl* jogControl = testSub->getJogControlInstance();
    // open device
    bool ret = jogControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeJogQPosCntControl);
    result = system("rm /tmp/devices/platform/ocp/48302000.epwmss/48302180.eqep/qposcnt_control");
    EXPECT_EQ(ret, true);
}
#endif // CORELIB_UNIT_TEST
