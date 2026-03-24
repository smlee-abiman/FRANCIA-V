#ifndef MODBUSCOMMANDGENERATOR_H
#define MODBUSCOMMANDGENERATOR_H

#include "corelibmodbuscommunication.h"

typedef struct
{
    QModbusPdu::FunctionCode    funcCode;
    quint16                     address;
    quint16                     size;
}stModbusPeridiocMonitor;

class ModbusCommandGenerator
{
public:
    static stModBusCommunicationData makeModbusReadCommand(
        QModbusPdu::FunctionCode funcCode, quint16 address, quint16 size, bool isBulkMode = false, bool isSummaryBulkMode = false)
    {
        stModBusCommunicationData data;
        data.funcCode = funcCode;
        data.readAddress = address;
        data.comSize = size;
        data.isBulkMode = isBulkMode;
        data.isSummaryBulkMode = isSummaryBulkMode;
        for(int i = 0; i < MODBUS_COMMUNICATION_DATA_MAX; i++) {
            data.readBuf[i] = 0;  // zero clear
        }
        return data;
    }

    static stModBusCommunicationData makeModbusWriteCommand(QModbusPdu::FunctionCode funcCode, quint16 address, quint16 size, QList<int> writeData)
    {
        stModBusCommunicationData data;
        data.funcCode = funcCode;
        data.writeAddress = address;
        data.comSize = size;
        for(int i = 0; i < MODBUS_COMMUNICATION_DATA_MAX; i++) {
            data.writeBuf[i] = 0;  // zero clear
        }
        if (funcCode == QModbusPdu::FunctionCode::WriteMultipleCoils){
            if (size > 256){
                qDebug("funcCode[%d] size[%d] is limit over", funcCode, size);
                return data;
            }
            // bit-write
            data.comSize = size;
            for(int i = 0; i < (int)size; i++){
                data.writeBuf[i / 16] |= ((writeData.at(i) & 0x01) << (i % 16));
            }
        } else {
            for(int i = 0; i < writeData.size(); i++) {
                if(i >= MODBUS_COMMUNICATION_DATA_MAX) {
                    // Write buffer not enough
                    continue;
                } else {
                    data.writeBuf[i] = writeData.at(i);
                }
            }
        }
        return data;
    }
};

#endif // MODBUSCOMMANDGENERATOR_H
