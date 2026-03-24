#ifndef CORELIBMODBUSCOMMUNICATION_H
#define CORELIBMODBUSCOMMUNICATION_H

#include <QModbusTcpClient>
#include <QUrl>
#include <QTimer>
#include <unistd.h>
#include "corelibmodbusdatamanager.h"
#include "corelibmemorydatamanager.h"
#include <QThread>
#ifdef CORELIB_UNIT_TEST
#include "corelibunittestmockmethod.h" 
#endif // CORELIB_UNIT_TEST
#ifdef REAL_TARGET
#else // REAL_TARGET
#include <QDir>
#endif // REAL_TARGET

//#define GET_POINT_MEMORY_HEADER_INFO_FROM_CORELIB

#define FUNCCODE01_BIT_MEMORY_MIN_ADDR  0x0000
#define FUNCCODE01_BIT_MEMORY_MAX_ADDR  0x1600
#define FUNCCODE02_BIT_MEMORY_MIN_ADDR  0x0000
#define FUNCCODE02_BIT_MEMORY_MAX_ADDR  0x0700
#define FUNCCODE04_BIT_MEMORY_MIN_ADDR  0x0000
#define FUNCCODE04_BIT_MEMORY_MAX_ADDR  0x0240
#define MODBUS_ERR_COUNT_STR_SIZE       5
#define MODBUS_ERR_COUNT_MAX(digit) \
    (digit == 1 ? 9 : \
     digit == 2 ? 99 : \
     digit == 3 ? 999 : \
     digit == 4 ? 9999 : \
     digit == 5 ? 99999: 99999)
#define GET_MODBUS_ERR_COUNT_DIGIT(error_count) \
    ((error_count / 10) == 0 ? 1 : \
     (error_count / 100) == 0 ? 2 : \
     (error_count / 1000) == 0 ? 3 : \
     (error_count / 10000) == 0 ? 4 : 5)
#define GET_POINT_MEMORY_INFO(index, data) \
    ((index == 0) ? ((data & 0x000F) >> (index * 4)) : \
     (index == 1) ? ((data & 0x00F0) >> (index * 4)) : \
     (index == 2) ? ((data & 0x0F00) >> (index * 4)) : \
     (index == 3) ? ((data & 0xF000) >> (index * 4)) : -1)
#define POINT_HEADER_INFO_TO_SIZE(header) \
    ((header == 0x0) ? 5960 : \
     (header == 0x1) ? 0x10000 : \
     (header == 0x2) ? 0x20000 : \
     (header == 0x3) ? 0x30000 : \
     (header == 0x4) ? 0x40000 : \
     (header == 0x5) ? 0x50000 : \
     (header == 0x6) ? 0x60000 : \
     (header == 0x7) ? 0x70000 : \
     (header == 0x8) ? 0x80000 : 5960)

typedef struct {
    QString aplInfo;
    QString serverIPAddress;
    int     portNum;
    int     serverAddress;
    int     timeout;
    int     retry;
} stModBusInitInfo;

class CoreLibModBusCommunication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int modBusConnectState READ modBusConnectStatus NOTIFY modBusComConnectStatusChanged)
    Q_PROPERTY(int modBusReadState READ modBusReadStatus NOTIFY modBusComReadStatusChanged)
    Q_PROPERTY(int modBusWriteState READ modBusWriteStatus NOTIFY modBusComWriteStatusChanged)
public:
    CoreLibModBusCommunication(stModBusInitInfo&);
    ~CoreLibModBusCommunication();
#ifdef CORELIB_UNIT_TEST
    CoreLibUnitTestMockMethod& getUnitTestMockMethod()
    {
        return mMockMethod;
    }
#endif // CORELIB_UNIT_TEST
    enum AplIndexInfo {
        APL_INDEX_INFO_ROBOT1 = 0,
        APL_INDEX_INFO_ROBOT2,
        APL_INDEX_INFO_ROBOT3,
        APL_INDEX_INFO_ROBOT4,
        APL_INDEX_INFO_MAX,
    };
    void startModBusCommunication();
    bool syncStartModBusCommunication();
    void stopModBusCommunication();
    bool syncStopModBusCommunication();
    bool rawReadRequestModBusData(stModBusCommunicationData&);
    bool rawWriteRequestModBusData(stModBusCommunicationData&);
    bool readModBusData(stModBusCommunicationData&);
    bool writeModBusData(stModBusCommunicationData&);
    bool readWriteModBusData(stModBusCommunicationData&);
    bool checkModBusBitMemory(stModBusCommunicationData&);
    bool checkModBusBitMemory(QModbusPdu::FunctionCode funcCode);
    ulong getModBusErrorCount();
    int modBusConnectStatus() {
        return m_modBusConnectState;
    }
    int modBusReadStatus() {
        return m_modBusReadState;
    }
    int modBusWriteStatus() {
        return m_modBusWriteState;
    }
    bool syncReadModBusData(stModBusCommunicationData* modBusCommunicationData);
    bool syncWriteModBusData(stModBusCommunicationData* modBusCommunicationData);
    bool syncReadModBusDataUnit(stModBusCommunicationData* modBusCommunicationData, QModbusDataUnit *dataUnit);
    bool syncReadWriteModBusData(stModBusCommunicationData* modBusCommunicationData);

    bool syncReadModBusDebugData(stModBusCommunicationData* modBusCommunicationData);
    bool syncWriteModBusDebugData(stModBusCommunicationData* modBusCommunicationData);
    bool syncReadWriteModBusDebugData(stModBusCommunicationData* modBusCommunicationData);

