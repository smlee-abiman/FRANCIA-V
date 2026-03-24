#include "teachingprogrampagemodel.h"
#include "teachingview/programview/ncparbitrarypoint.h"
#include "teachingviewdef.h"

#include "programview/ncpstep.h"    // NCPStep.
#include "programview/ncpcommand.h" // NCPCommand.
#include "programview/ncpcommandstring.h"
#include "axispointinfo.h"
#include "programview/teachingprogrampagepointinfo.h"
#include <QProcess>
#define RUNNING_PROGRAM_SR      (2785)
#define AUTO_DRIVE_ONE_CYCLE_DI (2669)
#define AUTO_DRIVE_STEP_DI      (2613)
#define AUTO_DRIVE_STEP_PREV_DI (2622)
#define AUTO_DRIVE_STEP_NEXT_DI (2623)

//------------------------------------------------------
// AsyncLoadNcpFileTask.
//------------------------------------------------------
AsyncLoadNcpFileTask::AsyncLoadNcpFileTask(ResourceManager *resource)
    : m_resource(resource)
{
}

AsyncLoadNcpFileTask::~AsyncLoadNcpFileTask()
{
    qDeleteAll(m_ncpFileQueue);
    m_ncpFileQueue.clear();
}

NCPFile* AsyncLoadNcpFileTask::popNcpFile()
{
    QMutexLocker locker(&m_ncpFileQueueMutex);
    return m_ncpFileQueue.dequeue();
}

void AsyncLoadNcpFileTask::pushNcpFile(NCPFile* ncpFile)
{
    QMutexLocker locker(&m_ncpFileQueueMutex);
    m_ncpFileQueue.enqueue(ncpFile);
}

void AsyncLoadNcpFileTask::run()
{
    auto ncpFile = new NCPFile(m_resource);
#ifdef REAL_TARGET
    ncpFile->load(AppEnvironment::getFtpFolderPath() + "/NCSTEP.NCP");
#else // REAL_TARGE m_ncpfilename
    ncpFile->load(AppEnvironment::getNcpFolderPath() + m_ncpfilename);    // for debug.

//    ncpFile->load(AppEnvironment::getNcpFolderPath() + "/DIR.NCP");    // for debug.
#endif // REAL_TARGET
    pushNcpFile(ncpFile);
    emit resultReady();
}

void AsyncLoadNcpFileTask::setncpFileName(QString name)
{
    m_ncpfilename = name;
}

QString AsyncLoadNcpFileTask::getncpFileName()
{
    return m_ncpfilename;
}

//------------------------------------------------------
// TeachingProgramPageModel.
//------------------------------------------------------
TeachingProgramPageModel::TeachingProgramPageModel(ResourceManager *resource, CommunicationThread * comm, ExternalMemoryViewModel *extMemView)
    : PageModelBase(resource, comm)
    , mSelectedProgramNo(-1)
    , mRunningStepNo(-1)
    , mRunningProgramNos({0, 0, 0, 0, 0, 0, 0, 0})
    , mRunningProgramSteps({0, 0, 0, 0, 0, 0, 0, 0})
    , m_extMemView(extMemView)
    , mTempNcpStep(nullptr)
{
    init();
}
//------------------------------------------------------
TeachingProgramPageModel::TeachingProgramPageModel(QObject *object)
    : TeachingProgramPageModel(nullptr, nullptr, nullptr)
{
    Q_UNUSED(object)
}

