#ifndef TEACHINGPROGRAMPAGEMODEL_H
#define TEACHINGPROGRAMPAGEMODEL_H

#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QRunnable>
#include <QThreadPool>
#include <QTimerEvent>
#include "pagemodelbase.h"
#include "teachingview/programview/ncpfile.h"   // NCPFile.
#include "teachingview/programview/ncpstep.h"   // NCPStep.
#include "ftpmanager.h"                         // Ftp
#include "programfilemanager.h"                 // ProgFile Mng
#include "programview/programmenu/programmenu1.h"
#include "programview/programmenu/programmenu2.h"
#include "programview/programmenu/programmenu3.h"
#include "programview/programmenu/programmenu4.h"
#include "programview/programmenu/programmenu5.h"
#include "programview/ncpoperate.h"
#include "externalmemoryviewmodel.h"            // ExtMem

class AsyncLoadNcpFileTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit AsyncLoadNcpFileTask(ResourceManager *resource);
    virtual ~AsyncLoadNcpFileTask();
    NCPFile* popNcpFile();
    virtual void run() override;    // QRunnable interface
    void setncpFileName(QString name);
    QString getncpFileName();

signals:
    void resultReady();
private:
    void pushNcpFile(NCPFile* ncpFile);
private:
    ResourceManager*        m_resource = nullptr;
    QMutex                  m_ncpFileQueueMutex;
    QQueue<NCPFile*>        m_ncpFileQueue;
    QString                 m_ncpfilename = "/VGR5STDA00.NCP"; // MODIFIED : changed npc file By. SH.HONG 25.07.30
};

class NCPCommandString;
class TeachingProgramPagePointInfo;

