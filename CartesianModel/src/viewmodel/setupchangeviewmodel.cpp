#include "setupchangeviewmodel.h"
#include <QMutexLocker>
#include <QTimer>

SetupChangeViewFileList::SetupChangeViewFileList(QObject *parent):QObject(parent)
{

}

void SetupChangeViewFileList::setFileAttribute(QString fileAttribute)
{
    if(m_fileAttribute != fileAttribute) {
        m_fileAttribute = fileAttribute;
        emit fileAttributeChanged(m_fileAttribute);
    }
}

void SetupChangeViewFileList::setFileNumber(QString fileNumber)
{
    if(m_fileNumber != fileNumber) {
        m_fileNumber = fileNumber;
        emit fileNumberChanged(m_fileNumber);
    }
}
void SetupChangeViewFileList::setFileUpdateTime(QString fileUpdateTime)
{
    if(m_fileUpdateTime != fileUpdateTime) {
        m_fileUpdateTime = fileUpdateTime;
        emit fileUpdateTimeChanged(m_fileUpdateTime);
    }
}
void SetupChangeViewFileList::setFileSetupName(QString fileSetupName)
{
    if(m_fileSetupName != fileSetupName) {
        m_fileSetupName = fileSetupName;
        emit fileSetupNameChanged(m_fileSetupName);
    }
}

void SetupChangeViewFileList::setFileViewIndex(int fileViewIndex)
{
    if(m_fileViewIndex != fileViewIndex) {
        m_fileViewIndex = fileViewIndex;
        emit fileViewIndexChanged(m_fileViewIndex);
    }
}

SetupChangeViewModel::SetupChangeViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm),
      m_commonView(commonView),
      mTimerId{0, 0}
{
    if (m_resource){
        mCsvReader = m_resource->getCsvReaderInstance();
    }
    mFtpManager = FtpManager::get_instance();
    initSettingPenSetData();
    initSetupChangeView();
    mMutexSetupSort = new QMutex(QMutex::Recursive);
    connect(mFtpManager, SIGNAL(ftpFileInfoChanged(int)), this, SLOT(onFtpFileInfoChanged(int)), Qt::DirectConnection);
    connect(mFtpManager, SIGNAL(ftpRemoteFileInfoChanged(bool)), this, SLOT(onFtpRemoteFileInfoChanged(bool)), Qt::DirectConnection);
    getCtrlCodeProcTimeout();
    clearCompareDetailResult();
}

SetupChangeViewModel::SetupChangeViewModel(QObject *object)
    : ViewModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

SetupChangeViewModel::~SetupChangeViewModel()
{
    disconnect(mFtpManager, SIGNAL(ftpRemoteFileInfoChanged(bool)), this, SLOT(onFtpRemoteFileInfoChanged(bool)));
    disconnect(mFtpManager, SIGNAL(ftpFileInfoChanged(int)), this, SLOT(onFtpFileInfoChanged(int)));
    delete mMutexSetupSort;
    mMutexSetupSort = nullptr;
    qDeleteAll(m_setupChangeViewFileList);
    qDeleteAll(m_saveSetupChangeViewFileList);
}

void SetupChangeViewModel::onActivate()
{
    qDebug("%s:%d m_isFtpFileAcquired[%d]", __func__, __LINE__, m_isFtpFileAcquired);
    QTimer::singleShot(10, this, &SetupChangeViewModel::onUpdateSingleTimer);
}

void SetupChangeViewModel::onDeactivate()
{
    qDebug("%s:%d m_isSetupProcActive[%d]", __func__, __LINE__, m_isSetupProcActive);
    m_isSetupProcActive = false;
}

void SetupChangeViewModel::initSetupChangeView()
{
    initSetupChangeViewFileList();
}

