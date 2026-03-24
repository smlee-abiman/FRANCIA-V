#include "teachingpasspagemodel.h"
#include "teachingviewdef.h"
#include "../areasetpointparser.h"
#include "../areasetpointinfo.h"

const int PASS_POINT_NUM = 10;

const QList<quint16> PASS_TIME_ADDRESS = {
//  SR
    2761,       // PASSç≈èIíºê¸à⁄ìÆãóó£1
    2763,       // PASSç≈èIíºê¸à⁄ìÆãóó£2
    2765,       // PASSç≈èIíºê¸à⁄ìÆãóó£3
    2767,       // PASSç≈èIíºê¸à⁄ìÆãóó£4
};

const QList<quint16> PASS_DISTANCE_ADDRESS = {
//   HR
    26593,      // ê≥‰]éÊèoâ∫ç~ãóó£.
    26587,      // ê≥‰]éÊèoè„è°ãóó£.
    26596,      // ïõ‰]éÊèoâ∫ç~ãóó£.
    26590,      // ïõ‰]éÊèoè„è°ãóó£.
    26752,      // íuï®â∫ç~ãóó£.
    26617,      // íuï®è„è°ãóó£.
};

TeachingPassPageModel::TeachingPassPageModel(ResourceManager *resource, CommunicationThread * comm)
    : PageModelBase(resource, comm)
{
    m_areaSetPointParser = new AreaSetPointParser(this);
    init();
}

TeachingPassPageModel::TeachingPassPageModel(QObject *object)
    : PageModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

TeachingPassPageModel::~TeachingPassPageModel()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingPassPageModel::onFinished);
    stopModbusOneShot();
    qDeleteAll(m_pointInfoList);
}

void TeachingPassPageModel::onActivate()
{
    startModbusOneShot();
    connect(m_communication, &CommunicationThread::finish, this, &TeachingPassPageModel::onFinished, Qt::QueuedConnection);
    connect(m_modbus, &ModbusManager::endModbusPageChange, this, &TeachingPassPageModel::endModbusPageChange, Qt::DirectConnection);
    for (int i = 0; i < PASS_DISTANCE_ADDRESS.length(); i++)
        modbusGetDistance(i);
}

void TeachingPassPageModel::onDeactivate()
{
    disconnect(m_modbus, &ModbusManager::endModbusPageChange, this, &TeachingPassPageModel::endModbusPageChange);
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingPassPageModel::onFinished);
    stopModbusOneShot();
}

void TeachingPassPageModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (!mIsModbusConnectOneShot) {
        connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &TeachingPassPageModel::onOneShotReadFinished, Qt::QueuedConnection);
        mIsModbusConnectOneShot = true;
    }
}

void TeachingPassPageModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (mIsModbusConnectOneShot){
        disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &TeachingPassPageModel::onOneShotReadFinished);
        mIsModbusConnectOneShot = false;
    }
}

void TeachingPassPageModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerTeachingPassPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void TeachingPassPageModel::init()
{
    for (int i = 0; i < PASS_TIME_ADDRESS.length(); i++)
        m_timeList.append(0);
    for (int i = 0; i < PASS_DISTANCE_ADDRESS.length(); i++)
        m_distanceList.append(0);

    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::PassPage));
    setPageLabelColor(m_resource->rgb(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::PassPage));
    setActivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_PASS_PAGE_ACTIVATE_ICON_NAME);
    setDeactivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_PASS_PAGE_DEACTIVATE_ICON_NAME);
    readPenSetData();
    // Detect language changes to update labels.
    connect(m_resource, &ResourceManager::languageChanged, this, &TeachingPassPageModel::onLanguageChanged, Qt::QueuedConnection);
}

void TeachingPassPageModel::readPenSetData()
{
    readDataFormat();
    readMaxMinSetting();
}

void TeachingPassPageModel::readDataFormat()
{
    m_dataFormatList.clear();
    QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_PASS_PAGE, PEN_SET_PASS_FORMAT_ROW);
    for (int i = 0; i < PASS_POINT_NUM; i++) {
        int format = rowData.at(i);
        m_dataFormatList.append(format);
    }
}

void TeachingPassPageModel::readMaxMinSetting()
{
    QList<QList<int>> maxMinSettngList = m_resource->getIntHexValueList(PEN_SET_GROUP_PASS_PAGE.toInt(), PEN_SET_PASS_MAX_MIN_ROW, 6);
    for (QList<int> maxMinSetting: maxMinSettngList) {
        AreaSetPointInfo *info = new AreaSetPointInfo();
        quint8 pointSetting = static_cast<quint8>(maxMinSetting.at(0));
        quint8 axis = static_cast<quint8>(maxMinSetting.at(1) - 1);
        m_areaSetPointParser->parse(info, pointSetting, axis);
        m_pointInfoList.append(info);
    }
}

void TeachingPassPageModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::PassPage));
}

void TeachingPassPageModel::onFinished()
{
    if (m_pageChanging == false && m_modbus->getIsPageChanging() == false) {
        for (int i = 0; i < PASS_TIME_ADDRESS.length(); i++)
            m_timeList.replace(i, m_modbus->getLongReceiveDataRG(PASS_TIME_ADDRESS.at(i)));
        emit timeListChanged(m_timeList);
        setDataLoaded(true);
    }
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void TeachingPassPageModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerTeachingPassPageModel){
        qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters)
        updateDistanceData(command.readAddress);
}

int TeachingPassPageModel::setDistance(int index, int value)
{
    if ((index < 0) || index >= PASS_DISTANCE_ADDRESS.length()) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid argument." << index;
        return -1;
    }
    modbusSaveDistance(PASS_DISTANCE_ADDRESS.at(index), value);
    m_operationRecord->recordSetPass(index, m_distanceList.at(index), value);
    modbusGetDistance(index);
    return 0;
}

QString TeachingPassPageModel::formatData(int point, int value)
{
    if ((point < 0) || (point >= m_dataFormatList.length())) {
        qWarning() << __func__ << "The point is out of range." << point << value;
        return "";
    }
    int format = m_dataFormatList.at(point);
    DataFormat dataFormat = DataFormat::fromCode(format);
    int roundedValue = roundOff(value, dataFormat.fraction);
    return m_resource->formatData(roundedValue, format, 2);
}

QString TeachingPassPageModel::formatPointMaxValue(int index)
{
    if ((index < 0) || (index >= m_pointInfoList.length())) {
        return "";
    }
    int value = m_areaSetPointParser->getMaxValue(m_pointInfoList.at(index));
    return m_resource->formatData(value, 241, 2, true);
}

QString TeachingPassPageModel::formatPointMinValue(int index)
{
    if ((index < 0) || (index >= m_pointInfoList.length())) {
        return "";
    }
    int value = m_areaSetPointParser->getMinValue(m_pointInfoList.at(index));
    QString valueStr = m_resource->formatData(value, 241, 2, true);
    if (valueStr == "****.**") {
        valueStr = "0.0";
    }
    return valueStr;
}

void TeachingPassPageModel::modbusSaveDistance(quint16 hrAddress, int value)
{
    QList<int> writeData;
    writeData.append((value & 0x0000FFFF));
    writeData.append((value & 0xFFFF0000) >> 16);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, 0x0002, writeData);
    m_communication->setModbusWriteCommand(command);
}

int TeachingPassPageModel::modbusGetDistance(int index)
{
    if ((index < 0) || index >= PASS_DISTANCE_ADDRESS.length()) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid argument." << index;
        return -1;
    }

    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, PASS_DISTANCE_ADDRESS.at(index), 2);
    setOneShotModbusCommand(command);
    return 0;
}

void TeachingPassPageModel::updateDistanceData(quint16 hrAddress)
{
    bool update = false;
    for (int i = 0; i < PASS_DISTANCE_ADDRESS.length(); i++) {
        quint16 address = PASS_DISTANCE_ADDRESS.at(i);
        if (hrAddress == address) {
            m_distanceList.replace(i, m_modbus->getLongReceiveDataHR(address));
            update = true;
        }
    }
    if (update)
        emit distanceListChanged(m_distanceList);
}

void TeachingPassPageModel::setTimeList(QList<int> &list)
{
    if (m_timeList != list) {
        m_timeList = list;
        emit timeListChanged(m_timeList);
    }
}
void TeachingPassPageModel::setDistanceList(QList<int> &list)
{
    if (m_distanceList != list) {
        m_distanceList = list;
        emit distanceListChanged(m_distanceList);
    }
}

void TeachingPassPageModel::modbusGetData(quint16 address)
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, address, 2);
    setOneShotModbusCommand(command);
}

int TeachingPassPageModel::roundOff(int value, int fraction)
{
    int ret = value;
    if (fraction == 0) {
        ret = (value + 50) / 100 * 100;
    } else if (fraction == 1) {
        ret = (value + 5) / 10 * 10;
    } else if (fraction == 2) {
        ret = value;
    }
    return ret;
}

void TeachingPassPageModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void TeachingPassPageModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}

void TeachingPassPageModel::endModbusPageChange(int mainScreenId, int subScreenId)
{
    if ((mainScreenId == 15) && (subScreenId == 0)) {
        m_pageChanging = false;
    }
}
