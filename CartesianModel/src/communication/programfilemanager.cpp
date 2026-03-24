#include "programfilemanager.h"

ProgramFileManager::ProgramFileManager(QObject *parent, QString aplName)
    : QObject(parent),
      mSdControl(nullptr),
      mModbusCom(nullptr),
      mMonitorCnt(0),
      progCompileErrInfo{"", -1 , -1}
{
    mSdControl = new CoreLibSdControl(aplName);
    mModbusCom = ModbusCommunication::get_instance();
    if (mModbusCom){
        //qDebug() << "ProgramFileManager:mModbusCom is share";
    }
    m_modbusManager = ModbusManager::get_instance();
    if (m_modbusManager){
        m_communication = m_modbusManager->getAsyncModbusCommunicationInstance();
        connect(m_communication, &CommunicationThread::oneShotReadFinished,
                this, &ProgramFileManager::onOneShotReadFinished, Qt::QueuedConnection);
    }
    qDebug() << "mProgFolderPath:" << mProgFolderPath;
    QDir dir;
#ifdef REAL_TARGET
    mProgFolderPath = mSdControl->getProgramFolderPath();
    qDebug() << "mProgFolderPath:" << mProgFolderPath;
#endif // REAL_TARGET
    if (!dir.exists(mProgFolderPath)){
        if (!dir.mkpath(mProgFolderPath)){
            qDebug() << "mkpath failed mProgFolderPath:" << mProgFolderPath;
            return;
        }
    }
    for(int i = 0; i < PROG_FILE_REGISTER_NUM_MAX; i++) {
        m_programNameList.append("");
    }
}

void ProgramFileManager::progfileMngTerminate()
{
    QMap<int, QList<QObject *>>::iterator iterator;
    for(iterator = m_programDataList.begin(); iterator != m_programDataList.end(); iterator++) {
        QList<QObject *> programDataList = iterator.value();
        qDeleteAll(programDataList);
    }
    if (mSdControl) {
        delete mSdControl;
        mSdControl = nullptr;
    }
}