class TeachingProgramPageModel : public PageModelBase
{
    Q_OBJECT
    Q_PROPERTY(bool isNcpFileLoaded READ isNcpFileLoaded NOTIFY isNcpFileLoadedChanged)
    Q_PROPERTY(QObject* ncpFile READ ncpFile NOTIFY ncpFileChanged)
    Q_PROPERTY(int selectedProgramNo READ selectedProgramNo WRITE setSelectedProgramNo NOTIFY selectedProgramNoChanged)
    Q_PROPERTY(int runningStepNo READ runningStepNo NOTIFY runningStepNoChanged)
    Q_PROPERTY(QList<int> runningProgramNos READ runningProgramNos NOTIFY runningProgramNosChanged)
    Q_PROPERTY(QList<int> runningProgramSteps READ runningProgramSteps NOTIFY runningProgramStepsChanged)
    // Q_PROPERTY(QList<bool> validAxes READ validAxes CONSTANT)    // todo.
    Q_PROPERTY(bool autoDriveOneCycle READ autoDriveOneCycle NOTIFY autoDriveOneCycleChanged)
    Q_PROPERTY(bool autoDriveStep READ autoDriveStep NOTIFY autoDriveStepChanged)
    Q_PROPERTY(bool autoDriveStepNext READ autoDriveStepNext NOTIFY autoDriveStepNextChanged)
    Q_PROPERTY(bool autoDriveStepPrev READ autoDriveStepPrev NOTIFY autoDriveStepPrevChanged)
    Q_PROPERTY(ProgramMenu1* progMenu1 READ progMenu1 CONSTANT)
    Q_PROPERTY(ProgramMenu2* progMenu2 READ progMenu2 CONSTANT)
    Q_PROPERTY(ProgramMenu3* progMenu3 READ progMenu3 CONSTANT)
    Q_PROPERTY(ProgramMenu4* progMenu4 READ progMenu4 CONSTANT)
    Q_PROPERTY(ProgramMenu5* progMenu5 READ progMenu5 CONSTANT)
    Q_PROPERTY(NcpOperate* ncp READ  ncp CONSTANT)
    Q_PROPERTY(QList<QObject *> menuModel READ menuModel CONSTANT)
    Q_PROPERTY(int Mainmenu_Index READ Mainmenu_Index WRITE setMainmenu_Index NOTIFY Mainmenu_IndexChanged)
    Q_PROPERTY(QObject *axisInfo READ axisInfo CONSTANT)
    Q_PROPERTY(QString CurrentProgTitle READ CurrentProgTitle WRITE setCurrentProgTitle NOTIFY CurrentProgTitleChanged)//当前程序号
    Q_PROPERTY(int compileErrorStep MEMBER m_compileErrorStep NOTIFY compileErrorStepChanged)
    Q_PROPERTY(bool processingResult MEMBER m_processingResult NOTIFY processingResultChanged)
    Q_PROPERTY(bool processingTimeout MEMBER m_processingTimeout NOTIFY processingTimeoutChanged)
    Q_PROPERTY(QString errCode READ errCode WRITE setErrCode NOTIFY errCodeChanged)
    Q_PROPERTY(int progSaveStatus MEMBER m_progSaveStatus NOTIFY progSaveStatusChanged)
    Q_PROPERTY(int compileStatusCode MEMBER m_compileStatusCode NOTIFY compileStatusCodeChanged)
    Q_PROPERTY(bool openSaveProgramDialog READ openSaveProgramDialog NOTIFY openSaveProgramDialogChanged)
    Q_PROPERTY(bool isCloseProgramWhenSaveCanceled READ isCloseProgramWhenSaveCanceled NOTIFY isCloseProgramWhenSaveCanceledChanged)
    Q_PROPERTY(int reasonClosingSaveProgramDialog MEMBER m_reasonClosingSaveProgramDialog NOTIFY reasonClosingSaveProgramDialogChanged)
    Q_PROPERTY(bool ncpStepListUpdate MEMBER m_ncpStepListUpdate NOTIFY ncpStepListUpdateChanged)
    Q_PROPERTY(bool isFtpFileAcquired MEMBER m_isFtpFileAcquired NOTIFY isFtpFileAcquiredChanged)
    Q_PROPERTY(bool oneCycle READ oneCycle NOTIFY oneCycleChanged)
    Q_PROPERTY(bool cycle READ cycle NOTIFY cycleChanged)
    Q_PROPERTY(bool step READ step NOTIFY stepChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
    Q_PROPERTY(QString editNcp READ editNcp WRITE setEditNcp NOTIFY editNcpChanged)//当前程序号

public:
    enum ProgramSaveStatus {
        ProgFtpPutErr       = -2,
        ProgTimeOut         = -1,
        ProgInit            = 0,
        ProgFtpTransfer     = 1,
        ProgCompiling       = 2,
        ProgCompileSuccess  = 3,
        ProgCompileFail     = 4,
    };
    explicit TeachingProgramPageModel(ResourceManager *resource, CommunicationThread * comm, ExternalMemoryViewModel *extMemView);
    explicit TeachingProgramPageModel(QObject *object = nullptr);
    ~TeachingProgramPageModel() override;

    Q_INVOKABLE void selectMenu1(){
        setMainmenu_Index(0);
        progMenu1()->setMenuIndex(0);
        progMenu1()->initData();
    }
    Q_INVOKABLE void selectMenu2(){
        QList<bool> list ={0,0,0,0,0,0,0,0};
        setMainmenu_Index(1);
        progMenu2()->setMenuIndex(1); // MODIFIED :  change index 0-> 1 25.07.08 IJ.YI
    }
    Q_INVOKABLE void selectMenu3(){
        setMainmenu_Index(2);
        progMenu3()->setMenu3_Index(0);
    }
    Q_INVOKABLE void selectMenu4(){
        setMainmenu_Index(3);
        progMenu4()->setMenuIndex(0);
    }
    Q_INVOKABLE void selectMenu5(){
        setMainmenu_Index(4);
        progMenu5()->setMenu5_Index(0);
    }

    Q_INVOKABLE void selectWizardMenuByStep(QObject* ncpStepObject);
    Q_INVOKABLE void createNcpProgram(int programNo);
    Q_INVOKABLE void deleteNcpProgram(int programNo);
    Q_INVOKABLE int assignLabelNo(QObject* ncpProgramObject, int lineIndex, int labelNo);
    Q_INVOKABLE int releaseLabel(QObject* ncpProgramObject, int lineIndex);
    Q_INVOKABLE int copyNcpStepToTemp(QObject* ncpProgramObject, int lineIndex);
    Q_INVOKABLE int pasteNcpStepFromTemp(QObject* ncpProgramObject, int lineIndex);
    Q_INVOKABLE int removeNcpStepInNcpProgram(QObject* ncpProgramObject, int lineIndex);
    Q_INVOKABLE int moveBackOneStepInNcpProgram(QObject* ncpProgramObject, int lineIndex);
    Q_INVOKABLE int moveForwardOneStepInNcpProgram(QObject* ncpProgramObject, int lineIndex);
    Q_INVOKABLE int insertNcpCommand(QObject* ncpProgramObject, int lineIndex);
    Q_INVOKABLE int updateModbusData();
    Q_INVOKABLE int replaceNcpCommand(QObject* ncpProgramObject, int lineIndex);
    Q_INVOKABLE int replaceNcpCommandByCommandLine(QObject* ncpProgramObject, int lineIndex, const QString& commandLine);
    Q_INVOKABLE QString toTranslatedString(NCPStep *step, int language) const;
    Q_INVOKABLE void restoreNcpFile();
    Q_INVOKABLE void requestOpenSaveProgramDialog(bool isCloseProgramWhenSaveCanceled = true);
    Q_INVOKABLE void requestCloseSaveProgramDialog();
    Q_INVOKABLE QString formatPosition(int position);
    Q_INVOKABLE void startPageChanging();
    Q_INVOKABLE void anysReloadNcp(QString ncpname);

    //接口
    ProgramMenu1* progMenu1() const{return (ProgramMenu1*)m_menuModel.at(0);}//菜单1
    ProgramMenu2* progMenu2() const{return (ProgramMenu2*)m_menuModel.at(1);}//菜单2
    ProgramMenu3* progMenu3() const{return (ProgramMenu3*)m_menuModel.at(2);}//菜单3
    ProgramMenu4* progMenu4() const{return (ProgramMenu4*)m_menuModel.at(3);}//菜单4
    ProgramMenu5* progMenu5() const{return (ProgramMenu5*)m_menuModel.at(4);}//菜单5
    NcpOperate* ncp() {return (NcpOperate*)m_menuModel.at(5);}
    QList<QObject *> menuModel() const{ return m_menuModel;}
    QObject *axisInfo() const { return m_axisInfo; }
    QString editNcp()const {return m_editNcp;}

    int Mainmenu_Index()const{ return m_Mainmenu_Index;}
    QString CurrentProgTitle(){return m_CurrentProgTitle;}
    bool oneCycle() { return m_oneCycle; }
    bool cycle() { return m_cycle; }
    bool step() { return m_step; }
    Q_INVOKABLE int saveNCPFile();
    Q_INVOKABLE void saveNCPFile1(QString filepath);
    Q_INVOKABLE void demoComplieNcp();
    void checkDemoComplieNcpResult();
    void parseDemoComplieNcpResult(int rst1,int rst2, int rst3);
    void updateNcpStepList();

    // gettter/setter.
    bool isNcpFileLoaded() const { return m_isNcpFileLoaded; }
    NCPFile *ncpFile() const { return mNcpFile; }
    int selectedProgramNo() const { return mSelectedProgramNo; }
    int runningStepNo() const { return mRunningStepNo; }
    QList<int> runningProgramNos() const { return mRunningProgramNos; }
    QList<int> runningProgramSteps() const { return mRunningProgramSteps; }
    // QList<bool> validAxes() const { return mValidAxes; } // todo.
    bool autoDriveOneCycle() const { return mAutoDriveOneCycle; }
    bool autoDriveStep() const { return mAutoDriveStep; }
    bool autoDriveStepNext() const { return mAutoDriveStepNext; }
    bool autoDriveStepPrev() const { return mAutoDriveStepPrev; }
    bool openSaveProgramDialog() const { return m_openSaveProgramDialog; }
    bool isCloseProgramWhenSaveCanceled() const { return m_isCloseProgramWhenSaveCanceled; }

    static QString pensetGroup() { return "16"; }
    void setErrCode(QString errCode);
    QString errCode() const { return m_errCode; }
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }
    void setEditNcp(QString ncp);

private:
    // gettter/setter.
    void setIsNcpFileLoaded(bool isNcpFileLoaded);
    void setSelectedProgramNo(int selectedProgramNo);
    void setRunningStepNo(int runningStepNo);
    void setAutoDriveOneCycle(bool autoDriveOneCycle);
    void setAutoDriveStep(bool autoDriveStep);
    void setAutoDriveStepNext(bool autoDriveStepNext);
    void setAutoDriveStepPrev(bool autoDriveStepPrev);
    void clearTempNcpStep();
    void checkFtpProcess();

signals:
    void isNcpFileLoadedChanged(bool);
    void ncpFileChanged(QObject*);
    void selectedProgramNoChanged(int);
    void runningStepNoChanged(int);
    void runningProgramNosChanged(QList<int>);
    void runningProgramStepsChanged(QList<int>);
    void autoDriveOneCycleChanged(bool);
    void autoDriveStepChanged(bool);
    void autoDriveStepNextChanged(bool);
    void autoDriveStepPrevChanged(bool);
    void Mainmenu_IndexChanged(int Mainmenu_Index);
    void CurrentProgTitleChanged(QString CurrentProgTitle);
    void Menu1_1StateChanged(QList<bool> Menu1_1State);
    void compileErrorStepChanged(int compileErrorStep);
    void processingResultChanged(bool processingResult);
    void processingTimeoutChanged(bool processingTimeout);
    void errCodeChanged(QString errCode);
    void progSaveStatusChanged(int progSaveStatus);
    void compileStatusCodeChanged(int compileStatusCode);
    void openSaveProgramDialogChanged(bool openSaveProgramDialog);
    void isCloseProgramWhenSaveCanceledChanged(bool isCloseProgramWhenSaveCanceled);
    void reasonClosingSaveProgramDialogChanged(int reasonClosingSaveProgramDialog);
    void ncpStepListUpdateChanged();
    void isFtpFileAcquiredChanged(bool isFtpFileAcquired);
    void oneCycleChanged(bool oneCycle);
    void cycleChanged(bool cycle);
    void stepChanged(bool step);
    void dataLoadedChanged(bool dataLoaded);
    void editNcpChanged(QString ncp);
    void demoCompileSuccess();
    void demoCompileFailed(QList<int> errTypeStep);

public slots:
    void onLanguageChanged(int language);
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command,CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void endModbusPageWriteReady(QList<int> writeData);
    void setMainmenu_Index(int Mainmenu_Index);
    void setCurrentProgTitle(QString CurrentProgTitle);
    void setOneCycle(bool oneCycle);
    void setCycle(bool cycle);
    void setStep(bool step);

protected:
    // override method.
    void onActivate() override;
    void onDeactivate() override;

