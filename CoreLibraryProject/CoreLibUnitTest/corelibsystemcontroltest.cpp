#include "corelibsystemcontroltest.h"
#include "corelibunittestmockmethod.h"
#include <QDebug>

CoreLibSystemControlTestSub::CoreLibSystemControlTestSub(QObject *parent)
    : QThread(parent)
{
    mSystemControl = new CoreLibSystemControl();
}

CoreLibSystemControlTestSub::~CoreLibSystemControlTestSub()
{
    if (mSystemControl){
        delete mSystemControl;
        mSystemControl = nullptr;
    }
}

int CoreLibSystemControlTestSub::createEthServiceFile()
{
    char curPath[PATHNAME_MAX_SIZE] = { 0 };
    QString copyCommandString = "";
    const char *copyCommand = NULL;
    int ret = 0;
    char *retGetCwd = NULL;

    QTextCodec* tc = QTextCodec::codecForLocale();
    // get current dir
    retGetCwd = getcwd(curPath, PATHNAME_MAX_SIZE);
    if (!retGetCwd){
        qDebug() << "getcwd failed errno:" << errno;
        return -1;
    }

    // get itz3000-eth0.service file path
    QString qCurPath = QString(tc->toUnicode(curPath));
    //qDebug() << "qCurPath:" << qCurPath;
    QString ethServiceFilePath = qCurPath + "/../../ITZ3000Linux/meta-local/recipes-tietech/itz3000/files/";
    //qDebug() << "ethServiceFilePath:" << ethServiceFilePath;
    copyCommandString = "cp " + ethServiceFilePath + "itz3000-eth0.service" + " " + "/lib/systemd/system/.";
    QByteArray str_arr = copyCommandString.toLocal8Bit();
    copyCommand = str_arr.constData();
    //qDebug("copyCommand[%s]", copyCommand);
    ret = system(copyCommand);
    if (ret < 0){
        qDebug("cp failed. %s", copyCommand);
    }
    return ret;
}
int CoreLibSystemControlTestSub::deleteEthServiceFile()
{
    int ret = 0;
    const char *deleteCommand = NULL;
    QString deleteCommandString = "rm -rf /lib/systemd/system/itz3000-eth0.service";
    QByteArray str_arr = deleteCommandString.toLocal8Bit();

    deleteCommand = str_arr.constData();
    //qDebug("deleteCommand[%s]", deleteCommand);
    ret = system(deleteCommand);
    if (ret < 0){
        qDebug("rm failed. %s", deleteCommand);
    }
    return ret;
}

bool CoreLibSystemControlTestSub::getMACAddress(QString *macAddress, QString ifrNameStr)
{
    (void)ifrNameStr;
    bool ret = true;
    uchar firstSearchCnt = 0;
    uchar secondSearchCnt = 0;
    //int macAddrTopPos = 0;
    int etherPos = 0;

    QFile file("/lib/systemd/system/itz3000-eth0.service");
    if (!file.open(QIODevice::ReadWrite)){
        qDebug() << "open failed: /lib/systemd/system/itz3000-eth0.service";
        return false;
    }
    QTextStream firstSearchTs(&file);
    QString line;
    QString execStartPreStr("ExecStartPre=");
    QString etherStr("ether ");
    QRegExp re("(([0-9]|[a-f]|[A-F]){2}):{5}(([0-9]|[a-f]|[A-F]){2})?");
    do {
        line = firstSearchTs.readLine();
        if (line.contains(execStartPreStr, Qt::CaseSensitive)) {;
            firstSearchCnt++;
            continue;
        }
    } while(!line.isNull());
    file.close();
    if (!file.open(QIODevice::ReadWrite)){
        qDebug() << "open failed: /lib/systemd/system/itz3000-eth0.service";
        return false;
    }
    QTextStream secondSearchTs(&file);
    qDebug("firstSearchCnt[%d]", firstSearchCnt);
    if (firstSearchCnt >= 2){
        do {
            line = secondSearchTs.readLine();
            if (secondSearchCnt == 0){
                if (line.contains(execStartPreStr, Qt::CaseSensitive)) {
                    secondSearchCnt++;
                    continue;
                }
            } else {
                if (line.contains(execStartPreStr, Qt::CaseSensitive)) {
                    secondSearchCnt++;
                    break;
                }
            }
        } while(!line.isNull());
    } else {
        do {
            line = secondSearchTs.readLine();
            if (line.contains(execStartPreStr, Qt::CaseSensitive)) {
                break;
            }
        } while(!line.isNull());
    }
    qDebug() << "line:" << line << "secondSearchCnt:" << secondSearchCnt;
    //macAddrTopPos = re.indexIn(line);
    etherPos = line.indexOf(etherStr);
    //qDebug() << "etherPos:" << etherPos << "macAddrTopPos:" << macAddrTopPos;
    qDebug() << "etherPos:" << etherPos;
    *macAddress = line.mid(etherPos + 6, 18);
    qDebug() << "macAddress : " << *macAddress;
    file.close();
    return ret;
}