TeachingProgramPageModel::~TeachingProgramPageModel()
{
    for (int i = 0; i < m_menuModel.length() - 1; i++) {
        auto menu = static_cast<ProgramMenu *>(m_menuModel.at(i));
        disconnect(menu, &ProgramMenu::ncpCommandDataChanged, this, &TeachingProgramPageModel::onNcpCommandDataChanged);
    }
    qDeleteAll(m_menuModel);
    clearTempNcpStep();

    disconnect(m_asyncLoadNcpFileTask, &AsyncLoadNcpFileTask::resultReady, this, &TeachingProgramPageModel::onAsyncTaskFinished);
    if (0 < m_threadPool.activeThreadCount()) {
        m_threadPool.clear();
        m_threadPool.waitForDone();
    }
    delete m_asyncLoadNcpFileTask;
}
//------------------------------------------------------
void TeachingProgramPageModel::setSelectedProgramNo(int selectedProgramNo) {
    // qDebug() << "called. selectedProgramNo:" << selectedProgramNo;
    if (mSelectedProgramNo == selectedProgramNo) { return; }
    mSelectedProgramNo = selectedProgramNo;
    emit selectedProgramNoChanged(mSelectedProgramNo);
}
//------------------------------------------------------
void TeachingProgramPageModel::setRunningStepNo(int runningStepNo) {
    if (mRunningStepNo == runningStepNo) { return; }
    // qDebug() << "called. mRunningStepNo:" << mRunningStepNo << ", runningStepNo:" << runningStepNo;
    mRunningStepNo = runningStepNo;
    emit runningStepNoChanged(mRunningStepNo);
}
//------------------------------------------------------
void TeachingProgramPageModel::setAutoDriveOneCycle(bool autoDriveOneCycle) {
    if (mAutoDriveOneCycle == autoDriveOneCycle) { return; }
    mAutoDriveOneCycle = autoDriveOneCycle;
    emit autoDriveOneCycleChanged(mAutoDriveOneCycle);
}
//------------------------------------------------------
void TeachingProgramPageModel::setAutoDriveStep(bool autoDriveStep) {
    if (mAutoDriveStep == autoDriveStep) { return; }
    mAutoDriveStep = autoDriveStep;
    emit autoDriveStepChanged(mAutoDriveStep);
}
//------------------------------------------------------
void TeachingProgramPageModel::setAutoDriveStepNext(bool autoDriveStepNext) {
    if (mAutoDriveStepNext == autoDriveStepNext) { return; }
    mAutoDriveStepNext = autoDriveStepNext;
    emit autoDriveStepNextChanged(mAutoDriveStepNext);
}
//------------------------------------------------------
void TeachingProgramPageModel::setAutoDriveStepPrev(bool autoDriveStepPrev) {
    if (mAutoDriveStepPrev == autoDriveStepPrev) { return; }
    mAutoDriveStepPrev = autoDriveStepPrev;
    emit autoDriveStepPrevChanged(mAutoDriveStepPrev);
}
//------------------------------------------------------
void TeachingProgramPageModel::clearTempNcpStep() {
    if (!mTempNcpStep) { return; }
    delete mTempNcpStep;
    mTempNcpStep = nullptr;
}
//------------------------------------------------------
void TeachingProgramPageModel::init()
{
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::ProgramPage));
    setPageLabelColor(m_resource->rgb(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::ProgramPage));
    setActivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_PROGRAM_PAGE_ACTIVATE_ICON_NAME);
    setDeactivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_PROGRAM_PAGE_DEACTIVATE_ICON_NAME);
    // Detect language changes to update labels.
    connect(m_resource, &ResourceManager::languageChanged, this, &TeachingProgramPageModel::onLanguageChanged, Qt::QueuedConnection);

    readPenSetData();

    // todo.
    mNcpFile = new NCPFile(m_resource);
    m_pointInfo = new TeachingProgramPagePointInfo(this, m_communication, this);
    // async task.
    m_threadPool.setMaxThreadCount(1);
    m_asyncLoadNcpFileTask = new AsyncLoadNcpFileTask(m_resource);
    m_asyncLoadNcpFileTask->setAutoDelete(false);
    connect(m_asyncLoadNcpFileTask, &AsyncLoadNcpFileTask::resultReady, this, &TeachingProgramPageModel::onAsyncTaskFinished);

    m_menuModel.append(new ProgramMenu1(m_resource,m_communication));
    m_menuModel.append(new ProgramMenu2(m_resource,m_communication, mNcpFile, m_axisInfo));
    m_menuModel.append(new ProgramMenu3(m_resource,m_communication));
    m_menuModel.append(new ProgramMenu4(m_resource,m_communication));
    m_menuModel.append(new ProgramMenu5(m_resource,m_communication));
    m_menuModel.append(new NcpOperate());

    for (int i = 0; i < m_menuModel.length() - 1; i++) {
        auto menuModel = static_cast<ProgramMenu *>(m_menuModel.at(i));
        connect(menuModel, &ProgramMenu::ncpCommandDataChanged, this, &TeachingProgramPageModel::onNcpCommandDataChanged, Qt::QueuedConnection);
    }

#ifdef REAL_TARGET
    //mNcpFile->load(AppEnvironment::getFtpFolderPath() + "/NCSTEP.NCP");
    // async get by onFtpFileInfoChanged....
#else // REAL_TARGET
    // mNcpFile->load(AppEnvironment::getNcpFolderPath() + "/DIR.NCP");    // for debug.
    startAsyncNcpFileLoad();    // for debug.
#endif // REAL_TARGET
    // backupNcpFileInstance();

    mFtpManager = FtpManager::get_instance();
#ifdef REAL_TARGET
    if (mFtpManager) {
        connect(mFtpManager, SIGNAL(ftpFileInfoChanged(int)), this, SLOT(onFtpFileInfoChanged(int)), Qt::QueuedConnection);
    }
#endif
    mProgFileManager = ProgramFileManager::get_instance();
    m_axisPointInfo = AxisPointInfo::get_instance();

    m_commandString = new NCPCommandString(this, m_resource, this);
    if(m_extMemView) {
        connect(m_extMemView, SIGNAL(isNcpFileUpdateChanged(bool)), this, SLOT(onIsNcpFileUpdateChanged(bool)), Qt::QueuedConnection);
    }

    if (mFtpManager) {
        int ftpTimeOut = mFtpManager->getFtpTimeOut();
        if(ftpTimeOut <= 0){
            mFtpTimeOutCnt = 0;
        } else {
            mFtpTimeOutCnt = ftpTimeOut * (1000 / 25);        // ftpTimeOut*40(by 25msインターバル)
        }
    }
    //qDebug("%s:%d mFtpTimeOutCnt[%d]", __func__, __LINE__, mFtpTimeOutCnt);
}
//------------------------------------------------------
int TeachingProgramPageModel::saveNCPFile()
{
    //qDebug("%s:%d", __func__, __LINE__);
    int result = 0;
    // NCPFile save
#ifdef REAL_TARGET
    QString NCPFileName = "NCSTEP.NCP";
    QString saveFilePath = AppEnvironment::getNcpFolderPath() + "/" + NCPFileName;
#else
    QString NCPFileName = m_asyncLoadNcpFileTask->getncpFileName();
    QString saveFilePath = AppEnvironment::getNcpFolderPath() + NCPFileName;
#endif

    mNcpFile->save(saveFilePath);
    mNcpFile->setEdited(false);
    // backupNcpFileInstance();
    // FTP転送(STOR)
#ifdef REAL_TARGET
    result = ftpTransfer(NCPFileName);
#endif
    return result;
}

