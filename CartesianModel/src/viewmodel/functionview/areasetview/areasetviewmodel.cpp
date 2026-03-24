#include "areasetviewmodel.h"
#include "areasetdata.h"
#include <QQmlEngine>

const int AREA_SET_DATA_MAGNIFICATION = 100;
const QString PEN_SET_GROUP_AREA_SET = "19";

const QList<QList<quint16>> AREA_SET_AXIS_DATA_ADDRESS_LIST = {
    // position(SR)    servo(DI) servo(DO) OVR(DI)
    {     2785,        2687,     1783,     2679, },     // Axis-1
    {     2787,        2686,     1782,     2678, },     // Axis-2
    {     2789,        2685,     1781,     2677, },     // Axis-3
    {     2791,        2684,     1780,     2676, },     // Axis-4
    {     2793,        2683,     1779,     2675, },     // Axis-5
    {     2795,        2682,     1778,     2674, },     // Axis-6
    {     2797,        2681,     1777,     2673, },     // Axis-7
    {     2799,        2680,     1776,     2672, },     // Axis-8
};

typedef enum {
    Position,
    ServoDI,
    ServoDO,
    Over,
} AreaSetAxisDataType;

const quint16 AREA_SET_MX_SX_SPACE_ADDRESS = 4942;

const QList<quint16> AREA_SET_IO_SIGNAL_ADDRESS_LIST = {
//  DI
    9,         // 取物側
    5,         // 置物側
    4,         // 製品側上位
    3          // 副側上位
};

const quint16 INVALID_ADDRESS = 0xFFFF;

const QList<QList<quint16>> AREA_SET_DATA_ADDRESS_LIST = {
    //    取出側Max(HR)      取出側Min(HR)        開放側MAX(HR)        開放側Min(HR)         下降Max(HR)         下降MaxMovement(HR)
    {     7888,             8128,               7912,               8152,               INVALID_ADDRESS,    8656  },     // Axis-1
    {     7891,             8131,               7915,               8155,               INVALID_ADDRESS,    8659  },     // Axis-2
    {     7894,             8134,               7918,               8158,               INVALID_ADDRESS,    8662  },     // Axis-3
    {     7945,             INVALID_ADDRESS,    7921,               INVALID_ADDRESS,    7897,               8665  },     // Axis-4
    {     7948,             INVALID_ADDRESS,    7924,               INVALID_ADDRESS,    7900,               8668  },     // Axis-5
    {     INVALID_ADDRESS,  INVALID_ADDRESS,    INVALID_ADDRESS,    INVALID_ADDRESS,    INVALID_ADDRESS,    8671, },     // Axis-6
    {     INVALID_ADDRESS,  INVALID_ADDRESS,    INVALID_ADDRESS,    INVALID_ADDRESS,    INVALID_ADDRESS,    8674, },     // Axis-7
    {     INVALID_ADDRESS,  INVALID_ADDRESS,    INVALID_ADDRESS,    INVALID_ADDRESS,    INVALID_ADDRESS,    8677, },     // Axis-8
};

const QList<quint16> AREA_SET_DELAY_READ_DATA_ADDRESS_LIST = {7888, 8128, 8656};
const int AREA_SET_DELAY_READ_DATA_SIZE = 125;

AreaSetViewModel::AreaSetViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm),
      m_commonView(commonView)
{
    init();
}

AreaSetViewModel::AreaSetViewModel(QObject *object)
    : ViewModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

AreaSetViewModel::~AreaSetViewModel()
{
}

int AreaSetViewModel::setDataByAreaSet(int areaType, int axis, int value)
{
    int ret = setData(areaType, axis, value);
    if (ret == 0) {
        int prevValue = static_cast<AreaSetData *>(m_datas.at(m_currentAreaType).at(axis))->value();
        if (value != prevValue)
            m_operationRecord->recordAreaSet(areaType, axis, prevValue, value);
    }
    return ret;
}
int AreaSetViewModel::setDataByAreaFix(int areaType, int axis, int value)
{
    int ret = setData(areaType, axis, value);
    if (ret == 0) {
        int prevValue = static_cast<AreaSetData *>(m_datas.at(m_currentAreaType).at(axis))->value();
        if (value != prevValue)
            m_operationRecord->recordAreaFix(areaType, axis, prevValue, value);
    }
    return ret;
}

