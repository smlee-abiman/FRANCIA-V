#include "maintainviewmodel.h"
#include "maintaindata.h"

const QString PEN_SET_GROUP_MAINTAIN_VIEW = "20";

const quint16 LAST_INSPECTION_MONTH_ADDRESS_OFFSET = 1;
const quint16 LAST_INSPECTION_DAY_ADDRESS_OFFSET = 2;
const quint16 LAST_INSPECTION_DATE_START_ADDRESS = 52273;
const quint16 SAVE_MAINTAIN_DATA_ADDRESS = 2533;
const quint16 DATE_DATA_SIZE = 3;
const QList<quint16> LAST_INSPECTION_DATE_ADDRESS_LIST = {
    LAST_INSPECTION_DATE_START_ADDRESS,
    52276,
    52279,
    52282,
    52285,
    52288,
    52291,
    52294,
    52297,
    52300
};

const quint16 DISTANCE_START_ADDRESS = 1901;
const quint16 DISTANCE_DATA_SIZE = 32;
const quint16 LAST_REFUELING_MONTH_ADDRESS_OFFSET = 1;
const quint16 LAST_REFUELING_DAY_ADDRESS_OFFSET = 2;
const quint16 LAST_REFUELING_DATE_START_ADDRESS = 52465;
const quint16 LAST_REFUELING_DATE_DATA_SIZE = 24;

const QList<QList<quint16>> MAINTAIN_DATA_ADDRESS_LIST = {
    { DISTANCE_START_ADDRESS, 1917, LAST_REFUELING_DATE_START_ADDRESS },
    { 1903, 1919, 52468 },
    { 1905, 1921, 52471 },
    { 1907, 1923, 52474 },
    { 1909, 1925, 52477 },
    { 1911, 1927, 52480 },
    { 1913, 1929, 52483 },
    { 1915, 1931, 52486 }
};

const int MAINTAIN_YEAR_OFFSET = 2000;

typedef enum {
    RefuelingDistance,
    CumulativeDistance,
    LastRefuelingDate,
} MaintainDataType;

MaintainViewModel::MaintainViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    init();
}

MaintainViewModel::MaintainViewModel(QObject *parent)
    : ViewModelBase(nullptr, nullptr)
{
    Q_UNUSED(parent)
}

void MaintainViewModel::saveLastInspectionDate(const QString& date)
{
    bool complete;
    bool update = false;
    for (int i = 0; i < m_maintainCompleteList.length(); i++) {
        complete = m_maintainCompleteList.at(i);
        if (complete) {
            int year;
            int month;
            int day;
            getDate(date, year, month, day);
            modbusSetLastInspectionDate(i, year - MAINTAIN_YEAR_OFFSET, month, day);
            update = true;
        }
        m_maintainCompleteList.replace(i, false);
    }
    if (update) {
        modbusSaveMaintainData();
        modbusGetLastInspectionDate();
    }
    emit maintainCompleteListChanged();
}

void MaintainViewModel::saveLastRefuelingDate(const QString& date)
{
    bool complete;
    bool update = false;
    for (int i = 0; i < m_refuelingCompleteList.length(); i++) {
        complete = m_refuelingCompleteList.at(i);
        if (complete) {
            int year;
            int month;
            int day;
            getDate(date, year, month, day);
            modbusSetLastRefuelingDate(i, year - MAINTAIN_YEAR_OFFSET, month, day);
            update = true;
        }
        m_refuelingCompleteList.replace(i, false);
    }
    if (update) {
        modbusSaveMaintainData();
        modbusGetDistanceData();
        modbusGetLastRefuelingDate();
    }
    emit refuelingCompleteListChanged();
}

QString MaintainViewModel::formatRefuelingDistance(int value)
{
    return m_resource->formatData(value, m_refuelingDistanceFormat, 5);
}

QString MaintainViewModel::formatCumulativeDistance(int value)
{
    return m_resource->formatData(value, m_cumulativeDistanceFormat, 2);
}

void MaintainViewModel::setMaintainCompleteList(QList<bool>& list)
{
    if (m_maintainCompleteList != list) {
        m_maintainCompleteList = list;
        emit maintainCompleteListChanged();
    }
}

