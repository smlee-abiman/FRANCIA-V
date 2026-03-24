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
#include "modbuscommandgenerator.h"
#include <QMutex>

Q_DECLARE_LOGGING_CATEGORY(sharedcontrol)

class CommunicationThread : public QThread
{
    Q_OBJECT
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
        CallerFunctionMachineDrvParamSimpleSetPageModel,
        CallerMonitorIOForcePageModel,
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
    void setModbusInfo(QString aplInfo, int portNo, int serverAddress, QString ipAddr);
    void setIntervalMiliSec(int msec);
    void setRequestPage(Enums::Pages page);
    void setOneShotReadCommand(stModBusCommunicationData &command);
    void setModbusWriteCommand(stModBusCommunicationData &command);
    void setOneShotModbusCaller(OneShotReadModBusCaller caller);
    void setOneShotModbusCommand(stModBusCommunicationData &command);
    void setPageReadCommandList(Enums::Pages page, stModBusCommunicationData &command);
    void resetPageReadCommandList(Enums::Pages page);
    //PhyKeyManager *getPhyKeyManagerInstance();
    void run();
    void writeModbusDO(int doNumber, bool isOn);
    void writeModbusDO(int doNumber, bool isOn, uchar anyValue);
    bool writeModbusHR(int HrNumber, int *write_data, int write_size);
    bool writeModbusHR(int HrNumber, QString writeDataStr);
    int getModbusConnectStatus();

signals:
    // Feed data that was comming from controller. (Data is ready from controller)
    void feed(stModBusCommunicationData command);
    void oneShotReadFinished(stModBusCommunicationData command);
    void oneShotReadFinishedIndividualSignal(stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void finish();

private slots:
    void onModBusComConnectStatusChanged(int state);
    void onTimeout();  // Fetch data from the controller

private:
    void initialize();
    void appendCommonModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendPageByPageModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendOneShotReadCommand(QList<stModBusCommunicationData> *commandList);
    void appendWriteModbusCommand(QList<stModBusCommunicationData> *commandList);
    void appendOneShotModbusCommand(QList<stModBusCommunicationData> *commandList);
    void notifyOneShotModbusRead(stModBusCommunicationData command);
    stModBusInitInfo m_modbusInitInfo;
    int m_intervalMiliSec;
    int m_modBusComConnectStatus;
    CoreLibModBusCommunication *m_modbus;
    Enums::Pages m_requestPage = Enums::Pages::InvalidPage;
    QMutex m_oneShotModbusCommandListMutex; // mutex for m_oneShotModbusCommandList access

    //PhyKeyManager *m_phykeymanager;
    OneShotReadModBusCaller oneShotReadCaller = CallerDefault;

public:
    QList<stModBusCommunicationData> m_pageReadCommandList;
    QList<stModBusCommunicationData> m_oneShotModbusCommandList;
    QList<stModBusCommunicationData> m_oneShotReadCommandList;
    QList<stModBusCommunicationData> m_modbusWriteCommandList;
    QMap<quint16, uchar> m_writeModbusDOMap;
    QMap<quint16, uchar> m_writeModbusDOucharMap;
};

#endif // COMMUNICATIONTHREAD_H