int AreaSetViewModel::setMemoryData(int areaType, int axis, int value)
{
    int pointNum = getPointNum(areaType,axis);
    qint32 max = m_commonView->formatPointMaxValue(axis, pointNum).toFloat() * 100;
    qint32 min = m_commonView->formatPointMinValue(axis, pointNum).toFloat() * 100;
    if(value > max || value < min){
        return -1;
    }
    int ret = setData(areaType, axis, value);
    if (ret == 0) {
        int prevValue = static_cast<AreaSetData *>(m_datas.at(m_currentAreaType).at(axis))->value();
        if (value != prevValue)
            m_operationRecord->recordAreaSetMemory(areaType, axis, prevValue, value);
    }
    return ret;
}

int AreaSetViewModel::setData(int areaType, int axis, int value)
{
    if ((areaType < Enums::AreaTypeOutputSideMax) || (areaType >= Enums::AreaTypeNum)) {
        return -1;
    }
    if ((axis < Enums::AxisY) || (axis >= Enums::AxisNum)) {
        return -1;
    }
    quint16 addr = AREA_SET_DATA_ADDRESS_LIST.at(axis).at(areaType);
    if (addr == INVALID_ADDRESS) {
        return -1;
    }

    modbusSaveData(addr, value);
    modbusGetData(addr);
    return 0;
}

int AreaSetViewModel::getPointNum(int areaType, int axis)
{
    int pointNo = 0;
    switch(areaType) {
    case Enums::AreaType::AreaTypeDescentMaxMovement:  // 最大移動値
        pointNo = 253;
        break;
    case  Enums::AreaType::AreaTypeOutputSideMax:       // 取出側区域(最大)
        if ((axis >= 0) && (axis <= 2)) {
            pointNo = 221;
        } else if ((axis == 3) || (axis == 4)) {
            pointNo = 223;
        }
        break;
    case Enums::AreaType::AreaTypeOutputSideMin:       // 取出側区域(最小)
        if ((axis >= 0) && (axis <= 2)) {
            pointNo = 231;
        }
        break;
    case Enums::AreaType::AreaTypeOpenSideMax:         // 開放側区域(最大)
        if ((axis >= 0) && (axis <= 4)) {
            pointNo = 222;
        }
        break;
    case Enums::AreaType::AreaTypeOpenSideMin:         // 開放側区域(最小)
        if ((axis >= 0) && (axis <= 2)) {
            pointNo = 232;
        }
        break;
    case Enums::AreaType::AreaTypeDescentMax:          // 下降待機(最大)
        if ((axis == 3) || (axis == 4)) {
            pointNo = 221;
        }
        break;
    default:
        break;
    }
    return pointNo;
}

void AreaSetViewModel::setMxSxSpaceValue(int value)
{
    modbusMxSxSpace(value);
    modbusGetMxSxSpace();
    m_operationRecord->recordAreaFixMxSx(m_mxSxSpace, value);
}

QString AreaSetViewModel::formatData(int areaType, int value)
{
    if ((areaType < Enums::AreaTypeOutputSideMax) || (areaType >= Enums::AreaTypeNum)) {
        return "";
    }
    int format = m_dataFormatList.at(areaType);
    return m_resource->formatData(value, format, 2, true);
}

QString AreaSetViewModel::formatPosition(int value)
{
    return m_resource->formatData(value, m_positionFormat, 2);
}

QString AreaSetViewModel::formatAreaFixData(int areaType, int value)
{
    if ((areaType < Enums::AreaTypeOutputSideMax) || (areaType >= Enums::AreaTypeNum)) {
        return "";
    }
    int format = m_areaFixDataFormatList.at(areaType);
    return m_resource->formatData(value, format, 2, true);
}

QString AreaSetViewModel::formatAreaFixPosition(int value)
{
    return m_resource->formatData(value, m_areaFixPositionFormat, 2);
}

QString AreaSetViewModel::formatMxSxSpace(int value)
{
    return m_resource->formatData(value, m_areaFixMxSxSpaceFormat, 2, true);
}

QList<QObject *> AreaSetViewModel::getDataList(int areaType)
{
    return m_datas.at(areaType);
}

int AreaSetViewModel::dataMagnification()
{
    return AREA_SET_DATA_MAGNIFICATION;
}

void AreaSetViewModel::setDataList(QList<QObject *> list)
{
    m_datas.replace(m_currentAreaType, list);
    emit dataListChanged();
}

void AreaSetViewModel::setCurrentAreaType(int type)
{
    if ((type >= Enums::AreaTypeOutputSideMax) && (type < Enums::AreaTypeNum)) {
        if (m_currentAreaType != type) {
            m_currentAreaType = type;
            emit currentAreaTypeChanged();
            emit dataListChanged();
        }
    }
}

void AreaSetViewModel::setIoSignalList(QList<bool> ioSignalList)
{
    if (m_ioSignalList != ioSignalList) {
        m_ioSignalList = ioSignalList;
        emit ioSignalListChanged();
    }
}