void SetupChangeViewModel::startFtpCommunication()
{
    if (!mFtpManager) {
        qCritical("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    mFtpManager->startFtpCommunication();
#ifdef REAL_TARGET
#else // REAL_TARGET
    getFtpFileData();
#endif // REAL_TARGET
}

void SetupChangeViewModel::stopFtpCommunication()
{
    if (!mFtpManager) {
        qCritical("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    mFtpManager->stopFtpCommunication();
}

void SetupChangeViewModel::setSetupChangeViewFileList(QList<QObject *> setupChangeViewFileList)
{
    m_setupChangeViewFileList = setupChangeViewFileList;
    emit setupChangeViewFileListChanged(m_setupChangeViewFileList);
}

void SetupChangeViewModel::setFileListNum(int fileListNum)
{
    if (m_fileListNum == fileListNum){
        return;
    }
    m_fileListNum = fileListNum;
    emit fileListNumChanged(m_fileListNum);
}

void SetupChangeViewModel::setCurFileNumber(QString curFileNumber)
{
    m_curFileNumber = curFileNumber;
    emit curFileNumberChanged(m_curFileNumber);
}

void SetupChangeViewModel::setCurFileName(QString curFileName)
{
    m_curFileName = curFileName;
    emit curFileNameChanged(m_curFileName);
}

void SetupChangeViewModel::setErrCode(QString errCode)
{
    if (m_errCode == errCode){
        return;
    }
    m_errCode = errCode;
    emit errCodeChanged(m_errCode);
}

void SetupChangeViewModel::setDandoriListUpdate(int dandoriListUpdate)
{
    if (m_dandoriListUpdate == dandoriListUpdate){
        return;
    }
    m_dandoriListUpdate = dandoriListUpdate;
    emit dandoriListUpdateChanged(m_dandoriListUpdate);
}

void SetupChangeViewModel::setCompareDiscordFactor(int compareDiscordFactor)
{
    if (m_compareDiscordFactor == compareDiscordFactor){
        return;
    }
    m_compareDiscordFactor = compareDiscordFactor;
    emit compareDiscordFactorChanged(m_compareDiscordFactor);
}

void SetupChangeViewModel::setNotifyingProcessResult(bool notifyingProcessResult)
{
    if (m_notifyingProcessResult == notifyingProcessResult){
        return;
    }
    m_notifyingProcessResult = notifyingProcessResult;
    emit notifyingProcessResultChanged(m_notifyingProcessResult);
}

void SetupChangeViewModel::clearCompareDetailResult()
{
    m_compareDetailResult.clear();
    m_compareDetailResultColor.clear();
    for(int i = 0; i < SETUP_FILE_COMPARE_FACTOR_MAX; i++){
        m_compareDetailResult.append("");
        m_compareDetailResultColor.append("");
    }
}

void SetupChangeViewModel::initSetupChangeViewFileList()
{
    for (int i = 0; i < mSetupMngFileNumMax; i++){
        SetupChangeViewFileList *setupChangeViewFileList
            = new  SetupChangeViewFileList();
        setupChangeViewFileList->setFileAttribute("");
        setupChangeViewFileList->setFileNumber("");
        setupChangeViewFileList->setFileUpdateTime("");
        setupChangeViewFileList->setFileSetupName("");
        setupChangeViewFileList->setFileViewIndex(i);
        m_setupChangeViewFileList.append(setupChangeViewFileList);
    }
    setSetupChangeViewFileList(m_setupChangeViewFileList);
}

void SetupChangeViewModel::initSettingPenSetData()
{
    m_setupSelectedTagRGB               = m_resource->dataColor(26, 1);
    m_setupCommandPressRGB              = m_resource->dataColor(26, 2);
    m_setupFileContentDispRGB           = m_resource->dataColor(26, 3);
    m_setupSelectedFileContentDispRGB   = m_resource->dataColor(26, 4);
    m_setupPushedFileContentDispRGB     = m_resource->dataColor(26, 5);
    //m_userLevelTabDispSettings          = m_resource->getIntValueList(26, 8, 5, false);
    m_userLevelCmdButtonDispSettings    = m_resource->getIntValueList(26, 8, 5, false);
#if 0
    qDebug() << "m_setupSelectedTagRGB:" << m_setupSelectedTagRGB;
    qDebug() << "m_setupCommandPressRGB:" << m_setupCommandPressRGB;
    qDebug() << "m_setupFileContentDispRGB:" << m_setupFileContentDispRGB;
    qDebug() << "m_setupSelectedFileContentDispRGB:" << m_setupSelectedFileContentDispRGB;
    qDebug() << "m_setupPushedFileContentDispRGB:" << m_setupPushedFileContentDispRGB;
#if 0
    for(int i = 0; i < 5; i++){
        qDebug() << "value(0):" << m_userLevelTabDispSettings.at(i).value(0) << "value(1):" << m_userLevelTabDispSettings.at(i).value(1);
    }
#endif
    for(int i = 0; i < 5; i++){
        qDebug() << "value(0):" << m_userLevelCmdButtonDispSettings.at(i).value(0) << "value(1):" << m_userLevelCmdButtonDispSettings.at(i).value(1)
                 << "value(2):" << m_userLevelCmdButtonDispSettings.at(i).value(2) << "value(3):" << m_userLevelCmdButtonDispSettings.at(i).value(3)
                 << "value(4):" << m_userLevelCmdButtonDispSettings.at(i).value(4);
    }
#endif
}

void SetupChangeViewModel::getModbusAsciiData(QList<int> &writeData, QString writeStr)
{
    if (writeStr.count() <= 0){
        qCritical() << "SetupChangeViewModel::getModbusAsciiData invalid writeStr:" << writeStr;
        return;
    }
    //QString emptyStr = " ";
    //qDebug("%s:%d writeStr length[%d]", __func__, __LINE__, writeStr.length());
    for(int i = 0; i < writeStr.length(); i++){
        if (i % 2){
            //qDebug("%s:%d 0x%02x, 0x%02x", __func__, __LINE__, writeStr.at(i-1).unicode(), writeStr.at(i).unicode());
            int fileNameAscii = (writeStr.at(i - 1).unicode() << 8) |  writeStr.at(i).unicode();
            writeData.append(fileNameAscii);
        } else {
            if (i == writeStr.length() - 1){
                //qDebug("%s:%d 0x%02x", __func__, __LINE__, writeStr.at(i).unicode());
                int fileNameAscii = (writeStr.at(i).unicode() << 8) | 0x20;
                writeData.append(fileNameAscii);
            }
        }
    }
#if 0
    for(int i = 0; i < writeData.length(); i++){
        qDebug("%s:%d writeData[%d]=[0x%02x]", __func__, __LINE__, i, writeData.at(i));
    }
#endif
}

void SetupChangeViewModel::getFtpFileList()
{
    if (!mFtpManager) {
        qCritical("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    mFtpManager->requestFtpListCommand("/");
}

void SetupChangeViewModel::getFtpFileInfo()
{
    if (!mFtpManager) {
        qCritical("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    mFtpManager->getFtpFileInfo(FtpManager::FtpFileInfo::FTP_DANDORI_CSV);
}

void SetupChangeViewModel::getFtpFileData(bool isReOrder)
{
    if (!mFtpManager) {
        qCritical("%s:%d mFtpManager is not share", __func__, __LINE__);
        if(isReOrder) {
            m_isReOrderSetupDataFinish = false;
            emit isReOrderSetupDataFinishChanged(m_isReOrderSetupDataFinish);
        }
        return;
    }
    parseDandoriFile(mFtpManager->getFtpFilePath(FtpManager::FtpFileInfo::FTP_DANDORI_CSV));
    if(isReOrder) {
        m_isReOrderSetupDataFinish = true;
        emit isReOrderSetupDataFinishChanged(m_isReOrderSetupDataFinish);
    }
}

void SetupChangeViewModel::getFtpFileInfoAndData()
{
    connect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
    connect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
    connect(mFtpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)), Qt::QueuedConnection);
    timerStart(SETUP_REORDER_FTP_INTERVAL, 1);
    getFtpFileInfo();
}

void SetupChangeViewModel::modbusGetCompareDiscordFactors()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, SETUP_COMPARE_DISCORD_FACTOR_HR, 0x0002
        );
    m_communication->setOneShotModbusCommand(readCommand);
}

void SetupChangeViewModel::getCompareDiscordFactorsAndNotify()
{
    if (!m_modbus) return;
    int discordFactor = m_modbus->getLongReceiveDataHR(SETUP_COMPARE_DISCORD_FACTOR_HR);
    qDebug("%s:%d discordFactor[%d][0x%08x]", __func__, __LINE__, discordFactor, discordFactor);
    initCtrlStatusCode();
    stopModbusMonitoring();
    if (discordFactor != 0){
        bool ret = parseCompareDetailResult(discordFactor);
        if (ret == false){
            emit compareDetailResultChanged(m_compareDetailResult);
            emit compareDetailResultColorChanged(m_compareDetailResultColor);
        }
        setCompareDiscordFactor(discordFactor);
    } else {
        emit compareDetailResultChanged(m_compareDetailResult);
        emit compareDetailResultColorChanged(m_compareDetailResultColor);
        setCompareDiscordFactor(discordFactor);
    }
}

bool SetupChangeViewModel::parseCompareDetailResult(int compareDiscordFactor)
{
    if (compareDiscordFactor <= 0){
        qCritical("%s:%d compareDiscordFactor[%d] is invalid", __func__, __LINE__, compareDiscordFactor);
        return false;
    }
    if (m_compareDetailResult.count() <= 0 || m_compareDetailResult.count() > SETUP_FILE_COMPARE_FACTOR_MAX){
        qCritical("%s:%d m_compareDetailResult count[%d] is invalid", __func__, __LINE__, m_compareDetailResult.count());
        return false;
    }
    int curListIndex = 0;
    for(int i = 0; i < m_compareDetailResult.count(); i++){
        if (((compareDiscordFactor >> i) & 0x01)){
            m_compareDetailResult.replace(curListIndex,
                 m_resource->label(SETUP_FILE_COMPARE_FACTOR_RES_GROUP, SETUP_FILE_COMPARE_FACTOR_RES_START_INDEX + i, m_resource->language()));
            m_compareDetailResultColor.replace(curListIndex,
                m_resource->rgb(SETUP_FILE_COMPARE_FACTOR_RES_GROUP, SETUP_FILE_COMPARE_FACTOR_RES_START_INDEX + i));
            curListIndex++;
        }
    }
    emit compareDetailResultChanged(m_compareDetailResult);
    emit compareDetailResultColorChanged(m_compareDetailResultColor);
    return true;
}

#if 0
int SetupChangeViewModel::getTabDispSetting(int tabIndex, int userLevel)
{
    if (userLevel < 0 || userLevel > 4){
        qCritical("%s:%d userLevel is invalid", __func__, __LINE__);
        return -1;
    }
    if (tabIndex < 0 || tabIndex > 1){
        qCritical("%s:%d tabIndex is invalid", __func__, __LINE__);
        return -1;
    }
    return m_userLevelTabDispSettings.at(userLevel).value(tabIndex);
}
#endif

int SetupChangeViewModel::getCmdButtonDispSetting(int btnIndex, int userLevel)
{
    if (userLevel < 0 || userLevel > 4){
        qCritical("%s:%d userLevel is invalid", __func__, __LINE__);
        return -1;
    }
    if (btnIndex < 0 || btnIndex > 4){
        qCritical("%s:%d btnIndex is invalid", __func__, __LINE__);
        return -1;
    }
    return m_userLevelCmdButtonDispSettings.at(userLevel).value(btnIndex);
}

void SetupChangeViewModel::sortFileNumber(int sortMode)
{
    //qDebug("%s:%d sortMode[%d]", __func__, __LINE__, sortMode);
    lockSortMutex();
    if(sortMode == 0){  // ファイルNo:降順ソート
        sortDescendOrderFileNumber();
    } else {            // ファイルNo:昇順ソート
        sortAscendOrderFileNumber();
    }
    unlockSortMutex();
}

void SetupChangeViewModel::sortFileName(int sortMode)
{
    //qDebug("%s:%d sortMode[%d]", __func__, __LINE__, sortMode);
    lockSortMutex();
    if(sortMode == 0){  // アルファベット(aaa→zzz)順ソート
        sortAscendAlphabetOrderFileName();
    } else {            // アルファベット(zzz→aaa)順ソート
        sortDescendAlphabetOrderFileName();
    }
    unlockSortMutex();
}

void SetupChangeViewModel::sortFileUpdateTime(int sortMode)
{
    //qDebug("%s:%d sortMode[%d]", __func__, __LINE__, sortMode);
    lockSortMutex();
    if(sortMode == 0){  // 時間古い順ソート
        sortOldestTimeOrderFileUpdateTime();
    } else {            // 時間新しい順ソート
        sortNewTimeOrderFileUpdateTime();
    }
    unlockSortMutex();
}

QString SetupChangeViewModel::searchFreeFileNumber(QString searchStartNumber)
{
    bool is_ok = false;
    int startNumber = 0;
    int freeNumber = 0;
    QString freeNumberStr = "";

    startNumber = searchStartNumber.toInt(&is_ok, 10);
    if (!is_ok){
        qCritical() << "SetupChangeViewModel::searchFreeFileNumber not is num:" << searchStartNumber;
        return freeNumberStr;
    }
    startNumber += 1;
    freeNumber = searchFromBeginningFileNumber(startNumber);
    if (freeNumber > SETUP_FILE_NUMBER_MAX){
        qCritical() << "SetupChangeViewModel::searchFreeFileNumber freeNumber is exceeding the limit:" << freeNumber;
        return freeNumberStr;
    }
    freeNumberStr = (freeNumber < 10 ? "00" + QString::number(freeNumber, 10) : \
                    (freeNumber < 100 ? "0" + QString::number(freeNumber, 10) : \
                    (freeNumber < 1000 ? QString::number(freeNumber, 10) : \
                    QString::number(freeNumber, 10))));
    return freeNumberStr;
}

void SetupChangeViewModel::modbusGetCurFileNumberAndName()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, SETUP_FILE_DIR_NUM_SR, SETUP_FILE_DIR_NAME_SIZE + 1
        );
    startModbusOneShot();
    m_communication->setOneShotModbusCommand(readCommand);
}

void SetupChangeViewModel::updateDandoriFile(QString fileNumber, QString fileSize, QString lastModified)
{
    qDebug() << "SetupChangeViewModel::updateDandoriFile fileNumber:" << fileNumber << "fileSize:" << fileSize << "lastModified:" << lastModified;
    if (!mFtpManager) return;
    stRemoteDirFileInfo updateFile;
    bool is_ok;
    if (m_dandoriListUpdate == (int)DandoriFileStatus::FileInit){
        updateFile.name = "COND0" + fileNumber + ".CND";
        updateFile.parent = "";
        updateFile.path = "/";
        updateFile.lastModified = QDateTime::fromString(lastModified, Qt::ISODate);
        updateFile.size = fileSize.toLong(&is_ok, 10);
        updateFile.isDir = false;
        mFtpManager->updateRemoteFileInfo(m_dandoriListUpdate, updateFile);
    } else if (m_dandoriListUpdate == (int)DandoriFileStatus::FileSave){
        updateFile.name = "COND0" + fileNumber + ".CND";
        if (mSaveDandoriFileName == updateFile.name){
            updateFile.parent = "";
            updateFile.path = "/";
            updateFile.lastModified = QDateTime::fromString(lastModified, Qt::ISODate);
            updateFile.size = fileSize.toLong(&is_ok, 10);
            updateFile.isDir = false;
            mFtpManager->updateRemoteFileInfo(m_dandoriListUpdate, updateFile);
        }
    } else if (m_dandoriListUpdate == (int)DandoriFileStatus::FileErase){
        updateFile.name = mEraseDandoriFileName;
        updateFile.parent = "";
        updateFile.path = "/";
        updateFile.lastModified = QDateTime::fromString(lastModified, Qt::ISODate);
        updateFile.size = fileSize.toLong(&is_ok, 10);
        updateFile.isDir = false;
        mFtpManager->updateRemoteFileInfo(m_dandoriListUpdate, updateFile);
        m_dandoriListUpdate = (int)DandoriFileStatus::NoOperation;
    } else if (m_dandoriListUpdate == (int)DandoriFileStatus::FileDownload){
        updateFile.name = "COND0" + fileNumber + ".CND";
        updateFile.parent = "";
        updateFile.path = "/";
        updateFile.lastModified = QDateTime::fromString(lastModified, Qt::ISODate);
        updateFile.size = fileSize.toLong(&is_ok, 10);
        updateFile.isDir = false;
        mFtpManager->updateRemoteFileInfo(m_dandoriListUpdate, updateFile);
    }
    return;
}

void SetupChangeViewModel::parseDandoriFile(QString filePath)
{
    if (!m_resource) return;
    QFile dandoriFile(filePath);

    if (!dandoriFile.exists()){
        qCritical("%s:%d dandoriFile is not exists", __func__, __LINE__);
        return;
    }
    if (!dandoriFile.open(QIODevice::ReadWrite)){
        qCritical("%s:%d dandoriFile open failed", __func__, __LINE__);
        return;
    }
    QTextStream in(&dandoriFile);
    int lineCount = 0;
    int startCharIndex = 1;
    int findCommaCnt = 0;
    QString fileListStr[5];
    while (!in.atEnd()) {
        QString line = in.readLine();
        startCharIndex = 2;
        //qDebug() << "dandoriFile line:" << line;
        if (lineCount > 0){
            for(int i = 1; i < line.length(); i++){
                if(line.at(i).unicode() == 0x2C && findCommaCnt < 4){       // ,
                    fileListStr[findCommaCnt] = line.mid(startCharIndex, i - (startCharIndex + 1));
                    startCharIndex = i + 2;
                    findCommaCnt++;
                    if (findCommaCnt >= 4){
                        fileListStr[findCommaCnt] = line.mid(startCharIndex, (line.length() - (startCharIndex + 1)));
                        break;
                    }
                }
            }
            SetupChangeViewFileList *setupChangeViewFileList
                = new  SetupChangeViewFileList();
            setupChangeViewFileList->setFileAttribute(fileListStr[3]);
            setupChangeViewFileList->setFileNumber(fileListStr[0]);
            setupChangeViewFileList->setFileUpdateTime(fileListStr[4]);
            setupChangeViewFileList->setFileSetupName(fileListStr[1]);
            setupChangeViewFileList->setFileViewIndex((lineCount - 1));
            m_setupChangeViewFileList.replace((lineCount - 1), setupChangeViewFileList);
            findCommaCnt = 0;
#if 1
#else
            updateDandoriFile(fileListStr[0], fileListStr[2], fileListStr[4]);
#endif
        }
        lineCount++;
    }
    if (lineCount >= 1) { mFtpManager->sortRemoteFile(); }
    setFileListNum(lineCount - 1);
    notifySetupFileList();
    qDebug("%s:%d lineCount[%d] m_dandoriListUpdate[%d]", __func__, __LINE__, lineCount, m_dandoriListUpdate);
    if (m_dandoriListUpdate >= 0){
        m_dandoriListUpdate = (int)DandoriFileStatus::NoOperation;
    }
}

void SetupChangeViewModel::sortAscendOrderFileNumber()
{
    std::sort(m_setupChangeViewFileList.begin(), m_setupChangeViewFileList.end(), [] (QObject *left, QObject *right) {
        auto *l = static_cast<SetupChangeViewFileList *>(left);
        auto *r = static_cast<SetupChangeViewFileList *>(right);
        return (l->fileNumber()).toInt() > (r->fileNumber()).toInt();        // ファイル番号の大小で並び替える
    });
    notifySetupFileList();
}

void SetupChangeViewModel::sortDescendOrderFileNumber()
{
    std::sort(m_setupChangeViewFileList.begin(), m_setupChangeViewFileList.end(), [] (QObject *left, QObject *right) {
        auto *l = static_cast<SetupChangeViewFileList *>(left);
        auto *r = static_cast<SetupChangeViewFileList *>(right);
        return (l->fileNumber()).toInt() > (r->fileNumber()).toInt();        // ファイル番号の大小で並び替える
    });
    for(int i = 0; i < m_fileListNum/2; i++){
        m_setupChangeViewFileList.swap(i, m_fileListNum - 1 - i);
    }
    notifySetupFileList();
}

void SetupChangeViewModel::sortAscendAlphabetOrderFileName()
{
    std::sort(m_setupChangeViewFileList.begin(), m_setupChangeViewFileList.end(), [] (QObject *left, QObject *right) {
        auto *l = static_cast<SetupChangeViewFileList *>(left);
        auto *r = static_cast<SetupChangeViewFileList *>(right);
        if(l->fileSetupName() == r->fileSetupName()) {              // ファイル名が同じ場合
            return (l->fileNumber()).toInt() > (r->fileNumber()).toInt();        // ファイル番号の大小で並び替える
        } else {
            return l->fileSetupName() > r->fileSetupName();        // ファイル名順で並び替える
        }
    });
    for(int i = 0; i < m_fileListNum/2; i++){
        m_setupChangeViewFileList.swap(i, m_fileListNum - 1 - i);
    }
    notifySetupFileList();
}

void SetupChangeViewModel::sortDescendAlphabetOrderFileName()
{
    std::sort(m_setupChangeViewFileList.begin(), m_setupChangeViewFileList.end(), [] (QObject *left, QObject *right) {
        auto *l = static_cast<SetupChangeViewFileList *>(left);
        auto *r = static_cast<SetupChangeViewFileList *>(right);
        if(l->fileSetupName() == r->fileSetupName()) {              // ファイル名が同じ場合
            return (l->fileNumber()).toInt() > (r->fileNumber()).toInt();        // ファイル番号の大小で並び替える
        } else {
            return l->fileSetupName() > r->fileSetupName();        // ファイル名順で並び替える
        }
    });
    notifySetupFileList();
}

void SetupChangeViewModel::sortOldestTimeOrderFileUpdateTime()
{
    std::sort(m_setupChangeViewFileList.begin(), m_setupChangeViewFileList.end(), [] (QObject *left, QObject *right) {
        auto *l = static_cast<SetupChangeViewFileList *>(left);
        auto *r = static_cast<SetupChangeViewFileList *>(right);
        if(l->fileUpdateTime() == r->fileUpdateTime()) {              // ファイル更新時間が同じ場合
            return (l->fileNumber()).toInt() > (r->fileNumber()).toInt();        // ファイル番号の大小で並び替える
        } else {
            return l->fileUpdateTime() > r->fileUpdateTime();        // ファイル更新時間で並び替える
        }
    });
    for(int i = 0; i < m_fileListNum/2; i++){
        m_setupChangeViewFileList.swap(i, m_fileListNum - 1 - i);
    }
    notifySetupFileList();
}

void SetupChangeViewModel::sortNewTimeOrderFileUpdateTime()
{
    std::sort(m_setupChangeViewFileList.begin(), m_setupChangeViewFileList.end(), [] (QObject *left, QObject *right) {
        auto *l = static_cast<SetupChangeViewFileList *>(left);
        auto *r = static_cast<SetupChangeViewFileList *>(right);
        if(l->fileUpdateTime() == r->fileUpdateTime()) {              // ファイル更新時間が同じ場合
            return (l->fileNumber()).toInt() > (r->fileNumber()).toInt();        // ファイル番号の大小で並び替える
        } else {
            return l->fileUpdateTime() > r->fileUpdateTime();        // ファイル更新時間で並び替える
        }
    });
    notifySetupFileList();
}

int SetupChangeViewModel::searchFromBeginningFileNumber(int startNumber)
{
    bool is_ok = false;
    int curFileNumber = 0;
    int nextFileNumber = 0;
    int grepNumber = 0;
    int freeNumber = 1;

    grepNumber = startNumber;
    QList<QString> fileNumberList;
    for(int i = 0; i < m_fileListNum; i++){
        SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_setupChangeViewFileList.at(i));
        fileNumberList.append(setupChangeViewFileList->fileNumber());
    }
    std::sort(fileNumberList.begin(), fileNumberList.end());
    for(int i = 0; i < fileNumberList.count(); i++){
        //qDebug() << "fileNumberList:" << fileNumberList.at(i);
        curFileNumber = fileNumberList.at(i).toInt(&is_ok, 10);
        if (!is_ok){
            qDebug() << "SetupChangeViewModel::searchFromBeginningFileNumber not is num:" << fileNumberList.at(i);
            continue;
        }
        if (grepNumber != curFileNumber){
            freeNumber = grepNumber;
            break;
        } else {
            if (i < fileNumberList.count() - 1){
                nextFileNumber = fileNumberList.at(i + 1).toInt(&is_ok, 10);
                if (!is_ok){
                    qDebug() << "not is num:" << fileNumberList.at(i + 1);
                    continue;
                }
                if (grepNumber + 1 == nextFileNumber){
                    grepNumber++;
                    continue;
                } else {
                    freeNumber = grepNumber + 1;
                    break;
                }
            } else {
                freeNumber = curFileNumber + 1;
            }
        }
    }
    qDebug("%s:%d freeNumber[%d]", __func__, __LINE__, freeNumber);
    return freeNumber;
}

void SetupChangeViewModel::timerStart(int timeOut, int mode)
{
    if (!mIsTimerStatus) {
        mTimerId[mode] = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void SetupChangeViewModel::timerStop(int mode)
{
    if (mIsTimerStatus) {
        killTimer(mTimerId[mode]);
        mTimerCnt = 0;
        mIsTimerStatus = false;
    }
}

bool SetupChangeViewModel::checkFileNumber(int fileNumber)
{
    bool ret = true;
    qDebug("%s:%d fileNumber[%d] m_fileListNum[%d]", __func__, __LINE__, fileNumber, m_fileListNum);
    for(int i = 0; i < m_fileListNum; i++){
        SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_setupChangeViewFileList.at(i));
        QString fileNumStr = setupChangeViewFileList->fileNumber();
        bool is_ok;
        int fileNum = fileNumStr.toInt(&is_ok, 10);
        if (!is_ok){
            continue;
        }
        if (fileNum == fileNumber){
            ret = false;
            break;
        }
    }
    return ret;
}

void SetupChangeViewModel::checkReOrderFtpStatus()
{
    if(mReOrderTimerCnt >= SETUP_REORDER_FTP_TIMEOUT_COUNT){
        qDebug("%s:%d mReOrderTimerCnt[%d]", __func__, __LINE__, mReOrderTimerCnt);
        disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
        disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
        disconnect(mFtpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)));
        timerStop(1);
        mReOrderTimerCnt = 0;
        m_isFtpGetFinish = false;
        m_isFtpGetError = false;
        m_isReOrderSetupDataListFinish= false;
        emit isReOrderSetupDataListFinishChanged(m_isReOrderSetupDataListFinish);
    } else {
        if(m_isFtpGetFinish || m_isFtpGetFinish){
            qDebug("%s:%d mReOrderTimerCnt[%d] m_isFtpGetFinish[%d] m_isFtpGetFinish[%d]",
                __func__, __LINE__, mReOrderTimerCnt, m_isFtpGetFinish, m_isFtpGetFinish);
            disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
            disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
            disconnect(mFtpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)));
            timerStop(1);
            mReOrderTimerCnt = 0;
            if (m_isFtpGetFinish){
                m_isReOrderSetupDataListFinish = true;
                emit isReOrderSetupDataListFinishChanged(m_isReOrderSetupDataListFinish);
                m_isFtpGetFinish = false;
                m_isFtpGetError = false;
            } else if (m_isFtpGetError) {
                m_isReOrderSetupDataListFinish= false;
                emit isReOrderSetupDataListFinishChanged(m_isReOrderSetupDataListFinish);
                m_isFtpGetFinish = false;
                m_isFtpGetError = false;
            }
        }
    }
}

