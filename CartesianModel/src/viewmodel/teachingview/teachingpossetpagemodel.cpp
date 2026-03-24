#include "teachingpossetpagemodel.h"
#include "teachingviewdef.h"
#include "pointinfo.h"
#include "axisoperateviewmodel.h"
#include "teachingmodepagemodel.h"
#include "axispointinfo.h"

TeachingPosSetPageModel::TeachingPosSetPageModel(ResourceManager *resource, CommunicationThread * comm, TeachingModePageModel* modePageModel, CommonViewModel *commonView)
    : PageModelBase(resource, comm),
      m_modePageModel(modePageModel),
      m_commonView(commonView)
{
    init();
}

TeachingPosSetPageModel::TeachingPosSetPageModel(QObject *object)
    : PageModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

TeachingPosSetPageModel::~TeachingPosSetPageModel()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &TeachingPosSetPageModel::onOneShotReadFinished);
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingPosSetPageModel::onFinished);
}

void TeachingPosSetPageModel::viewPointControl(bool reverse)
{
    m_operationRecord->recordOpeManualViewPoint(m_viewPoint, reverse);
    QList<int> writeData;
    writeData.append(reverse ? 1 : 0);
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, VIEWPOINT_DO, 0x0001, writeData);
    setOneShotModbusCommand(command);
}

int TeachingPosSetPageModel::setPosition(int pointIndex, int axis, int position, bool fromPosSet)
{
    if ((pointIndex < 0) || (pointIndex >= m_pointList.length())) {
        qWarning() << __func__ << "The pointIndex is out of range." << pointIndex << axis << position;
        return -1;
    }
    PointInfo *pointInfo = static_cast<PointInfo *>(m_pointList.at(pointIndex));
    int max = m_commonView->formatPointMaxValue(axis, pointInfo->pointNo()).toFloat() * 100;
    int min = m_commonView->formatPointMinValue(axis,pointInfo->pointNo()).toFloat() * 100;
    if((position > max || position < min) && position >= 0){
        return -1;
    }

    int address = pointInfo->positionAddress(axis);
    if (address >= 0) {
        QList<int> writeData;
        writeData.append((position & 0x0000FFFF));
        writeData.append((position & 0xFFFF0000) >> 16);
        stModBusCommunicationData command = ModbusCommandGenerator::makeModbusWriteCommand(QModbusPdu::FunctionCode::WriteMultipleRegisters, static_cast<quint16>(address), 2, writeData);
        m_communication->setOneShotModbusCommand(command);
        if (fromPosSet) {
            m_operationRecord->recordSetPositionByPosSet(pointInfo->pointNo(), axis, pointInfo->getPosition(axis), position);
        } else {
            m_operationRecord->recordSetPositionByPosFix(pointInfo->pointNo(), axis, pointInfo->getPosition(axis), position);
        }
        updatePointData(pointIndex);
    } else {
        qWarning() << __func__ << "Invalid address." << address << pointIndex << axis << position;
        return -1;
    }
    return 0;
}

int TeachingPosSetPageModel::setSpeed(int pointIndex, int axis, int speed, bool fromPosSet)
{
    if ((pointIndex < 0) || (pointIndex >= m_pointList.length())) {
        qWarning() << __func__ << "The pointIndex is out of range." << pointIndex << axis << speed;
        return -1;
    }
    PointInfo *pointInfo = static_cast<PointInfo *>(m_pointList.at(pointIndex));
    int address = pointInfo->speedAddress(axis);
    if (address >= 0) {
        m_communication->writeModbusHR(address, &speed, 1);
        if (fromPosSet) {
            m_operationRecord->recordSetSpeedByPosSet(pointInfo->pointNo(), axis, pointInfo->getSpeed(axis), speed);
        } else {
            m_operationRecord->recordSetSpeedByPosFix(pointInfo->pointNo(), axis, pointInfo->getSpeed(axis), speed);
        }
        updatePointData(pointIndex);
    } else {
        qWarning() << __func__ << "Invalid address." << address << pointIndex << axis << speed;
        return -1;
    }
    return 0;
}

void TeachingPosSetPageModel::setViewpoint(bool reverse)
{
    if (m_viewPoint != reverse) {
        m_viewPoint = reverse;
        emit viewpointChanged(m_viewPoint);
    }
}