void TeachingProgramPageModel::saveNCPFile1(QString filepath)
{
    QString saveFilePath =  filepath.midRef(8).toString();
    QFile file(saveFilePath);
    if(!file.exists()){
        file.open(QFile::WriteOnly|QFile::Text|QIODevice::Append);
    }
    mNcpFile->save(saveFilePath);
    mNcpFile->setEdited(false);
}
//------------------------------------------------------
void TeachingProgramPageModel::demoComplieNcp()
{
//QString saveFilePath = AppEnvironment::getNcpFolderPath() + "/" + ncpFileName;
    QString editedFile = AppEnvironment::getNcpFolderPath() + "/" + m_editNcp;
    QString copyFilePath = AppEnvironment::getNcpFolderPath() + "/" + "ncstep1.ncp";
    QString compFilePath = AppEnvironment::getNcpFolderPath() + "/" + "comp.rst";

    if(QFile::exists(copyFilePath)){
        QFile::remove(copyFilePath);
    }
    if(QFile::exists(compFilePath)){
        QFile::remove(compFilePath);
    }
    QFile::copy(editedFile, copyFilePath);
    m_resource->changeApplication(AppEnvironment::getExeFolderPath() + "/PenW_NCP0.exe", "--fromAppName pendant-gui.exe");
    timerStart(500,1);

}//调用7英寸exe编译ncp
//------------------------------------------------------
void TeachingProgramPageModel::checkDemoComplieNcpResult()
{
    QString compFilePath = AppEnvironment::getNcpFolderPath() + "/" + "comp.rst";
    QString result;
    if(QFile::exists(compFilePath)){
        timerStop(1);
        QFile compFile(compFilePath);
        if(compFile.open(QIODevice::ReadOnly)){
            QTextStream stream(&compFile);
            while(!stream.atEnd()){
                result = stream.readLine();
            }
            QStringList zz = result.split(",");
            parseDemoComplieNcpResult(zz.at(0).toInt(),zz.at(1).toInt(),zz.at(2).toInt());
        }
    }
}
//------------------------------------------------------
void TeachingProgramPageModel::parseDemoComplieNcpResult(int rst1,int rst2, int rst3)
{
    if(rst1 == 1){
        emit demoCompileSuccess();
    }else if (rst1 == 2) {
        QList<int> zz = {rst2, rst3};
        emit demoCompileFailed(zz);
    }
}
//------------------------------------------------------
void TeachingProgramPageModel::updateNcpStepList()
{
    m_ncpStepListUpdate = !m_ncpStepListUpdate;
    emit ncpStepListUpdateChanged();
}
//------------------------------------------------------
void TeachingProgramPageModel::backupNcpFileInstance()
{
    if (mNcpFileBackup) {
        delete mNcpFileBackup;
    }
    mNcpFileBackup = mNcpFile->clone();
}
//------------------------------------------------------
void TeachingProgramPageModel::restoreNcpFileInstance()
{
    auto tempNcpFile = mNcpFile;
    mNcpFile = mNcpFileBackup->clone();
    emit ncpFileChanged(mNcpFile);
    if (tempNcpFile) {
        delete tempNcpFile;
    }
}
//------------------------------------------------------
void TeachingProgramPageModel::readPenSetData()
{
    //todo.
}
//------------------------------------------------------
void TeachingProgramPageModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::ProgramPage));
}
//------------------------------------------------------
void TeachingProgramPageModel::onFinished()
{
//qDebug() << "called.";
    if (m_pageChanging == false && m_modbus->getIsPageChanging() == false) {
        updateRunningProgramData();
        updateAutoDriveData();
        setOneCycle(m_modbus->getReceiveDataDI(2669));
        setCycle(m_modbus->getReceiveDataDI(2670));
        setStep(m_modbus->getReceiveDataDI(2613));
        setDataLoaded(true);
    }
}
//------------------------------------------------------
void TeachingProgramPageModel::onOneShotReadFinished(
        stModBusCommunicationData /*command*/,
        CommunicationThread::OneShotReadModBusCaller /*oneShotReadCaller*/
        )
{
    // nop.
}
//------------------------------------------------------
void TeachingProgramPageModel::onActivate()
{
//qDebug() << "called.";
    startModbusPeriodic();
    updateNcpStepList();
}
//------------------------------------------------------
void TeachingProgramPageModel::onDeactivate()
{
//qDebug() << "called.";
    clearTempNcpStep();
    stopModbusPeriodic();
}
//------------------------------------------------------
void TeachingProgramPageModel::startModbusPeriodic()
{
//qDebug() << "called.";
    if (!m_communication) return;
    connect(m_communication, &CommunicationThread::finish, this, &TeachingProgramPageModel::onFinished, Qt::QueuedConnection);
    connect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &TeachingProgramPageModel::endModbusPageWriteReady, Qt::DirectConnection);
}
//------------------------------------------------------
void TeachingProgramPageModel::stopModbusPeriodic()
{
//qDebug() << "called.";
    if (!m_communication) return;
    disconnect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &TeachingProgramPageModel::endModbusPageWriteReady);
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingProgramPageModel::onFinished);
}
//------------------------------------------------------
int TeachingProgramPageModel::ftpTransfer(QString ncpFileName)
{
    QString saveFilePath = AppEnvironment::getNcpFolderPath() + "/" + ncpFileName;
    // NCPFile get
    mPutFile = new QFile(saveFilePath);
    if (!mPutFile->exists()){
        qWarning() << QString("%1(%2): saveFilePath:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << saveFilePath;
        return -1;
    }
    // FTP転送中通知
    emit progSaveStatusChanged((int)ProgramSaveStatus::ProgFtpTransfer);
    //int ncpFileSize = (int)(mNcpFile->size(saveFilePath));
    // FTP転送(STOR)
    int result = mFtpManager->execFtpCommand(Enums::FtpCommand::FtpCommandSTOR, mPutFile, ncpFileName);
    qDebug("%s:%d FtpCommand::FtpCommandSTOR result[%d]", __func__, __LINE__, result);
    // FTP結果取得後コンパイル要求(非同期)
    stFtpPutArgument arg;
    arg.putFile = mPutFile;
    arg.selectedIndex = 0;  // TODO
    mFtpManager->setFtpPutArgment(result, arg);
    timerStart(25,0);
    connect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
    connect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
    return 0;
}
//------------------------------------------------------
void TeachingProgramPageModel::checkFtpProcess()
{
    if (mFtpStatus != (int)Enums::FtpStatus::FtpStatusPutFinished){
        qDebug("%s:%d ftp transfer timeout[%d] mFtpStatus[%d]", __func__, __LINE__, mTimerCnt, mFtpStatus);
        timerStop(0);
        disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
        disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
        mFtpStatus = (int)Enums::FtpStatus::FtpStatusLoggedIn;
        m_compileErrorStep = -1;
        emit compileErrorStepChanged(m_compileErrorStep);
        emit progSaveStatusChanged((int)ProgramSaveStatus::ProgTimeOut);
    } else {
        // PROG転送成功
        timerStop(0);
        disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
        disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
        mFtpStatus = (int)Enums::FtpStatus::FtpStatusLoggedIn;
        emit progSaveStatusChanged((int)ProgramSaveStatus::ProgCompiling);
        // コンパイル要求
        connect(mProgFileManager, SIGNAL(compileResultChanged(bool)), this, SLOT(onCompileResultChanged(bool)));
        connect(mProgFileManager, SIGNAL(processingTimeoutChanged(bool)), this, SLOT(onProcessingTimeoutChanged(bool)));
        connect(mProgFileManager, SIGNAL(compileStatusCodeChanged(int)), this, SLOT(onCompileStatusCodeChanged(int)));
        mProgFileManager->requestCompile();
    }
}

void TeachingProgramPageModel::setErrCode(QString errCode)
{
    if (m_errCode == errCode){
        return;
    }
    m_errCode = errCode;
    emit errCodeChanged(m_errCode);
}

void TeachingProgramPageModel::timerStart(int timeOut,int id)
{
    if (!mIsTimerStatus && id == 0) {
        mTimerId = startTimer(timeOut);
        mIsTimerStatus = true;
    }else if (!mIsTimerStatus && id == 1) {
        mTimerId1 = startTimer(timeOut);
        mIsTimerStatus = true;
    }
}

void TeachingProgramPageModel::timerStop(int id)
{
    if (mIsTimerStatus && id == 0) {
        killTimer(mTimerId);
        mTimerCnt = 0;
        mIsTimerStatus = false;
    }else if (mIsTimerStatus && id == 1) {
        killTimer(mTimerId1);
        mTimerCnt = 0;
        mIsTimerStatus = false;
    }
}

void TeachingProgramPageModel::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent || !mFtpManager || !mProgFileManager) return;
    if(timerEvent->timerId() == mTimerId){
        if (mFtpTimeOutCnt > 0){
            mTimerCnt++;
            if(mTimerCnt >= mFtpTimeOutCnt){
                checkFtpProcess();
            }
        } else {
            //qDebug("%s:%d ftp transfer timeout[%d] mFtpStatus[%d] mFtpTimeOutCnt[%d]", __func__, __LINE__, mTimerCnt, mFtpStatus, mFtpTimeOutCnt);
            checkFtpProcess();
        }
    }else if (timerEvent->timerId() == mTimerId1) {
        checkDemoComplieNcpResult();
    }
}

