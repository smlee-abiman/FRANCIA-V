#include "corelibsdcontroltest.h"
#include "corelibunittestmockmethod.h"
#include <QDebug>

CoreLibSdControlTestSub::CoreLibSdControlTestSub(QObject *parent)
    : QThread(parent)
{
    mSdControl = new CoreLibSdControl();
}

CoreLibSdControlTestSub::~CoreLibSdControlTestSub()
{
    if (mSdControl){
        delete mSdControl;
        mSdControl = nullptr;
    }
}

void CoreLibSdControlTestSub::createTestDir()
{
    QDir dir;
    if (!dir.exists(mTestDirPath)){
        if (!dir.mkpath(mTestDirPath)){
            qDebug() << "mkpath failed mTestDirPath:" << mTestDirPath;
            return;
        }
    }
    return;
}
void CoreLibSdControlTestSub::createTestDir(int num)
{
    QString testDirName = "";
    for(int i = 1; i <= num; i++){
        testDirName = "00" + QString::number(i) + QString::number(i) + "Test";
        QDir dir(mTestDirPath + "/" + testDirName);
        mSdControl->createFolder(dir.absolutePath());
        testDirName = "";
    }
}
void CoreLibSdControlTestSub::createTestFile(int num)
{
    QString testFileName = "";
    for(int i = 1; i <= num; i++){
        testFileName = "00" + QString::number(i) + QString::number(i) + "Test" + ".prg";
        QFile file(mTestDirPath + "/" + testFileName);
        if (!file.open(QIODevice::ReadWrite)){
            qDebug() << "open failed filePath:" << mTestDirPath + "/" + testFileName;
            return;
        }
        QTextStream ts(&file);
        ts << testFileName;
        ts << "\n";
        file.close();
        testFileName = "";
    }
}
void CoreLibSdControlTestSub::deleteTestDir()
{
    int ret  = 0;
    QDir dir;
    const char *rmCommand = NULL;
    QString rmCommandString = "";
    if (dir.exists(mTestDirPath)){
        rmCommandString = "rm -rf " + mTestDirPath;
        //qDebug() << "rmCommandString:" << rmCommandString;
        QByteArray str_arr = rmCommandString.toLocal8Bit();
        rmCommand = str_arr.constData();
        //qDebug("rmCommand[%s]", rmCommand);
        ret = system(rmCommand);
        if (ret < 0){
            qDebug("rm failed. %s", rmCommand);
        }
    }
    return;
}

QDateTime CoreLibSdControlTestSub::getCurrentDateTime()
{
    return QDateTime::currentDateTime();
}