void AreaSetViewModel::setMxSxSpace(int value)
{
    if (m_mxSxSpace != value) {
        m_mxSxSpace = value;
        emit mxSxSpaceChanged();
    }
}

void AreaSetViewModel::init()
{
    readDataFormatCsv();
    initData();
    startModbusOneShot();
    for (quint16 hrAddress : AREA_SET_DELAY_READ_DATA_ADDRESS_LIST)
        modbusDelayGetData(hrAddress, AREA_SET_DELAY_READ_DATA_SIZE);
    m_remainAllAreaDataNum = AREA_SET_DELAY_READ_DATA_ADDRESS_LIST.length();
    m_delayReadingAllAreaData = true;
}

void AreaSetViewModel::initData()
{
    for (int i = 0; i < Enums::AreaTypeNum; i++) {
        QList<QObject *> list;
        for (int axis = 0; axis < Enums::AxisNum; axis++) {
            AreaSetData* data = new AreaSetData(this);
            bool enable = (AREA_SET_DATA_ADDRESS_LIST.at(axis).at(i) != INVALID_ADDRESS);
            data->setEnable(enable);
            QQmlEngine::setObjectOwnership(data, QQmlEngine::CppOwnership);
            list.append(data);
        }
        m_datas.append(list);
    }

    for (int i = 0; i < AREA_SET_IO_SIGNAL_ADDRESS_LIST.length(); i++)
        m_ioSignalList.append(false);
}

void AreaSetViewModel::onActivate()
{
    connect(m_communication, &CommunicationThread::finish, this, &AreaSetViewModel::onFinished, Qt::QueuedConnection);
    connect(m_modbus, &ModbusManager::endModbusPageChange, this, &AreaSetViewModel::endModbusPageChange, Qt::DirectConnection);
}

void AreaSetViewModel::onDeactivate()
{
    disconnect(m_modbus, &ModbusManager::endModbusPageChange, this, &AreaSetViewModel::endModbusPageChange);
    disconnect(m_communication, &CommunicationThread::finish, this, &AreaSetViewModel::onFinished);
    stopModbusOneShot();
}

void AreaSetViewModel::readDataFormatCsv()
{
    readPenSetData();
}

void AreaSetViewModel::readPenSetData()
{
    m_dataFormatList.clear();
    QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_AREA_SET, 10);
    m_positionFormat = rowData.at(0);
    for (int i = 1; i < rowData.length(); i++) {
        m_dataFormatList.append(rowData.at(i));
    }
    readPenSetAreaFixData();
}

void AreaSetViewModel::readPenSetAreaFixData()
{
    m_areaFixDataFormatList.clear();
    QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_AREA_SET, 23);
    m_areaFixPositionFormat = rowData.at(0);
    for (int i = 1; i < rowData.length() - 1; i++) {
        m_areaFixDataFormatList.append(rowData.at(i));
    }
    m_areaFixMxSxSpaceFormat = rowData.at(7);
}

bool AreaSetViewModel::updateAxisData()
{
    bool ret = false;
    if (m_pageChanging == false && m_modbus->getIsPageChanging() == false) {
        bool servo;
        for (int i = 0; i < AREA_SET_AXIS_DATA_ADDRESS_LIST.length(); i++) {
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QList<quint16> addressList = AREA_SET_AXIS_DATA_ADDRESS_LIST.at(i);
            servo = m_modbus->getReceiveDataDI(addressList.at(AreaSetAxisDataType::ServoDI)) == 0;
            QString encoder = m_resource->formatData((short)m_modbus->getWordReceiveDataRG(2761 + i), 64);

            m_axisInfo->setServo(axis, servo);
            m_axisInfo->setOver(axis, m_modbus->getReceiveDataDI(addressList.at(AreaSetAxisDataType::Over)));
            m_axisInfo->setEncoder(axis,encoder);
        }
        ret = true;
    }
    return ret;
}

bool AreaSetViewModel::updateIoSignal()
{
    bool ret = false;
    if (m_pageChanging == false && m_modbus->getIsPageChanging() == false) {
        bool update = false;
        for (int i = 0; i < AREA_SET_IO_SIGNAL_ADDRESS_LIST.length(); i++) {
            quint16 addr = AREA_SET_IO_SIGNAL_ADDRESS_LIST.at(i);
            bool prev = m_ioSignalList.at(i);
            bool current = m_modbus->getReceiveDataDI(addr);
            if (current != prev) {
                m_ioSignalList.replace(i, current);
                update = true;
            }
        }
        if (update)
            emit ioSignalListChanged();
        ret = true;
    }
    return ret;
}

