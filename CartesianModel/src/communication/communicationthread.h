/***
 *
 * This is a thread class to communicate with another process ,which is to communicate with MOT,
 * and transfer the data to Qt application
 * The communication is via sharedmemory. Runs a loop in 25ms interval.
 */
#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H

#include <QThread>
#include <QTimer>
#include <QLoggingCategory>
#include "corelibmodbuscommunication.h"
#include <QList>
#include "enums.h"
#include "phykeymanager.h"
#include "modbuscommandgenerator.h"
#include <QMutex>
#include <QTimerEvent>
#include "ftpmanager.h"

Q_DECLARE_LOGGING_CATEGORY(sharedcontrol)

#define JUDGE_NOTIFY_TIMEOUT_COUNT    2

typedef struct
{
    QList<stModBusCommunicationData> commandList;
} stDelayModbusDataSet;

typedef struct
{
    int retryCount;
    stModBusCommunicationData modbusData;
} stModBusCommunicationRetryData;

class CommunicationThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(int modBusComConnectStatus MEMBER m_modBusComConnectStatus NOTIFY modBusComConnectStatusChanged)
private:
    explicit CommunicationThread(QObject *parent = 0);
    ~CommunicationThread() = default;

    static CommunicationThread* instance;
public:
    enum OneShotReadModBusCaller {
        CallerDefault = 0,
        CallerProgFileMng,
        CallerPointDataPageModel,
        CallerProgVarPageModel,
        CallerControllerPageModel,
        CallerMachineParamPageModel,
        CallerAmpMonitorPageModel,
        CallerSetupChgOpePageModel,
        CallerTeachingViewModel,
        CallerManualAxisOpeViewModel,
        CallerTeachingPosSetPageModel,
        CallerTeachingStackPageModel,
        CallerTeachingTimerPageModel,
        CallerTeachingCounterPageModel,
        CallerTeachingPassPageModel,
        CallerTeachingProgramPageModel,
        CallerFunctionControllerSettingSystemSetPageModel,
        CallerFunctionControllerSettingUserSetPageModel,
        CallerFunctionControllerSettingNetworkSetPageModel,
        CallerFunctionMachineDrvParamSimpleSetPageModel,
        CallerFunctionAreaSetPageModel,
        CallerFunctionMaintainPageModel,
        CallerFunctionVersionPageModel,
        CallerMonitorIOForcePageModel,
        CallerSetupChangePageModel,
    };
    static CommunicationThread* create_instance(QObject *parent = 0) {
        if(!instance){
            instance = new CommunicationThread(parent);
        }
        return instance;
    }

    static CommunicationThread* get_instance() {
        return instance;
    }

    static void destroy_instance() {
        delete instance;
        instance = nullptr;
    }
    void setModbusInfo(QString aplInfo, int portNo, int serverAddress, QString ipAddr, int timeout, int retry);
    void setIntervalMiliSec(int msec);
    void setRequestPage(Enums::Pages page);
    void setOneShotReadCommand(stModBusCommunicationData &command);
    void setModbusWriteCommand(stModBusCommunicationData &command);
    void setOneShotModbusCaller(OneShotReadModBusCaller caller);
    void setOneShotModbusCommand(stModBusCommunicationData &command);
    void setPageReadCommandList(Enums::Pages page, stModBusCommunicationData &command);
    void setJogModbusCommand(stModBusCommunicationData &command);
    void resetPageReadCommandList(Enums::Pages page);
    void removePageReadCommandList(Enums::Pages page, int index);
    void setCommonPageReadCommandList(stModBusCommunicationData &command);
    void resetCommonPageReadCommandList();
    void setDelayCommandList(Enums::Pages page, stModBusCommunicationData &command);
    void setDelayCommandList(Enums::Pages page, stModBusCommunicationData &writeCommand, stModBusCommunicationData &readCommand);
    PhyKeyManager *getPhyKeyManagerInstance();
    void run();
    void writeModbusDO(int doNumber, bool isOn);
    void writeModbusDO(int doNumber, bool isOn, uchar anyValue);
    void writeModbusDO(int doNumber, uchar anyValue, int write_size);
    bool writeModbusHR(int HrNumber, int *write_data, int write_size);
    bool writeModbusHR(int HrNumber, QString writeDataStr);
    void writeModbusHR(quint16 addr, int value);
    void readModbusDI(CommunicationThread::OneShotReadModBusCaller caller, quint16 addr, quint16 size);
    void readModbusHR(CommunicationThread::OneShotReadModBusCaller caller, quint16 addr, quint16 size);
    void setOneShotModbusCommand(CommunicationThread::OneShotReadModBusCaller caller, stModBusCommunicationData &data);

    int getModbusConnectStatus();
    void reConnect();
    void invalidCommonModbusCmd();
    void timerStart(int timeOut);
    void timerStop();
	void updateLinkLed(bool inonoff);
    void initIndividualPageModbusTimeOutVariable(){
        mIndividualPageModbusTimeOutCnt = 0;
        mIsIndividualPageModbusTimeOutNotify = false;
    }