void TeachingProgramPageModel::onFtpStatusChanged(int ftpStatus)
{
    if (!mProgFileManager) return;
    qDebug("%s:%d ftpStatus[%d]", __func__, __LINE__, ftpStatus);
    mFtpStatus = ftpStatus;
    if (mFtpStatus == (int)Enums::FtpStatus::FtpStatusPutFinished){
        timerStop(0);
        disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
        disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
        mFtpStatus = (int)Enums::FtpStatus::FtpStatusLoggedIn;
        emit progSaveStatusChanged((int)ProgramSaveStatus::ProgCompiling);
        // コンパイル要求
        connect(mProgFileManager, SIGNAL(compileResultChanged(bool)), this, SLOT(onCompileResultChanged(bool)));
        connect(mProgFileManager, SIGNAL(processingTimeoutChanged(bool)), this, SLOT(onProcessingTimeoutChanged(bool)));
        connect(mProgFileManager, SIGNAL(compileStatusCodeChanged(int)), this, SLOT(onCompileStatusCodeChanged(int)));
        mProgFileManager->requestCompile();
    }
}

void TeachingProgramPageModel::onCompileResultChanged(bool compileResult)
{
    if (!mProgFileManager) return;
    qDebug("%s:%d compileResult[%d]", __func__, __LINE__, compileResult);
    disconnect(mProgFileManager, SIGNAL(compileResultChanged(bool)), this, SLOT(onCompileResultChanged(bool)));
    disconnect(mProgFileManager, SIGNAL(processingTimeoutChanged(bool)), this, SLOT(onProcessingTimeoutChanged(bool)));
    disconnect(mProgFileManager, SIGNAL(compileStatusCodeChanged(int)), this, SLOT(onCompileStatusCodeChanged(int)));
    if (compileResult){
        // コンパイル成功通知
        emit progSaveStatusChanged((int)ProgramSaveStatus::ProgCompileSuccess);
        m_compileErrorStep = 0;
        emit compileErrorStepChanged(m_compileErrorStep);
        m_processingResult = compileResult;
        emit processingResultChanged(m_processingResult);
    } else {
        connect(mProgFileManager, SIGNAL(compileErrorStepChanged(int)), this, SLOT(onCompileErrorStepChanged(int)));
        mProgFileManager->getCompileErrorInfo();
    }
}