void ProgramFileManager::timerStart(int timeOut)
{
    if (!mIsTimerStatus) {
        mMonitorCnt = 0;
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}
void ProgramFileManager::timerStop()
{
    if (mIsTimerStatus) {
        killTimer(mTimerId);
        mIsTimerStatus = false;
        mMonitorCnt = 0;
    }
}

bool ProgramFileManager::transferOpeProgramFile(QString *fileName)
{
    bool ret = false;
    int curProgFileNum = 0;

    if (!mModbusCom){
        qDebug() << "ProgramFileManager:mModbusCom is not share";
        return false;
    }
    // check modbus status
    if (mModbusCom->getModbusConnectStatus() != QModbusDevice::ConnectedState){
        mModbusCom->modbusCommunicationStart();
        qDebug() << "sync modbus is not connected";
        return false;
    }
    // get program file list num
    curProgFileNum = getProgramFileListNum();
    if (curProgFileNum > PROG_FILE_REGISTER_NUM_MAX){
        qDebug() << "curProgFileNum is over max:" << curProgFileNum;
        return false;
    }
    if (!fileName){
        // program file 全転送
        ret = transferProgramFileAll();
    } else {
        // program file 個別転送
        ret = transferProgramFileIndividual(*fileName);
    }
    return ret;
}

bool ProgramFileManager::confirmProgramFileExist()
{
    bool ret = true;
    QFileInfoList filelist = getWorkProgramFileList();
    if (filelist.count() <= 0){
        qDebug() << "ProgramFile is not exist";
        ret = false;
    }
    return ret;
}

QFileInfoList ProgramFileManager::getProgramFileList()
{
    //qDebug() << "mProgFolderPath:" << mProgFolderPath;
    return mSdControl->getFileList(mProgFolderPath);
}

QFileInfoList ProgramFileManager::getWorkProgramFileList()
{
    return mSdControl->getFileList(mWorkProgFolderPath);
}

int ProgramFileManager::getProgramFileListNum()
{
    mFileInfoList = getProgramFileList();
    return mFileInfoList.count();
}

QList<stProgFileInfo> ProgramFileManager::getProgramFileInfo()
{
    stProgFileInfo progFileInfo;
    QString fileUpdateTime = "";

    mProgFileInfo.clear();
    if (!mSdControl){
        return mProgFileInfo;
    }
    for (int i = 0; i < getProgramFileListNum(); i++){
        QDateTime dateTime = mSdControl->getFolderLastModified(mFileInfoList, i);
        progFileInfo.fileUpdateTime =
            QString::number(dateTime.date().year()) + "/" + 
            (dateTime.date().month() < 10 ? "0" : "") + QString::number(dateTime.date().month()) + "/" + 
            (dateTime.date().day() < 10 ? "0" : "") + QString::number(dateTime.date().day()) + " " + 
            (dateTime.time().hour() < 10 ? "0" : "") + QString::number(dateTime.time().hour()) + ":" + 
            (dateTime.time().minute() < 10 ? "0" : "") + QString::number(dateTime.time().minute()) + ":" +
            (dateTime.time().second() < 10 ? "0" : "") + QString::number(dateTime.time().second());
        progFileInfo.fileName = mFileInfoList.at(i).fileName();
        progFileInfo.fileNumber = (i + 1 < 100 ? (i + 1 < 10 ? ("00" + QString::number(i + 1)) : ("0" + QString::number(i + 1))) : QString::number(i + 1));
        mProgFileInfo.append(progFileInfo);
    }
    return mProgFileInfo;
}

bool ProgramFileManager::transferProgramFileAll()
{
    bool ret = true;

    for(int i = 0; i < mFileInfoList.count(); i++){
        // modbus prog transfer individual
        ret = transferProgramFileIndividual(mFileInfoList.at(i).fileName());
        if (!ret){
            qDebug() << "transferProgramFileIndividual failed:" << ret;
            goto error;
        }
    }
error:
    return ret;
}

bool ProgramFileManager::transferProgramFileIndividual(QString fileName)
{
    bool ret = true;
    stTranferInfo transInfo;
    QString fileData = "";
    int curTransSize = 0;
    int fileIndex = 0;

    fileIndex = getFileListIndex(fileName);
    if (fileIndex < 0) {
        qDebug() << "mProgFolderPath:" << mProgFolderPath << "file is not exit:" << fileName;
        return false;
    }
    QFile file(mFileInfoList.at(fileIndex).filePath());
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "file open error filePath:" << mFileInfoList.at(fileIndex).filePath();
        ret = false;
        goto error2;
    }
    // get file size
    ret = getTransferInfo(&transInfo, &file);
    if (!ret){
        goto error1;
    }
    while (!file.atEnd()){
        fileData += file.readLine();
    }
    curTransSize = 0;
    for(int j = transInfo.transNum; j > 0; j--){
        if (j == transInfo.transNum){
            // prog転送開始
            ret = modbusWriteProgTransStartMode(transInfo.transNum, mFileInfoList.at(fileIndex).fileName());
            if (!ret){
                goto error1;
            }
            // prog転送ファースト
            ret = modbusWriteProgTranserData(transInfo.transNum - 1, transInfo.firstTransSize, fileData, curTransSize);
            if (!ret){
                goto error1;
            }
            curTransSize += transInfo.firstTransSize;
        } else {
            if (j == 1){
                // prog転送ラスト
                ret = modbusWriteProgTranserData(j, transInfo.lastTransSize, fileData, curTransSize);
                if (!ret){
                    goto error1;
                }
                curTransSize += transInfo.lastTransSize;
            } else {
                // prog転送
                ret = modbusWriteProgTranserData(j, transInfo.middleTransSize, fileData, curTransSize);
                if (!ret){
                    goto error1;
                }
                curTransSize += transInfo.middleTransSize;
            }
        }
    }
    ret = modbusWriteProgTransEndMode();
    if (!ret){
        goto error1;
    }
error1:
    file.close();
error2:
    return ret;
}

bool ProgramFileManager::getTransferInfo(stTranferInfo *transInfo, QFile *file)
{
    if (!transInfo || !file){
        return false;
    }
    transInfo->fileSize = file->size();
    transInfo->transNum = (transInfo->fileSize < PROG_FILE_TRANSER_SIZE_MAX ? 1 : \
        (!(transInfo->fileSize % PROG_FILE_TRANSER_SIZE_MAX) ? transInfo->fileSize / PROG_FILE_TRANSER_SIZE_MAX : \
          (transInfo->fileSize / PROG_FILE_TRANSER_SIZE_MAX) + 1));
    transInfo->firstTransSize = (transInfo->fileSize < PROG_FILE_TRANSER_SIZE_MAX ? transInfo->fileSize : PROG_FILE_TRANSER_SIZE_MAX);
    transInfo->middleTransSize = (transInfo->fileSize < PROG_FILE_TRANSER_SIZE_MAX ? 0 : \
        ((transInfo->transNum == 2) ? transInfo->fileSize % PROG_FILE_TRANSER_SIZE_MAX : PROG_FILE_TRANSER_SIZE_MAX));
    transInfo->lastTransSize = ((transInfo->transNum <= 1) ? transInfo->fileSize : transInfo->fileSize % PROG_FILE_TRANSER_SIZE_MAX);
//    qDebug() << "fileSize:" << transInfo->fileSize << "transNum:" << transInfo->transNum;
//    qDebug() << "firstTransSize:" << transInfo->firstTransSize << "middleTransSize:" << transInfo->middleTransSize
//        << "lastTransSize:" << transInfo->lastTransSize;
    return true;
}