    // method for modbus.
    void startModbusPeriodic();
    void stopModbusPeriodic();
    // void startModbusOneShot();
    // void stopModbusOneShot();
    // void modbusWriteHR(CommunicationThread::OneShotReadModBusCaller caller, quint16 addr, int value);
    // void modbusReadDI(CommunicationThread::OneShotReadModBusCaller caller, quint16 addr, quint16 size);
    // void modbusReadHR(CommunicationThread::OneShotReadModBusCaller caller, quint16 addr, quint16 size);
    // void setOneShotModbusCommand(CommunicationThread::OneShotReadModBusCaller caller, stModBusCommunicationData &data);
    void timerEvent(QTimerEvent *timerEvent) override;

private:
    // method.
    void init();
    void backupNcpFileInstance();
    void restoreNcpFileInstance();
    void readPenSetData();
    void updateRunningProgramData();
    void updateAutoDriveData();
    int ftpTransfer(QString ncpFileName);
    void timerStart(int timeOut,int id);
    void timerStop(int id);
    void startAsyncNcpFileLoad();

private:
    // common.
    bool                    m_isNcpFileLoaded = false;
    NCPFile                 *mNcpFile = nullptr;
    NCPFile                 *mNcpFileBackup = nullptr;
    FtpManager              *mFtpManager;
    ProgramFileManager      *mProgFileManager;
    QFile                   *mPutFile = nullptr;
    int                     mSelectedProgramNo;
    NCPCommandString        *m_commandString;