void TeachingPosSetPageModel::init()
{
    m_axisPointInfo = new AxisPointInfo(m_resource, this);

    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::PosSetPage));
    setPageLabelColor(m_resource->rgb(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::PosSetPage));
    setActivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_POSSET_PAGE_ACTIVATE_ICON_NAME);
    setDeactivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_POSSET_PAGE_DEACTIVATE_ICON_NAME);
    readDataFormatCsv();

    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &TeachingPosSetPageModel::onOneShotReadFinished, Qt::QueuedConnection);
    // Detect language changes to update labels.
    connect(m_resource, &ResourceManager::languageChanged, this, &TeachingPosSetPageModel::onLanguageChanged, Qt::QueuedConnection);

    updatePointData(m_axisPointInfo->getStartFixPointNo(), m_axisPointInfo->getEndFixPointNo());
    updatePointData(m_axisPointInfo->getStartStandardPalletizePointNo(), m_axisPointInfo->getEndStandardPalletizePointNo());
}

void TeachingPosSetPageModel::fileSwitchUpdatePoint()
{
    updatePointData(m_axisPointInfo->getStartFixPointNo(), m_axisPointInfo->getEndFixPointNo());
    updatePointData(m_axisPointInfo->getStartStandardPalletizePointNo(), m_axisPointInfo->getEndStandardPalletizePointNo());
}

void TeachingPosSetPageModel::moveSelectdPoint(int point, bool isRelease)
{
    if(point == -1){
        return;
    }
    QList<int> writeData;
    quint16 address = 4504;///P80 Point
    QList<int> bitWrite;
    QList<int> bitWrite1 = {0,0,0,0,0,0,0,0};

    PointInfo *pointInfo = static_cast<PointInfo *>(m_pointList.at(point));
    for (int i = 0; i < pointInfo->positionList().size(); i++) {
        PointData *pointData1 = static_cast<PointData *>(pointInfo->positionList().at(i));
        PointData *pointData2 = static_cast<PointData *>(pointInfo->sppedList().at(i));
        writeData.append((pointData1->value() & 0x0000FFFF));
        writeData.append((pointData1->value() & 0xFFFF0000) >> 16);
        writeData.append(pointData2->value());
        if(i >= 7){
            continue;
        }
        if((pointData1->value() >= 0x8000000) || (pointData1->value() <= -0x8000000)){
            bitWrite.push_front(0);
        }else{
            bitWrite.push_front(1);
        }
    }
    bitWrite.push_front(1);

    if(isRelease){
        QTimer::singleShot(60,[=](){
            stModBusCommunicationData command1 = ModbusCommandGenerator::makeModbusWriteCommand(QModbusPdu::FunctionCode::WriteMultipleCoils, 2480, bitWrite1.size(), bitWrite1);
            m_communication->setOneShotModbusCommand(command1);
        });
        return;
    }

    stModBusCommunicationData command = ModbusCommandGenerator::makeModbusWriteCommand(QModbusPdu::FunctionCode::WriteMultipleRegisters, static_cast<quint16>(address), writeData.size(), writeData);
    m_communication->setOneShotModbusCommand(command);
    QTimer::singleShot(50,[=](){
        stModBusCommunicationData command1 = ModbusCommandGenerator::makeModbusWriteCommand(QModbusPdu::FunctionCode::WriteMultipleCoils, 2480, bitWrite.size(), bitWrite);
        m_communication->setOneShotModbusCommand(command1);
    });


}

void TeachingPosSetPageModel::readDataFormatCsv()
{
    readPointDispOrder();
    readPenMsgData();
    readPenSetData();
}

void TeachingPosSetPageModel::readPenMsgData()
{
    PointInfo *pointInfo;
    int row;
    for (int i = 0; i < m_numPoint; i++) {
        pointInfo = static_cast<PointInfo *>(m_pointList.at(i));
        // MODIFIED : code value : 25.06.06 by IJ.YI
        row = PEN_MSG_POINT_NAME_START_ROW + (m_pointDispOrderSetting.at(i / 6).value(i % 6) - ((m_pointDispOrderSetting.at(i / 6).value(i % 6) > 80 ? POS_SET_POINT_NUMBER_INDEX_ADJUST_VALUE : 1)));
        pointInfo->setLabelName(m_resource->label(PEN_MSG_GROUP_POS_SET_PAGE, row));
        pointInfo->setLabelColor(m_resource->rgb(PEN_MSG_GROUP_POS_SET_PAGE, row));
    }
}