bool ProgramFileManager::modbusWriteProgTransStartMode(int transNum, QString fileName)
{
    bool ret = true;
    int writeData[8] = { 0 };
    int writeSize = (fileName.length() / 2) + 3;

    // 転送モード
    writeData[0] = PROG_FILE_TRANSER_START_MODE;
    // 転送回数
    writeData[1] = transNum;
    // ファイル名
    chgStrToAscii(fileName, writeData, 2);
    // HR001に転送モードを送信
    ret = mModbusCom->modbusWriteHR(PROG_FILE_TRANSER_MODE_HR, writeData, writeSize);
    if (!ret){
        qDebug() << "modbusWriteHR failed:" << ret;
    }
    return ret;
}

bool ProgramFileManager::modbusWriteProgTranserData
    (int remainTransNum, int transSize, QString transData, int curTransSize)
{
    bool ret = true;
    int writeData[59] = { 0 };
    QString writeDataStr = "";
    int writeSize = 0;

    // 転送モード
    writeData[0] = PROG_FILE_TRANSER_MODE;
    // 残り回数
    writeData[1] = remainTransNum;
    // 送信サイズ
    writeData[2] = transSize;
    // ファイルデータ
    for(int i = curTransSize; i < transSize; i++){
        writeDataStr += transData.at(i);
    }
    writeSize = writeDataStr.length() / 2 + 3;
    for(int j = 0; j < writeDataStr.length(); j++){
        if (j % 2) {
            if (writeDataStr.at(j) == '\xa') {
                writeData[3 + (j / 2 + j % 2)] = 0x0A00 | writeDataStr.at(j - 1).unicode();
            } else if (writeDataStr.at(j - 1) == '\xa') {
                writeData[3 + (j / 2 + j % 2)] = (writeDataStr.at(j).unicode() << 8) | 0x000A;
            } else {
                writeData[3 + (j / 2 + j % 2)] = (writeDataStr.at(j).unicode() << 8) | writeDataStr.at(j - 1).unicode();
            }
        }
    }
    // HR001に転送モードを送信
    ret = mModbusCom->modbusWriteHR(PROG_FILE_TRANSER_MODE_HR, writeData, writeSize);
    if (!ret){
        qDebug() << "modbusWriteHR failed:" << ret;
    }
    return ret;
}

bool ProgramFileManager::modbusWriteProgTransEndMode()
{
    bool ret = true;
    int writeData[5] = { 0 };
    QString writeDataStr = "END";
    int writeSize = 5;

    // 転送モード
    writeData[0] = PROG_FILE_TRANSER_MODE;
    // 残り回数
    writeData[1] = 0x000;
    // 送信サイズ
    writeData[2] = 0x0003;
    // ファイルデータ
    for(int i = 0; i < writeDataStr.length(); i++){
        if (i == writeDataStr.length() - 1){
            writeData[2 + (i / 2 + i % 2)] = 0x0000 | writeDataStr.at(i).unicode();
        } else {
            if (i % 2) {
                writeData[2 + (i / 2 + i % 2)] = (writeDataStr.at(i).unicode() << 8) | writeDataStr.at(i - 1).unicode();
            }
        }
    }
    // HR001に転送モードを送信
    ret = mModbusCom->modbusWriteHR(PROG_FILE_TRANSER_MODE_HR, writeData, writeSize);
    if (!ret){
        qDebug() << "modbusWriteHR failed:" << ret;
    }
    return ret;
}

void ProgramFileManager::chgStrToAscii(QString fileName, int *writeData, int writeOffset)
{
    bool findDot = false;
    if (!writeData || writeOffset < 1){
        return;
    }
    for (int i = 0; i < fileName.length(); i++){
        if (fileName.at(i).unicode() == 0x2E){
            writeData[writeOffset + (i / 2 + i % 2)] = 0x0000;
            findDot = true;
        }else {
            if (findDot){
                if (i == fileName.length() - 1){
                    writeData[writeOffset + (i / 2 + i % 2)] = 0x0000 | fileName.at(i).unicode();
                } else {
                    if (!(i % 2)) {
                        writeData[writeOffset + (i / 2 + i % 2)] = (fileName.at(i).unicode() << 8) | fileName.at(i - 1).unicode();
                    }
                }
            } else {
                if (i % 2) {
                    writeData[(writeOffset - 1) + (i / 2 + i % 2)] = (fileName.at(i).unicode() << 8) | fileName.at(i - 1).unicode();
                }
            }
        }
    }
}

void ProgramFileManager::chgStrToAscii(QString fileName, int *writeData)
{
    bool findDot = false;
    if (!writeData){
        return;
    }
    for (int i = 0; i < fileName.length(); i++){
        if (fileName.at(i).unicode() == 0x2E){
            writeData[(i / 2 + i % 2) - 1] = 0x0000;
            findDot = true;
        } else {
            if (findDot){
                if (i == fileName.length() - 1){
                    writeData[(i / 2 + i % 2) - 1] = 0x0000 | fileName.at(i).unicode();
                } else {
                    if (!(i % 2)) {
                        writeData[(i / 2 + i % 2) - 1] = (fileName.at(i).unicode() << 8) | fileName.at(i - 1).unicode();
                    }
                }
            } else {
                if (i % 2) {
                    writeData[(i / 2 + i % 2) - 1] = (fileName.at(i).unicode() << 8) | fileName.at(i - 1).unicode();
                }
            }
        }
    }
}

