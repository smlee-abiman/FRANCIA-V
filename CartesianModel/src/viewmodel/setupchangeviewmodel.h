#ifndef SETUPCHANGEVIEWMODEL_H
#define SETUPCHANGEVIEWMODEL_H

#include <QDebug>
#include "viewmodelbase.h"
#include "enums.h"
#include "ftpmanager.h"
#include "commonviewmodel.h"
#include <QTimerEvent>
#include <QMutex>

#define SETUP_MOT_CONTROL_COMMAND_SR                1695
#define SETUP_EXECUTION_STATE_SR                    1696
#define SETUP_FILE_DIR_NUM_SR                       1699
#define SETUP_FILE_DIR_NAME_SR                      1700

#define SETUP_MOT_CONTROL_COMMAND_HR                1657
#define SETUP_FILE_DIR_NUM_HR                       1660
#define SETUP_FILE_DIR_NAME_HR                      1661
#define SETUP_FILE_ATTR_HR                          2532    // 1:教導 2:標準
#define SETUP_COMPARE_DISCORD_FACTOR_HR             2540
#define SETUP_COMPARE_DISCORD_FACTOR_SIZE           2

#define SETUP_FILE_NUMBER_MAX                       999
#define SETUP_FILE_TIMER_INTERVAL                   100
#define SETUP_FILE_DIR_NAME_SIZE                    10      // word

#define SETUP_REORDER_FTP_INTERVAL                  100
#define SETUP_REORDER_FTP_TIMEOUT_COUNT             150

#define SETUP_FILE_COMPARE_FACTOR_MAX               32
#define SETUP_FILE_COMPARE_FACTOR_RES_GROUP         51
#define SETUP_FILE_COMPARE_FACTOR_RES_START_INDEX   60

class SetupChangeViewFileList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileAttribute READ fileAttribute WRITE setFileAttribute NOTIFY fileAttributeChanged)
    Q_PROPERTY(QString fileNumber READ fileNumber WRITE setFileNumber NOTIFY fileNumberChanged)
    Q_PROPERTY(QString fileUpdateTime READ fileUpdateTime WRITE setFileUpdateTime NOTIFY fileUpdateTimeChanged)
    Q_PROPERTY(QString fileSetupName READ fileSetupName WRITE setFileSetupName NOTIFY fileSetupNameChanged)
    Q_PROPERTY(int fileViewIndex READ fileViewIndex WRITE setFileViewIndex NOTIFY fileViewIndexChanged)
public:
    explicit SetupChangeViewFileList(QObject *parent = nullptr);
    void setFileAttribute(QString fileAttribute);
    void setFileNumber(QString fileNumber);
    void setFileUpdateTime(QString fileUpdateTime);
    void setFileSetupName(QString fileSetupName);
    void setFileViewIndex(int fileViewIndex);

    QString fileAttribute() const { return m_fileAttribute; }
    QString fileNumber() const { return m_fileNumber; }
    QString fileUpdateTime() const { return m_fileUpdateTime; }
    QString fileSetupName() const { return m_fileSetupName; }
    int     fileViewIndex() const { return m_fileViewIndex; }

private:
    QString m_fileAttribute = "";
    QString m_fileNumber = "";
    QString m_fileUpdateTime = "";
    QString m_fileSetupName = "";
    int     m_fileViewIndex = 0;

signals:
    void fileAttributeChanged(QString fileAttribute);
    void fileNumberChanged(QString fileNumber);
    void fileUpdateTimeChanged(QString fileUpdateTime);
    void fileSetupNameChanged(QString fileSetupName);
    void fileViewIndexChanged(int fileViewIndex);
};

class SetupChangeViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> setupChangeViewFileList READ setupChangeViewFileList WRITE setSetupChangeViewFileList NOTIFY setupChangeViewFileListChanged)
    Q_PROPERTY(int fileListNum READ fileListNum WRITE setFileListNum NOTIFY fileListNumChanged)
    Q_PROPERTY(QString curFileNumber READ curFileNumber WRITE setCurFileNumber NOTIFY curFileNumberChanged)
    Q_PROPERTY(QString curFileName READ curFileName WRITE setCurFileName NOTIFY curFileNameChanged)
    Q_PROPERTY(bool processingResult MEMBER m_processingResult NOTIFY processingResultChanged)
    Q_PROPERTY(bool processingTimeout MEMBER m_processingTimeout NOTIFY processingTimeoutChanged)
    Q_PROPERTY(QString errCode READ errCode WRITE setErrCode NOTIFY errCodeChanged)
    Q_PROPERTY(bool updateList MEMBER m_updateList NOTIFY updateListChanged)
    Q_PROPERTY(bool requestPowerRestart MEMBER m_requestPowerRestart NOTIFY requestPowerRestartChanged)
    Q_PROPERTY(bool isNeedUpdateSetupData READ isNeedUpdateSetupData WRITE setIsNeedUpdateSetupData NOTIFY isNeedUpdateSetupDataChanged)
    Q_PROPERTY(bool isReOrderSetupDataListFinish MEMBER m_isReOrderSetupDataListFinish NOTIFY isReOrderSetupDataListFinishChanged)
    Q_PROPERTY(bool isReOrderSetupDataFinish MEMBER m_isReOrderSetupDataFinish NOTIFY isReOrderSetupDataFinishChanged)
    Q_PROPERTY(bool isFtpFileAcquired MEMBER m_isFtpFileAcquired NOTIFY isFtpFileAcquiredChanged)
    Q_PROPERTY(int compareDiscordFactor READ compareDiscordFactor WRITE setCompareDiscordFactor NOTIFY compareDiscordFactorChanged)
    Q_PROPERTY(int dandoriListUpdate READ dandoriListUpdate WRITE setDandoriListUpdate NOTIFY dandoriListUpdateChanged)
    Q_PROPERTY(bool isSetupProcActive MEMBER m_isSetupProcActive NOTIFY isSetupProcActiveChanged)
    Q_PROPERTY(bool notifyingProcessResult READ notifyingProcessResult WRITE setNotifyingProcessResult NOTIFY notifyingProcessResultChanged)

    Q_PROPERTY(QList<QString> compareDetailResult MEMBER m_compareDetailResult NOTIFY compareDetailResultChanged)
    Q_PROPERTY(QList<QString> compareDetailResultColor MEMBER m_compareDetailResultColor NOTIFY compareDetailResultColorChanged)
public:
    enum StatusCode {
        CmdWait                     = 0,
        CmdSuccess1                 = 1,
        CmdReadSuccess              = 15,
        CmdWriteSaveSuccess         = 16,
        CmdCompareSuccess           = 17,
        CmdEraseSuccess             = 18,
        CmdSuccess41                = 41,
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
        CmdNotSupported             = 254,             // 0xFE
        CmdExe                      = 255,             // 0xFF
    };
    enum ControlCode {
        SetupFileRead               = 15,
        SetupFileWriteSave,
        SetupFileCompare,
        SetupFileErase,
    };
    enum InternalStatus {
        CurCmdConfirmBefore         = 0,
        CmdExeStart                 = 1,
        CmdExecuted                 = 2,
        CmdExeError                 = 3,
        CmdExeSuccess               = 4,
        CmdExePostProc              = 5,
    };
    enum DandoriFileStatus {
        NoOperation                 = -1,
        FileInit,
        FileSave,
        FileErase,
        FileDownload,
    };
    explicit SetupChangeViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    explicit SetupChangeViewModel(QObject *object = nullptr);
    ~SetupChangeViewModel();
    void initSetupChangeView();

    Q_INVOKABLE void startFtpCommunication();
    Q_INVOKABLE void stopFtpCommunication();
    Q_INVOKABLE void getFtpFileList();
    Q_INVOKABLE void getFtpFileInfo();
    Q_INVOKABLE void getFtpFileData(bool isReOrder = false);
    Q_INVOKABLE int getSetupMngFileNumMax() { return mSetupMngFileNumMax; }
#if 0
    Q_INVOKABLE int getTabDispSetting(int tabIndex, int userLevel);