void TeachingPosSetPageModel::readPenSetData()
{
    readPointAddress();
    readPointStyle();
    readPointSettableAxisTable();
}

void TeachingPosSetPageModel::readPointAddress()
{
    PointInfo *pointInfo;
    int row;
    int positionAddress;
    int speedAddress;
    int axis;
    int numAxis = m_axisInfo->numAxis();
    for (int i = 0; i < m_numPoint; i++) {
        pointInfo = static_cast<PointInfo *>(m_pointList.at(i));
        // MODIFIED : code value : 25.07.04 by IJ.YI
        row = PEN_SET_POS_SET_ADDRESS_START_ROW + (m_pointDispOrderSetting.at(i / 6).value(i % 6) - ((m_pointDispOrderSetting.at(i / 6).value(i % 6) > 80 ? POS_SET_POINT_NUMBER_INDEX_ADJUST_VALUE : 1)));
        QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_POS_SET_PAGE, row);
        for (int j = 0; j < numAxis; j++) {
            axis = j;
            positionAddress = rowData.at(j * 2);
            speedAddress = rowData.at(j * 2 + 1);
            pointInfo->setAddress(axis, positionAddress, speedAddress);
        }
        int startAddress = rowData.at(0);
        int endAddress = rowData.at(rowData.length() - 2);
        m_startAddressPointConversionMap.insert(startAddress, i);
        int dataSize = endAddress - startAddress + 1;
        if (dataSize > 0) {
            pointInfo->setDataSize(static_cast<quint16>(dataSize));
        } else {
           qWarning() << __func__ << "Negative data size." << dataSize << row << rowData.at(0) << rowData.at(rowData.length() - 2);
        }
        int pointNo = rowData.at(16);
        pointInfo->setPointNo(pointNo);
    }
}

void TeachingPosSetPageModel::readPointStyle()
{
    PointInfo *pointInfo;
    int row;
    int positioStyle;
    int speedStyle;
    int axis;
    int numAxis = m_axisInfo->numAxis();
    for (int i = 0; i < m_numPoint; i++) {
        axis = 0;
        pointInfo = static_cast<PointInfo *>(m_pointList.at(i));
        // MODIFIED : code value : 25.06.06 by IJ.YI
        row = PEN_SET_POS_SET_STYLE_START_ROW + (m_pointDispOrderSetting.at(i / 6).value(i % 6) - ((m_pointDispOrderSetting.at(i / 6).value(i % 6) > 80 ? POS_SET_POINT_NUMBER_INDEX_ADJUST_VALUE : 1)));
        QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_POS_SET_PAGE, row);
        for (int j = 0; j < numAxis; j++) {
            axis = j;
            positioStyle = rowData.at(j * 2);
            speedStyle = rowData.at(j * 2 + 1);
            pointInfo->setStyle(axis, positioStyle, speedStyle);
        }
    }
}

// ADD : Add a formula for list display count : 25.06.06 by IJ.YI
void TeachingPosSetPageModel::readPointDispOrder()
{
    m_pointDispOrderSetting = m_resource->getIntValueList(
        PEN_SET_GROUP_POS_SET_PAGE.toInt(), PEN_SET_POS_ADJUST_POINT_DISP_ORDER_ROW, 11);
    QList<int> allOrderSetting;
for (int i = 0; i < m_pointDispOrderSetting.size(); i++) {
    for (int j = 0; j < m_pointDispOrderSetting[i].size(); j++) {
        allOrderSetting.append(m_pointDispOrderSetting[i][j]);
    }
}
QList<QList<int>> result;
QList<int> currentGroup;
for (int i = 0; i < allOrderSetting.size(); ++i) {
    currentGroup.append(allOrderSetting[i]);
    if (currentGroup.size() == 6 || i == allOrderSetting.size() - 1) {
        result.append(currentGroup);
        currentGroup.clear();
    }
}
int sizeIndex = result.size();
if(result.size() < 15){
    QList<int> nullGroup = {};
    for (int i = 0; i < (15 - sizeIndex); i++) {
        result.append(nullGroup);
    }
}
m_pointDispOrderSetting = result;
for(int i = 0; i < 14; i++ ){
    for(int j = 0; j < 6; j++){
        if(m_pointDispOrderSetting.at(i).value(j) != 0){
            PointInfo *pointInfo = new PointInfo(m_axisInfo->numAxis(), this);
            m_pointList.append(pointInfo);
            m_numPoint++;
        }
    }
}
for(int j = 0; j < 4; j++){
    if(m_pointDispOrderSetting.at(14).value(j) != 0){
        PointInfo *pointInfo = new PointInfo(m_axisInfo->numAxis(), this);
        m_pointList.append(pointInfo);
        m_numPoint++;
    }
    }
#if 0
    for(int i = 0; i < 11; i++){
        qDebug() << "value(0):" << m_pointDispOrderSetting.at(i).value(0) << "value(1):" << m_pointDispOrderSetting.at(i).value(1)
                 << "value(2):" << m_pointDispOrderSetting.at(i).value(2) << "value(3):" << m_pointDispOrderSetting.at(i).value(3)
                 << "value(4):" << m_pointDispOrderSetting.at(i).value(4) << "value(5):" << m_pointDispOrderSetting.at(i).value(5)
                 << "value(6):" << m_pointDispOrderSetting.at(i).value(6) << "value(7):" << m_pointDispOrderSetting.at(i).value(7);
    }
#endif
}