int ProgramFileManager::getFileListIndex(QString fileName)
{
    int retIndex = -1;
    int fileCount = 0;
    fileCount = mFileInfoList.count();
    for(int i = 0; i < fileCount; i++){
        if (mFileInfoList.at(i).fileName() == fileName){
            retIndex = i;
            break;
        }
    }
    return retIndex;   
}

bool ProgramFileManager::deleteProgramFile(QString *fileName)
{
    bool ret = false;

    if (!fileName){
        return ret;
    }
    if (!mModbusCom){
        qDebug() << "ProgramFileManager:mModbusCom is not share";
        return false;
    }
    // check modbus status
    if (mModbusCom->getModbusConnectStatus() != QModbusDevice::ConnectedState){
        mModbusCom->modbusCommunicationStart();
        qDebug() << "sync modbus is not connected";
        return false;
    }
    // HR001に転送モードを送信
    ret = modbusWriteProgTranferMode(PROG_FILE_TRANSER_DELETE_MODE);
    if (!ret){
        qDebug() << "modbusWriteProgTranferMode failed:" << ret;
        return ret;
    }
    // write prog file Name
    ret = modbusWriteProgFileName(fileName);
    if (!ret){
        qDebug() << "modbusWriteProgFileName failed:" << ret;
        return ret;
    }
    return ret;
}

bool ProgramFileManager::modbusWriteProgTranferMode(int transMode)
{
    bool ret = false;

    if (!mModbusCom){
        qDebug("%s:%d mModbusCom is not share", __func__, __LINE__);
        return false;
    }
    mModbusCom->modbusWriteHR(PROG_FILE_TRANSER_MODE_HR, transMode);
    if (!ret){
        qDebug() << "modbusWriteHR failed:" << ret;
        return ret;
    }
    return ret;
}

bool ProgramFileManager::modbusWriteProgFileName(QString *fileName)
{
    bool ret = false;
    int writeData[6] = { 0 };
    int writeSize = 0;

    if (!fileName){
        return false;
    }
    writeSize = (fileName->length() / 2) + 3;
    // MODBUS転送データ作成
    chgStrToAscii(*fileName, writeData, 1);
    ret = mModbusCom->modbusWriteHR(PROG_FILE_TRANSER_DATA_HR, writeData, writeSize);
    if (!ret){
        qDebug() << "modbusWriteHR failed:" << ret;
    }
    return ret;
}

QList<QString> ProgramFileManager::getOpeProgramFileName()
{
    bool ret = false;
    stModBusCommunicationData readCommand;

    mProgFileNameList.clear();
    if (!mModbusCom){
        qDebug() << "ProgramFileManager:mModbusCom is not share";
        return mProgFileNameList;
    }
    // check modbus status
    if (mModbusCom->getModbusConnectStatus() != QModbusDevice::ConnectedState){
        mModbusCom->modbusCommunicationStart();
        qDebug() << "sync modbus is not connected";
        return mProgFileNameList;
    }
    // get progfile name by modbus
    ret = mModbusCom->modbusReadHR(PROG_FILE_NAME_HR, PROG_FILE_NAME_SIZE, &readCommand);
    if (!ret){
        qDebug() << "modbusReadHR failed:" << ret;
        return mProgFileNameList;
    }
    // change to Str from Ascii
    for(int i = 0; i < PROG_FILE_NAME_SIZE / PROG_ONE_FILE_NAME_SIZE_MAX; i++){
        mProgFileNameList.append(
            chgAsciiToStr(PROG_ONE_FILE_NAME_SIZE_MAX, &readCommand.readBuf[i * PROG_ONE_FILE_NAME_SIZE_MAX])
        );
    }
    //qDebug() << "mProgFileNameList count:" << mProgFileNameList.count();
    return mProgFileNameList;
}

void ProgramFileManager::startModbus()
{
    if(!mModbusCom){
        return;
    }
    mModbusCom->modbusCommunicationStart();
}

void ProgramFileManager::stopModbus()
{
    if(!mModbusCom){
        return;
    }
    mModbusCom->modbusCommunicationStop();
}

QString ProgramFileManager::chgAsciiToStr(ushort readSize, quint16 *readBuf)
{
    QString result = "";
    QChar str[2];
    if(!readBuf || readSize <= 0){
        return result;
    }
    for(int i = 0; i < readSize; i++){
        str[0] = (QChar)(readBuf[i] & 0x00FF);
        str[1] = (QChar)((readBuf[i] & 0xFF00) >> 8);
        result.append(str[0].toLatin1());
        result.append(str[1].toLatin1());
    }
    return result;
}

void ProgramFileManager::modbusReadProgramList() {
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadInputRegisters, 0x00A0, 0x0020);
    if(m_communication != nullptr) {
        m_communication->setOneShotModbusCaller(CommunicationThread::OneShotReadModBusCaller::CallerProgFileMng);
        m_communication->setOneShotReadCommand(command);
    }
}