void TeachingProgramPageModel::onCompileErrorStepChanged(int compileErrorStep)
{
    qDebug("%s:%d compileErrorStep[%d]", __func__, __LINE__, compileErrorStep);
    disconnect(mProgFileManager, SIGNAL(compileErrorStepChanged(int)), this, SLOT(onCompileErrorStepChanged(int)));
    m_compileErrorStep = compileErrorStep;
    emit compileErrorStepChanged(m_compileErrorStep);
    m_processingResult = false;
    emit processingResultChanged(m_processingResult);
    // コンパイル失敗
    emit progSaveStatusChanged((int)ProgramSaveStatus::ProgCompileFail);
}

void TeachingProgramPageModel::onProcessingTimeoutChanged(bool processingTimeout)
{
    qDebug("%s:%d processingTimeout[%d]", __func__, __LINE__, processingTimeout);
    disconnect(mProgFileManager, SIGNAL(compileResultChanged(bool)), this, SLOT(onCompileResultChanged(bool)));
    disconnect(mProgFileManager, SIGNAL(processingTimeoutChanged(bool)), this, SLOT(onProcessingTimeoutChanged(bool)));
    disconnect(mProgFileManager, SIGNAL(compileStatusCodeChanged(int)), this, SLOT(onCompileStatusCodeChanged(int)));
    m_processingTimeout = processingTimeout;
    emit processingTimeoutChanged(m_processingTimeout);
    emit progSaveStatusChanged((int)ProgramSaveStatus::ProgTimeOut);
}

void TeachingProgramPageModel::onCompileStatusCodeChanged(int compileStatusCode)
{
    qDebug("%s:%d compileStatusCode[%d]", __func__, __LINE__, compileStatusCode);
    m_compileStatusCode = compileStatusCode;
    emit compileStatusCodeChanged(m_compileStatusCode);
}