void MaintainViewModel::setLastInspectionDateList(QList<QString>& list)
{
    if (m_lastInspectionDateList != list) {
        m_lastInspectionDateList = list;
        emit lastInspectionDateListChanged();
    }
}

void MaintainViewModel::setRefuelingCompleteList(QList<bool> &list)
{
    if (m_refuelingCompleteList != list) {
        m_refuelingCompleteList = list;
        emit refuelingCompleteListChanged();
    }
}

void MaintainViewModel::setMaintainDataList(QList<QObject *>& list)
{
    if (m_maintainDataList != list) {
        m_maintainDataList = list;
        emit maintainDataListChanged();
    }
}

void MaintainViewModel::init()
{
    readDataFormatCsv();
    initData();
}

void MaintainViewModel::readDataFormatCsv()
{
    readPenSetData();
}

void MaintainViewModel::readPenSetData()
{
    QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_MAINTAIN_VIEW, 15);
    m_refuelingDistanceFormat = rowData.at(0);
    m_cumulativeDistanceFormat = rowData.at(1);
}

void MaintainViewModel::initData()
{
    for (int i = 0; i < LAST_INSPECTION_DATE_ADDRESS_LIST.length(); i++) {
        m_maintainCompleteList.append(false);
        m_lastInspectionDateList.append("");
    }

    for (int i = 0; i < MAINTAIN_DATA_ADDRESS_LIST.length(); i++) {
        m_refuelingCompleteList.append(false);
        MaintainData *data = new MaintainData(this);
        data->setRefuelingDistance(1000 * (i + 1));
        data->setCumulativeDistance(20000 * (i + 1));
        data->setLastRefuelingDate(QString::number(30 * (i + 1)));
        m_maintainDataList.append(data);
    }
}

void MaintainViewModel::onActivate()
{
    startModbusOneShot();
    modbusGetLastInspectionDate();
    modbusGetDistanceData();
    modbusGetLastRefuelingDate();
    connect(m_communication, &CommunicationThread::finish, this, &MaintainViewModel::onFinished, Qt::QueuedConnection);
}

void MaintainViewModel::onDeactivate()
{
    stopModbusOneShot();
    disconnect(m_communication, &CommunicationThread::finish, this, &MaintainViewModel::onFinished);
 }

int MaintainViewModel::modbusSetLastInspectionDate(int dataIndex, int year, int month, int day)
{
    if ((dataIndex < 0) || (dataIndex >= LAST_INSPECTION_DATE_ADDRESS_LIST.length())) {
        return -1;
    }
    QList<int> writeData;
    writeData.append(year);
    writeData.append(month);
    writeData.append(day);
    quint16 address = LAST_INSPECTION_DATE_ADDRESS_LIST.at(dataIndex);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, address, DATE_DATA_SIZE, writeData);
    setOneShotModbusCommand(command);
    return 0;
}

int MaintainViewModel::modbusSetLastRefuelingDate(int axisIndex, int year, int month, int day)
{
    if ((axisIndex < 0) || (axisIndex >= MAINTAIN_DATA_ADDRESS_LIST.length())) {
        return -1;
    }
    QList<int> writeData;
    writeData.append(year);
    writeData.append(month);
    writeData.append(day);
    quint16 address = MAINTAIN_DATA_ADDRESS_LIST.at(axisIndex).at(MaintainDataType::LastRefuelingDate);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, address, DATE_DATA_SIZE, writeData);
    setOneShotModbusCommand(command);
    return 0;
}

int MaintainViewModel::modbusSaveMaintainData()
{
    QList<int> writeData({3});
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, SAVE_MAINTAIN_DATA_ADDRESS, writeData.size(), writeData);
    setOneShotModbusCommand(command);
    return 0;
}

void MaintainViewModel::modbusGetLastInspectionDate()
{
    quint16 size = static_cast<quint16>(LAST_INSPECTION_DATE_ADDRESS_LIST.length() * DATE_DATA_SIZE);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, LAST_INSPECTION_DATE_START_ADDRESS, size);
    setOneShotModbusCommand(command);
}

void MaintainViewModel::modbusGetDistanceData()
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, DISTANCE_START_ADDRESS, DISTANCE_DATA_SIZE);
    setOneShotModbusCommand(command);
}