QList<QObject *> ProgramFileManager::getOperationProgramData(int taskNo) {
    return m_programDataList.value(taskNo);
}

QString ProgramFileManager::getProgramName(int taskNo) {
    return m_programNameList.at(taskNo);
}

bool ProgramFileManager::registerOpeProgramName(int progNo, QString progName)
{
    bool ret = false;
    int writeData[8] = { 0 };
    int writeSize = (progName.length() / 2) + 3;

    if (!mModbusCom || progNo < 1 || progNo > PROG_FILE_REGISTER_NUM_MAX){
        return false;
    }
    stProgFileModbusInfo modbusInfo = progModbusInfo[progNo - 1];
    chgStrToAscii(progName, writeData);
    // mainプログラム名を登録
    ret = mModbusCom->modbusWriteHR(modbusInfo.hrNumber, writeData, writeSize);
    if (!ret){
        qDebug() << "modbusWriteHR failed:" << ret;
    }
    return ret;
}

bool ProgramFileManager::executeCompile()
{
    bool ret = false;

    if (!mModbusCom){
        return false;
    }
    // write DO032==1
    ret = mModbusCom->modbusWriteDO(PROG_FILE_EXE_COMPILE1_DO, true);
    if (!ret){
        qDebug() << "modbusWriteDO failed:" << ret;
        return ret;
    }
    // write DO025==0
    ret = mModbusCom->modbusWriteDO(PROG_FILE_EXE_COMPILE2_DO, false);
    if (!ret){
        qDebug() << "modbusWriteDO failed:" << ret;
        return ret;
    }
    return ret;
}

bool ProgramFileManager::monitorCompileResult()
{
    bool ret = true;
    if (!m_communication){
        return false;
    }
    timerStart(25);
    connect(m_communication, &CommunicationThread::feed, this, &ProgramFileManager::updatePeriodResult, Qt::QueuedConnection);
    return ret;
}

bool ProgramFileManager::updateOpeProgramFile(QString progName)
{
    bool ret = false;
    int writeData[8] = { 0 };
    int writeSize = (progName.length() / 2) + 3;

    if (!mModbusCom){
        return false;
    }
    chgStrToAscii(progName, writeData);
    // 動作プログラム1ファイル名(HR073～HR076）にmainプログラム名を登録
    ret = mModbusCom->modbusWriteHR(PROG_FILE_NAME_HR, writeData, writeSize);
    if (!ret){
        qDebug() << "modbusWriteHR failed:" << ret;
        return ret;
    }
    // write DO032==0
    ret = mModbusCom->modbusWriteDO(PROG_FILE_UPDATE1_DO, false);
    if (!ret){
        qDebug() << "modbusWriteDO failed:" << ret;
        return ret;
    }
    // write DO025==1
    ret = mModbusCom->modbusWriteDO(PROG_FILE_UPDATE2_DO, true);
    if (!ret){
        qDebug() << "modbusWriteDO failed:" << ret;
        return ret;
    }
    return ret;
}

void ProgramFileManager::readProgramName()
{
    if (!m_modbusManager){
        return;
    }
    for(int i = 0; i < m_programNameList.size(); i++) {
        QByteArray byteArray;
        int data1 = m_modbusManager->getReceiveDataRG((161+(i*4)) + 0);
        int data2 = m_modbusManager->getReceiveDataRG((161+(i*4)) + 1);
        int data3 = m_modbusManager->getReceiveDataRG((161+(i*4)) + 2);
        int data4 = m_modbusManager->getReceiveDataRG((161+(i*4)) + 3);
        byteArray.append((data1 & 0x00FF));
        byteArray.append((data1 & 0xFF00) >> 8);
        byteArray.append((data2 & 0x00FF));
        byteArray.append((data2 & 0xFF00) >> 8);
        byteArray.append((data3 & 0x00FF));
        byteArray.append((data3 & 0xFF00) >> 8);
        byteArray.append((data4 & 0x00FF));
        byteArray.append((data4 & 0xFF00) >> 8);
        QString programName = QString::fromLocal8Bit(byteArray);
        if(programName.isEmpty()) {
            // NOP
        } else {
            programName.append(".prg");
            m_programNameList.replace(i, programName);
        }
    }
}

void ProgramFileManager::loadMatchedProgramData(QStringList matchingKeyList)
{
    QList<QFileInfo> progFileList = getProgramFileList();

    for(int i = 0; i < matchingKeyList.size(); i++) {
        QString programName = matchingKeyList.at(i);

        for(int progFileIndex = 0; progFileIndex < progFileList.size(); progFileIndex++) {
            QFileInfo fileInfo = progFileList.at(progFileIndex);
            if(programName == fileInfo.fileName()) {
                QFile progFile(fileInfo.filePath());
                if (! progFile.open(QIODevice::ReadOnly)) {
                    QString errStr = "ファイル(" + fileInfo.suffix() + ")オープンエラー:" + progFile.errorString();
                    qDebug() << errStr;
                    break;
                }
                QTextStream in(&progFile);

                int lineNo = 1;
                QList<QObject *> programDataList;
                while (! in.atEnd()) {
                    OperationProgramLineData *lineData = new OperationProgramLineData();
                    lineData->setLine(lineNo);
                    lineData->setData(in.readLine());
                    programDataList.append(lineData);
                    lineNo++;
                }
                m_programDataList[i] = programDataList;
                progFile.close();
                break;
            }
        }
    }
}