private:
    CoreLibModBusDataManager *mModBusDataMgr;
    CoreLibMemoryDataManager *mMemoryDataMgr = CoreLibMemoryDataManager::get_instance();
    QModbusTcpClient *mModBusDevice;
    QString mServerUrl;
    int mModBusTimeout = 450;
    int mModBusRetryCnt = 1;
    int mModBusServerAddress;
    QString mAplInfo;
    bool mIsBitMemory;
    int m_modBusConnectState;
    int m_modBusReadState;
    int m_modBusWriteState;
    ulong mModBusErrCount;
    QFile *mModBusErrCountFile;
#ifdef REAL_TARGET
    const QString mHomePath = "/home/root";
#else // REAL_TARGET
    const QString mHomePath = QDir::homePath();
#endif // REAL_TARGET
    const QString mModBusErrCountFileName = "modbus_err_count";
#ifdef REAL_TARGET
#else // REAL_TARGET
    QString mModBusAplPath;
#endif // REAL_TARGET
    QString mModBusBasePath;
    QString mModBusErrCountFilePath;
    QString mModBusErrCountInitValue;
    const ushort mPointMemoryDataHeaderAddress = 0x3320;
    const ushort mPointMemoryDataHeaderSize = 1;
    const int mPointMemoryDataMaxLimitSize = 0xB0000;
    QModbusReply *mModbusResult;
    QModbusDevice::State mModBusStatus;
    const int mModBusConnectTickCnt = 10;
    const uchar mMaxModBusConnectTimeOutCnt = 40;
    const int mModBusDisConnectTickCnt = 10;
    const uchar mMaxModBusDisConnectTimeOutCnt = 40;
#ifdef REAL_TARGET
#else // REAL_TARGET
    QDir *mQDir;
#endif // REAL_TARGET

    QModbusReply* modBusRawRequest(QModbusRequest&);
    QModbusReply* modBusReadRequest();
    QModbusReply* modBusWriteRequest();
    QModbusReply* modBusReadWriteRequest();
    void connectModBusDevice();
    bool syncConnectModBusDevice();
    void disconnectModBusDevice();
    bool syncDisconnectModBusDevice();
    bool readyModBusDataUnit(stModBusCommunicationData&, CoreLibModBusDataManager::ModbusRequestType requestType);
    void setModBusDataUnit(QModbusDataUnit &memoryDataUnit, stModBusCommunicationData &modBusCommunicationData);
    void setModBusDataUnitForReadWrite(QModbusDataUnit &memoryDataUnit, stModBusCommunicationData &modBusCommunicationData);
    bool checkModBusCommunicationSize(stModBusCommunicationData&);
    bool checkModBusWriteCommunicationSize(stModBusCommunicationData&);
    void updateModBusErrCount();
    void readyModBusErrorCountFile(QString&);
    void readPointMemoryHeaderData();
//    void parsePointMemorySize(QModbusDataUnit&);
    int getPointMemorySize(short headerInfo);

protected:
#ifdef CORELIB_UNIT_TEST
    CoreLibUnitTestMockMethod mMockMethod;
#endif // CORELIB_UNIT_TEST

private slots:
    void onStateChanged(int state);
    void modBusReadReady();
    void modBusAnyReadyForSync();
    void modBusWriteReady();
    void modBusReadWriteReady();
    void modBusRawReadReady();
    void modBusRawWriteReady();

signals:
    void modBusComConnectStatusChanged(int m_modBusConnectState);
    void modBusComReadStatusChanged(int m_modBusReadState);
    void modBusComWriteStatusChanged(int m_modBusWriteState);
    void modBusReplyArrivedForSync();
};

#endif // CORELIBMODBUSCOMMUNICATION_H