void MaintainViewModel::modbusGetLastRefuelingDate()
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, LAST_REFUELING_DATE_START_ADDRESS, LAST_REFUELING_DATE_DATA_SIZE);
    setOneShotModbusCommand(command);
}

void MaintainViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerFunctionMaintainPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void MaintainViewModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (!m_isModbusConnectOneShot) {
        connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &MaintainViewModel::onOneShotReadFinished, Qt::QueuedConnection);
        m_isModbusConnectOneShot = true;
    }
}

void MaintainViewModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (m_isModbusConnectOneShot){
        disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &MaintainViewModel::onOneShotReadFinished);
        m_isModbusConnectOneShot = false;
    }
}

void MaintainViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerFunctionMaintainPageModel){
        qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if (command.readAddress == LAST_INSPECTION_DATE_START_ADDRESS) {
            updateAllLastInspectionDate();
        } else if (command.readAddress == DISTANCE_START_ADDRESS) {
            updateAllDistanceData();
        } else if (command.readAddress == LAST_REFUELING_DATE_START_ADDRESS) {
            updateAllLastRefuelingDate();
        }
    }
}

void MaintainViewModel::onFinished()
{
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void MaintainViewModel::updateAllLastInspectionDate()
{
    quint16 addr;
    int year;
    int month;
    int day;
    for (int i = 0; i < LAST_INSPECTION_DATE_ADDRESS_LIST.length(); i++) {
        addr = LAST_INSPECTION_DATE_ADDRESS_LIST.at(i);
        year = m_modbus->getReceiveDataHR(addr) + MAINTAIN_YEAR_OFFSET;
        month = m_modbus->getReceiveDataHR(addr + LAST_INSPECTION_MONTH_ADDRESS_OFFSET);
        day = m_modbus->getReceiveDataHR(addr + LAST_INSPECTION_DAY_ADDRESS_OFFSET);
        QString dateStr = "";
        dateStr.append(QString::asprintf("%04d/%02d/%02d", year, month, day));
        m_lastInspectionDateList.replace(i, dateStr);
    }
    emit lastInspectionDateListChanged();
}

void MaintainViewModel::updateAllDistanceData()
{
    QList<quint16> addrList;
    int refuelingDistance;
    int cumulativeDistance;
    for (int i = 0; i < MAINTAIN_DATA_ADDRESS_LIST.length(); i++) {
        MaintainData *data = static_cast<MaintainData *>(m_maintainDataList.at(i));
        addrList = MAINTAIN_DATA_ADDRESS_LIST.at(i);
        refuelingDistance = m_modbus->getLongReceiveDataHR(addrList.at(MaintainDataType::RefuelingDistance));
        data->setRefuelingDistance(refuelingDistance);
        cumulativeDistance = m_modbus->getLongReceiveDataHR(addrList.at(MaintainDataType::CumulativeDistance));
        data->setCumulativeDistance(cumulativeDistance);
    }
}

void MaintainViewModel::updateAllLastRefuelingDate()
{
    quint16 addr;
    int year;
    int month;
    int day;
    for (int i = 0; i < MAINTAIN_DATA_ADDRESS_LIST.length(); i++) {
        addr = MAINTAIN_DATA_ADDRESS_LIST.at(i).at(MaintainDataType::LastRefuelingDate);
        year = m_modbus->getReceiveDataHR(addr) + MAINTAIN_YEAR_OFFSET;
        month = m_modbus->getReceiveDataHR(addr + LAST_REFUELING_MONTH_ADDRESS_OFFSET);
        day = m_modbus->getReceiveDataHR(addr + LAST_REFUELING_DAY_ADDRESS_OFFSET);
        QString dateStr = "";
        dateStr.append(QString::asprintf("%04d/%02d/%02d", year, month, day));
        MaintainData *data = static_cast<MaintainData *>(m_maintainDataList.at(i));
        data->setLastRefuelingDate(dateStr);
    }
}

void MaintainViewModel::getDate(const QString &dateString, int &year, int &month, int &day)
{
    QDateTime dateTime = QDateTime::fromString(dateString, "yyyy/MM/dd");
    QDate date = dateTime.date();
    year = date.year();
    month = date.month();
    day = date.day();
}