void SetupChangeViewModel::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent) return;
    if(timerEvent->timerId() == mTimerId[0]){
        mTimerCnt++;
        checkStatusCode();
    } else if (timerEvent->timerId() == mTimerId[1]){
        mReOrderTimerCnt++;
        checkReOrderFtpStatus();
    }
}

void SetupChangeViewModel::startModbusOneShot()
{
    if (!m_communication) return;
    m_communication->setOneShotModbusCaller(CommunicationThread::OneShotReadModBusCaller::CallerSetupChangePageModel);
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this,
       &SetupChangeViewModel::onOneShotReadFinished, Qt::QueuedConnection);
}

void SetupChangeViewModel::stopModbusOneShot()
{
    if (!m_communication) return;
    m_communication->setOneShotModbusCaller(CommunicationThread::OneShotReadModBusCaller::CallerDefault);
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this,
       &SetupChangeViewModel::onOneShotReadFinished);
}

void SetupChangeViewModel::startModbusMonitoring()
{
    if (!m_communication) return;
    if (!mIsModbusMonitor){
        connect(m_communication, &CommunicationThread::feed, this, &SetupChangeViewModel::update, Qt::QueuedConnection);
        mIsModbusMonitor = true;
    }
}

void SetupChangeViewModel::stopModbusMonitoring()
{
    if (!m_communication) return;
    if (mIsModbusMonitor){
        disconnect(m_communication, &CommunicationThread::feed, this, &SetupChangeViewModel::update);
        mIsModbusMonitor = false;
    }
}