void ProgramFileManager::getCompileErrInfoAndNotify()
{
    bool ret = true;
    stModBusCommunicationData readCommand;

    if (!mModbusCom){
        qCritical() << "mModbusCom is Null";
        emit compileResultChanged(false);
    }
    ret = mModbusCom->modbusReadRG(PROG_FILE_NAME_START_RG,
        (PROG_FILE_NAME_SIZE_MAX_RG + PROG_FILE_ERR_INFO_SIZE_MAX_RG), &readCommand);
    if (!ret){
        qCritical() << "modbusReadRG failed:" << ret;
        emit compileResultChanged(false);
    } else {
        progCompileErrInfo.errProgName =
            chgAsciiToStr(PROG_FILE_NAME_SIZE_MAX_RG, &readCommand.readBuf[0]);
        progCompileErrInfo.errNumber =
            readCommand.readBuf[PROG_FILE_NAME_SIZE_MAX_RG];
        progCompileErrInfo.errLineNum =
            readCommand.readBuf[PROG_FILE_NAME_SIZE_MAX_RG + 1];
        emit compileResultChanged(false);
    }
    if (m_communication) {
        disconnect(m_communication, &CommunicationThread::feed, this,
            &ProgramFileManager::updatePeriodResult);
    }
    mMonitorCnt = 0;
    return;
}

void ProgramFileManager::parseCompileResultAndNotify()
{
    bool stateDI014 = false, stateDI015 = false, stateDI016 = false;
    bool isTimeOut = false, isCompileResult = false;

    if (!m_modbusManager){
        isCompileResult = false;
        goto abnormal_notify;
    }
    stateDI014 = m_modbusManager->getReceiveDataDI(14);
    stateDI015 = m_modbusManager->getReceiveDataDI(15);
    stateDI016 = m_modbusManager->getReceiveDataDI(16);
    if (mMonitorCnt >= mMonitorTimeOutCnt){
        // timeOut notify
        isTimeOut = true;
        isCompileResult = false;
        goto abnormal_notify;
    } else {
        if (stateDI014){
            if (stateDI016){    // コンパイル完了(正常)
                isCompileResult = true;
                goto success_notify;
            }
            if (stateDI015){    // コンパイルエラー
                getCompileErrInfoAndNotify();
            }
            if (!stateDI016 && !stateDI015){
                mMonitorCnt++;
            }
        } else {
            mMonitorCnt++;
        }
    }
    return;
abnormal_notify:
    if (isTimeOut){
        qCritical("mMonitorCnt[%d] stateDI014[%d] stateDI015[%d] stateDI016[%d]",
            mMonitorCnt, stateDI014, stateDI015, stateDI016);
    }
success_notify:
    if (m_communication) {
        disconnect(m_communication, &CommunicationThread::feed, this,
            &ProgramFileManager::updatePeriodResult);
    }
    timerStop();
    emit compileResultChanged(isCompileResult);
    return;
}

void ProgramFileManager::onOneShotReadFinished(stModBusCommunicationData command)
{
    if((command.funcCode == QModbusPdu::FunctionCode::ReadInputRegisters)
            && (command.readAddress == 0x00A0)
            && (command.comSize == 0x0020)) {
        // read program name (RG161~RG192)
        readProgramName();
        loadMatchedProgramData(m_programNameList);
    }
    return;
}

void ProgramFileManager::updatePeriodResult(stModBusCommunicationData command)
{
    //qDebug("mMonitorCnt[%d] funcCode[0x%02x]", mMonitorCnt, command.funcCode);
    switch(command.funcCode) {
    case QModbusPdu::FunctionCode::ReadCoils:
        // NOP
        break;
    case QModbusPdu::FunctionCode::ReadDiscreteInputs:
        parseCompileResultAndNotify();
        break;
    default:
        // NOP
        break;
    }
    return;
}

void ProgramFileManager::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent){
        qCritical("timerEvent is Null");
        return;
    }
    if(timerEvent->timerId() == mTimerId){
        mMonitorCnt++;
        checkStatusCode();
    }
}

void ProgramFileManager::initCtrlStatusCode()
{
    mCurCtrlCode = -1;
    mCurStatusCode = -1;
}

void ProgramFileManager::startModbusMonitoring()
{
    if (!m_communication) return;
    if (!mIsModbusMonitor ) {
        connect(m_communication, &CommunicationThread::feed, this, &ProgramFileManager::update, Qt::QueuedConnection);
        mIsModbusMonitor = true;
    }
}