#ifdef CORELIB_UNIT_TEST
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest001</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>getMACAddress()</category2>
    <category3>MacAddress-get</category3>
    <case>
        1)socket()が2を返却するように設定しておく
        2)MACAddress[01:23:45:67:89:AB]を返却するように設定しておく
        3)getMACAddress()を実行する
    </case>
    <check>
        1)getMACAddress()の戻り値がtrueになっていること
        2)getMacAddressが"01:23:45:67:89:ab"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest001)
{
    bool result = false;
    struct ifreq ifr;
    char sa_data[6] = {0x01, 0x23, 0x45, 0x67, (char)0x89, (char)0xAB};
    QString getMacAddress = "";

    memcpy(ifr.ifr_hwaddr.sa_data, sa_data, sizeof(char)*6);
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    EXPECT_CALL(testSub->getSystemControlInstance()->getUnitTestMockMethod(), mock_socket(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getSystemControlInstance()->getUnitTestMockMethod(), mock_ioctl1(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(DoAll(SetArgPointee<2>(ifr), Return(0)));
    EXPECT_CALL(testSub->getSystemControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));

    // get macaddress setting
    result = systemCtrl->getMACAddress(&getMacAddress);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getMacAddress, "01:23:45:67:89:ab");
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>SystemCtrlTest002</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>MacAddress-get</category3>
    <case>
        1)socket()がエラーを返却するように設定しておく
        2)getMACAddress()を実行する
    </case>
    <check>
        1)getMACAddress()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest002)
{
    bool result = false;
    QString getMacAddress = "";

    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    EXPECT_CALL(testSub->getSystemControlInstance()->getUnitTestMockMethod(), mock_socket(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(-1));

    // get macaddress setting
    result = systemCtrl->getMACAddress(&getMacAddress);
    EXPECT_EQ(result, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>SystemCtrlTest003</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>MacAddress-get</category3>
    <case>
        1)socket()が2を返却するように設定しておく
        2)iotcl()がエラーを返却するように設定しておく
        3)getMACAddress()を実行する
    </case>
    <check>
        1)getMACAddress()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest003)
{
    bool result = false;
    QString getMacAddress = "";

    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    EXPECT_CALL(testSub->getSystemControlInstance()->getUnitTestMockMethod(), mock_socket(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(2));
    EXPECT_CALL(testSub->getSystemControlInstance()->getUnitTestMockMethod(), mock_ioctl1(_,_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(-1));
    EXPECT_CALL(testSub->getSystemControlInstance()->getUnitTestMockMethod(), mock_close(_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(1));
    // get macaddress setting
    result = systemCtrl->getMACAddress(&getMacAddress);
    EXPECT_EQ(result, false);
}
#else // CORELIB_UNIT_TEST
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest004</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.1.5", SubNetMask="255.255.255.255")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.1.5"になっていること
        4)SubNetMask"255.255.255.255"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest004)
{
    int ret = 0;
    QString setIpAddress = "192.168.1.5";
    QString setSubNetMask = "255.255.255.255";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.1.5");
    EXPECT_EQ(getSubNetMask, "255.255.255.255");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest005</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.1.10", SubNetMask="255.255.255.0")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.1.10"になっていること
        4)SubNetMask"255.255.255.0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest005)
{
    int ret = 0;
    QString setIpAddress = "192.168.1.10";
    QString setSubNetMask = "255.255.255.0";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.1.10");
    EXPECT_EQ(getSubNetMask, "255.255.255.0");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest006</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.1.100", SubNetMask="255.255.254.0")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.1.100"になっていること
        4)SubNetMask"255.255.254.0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest006)
{
    int ret = 0;
    QString setIpAddress = "192.168.1.100";
    QString setSubNetMask = "255.255.254.0";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.1.100");
    EXPECT_EQ(getSubNetMask, "255.255.254.0");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest007</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.10.3", SubNetMask="255.255.128.0")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.10.3"になっていること
        4)SubNetMask"255.255.128.0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest007)
{
    int ret = 0;
    QString setIpAddress = "192.168.10.3";
    QString setSubNetMask = "255.255.128.0";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.10.3");
    EXPECT_EQ(getSubNetMask, "255.255.128.0");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest008</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.111.3", SubNetMask="255.252.0.0")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.111.3"になっていること
        4)SubNetMask"255.252.0.0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest008)
{
    int ret = 0;
    QString setIpAddress = "192.168.111.3";
    QString setSubNetMask = "255.252.0.0";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.111.3");
    EXPECT_EQ(getSubNetMask, "255.252.0.0");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest009</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.15.24", SubNetMask="255.192.0.0")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.15.24"になっていること
        4)SubNetMask"255.192.0.0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest009)
{
    int ret = 0;
    QString setIpAddress = "192.168.15.24";
    QString setSubNetMask = "255.192.0.0";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.15.24");
    EXPECT_EQ(getSubNetMask, "255.192.0.0");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest010</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.15.32", SubNetMask="248.0.0.0")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.15.32"になっていること
        4)SubNetMask"248.0.0.0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest010)
{
    int ret = 0;
    QString setIpAddress = "192.168.15.32";
    QString setSubNetMask = "248.0.0.0";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.15.32");
    EXPECT_EQ(getSubNetMask, "248.0.0.0");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest011</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.128.10", SubNetMask="224.0.0.0")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.128.10"になっていること
        4)SubNetMask"224.0.0.0"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest011)
{
    int ret = 0;
    QString setIpAddress = "192.168.128.10";
    QString setSubNetMask = "224.0.0.0";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.128.10");
    EXPECT_EQ(getSubNetMask, "224.0.0.0");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest012</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>network-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setNetworkSetting(IPAddress="192.168.128.101", SubNetMask="255.255.255.240")を実行する
        3)getNetworkSetting()を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setNetworkSetting()の戻り値がtrueになっていること
        2)getNetworkSetting()の戻り値がtrueになっていること
        3)IPAddressが"192.168.128.101"になっていること
        4)SubNetMask"255.255.255.240"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest012)
{
    int ret = 0;
    QString setIpAddress = "192.168.128.101";
    QString setSubNetMask = "255.255.255.240";
    QString getIpAddress = "";
    QString getSubNetMask = "";
    bool result = false;

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setNetworkSetting(setIpAddress, setSubNetMask);
    EXPECT_EQ(result, true);

    // set network setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, true);
    EXPECT_EQ(getIpAddress, "192.168.128.101");
    EXPECT_EQ(getSubNetMask, "255.255.255.240");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest013</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>macaddress-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setMACAddress(MacAddress="AB:CD:EF:01:23:45")を実行する
        3)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)setMACAddress()の戻り値がtrueになっていること
        2)MacAddress"AB:CD:EF:01:23:45"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest013)
{
    int ret = 0;
    bool result = 0;
    QString setMacAddress = "AB:CD:EF:01:23:45";
    QString getMacAddress = "";

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    // set Mac address
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setMACAddress(setMacAddress);
    EXPECT_EQ(result, true);

    // get Mac address
    result = testSub->getMACAddress(&getMacAddress);
    EXPECT_EQ(getMacAddress, "AB:CD:EF:01:23:45");
    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SystemCtrlTest014</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>setNetworkSetting()</category2>
    <category3>macaddress-setting</category3>
    <case>
        1)itz3000-eth0.serviceファイルを"/lib/systemd/system/"に生成する
        2)setMACAddress(MacAddress="01:23:45:89:AB:CD")を実行する
        3)setMACAddress(MacAddress="EF:01:23:45:67:89")を実行する
        4)itz3000-eth0.serviceファイルを"/lib/systemd/system/"配下から削除する
    </case>
    <check>
        1)2)のsetMACAddress()の戻り値がtrueになっていること
        2)2)の結果MacAddress"01:23:45:89:AB:CD"になっていること
        3)3)のsetMACAddress()の戻り値がtrueになっていること
        4)3)の結果MacAddress"EF:01:23:45:67:89"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest014)
{
    int ret = 0;
    bool result = 0;
    QString setMacAddress = "01:23:45:89:AB:CD";
    QString getMacAddress = "";

    // itz3000-eth0.service file ready
    ret = testSub->createEthServiceFile();
    if (ret < 0){
        qDebug("createEthServiceFile failed[%d]", ret);
    }
    // set Mac address
    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // set network setting
    result = systemCtrl->setMACAddress(setMacAddress);
    EXPECT_EQ(result, true);

    // get Mac address
    result = testSub->getMACAddress(&getMacAddress);
    EXPECT_EQ(getMacAddress, "01:23:45:89:AB:CD");

    // set network setting
    setMacAddress = "EF:01:23:45:67:89";
    result = systemCtrl->setMACAddress(setMacAddress);
    EXPECT_EQ(result, true);

    // get Mac address
    result = testSub->getMACAddress(&getMacAddress);
    EXPECT_EQ(getMacAddress, "EF:01:23:45:67:89");

    // itz3000-eth0.service file delete
    ret = testSub->deleteEthServiceFile();
    if (ret < 0){
        qDebug("deleteEthServiceFile failed[%d]", ret);
    }
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>SystemCtrlTest015</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>getMACAddress()</category2>
    <category3>MacAddress-get</category3>
    <case>
        1)getMACAddress(ifrNameStr="eth0-custom")を実行する
    </case>
    <check>
        1)getMACAddress()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest015)
{
    bool result = false;
    QString getMacAddress = "";

    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // get macaddress setting
    result = systemCtrl->getMACAddress(&getMacAddress, "eth0-custom");
    EXPECT_EQ(result, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>SystemCtrlTest016</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>getMACAddress()</category2>
    <category3>MacAddress-get</category3>
    <case>
        1)getMACAddress(&getMacAddress==nullptr)を実行する
    </case>
    <check>
        1)getMACAddress()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest016)
{
    bool result = false;
    QString *getMacAddress = nullptr;

    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // get macaddress setting
    result = systemCtrl->getMACAddress(getMacAddress);
    EXPECT_EQ(result, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>SystemCtrlTest015</testname>
    <category1>CoreLibSystemControl</category1>
    <category2>getMACAddress()</category2>
    <category3>MacAddress-get</category3>
    <case>
        1)getMACAddress()を実行する
    </case>
    <check>
        1)getMACAddress()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSystemControlTestBase, SystemCtrlTest017)
{
    bool result = false;
    QString getIpAddress = "";
    QString getSubNetMask = "";

    CoreLibSystemControl* systemCtrl = testSub->getSystemControlInstance();
    if (!systemCtrl){
        qDebug("systemCtrl is Null[%p]", systemCtrl);
    }
    // get macaddress setting
    result = systemCtrl->getNetworkSetting(&getIpAddress, &getSubNetMask);
    EXPECT_EQ(result, false);
}
#endif // CORELIB_UNIT_TEST