void TeachingProgramPageModel::onFtpErrorStrNotify(QString errorStr)
{
    qDebug() << "onFtpErrorStrNotify errorStr:" << errorStr;
    qDebug("%s:%d ftp process error mTimerCnt[%d]", __func__, __LINE__, mTimerCnt);
    timerStop(0);
    disconnect(mFtpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
    disconnect(mFtpManager, SIGNAL(ftpErrorStrNotify(QString)), this, SLOT(onFtpErrorStrNotify(QString)));
    emit progSaveStatusChanged((int)ProgramSaveStatus::ProgFtpPutErr);
}

////------------------------------------------------------
//void TeachingProgramPageModel::startModbusOneShot()
//{
//    if (!m_communication) return;
//    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
//            this, &TeachingProgramPageModel::onOneShotReadFinished, Qt::QueuedConnection);
//}
////------------------------------------------------------
//void TeachingProgramPageModel::stopModbusOneShot()
//{
//    if (!m_communication) return;
//    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
//        this, &TeachingProgramPageModel::onOneShotReadFinished);
//}
//------------------------------------------------------
void TeachingProgramPageModel::updateRunningProgramData()
{
    int runningStepNo = -1;
    for (int i = 0; i < mRunningProgramNos.size(); i++) {
        quint16 srAddress = RUNNING_PROGRAM_SR + i;
        auto value = m_modbus->getWordReceiveDataRG(srAddress);
        auto runningProgramNo = (value & 0xFF00) >> 8;
        auto runningProgramStep = value & 0x00FF;

        mRunningProgramNos[i] = runningProgramNo;
        mRunningProgramSteps[i] = runningProgramStep;

        if (mSelectedProgramNo == runningProgramNo) {
            runningStepNo = runningProgramStep;
        }
    }
    setRunningStepNo(runningStepNo);
    emit runningProgramNosChanged(mRunningProgramNos);
    emit runningProgramStepsChanged(mRunningProgramSteps);
}
//------------------------------------------------------
void TeachingProgramPageModel::updateAutoDriveData()
{
    setAutoDriveOneCycle(m_modbus->getReceiveDataDI(AUTO_DRIVE_ONE_CYCLE_DI));
    setAutoDriveStep(m_modbus->getReceiveDataDI(AUTO_DRIVE_STEP_DI));
    setAutoDriveStepPrev(m_modbus->getReceiveDataDI(AUTO_DRIVE_STEP_PREV_DI));
    setAutoDriveStepNext(m_modbus->getReceiveDataDI(AUTO_DRIVE_STEP_NEXT_DI));
}
//------------------------------------------------------
void TeachingProgramPageModel::selectWizardMenuByStep(QObject *ncpStepObject)
{
    auto ncpStep = dynamic_cast<NCPStep*>(ncpStepObject);
    if (ncpStep == nullptr) {
        qWarning() << QString("%1(%2): argument object is not NCPStep*.").arg(__PRETTY_FUNCTION__).arg(__LINE__);
        return;
    }

    auto ncpCommand = ncpStep->command();
    auto wizardMenuIndex = ncpCommand->wizardMenuIndex();
    if (wizardMenuIndex < NCPCommand::WizardMenuIndex::Menu1 ||
        NCPCommand::WizardMenuIndex::Menu5 < wizardMenuIndex) {
        qWarning() << QString("%1(%2): unknown wizard menu index:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << wizardMenuIndex;
        return;
    }

    auto wizardMenu = dynamic_cast<ProgramMenu*>(m_menuModel.at(wizardMenuIndex));
    wizardMenu->show(ncpCommand);
    setMainmenu_Index(wizardMenuIndex);
}
//------------------------------------------------------
void TeachingProgramPageModel::createNcpProgram(int programNo)
{
    mNcpFile->createProgramForQml(programNo);
    mNcpFile->setEdited(true);
}
//------------------------------------------------------
void TeachingProgramPageModel::deleteNcpProgram(int programNo)
{
    mNcpFile->deleteProgram(programNo);
    mNcpFile->setEdited(true);
}
//------------------------------------------------------
int TeachingProgramPageModel::assignLabelNo(QObject* ncpProgramObject, int lineIndex, int labelNo)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << QString("%1(%2): argument object is not NCPProgram*.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        return -1;
    }
    auto ret = program->assignLabelNoInNcpStep(lineIndex, labelNo);
    if (!ret) {
        return -1;
    }
    mNcpFile->setEdited(true);
    return 0;
}
//------------------------------------------------------
int TeachingProgramPageModel::releaseLabel(QObject* ncpProgramObject, int lineIndex)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << QString("%1(%2): argument object is not NCPProgram*.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        return -1;
    }
    auto ret = program->releaseLabelInNcpStep(lineIndex);
    if (!ret) {
        return -1;
    }
    mNcpFile->setEdited(true);
    return 0;
}
//------------------------------------------------------
int TeachingProgramPageModel::copyNcpStepToTemp(QObject* ncpProgramObject, int lineIndex)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << QString("%1(%2): argument object is not NCPProgram*.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        return -1;
    }
    if (program->steps().size() <= lineIndex) {
        qWarning() << QString("%1(%2): out of range, lineIndex.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex << program->steps().size();
        return -1;
    }
    clearTempNcpStep();
    mTempNcpStep = program->cloneNcpStep(lineIndex);
    m_operationRecord->recordProgramCopy(program->no());
    return 0;
}
//------------------------------------------------------
int TeachingProgramPageModel::pasteNcpStepFromTemp(QObject* ncpProgramObject, int lineIndex)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << QString("%1(%2): argument object is not NCPProgram*.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        return -1;
    }
    if (program->steps().size() <= lineIndex) {
        qWarning() << QString("%1(%2): out of range, lineIndex.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex << program->steps().size();
        return -1;
    }
    if (!mTempNcpStep) {
        return -1;  // NCPStep not copied.
    }
    auto cloneNcpStep = mTempNcpStep->clone();

    if(cloneNcpStep->command()->toCode().left(4) ==  "MOVE" && cloneNcpStep->command()->pointNo() >= 101){
        int freepointNo = -1;
        auto pointObj = mNcpFile->findFreeArbitraryPoint();
        if (pointObj != nullptr) {
            freepointNo = pointObj->no();
        }
        cloneNcpStep->command()->setPointNo(freepointNo);
    }//筛选任意点指令粘贴

    auto ret = program->insertStep(lineIndex, cloneNcpStep);
    if (!ret) {
        qWarning() << QString("%1(%2): insert failed.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        delete cloneNcpStep;
        return -1;
    }
    mNcpFile->setEdited(true);
    m_operationRecord->recordProgramPaste(program->no());
    return 0;
}
//------------------------------------------------------
int TeachingProgramPageModel::removeNcpStepInNcpProgram(QObject* ncpProgramObject, int lineIndex)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << QString("%1(%2): argument object is not NCPProgram*.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        return -1;
    }
    program->removeStep(lineIndex);
    mNcpFile->setEdited(true);
    m_operationRecord->recordProgramDelete(program->no());
    return 0;
}
//------------------------------------------------------
int TeachingProgramPageModel::moveBackOneStepInNcpProgram(QObject* ncpProgramObject, int lineIndex)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << QString("%1(%2): argument object is not NCPProgram*.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        return -1;
    }
    program->moveBackOneStep(lineIndex);
    mNcpFile->setEdited(true);
    m_operationRecord->recordProgramMoveUp(program->no());
    return 0;
}
//------------------------------------------------------
int TeachingProgramPageModel::moveForwardOneStepInNcpProgram(QObject* ncpProgramObject, int lineIndex)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << QString("%1(%2): argument object is not NCPProgram*.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        return -1;
    }
    program->moveForwardOneStep(lineIndex);
    mNcpFile->setEdited(true);
    m_operationRecord->recordProgramMoveDown(program->no());
    return 0;
}
//------------------------------------------------------
int TeachingProgramPageModel::insertNcpCommand(QObject* ncpProgramObject, int lineIndex)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << QString("%1(%2): argument object is not NCPProgram*.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
        return -1;
    }
    int ret = 0;
    ProgramMenu *menu = static_cast<ProgramMenu *>(m_menuModel.at(Mainmenu_Index()));
    if (menu->commandCreateMethod() == ProgramMenu::CreateMethod::SimpleNcpCommand) {
        auto command = menu->createNcpCommand();
        if (command == nullptr) {
            qDebug("not create command.");
            return -1;
        }
        auto step = new NCPStep(command);
        if (step == nullptr) {
            qWarning() << QString("%1(%2): not create step.").arg(__PRETTY_FUNCTION__).arg(__LINE__);
            delete command;
            return -1;
        }
        qDebug() << __func__ << program->no() << lineIndex << command->toString() << command->toCode();
        auto ret = program->insertStep(lineIndex, step);
        if (!ret) {
            qWarning() << QString("%1(%2): insert failed.").arg(__PRETTY_FUNCTION__).arg(__LINE__) << ncpProgramObject << lineIndex;
            delete step;
            return -1;
        }
    } else {
        ret = menu->insertCustomNcpCommand(ncpFile(), program, lineIndex);
    }
    mNcpFile->setEdited(true);
    m_operationRecord->recordProgramInsert(program->no());
    return ret;
}
//------------------------------------------------------
int TeachingProgramPageModel::updateModbusData()
{
    ProgramMenu *menu = static_cast<ProgramMenu *>(m_menuModel.at(Mainmenu_Index()));
    return menu->updateModbusData();
}
//------------------------------------------------------
int TeachingProgramPageModel::replaceNcpCommand(QObject* ncpProgramObject, int lineIndex)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << "argument object is not NCPProgram*." << ncpProgramObject << lineIndex;
        return -1;
    }
    int ret = 0;
    ProgramMenu *menu = static_cast<ProgramMenu *>(m_menuModel.at(Mainmenu_Index()));
    if (menu->commandCreateMethod() == ProgramMenu::CreateMethod::SimpleNcpCommand) {
        auto command = menu->createNcpCommand();
        if (command == nullptr) {
            qDebug("not create command.");
            return -1;
        }
        qDebug() << __func__ << program->no() << lineIndex << command->toString() << command->toCode();
        program->replaceCommand(lineIndex, command);
    } else {
        ret = menu->replaceCustomNcpCommand(ncpFile(), program, lineIndex);
    }
    mNcpFile->setEdited(true);
    m_operationRecord->recordProgramReplace(program->no());
    return ret;
}
//------------------------------------------------------
int TeachingProgramPageModel::replaceNcpCommandByCommandLine(QObject* ncpProgramObject, int lineIndex, const QString& commandLine)
{
    auto program = dynamic_cast<NCPProgram*>(ncpProgramObject);
    if (!program) {
        qWarning() << "argument object is not NCPProgram*." << ncpProgramObject << lineIndex;
        return -1;
    }
    qDebug() << __func__ << program->no() << lineIndex << commandLine;
    auto ret = mNcpFile->replaceNcpCommand(program, lineIndex, commandLine);
    if (!ret) {
        return -1;
    }
    mNcpFile->setEdited(true);
    return 0;
}