void ProgramFileManager::stopModbusMonitoring()
{
    if (!m_communication) return;
    if (mIsModbusMonitor ) {
        disconnect(m_communication, &CommunicationThread::feed, this, &ProgramFileManager::update);
        mIsModbusMonitor = false;
    }
}

void ProgramFileManager::requestCompile()
{
    if(!m_communication) return;
    m_communication->writeModbusHR(PROG_EDIT_REQUEST_COMPILE_HR, (int)ControlCode::ProgFileCompile);
    modbusGetCurrentCode(ControlCode::ProgFileCompile);
}

void ProgramFileManager::getCompileErrorInfo()
{
    if(!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, PROG_EDIT_COMPILE_ERROR_STEP_SR, 0x0001
        );
    m_communication->setOneShotModbusCommand(readCommand);
    startModbusMonitoring();
}

void ProgramFileManager::modbusGetCurrentCode(ProgramFileManager::ControlCode ctrlCode, bool isFirst)
{
    mExpectedStateCode = (int)ctrlCode;
    modbusReadCurrentCode();
    if (isFirst) {
        mCmdInternalStatus = (int)InternalStatus::CurCmdConfirmBefore;
        //QThread::msleep(25);                    // sleep 25ms
        timerStart(100);
    }
    startModbusMonitoring();
}

void ProgramFileManager::modbusGetStatusCode(ProgramFileManager::ControlCode ctrlCode, bool isFirst)
{
    if (!m_communication) return;
    mExpectedStateCode = (int)ctrlCode;
    modbusReadStatusCode();
    if (isFirst) {
        mCmdInternalStatus = (int)InternalStatus::CurCmdConfirmBefore;
        //QThread::msleep(25);                    // sleep 25ms
        timerStart(100);
    }
    startModbusMonitoring();
}

void ProgramFileManager::modbusReadCurrentCode()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, PROG_EDIT_CONTROL_COMMAND_SR, 0x0001
        );
    m_communication->setOneShotModbusCommand(readCommand);
}

void ProgramFileManager::modbusReadStatusCode()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, PROG_EDIT_EXECUTION_STATE_SR, 0x0001
        );
    m_communication->setOneShotModbusCommand(readCommand);
}

void ProgramFileManager::modbusReadStatusCurrentCode()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, PROG_EDIT_CONTROL_COMMAND_SR, 0x0002
        );
    m_communication->setOneShotModbusCommand(readCommand);
}

void ProgramFileManager::checkCompileOpeStatusCode()
{
    if (!m_modbusManager) {
        initCtrlStatusCode();
        stopModbusMonitoring();
        timerStop();
        return;
    }
    switch((ProgramFileManager::InternalStatus)mCmdInternalStatus){
    case ProgramFileManager::InternalStatus::CurCmdConfirmBefore:
        if (mCurCtrlCode == mExpectedStateCode){
            if (mCurStatusCode == (int)ProgramFileManager::StatusCode::CmdWait){
                mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExeStart;
                modbusGetStatusCode(ProgramFileManager::ControlCode::ProgFileCompile, false);
            } else if (mCurStatusCode == (int)ProgramFileManager::StatusCode::CmdExe){
                mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExecuted;
                modbusGetStatusCode(ProgramFileManager::ControlCode::ProgFileCompile, false);
            } else if (mCurStatusCode == (int)ProgramFileManager::StatusCode::CmdCompileSuccess){
                mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExeSuccess;
                goto compileSuccess;
            } else {
                if (mCurStatusCode >= (int)ProgramFileManager::StatusCode::CmdAbnormal1
                    && mCurStatusCode <= (int)ProgramFileManager::StatusCode::CmdNotSupported){
                    mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExeError;
                    qDebug("%s:%d mCmdInternalStatus[%d] mCurStatusCode[%d]", __func__, __LINE__, mCmdInternalStatus, mCurStatusCode);
                    emit compileStatusCodeChanged(mCurStatusCode);
                    initCtrlStatusCode();
                    stopModbusMonitoring();
                    timerStop();
                    m_compileResult = false;
                    emit compileResultChanged(m_compileResult);
                } else {
                    modbusGetStatusCode(ProgramFileManager::ControlCode::ProgFileCompile, false);
                }
            }
        } else {
            modbusGetCurrentCode(ProgramFileManager::ControlCode::ProgFileCompile, false);
        }
        break;
    case ProgramFileManager::InternalStatus::CmdExeStart:
        if (mCurStatusCode == (int)ProgramFileManager::StatusCode::CmdWait){
            mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExeStart;
            modbusGetStatusCode(ProgramFileManager::ControlCode::ProgFileCompile, false);
        } else if (mCurStatusCode == (int)ProgramFileManager::StatusCode::CmdExe){
            mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExecuted;
            modbusGetStatusCode(ProgramFileManager::ControlCode::ProgFileCompile, false);
        } else if (mCurStatusCode == (int)ProgramFileManager::StatusCode::CmdCompileSuccess){
            mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExeSuccess;
            goto compileSuccess;
        } else {
            if (mCurStatusCode >= (int)ProgramFileManager::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)ProgramFileManager::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExeError;
                qDebug("%s:%d mCmdInternalStatus[%d] mCurStatusCode[%d]", __func__, __LINE__, mCmdInternalStatus, mCurStatusCode);
                emit compileStatusCodeChanged(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                m_compileResult = false;
                emit compileResultChanged(m_compileResult);
            } else {
                modbusGetStatusCode(ProgramFileManager::ControlCode::ProgFileCompile, false);
            }
        }
        break;
    case ProgramFileManager::InternalStatus::CmdExecuted:
        if (mCurStatusCode == (int)ProgramFileManager::StatusCode::CmdCompileSuccess){
            mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExeSuccess;
            goto compileSuccess;
        } else {
            if (mCurStatusCode >= (int)ProgramFileManager::StatusCode::CmdAbnormal1
                && mCurStatusCode <= (int)ProgramFileManager::StatusCode::CmdNotSupported){
                mCmdInternalStatus = (int)ProgramFileManager::InternalStatus::CmdExeError;
                qDebug("%s:%d mCmdInternalStatus[%d] mCurStatusCode[%d]", __func__, __LINE__, mCmdInternalStatus, mCurStatusCode);
                emit compileStatusCodeChanged(mCurStatusCode);
                initCtrlStatusCode();
                stopModbusMonitoring();
                timerStop();
                m_compileResult = false;
                emit compileResultChanged(m_compileResult);
            } else {
                modbusGetStatusCode(ProgramFileManager::ControlCode::ProgFileCompile, false);
            }
        }
        break;
    case ProgramFileManager::InternalStatus::CmdExeSuccess:
        goto compileSuccess;
    default:
        modbusReadStatusCurrentCode();
        break;
    }
    return;
compileSuccess:
    initCtrlStatusCode();
    stopModbusMonitoring();
    qDebug("%s:%d mMonitorCnt[%d]", __func__, __LINE__, mMonitorCnt);
    timerStop();
    m_compileResult = true;
    emit compileResultChanged(m_compileResult);
}