void SetupChangeViewModel::modbusReadStatusCode()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, SETUP_EXECUTION_STATE_SR, 0x0001
        );
    m_communication->setOneShotModbusCommand(readCommand);
}

void SetupChangeViewModel::modbusReadCurrentCode()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, SETUP_MOT_CONTROL_COMMAND_SR, 0x0001
        );
    m_communication->setOneShotModbusCommand(readCommand);
}

void SetupChangeViewModel::modbusReadStatusCurrentCode()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, SETUP_MOT_CONTROL_COMMAND_SR, 0x0002
        );
    m_communication->setOneShotModbusCommand(readCommand);
}

void SetupChangeViewModel::initCtrlStatusCode()
{
    mCurCtrlCode = -1;
    mCurStatusCode = -1;
}

void SetupChangeViewModel::checkErrCodeAndNotify(int errCode)
{
    if (errCode < 0){
        qCritical("%s:%d errCode[%d]", __func__, __LINE__, errCode);
        return;
    }
    QString errCodeStr = QString::number(errCode);
    qDebug() << "checkErrCodeAndNotify errCodeStr" << errCodeStr;
    setErrCode(errCodeStr);
}

void SetupChangeViewModel::holdSaveSetupFile(QString fileNumber, QString fileComment, QString fileAttr)
{
    if (!m_commonView) return;
    bool is_ok;
    m_saveSetupChangeViewFileList.clear();
    qDebug() << "holdSaveSetupFile fileNumber:" << fileNumber << "fileComment:" << fileComment << "fileAttr:" << fileAttr
        << "curDate:" << m_commonView->curDate() << "curTime:" << m_commonView->curTime() << "curTimeUptoMinutes:" << m_commonView->curTimeUptoMinutes();
    QString curDateTime = m_commonView->curDate() + " " + m_commonView->curTimeUptoMinutes();
    qDebug() << "holdSaveSetupFile curDateTime:" << curDateTime;
    SetupChangeViewFileList *setupChangeViewFileList
        = new  SetupChangeViewFileList();
    setupChangeViewFileList->setFileAttribute(fileAttr);
    setupChangeViewFileList->setFileNumber(fileNumber);
    setupChangeViewFileList->setFileUpdateTime(curDateTime);
    setupChangeViewFileList->setFileSetupName(fileComment);
    setupChangeViewFileList->setFileViewIndex(m_fileListNum);
    m_saveSetupChangeViewFileList.append(setupChangeViewFileList);
    int fileNum = fileNumber.toInt(&is_ok, 10);
    mSaveDandoriFileName = (fileNum < 10) ? ("COND0" + fileNumber + ".CND") : ((fileNum < 100) ? ("COND0" + fileNumber + ".CND") : ("COND0" + fileNumber + ".CND"));
}