    // for async ncp file load.
    AsyncLoadNcpFileTask    *m_asyncLoadNcpFileTask = nullptr;
    QThreadPool             m_threadPool;   // for async task.

    // running program.
    int                     mRunningStepNo;    // running step number of the selected program.
    QList<int>              mRunningProgramNos;
    QList<int>              mRunningProgramSteps;

    // for expand screen.
    bool                    mAutoDriveOneCycle;
    bool                    mAutoDriveStep;
    bool                    mAutoDriveStepPrev;
    bool                    mAutoDriveStepNext;
    bool                    m_oneCycle;
    bool                    m_cycle;
    bool                    m_step;

    QList<QObject *>        m_menuModel;
    NcpOperate              *m_ncp;
    int                     m_Mainmenu_Index=0;
    QList<int>              m_AutoProgDisplay;
    QString                 m_CurrentProgTitle;
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();
    AxisPointInfo           *m_axisPointInfo = nullptr;
    ExternalMemoryViewModel *m_extMemView = nullptr;
    QList<QList<quint16>>   m_axisInfoDataFormat;
    bool                    mIsTimerStatus = false;
    int                     mTimerCnt = 0;
    int                     mTimerId;
    int                     mTimerId1;
    int                     mFtpStatus = 0;
    int                     m_compileErrorStep = 0;
    bool                    m_processingResult = false;
    bool                    m_processingTimeout = false;
    QString                 m_errCode = "";
    int                     m_compileStatusCode = 0;
    int                     mFtpTimeOutCnt = 0;
    bool                    m_pageChanging = false;
    bool                    m_dataLoaded = false;

