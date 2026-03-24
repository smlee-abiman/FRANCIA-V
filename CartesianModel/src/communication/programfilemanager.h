#ifndef PROGRAMFILEMANAGER_H
#define PROGRAMFILEMANAGER_H

#include <QObject>
#include <QtGlobal>
#include <QVector>
#include <QTimerEvent>
#include <cstring>
#include "corelibsdcontrol.h"
#include "appenvironment.h"
#include "corelibmodbuscommunication.h"
#include "modbusmanager.h"
#include "operationprogramlinedata.h"

typedef struct {
    int fileSize;
    int transNum;
    int firstTransSize;
    int middleTransSize;
    int lastTransSize;
} stTranferInfo;

typedef struct {
    QString fileNumber;
    QString fileName;
    QString fileCreateTime;
    QString fileUpdateTime;
    int fileSize;
} stProgFileInfo;

typedef struct {
    int hrNumber;
    int size;
} stProgFileModbusInfo;

struct ProgCompileErrInfo {
    Q_GADGET
    Q_PROPERTY(QString errProgName MEMBER errProgName)
    Q_PROPERTY(int errNumber MEMBER errNumber)
    Q_PROPERTY(int errLineNum MEMBER errLineNum)
public:
    QString errProgName;
    int errNumber;
    int errLineNum;
};

#define PROG_FILE_REGISTER_NUM_MAX      8
#define PROG_FILE_TRANSER_SIZE_MAX      112
#define PROG_FILE_TRANSER_START_MODE    0x0001
#define PROG_FILE_TRANSER_MODE          0x0002
#define PROG_FILE_TRANSER_DELETE_MODE   0x0003
#define PROG_FILE_TRANSER_MODE_HR       1
#define PROG_FILE_TRANSER_DATA_HR       2
#define PROG_FILE_NAME_HR               73
#define PROG_FILE_NAME_SIZE             32      // unit word
#define PROG_ONE_FILE_NAME_SIZE_MAX     4       // unit word
#define PROG_FILE_NAME_START_RG         193
#define PROG_FILE_NAME_SIZE_MAX_RG      4       // unit word
#define PROG_FILE_ERROR_NUM_RG          197
#define PROG_FILE_ERROR_LINE_NUM_RG     198
#define PROG_FILE_ERR_INFO_SIZE_MAX_RG  2
#define PROG_FILE_EXE_COMPILE1_DO       32
#define PROG_FILE_EXE_COMPILE2_DO       25
#define PROG_FILE_UPDATE1_DO            32
#define PROG_FILE_UPDATE2_DO            25

#define PROG_EDIT_COMPILE_TIMEOUT_COUNT             600     // interval:100ms timeout:60s
#define PROG_EDIT_REQUEST_COMPILE_HR                1657
#define PROG_EDIT_CONTROL_COMMAND_SR                1695
#define PROG_EDIT_EXECUTION_STATE_SR                1696
#define PROG_EDIT_COMPILE_ERROR_STEP_SR             1697

class ProgramFileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool compileResult MEMBER m_compileResult NOTIFY compileResultChanged)
    Q_PROPERTY(int compileErrorStep MEMBER m_compileErrorStep NOTIFY compileErrorStepChanged)
    Q_PROPERTY(bool processingTimeout MEMBER m_processingTimeout NOTIFY processingTimeoutChanged)

private:
    explicit ProgramFileManager(QObject *parent = 0, QString aplName = "pendant-gui");
    ~ProgramFileManager() = default;

    static ProgramFileManager* instance;
public:

    enum StatusCode {
        CmdWait                     = 0,
        CmdSuccess1                 = 1,
        CmdCompileSuccess           = 30,
        CmdAbnormal1                = 129,
        FromWriteOutOfRange         = 136,
        FromWritAbnormal            = 137,
        FromReadAbnormal            = 138,
        InternalAreaReadAbnormal    = 141,
        InternalAreaWriteAbnormal   = 142,
        InternalAreaFileNoNotExist  = 143,
        InternalAreaOverCapacity    = 144,
        InternalAreaCompareMismatch = 145,
        InternalAreaDeleteAbnormal  = 146,
        GrammarError                = 148,              // 文法エラー
        IOSymbolError               = 149,              // IOシンボルエラー
        CommandError                = 150,              // コマンドエラー
        CommandNumError             = 151,              // コマンド数エラー
        LabelError                  = 152,              // レベルエラー
        LabelNumError               = 153,              // レベル数エラー
        JumpError                   = 154,              // ジャンプエラー
        CodeSizeError               = 155,              // コードサイズエラー
        DuplicatePointsError        = 156,              // ポイント重複エラー
        NexTSetError                = 157,              // NEXT-SETエラー
        OtherError                  = 158,              // その他エラー
        CmdNotSupported             = 254,             // 0xFE
        CmdExe                      = 255,             // 0xFF
    };
    enum ControlCode {
        ProgFileCompile             = 30,
    };
    enum InternalStatus {
        CurCmdConfirmBefore         = 0,
        CmdExeStart                 = 1,
        CmdExecuted                 = 2,
        CmdExeError                 = 3,
        CmdExeSuccess               = 4,
    };

    static ProgramFileManager* create_instance(QObject *parent = 0) {
        if(!instance){
            instance = new ProgramFileManager(parent);
        }
        return instance;
    }

    static ProgramFileManager* get_instance() {
        return instance;
    }

    static void destroy_instance() {
        delete instance;
        instance = nullptr;
    }
    ProgCompileErrInfo compileErrInfo() const{ return progCompileErrInfo; }
    Q_INVOKABLE bool transferOpeProgramFile(QString *fileName = nullptr);
    Q_INVOKABLE bool deleteProgramFile(QString *fileName);
    int getProgramFileListNum();
    QList<stProgFileInfo> getProgramFileInfo();
    Q_INVOKABLE bool confirmProgramFileExist();
    QFileInfoList getProgramFileList();
    QFileInfoList getWorkProgramFileList();
    QList<QString> getOpeProgramFileName();
    Q_INVOKABLE void startModbus();
    Q_INVOKABLE void stopModbus();
    void progfileMngTerminate();
    Q_INVOKABLE void modbusReadProgramList();
    Q_INVOKABLE QList<QObject *> getOperationProgramData(int taskNo);
    Q_INVOKABLE QString getProgramName(int taskNo);
    Q_INVOKABLE bool registerOpeProgramName(int progNo, QString progName);
    Q_INVOKABLE bool executeCompile();
    Q_INVOKABLE bool monitorCompileResult();
    Q_INVOKABLE bool updateOpeProgramFile(QString progName);
    Q_INVOKABLE void requestCompile();
    Q_INVOKABLE void getCompileErrorInfo();