void SetupChangeViewModel::updateSetupFile()
{
    for(int i = 0; i < m_saveSetupChangeViewFileList.count(); i++){
        SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_saveSetupChangeViewFileList.at(i));
        //qDebug() << "fileNumber:" << setupChangeViewFileList->fileNumber()
        //         << "fileComment:" << setupChangeViewFileList->fileSetupName()
        //         << "fileAttribute:" << setupChangeViewFileList->fileAttribute()
        //         << "fileUpdateTime:" << setupChangeViewFileList->fileUpdateTime()
        //         << "fileViewIndex:" << setupChangeViewFileList->fileViewIndex();
        m_setupChangeViewFileList.replace(setupChangeViewFileList->fileViewIndex(), setupChangeViewFileList);
    }
    setFileListNum(m_fileListNum + 1);
    // for(int i = 0; i < m_fileListNum; i++){
    //    SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_setupChangeViewFileList.at(i));
    //    qDebug() << "fileNumber:" << setupChangeViewFileList->fileNumber() << "fileUpdateTime:" << setupChangeViewFileList->fileUpdateTime()
    //             << "fileSetupName:" << setupChangeViewFileList->fileSetupName() << "fileAttribute:" << setupChangeViewFileList->fileAttribute();
    // }
    sortDescendOrderFileNumber();
}

void SetupChangeViewModel::holdEraseSetupFile(int listIndex)
{
    bool is_ok;
    mEraseListIndex = listIndex;
    SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_setupChangeViewFileList.at(listIndex));
    int fileNum = (setupChangeViewFileList->fileNumber()).toInt(&is_ok, 10);
    mEraseDandoriFileName = (fileNum < 10) ? ("COND0" + setupChangeViewFileList->fileNumber() + ".CND") :
        ((fileNum < 100) ? ("COND0" + setupChangeViewFileList->fileNumber() + ".CND") : ("COND0" + setupChangeViewFileList->fileNumber() + ".CND"));
    qDebug() << "holdEraseSetupFile mEraseDandoriFileName:" <<mEraseDandoriFileName << "listIndex:" << listIndex << "fileNum:" << fileNum;
}

void SetupChangeViewModel::updateEraseSetupFile()
{
    if (mEraseListIndex < 0){
        return;
    }
    m_setupChangeViewFileList.removeAt(mEraseListIndex);
    addSetupFile();
    setFileListNum(m_fileListNum - 1);
    notifySetupFileList();
}

void SetupChangeViewModel::addSetupFile()
{
    SetupChangeViewFileList *setupChangeViewFileList
        = new  SetupChangeViewFileList();
    setupChangeViewFileList->setFileAttribute("");
    setupChangeViewFileList->setFileNumber("");
    setupChangeViewFileList->setFileUpdateTime("");
    setupChangeViewFileList->setFileSetupName("");
    setupChangeViewFileList->setFileViewIndex(mSetupMngFileNumMax - 1);
    m_setupChangeViewFileList.append(setupChangeViewFileList);
}

void SetupChangeViewModel::lockSortMutex()
{
    if (mMutexSetupSort){
        mMutexSetupSort->lock();
        mIsMutexSetupSort = true;
    }
}

void SetupChangeViewModel::unlockSortMutex()
{
    if (mMutexSetupSort){
        if (mIsMutexSetupSort) {
            mMutexSetupSort->unlock();
        }
        mIsMutexSetupSort = false;
    }
}

void SetupChangeViewModel::notifySetupFileList()
{
    setSetupChangeViewFileList(m_setupChangeViewFileList);
    m_updateList = true;
    emit updateListChanged(m_updateList);
}

void SetupChangeViewModel::checkReadOpeStatusCode()
{
    if (!m_modbus) {
        initCtrlStatusCode();
        stopModbusMonitoring();
        return;
    }
    switch((SetupChangeViewModel::InternalStatus)mCmdInternalStatus){
    case SetupChangeViewModel::InternalStatus::CurCmdConfirmBefore:
        if (mCurCtrlCode == mExpectedStateCode){
            if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileRead, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileRead, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdReadSuccess){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
                modbusGetSetupFileInfo();
                goto readSuccess;
            } else {
                if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                    && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                    mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                    checkErrCodeAndNotify(mCurStatusCode);
                    initCtrlStatusCode();
                    stopModbusMonitoring();
                    timerStop();
                    m_processingResult = false;
                    emit processingResultChanged(m_processingResult);
                } else {
                    modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileRead, false);
                }
            }
        } else {
            modbusGetCurrentCode(SetupChangeViewModel::ControlCode::SetupFileRead, false, false);
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeStart:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileRead, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileRead, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdReadSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto readSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                checkErrCodeAndNotify(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileRead, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExecuted:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdReadSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto readSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                checkErrCodeAndNotify(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileRead, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeSuccess:
        goto readSuccess;
    default:
        modbusReadStatusCurrentCode();
        break;
    }
    return;
readSuccess:
    initCtrlStatusCode();
    stopModbusMonitoring();
    updateSetupFileInfo();
    qDebug("%s:%d readSuccess mTimerCnt[%d]", __func__, __LINE__, mTimerCnt);
    timerStop();
    m_processingResult = true;
    emit processingResultChanged(m_processingResult);
    if(mFtpManager) {
        qDebug("%s:%d getFtpFileInfo(FtpManager::FtpFileInfo::FTP_NCSTEP_NCP)", __func__, __LINE__);
        mFtpManager->getFtpFileInfo(FtpManager::FtpFileInfo::FTP_NCSTEP_NCP);
    }
}

void SetupChangeViewModel::checkWriteOpeStatusCode()
{
    if (!m_modbus) {
        initCtrlStatusCode();
        stopModbusMonitoring();
        return;
    }
    switch((SetupChangeViewModel::InternalStatus)mCmdInternalStatus){
    case SetupChangeViewModel::InternalStatus::CurCmdConfirmBefore:
        if (mCurCtrlCode == mExpectedStateCode){
            if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWriteSaveSuccess){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
                modbusGetSetupFileInfo();
                goto writeSuccess;
            } else {
                if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                    && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                    mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                    checkErrCodeAndNotify(mCurStatusCode);
                    initCtrlStatusCode();
                    stopModbusMonitoring();
                    timerStop();
                    m_processingResult = false;
                    emit processingResultChanged(m_processingResult);
                } else {
                    modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
                }
            }
        } else {
            modbusGetCurrentCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false, false);
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeStart:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWriteSaveSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto writeSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                checkErrCodeAndNotify(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExecuted:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWriteSaveSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto writeSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                checkErrCodeAndNotify(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeSuccess:
        goto writeSuccess;
    default:
        modbusReadStatusCurrentCode();
        break;
    }
    return;
writeSuccess:
    initCtrlStatusCode();
    stopModbusMonitoring();
    //updateSetupFileInfo();
    qDebug("%s:%d writeSuccess mTimerCnt[%d]", __func__, __LINE__, mTimerCnt);
    timerStop();
    m_processingResult = true;
    emit processingResultChanged(m_processingResult);
}

void SetupChangeViewModel::checkCompareOpeStatusCode()
{
    if (!m_modbus) {
        initCtrlStatusCode();
        stopModbusMonitoring();
        return;
    }
    switch((SetupChangeViewModel::InternalStatus)mCmdInternalStatus){
    case SetupChangeViewModel::InternalStatus::CurCmdConfirmBefore:
        if (mCurCtrlCode == mExpectedStateCode){
            if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileCompare, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileCompare, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdCompareSuccess){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
                modbusGetSetupFileInfo();
                goto compareSuccess;
            } else {
                if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                    && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                    if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::InternalAreaCompareMismatch){
                        timerStop();
                        mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExePostProc;
                        modbusGetCompareDiscordFactors();
                    } else {
                        mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                        checkErrCodeAndNotify(mCurStatusCode);
                        initCtrlStatusCode();
                        stopModbusMonitoring();
                        timerStop();
                        m_processingResult = false;
                        emit processingResultChanged(m_processingResult);
                    }
                } else {
                    modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileCompare, false);
                }
            }
        } else {
            modbusGetCurrentCode(SetupChangeViewModel::ControlCode::SetupFileCompare, false, false);
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeStart:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileCompare, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileCompare, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdCompareSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto compareSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::InternalAreaCompareMismatch){
                    timerStop();
                    mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExePostProc;
                    modbusGetCompareDiscordFactors();
                } else {
                    mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                    checkErrCodeAndNotify(mCurStatusCode);
                    initCtrlStatusCode();
                    stopModbusMonitoring();
                    timerStop();
                    m_processingResult = false;
                    emit processingResultChanged(m_processingResult);
                }
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileCompare, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExecuted:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdCompareSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto compareSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::InternalAreaCompareMismatch){
                    timerStop();
                    mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExePostProc;
                    modbusGetCompareDiscordFactors();
                } else {
                    mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                    checkErrCodeAndNotify(mCurStatusCode);
                    initCtrlStatusCode();
                    stopModbusMonitoring();
                    timerStop();
                    m_processingResult = false;
                    emit processingResultChanged(m_processingResult);
                }
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileCompare, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeSuccess:
        goto compareSuccess;
    case SetupChangeViewModel::InternalStatus::CmdExePostProc:
        getCompareDiscordFactorsAndNotify();
        break;
    default:
        modbusReadStatusCurrentCode();
        break;
    }
    return;