    // copy & paste.
    NCPStep*                mTempNcpStep;
    int                     m_progSaveStatus = 0;
    QString                 m_editNcp = "VGR5STDA00.NCP"; // MODIFIED : changed npc file By. SH.HONG 25.07.30

    // save program dialog.
    bool                    m_openSaveProgramDialog = false;
    bool                    m_isCloseProgramWhenSaveCanceled = true;
    int                     m_reasonClosingSaveProgramDialog = -1;
    bool                    m_ncpStepListUpdate = false;
    bool                    m_isFtpFileAcquired = false;

    TeachingProgramPagePointInfo    *m_pointInfo;

private slots:
    void onFtpStatusChanged(int ftpStatus);
    void onCompileResultChanged(bool compileResult);
    void onCompileErrorStepChanged(int compileErrorStep);
    void onProcessingTimeoutChanged(bool processingTimeout);
    void onCompileStatusCodeChanged(int compileStatusCode);
    void onFtpErrorStrNotify(QString errorStr);
    void onNcpCommandDataChanged();
    void onIsNcpFileUpdateChanged(bool isNcpFileUpdate);
#ifdef REAL_TARGET
    void onFtpFileInfoChanged(int ftpFileInfo);
#endif
    void onAsyncTaskFinished();
};

#endif // TEACHINGPROGRAMPAGEMODEL_H