void TeachingPosSetPageModel::readPointSettableAxisTable()
{
    for(int i = 0; i < POS_SET_MODE_NUM_MAX; i++){
        m_pointSettableAxisTable[i] = m_resource->getIntHexValueList(
            PEN_SET_GROUP_POS_SET_PAGE.toInt(), PEN_SET_POS_ADJUST_POINT_SETTABLE_AXIS_START_ROW + i*12, 11);
#if 0
    for(int j = 0; j < 11; j++){
        qDebug() << "value(0):" << m_pointSettableAxisTable[i].at(j).value(0) << "value(1):" << m_pointSettableAxisTable[i].at(j).value(1)
                 << "value(2):" << m_pointSettableAxisTable[i].at(j).value(2) << "value(3):" << m_pointSettableAxisTable[i].at(j).value(3)
                 << "value(4):" << m_pointSettableAxisTable[i].at(j).value(4) << "value(5):" << m_pointSettableAxisTable[i].at(j).value(5)
                 << "value(6):" << m_pointSettableAxisTable[i].at(j).value(6) << "value(7):" << m_pointSettableAxisTable[i].at(j).value(7);
    }
#endif
    }
}

QString TeachingPosSetPageModel::formatCurrentPosition(int axis, int value)
{
    if ((axis < 0) || (axis >= m_axisInfo->numAxis())) {
        qWarning() << __func__ << "The axis is out of range." << axis << value;
        return "";
    }
    int format = m_resource->getPenSetIntValue(PEN_SET_GROUP_POS_SET_PAGE, PEN_SET_POS_SET_AXIS_STYLE_START_ROW + axis, 0);
    return m_resource->formatData(value, format, 2);
}

QString TeachingPosSetPageModel::formatPosition(int axis, int value)
{
    if ((axis < 0) || (axis >= m_axisInfo->numAxis())) {
        qWarning() << __func__ << "The axis is out of range." << axis << value;
        return "";
    }
    int format = m_resource->getPenSetIntValue(PEN_SET_GROUP_POS_SET_PAGE, PEN_SET_POS_SET_AXIS_STYLE_START_ROW + axis, 2);
    return m_resource->formatData(value, format, 2);
}

QString TeachingPosSetPageModel::formatPosAdujustSpeed(int axis, int value)
{
    if ((axis < 0) || (axis >= m_axisInfo->numAxis())) {
        qWarning() << __func__ << "The axis is out of range." << axis << value;
        return "";
    }
    int format = m_resource->getPenSetIntValue(PEN_SET_GROUP_POS_SET_PAGE, PEN_SET_POS_ADJUST_CURRENT_POSITION_STYLE_ROW, axis * 2 + 1);
    return m_resource->formatData(value, format);
}