signals:
    // Feed data that was comming from controller. (Data is ready from controller)
    void feed(stModBusCommunicationData command);
    void oneShotReadFinished(stModBusCommunicationData command);
    void oneShotReadFinishedIndividualSignal(stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void finish();
    void timeout(int communicationAbnormalStatus);
    void timeoutError();
    void modBusComConnectStatusChanged(int modBusComConnectStatus);
	void linkStateChange(bool isonoff);
    void replyAbortedError();

private slots:
    void onModBusComConnectStatusChanged(int state);
    void onTimeout();  // Fetch data from the controller
    void onModBusComReadStatusChanged(int modBusReadState);

public slots:
    void onUpdateJogCounterInfo(int jogCounter);
    void onUpdateOperableSwInfo(bool operableSwInfo);
    void onUpdateReleaseOperableSwInfo(bool operableSwReleaseInfo);

private:
    void initialize();
    void appendCommonModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendPageByPageModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendOneShotReadCommand(QList<stModBusCommunicationData> *commandList);
    void appendWriteModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendOneShotModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendPageByCommonModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendDelayCommonModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendJogModbusCommand(QList<stModBusCommunicationData> *commandList);

    bool syncReadCommandList(QList<stModBusCommunicationData> readCommandList);
    bool syncOneShotReadCommandList(QList<stModBusCommunicationData> oneShotReadCommandList);
    bool syncWriteCommandList(QList<stModBusCommunicationData> writeCommandList);
    bool syncOneShotModBusCommandList(QList<stModBusCommunicationData> oneShotModbusCommandList);
    bool syncReadDebugCommandList(QList<stModBusCommunicationData> readCommandList);
    bool syncOneShotReadDebugCommandList(QList<stModBusCommunicationData> oneShotReadCommandList);
    bool syncWriteDebugCommandList(QList<stModBusCommunicationData> writeCommandList);
    bool syncOneShotModBusDebugCommandList(QList<stModBusCommunicationData> oneShotModbusCommandList);

    void appendRetryCommandList(QList<stModBusCommunicationData> *destList, QList<stModBusCommunicationRetryData> *srcList);
    void checkRetryCommandList(QList<stModBusCommunicationRetryData> *commandList);
    void appendNewRetryCommandList(QList<stModBusCommunicationRetryData> *destList, QList<stModBusCommunicationData> *srcList);

    void makeJogKeyReleaseCommand();
    bool judgeComStatusNotify();
    bool judgeSeriousComStatus();
    bool judgeSeriousSubjectScreen();
    void judgeModbusConnectState(bool isonoff);

    stModBusInitInfo m_modbusInitInfo;
    int m_intervalMiliSec;
    int m_modBusComConnectStatus;
    CoreLibModBusCommunication *m_modbus;
    Enums::Pages m_requestPage = Enums::Pages::InvalidPage;
    QMutex m_modbusWriteCommandListMutex; // mutex for m_modbusWriteCommandList access
    QList<stModBusCommunicationData> m_modbusWriteCommandList;
    QMutex m_oneShotReadCommandListMutex; // mutex for m_oneShotReadCommandList access
    QList<stModBusCommunicationData> m_oneShotReadCommandList;
    QMutex m_oneShotModbusCommandListMutex; // mutex for m_oneShotModbusCommandList access
    QList<stModBusCommunicationData> m_oneShotModbusCommandList;
    PhyKeyManager *m_phykeymanager;
    OneShotReadModBusCaller oneShotReadCaller = CallerDefault;
    QMutex m_pageReadCommandListMutex;
    QList<stModBusCommunicationData> m_pageReadCommandList[Enums::Pages::pageMax];
    QMutex m_commonPageReadCommandListMutex;
    QList<stModBusCommunicationData> m_commonPageReadCommandList;
    QMutex m_delayCommandListMutex;
    QMap<Enums::Pages, QList<stDelayModbusDataSet>> m_delayCommandList;
    QMutex m_jogCommandMutex;
    stModBusCommunicationData m_jogCommand;
    stModBusCommunicationData m_jogReleaseCommand;
    bool m_jogCommandIsWrote = true;
    bool m_jogKeyReleaseCommandSendRequired = false;
    int m_jogKeyReleaseCommandSendCounter = 0;

    QList<stModBusCommunicationRetryData> m_retryWriteCommandList;
    QList<stModBusCommunicationRetryData> m_retryOneShotReadCommandList;
    QList<stModBusCommunicationRetryData> m_retryOneShotModbusCommandList;

    bool retryDlgShowed = false;
    int connectRetry = 150;
    int m_connectRetryCount = 0;
    bool mIsCommonModbusCmd = true;
    int mModbusTimeOutCnt = 0;
    int mTimerId;
    bool mIsTimerStatus = false;
    int m_commandRetryMax = 10;
    bool m_modbusConnectState = true;
	bool m_greenLedState = true;

    FtpManager  *m_ftpManager = nullptr;
    bool mIsAlreadyFtpGetList = false;
    int mComThreadFtpStatusCnt = 0;
    bool mIsAlreadyFtpGetDandoriList = false;
    FtpManager::FtpFileInfo mAlreadyFtpGetFile = FtpManager::FtpFileInfo::FTP_FILE_INFO_MAX;
    int mIndividualPageModbusTimeOutCnt = 0;
    bool mIsIndividualPageModbusTimeOutNotify = false;
    int m_modBusReadState = 0;

protected:
    void timerEvent(QTimerEvent *timerEvent);
};

#endif // COMMUNICATIONTHREAD_H