void AreaSetViewModel::getAllAreaData()
{
    for (quint16 hrAddress : AREA_SET_DELAY_READ_DATA_ADDRESS_LIST)
        modbusGetData(hrAddress, AREA_SET_DELAY_READ_DATA_SIZE);
    m_remainAllAreaDataNum = AREA_SET_DELAY_READ_DATA_ADDRESS_LIST.length();
}

void AreaSetViewModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (!m_isModbusConnectOneShot) {
        connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &AreaSetViewModel::onOneShotReadFinished, Qt::QueuedConnection);
        m_isModbusConnectOneShot = true;
    }
}

void AreaSetViewModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (m_isModbusConnectOneShot){
        disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &AreaSetViewModel::onOneShotReadFinished);
        m_isModbusConnectOneShot = false;
    }
}

void AreaSetViewModel::modbusSaveData(quint16 address, int value)
{
    QList<int> writeData;
    writeData.append((value & 0x0000FFFF));
    writeData.append((value & 0xFFFF0000) >> 16);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, address, 0x0002, writeData);
    setOneShotModbusCommand(command);
}

void AreaSetViewModel::modbusGetData(quint16 address)
{
    modbusGetData(address, 2);
}

void AreaSetViewModel::modbusGetData(quint16 address, quint16 size)
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, address, size);
    setOneShotModbusCommand(command);
}

void AreaSetViewModel::modbusDelayGetData(quint16 address, quint16 size)
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, address, size);
    m_communication->setDelayCommandList(Enums::Pages::FunctionAreaSet, command);
}

void AreaSetViewModel::modbusMxSxSpace(int value)
{
    QList<int> writeData;
    writeData.append((value & 0x0000FFFF));
    writeData.append((value & 0xFFFF0000) >> 16);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, AREA_SET_MX_SX_SPACE_ADDRESS, 2, writeData);
    setOneShotModbusCommand(command);
}

void AreaSetViewModel::modbusGetMxSxSpace()
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, AREA_SET_MX_SX_SPACE_ADDRESS, 2);
    setOneShotModbusCommand(command);
}

void AreaSetViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerFunctionAreaSetPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void AreaSetViewModel::onFinished()
{
    bool updated = updateAxisData();
    updated |= updateIoSignal();
    if (!m_dataLoaded && updated) {
        setDataLoaded(true);
        startModbusOneShot();
        getAllAreaData();
        modbusGetMxSxSpace();
    }

    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void AreaSetViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (!m_delayReadingAllAreaData && (oneShotReadCaller != CommunicationThread::CallerFunctionAreaSetPageModel)){
        qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if (command.comSize == AREA_SET_DELAY_READ_DATA_SIZE) {
            for (quint16 hrAddress : AREA_SET_DELAY_READ_DATA_ADDRESS_LIST) {
                if (command.readAddress == hrAddress) {
                    if (m_remainAllAreaDataNum >= 1) {
                        m_remainAllAreaDataNum--;
                    }
                    if (m_remainAllAreaDataNum == 0) {
                        for (int i = 0; i < Enums::AreaTypeNum; i++) {
                            for (int axis = 0; axis < Enums::AxisNum; axis++) {
                                quint16 addr = AREA_SET_DATA_ADDRESS_LIST.at(axis).at(i);
                                if (addr == INVALID_ADDRESS)
                                    continue;
                                AreaSetData *data = static_cast<AreaSetData *>(m_datas.at(i).at(axis));
                                data->setValue(m_modbus->getLongReceiveDataHR(addr));
                            }
                        }
                        if (m_delayReadingAllAreaData) {
                            if (!isViewActive()) {
                                stopModbusOneShot();
                            }
                            m_delayReadingAllAreaData = false;
                        }
                    }
                }
            }
            return;
        }
        if (command.readAddress == AREA_SET_MX_SX_SPACE_ADDRESS) {
            setMxSxSpace(m_modbus->getLongReceiveDataHR(command.readAddress));
        } else {
            for (int i = 0; i < Enums::AreaTypeNum; i++) {
                for (int axis = 0; axis < Enums::AxisNum; axis++) {
                    if (AREA_SET_DATA_ADDRESS_LIST.at(axis).at(i) == command.readAddress) {
                        AreaSetData *data = static_cast<AreaSetData *>(m_datas.at(i).at(axis));
                        data->setValue(m_modbus->getLongReceiveDataHR(command.readAddress));
                        break;
                    }
                }
            }
        }
    }
}

void AreaSetViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void AreaSetViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}

void AreaSetViewModel::endModbusPageChange(int mainScreenId, int subScreenId)
{
    if ((mainScreenId == 19) && ((subScreenId == 0) || (subScreenId == 1))) {
        m_pageChanging = false;
    }
}