#endif
    Q_INVOKABLE int getCmdButtonDispSetting(int btnIndex, int userLevel);
    Q_INVOKABLE void sortFileNumber(int sortMode);
    Q_INVOKABLE void sortFileName(int sortMode);
    Q_INVOKABLE void sortFileUpdateTime(int sortMode);
    Q_INVOKABLE QString searchFreeFileNumber(QString searchStartNumber);
    Q_INVOKABLE void modbusGetCurFileNumberAndName();
    Q_INVOKABLE void startModbusOneShot();
    Q_INVOKABLE void stopModbusOneShot();
    void setSetupChangeViewFileList(QList<QObject *> setupChangeViewFileList);
    QList<QObject *> setupChangeViewFileList() const { return m_setupChangeViewFileList; }
    void setFileListNum(int fileListNum);
    int fileListNum() const { return m_fileListNum; }
    void setCurFileNumber(QString curFileNumber);
    QString curFileNumber() const { return m_curFileNumber; }
    void setCurFileName(QString curFileName);
    QString curFileName() const { return m_curFileName; }
    void setErrCode(QString errCode);
    QString errCode() const { return m_errCode; }

    Q_INVOKABLE int readSetupFile(int listIndex);
    Q_INVOKABLE int writeSetupFile(int listIndex);
    Q_INVOKABLE int saveSetupFile(QString fileNumber, QString fileComment, int fileAttrIndex);
    Q_INVOKABLE int compareSetupFile(int listIndex);
    Q_INVOKABLE int eraseSetupFile(int listIndex);

    Q_INVOKABLE QString getSelectedTagColor() { return m_setupSelectedTagRGB; }
    Q_INVOKABLE QString getCommandPressColor() { return m_setupCommandPressRGB; }
    Q_INVOKABLE QString getFileContentDispColor() { return m_setupFileContentDispRGB; }
    Q_INVOKABLE QString getSelectedFileContentDispColor() { return m_setupSelectedFileContentDispRGB; }
    Q_INVOKABLE QString getPushedFileContentDispColor() { return m_setupPushedFileContentDispRGB; }
    Q_INVOKABLE void timerStop(int mode = 0);
    Q_INVOKABLE bool checkFileNumber(int fileNumber);
    void setIsNeedUpdateSetupData(bool isNeedUpdateSetupData);
    bool isNeedUpdateSetupData() const { return m_isNeedUpdateSetupData; }
    Q_INVOKABLE void getFtpFileInfoAndData();
    void modbusGetCompareDiscordFactors();
    void getCompareDiscordFactorsAndNotify();
    void setDandoriListUpdate(int dandoriListUpdate);
    int dandoriListUpdate() const { return m_dandoriListUpdate; }
    void setCompareDiscordFactor(int compareDiscordFactor);
    int compareDiscordFactor() const { return m_compareDiscordFactor; }
    void setNotifyingProcessResult(bool notifyingProcessResult);
    bool notifyingProcessResult() const { return m_notifyingProcessResult; }
    Q_INVOKABLE void clearCompareDetailResult();