QString TeachingPosSetPageModel::formatPosition(int pointIndex, int axis, int value)
{
    if ((axis < 0) || (axis >= m_axisInfo->numAxis())) {
        qWarning() << __func__ << "The axis is out of range." << axis << value;
        return "";
    }
//    if (pointIndex < 0 || pointIndex >= m_numPoint) {
//        qWarning() << __func__ << "The pointIndex is out of range." << pointIndex;
//        return "";
//    }
    if(pointIndex >=119 && pointIndex < 207){
        pointIndex = pointIndex - 119;
    }
    int format = m_resource->getPenSetIntValue(PEN_SET_GROUP_POS_SET_PAGE, PEN_SET_POS_SET_STYLE_START_ROW + pointIndex, axis * 2);
    return ((format <= 0) ? "" : m_resource->formatData(value, format, 2, true));
}

QString TeachingPosSetPageModel::formatPosAdujustSpeed(int pointIndex, int axis, int value)
{
    if ((axis < 0) || (axis >= m_axisInfo->numAxis())) {
        qWarning() << __func__ << "The axis is out of range." << axis << value;
        return "";
    }
    if (pointIndex < 0 || pointIndex >= m_numPoint) {
        qWarning() << __func__ << "The pointIndex is out of range." << pointIndex;
        return "";
    }
    int format = m_resource->getPenSetIntValue(PEN_SET_GROUP_POS_SET_PAGE, PEN_SET_POS_SET_STYLE_START_ROW + pointIndex, axis * 2 + 1);
    return ((format <= 0) ? "" : m_resource->formatData(value, format));
}

int TeachingPosSetPageModel::getPointNo(int pageNum, int index)
{
    if (pageNum < 0 || pageNum > 10) return -1;
    if (index < 0 || index > 7) return -1;

    return (m_pointDispOrderSetting.at(pageNum).value(index) - 1);
}

int TeachingPosSetPageModel::getPointSettable(int pointIndex)
{
    if(pointIndex >=119 && pointIndex < 207){
        pointIndex = pointIndex - 119;
    }
    QList<QObject *> modeDataList = m_modePageModel->modeDataList();
    int settableValue;
    int pointSettable = 0xFF;//有mode on时返回
    int pointSettableoff = 0xFF;//所有mode off时返回
    bool ismodeon = false;
    for (int mode = 0; mode < POS_SET_MODE_NUM_MAX; mode++) {
        settableValue = m_pointSettableAxisTable[mode].at(pointIndex/8).value(pointIndex%8) & 0xFF;//64个模式对应各自的point设置
        if (settableValue != 0) {
            bool modeOn = static_cast<TeachingModeData *>(modeDataList.at(mode))->status();
            if (modeOn) {
                if(!ismodeon){
                    pointSettable &= settableValue;
                }else {
                    pointSettable |= settableValue;
                }
                ismodeon = true;
            } else {
                pointSettableoff &= ~settableValue;
            }
            if (pointSettable == 0) {
                break;
            }
        }
    }
    if(ismodeon){
        return pointSettable;
    }else {
        return pointSettableoff;
    }
}

void TeachingPosSetPageModel::updateMaxPosition()
{
    for (int i = 0; i < AXIS_ADDRESS_LIST.length(); i++) {
        AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
        QList<quint16> addressList = AXIS_ADDRESS_LIST.at(i);
        QString maxPosition = QString::number(m_modbus->getLongReceiveDataHR(addressList.at(AxisAddressType::MaxPosition)));
        m_axisInfo->setMaxPosition(axis, maxPosition);
    }
}

void TeachingPosSetPageModel::modbusGetMaxPosition()
{
    quint16 numAxis = static_cast<quint16>(m_axisInfo->numAxis());
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, AXIS_MAX_POSITION_TOP_HR, 6 * numAxis);
    setOneShotModbusCommand(command);
}

void TeachingPosSetPageModel::onActivate()
{
    connect(m_communication, &CommunicationThread::finish, this, &TeachingPosSetPageModel::onFinished, Qt::QueuedConnection);
    connect(m_modbus, &ModbusManager::endModbusPageChange, this, &TeachingPosSetPageModel::endModbusPageChange, Qt::DirectConnection);
    modbusGetMaxPosition();
}

void TeachingPosSetPageModel::onDeactivate()
{
    disconnect(m_modbus, &ModbusManager::endModbusPageChange, this, &TeachingPosSetPageModel::endModbusPageChange);
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingPosSetPageModel::onFinished);
}

void TeachingPosSetPageModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::PosSetPage));
    readPenMsgData();
}