void ProgramFileManager::checkStatusCode()
{
    if (mMonitorCnt >= PROG_EDIT_COMPILE_TIMEOUT_COUNT){
        qDebug("%s:%d mMonitorCnt[%d]", __func__, __LINE__, mMonitorCnt);
        initCtrlStatusCode();
        stopModbusMonitoring();
        timerStop();
        m_processingTimeout = true;
        emit processingTimeoutChanged(m_processingTimeout);
        return;
    }
    switch((ProgramFileManager::ControlCode)mExpectedStateCode){
    case ProgramFileManager::ControlCode::ProgFileCompile:
        checkCompileOpeStatusCode();
        break;
    default:
        initCtrlStatusCode();
        stopModbusMonitoring();
        timerStop();
        break;
    }
}


void ProgramFileManager::update(stModBusCommunicationData command)
{
    //qDebug("%s:%d readAddress[%d]", __func__, __LINE__, command.readAddress);
    if (command.readAddress == PROG_EDIT_CONTROL_COMMAND_SR && command.comSize == 1){
        mCurCtrlCode = m_modbusManager->getReceiveDataRG(PROG_EDIT_CONTROL_COMMAND_SR);
    } else if (command.readAddress == PROG_EDIT_EXECUTION_STATE_SR && command.comSize == 1){
        mCurStatusCode = m_modbusManager->getReceiveDataRG(PROG_EDIT_EXECUTION_STATE_SR);
        //qDebug("%s:%d readAddress[SR%d] funcCode[%d] comSize[%d] readBuf[%d] mCurStatusCode[%d]",
        //    __func__, __LINE__, command.readAddress, command.funcCode, command.comSize, command.readBuf[0], mCurStatusCode);
    } else if (command.readAddress == PROG_EDIT_CONTROL_COMMAND_SR && command.comSize == 2){
        //qDebug("%s:%d readAddress[SR%d] funcCode[%d] comSize[%d] readBuf[%d][%d]",
        //    __func__, __LINE__, command.readAddress, command.funcCode, command.comSize, command.readBuf[0], command.readBuf[1]);
        mCurCtrlCode = m_modbusManager->getReceiveDataRG(PROG_EDIT_CONTROL_COMMAND_SR);
        mCurStatusCode = m_modbusManager->getReceiveDataRG(PROG_EDIT_EXECUTION_STATE_SR);
    } else if (command.readAddress == PROG_EDIT_COMPILE_ERROR_STEP_SR && command.comSize == 1) {
        qDebug("%s:%d readAddress[SR%d] funcCode[%d] comSize[%d] readBuf[%d]",
            __func__, __LINE__, command.readAddress, command.funcCode, command.comSize, command.readBuf[0]);
        emit compileErrorStepChanged((int)command.readBuf[0]);
        stopModbusMonitoring();
    }
}

ProgramFileManager* ProgramFileManager::instance = nullptr;