QString TeachingProgramPageModel::toTranslatedString(NCPStep *step, int language) const
{
    return m_commandString->toTranslatedString(step, language);
}
//------------------------------------------------------
void TeachingProgramPageModel::restoreNcpFile()
{
    if (mNcpFile->edited()) {
        restoreNcpFileInstance();
    }
}
//------------------------------------------------------
void TeachingProgramPageModel::requestOpenSaveProgramDialog(bool isCloseProgramWhenSaveCanceled)
{
    m_isCloseProgramWhenSaveCanceled = isCloseProgramWhenSaveCanceled;
    emit isCloseProgramWhenSaveCanceledChanged(m_isCloseProgramWhenSaveCanceled);
    m_openSaveProgramDialog = true;
    emit openSaveProgramDialogChanged(m_openSaveProgramDialog);
}
//------------------------------------------------------
void TeachingProgramPageModel::requestCloseSaveProgramDialog()
{
    m_openSaveProgramDialog = false;
    emit openSaveProgramDialogChanged(m_openSaveProgramDialog);
}

void TeachingProgramPageModel::onNcpCommandDataChanged()
{
    m_ncpStepListUpdate = !m_ncpStepListUpdate;
    emit ncpStepListUpdateChanged();
}
//------------------------------------------------------
QString TeachingProgramPageModel::formatPosition(int position)
{
    const int format = m_axisPointInfo->formatArbitraryPointPosition();
    return m_resource->formatData(position, format, 2, true);
}
//------------------------------------------------------
void TeachingProgramPageModel::onIsNcpFileUpdateChanged(bool isNcpFileUpdate)
{
    if (!isNcpFileUpdate) {
        return;
    }
    startAsyncNcpFileLoad();
}
//------------------------------------------------------
#ifdef REAL_TARGET
void TeachingProgramPageModel::onFtpFileInfoChanged(int ftpFileInfo)
{
    if (ftpFileInfo != (int)FtpManager::FtpFileInfo::FTP_NCSTEP_NCP){
        //qDebug() << "TeachingProgramPageModel::onFtpFileInfoChanged ftpFileInfo:" << ftpFileInfo;
        return;
    }
    startAsyncNcpFileLoad();
}
#endif // REAL_TARGET

