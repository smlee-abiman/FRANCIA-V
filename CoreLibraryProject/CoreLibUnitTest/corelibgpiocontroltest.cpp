#include "corelibgpiocontroltest.h"
#include "corelibunittestmockmethod.h"
#include <QDebug>

CoreLibGpioControlTestSub::CoreLibGpioControlTestSub(QObject *parent)
    : QThread(parent)
{
    mGpioControl = new CoreLibGpioControl();
}

CoreLibGpioControlTestSub::~CoreLibGpioControlTestSub()
{
    if (mGpioControl){
        delete mGpioControl;
        mGpioControl = nullptr;
    }
}

#ifdef CORELIB_UNIT_TEST
int CoreLibGpioControlTestSub::createLedDevFile(CoreLibDriverManager::LedType ledType)
{
    int ret = -1;
    QString createledDevFilePath = "";
    QString createledDevFilePathCmdString = "";
    QString createCommandString = "";
    QString ledDevFilePathString = "";
    QString chmodCommandString = "";
    const char *createCommand = NULL;
    const char *createPathCommand = NULL;
    const char *chmodCommand = NULL;

    switch(ledType){
    case CoreLibDriverManager::LedType::LedTypeBlue:
        createledDevFilePath = "/tmp/class/leds/blue";
        createledDevFilePathCmdString = "mkdir -p " + createledDevFilePath;
        ledDevFilePathString = "/tmp/class/leds/blue/brightness";
        createCommandString = "touch " + ledDevFilePathString;
        chmodCommandString = "chmod 777 " + ledDevFilePathString;
        break;
    case CoreLibDriverManager::LedType::LedTypeGreen:
        createledDevFilePath = "/tmp/class/leds/green";
        createledDevFilePathCmdString = "mkdir -p " + createledDevFilePath;
        ledDevFilePathString = "/tmp/class/leds/green/brightness";
        createCommandString = "touch " + ledDevFilePathString;
        chmodCommandString = "chmod 777 " + ledDevFilePathString;
        break;
    case CoreLibDriverManager::LedType::LedTypeRed:
        createledDevFilePath = "/tmp/class/leds/red";
        createledDevFilePathCmdString = "mkdir -p " + createledDevFilePath;
        ledDevFilePathString = "/tmp/class/leds/red/brightness";
        createCommandString = "touch " + ledDevFilePathString;
        chmodCommandString = "chmod 777 " + ledDevFilePathString;
        break;
    default:
        return ret;
    }
    // create folder
    QByteArray str_path_arr = createledDevFilePathCmdString.toLocal8Bit();
    createPathCommand = str_path_arr.constData();
    ret = system(createPathCommand);
    if (ret < 0){
        qDebug("mkdir failed. %s", createPathCommand);
    }
    QByteArray str_arr = createCommandString.toLocal8Bit();
    createCommand = str_arr.constData();
    ret = system(createCommand);
    if (ret < 0){
        qDebug("create failed. %s", createCommand);
    }
    QByteArray str_chmod_arr = chmodCommandString.toLocal8Bit();
    chmodCommand = str_chmod_arr.constData();
    ret = system(chmodCommand);
    if (ret < 0){
        qDebug("chmod failed. %s", chmodCommand);
    }
    return ret;
}
bool CoreLibGpioControlTestSub::confirmLedDevFile(bool isLed, CoreLibDriverManager::LedType ledType)
{
    bool ret = false;
    QString confirmledDevFilePath = "";
    QString confirmLedNum = (isLed == true ? "255" : "0");

    switch(ledType){
    case CoreLibDriverManager::LedType::LedTypeBlue:
        confirmledDevFilePath = "/tmp/class/leds/blue/brightness";
        break;
    case CoreLibDriverManager::LedType::LedTypeGreen:
        confirmledDevFilePath = "/tmp/class/leds/green/brightness";
        break;
    case CoreLibDriverManager::LedType::LedTypeRed:
        confirmledDevFilePath = "/tmp/class/leds/red/brightness";
        break;
    default:
        return ret;
    }
    QFile file(confirmledDevFilePath);
    if (!file.open(QIODevice::ReadWrite)){
        qDebug("file open failed.");
        return false;
    }
    QTextStream ts(&file);
    QString line;
    do {
        line = ts.readLine();
        if (line.contains(confirmLedNum, Qt::CaseSensitive)) {
            ret = true;
            break;
        }
    } while(!line.isNull());
    file.close();
    return ret;
}
#endif // CORELIB_UNIT_TEST