compareSuccess:
    initCtrlStatusCode();
    stopModbusMonitoring();
    //updateSetupFileInfo();
    qDebug("%s:%d compareSuccess mTimerCnt[%d]", __func__, __LINE__, mTimerCnt);
    timerStop();
    m_processingResult = true;
    emit processingResultChanged(m_processingResult);
}

void SetupChangeViewModel::checkEraseOpeStatusCode()
{
    if (!m_modbus) {
        initCtrlStatusCode();
        stopModbusMonitoring();
        return;
    }
    switch((SetupChangeViewModel::InternalStatus)mCmdInternalStatus){
    case SetupChangeViewModel::InternalStatus::CurCmdConfirmBefore:
        if (mCurCtrlCode == mExpectedStateCode){
            if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileErase, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileErase, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdEraseSuccess){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
                modbusGetSetupFileInfo();
                goto eraseSuccess;
            } else {
                if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                    && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                    mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                    checkErrCodeAndNotify(mCurStatusCode);
                    initCtrlStatusCode();
                    stopModbusMonitoring();
                    timerStop();
                    setNotifyingProcessResult(true);
                    m_processingResult = false;
                    emit processingResultChanged(m_processingResult);
                } else {
                    modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileErase, false);
                }
            }
        } else {
            modbusGetCurrentCode(SetupChangeViewModel::ControlCode::SetupFileErase, false, false);
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeStart:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileErase, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileErase, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdEraseSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto eraseSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                checkErrCodeAndNotify(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                setNotifyingProcessResult(true);
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileErase, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExecuted:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdEraseSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto eraseSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                checkErrCodeAndNotify(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                setNotifyingProcessResult(true);
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileErase, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeSuccess:
        goto eraseSuccess;
    default:
        modbusReadStatusCurrentCode();
        break;
    }
    return;
eraseSuccess:
    initCtrlStatusCode();
    stopModbusMonitoring();
    //updateSetupFileInfo();
    qDebug("%s:%d eraseSuccess mTimerCnt[%d]", __func__, __LINE__, mTimerCnt);
#if 1
    //updateEraseSetupFile();
    //mEraseListIndex = -1;
#else
    getFtpFileList();
#endif
    timerStop();
    m_dandoriListUpdate = (int)DandoriFileStatus::FileErase;
    setNotifyingProcessResult(true);
    m_processingResult = true;
    emit processingResultChanged(m_processingResult);
    updateEraseSetupFile();
    mEraseListIndex = -1;
    m_isFtpFileAcquired = false;
    setIsNeedUpdateSetupData(true);
    m_isSetupProcActive = true;
    m_isRequestGetDandoriData = true;
    getFtpFileList();   // getFtpFileInfo -> getFtpFileList
}

void SetupChangeViewModel::checkSaveOpeStatusCode()
{
    if (!m_modbus) {
        initCtrlStatusCode();
        stopModbusMonitoring();
        return;
    }
    //qDebug() << "(SetupChangeViewModel::InternalStatus)mCmdInternalStatus:::::" <<(SetupChangeViewModel::InternalStatus)mCmdInternalStatus << ":::" << mCmdInternalStatus;
    switch((SetupChangeViewModel::InternalStatus)mCmdInternalStatus){
    case SetupChangeViewModel::InternalStatus::CurCmdConfirmBefore:
        if (mCurCtrlCode == mExpectedStateCode){
            if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
            } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWriteSaveSuccess){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
                modbusGetSetupFileInfo();
                goto saveSuccess;
            } else {
                if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                    && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                    mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                    checkErrCodeAndNotify(mCurStatusCode);
                    initCtrlStatusCode();
                    stopModbusMonitoring();
                    timerStop();
                    setNotifyingProcessResult(true);
                    m_processingResult = false;
                    emit processingResultChanged(m_processingResult);
                } else {
                    modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
                }
            }
        } else {
            modbusGetCurrentCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, true, false);
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeStart:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWait){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeStart;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdExe){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExecuted;
            modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
        } else if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWriteSaveSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto saveSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                checkErrCodeAndNotify(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                setNotifyingProcessResult(true);
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExecuted:
        if (mCurStatusCode == (int)SetupChangeViewModel::StatusCode::CmdWriteSaveSuccess){
            mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeSuccess;
            modbusGetSetupFileInfo();
            goto saveSuccess;
        } else {
            if (mCurStatusCode >= (int)SetupChangeViewModel::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)SetupChangeViewModel::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)SetupChangeViewModel::InternalStatus::CmdExeError;
                checkErrCodeAndNotify(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                setNotifyingProcessResult(true);
                m_processingResult = false;
                emit processingResultChanged(m_processingResult);
            } else {
                modbusGetStatusCode(SetupChangeViewModel::ControlCode::SetupFileWriteSave, false);
            }
        }
        break;
    case SetupChangeViewModel::InternalStatus::CmdExeSuccess:
        goto saveSuccess;
    default:
        modbusReadStatusCurrentCode();
        break;
    }
    return;
saveSuccess:
    initCtrlStatusCode();
    stopModbusMonitoring();
    //updateSetupFileInfo();
    qDebug("%s:%d saveSuccess mTimerCnt[%d]", __func__, __LINE__, mTimerCnt);
#if 1
    updateSetupFile();
#else
    getFtpFileList();
#endif
    timerStop();
    m_dandoriListUpdate = (int)DandoriFileStatus::FileSave;
    setNotifyingProcessResult(true);
    m_processingResult = true;
    emit processingResultChanged(m_processingResult);
    m_isFtpFileAcquired = false;
    setIsNeedUpdateSetupData(true);
    //updateSetupFile();
    m_isSetupProcActive = true;
    m_isRequestGetDandoriData = true;
    getFtpFileList();   // getFtpFileInfo -> getFtpFileList
}

void SetupChangeViewModel::checkStatusCode()
{
    if (mCtrlCodeProcTimeoutCnt > 0){
        if (mTimerCnt >= mCtrlCodeProcTimeoutCnt){
            qCritical("%s:%d mTimerCnt[%d]", __func__, __LINE__, mTimerCnt);
            initCtrlStatusCode();
            stopModbusMonitoring();
            timerStop();
            m_processingTimeout = true;
            emit processingTimeoutChanged(m_processingTimeout);
            return;
        }
    }
    switch((SetupChangeViewModel::ControlCode)mExpectedStateCode){
    case SetupChangeViewModel::ControlCode::SetupFileRead:
        checkReadOpeStatusCode();
        break;
    case SetupChangeViewModel::ControlCode::SetupFileWriteSave:
        if (mProcessingSave){
            checkSaveOpeStatusCode();
        } else {
            checkWriteOpeStatusCode();
        }
        break;
    case SetupChangeViewModel::ControlCode::SetupFileCompare:
        checkCompareOpeStatusCode();
        break;
    case SetupChangeViewModel::ControlCode::SetupFileErase:
        checkEraseOpeStatusCode();
        break;
    default:
        initCtrlStatusCode();
        stopModbusMonitoring();
        timerStop();
        break;
    }
}

void SetupChangeViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    if (!m_communication) return;
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerSetupChangePageModel);
    m_communication->setOneShotModbusCommand(data);
}

void SetupChangeViewModel::modbusGetStatusCode(SetupChangeViewModel::ControlCode ctrlCode, bool isFirst)
{
    if (!m_communication) return;
    mExpectedStateCode = (int)ctrlCode;
    modbusReadStatusCode();
    if (isFirst) {
        mCmdInternalStatus = (int)InternalStatus::CurCmdConfirmBefore;
        QThread::msleep(25);                    // sleep 25ms
        timerStart(SETUP_FILE_TIMER_INTERVAL);
    }
    startModbusMonitoring();
}