private:
    CoreLibSdControl *mSdControl;
    ModbusManager   *m_modbusManager;
    CommunicationThread *m_communication;
    QString mProgFolderPath = AppEnvironment::getProgFolderPath();
    QString mWorkProgFolderPath = AppEnvironment::getWorkProgFolderPath();
    QFileInfoList mFileInfoList;
    QList<stProgFileInfo> mProgFileInfo;
    ModbusCommunication *mModbusCom;
    QList<QString> mProgFileNameList;
    QMap<int, QList<QObject *>> m_programDataList;
    QStringList m_programNameList;
    int mMonitorCnt;
    const int mMonitorTimeOutCnt = 120;          // 25ms*120=3000ms
    quint16 mProgCompileResult[PROG_FILE_NAME_SIZE_MAX_RG + 2];
    stProgFileModbusInfo progModbusInfo[PROG_FILE_REGISTER_NUM_MAX] = {
        { 73, 4}, { 77, 4}, { 81, 4}, { 85, 4},
        { 89, 4}, { 93, 4}, { 97, 4}, { 101, 4},
    };
    bool m_compileResult = true;
    ProgCompileErrInfo progCompileErrInfo;
    int mTimerId;
    bool mIsTimerStatus = false;
    bool    mIsModbusMonitor = false;
    int     mTimerCnt = 0;
    int     mExpectedStateCode = 0;
    int     mCmdInternalStatus = 0;
    int     mCurCtrlCode = -1;
    int     mCurStatusCode = -1;
    int     m_compileErrorStep = 0;
    bool    m_processingTimeout = false;

    bool transferProgramFileAll();
    bool transferProgramFileIndividual(QString fileName);
    bool getTransferInfo(stTranferInfo *transInfo, QFile *file);
    bool modbusWriteProgTransStartMode(int transNum, QString fileName);
    bool modbusWriteProgTranserData(int remainTransNum, int transSize, QString transData, int curTransSize);
    bool modbusWriteProgTransEndMode();
    void chgStrToAscii(QString fileName, int *writeData, int writeOffset);
    void chgStrToAscii(QString fileName, int *writeData);
    QString chgAsciiToStr(ushort readSize, quint16 *readBuf);
    int getFileListIndex(QString fileName);
    bool modbusWriteProgTranferMode(int transMode);
    bool modbusWriteProgFileName(QString *fileName);
    void readProgramName();
    void loadMatchedProgramData(QStringList matchingKeyList);
    void getCompileErrInfoAndNotify();
    void parseCompileResultAndNotify();
    void timerStart(int timeOut);
    void timerStop();
    void startModbusMonitoring();
    void stopModbusMonitoring();
    void initCtrlStatusCode();
    void modbusGetCurrentCode(ProgramFileManager::ControlCode ctrlCode, bool isFirst = true);
    void modbusGetStatusCode(ProgramFileManager::ControlCode ctrlCode, bool isFirst);
    void modbusReadCurrentCode();
    void modbusReadStatusCode();
    void modbusReadStatusCurrentCode();
    void checkStatusCode();
    void checkCompileOpeStatusCode();

protected:
    void timerEvent(QTimerEvent *timerEvent);

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command);

private slots:
    void updatePeriodResult(stModBusCommunicationData command);
    void update(stModBusCommunicationData command);

signals:
    void compileResultChanged(bool compileResult);
    void compileErrorStepChanged(int compileErrorStep);
    void processingTimeoutChanged(bool processingTimeout);
    void compileStatusCodeChanged(int compileStatusCode);
};

#endif // PROGRAMFILEMANAGER_H
