#ifndef CORELIBMODBUSDATAMANAGER_H
#define CORELIBMODBUSDATAMANAGER_H

#include <QObject>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QMutex>
#include <QBitArray>
#include <QByteArray>
#include <QFile>

#define MODBUS_COMMUNICATION_DATA_MAX                   128         // max comSize
#define MODBUS_FUNCCODE01_COMSIZE_MAX                   128         // funcCode01:0x01(Read Coils)
#define MODBUS_FUNCCODE02_COMSIZE_MAX                   128         // funcCode02:0x02(Read Input Discretes)
#define MODBUS_FUNCCODE03_COMSIZE_MAX                   128         // funcCode03:0x03(Read Multiple Registers)
#define MODBUS_FUNCCODE04_COMSIZE_MAX                   128         // funcCode04:0x04(Read Input Registers)
#define MODBUS_FUNCCODE05_COMSIZE_MAX                   128         // funcCode05:0x05(Write Coils)
#define MODBUS_FUNCCODE06_COMSIZE_MAX                   128         // funcCode06:0x06(Write Single Register)
#define MODBUS_FUNCCODE0F_COMSIZE_MAX                   128         // funcCode0F:0x0F(Force Multiple Coils)
#define MODBUS_FUNCCODE10_COMSIZE_MAX                   128         // funcCode10:0x10(Write Multiple Registers)
#define MODBUS_POINT_MEMORY_DATA_HEADER_SIZE            2
#define MODBUS_POINT_MEMORY_DATA_HEADER_REALITY_ADDRESS 0x6480
#define MODBUS_COMSIZE_CHECK(funcCode, comSize) \
    (funcCode == QModbusPdu::FunctionCode::ReadCoils                ? ((comSize > MODBUS_FUNCCODE01_COMSIZE_MAX * 16) ? false : true): \
     funcCode == QModbusPdu::FunctionCode::ReadDiscreteInputs       ? ((comSize > MODBUS_FUNCCODE02_COMSIZE_MAX * 16) ? false : true): \
     funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters     ? ((comSize > MODBUS_FUNCCODE03_COMSIZE_MAX) ? false : true): \
     funcCode == QModbusPdu::FunctionCode::ReadInputRegisters       ? ((comSize > MODBUS_FUNCCODE04_COMSIZE_MAX) ? false : true): \
     funcCode == QModbusPdu::FunctionCode::WriteSingleCoil          ? ((comSize > MODBUS_FUNCCODE05_COMSIZE_MAX) ? false : true): \
     funcCode == QModbusPdu::FunctionCode::WriteSingleRegister      ? ((comSize > MODBUS_FUNCCODE06_COMSIZE_MAX) ? false : true): \
     funcCode == QModbusPdu::FunctionCode::WriteMultipleCoils       ? ((comSize > MODBUS_FUNCCODE0F_COMSIZE_MAX * 16) ? false : true): \
     funcCode == QModbusPdu::FunctionCode::WriteMultipleRegisters   ? ((comSize > MODBUS_FUNCCODE10_COMSIZE_MAX) ? false : true): false)

#define MODBUS_BIT_SHIFT_MASK_NUM(i)    \
    (i == 0 ? 0xFFFE : \
     i == 1 ? 0xFFFD : \
     i == 2 ? 0xFFFB : \
     i == 3 ? 0xFFF7 : \
     i == 4 ? 0xFFEF : \
     i == 5 ? 0xFFDF : \
     i == 6 ? 0xFFBF : \
     i == 7 ? 0xFF7F : \
     i == 8 ? 0xFEFF : \
     i == 9 ? 0xFDFF : \
     i == 10 ? 0xFBFF : \
     i == 11 ? 0xF7FF : \
     i == 12 ? 0xEFFF : \
     i == 13 ? 0xDFFF : \
     i == 14 ? 0xBFFF : \
     i == 15 ? 0x7FFF : 0xFFFF)

typedef struct
{
    QModbusPdu::FunctionCode    funcCode;                                   // function code
    QModbusPdu::FunctionCode    writeFuncCode;                              // function code(readWrite用writeFuncCode)
    ushort                      readAddress;                                // address(read)
    ushort                      writeAddress;                               // address(write)
    ushort                      comSize;                                    // communication size
    ushort                      writeComSize;                               // communication size(readWrite用writeComSize)
    QBitArray                   bitReadBuf;                                 // read Buf(bit)
    quint16                     readBuf[MODBUS_COMMUNICATION_DATA_MAX];     // read Buf
    quint16                     writeBuf[MODBUS_COMMUNICATION_DATA_MAX];    // write Buf
    QByteArray                  readByteArray;                              // read Buf(for rawRequest)
    QByteArray                  writeByteArray;                             // write Buf(for rawRequest)
    bool                        isBulkMode;                                 // false:Normal Mode/true:Bulk Mode
    bool                        isSummaryBulkMode;                          // false:Normal Mode/true:Summary Bulk Mode
} stModBusCommunicationData;

Q_DECLARE_METATYPE(stModBusCommunicationData)

class CoreLibModBusDataManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QBitArray modBusBitMemoryData READ modBusBitMemoryData NOTIFY modBusBitMemDataChanged)
    Q_PROPERTY(stModBusCommunicationData modBusMemData READ modBusMemoryData NOTIFY modBusMemDataChanged)
private:
    explicit CoreLibModBusDataManager(QObject *parent = nullptr);
    ~CoreLibModBusDataManager();
public:
    static CoreLibModBusDataManager* get_instance() {
        static CoreLibModBusDataManager instance;
        return &instance;
    }

    enum ModbusRequestType {
        ModbusRequestTypeRead,
        ModbusRequestTypeWrite,
        ModbusRequestTypeReadWrite,
    };
    QModbusDataUnit* getModBudDataUnit(ModbusRequestType reqType);
    //void getModBusMemoryData(int address, size_t size, char* dest);
    //void setModBusMemoryData(ushort address, char* val, size_t size);

    QModbusDataUnit::RegisterType getRegisterType(QModbusPdu::FunctionCode funcCode);
    void lockModBusMutex(ModbusRequestType reqType);
    void unlockModBusMutex(ModbusRequestType reqType);
    QBitArray* getModBusBitMemoryData();
    stModBusCommunicationData* getModBusComData();
    void setModBusComData(stModBusCommunicationData&);
    QBitArray modBusBitMemoryData() {
        return m_modBusBitMemoryData;
    }
    stModBusCommunicationData modBusMemoryData() {
        return m_modBusMemData;
    }

private:
    QModbusDataUnit mReadModBusDataUnit;
    QModbusDataUnit mWriteModBusDataUnit;
    QModbusRequest mRawModBusRequest;
    QMutex *mMutexReadModBus;
    QMutex *mMutexWriteModBus;
    //char mModBusMemoryData[MODBUS_POINT_MEMORY_DATA_HEADER_REALITY_ADDRESS+MODBUS_POINT_MEMORY_DATA_HEADER_SIZE];
    QBitArray m_modBusBitMemoryData;
    stModBusCommunicationData m_modBusMemData;

signals:
    //void dataChanged(int address, size_t size);
    void modBusBitMemDataChanged(QBitArray m_modBusBitMemoryData);
    void modBusMemDataChanged(stModBusCommunicationData m_modBusMemData);
};

#endif // CORELIBMODBUSDATAMANAGER_H