void SetupChangeViewModel::modbusGetCurrentCode(SetupChangeViewModel::ControlCode ctrlCode, bool isSave, bool isFirst)
{
    if (!m_communication) return;
    mExpectedStateCode = (int)ctrlCode;
    mProcessingSave = isSave;
    modbusReadCurrentCode();
    if (isFirst) {
        mCmdInternalStatus = (int)InternalStatus::CurCmdConfirmBefore;
        //QThread::msleep(25);                    // sleep 25ms
        timerStart(SETUP_FILE_TIMER_INTERVAL);
    }
    startModbusMonitoring();
}

void SetupChangeViewModel::updateSetupFileNumberAndName(stModBusCommunicationData &command)
{
    int setupFileNum = (int)(command.readBuf[0]);
    qDebug("%s:%d setupFileNum[%d]", __func__, __LINE__, setupFileNum);
    QString curFileNumber = (setupFileNum < 10 ? "000" + QString::number(setupFileNum, 10) : \
                    (setupFileNum < 100 ? "00" + QString::number(setupFileNum, 10) : \
                    (setupFileNum < 1000 ? "0" + QString::number(setupFileNum, 10) : \
                    QString::number(setupFileNum, 10))));
    setCurFileNumber(curFileNumber);
    QString curFileName = "";
    QChar str[2];
    for(int i = 0; i < SETUP_FILE_DIR_NAME_SIZE; i++){
        str[0] = (QChar)((command.readBuf[1 + i] & 0xFF00) >> 8);
        str[1] = (QChar)(command.readBuf[1 + i] & 0x00FF);
        curFileName.append(str[0].toLatin1());
        curFileName.append(str[1].toLatin1());
    }
    qDebug() << "updateSetupFileNumberAndName curFileName:" << curFileName;
    setCurFileName(curFileName);
}

void SetupChangeViewModel::updateSetupFileInfo()
{
    if (!m_modbus) return;
    int setupFileNum = m_modbus->getReceiveDataRG(SETUP_FILE_DIR_NUM_SR);
    qDebug("%s:%d setupFileNum[%d]", __func__, __LINE__, setupFileNum);
    QString curFileNumber = (setupFileNum < 10 ? "000" + QString::number(setupFileNum, 10) : \
                    (setupFileNum < 100 ? "00" + QString::number(setupFileNum, 10) : \
                    (setupFileNum < 1000 ? "0" + QString::number(setupFileNum, 10) : \
                    QString::number(setupFileNum, 10))));
    QString curFileName = "";
    setCurFileNumber(curFileNumber);
    QChar str[2];
    for(int i = 0; i < SETUP_FILE_DIR_NAME_SIZE; i++){
        int fileNameInfo = m_modbus->getReceiveDataRG(SETUP_FILE_DIR_NAME_SR + i);
        str[0] = (QChar)((fileNameInfo & 0xFF00) >> 8);
        str[1] = (QChar)(fileNameInfo & 0x00FF);
        curFileName.append(str[0].toLatin1());
        curFileName.append(str[1].toLatin1());
    }
    qDebug() << "updateSetupFileInfo curFileName:" << curFileName;
    setCurFileName(curFileName);
}

void SetupChangeViewModel::modbusGetSetupFileInfo()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, SETUP_FILE_DIR_NUM_SR, SETUP_FILE_DIR_NAME_SIZE + 1
        );
    m_communication->setOneShotModbusCommand(readCommand);
}

int SetupChangeViewModel::readSetupFile(int listIndex)
{
    qDebug("%s:%d listIndex[%d]", __func__, __LINE__, listIndex);
    if (m_setupChangeViewFileList.count() <= 0){
        qCritical("%s:%d FileList is not register count[%d]", __func__, __LINE__, m_setupChangeViewFileList.count());
        return -1;
    }
    if (listIndex < 0 || listIndex >= mSetupMngFileNumMax){
        qCritical("%s:%d listIndex is invalid[%d]", __func__, __LINE__, listIndex);
        return -1;
    }
    SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_setupChangeViewFileList.at(listIndex));
    int fileNumber = setupChangeViewFileList->fileNumber().toInt();
    QList<int> writeData;
    stModBusCommunicationData writeCommand[2];
    qDebug("%s:%d fileNumber[%d]", __func__, __LINE__, fileNumber);
    writeData.append(fileNumber);                                   // 段取換No
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_FILE_DIR_NUM_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[0]);
    writeData.clear();
    writeData.append((int)ControlCode::SetupFileRead);              // 制御code=15
    writeCommand[1] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_MOT_CONTROL_COMMAND_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[1]);
    modbusGetCurrentCode(ControlCode::SetupFileRead, false);
    return 0;
}

int SetupChangeViewModel::writeSetupFile(int listIndex)
{
    qDebug("%s:%d listIndex[%d]", __func__, __LINE__, listIndex);
    if (m_setupChangeViewFileList.count() <= 0){
        qCritical("%s:%d FileList is not register count[%d]", __func__, __LINE__, m_setupChangeViewFileList.count());
        return -1;
    }
    if (listIndex < 0 || listIndex >= mSetupMngFileNumMax){
        qCritical("%s:%d listIndex is invalid[%d]", __func__, __LINE__, listIndex);
        return -1;
    }
    SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_setupChangeViewFileList.at(listIndex));
    int fileNumber = setupChangeViewFileList->fileNumber().toInt();
    QString fileSetupName = setupChangeViewFileList->fileSetupName();
    QList<int> writeData;
    stModBusCommunicationData writeCommand[2];
    writeData.append(fileNumber);
    qDebug("%s:%d fileNumber[%d]", __func__, __LINE__, fileNumber);
    getModbusAsciiData(writeData, fileSetupName);
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_FILE_DIR_NUM_HR, writeData.count(), writeData);
    m_communication->setOneShotModbusCommand(writeCommand[0]);
    writeData.clear();
    writeData.append((int)ControlCode::SetupFileWriteSave);              // 制御code=16
    writeCommand[1] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_MOT_CONTROL_COMMAND_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[1]);
    modbusGetCurrentCode(ControlCode::SetupFileWriteSave, false);
    return 0;
}

int SetupChangeViewModel::saveSetupFile(QString fileNumber, QString fileComment, int fileAttrIndex)
{
    qDebug() << "saveSetupFile fileNumber:" << fileNumber << "fileComment:" << fileComment << "fileAttrIndex:" << fileAttrIndex;
    if (fileComment == "") return -2;
    if (!m_resource) return -1;

    int file_Number = fileNumber.toInt();
    if (file_Number <= 0 || file_Number > 100){
        qCritical("%s:%d file_Number[%d] is invalid", __func__, __LINE__, file_Number);
        return -1;
    }
    //qDebug("%s:%d file_Number[%d]", __func__, __LINE__, file_Number);
    int fileAttrNumber = (fileAttrIndex == 0) ? 2 : 3;
    QList<int> writeData;
    stModBusCommunicationData writeCommand[4];
    writeData.append(file_Number);
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_FILE_DIR_NUM_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[0]);
    writeData.clear();
    getModbusAsciiData(writeData, fileComment);
    //qDebug("%s:%d writeData count[%d]", __func__, __LINE__, writeData.count());
    writeCommand[1] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_FILE_DIR_NAME_HR, writeData.count(), writeData);
    m_communication->setOneShotModbusCommand(writeCommand[1]);
    writeData.clear();
    writeData.append(fileAttrNumber);                                   // ファイル属性(1，教導属性 2，標準ファイル)
    writeCommand[2] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_FILE_ATTR_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[2]);
    writeData.clear();
    writeData.append((int)ControlCode::SetupFileWriteSave);              // 制御code=16
    writeCommand[3] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_MOT_CONTROL_COMMAND_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[3]);
    holdSaveSetupFile(fileNumber, fileComment, (fileAttrIndex == 0 ? "2" : "1"));
    modbusGetCurrentCode(ControlCode::SetupFileWriteSave, true);
    return 0;
}

int SetupChangeViewModel::compareSetupFile(int listIndex)
{
    qDebug("%s:%d listIndex[%d]", __func__, __LINE__, listIndex);
    if (m_setupChangeViewFileList.count() <= 0){
        qCritical("%s:%d FileList is not register count[%d]", __func__, __LINE__, m_setupChangeViewFileList.count());
        return -1;
    }
    if (listIndex < 0 || listIndex >= mSetupMngFileNumMax){
        qCritical("%s:%d listIndex is invalid[%d]", __func__, __LINE__, listIndex);
        return -1;
    }
    SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_setupChangeViewFileList.at(listIndex));
    int fileNumber = setupChangeViewFileList->fileNumber().toInt();
    QString fileSetupName = setupChangeViewFileList->fileSetupName();
    QList<int> writeData;
    stModBusCommunicationData writeCommand[2];
    qDebug("%s:%d fileNumber[%d]", __func__, __LINE__, fileNumber);
    writeData.append(fileNumber);                                       // 段取換No
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_FILE_DIR_NUM_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[0]);
    writeData.clear();
    writeData.append((int)ControlCode::SetupFileCompare);               // 制御code=17
    writeCommand[1] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_MOT_CONTROL_COMMAND_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[1]);
    modbusGetCurrentCode(ControlCode::SetupFileCompare, false);
    return 0;
}