void TeachingProgramPageModel::setIsNcpFileLoaded(bool isNcpFileLoaded)
{
    if (m_isNcpFileLoaded == isNcpFileLoaded) { return; }
    m_isNcpFileLoaded = isNcpFileLoaded;
    emit isNcpFileLoadedChanged(m_isNcpFileLoaded);
}

void TeachingProgramPageModel::startAsyncNcpFileLoad()
{
    setIsNcpFileLoaded(false);
    m_threadPool.clear();
    m_threadPool.start(m_asyncLoadNcpFileTask);
}

void TeachingProgramPageModel::onAsyncTaskFinished()
{
    auto ncpFile = m_asyncLoadNcpFileTask->popNcpFile();
    if (!ncpFile) {
        qWarning() << QString("%1(%2): failed to load ncp file.").arg(__PRETTY_FUNCTION__).arg(__LINE__);
        return;
    }
    auto tempNcpFile = mNcpFile;
    mNcpFile = ncpFile->clone();

    progMenu2()->setNcpFile(mNcpFile);

    emit ncpFileChanged(mNcpFile);
    if (tempNcpFile) {
        delete tempNcpFile;
    }
    delete ncpFile;
    setIsNcpFileLoaded(true);
}

void TeachingProgramPageModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void TeachingProgramPageModel::anysReloadNcp(QString ncpname)
{
    QStringList name = ncpname.split('/', QString::SkipEmptyParts);
    QString ncp = "/";
    ncp.append(name.last());
    m_asyncLoadNcpFileTask->setncpFileName(ncp);
    startAsyncNcpFileLoad();
    setEditNcp(name.last());
}

void TeachingProgramPageModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}

void TeachingProgramPageModel::setEditNcp(QString ncp)
{
    if(m_editNcp != ncp){
        m_editNcp = ncp;
        emit editNcpChanged(m_editNcp);
    }
}

void TeachingProgramPageModel::endModbusPageWriteReady(QList<int> writeData)
{
    if ((writeData.size() == 3) &&
        ((writeData[0] == 8) && (writeData[1] == 0) && (writeData[2] == 0))) {
        m_pageChanging = false;
    }
}

void TeachingProgramPageModel::setMainmenu_Index(int Mainmenu_Index){
    if(m_Mainmenu_Index == Mainmenu_Index)
        return;
    m_Mainmenu_Index = Mainmenu_Index;
    emit Mainmenu_IndexChanged(m_Mainmenu_Index);
}

void TeachingProgramPageModel::setCurrentProgTitle(QString CurrentProgTitle){
    if(m_CurrentProgTitle == CurrentProgTitle)
        return;
    m_CurrentProgTitle = CurrentProgTitle;
    emit CurrentProgTitleChanged(CurrentProgTitle);
}

void TeachingProgramPageModel::setOneCycle(bool oneCycle){
    if(m_oneCycle != oneCycle){
        m_oneCycle = oneCycle;
        emit oneCycleChanged(m_oneCycle);
    }
}

void TeachingProgramPageModel::setCycle(bool cycle){
    if(m_cycle != cycle){
        m_cycle = cycle;
        emit cycleChanged(m_cycle);
    }
}

void TeachingProgramPageModel::setStep(bool step){
    if(m_step != step){
        m_step = step;
        emit stepChanged(m_step);
    }
}
