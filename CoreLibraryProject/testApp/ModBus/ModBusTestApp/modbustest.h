#ifndef MODBUSTEST_H
#define MODBUSTEST_H

#include <QObject>
#include <QDebug>
#include "corelibmodbuscommunication.h"

class ModBusTest : public QObject
{
    Q_OBJECT
public:
    explicit ModBusTest(QObject *parent = nullptr);

public slots:
    void connectSlot();
    void readBitSlot();
    void readByteSlot();
    void readWordSlot();
    void readLongSlot();
    void readStringSlot();
    void writeBitSlot();
    void writeByteSlot();
    void writeWordSlot();
    void writeLongSlot();
    void writeStringSlot();
    void disconnectSlot();
    void initializeSlot();
    void syncConnectSlot();
    void syncDisconnectSlot();

private:
    CoreLibModBusCommunication *mCoreLibModBus;
    stModBusInitInfo mModBusInitInfo;
    stModBusCommunicationData mModBusComData;
    void modBusReadConnect();

public slots:
    void modBusConnectStatusChanged(int state);
    void modBusReadStatusChanged(int state);
    void modBusBitMemoryDataChanged(QBitArray bitMemoryData);
    void modBusMemoryDataChanged(stModBusCommunicationData modBusMemData);
};

#endif // MODBUSTEST_H