#ifdef CORELIB_UNIT_TEST
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest001</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>ブザー制御用デバイスファイルオープン</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/buzzer_control"を生成しないでおく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest001)
{
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/buzzer_control";
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
    EXPECT_EQ(true, ret);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest002</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>controlBuzzer()</category2>
    <category3>ブザー制御(ON)</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/buzzer_control"を生成しないでおく
        2)openDevice()を実行する
        3)controlBuzzer(isBuzzer=true)を実行する
    </case>
    <check>
        1)2)の戻り値がtrueであること
        2)3)の戻り値が4であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest002)
{
    int result = 0;
    stBuzzerOutput buzzerOutput;
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/buzzer_control";

    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_write4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
    EXPECT_EQ(true, ret);
    // buzzer on
    result = gpioControl->controlBuzzer(true, buzzerOutput);
    EXPECT_EQ(result, 4);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest003</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>controlBuzzer()</category2>
    <category3>ブザー制御(OFF)</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/buzzer_control"を生成しないでおく
        2)openDevice()を実行する
        3)controlBuzzer(isBuzzer=false)を実行する
    </case>
    <check>
        1)2)の戻り値がtrueであること
        2)3)の戻り値が4であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest003)
{
    int result = 0;
    stBuzzerOutput buzzerOutput;
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/buzzer_control";

    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_write4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
    EXPECT_EQ(true, ret);
    // buzzer off
    result = gpioControl->controlBuzzer(false, buzzerOutput);
    EXPECT_EQ(result, 4);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>GpioControlTest004</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>controlBuzzer()</category2>
    <category3>ブザー制御(OFF)</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/buzzer_control"を生成しないでおく
        2)openDevice()を実行する
        3)lseekが-1を返却するように設定しておく
        4)controlBuzzer(isBuzzer=false)を実行する
    </case>
    <check>
        1)2)の戻り値がtrueであること
        2)4)の戻り値が-1であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest004)
{
    int result = 0;
    stBuzzerOutput buzzerOutput;
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/buzzer_control";

    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(-1));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
    EXPECT_EQ(true, ret);
    // buzzer off
    result = gpioControl->controlBuzzer(false, buzzerOutput);
    EXPECT_EQ(result, -1);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>GpioControlTest005</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>controlBuzzer()</category2>
    <category3>ブザー制御(OFF)</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/buzzer_control"を生成しないでおく
        2)openDevice()を実行する
        3)writeが-1を返却するように設定しておく
        4)controlBuzzer(isBuzzer=false)を実行する
    </case>
    <check>
        1)2)の戻り値がtrueであること
        2)4)の戻り値が-1であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest005)
{
    int result = 0;
    stBuzzerOutput buzzerOutput;
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/buzzer_control";

    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_write4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(-1));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
    EXPECT_EQ(true, ret);
    // buzzer off
    result = gpioControl->controlBuzzer(false, buzzerOutput);
    EXPECT_EQ(result, -1);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioBuzzer);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest006</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>入力バッファ制御用デバイスファイルオープン</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/data_buffer"を生成しないでおく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest006)
{
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/data_buffer";
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    EXPECT_EQ(true, ret);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest007</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>入力バッファ制御用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/data_buffer"を生成しないでおく
        2)openDevice()を実行する
        3)inputDataBuf[128]に対して最新6件分、最新から0x00000001,0x00010002,0x00020003,0x00030004,0x00040005.0x00050006を返すように設定しておく
        4)getInputDataBuffer(holdCount=6)を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest007)
{
    int result = 0;
    int inputDataBuf[128] = { 0 };
    uchar holdCount = 6;
    QBitArray bitArrayDataBuf[6];
    QBitArray compareBitArrayDataBuf[6];
    int inputBuf[6] = {0x00000001, 0x00010002, 0x00020003, 0x00030004, 0x00040005, 0x00050006};
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/data_buffer";

    for (int i = 0; i < (int)holdCount; i++){
        bitArrayDataBuf[i].resize(32);
        compareBitArrayDataBuf[i].resize(32);
    }
    for (int i = 1; i <= (int)holdCount; i++){
        inputDataBuf[128 - i] = inputBuf[i -1];
    }
    for(int i = 1; i <= (int)holdCount; i++){
        for(int j = 0; j < 32; j++){
            compareBitArrayDataBuf[i - 1].setBit(31 - j,
                (((inputDataBuf[128 - i] >> (31 - j)) & ~0xFFFFFFFE) == 0x000000001 ? 1 : 0));
        }
    }
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_read4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArrayArgument<1>(inputDataBuf, inputDataBuf + 128), Return(512)));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));

    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    EXPECT_EQ(true, ret);
    // get input buffer
    result = gpioControl->getInputDataBuffer(bitArrayDataBuf, holdCount);
    EXPECT_EQ(result, 512);
    for(int i = 0; i < (int)holdCount; i++) {
        for (int j = 0; j < 32; j++){
            EXPECT_EQ(compareBitArrayDataBuf[i].at(j), bitArrayDataBuf[i].at(j));
        }
    }
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>GpioControlTest008</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>入力バッファ制御用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/data_buffer"を生成しないでおく
        2)lseekが-1を返却するように設定しておく
        3)openDevice()を実行する
        4)getInputDataBuffer()を実行する
    </case>
    <check>
        1)3)の戻り値がtrueであること
        2)4)の戻り値が-1でること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest008)
{
    int result = 0;
    uchar holdCount = 6;
    QBitArray bitArrayDataBuf[6];
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/data_buffer";
    for (int i = 0; i < (int)holdCount; i++){
        bitArrayDataBuf[i].resize(32);
    }
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(-1));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    EXPECT_EQ(true, ret);
    // get input buffer
    result = gpioControl->getInputDataBuffer(bitArrayDataBuf, holdCount);
    EXPECT_EQ(result, -1);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>GpioControlTest009</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>入力バッファ制御用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/data_buffer"を生成しないでおく
        2)readが-1を返却するように設定しておく
        3)openDevice()を実行する
        4)getInputDataBuffer()を実行する
    </case>
    <check>
        1)3)の戻り値がtrueであること
        2)4)の戻り値が-1でること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest009)
{
    int result = 0;
    uchar holdCount = 6;
    QBitArray bitArrayDataBuf[6];
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/data_buffer";
    for (int i = 0; i < (int)holdCount; i++){
        bitArrayDataBuf[i].resize(32);
    }
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_lseek(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(4));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_read4(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(-1));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));

    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    EXPECT_EQ(true, ret);
    // get input buffer
    result = gpioControl->getInputDataBuffer(bitArrayDataBuf, holdCount);
    EXPECT_EQ(result, -1);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>GpioControlTest010</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>入力バッファ制御用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/data_buffer"を生成しないでおく
        2)openDevice()を実行する
        3)getInputDataBuffer(pInputDataBuf=nulptr)を実行する
    </case>
    <check>
        1)2)の戻り値がtrueであること
        2)3)の戻り値が-1でること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest010)
{
    int result = 0;
    uchar holdCount = 6;
    QBitArray *pBitArrayDataBuf = nullptr;
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/data_buffer";

    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));

    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    EXPECT_EQ(true, ret);
    // get input buffer
    result = gpioControl->getInputDataBuffer(pBitArrayDataBuf, holdCount);
    EXPECT_EQ(result, -1);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>GpioControlTest011</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>入力バッファ制御用デバイスファイル読み出し</category3>
    <case>
        1)"/sys/devices/platform/itz3000-misc/data_buffer"を生成しないでおく
        2)openDevice()を実行する
        3)getInputDataBuffer(holdCount=129)を実行する
    </case>
    <check>
        1)2)の戻り値がtrueであること
        2)3)の戻り値が-1でること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest011)
{
    int result = 0;
    uchar holdCount = 129;
    QBitArray bitArrayDataBuf[129];
    QString devFilePath = "/tmp/devices/platform/itz3000-misc/data_buffer";
    for (int i = 0; i < (int)holdCount; i++){
        bitArrayDataBuf[i].resize(32);
    }
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_open(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));

    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
    EXPECT_EQ(true, ret);
    // get input buffer
    result = gpioControl->getInputDataBuffer(bitArrayDataBuf, holdCount);
    EXPECT_EQ(result, -1);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioInputDataBuf);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest012</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>青色LED制御用デバイスファイルオープン</category3>
    <case>
        1)"/sys/class/leds/blue/brightness"を生成しないでおく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest012)
{
    int result = -1;
    QString devFilePath = "/tmp/class/leds/blue/brightness";

    // led dev create(blue)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeBlue);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
//    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_qfile_exists())
//                        .Times(AtLeast(1))
//                        .WillOnce(Return(true));
//    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_qfile_open(_))
//                        .Times(AtLeast(1))
//                        .WillOnce(Return(true));
//    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_qfile_close())
//                        .Times(AtLeast(1));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    EXPECT_EQ(true, ret);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    // led dev delete(blue)
    result = system("rm -rf /tmp/class/leds/blue/brightness");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest013</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>緑色LED制御用デバイスファイルオープン</category3>
    <case>
        1)"/sys/class/leds/green/brightness"を生成しないでおく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest013)
{
    int result = -1;
    QString devFilePath = "/tmp/class/leds/green/brightness";

    // led dev create(green)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeGreen);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
    EXPECT_EQ(true, ret);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
    // led dev delete(gree)
    result = system("rm -rf /tmp/class/leds/green/brightness");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest014</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>赤色LED制御用デバイスファイルオープン</category3>
    <case>
        1)"/sys/class/leds/red/brightness"を生成しないでおく
        2)openDevice()を実行する
    </case>
    <check>
        1)戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest014)
{
    int result = -1;
    QString devFilePath = "/tmp/class/leds/red/brightness";

    // led dev create(red)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeRed);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    bool ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    EXPECT_EQ(true, ret);
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    result = system("rm -rf /tmp/class/leds/red/brightness");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest015</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>青色LED制御(ON)</category3>
    <case>
        1)"/tmp/class/leds/blue/brightness"を生成しておく
        2)getdevfilenameで"/tmp/class/leds/blue/brightness"を指定しておく
        3)openDevice()を実行する
        4)controlLed(isLed=true,ledType=LedTypeBlue)を実行する
    </case>
    <check>
        1)3)の戻り値がtrueであること
        2)4)の戻り値がtrueであること
        3)"/tmp/class/leds/blue/brightness"の値が"255"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest015)
{
    int result = -1;
    bool ret = false;
    QString devFilePath = "/tmp/class/leds/blue/brightness";

    // led dev create(blue)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeBlue);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    EXPECT_EQ(true, ret);
    // blue led on
    ret = gpioControl->controlLed(true, CoreLibDriverManager::LedType::LedTypeBlue);
    EXPECT_EQ(true, ret);
    // blue led dev file confirm
    ret = testSub->confirmLedDevFile(true, CoreLibDriverManager::LedType::LedTypeBlue);
    EXPECT_EQ(true, ret);
    // close device
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    // led dev delete(blue)
    result = system("rm -rf /tmp/class/leds/blue/brightness");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest016</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>青色LED制御(OFF)</category3>
    <case>
        1)"/tmp/class/leds/blue/brightness"を生成しておく
        2)getdevfilenameで"/tmp/class/leds/blue/brightness"を指定しておく
        3)openDevice()を実行する
        4)controlLed(isLed=false,ledType=LedTypeBlue)を実行する
    </case>
    <check>
        1)3)の戻り値がtrueであること
        2)4)の戻り値がtrueであること
        3)"/tmp/class/leds/blue/brightness"の値が"0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest016)
{
    int result = -1;
    bool ret = false;
    QString devFilePath = "/tmp/class/leds/blue/brightness";

    // led dev create(blue)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeBlue);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    EXPECT_EQ(true, ret);
    // blue led off
    ret = gpioControl->controlLed(false, CoreLibDriverManager::LedType::LedTypeBlue);
    EXPECT_EQ(true, ret);
    // blue led dev file confirm
    ret = testSub->confirmLedDevFile(false, CoreLibDriverManager::LedType::LedTypeBlue);
    EXPECT_EQ(true, ret);
    // close device
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedBlue);
    // led dev delete(blue)
    result = system("rm -rf /tmp/class/leds/blue/brightness");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest017</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>緑色LED制御(ON)</category3>
    <case>
        1)"/tmp/class/leds/green/brightness"を生成しておく
        2)getdevfilenameで"/tmp/class/leds/green/brightness"を指定しておく
        3)openDevice()を実行する
        4)controlLed(isLed=true,ledType=LedTypeGreen)を実行する
    </case>
    <check>
        1)3)の戻り値がtrueであること
        2)4)の戻り値がtrueであること
        3)"/tmp/class/leds/green/brightness"の値が"255"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest017)
{
    int result = -1;
    bool ret = false;
    QString devFilePath = "/tmp/class/leds/green/brightness";

    // led dev create(green)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeGreen);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
    EXPECT_EQ(true, ret);
    // green led on
    ret = gpioControl->controlLed(true, CoreLibDriverManager::LedType::LedTypeGreen);
    EXPECT_EQ(true, ret);
    // green led dev file confirm
    ret = testSub->confirmLedDevFile(true, CoreLibDriverManager::LedType::LedTypeGreen);
    EXPECT_EQ(true, ret);
    // close device
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
    // led dev delete(green)
    result = system("rm -rf /tmp/class/leds/green/brightness");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest018</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>緑色LED制御(OFF)</category3>
    <case>
        1)"/tmp/class/leds/blue/brightness"を生成しておく
        2)getdevfilenameで"/tmp/class/leds/green/brightness"を指定しておく
        3)openDevice()を実行する
        4)controlLed(isLed=false,ledType=LedTypeGreen)を実行する
    </case>
    <check>
        1)3)の戻り値がtrueであること
        2)4)の戻り値がtrueであること
        3)"/tmp/class/leds/green/brightness"の値が"0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest018)
{
    int result = -1;
    bool ret = false;
    QString devFilePath = "/tmp/class/leds/green/brightness";

    // led dev create(green)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeGreen);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
    EXPECT_EQ(true, ret);
    // green led off
    ret = gpioControl->controlLed(false, CoreLibDriverManager::LedType::LedTypeGreen);
    EXPECT_EQ(true, ret);
    // green led dev file confirm
    ret = testSub->confirmLedDevFile(false, CoreLibDriverManager::LedType::LedTypeGreen);
    EXPECT_EQ(true, ret);
    // close device
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedGreen);
    // led dev delete(green)
    result = system("rm -rf /tmp/class/leds/green/brightness");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest019</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>赤色LED制御(ON)</category3>
    <case>
        1)"/tmp/class/leds/red/brightness"を生成しておく
        2)getdevfilenameで"/tmp/class/leds/red/brightness"を指定しておく
        3)openDevice()を実行する
        4)controlLed(isLed=true,ledType=LedTypeRed)を実行する
    </case>
    <check>
        1)3)の戻り値がtrueであること
        2)4)の戻り値がtrueであること
        3)"/tmp/class/leds/red/brightness"の値が"255"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest019)
{
    int result = -1;
    bool ret = false;
    QString devFilePath = "/tmp/class/leds/red/brightness";

    // led dev create(red)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeRed);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    EXPECT_EQ(true, ret);
    // red led on
    ret = gpioControl->controlLed(true, CoreLibDriverManager::LedType::LedTypeRed);
    EXPECT_EQ(true, ret);
    // red led dev file confirm
    ret = testSub->confirmLedDevFile(true, CoreLibDriverManager::LedType::LedTypeRed);
    EXPECT_EQ(true, ret);
    // close device
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    // led dev delete(red)
    result = system("rm -rf /tmp/class/leds/red/brightness");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>GpioControlTest020</testname>
    <category1>CoreLibGpioControl</category1>
    <category2>openDevice()</category2>
    <category3>赤色LED制御(OFF)</category3>
    <case>
        1)"/tmp/class/leds/red/brightness"を生成しておく
        2)getdevfilenameで"/tmp/class/leds/red/brightness"を指定しておく
        3)openDevice()を実行する
        4)controlLed(isLed=false,ledType=LedTypeRed)を実行する
    </case>
    <check>
        1)3)の戻り値がtrueであること
        2)4)の戻り値がtrueであること
        3)"/tmp/class/leds/red/brightness"の値が"0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibGpioControlTestBase, GpioControlTest020)
{
    int result = -1;
    bool ret = false;
    QString devFilePath = "/tmp/class/leds/red/brightness";

    // led dev create(red)
    result = testSub->createLedDevFile(CoreLibDriverManager::LedType::LedTypeRed);
    if (result < 0){
        qDebug("result[%d]", result);
    }
    EXPECT_CALL(testSub->getGpioControlInstance()->getUnitTestMockMethod(), mock_get_dev_file(_))
                        .Times(AtLeast(1))
                        .WillOnce(Return(devFilePath));
    CoreLibGpioControl* gpioControl = testSub->getGpioControlInstance();
    // open device
    ret = gpioControl->openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    EXPECT_EQ(true, ret);
    // red led off
    ret = gpioControl->controlLed(false, CoreLibDriverManager::LedType::LedTypeRed);
    EXPECT_EQ(true, ret);
    // red led dev file confirm
    ret = testSub->confirmLedDevFile(false, CoreLibDriverManager::LedType::LedTypeRed);
    EXPECT_EQ(true, ret);
    // close device
    gpioControl->closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeGpioLedRed);
    // led dev delete(red)
    result = system("rm -rf /tmp/class/leds/red/brightness");
}
#else // CORELIB_UNIT_TEST
// No Test Case
#endif // CORELIB_UNIT_TEST