private:
    QList<QObject *>    m_setupChangeViewFileList;
    QList<QObject *>    m_saveSetupChangeViewFileList;
    CsvReader           *mCsvReader;
    FtpManager          *mFtpManager;
    CommonViewModel     *m_commonView = nullptr;
    QFile               *mDandoriFile;
    int                 mSetupMngFileNumMax = 999;
    int                 m_fileListNum = 0;
    QString             m_setupSelectedTagRGB;
    QString             m_setupCommandPressRGB;
    QString             m_setupFileContentDispRGB;
    QString             m_setupSelectedFileContentDispRGB;
    QString             m_setupPushedFileContentDispRGB;
    //QList<QList<int>>   m_userLevelTabDispSettings;
    QList<QList<int>>   m_userLevelCmdButtonDispSettings;
    int                 mExpectedStateCode = 0;
    int                 mCmdInternalStatus = 0;
    int                 mTimerId[2];
    bool                mIsTimerStatus = false;
    int                 mTimerCnt = 0;
    QString             m_curFileNumber = "";
    QString             m_curFileName = "";
    bool                m_processingResult = false;
    bool                m_processingTimeout = false;
    bool                mProcessingSave = false;
    bool                mIsModbusMonitor = false;
    int                 mCurCtrlCode = -1;
    int                 mCurStatusCode = -1;
    QString             m_errCode = "";
    int                 mEraseListIndex = -1;
    bool                m_updateList = false;
    QMutex              *mMutexSetupSort;
    bool                mIsMutexSetupSort = false;
    bool                m_requestPowerRestart = false;
    bool                m_isNeedUpdateSetupData = false;
    bool                m_isFtpGetFinish = false;
    bool                m_isFtpGetError = false;
    int                 mReOrderTimerCnt = 0;
    bool                m_isReOrderSetupDataListFinish = false;
    bool                m_isReOrderSetupDataFinish = false;
    bool                m_isFtpFileAcquired = false;
    int                 mCtrlCodeProcTimeout = 0;
    int                 mCtrlCodeProcTimeoutCnt = 0;
    int                 m_compareDiscordFactor = -1;
    int                 m_dandoriListUpdate = 0;
    QString             mSaveDandoriFileName = "";
    QString             mEraseDandoriFileName = "";
    bool                m_isSetupProcActive = false;
    bool                m_notifyingProcessResult = false;
    QList<QString>      m_compareDetailResult;
    QList<QString>      m_compareDetailResultColor;
    bool                m_isRequestGetDandoriData = false;

    void initSetupChangeViewFileList();
    void initSettingPenSetData();
    void getModbusAsciiData(QList<int> &writeData, QString writeStr);
    void parseDandoriFile(QString filePath);
    void sortAscendOrderFileNumber();
    void sortDescendOrderFileNumber();
    void sortAscendAlphabetOrderFileName();
    void sortDescendAlphabetOrderFileName();
    void sortOldestTimeOrderFileUpdateTime();
    void sortNewTimeOrderFileUpdateTime();
    int searchFromBeginningFileNumber(int startNumber);
    void timerStart(int timeOut, int mode = 0);
    void checkReadOpeStatusCode();
    void checkWriteOpeStatusCode();
    void checkCompareOpeStatusCode();
    void checkEraseOpeStatusCode();
    void checkSaveOpeStatusCode();
    void checkStatusCode();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void modbusGetStatusCode(SetupChangeViewModel::ControlCode ctrlCode, bool isFirst = false);
    void modbusGetCurrentCode(SetupChangeViewModel::ControlCode ctrlCode, bool isSave, bool isFirst = true);
    void modbusGetSetupFileInfo();
    void updateSetupFileNumberAndName(stModBusCommunicationData &command);
    void updateSetupFileInfo();
    void startModbusMonitoring();
    void stopModbusMonitoring();
    void modbusReadStatusCode();
    void modbusReadCurrentCode();
    void modbusReadStatusCurrentCode();
    void initCtrlStatusCode();
    void checkErrCodeAndNotify(int errCode);
    void holdSaveSetupFile(QString fileNumber, QString fileComment, QString fileAttr);
    void updateSetupFile();
    void holdEraseSetupFile(int listIndex);
    void updateEraseSetupFile();
    void addSetupFile();
    void lockSortMutex();
    void unlockSortMutex();
    void notifySetupFileList();
    void checkReOrderFtpStatus();
    void getCtrlCodeProcTimeout();
    void updateDandoriFile(QString fileNumber, QString fileSize, QString lastModified);
    bool parseCompareDetailResult(int compareDiscordFactor);

protected:
    void timerEvent(QTimerEvent *timerEvent) override;
    void onActivate() override;
    void onDeactivate() override;

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void onFtpGetFinished(const QString &filePath);

private slots:
    void update(stModBusCommunicationData command);
    void onFtpStatusChanged(int ftpStatus);
    void onFtpErrorStrNotify(QString errorStr);
    void onFtpFileInfoChanged(int ftpFileInfo);
    void onUpdateSingleTimer();
    void onFtpRemoteFileInfoChanged(bool ftpRemoteFileInfo);

signals:
    void setupChangeViewFileListChanged(QList<QObject *> setupChangeViewFileList);
    void fileListNumChanged(int fileListNum);
    void curFileNumberChanged(QString curFileNumber);
    void curFileNameChanged(QString curFileName);
    void processingResultChanged(bool processingResult);
    void processingTimeoutChanged(bool processingTimeout);
    void errCodeChanged(QString errCode);
    void updateListChanged(bool updateList);
    void requestPowerRestartChanged(bool requestPowerRestart);
    void isNeedUpdateSetupDataChanged(bool isNeedUpdateSetupData);
    void isReOrderSetupDataListFinishChanged(bool isReOrderSetupDataListFinish);
    void isReOrderSetupDataFinishChanged(bool isReOrderSetupDataFinish);
    void isFtpFileAcquiredChanged(bool isFtpFileAcquired);
    void compareDiscordFactorChanged(int compareDiscordFactor);
    void dandoriListUpdateChanged(int dandoriListUpdate);
    void isSetupProcActiveChanged(bool isSetupProcActive);
    void notifyingProcessResultChanged(bool notifyingProcessResult);
    void compareDetailResultChanged(QList<QString> m_compareDetailResult);
    void compareDetailResultColorChanged(QList<QString> m_compareDetailResultColor);
};

#endif // SETUPCHANGEVIEWMODEL_H