#ifdef CORELIB_UNIT_TEST    /* Debug */
#else // CORELIB_UNIT_TEST  /* Release */
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SdCtrlTest001</testname>
    <category1>CoreLibSdControl</category1>
    <category2>getStorageFolderPath()</category2>
    <category3>StorageFolderPath-get</category3>
    <case>
        1)getStorageFolderPath()を実行する
    </case>
    <check>
        1)戻り値が"/home/{user}/itz3000-gui/settings/storage_folder"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSdControlTestBase, SdCtrlTest001)
{
    QString userPath = QDir::homePath();
    QString storagePath = userPath + "/itz3000-gui/settings/storage_folder";
    QString retStoragePath = "";

    CoreLibSdControl* sdCtrl = testSub->getSdControlInstance();
    if (!sdCtrl){
        qDebug("sdCtrl is Null[%p]", sdCtrl);
    }
    //qDebug() << "storagePath:" << storagePath;
    retStoragePath = sdCtrl->getStorageFolderPath();
    //qDebug() << "retStoragePath:" << retStoragePath;

    EXPECT_EQ(retStoragePath.compare(storagePath, Qt::CaseSensitive), 0);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SdCtrlTest002</testname>
    <category1>CoreLibSdControl</category1>
    <category2>getProgramFolderPath()</category2>
    <category3>ProgramFolderPath-get</category3>
    <case>
        1)getProgramFolderPath()を実行する
    </case>
    <check>
        1)戻り値が"/home/{user}/itz3000-gui/settings/prog_folder"になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSdControlTestBase, SdCtrlTest002)
{
    QString userPath = QDir::homePath();
    QString progPath = userPath + "/itz3000-gui/settings/prog_folder";
    QString retProgPath = "";

    CoreLibSdControl* sdCtrl = testSub->getSdControlInstance();
    if (!sdCtrl){
        qDebug("sdCtrl is Null[%p]", sdCtrl);
    }
    //qDebug() << "storagePath:" << storagePath;
    retProgPath = sdCtrl->getProgramFolderPath();
    //qDebug() << "retStoragePath:" << retStoragePath;

    EXPECT_EQ(retProgPath.compare(progPath, Qt::CaseSensitive), 0);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SdCtrlTest003</testname>
    <category1>CoreLibSdControl</category1>
    <category2>getFolderCount()</category2>
    <category3>フォルダ数取得</category3>
    <case>
        1)Testディレクトリ(test_folder)を作成する
        2)1)のディレクトリの中にtestディレクトリを5個作成する
        3)1)のディレクトリの中にファイルを3個作成する
        4)getFolderCount("/home/{user}/itz3000-gui/settings/test_folder")を実行する
    </case>
    <check>
        1)戻り値が5になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSdControlTestBase, SdCtrlTest003)
{
    QString userPath = QDir::homePath();
    QString testFolderPath = userPath + "/itz3000-gui/settings/test_folder";
    int testDirCnt = 5;
    int testFileCnt = 3;
    int folderCnt = 0;

    CoreLibSdControl* sdCtrl = testSub->getSdControlInstance();
    if (!sdCtrl){
        qDebug("sdCtrl is Null[%p]", sdCtrl);
    }
    // Testディレクトリ(test_folder)を作成する
    testSub->createTestDir();
    // Testディレクトリ(0011Test/0022Test/0033Test/0044Test/0055Test)を作成する
    testSub->createTestDir(testDirCnt);
    // Testファイル(0011Test.prg/0022Test.prg/0033Test.prg)を作成する
    testSub->createTestFile(testFileCnt);
    folderCnt = sdCtrl->getFolderCount(testFolderPath);
    EXPECT_EQ(folderCnt, testDirCnt);
    // Testディレクトリ(test_folder)を削除する
    testSub->deleteTestDir();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SdCtrlTest004</testname>
    <category1>CoreLibSdControl</category1>
    <category2>getFolderCount()</category2>
    <category3>フォルダ数取得</category3>
    <case>
        1)Testディレクトリ(test_folder)を作成する
        2)1)のディレクトリの中にtestディレクトリを5個作成する
        3)1)のディレクトリの中にファイルを3個作成する
        4)getFolderList("/home/{user}/itz3000-gui/settings/test_folder")を実行する
    </case>
    <check>
        1)フォルダ数が5になっていること
        2)フォルダリスト名前がtestFolderNamePathテストデータと一致していること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSdControlTestBase, SdCtrlTest004)
{
    QString userPath = QDir::homePath();
    QString testFolderPath = userPath + "/itz3000-gui/settings/test_folder";
    QString testFolderNamePath[5] = {
        "0011Test",
        "0022Test",
        "0033Test",
        "0044Test",
        "0055Test",
    };
    int testDirCnt = 5;
    int testFileCnt = 3;
    QFileInfoList folderList;

    CoreLibSdControl* sdCtrl = testSub->getSdControlInstance();
    if (!sdCtrl){
        qDebug("sdCtrl is Null[%p]", sdCtrl);
    }
    // Testディレクトリ(test_folder)を作成する
    testSub->createTestDir();
    // Testディレクトリ(0011Test/0022Test/0033Test/0044Test/0055Test)を作成する
    testSub->createTestDir(testDirCnt);
    // Testファイル(0011Test.prg/0022Test.prg/0033Test.prg)を作成する
    testSub->createTestFile(testFileCnt);
    folderList = sdCtrl->getFolderList(testFolderPath);
    qDebug("folderList count[%d]", folderList.count());
    for(int i = 0; i < folderList.count(); i++){
        //qDebug() << "folderList name:" << folderList.at(i).fileName();
        EXPECT_EQ(folderList.at(i).fileName(), testFolderNamePath[i]);
    }
    // Testディレクトリ(test_folder)を削除する
    testSub->deleteTestDir();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SdCtrlTest005</testname>
    <category1>CoreLibSdControl</category1>
    <category2>getFolderCount()</category2>
    <category3>フォルダ数取得</category3>
    <case>
        1)現在時刻を取得する
        2)Testディレクトリ(test_folder)を作成する
        3)1)のディレクトリの中にtestディレクトリを5個作成する
        4)1)のディレクトリの中にファイルを3個作成する
        5)getFolderList("/home/{user}/itz3000-gui/settings/test_folder")を実行する
        6)getFolderLastModified()を実行する
    </case>
    <check>
        1)getFolderLastModified()で取得した時刻情報(yyyy/MM/dd hh:mm:ss)が現在時刻+1秒以内に収まっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSdControlTestBase, SdCtrlTest005)
{
    QString userPath = QDir::homePath();
    QString testFolderPath = userPath + "/itz3000-gui/settings/test_folder";
    int testDirCnt = 5;
    int testFileCnt = 3;
    QFileInfoList folderList;
    QDateTime dt;
    QString curDateTime = "";
    QString upperLimitDateTime = "";
    QString modifyDateTime = "";

    CoreLibSdControl* sdCtrl = testSub->getSdControlInstance();
    if (!sdCtrl){
        qDebug("sdCtrl is Null[%p]", sdCtrl);
    }
    // 現在時刻取得
    dt = testSub->getCurrentDateTime();
    curDateTime = dt.toString("yyyy/MM/dd hh:mm:ss");
    upperLimitDateTime = QString::number(dt.date().year()) + "/" +
        (dt.date().month() < 10 ? "0" : "") + QString::number(dt.date().month()) + "/" +
        (dt.date().day() < 10 ? "0" : "") + QString::number(dt.date().day()) + " " +
        (dt.time().hour() < 10 ? "0" : "") + QString::number(dt.time().hour()) + ":" +
        (dt.time().minute() < 10 ? "0" : "") + QString::number(dt.time().minute()) + ":" +
        (dt.time().second() + 1 < 10 ? "0" : "") + QString::number(dt.time().second() + 1);
    // Testディレクトリ(test_folder)を作成する
    testSub->createTestDir();
    // Testディレクトリ(0011Test/0022Test/0033Test/0044Test/0055Test)を作成する
    testSub->createTestDir(testDirCnt);
    // Testファイル(0011Test.prg/0022Test.prg/0033Test.prg)を作成する
    testSub->createTestFile(testFileCnt);
    folderList = sdCtrl->getFolderList(testFolderPath);
    qDebug("folderList count[%d]", folderList.count());
    for(int i = 0; i < folderList.count(); i++){
        QDateTime dateTime = sdCtrl->getFolderLastModified(folderList, i);
        modifyDateTime = QString::number(dateTime.date().year()) + "/" +
            (dateTime.date().month() < 10 ? "0" : "") + QString::number(dateTime.date().month()) + "/" +
            (dateTime.date().day() < 10 ? "0" : "") + QString::number(dateTime.date().day()) + " " +
            (dateTime.time().hour() < 10 ? "0" : "") + QString::number(dateTime.time().hour()) + ":" +
            (dateTime.time().minute() < 10 ? "0" : "") + QString::number(dateTime.time().minute()) + ":" +
            (dateTime.time().second() < 10 ? "0" : "") + QString::number(dateTime.time().second());    
        qDebug() << "folder dateTime:" << dateTime;
        qDebug() << "folder modifyDateTime:" << modifyDateTime;
        ASSERT_TRUE(modifyDateTime.compare(curDateTime, Qt::CaseSensitive) == 0
            || modifyDateTime.compare(upperLimitDateTime, Qt::CaseSensitive) == 0)
            << "curDateTime compare = " << modifyDateTime.compare(curDateTime, Qt::CaseSensitive)
            << "upperLimitDateTime compare = " << modifyDateTime.compare(upperLimitDateTime, Qt::CaseSensitive);
    }
    // Testディレクトリ(test_folder)を削除する
    testSub->deleteTestDir();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SdCtrlTest006</testname>
    <category1>CoreLibSdControl</category1>
    <category2>getFolderCount()</category2>
    <category3>フォルダ数取得</category3>
    <case>
        1)現在時刻を取得する
        2)Testディレクトリ(test_folder)を作成する
        3)1)のディレクトリの中にtestディレクトリを4個作成する
        4)1)のディレクトリの中にファイルを3個作成する
        5)getFolderList("/home/{user}/itz3000-gui/settings/test_folder")を実行する
        6)getFolderPermissions()を実行する
    </case>
    <check>
        1)permission結果がテストデータ用パーミッション設定値とのOR結果が真であること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSdControlTestBase, SdCtrlTest006)
{
    QString userPath = QDir::homePath();
    QString testFolderPath = userPath + "/itz3000-gui/settings/test_folder";
    int testDirCnt = 4;
    int testFileCnt = 3;
    QFileInfoList folderList;
    QFile::Permissions defaultPermission =
        QFileDevice::ExeUser | QFileDevice::WriteUser | QFileDevice::ExeOwner | QFileDevice::WriteOwner;
    QFile::Permissions folderPermission[4] = {
        (defaultPermission | QFileDevice::ReadOwner | QFileDevice::ExeOwner | QFileDevice::ReadUser),
        (defaultPermission | QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner | QFileDevice::ReadUser),
        (defaultPermission | QFileDevice::ReadOwner | QFileDevice::ExeOwner | QFileDevice::ReadUser),
        (defaultPermission | QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner | QFileDevice::ReadUser)
    };
    QString curDateTime = "";
    QString upperLimitDateTime = "";
    QString modifyDateTime = "";
    bool ret = false;

    CoreLibSdControl* sdCtrl = testSub->getSdControlInstance();
    if (!sdCtrl){
        qDebug("sdCtrl is Null[%p]", sdCtrl);
    }
    // Testディレクトリ(test_folder)を作成する
    testSub->createTestDir();
    // Testディレクトリ(0011Test/0022Test/0033Test/0044Test/0055Test)を作成する
    testSub->createTestDir(testDirCnt);
    // Testファイル(0011Test.prg/0022Test.prg/0033Test.prg)を作成する
    testSub->createTestFile(testFileCnt);
    folderList = sdCtrl->getFolderList(testFolderPath);
    qDebug("folderList count[%d]", folderList.count());
    for(int i = 0; i < folderList.count(); i++){
        QDir dir(testFolderPath + "/" + folderList.at(i).fileName());
        ret = sdCtrl->setFolderPermissions(dir.absolutePath(), folderPermission[i]);
        if (!ret){
            qDebug() << "setFolderPermissions failed Path:" << dir.absolutePath();
        }
        QFile::Permissions permission = sdCtrl->getFolderPermissions(folderList, i);
        qDebug() << "permission:" << permission << "folderPermission:" << folderPermission[i];
        EXPECT_EQ(permission, folderPermission[i]) << "i = " << i;
        //ASSERT_TRUE((permission & folderPermission[i] == folderPermission[i])) << "i = " << i;
    }
    // Testディレクトリ(test_folder)を削除する
    testSub->deleteTestDir();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SdCtrlTest007</testname>
    <category1>CoreLibSdControl</category1>
    <category2>getFolderCount()</category2>
    <category3>フォルダ数取得</category3>
    <case>
        1)Testディレクトリ(test_folder)を作成する
        2)1)のディレクトリの中にtestディレクトリを4個作成する
        3)1)のディレクトリの中にファイルを3個作成する
        4)stat関数でフォルダサイズを取得する
        5)getFolderSize()を実行する
    </case>
    <check>
        1)4)と5)の結果が同じであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSdControlTestBase, SdCtrlTest007)
{
    QString userPath = QDir::homePath();
    QString testFolderPath = userPath + "/itz3000-gui/settings/test_folder";
    int testDirCnt = 4;
    int testFileCnt = 3;
    QFileInfoList folderList;
    int ret  = 0;
    struct stat _stat;
    off_t _fileSize = 0;
    QString _testFolderPath = "";
    const char *_filePath = NULL;
    qint64 fileSize = 0;

    CoreLibSdControl* sdCtrl = testSub->getSdControlInstance();
    if (!sdCtrl){
        qDebug("sdCtrl is Null[%p]", sdCtrl);
    }
    // Testディレクトリ(test_folder)を作成する
    testSub->createTestDir();
    // Testディレクトリ(0011Test/0022Test/0033Test/0044Test/0055Test)を作成する
    testSub->createTestDir(testDirCnt);
    // Testファイル(0011Test.prg/0022Test.prg/0033Test.prg)を作成する
    testSub->createTestFile(testFileCnt);
    folderList = sdCtrl->getFolderList(testFolderPath);
    for(int i = 0; i < folderList.count(); i++){
        _testFolderPath = testFolderPath + "/" + folderList.at(i).fileName();
        QByteArray _str_path = _testFolderPath.toLocal8Bit();
        _filePath = _str_path.constData();
        ret = stat(_filePath, &_stat);
        if (ret == 0){
            _fileSize += _stat.st_size;
        }
    }
    fileSize = sdCtrl->getFolderSize(folderList);
    //qDebug("fileSize[%lld]", fileSize);
    EXPECT_EQ(fileSize, _fileSize);

    // Testディレクトリ(test_folder)を削除する
    testSub->deleteTestDir();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>SdCtrlTest008</testname>
    <category1>CoreLibSdControl</category1>
    <category2>getFolderCount()</category2>
    <category3>フォルダ数取得</category3>
    <case>
        1)Testディレクトリ(test_folder)を作成する
        2)1)のディレクトリの中にtestディレクトリを5個作成する
        3)1)のディレクトリの中にファイルを3個作成する
        4)getFolderList("/home/{user}/itz3000-gui/settings/test_folder")を実行する
    </case>
    <check>
        1)フォルダ数が5になっていること
        2)フォルダリスト名前がtestFolderNamePathテストデータと一致していること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibSdControlTestBase, SdCtrlTest008)
{
    QString userPath = QDir::homePath();
    QString testFolderPath = userPath + "/itz3000-gui/settings/test_folder";
    QString testFileNameList[3] = {
        "0011Test.prg",
        "0022Test.prg",
        "0033Test.prg",
    };
    int testDirCnt = 5;
    int testFileCnt = 3;
    QFileInfoList fileList;

    CoreLibSdControl* sdCtrl = testSub->getSdControlInstance();
    if (!sdCtrl){
        qDebug("sdCtrl is Null[%p]", sdCtrl);
    }
    // Testディレクトリ(test_folder)を作成する
    testSub->createTestDir();
    // Testディレクトリ(0011Test/0022Test/0033Test/0044Test/0055Test)を作成する
    testSub->createTestDir(testDirCnt);
    // Testファイル(0011Test.prg/0022Test.prg/0033Test.prg)を作成する
    testSub->createTestFile(testFileCnt);
    fileList = sdCtrl->getFileList(testFolderPath);
    qDebug("fileList count[%d]", fileList.count());
    EXPECT_EQ(fileList.count(), testFileCnt);
    for(int i = 0; i < fileList.count(); i++){
        EXPECT_EQ(fileList.at(i).fileName(), testFileNameList[i]);
    }
    // Testディレクトリ(test_folder)を削除する
    testSub->deleteTestDir();
}
#endif // CORELIB_UNIT_TEST
