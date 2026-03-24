#ifndef MODBUSCOMMUNICATION_H
#define MODBUSCOMMUNICATION_H

#include <QObject>
#include <QDebug>
#include <QList>
#include "corelibsdcontrol.h"
#include "corelibmodbuscommunication.h"
#include "modbuscommandgenerator.h"

class ModbusCommunication : public QObject
{
    Q_OBJECT
private:
    explicit ModbusCommunication(QObject *parent = 0);
    ~ModbusCommunication() = default;

    static ModbusCommunication* instance;
public:
    static ModbusCommunication* create_instance(QObject *parent = 0) {
        if(!instance){
            instance = new ModbusCommunication(parent);
        }
        return instance;
    }

    static ModbusCommunication* get_instance() {
        return instance;
    }

    static void destroy_instance() {
        delete instance;
        instance = nullptr;
    }
    void modbusCommunicationInit(stModBusInitInfo& modbusInitInfo);
    void modbusCommunicationTerminate();
    void modbusCommunicationStart();
    void syncModbusCommunicationStart();
    void syncModbusCommunicationStop();
    void modbusCommunicationStop();
    int getModbusConnectStatus();
    bool writeModbusData(stModBusCommunicationData &command);
    bool readModbusData(stModBusCommunicationData &command);
    bool modbusWriteDO(int doNumber, bool isOn);
    bool asyncModbusWriteDO(int doNumber, uchar anyValue, int write_size);
    bool modbusWriteHR(int HrNumber, int write_data);
    bool modbusWriteHR(int HrNumber, int *write_data, int write_size);
    bool modbusWriteHR(int HrNumber, QString writeDataStr);
    bool modbusReadDI();
    bool modbusReadHR(int HrNumber, int read_size, stModBusCommunicationData* modbusComData);
    bool modbusReadRG(int RgNumber, int read_size, stModBusCommunicationData* modbusComData);


private:
    stModBusInitInfo mModbusInitInfo;
    CoreLibModBusCommunication *mModbusCom;
    bool mIsModbusConnect;
    int mModBusComConnectStatus;

    void initialize();

private slots:
    void onModBusComConnectStatusChanged(int state);

signals:

};

#endif // MODBUSCOMMUNICATION_H