int SetupChangeViewModel::eraseSetupFile(int listIndex)
{
    qDebug("%s:%d listIndex[%d]", __func__, __LINE__, listIndex);
    if (m_setupChangeViewFileList.count() <= 0){
        qCritical("%s:%d FileList is not register count[%d]", __func__, __LINE__, m_setupChangeViewFileList.count());
        return -1;
    }
    if (listIndex < 0 || listIndex >= mSetupMngFileNumMax){
        qCritical("%s:%d listIndex is invalid[%d]", __func__, __LINE__, listIndex);
        return -1;
    }
    SetupChangeViewFileList *setupChangeViewFileList = static_cast<SetupChangeViewFileList *>(m_setupChangeViewFileList.at(listIndex));
    int fileNumber = setupChangeViewFileList->fileNumber().toInt();
    QString fileSetupName = setupChangeViewFileList->fileSetupName();
    QList<int> writeData;
    stModBusCommunicationData writeCommand[2];
    qDebug("%s:%d fileNumber[%d]", __func__, __LINE__, fileNumber);
    writeData.append(fileNumber);                                       // 段取換No
    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_FILE_DIR_NUM_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[0]);
    writeData.clear();
    writeData.append((int)ControlCode::SetupFileErase);                 // 制御code=18
    writeCommand[1] = ModbusCommandGenerator::makeModbusWriteCommand(
                            QModbusPdu::FunctionCode::WriteSingleRegister, SETUP_MOT_CONTROL_COMMAND_HR, 1, writeData);
    m_communication->setOneShotModbusCommand(writeCommand[1]);
    holdEraseSetupFile(listIndex);
    modbusGetCurrentCode(ControlCode::SetupFileErase, false);
    return 0;
}

void SetupChangeViewModel::setIsNeedUpdateSetupData(bool isNeedUpdateSetupData)
{
    if (m_isNeedUpdateSetupData == isNeedUpdateSetupData){
        return;
    }
    m_isNeedUpdateSetupData = isNeedUpdateSetupData;
    emit isNeedUpdateSetupDataChanged(m_isNeedUpdateSetupData);
}

void SetupChangeViewModel::getCtrlCodeProcTimeout()
{
    if (!m_resource){
        qCritical("%s:%d m_resource is invalid", __func__, __LINE__);
        return;
    }
    bool is_ok;
    QString ctrlCodeProcTimeout = m_resource->getPenSetStringValue("1", 26, 0);
    mCtrlCodeProcTimeout = ctrlCodeProcTimeout.toInt(&is_ok, 10);
    if (!is_ok){
        qCritical() << "SetupChangeViewModel::getCtrlCodeProcTimeout failed is_ok:" << is_ok << "ctrlCodeProcTimeout:" << ctrlCodeProcTimeout;
        mCtrlCodeProcTimeout = 0;
        return;
    }
    if((mCtrlCodeProcTimeout > 0 && mCtrlCodeProcTimeout < 10) || mCtrlCodeProcTimeout > 999){
        mCtrlCodeProcTimeout = 0;
    }
    qDebug() << "mCtrlCodeProcTimeout:" << mCtrlCodeProcTimeout;
    if(mCtrlCodeProcTimeout <= 0){
        mCtrlCodeProcTimeoutCnt = 0;
    } else {
        mCtrlCodeProcTimeoutCnt = mCtrlCodeProcTimeout * (1000 / SETUP_FILE_TIMER_INTERVAL);        // mCtrlCodeProcTimeout*10(by 100msインターバル)
    }
    qDebug() << "mCtrlCodeProcTimeoutCnt:" << mCtrlCodeProcTimeoutCnt;
    return;
}

void SetupChangeViewModel::onOneShotReadFinished(stModBusCommunicationData command,
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::OneShotReadModBusCaller::CallerSetupChangePageModel){
//        qDebug("%s:%d oneShotReadCaller[%d] funcCode[0x%02x] readAddress[%d] size[%d]", __func__, __LINE__,
//            oneShotReadCaller, command.funcCode, command.readAddress, command.comSize);
        return;
    }
    switch(command.readAddress){
    case SETUP_FILE_DIR_NUM_SR:
        updateSetupFileNumberAndName(command);
        stopModbusOneShot();
        break;
    case SETUP_FILE_DIR_NAME_SR:
        break;
    default:
        break;
    }
}

void SetupChangeViewModel::update(stModBusCommunicationData command)
{
    if (command.readAddress == SETUP_MOT_CONTROL_COMMAND_SR && command.comSize == 1){
        mCurCtrlCode = m_modbus->getReceiveDataRG(SETUP_MOT_CONTROL_COMMAND_SR);
    } else if (command.readAddress == SETUP_EXECUTION_STATE_SR && command.comSize == 1){
        mCurStatusCode = m_modbus->getReceiveDataRG(SETUP_EXECUTION_STATE_SR);
    } else if (command.readAddress == SETUP_MOT_CONTROL_COMMAND_SR && command.comSize == 2){
        //qDebug("%s:%d readAddress[SR%d] funcCode[%d] comSize[%d] readBuf[%d][%d]",
        //    __func__, __LINE__, command.readAddress, command.funcCode, command.comSize, command.readBuf[0], command.readBuf[1]);
        mCurCtrlCode = m_modbus->getReceiveDataRG(SETUP_MOT_CONTROL_COMMAND_SR);
        mCurStatusCode = m_modbus->getReceiveDataRG(SETUP_EXECUTION_STATE_SR);
    } else if (command.readAddress == SETUP_FILE_DIR_NUM_SR && command.comSize == (SETUP_FILE_DIR_NAME_SIZE + 1)) {
        //qDebug("%s:%d readAddress[SR%d] funcCode[%d] comSize[%d] readBuf[%d][%d]",
        //    __func__, __LINE__, command.readAddress, command.funcCode, command.comSize, command.readBuf[0], command.readBuf[1]);
        updateSetupFileInfo();
        stopModbusMonitoring();
    } else if (
        command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters &&
        command.readAddress == SETUP_COMPARE_DISCORD_FACTOR_HR && command.comSize == SETUP_COMPARE_DISCORD_FACTOR_SIZE){
        checkCompareOpeStatusCode();
    }
}

void SetupChangeViewModel::onFtpStatusChanged(int ftpStatus)
{
    //qDebug("SetupChangeViewModel %s:%d ftpStatus[%d]", __func__, __LINE__, ftpStatus);
    if (ftpStatus == (int)Enums::FtpStatus::FtpStatusGetFinished){

    }
}

void SetupChangeViewModel::onFtpErrorStrNotify(QString errorStr)
{
    qCritical() << "SetupChangeViewModel::onFtpErrorStrNotify errorStr:" << errorStr;
    m_isFtpGetError = true;
}

void SetupChangeViewModel::onFtpGetFinished(const QString &filePath)
{
    qDebug() << "SetupChangeViewModel::onFtpGetFinished filePath:" << filePath;
    (void)filePath;
    m_isFtpGetFinish = true;
}

void SetupChangeViewModel::onFtpFileInfoChanged(int ftpFileInfo)
{
    if (ftpFileInfo != (int)FtpManager::FtpFileInfo::FTP_DANDORI_CSV){
        //qDebug() << "SetupChangeViewModel::onFtpFileInfoChanged ftpFileInfo:" << ftpFileInfo;
        return;
    }
    m_isFtpFileAcquired = true;
    emit isFtpFileAcquiredChanged(m_isFtpFileAcquired);
    mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandNOOP, nullptr, "");
    if(m_dandoriListUpdate > (int)DandoriFileStatus::FileInit){
        getFtpFileData(false);
    }
}

void SetupChangeViewModel::onUpdateSingleTimer()
{
    qDebug("%s:%d m_isFtpFileAcquired[%d]", __func__, __LINE__, m_isFtpFileAcquired);
    emit isFtpFileAcquiredChanged(m_isFtpFileAcquired);
}

void SetupChangeViewModel::onFtpRemoteFileInfoChanged(bool ftpRemoteFileInfo)
{
    qDebug("SetupChangeViewModel::%s:%d ftpRemoteFileInfo[%d] m_isRequestGetDandoriData[%d]",
        __func__, __LINE__, ftpRemoteFileInfo, m_isRequestGetDandoriData);
    if (ftpRemoteFileInfo){
        if (m_isRequestGetDandoriData){
            getFtpFileInfo();
            m_isRequestGetDandoriData = false;
        }
    }
}