int TeachingPosSetPageModel::updatePointData(int point)
{
    if ((point < 0) || (point >= m_pointList.length())) {
        qWarning() << __func__ << "The point is out of range." << point;
        return -1;
    }
    PointInfo *pointInfo = static_cast<PointInfo *>(m_pointList.at(point));
    quint16 dataSize = pointInfo->dataSize();
    int startHR = pointInfo->positionAddress(0);

    if (startHR >= 0){
        stModBusCommunicationData readCommand;
        readCommand = ModbusCommandGenerator::makeModbusReadCommand(
                    QModbusPdu::FunctionCode::ReadHoldingRegisters,
                    static_cast<quint16>(startHR),
                    dataSize);
        setOneShotModbusCommand(readCommand);
    } else {
        qWarning() << __func__ << "Invalid address." << startHR << point;
        return -1;
    }
    return 0;
}

void TeachingPosSetPageModel::updatePointData(int startPointNo, int endPointNo)
{
    quint16 dataSize;
    quint16 pointDataSize = m_axisPointInfo->pointDataSize();
    int remainPoints = endPointNo - startPointNo + 1;
    int point = startPointNo;
    while (remainPoints > 0) {
        quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(point, 0);
        if (remainPoints >= 5) {
            dataSize = 5 * pointDataSize;
            point += 5;
            remainPoints -= 5;
        } else {
            dataSize = static_cast<quint16>(remainPoints * pointDataSize);
            point += remainPoints;
            remainPoints = 0;
        }
        stModBusCommunicationData readCommand;
        readCommand = ModbusCommandGenerator::makeModbusReadCommand(
                    QModbusPdu::FunctionCode::ReadHoldingRegisters,
                    hrAddress,
                    dataSize);
        m_communication->setDelayCommandList(Enums::Pages::TeachingPosSet, readCommand);
    }
}

void TeachingPosSetPageModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerTeachingPosSetPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void TeachingPosSetPageModel::onFinished()
{
    if (m_pageChanging == false && m_modbus->getIsPageChanging() == false) {
        bool servo;
        for (int i = 0; i < POS_SET_ADDRESS_LIST.length(); i++) {
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QList<quint16> addressList = POS_SET_ADDRESS_LIST.at(i);
            servo = m_modbus->getReceiveDataDI(addressList.at(static_cast<int>(PosSetAddressType::servodi))) == 0;
            m_axisInfo->setServo(axis, servo);
        }
        setViewpoint(m_modbus->getReceiveDataRG(VIEWPOINT_SR) & VIEWPOINT_SR_BIT_MASK);
        setDataLoaded(true);
        if(m_resource->axisorder().size() > 5) {
            for(int i = 5; i < 8; i++){
                AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
                QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
                m_axisInfo->setPosition(axis,position);
            }
        }
    }
}

void TeachingPosSetPageModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    Q_UNUSED(oneShotReadCaller)
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if (m_startAddressPointConversionMap.contains(command.readAddress)) {
            quint16 pointDataSize = m_axisPointInfo->pointDataSize();
            int remainDataSize = command.comSize;
            int hrAddress = command.readAddress;
            while (remainDataSize > 0) {
                if (m_startAddressPointConversionMap.contains(hrAddress)) {
                    int point = m_startAddressPointConversionMap.value(hrAddress);
                    PointInfo *pointInfo = static_cast<PointInfo *>(m_pointList.at(point));
                    if (hrAddress == pointInfo->positionAddress(0)) {
                        int numAxis = m_axisInfo->numAxis();
                        for (int axis = 0; axis < numAxis; axis++) {
                            pointInfo->setPosition(axis, m_modbus->getLongReceiveDataHR(static_cast<quint16>(pointInfo->positionAddress(axis))));
                            pointInfo->setSpeed(axis, m_modbus->getReceiveDataHR(static_cast<quint16>(pointInfo->speedAddress(axis))));
                        }
                    }
                }
                hrAddress += pointDataSize;
                remainDataSize -= pointDataSize;
            }
        } else if (command.readAddress == AXIS_MAX_POSITION_TOP_HR) {
            updateMaxPosition();
        }
    }
}

void TeachingPosSetPageModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void TeachingPosSetPageModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}

void TeachingPosSetPageModel::endModbusPageChange(int mainScreenId, int subScreenId)
{
    if ((mainScreenId == 11) && ((subScreenId == 0) || (subScreenId == 1))) {
        m_pageChanging = false;
    }
}
