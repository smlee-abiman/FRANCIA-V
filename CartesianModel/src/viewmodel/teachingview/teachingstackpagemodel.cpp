#include "teachingstackpagemodel.h"
#include "teachingviewdef.h"

TeachingStackPageModel::TeachingStackPageModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : PageModelBase(resource, comm)
{
    m_commonView = commonView;
    if (m_resource){
        mCsvReader = m_resource->getCsvReaderInstance();
    }
    init();
    initSettingPenSetData();
    //initPageCommand();
    initPointList();
    for(int j = 0; j < 4; j++){
        modbusGetPointData(209, j);
    }
}

TeachingStackPageModel::TeachingStackPageModel(QObject *object)
    : PageModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

TeachingStackPageModel::~TeachingStackPageModel()
{
    qDeleteAll(m_pointList);
}

void TeachingStackPageModel::init()
{
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::StackPage));
    setPageLabelColor(m_resource->rgb(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::StackPage));
    setActivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_STACK_PAGE_ACTIVATE_ICON_NAME);
    setDeactivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR +TEACHING_STACK_PAGE_DEACTIVATE_ICON_NAME);
    // Detect language changes to update labels.
    connect(m_resource, &ResourceManager::languageChanged, this, &TeachingStackPageModel::onLanguageChanged, Qt::QueuedConnection);
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &TeachingStackPageModel::onOneShotReadFinished, Qt::QueuedConnection);
}

void TeachingStackPageModel::initSettingPenSetData()
{
    if (!m_resource || !mCsvReader || !m_commonView){
        return;
    }
    m_axisInfoDataFormat            = m_resource->formatList(12, 14, 8);
    m_standardAxisOrder             = m_resource->getIntValueList(12, 23, 8);
    m_decelerateInfoDataFormat      = m_resource->formatList(12, 31, 1);
    for(int i = 0; i < 7; i++){
        m_freePalletizeColorList.append(m_resource->dataColor(12, 32 + i));
    }
    m_settableFreeFPNumMax          = m_resource->getIntValueList(12, 45, 8);
    m_axisInfoFreeDataFormat        = m_resource->formatList(12, 53, 3);
    mCmnAxisOrder                   = m_commonView->getAxisOrder();
#if 0
    for(int i = 0; i < 8; i++){
        qDebug() << "value(0):" << m_axisInfoDataFormat.at(i).value(0) << "value(1):" << m_axisInfoDataFormat.at(i).value(1)
                 << "value(2):" << m_axisInfoDataFormat.at(i).value(2) << "value(3):" << m_axisInfoDataFormat.at(i).value(3);
        qDebug() << "value(4):" << m_axisInfoDataFormat.at(i).value(4) << "value(5):" << m_axisInfoDataFormat.at(i).value(5)
                 << "value(6):" << m_axisInfoDataFormat.at(i).value(6) << "value(7):" << m_axisInfoDataFormat.at(i).value(7);
    }
    for(int i = 0; i < 3; i++){
        qDebug() << "value(0):" << m_axisInfoFreeDataFormat.at(i).value(0) << "value(1):" << m_axisInfoFreeDataFormat.at(i).value(1)
                 << "value(2):" << m_axisInfoFreeDataFormat.at(i).value(2) << "value(3):" << m_axisInfoFreeDataFormat.at(i).value(3);
        qDebug() << "value(4):" << m_axisInfoFreeDataFormat.at(i).value(4) << "value(5):" << m_axisInfoFreeDataFormat.at(i).value(5)
                 << "value(6):" << m_axisInfoFreeDataFormat.at(i).value(6) << "value(7):" << m_axisInfoFreeDataFormat.at(i).value(7);
    }
    qDebug() << "value(0):" << m_decelerateInfoDataFormat.at(0).value(0) << "value(1):" << m_decelerateInfoDataFormat.at(0).value(1)
             << "value(2):" << m_decelerateInfoDataFormat.at(0).value(2) << "value(3):" << m_decelerateInfoDataFormat.at(0).value(3);
    for(int i = 0; i < 8; i++){
        qDebug() << "value(0):" << m_settableFreeFPNumMax.at(i).value(0);
    }
#endif
#if 0
    for(int i = 0; i < mCmnAxisOrder.count(); i++){
        qDebug() << "TeachingStackPageModel mCmnAxisOrder value:" << mCmnAxisOrder.at(i);
    }
#endif
#if 0
    for(int i = 0; i < m_standardAxisOrder.count(); i++){
        qDebug() << "value(0):" << m_standardAxisOrder.at(i).value(0) << "value(1):" << m_standardAxisOrder.at(i).value(1)
                 << "value(2):" << m_standardAxisOrder.at(i).value(2)<< "value(2):" << m_standardAxisOrder.at(i).value(3)<< "value(2):" << m_standardAxisOrder.at(i).value(4);
    }
#endif

    int axisCnt = 0;
    if (mCmnAxisOrder.count() < mAxisCount){
        axisCnt = mCmnAxisOrder.count();
    } else {
        axisCnt = mAxisCount;
    }
    //qDebug() << "TeachingStackPageModel axisCnt:" << axisCnt;
    for(int i = 0; i < axisCnt; i++){
        m_axisName.append((int)(mCmnAxisOrder.at(i) + 1));
    }
    for(int i = 0; i < mAxisCount; i++){
        m_axisOrder.append(0);
    }
}

void TeachingStackPageModel::initPageCommand()
{
    if (!m_communication || (m_curTabIndex < 0 || m_curTabIndex >= PalletizeType::PalletizeTypeMax)) {
        qCritical("%s:%d m_communication is invalid m_curTabIndex[%d]", __func__, __LINE__, m_curTabIndex);
        return;
    }
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:{
        qDebug("%s:%d m_curTabIndex[%d]", __func__, __LINE__, m_curTabIndex);
        stModBusCommunicationData readCommand[STANDARD_PALLETIZE_PERIODIC_MONITOR_CMD_LIST_NUM];
        for(int i = 0; i < STANDARD_PALLETIZE_PERIODIC_MONITOR_CMD_LIST_NUM; i++){
            readCommand[i] = ModbusCommandGenerator::makeModbusReadCommand(
                                initStandardModbusPageCmd[i].funcCode,
                                initStandardModbusPageCmd[i].address,
                                initStandardModbusPageCmd[i].size
                                );
            m_communication->setPageReadCommandList(Enums::Pages::TeachingStandardPalletize, readCommand[i]);
        }
        break;
    }
    case PalletizeType::PalletizeTypeFree:{
        qDebug("%s:%d m_curTabIndex[%d]", __func__, __LINE__, m_curTabIndex);
        stModBusCommunicationData readCommand[FREE_PALLETIZE_PERIODIC_MONITOR_CMD_LIST_NUM];
        for(int i = 0; i < FREE_PALLETIZE_PERIODIC_MONITOR_CMD_LIST_NUM; i++){
            ushort adjSize = 0;
            ushort adjAddr = 0;
            adjAddr = m_curFreePalletizeNum * FREE_PALLETIZE_POINT_INFO_LIST.at(m_curFreePalletizeNum).value(1);
            if (mCurPage == 1){
                if(i == FREE_PALLETIZE_LOW_POINT_CMD_LIST_INDEX){
                    adjSize = 0x0030;
                }
            } else {
                if(i == FREE_PALLETIZE_LOW_POINT_CMD_LIST_INDEX
                    || i == FREE_PALLETIZE_LOW_POINT_CMD_LIST_INDEX){
                    adjAddr += (8 * (mCurPage - 1) - 2) * 24;
                }
            }
            readCommand[i] = ModbusCommandGenerator::makeModbusReadCommand(
                                initFreeModbusPageCmd[i].funcCode,
                                (initFreeModbusPageCmd[i].address + adjAddr),
                                (initFreeModbusPageCmd[i].size - adjSize)
                                );
            m_communication->setPageReadCommandList(Enums::Pages::TeachingFreePalletize, readCommand[i]);
        }
        break;
    }
    default:
        break;
    }
}

void TeachingStackPageModel::initPointList()
{
    m_pointList.clear();
    for (int i = 0; i < 8; i++) {
        PointInfo *pointInfo = new PointInfo(mAxisCount, this);
        m_pointList.append(pointInfo);
    }
}

void TeachingStackPageModel::initPointNameList()
{
    if ((m_curFreePalletizeNum < 0 || m_curFreePalletizeNum >= FREE_PALLETIZE_NUM_MAX)
        || FREE_PALLETIZE_POINT_INFO_LIST.count() <= 0){
        qCritical("%s:%d m_curFreePalletizeNum[%d] or FREE_PALLETIZE_POINT_INFO_LIST count[%d] is invalid",
            __func__, __LINE__, m_curFreePalletizeNum, FREE_PALLETIZE_POINT_INFO_LIST.count());
    }
    m_pointNameList.clear();
    m_pointNameColorList.clear();
    int maxNum = FREE_PALLETIZE_POINT_INFO_LIST.at(m_curFreePalletizeNum).value(1);
    int totalPage = (((maxNum + 2) % 8) ? maxNum / 8 + 1: maxNum / 8);
    for(int i = 0; i < totalPage * 8; i++){
        m_pointNameList.append("");
        m_pointNameColorList.append("");
    }
    updateFreePalletizePointNameAndColorList();
}

void TeachingStackPageModel::onActivate()
{
    initPointNameList();
    startModbusMonitoring();
}

void TeachingStackPageModel::onDeactivate()
{
    stopModbusMonitoring();
}

void TeachingStackPageModel::startModbusMonitoring()
{
    if (!m_communication) return;
    connect(m_communication, &CommunicationThread::finish,
        this, &TeachingStackPageModel::onFinished, Qt::QueuedConnection);
    connect(m_modbus, &ModbusManager::endModbusPageChange, this, &TeachingStackPageModel::endModbusPageChange, Qt::DirectConnection);
}

void TeachingStackPageModel::stopModbusMonitoring()
{
    if (!m_communication) return;
    disconnect(m_communication, &CommunicationThread::finish,
        this, &TeachingStackPageModel::onFinished);
    disconnect(m_modbus, &ModbusManager::endModbusPageChange, this, &TeachingStackPageModel::endModbusPageChange);
}

void TeachingStackPageModel::deinitPageCommand()
{
    if (!m_communication) return;
    Enums::Pages page;
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        page = Enums::Pages::TeachingStandardPalletize;
        break;
    case PalletizeType::PalletizeTypeFree:
        page = Enums::Pages::TeachingFreePalletize;
        break;
    default:
        return;
    }
    m_communication->resetPageReadCommandList(page);
}

void TeachingStackPageModel::reinitPageCommand()
{
    deinitPageCommand();
    initPageCommand();
#if 0
    for(int i = 0; i < 8; i++){
        PointInfo *pointInfo = static_cast<PointInfo *>(m_pointList.at(i));
        for(int j = 0; j < mAxisCount; j++){
            QString space = " ";
            pointInfo->setPosition(j, space);
        }
    }
#endif
}

void TeachingStackPageModel::updateStorageMethod(stModBusCommunicationData command)
{
    if (!m_modbus || command.readAddress != PALLETIZE_STORAGE_METHOD_DI){
        return;
    }
    setStorageMethod(m_modbus->getReceiveDataDI(PALLETIZE_STORAGE_METHOD_DI));
}

void TeachingStackPageModel::updateDecelerateInfo(stModBusCommunicationData command)
{
    if (!m_resource) return;
    switch(command.readAddress){
    case PALLETIZE_DOWN_DECELERATE_POS_HR_AXIS_M :
        setAxisMdownDeceleratePos(m_resource->formatData(
            (int)((command.readBuf[1] << 16) + command.readBuf[0]), m_decelerateInfoDataFormat.at(0).value(0), 2));
        break;
    case PALLETIZE_DOWN_DECELERATE_SPEED_HR_AXIS_M :
        setAxisMdownDecelerateSpeed(m_resource->formatData((short)command.readBuf[0], m_decelerateInfoDataFormat.at(0).value(1)));
        break;
    case PALLETIZE_UP_DECELERATE_POS_HR_AXIS_M :
        setAxisMupDeceleratePos(m_resource->formatData(
            (int)((command.readBuf[1] << 16) + command.readBuf[0]), m_decelerateInfoDataFormat.at(0).value(2), 2));
        break;
    case PALLETIZE_UP_DECELERATE_SPEED_HR_AXIS_M :
        setAxisMupDecelerateSpeed(m_resource->formatData((short)command.readBuf[0], m_decelerateInfoDataFormat.at(0).value(3)));
        break;
    case PALLETIZE_DOWN_DECELERATE_POS_HR_AXIS_S :
        setAxisSdownDeceleratePos(m_resource->formatData(
            (int)((command.readBuf[1] << 16) + command.readBuf[0]), m_decelerateInfoDataFormat.at(0).value(0), 2));
        break;
    case PALLETIZE_DOWN_DECELERATE_SPEED_HR_AXIS_S :
        setAxisSdownDecelerateSpeed(m_resource->formatData((short)command.readBuf[0], m_decelerateInfoDataFormat.at(0).value(1)));
        break;
    case PALLETIZE_UP_DECELERATE_POS_HR_AXIS_S :
        setAxisSupDeceleratePos(m_resource->formatData(
            (int)((command.readBuf[1] << 16) + command.readBuf[0]), m_decelerateInfoDataFormat.at(0).value(2), 2));
        break;
    case PALLETIZE_UP_DECELERATE_SPEED_HR_AXIS_S :
        setAxisSupDecelerateSpeed(m_resource->formatData((short)command.readBuf[0], m_decelerateInfoDataFormat.at(0).value(3)));
        break;
    default:
        return;
    }
}

void TeachingStackPageModel::updatePointDataInfo(stModBusCommunicationData command)
{
    if (!m_resource) return;
    if (m_curFreePalletizeNum < 0 || m_curTabIndex >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
    ushort targetPointCountAddr = FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(0);
    ushort targetPointValueAddr = FREE_PALLETIZE_POINT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(0);
    //qDebug("%s:%d readAddress[%d] readBuf[%d] targetPointCountAddr[%d] targetPointValueAddr[%d]",
    //    __func__, __LINE__, command.readAddress, command.readBuf[0], targetPointCountAddr, targetPointValueAddr);
    if (command.readAddress == targetPointCountAddr){
        setFreePointCount(m_resource->formatData((short)command.readBuf[0], m_axisInfoFreeDataFormat.at(0).value(0)));
    }
    if (command.readAddress == targetPointValueAddr){
        setFreePointValue(m_resource->formatData((short)command.readBuf[0], m_axisInfoFreeDataFormat.at(0).value(1)));
    }
}

void TeachingStackPageModel::recieveStandardPalletizeStartPos(int palletizeNum, int index, int axisNum)
{
    (void)index;
    qDebug("%s:%d palletizeNum[%d] axisNum[%d]", __func__, __LINE__, palletizeNum, axisNum);
    if (!m_communication) return;
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (axisNum < 0 || axisNum >= 5){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    qDebug("%s:%d addr[%d]", __func__, __LINE__, (PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.at(palletizeNum).value(0 + axisNum * 2)));
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters,
            PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.at(palletizeNum).value(0 + axisNum * 2), 0x0002);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::recieveStandardPalletizeStartSpeed(int palletizeNum, int index, int axisNum)
{
    (void)index;
    qDebug("%s:%d palletizeNum[%d] axisNum[%d]", __func__, __LINE__, palletizeNum, axisNum);
    if (!m_communication) return;
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (axisNum < 0 || axisNum >= 5){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    qDebug("%s:%d addr[%d]", __func__, __LINE__, (PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.at(palletizeNum).value(1 + axisNum * 2)));
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters,
            PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.at(palletizeNum).value(1 + axisNum * 2), 0x0001);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::recieveStandardPalletizePointValue(int palletizeNum, int pointAxisNum)
{
    if (!m_communication) return;
    qDebug("%s:%d palletizeNum[%d] pointAxisNum[%d]", __func__, __LINE__, palletizeNum, pointAxisNum);
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_COUNT_POINT_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (pointAxisNum < 0 || pointAxisNum >= 3){
        qCritical("%s:%d pointAxisNum[%d] is invalid", __func__, __LINE__, pointAxisNum);
        return;
    }
    //qDebug("%s:%d addr[%d]", __func__, __LINE__, (PALLETIZE_COUNT_POINT_ADDR_LIST.at(palletizeNum).value(1 + pointAxisNum * 2)));
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters,
            PALLETIZE_COUNT_POINT_ADDR_LIST.at(palletizeNum).value(1 + pointAxisNum * 2), 0x0001);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::recieveFreePalletizePointValue(int palletizeNum, int pointAxisNum)
{
    qDebug("%s:%d palletizeNum[%d] pointAxisNum[%d]", __func__, __LINE__, palletizeNum, pointAxisNum);
    if (palletizeNum < 0 || palletizeNum > FREE_PALLETIZE_POINT_VALUE_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (pointAxisNum < 0 || pointAxisNum >= 3){
        qCritical("%s:%d pointAxisNum[%d] is invalid", __func__, __LINE__, pointAxisNum);
        return;
    }
    //qDebug("%s:%d addr[%d]", __func__, __LINE__, FREE_PALLETIZE_POINT_VALUE_ADDR_LIST.at(palletizeNum).value(0));
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters,
            FREE_PALLETIZE_POINT_VALUE_ADDR_LIST.at(palletizeNum).value(0), 0x0001);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::updateStandardPalletizePointValue(int palletizeNum, int pointAxisNum, int writeValue)
{
    if (!m_communication) return;
    //qDebug("%s:%d palletizeNum[%d] pointAxisNum[%d] writeValue[%d]", __func__, __LINE__, palletizeNum, pointAxisNum, writeValue);
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_COUNT_POINT_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (pointAxisNum < 0 || pointAxisNum >= 3){
        qCritical("%s:%d pointAxisNum[%d] is invalid", __func__, __LINE__, pointAxisNum);
        return;
    }
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    writeData.append(writeValue);
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            PALLETIZE_COUNT_POINT_ADDR_LIST.at(palletizeNum).value(1 + pointAxisNum * 2),
            0x0001,
            writeData);
    setOneShotModbusCommand(writeCommand);
}

void TeachingStackPageModel::updateFreePalletizePointValue(int palletizeNum, int pointAxisNum, int writeValue)
{
    if (!m_communication) return;
    //qDebug("%s:%d palletizeNum[%d] pointAxisNum[%d] writeValue[%d]", __func__, __LINE__, palletizeNum, pointAxisNum, writeValue);
    if (palletizeNum < 0 || palletizeNum > FREE_PALLETIZE_POINT_VALUE_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (pointAxisNum < 0 || pointAxisNum >= 3){
        qCritical("%s:%d pointAxisNum[%d] is invalid", __func__, __LINE__, pointAxisNum);
        return;
    }
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    writeData.append(writeValue);
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            FREE_PALLETIZE_POINT_VALUE_ADDR_LIST.at(palletizeNum).value(0),
            0x0001,
            writeData);
    setOneShotModbusCommand(writeCommand);
}

void TeachingStackPageModel::recieveFreePalletizeCorrectValue(int palletizeNum, int axisNum)
{
    //qDebug("%s:%d palletizeNum[%d] axisNum[%d]", __func__, __LINE__, palletizeNum, axisNum);
    if (palletizeNum < 0 || palletizeNum > FREE_PALLETIZE_AXIS_CORRECT_VALUE_SR_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters,
            FREE_PALLETIZE_AXIS_CORRECT_VALUE_SR_ADDR_LIST.at(palletizeNum).value(axisNum), 0x0002);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::recieveFreePalletizeStartSpeed(int palletizeNum, int index, int axisNum)
{
    (void)index;
    //qDebug("%s:%d palletizeNum[%d] axisNum[%d]", __func__, __LINE__, palletizeNum, axisNum);
    if (palletizeNum < 0 || palletizeNum > FREE_PALLETIZE_AXIS_SPEED_SR_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    //qDebug("%s:%d addr[%d]", __func__, __LINE__, FREE_PALLETIZE_AXIS_SPEED_SR_ADDR_LIST.at(palletizeNum).value(axisNum));
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters,
            FREE_PALLETIZE_AXIS_SPEED_SR_ADDR_LIST.at(palletizeNum).value(axisNum), 0x0001);
    setOneShotModbusCommand(readCommand);
}

ushort TeachingStackPageModel::getFreePalletizeFPNumAccumulationValue(int palletizeNum)
{
    ushort accumulationValue = 0;
    if (palletizeNum < 0 || palletizeNum > 7){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return 0;
    }
    for(int i = 0; i < palletizeNum; i++){
        accumulationValue += getFreePalletizeFPNumMax(i);
    }
    return accumulationValue;
}

void TeachingStackPageModel::updateStandardPalletizeAxisOrderInfo(stModBusCommunicationData command)
{
    if (m_curStandardPalletizeNum < 0 || m_curStandardPalletizeNum > 7){
        qCritical("%s:%d m_curStandardPalletizeNum[%d]", __func__, __LINE__, m_curStandardPalletizeNum);
        return;
    }
    int axisOrder = -1;
    for(int i = 0; i < 6; i++){
        if(orderNumTable[i].orderValue[m_curStandardPalletizeNum] == command.readBuf[0]){
            axisOrder = i;
            break;
        }
    }
    if (axisOrder >= 0){
        setCurAxisOrder(axisOrder);
    }
}

void TeachingStackPageModel::updateStandardPalletizeAxisTypeInfo(stModBusCommunicationData command)
{
    if (m_curStandardPalletizeNum < 0 || m_curStandardPalletizeNum > 7){
        qCritical("%s:%d m_curStandardPalletizeNum[%d]", __func__, __LINE__, m_curStandardPalletizeNum);
        return;
    }
    QString str = QString::number(command.readBuf[0],2);

    int size = str.length();
    for(int i = 0; i < (8 - size); i++){
        str.insert(0,'0');
    }
    if(str.at(2) == '0' && str.at(3) == '1'){
        m_stackType[m_curStandardPalletizeNum] = 0;
    }else if(str.at(2) == '1' && str.at(3) == '0'){
        m_stackType[m_curStandardPalletizeNum] = 1;
    }else if(str.at(2) == '1' && str.at(3) == '1'){
        m_stackType[m_curStandardPalletizeNum] = 2;
    }
    emit stackTypeChanged();
}

void TeachingStackPageModel::updateAxis6_8Position()
{
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void TeachingStackPageModel::modbusGetStackType(int palletizeNum)
{
    if (palletizeNum < 0 || palletizeNum >= PALLETIZE_ORDER_NUM_MAX){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    //qDebug("%s:%d palletizeNum[%d] addr[%d]", __func__, __LINE__, palletizeNum, orderNumTable[0].hrNumber[palletizeNum]);
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, orderNumTable[0].hrNumber[palletizeNum], 0x0001);
    setOneShotModbusCommand(readCommand);
}


void TeachingStackPageModel::setStorageMethod(bool storageMethod)
{
    if (m_storageMethod == storageMethod){
#if 0
        if (mStorageMethodMonitorCnt >= 5){
           stopModbusMonitorStorageMethod();
        }
        mStorageMethodMonitorCnt++;
#endif
        return;
    }

    m_storageMethod = storageMethod;
    emit storageMethodChanged(m_storageMethod);
#if 0
    stopModbusMonitorStorageMethod();
#endif
}

void TeachingStackPageModel::setAxisMdownDeceleratePos(QString downDeceleratePos)
{
    if (m_axisMdownDeceleratePos == downDeceleratePos){
        return;
    }

    m_axisMdownDeceleratePos = downDeceleratePos;
    emit axisMdownDeceleratePosChanged(m_axisMdownDeceleratePos);
}

void TeachingStackPageModel::setAxisMdownDecelerateSpeed(QString downDecelerateSpeed)
{
    if (m_axisMdownDecelerateSpeed == downDecelerateSpeed){
        return;
    }

    m_axisMdownDecelerateSpeed = downDecelerateSpeed;
    emit axisMdownDecelerateSpeedChanged(m_axisMdownDecelerateSpeed);
}

void TeachingStackPageModel::setAxisMupDeceleratePos(QString upDeceleratePos)
{
    if (m_axisMupDeceleratePos == upDeceleratePos){
        return;
    }

    m_axisMupDeceleratePos = upDeceleratePos;
    emit axisMupDeceleratePosChanged(m_axisMupDeceleratePos);
}

void TeachingStackPageModel::setAxisMupDecelerateSpeed(QString upDecelerateSpeed)
{
    if (m_axisMupDecelerateSpeed == upDecelerateSpeed){
        return;
    }

    m_axisMupDecelerateSpeed = upDecelerateSpeed;
    emit axisMupDecelerateSpeedChanged(m_axisMupDecelerateSpeed);
}

void TeachingStackPageModel::setAxisSdownDeceleratePos(QString downDeceleratePos)
{
    if (m_axisSdownDeceleratePos == downDeceleratePos){
        return;
    }

    m_axisSdownDeceleratePos = downDeceleratePos;
    emit axisSdownDeceleratePosChanged(m_axisSdownDeceleratePos);
}

void TeachingStackPageModel::setAxisSdownDecelerateSpeed(QString downDecelerateSpeed)
{
    if (m_axisSdownDecelerateSpeed == downDecelerateSpeed){
        return;
    }

    m_axisSdownDecelerateSpeed = downDecelerateSpeed;
    emit axisSdownDecelerateSpeedChanged(m_axisSdownDecelerateSpeed);
}

void TeachingStackPageModel::setAxisSupDeceleratePos(QString upDeceleratePos)
{
    if (m_axisSupDeceleratePos == upDeceleratePos){
        return;
    }

    m_axisSupDeceleratePos = upDeceleratePos;
    emit axisSupDeceleratePosChanged(m_axisSupDeceleratePos);
}

void TeachingStackPageModel::setAxisSupDecelerateSpeed(QString upDecelerateSpeed)
{
    if (m_axisSupDecelerateSpeed == upDecelerateSpeed){
        return;
    }

    m_axisSupDecelerateSpeed = upDecelerateSpeed;
    emit axisSupDecelerateSpeedChanged(m_axisSupDecelerateSpeed);
}

void TeachingStackPageModel::setCurStandardPalletizeNum(int curStandardPalletizeNum)
{
    if (m_curStandardPalletizeNum == curStandardPalletizeNum){
        return;
    }
    m_curStandardPalletizeNum = curStandardPalletizeNum;
    emit curStandardPalletizeNumChanged(m_curStandardPalletizeNum);
}

void TeachingStackPageModel::setAxisOrder(QList<int> axisOrder)
{
    if (m_axisOrder == axisOrder){
        return;
    }
    m_axisOrder = axisOrder;
    emit axisOrderChanged(m_axisOrder);
}

void TeachingStackPageModel::setCurTabIndex(int curTabIndex)
{
    //qDebug("%s:%d m_curTabIndex[%d] curTabIndex[%d]", __func__, __LINE__, m_curTabIndex, curTabIndex);
    if (m_curTabIndex == curTabIndex){
        return;
    }
    m_curTabIndex = curTabIndex;
    emit curTabIndexChanged(m_curTabIndex);
}

void TeachingStackPageModel::setCurFreePalletizeNum(int curFreePalletizeNum)
{
    if (m_curFreePalletizeNum == curFreePalletizeNum){
        return;
    }
    m_curFreePalletizeNum = curFreePalletizeNum;
    initPointNameList();
    //reinitPageCommand();
    emit curFreePalletizeNumChanged(m_curFreePalletizeNum);
}

void TeachingStackPageModel::setAxisName(QList<int> axisName)
{
    if (m_axisName == axisName){
        return;
    }
    m_axisName = axisName;
    emit axisNameChanged(m_axisName);
}

void TeachingStackPageModel::setFreePalletizeColorList(QList<QString> freePalletizeColorList)
{
    if (m_freePalletizeColorList == freePalletizeColorList){
        return;
    }

    m_freePalletizeColorList = freePalletizeColorList;
    emit freePalletizeColorListChanged(m_freePalletizeColorList);
}

void TeachingStackPageModel::setPointNameList(QList<QString> pointNameList)
{
    if (m_pointNameList == pointNameList){
        return;
    }
    m_pointNameList = pointNameList;
    emit pointNameListChanged(m_pointNameList);
}

void TeachingStackPageModel::setFreePointCount(QString freePointCount)
{
    if (m_freePointCount == freePointCount){
        return;
    }

    m_freePointCount = freePointCount;
    emit freePointCountChanged(m_freePointCount);
}
void TeachingStackPageModel::setFreePointValue(QString freePointValue)
{
    if (m_freePointValue == freePointValue){
        return;
    }

    m_freePointValue = freePointValue;
    emit freePointValueChanged(m_freePointValue);
}
void TeachingStackPageModel::setPointNameColorList(QList<QString> pointNameColorList)
{
    if (m_pointNameColorList == pointNameColorList){
        return;
    }

    m_pointNameColorList = pointNameColorList;
    emit pointNameColorListChanged(m_pointNameColorList);
}

void TeachingStackPageModel::setCurAxisOrder(int curAxisOrder)
{
    if (m_curAxisOrder == curAxisOrder){
        return;
    }

    m_curAxisOrder = curAxisOrder;
    emit curAxisOrderChanged(m_curAxisOrder);
}

void TeachingStackPageModel::modbusSetPalletizeNumber(int order, int palletizeNum)
{
    if (order < PalletizeOrder::PalletizeOrderYMxMz || order > PalletizeOrder::PalletizeOrderMax){
        qCritical("%s:%d order[%d] is invalid", __func__, __LINE__, order);
        return;
    }
    if (palletizeNum < 0 || palletizeNum >= PALLETIZE_ORDER_NUM_MAX){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    //qDebug("%s:%d hrNumber[%d] value[%d]", __func__, __LINE__, orderNumTable[order].hrNumber[palletizeNum], orderNumTable[order].orderValue[palletizeNum]);
    QList<int> writeData;
    writeData.append(orderNumTable[order].orderValue[palletizeNum]);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, orderNumTable[order].hrNumber[palletizeNum], 0x0001, writeData);
    m_communication->setModbusWriteCommand(command);
    modbusGetAxisOrderValue(palletizeNum);
}

void TeachingStackPageModel::modbusSetPointData(int pointNum, int index, int writeValue)
{
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        updateStandardPalletizePointData(pointNum, index, writeValue);
        break;
    case PalletizeType::PalletizeTypeFree:
        updateFreePalletizePointData(index, writeValue);
        break;
    default:
        break;
    }
}

void TeachingStackPageModel::modbusSetStartPos(int palletizeNum, int index, int axisNum, quint32 writeValue, float decimalWriteValue)
{
    (void)index;
    if (!m_communication) return;
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_AXIS_STARTPOS_SPEED_HR_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (axisNum < 0 || axisNum >= 5){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    //qDebug("%s:%d index[%d] axisNum[%d] writeValue[%u] decimalWriteValue[%3.2f]", __func__, __LINE__, index, axisNum, writeValue, decimalWriteValue);
    quint32 max = m_commonView->formatPointMaxValue(axisNum, getPalletizePointNumber(0, m_curStandardPalletizeNum)).toFloat() * 100;
    quint32 min = m_commonView->formatPointMinValue(axisNum, getPalletizePointNumber(0, m_curStandardPalletizeNum)).toFloat() * 100;
    if(((decimalWriteValue * 100) > max || (decimalWriteValue * 100) < min) && writeValue < 1000000000){
        return ;
    }
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
#if 1
    if (writeValue != PALLETIZE_TENKEY_DEFAULT_VALUE){
        if(axisNum == 3){
            decimalWriteValue = (float)lround(((decimalWriteValue - m_axisMdownDeceleratePos.toFloat()) * 100.0));
            //qDebug("%s:%d axisNum[%d] decimalWriteValue[%3.2f]", __func__, __LINE__, axisNum, decimalWriteValue);
            writeData.append((int)((int)decimalWriteValue & 0x0000FFFF));
            writeData.append((int)(((int)decimalWriteValue & 0xFFFF0000) >> 16));
        }else if(axisNum == 4){
            decimalWriteValue = (float)lround(((decimalWriteValue - m_axisSdownDeceleratePos.toFloat()) * 100.0));
            //qDebug("%s:%d axisNum[%d] decimalWriteValue[%3.2f]", __func__, __LINE__, axisNum, decimalWriteValue);
            writeData.append((int)((int)decimalWriteValue & 0x0000FFFF));
            writeData.append((int)(((int)decimalWriteValue & 0xFFFF0000) >> 16));
        }else {
            decimalWriteValue = (float)lround((decimalWriteValue * 100.0));
            //qDebug("%s:%d axisNum[%d] decimalWriteValue[%3.2f]", __func__, __LINE__, axisNum, decimalWriteValue);
            writeData.append((int)((int)decimalWriteValue & 0x0000FFFF));
            writeData.append((int)(((int)decimalWriteValue & 0xFFFF0000) >> 16));
        }
        //qDebug("%s:%d writeValue[%d] decimalWriteValue[%3.2f]", __func__, __LINE__, writeValue, decimalWriteValue);
    } else {
        if(axisNum == 3){
            writeValue = writeValue - m_axisMdownDeceleratePos.toUInt();
            writeData.append((int)(writeValue & 0x0000FFFF));
            writeData.append((int)((writeValue & 0xFFFF0000) >> 16));
        }else if(axisNum == 4){
            writeValue = writeValue - m_axisSdownDeceleratePos.toUInt();
            writeData.append((int)(writeValue & 0x0000FFFF));
            writeData.append((int)((writeValue & 0xFFFF0000) >> 16));
        }else {
            writeData.append((int)(writeValue & 0x0000FFFF));
            writeData.append((int)((writeValue & 0xFFFF0000) >> 16));
        }
    }
#else
    DataFormat dataFormat = DataFormat::fromCode(m_axisInfoDataFormat.at(index).value(0));
    int fraction = (dataFormat.fraction <= 0 ? 1 : dataFormat.fraction);
    int write_data = (int)(writeValue * pow(10.0, fraction));
    writeData.append(write_data);
#endif
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            PALLETIZE_AXIS_STARTPOS_SPEED_HR_ADDR_LIST.at(palletizeNum).value(0 + axisNum * 2),
            0x0002,
            writeData);
#if 1
    setOneShotModbusCommand(writeCommand);
    modbusGetStartPos(palletizeNum, index, axisNum);
#else
    m_communication->setModbusWriteCommand(writeCommand);
    QThread::msleep(25);
#endif
    updatePointData();
}

void TeachingStackPageModel::modbusSetStartSpeed(int palletizeNum, int axisNum, int writeValue)
{
    if (!m_communication) return;
    //qDebug("%s:%d palletizeNum[%d] axisNum[%d] writeValue[%d]", __func__, __LINE__, palletizeNum, axisNum, writeValue);
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_AXIS_STARTPOS_SPEED_HR_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (axisNum < 0 || axisNum >= 5){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    writeData.append(writeValue);
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            PALLETIZE_AXIS_STARTPOS_SPEED_HR_ADDR_LIST.at(palletizeNum).value(1 + axisNum * 2),
            0x0001,
            writeData);
    setOneShotModbusCommand(writeCommand);
    modbusGetStartSpeed(palletizeNum, 0, axisNum);
}

void TeachingStackPageModel::modbusSetPointCount(int palletizeNum, int pointAxisNum, int writeValue)
{
    if (!m_communication) return;
    //qDebug("%s:%d palletizeNum[%d] pointAxisNum[%d] writeValue[%d]", __func__, __LINE__, palletizeNum, pointAxisNum, writeValue);
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_COUNT_HR_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (pointAxisNum < 0 || pointAxisNum >= 3){
        qCritical("%s:%d pointAxisNum[%d] is invalid", __func__, __LINE__, pointAxisNum);
        return;
    }
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    writeData.append(writeValue);
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            PALLETIZE_COUNT_HR_ADDR_LIST.at(palletizeNum).value(pointAxisNum),
            0x0001,
            writeData);
    m_communication->setOneShotModbusCommand(writeCommand);
}

void TeachingStackPageModel::modbusSetPointValue(int palletizeNum, int pointAxisNum, int writeValue)
{
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        updateStandardPalletizePointValue(palletizeNum, pointAxisNum, writeValue);
        break;
    case PalletizeType::PalletizeTypeFree:
        updateFreePalletizePointValue(palletizeNum, pointAxisNum, writeValue);
        break;
    default:
        return;
    }
    modbusGetPointValue(palletizeNum, pointAxisNum);
}

void TeachingStackPageModel::modbusSetPitch(int palletizeNum, int index, int axisNum, float writeValue)
{
    if (!m_communication) return;
    //qDebug("%s:%d palletizeNum[%d] index[%d] axisNum[%d] writeValue[%f]", __func__, __LINE__, palletizeNum, index, axisNum, writeValue);
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_PITCH_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
//    if (index < 0 || index >= 3){
//        qCritical("%s:%d index[%d] is invalid", __func__, __LINE__, index);
//        return;
//    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    DataFormat dataFormat = DataFormat::fromCode(m_axisInfoDataFormat.at(index).value(5));
    int fraction = (dataFormat.fraction <= 0 ? 1 : dataFormat.fraction);
    int write_data = (int)lround((writeValue * pow(10.0, fraction)));
//    writeData.append((write_data & 0x0000FFFF));
//    writeData.append((write_data & 0xFFFF0000) >> 16);
    writeData.append(write_data);
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            PALLETIZE_PITCH_ADDR_LIST.at(palletizeNum).value(axisNum),
            0x0001,
            writeData);
    setOneShotModbusCommand(writeCommand);
    modbusGetPitch(palletizeNum, index, axisNum);
    int currentPitch = static_cast<AxisData *>(m_axisInfo->axisDataList().at(axisNum))->pitch().toInt();
    m_operationRecord->recordSetStdStackPitch(palletizeNum, axisNum, currentPitch, write_data);
}

void TeachingStackPageModel::modbusSetDirection(int palletizeNum, int index, int axisNum, bool direction)
{
    if (!m_communication || !m_modbus) return;
    //qDebug("%s:%d palletizeNum[%d] index[%d] axisNum[%d] direction[%d]", __func__, __LINE__, palletizeNum, index, axisNum, direction);
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_PITCH_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
//    if (index < 0 || index >= 3){
//        qCritical("%s:%d index[%d] is invalid", __func__, __LINE__, index);
//        return;
//    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    // get current pitch data
    int pitchData = m_modbus->getLongReceiveDataHR(PALLETIZE_PITCH_ADDR_LIST.at(palletizeNum).value(axisNum));
    QList<int> writeData;
    int writeValue = 0;
    stModBusCommunicationData writeCommand;
    if (direction){
        writeValue = pitchData | 0x00800000;
        writeData.append((writeValue & 0x0000FFFF));
        writeData.append((writeValue & 0xFFFF0000) >> 16);
    } else {
        writeValue = pitchData & ~(0x00800000);
        writeData.append((writeValue & 0x0000FFFF));
        writeData.append((writeValue & 0xFFFF0000) >> 16);
    }
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            PALLETIZE_PITCH_ADDR_LIST.at(palletizeNum).value(axisNum),
            0x0002,
            writeData);
    setOneShotModbusCommand(writeCommand);
    modbusGetPitch(palletizeNum, index, axisNum);
}

int TeachingStackPageModel::getAxisCount()
{
    return mAxisCount;
}

void TeachingStackPageModel::startModbusMonitorStorageMethod()
{
    if (!m_communication || mIsStorageMethodMonitor){
        return;
    }
#if 1
    modBusReadStorageMethod();
    mIsStorageMethodMonitor = true;
    connect(m_communication, &CommunicationThread::feed, this, &TeachingStackPageModel::update, Qt::QueuedConnection);
#else
    stModBusCommunicationData readCommand;
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerTeachingStackPageModel);
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadDiscreteInputs,
                PALLETIZE_STORAGE_METHOD_DI,
                0x0001);
    m_communication->setOneShotModbusCommand(readCommand);
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &TeachingStackPageModel::onOneShotReadFinished, Qt::QueuedConnection);
#endif
}

void TeachingStackPageModel::stopModbusMonitorStorageMethod()
{
    if (!m_communication){
        return;
    }
    disconnect(m_communication, &CommunicationThread::feed, this, &TeachingStackPageModel::update);
    modBusClearStorageMethod();
    mIsStorageMethodMonitor = false;
}

void TeachingStackPageModel::modbusGetPointData(int pointNum, int index)
{
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        recieveStandardPalletizePointData(pointNum, index);
        break;
    case PalletizeType::PalletizeTypeFree:
        recieveFreePalletizePointData(index);
        break;
    default:
        break;
    }
}

void TeachingStackPageModel::setFreePalletizePageInfo(int curPage, int totalPage)
{
    mCurPage = curPage;
    mTotalPage = totalPage;
}

void TeachingStackPageModel::modbusSetFreePalletizeCorrectValue(int axisIndex, int axisNum, float writeValue)
{
    if (!m_communication) return;
    if (m_curFreePalletizeNum < 0 || m_curFreePalletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
    if (axisIndex < 0 || axisIndex >= 3){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return;
    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    //qDebug("%s:%d axisNum[%d] writeValue[%f] m_curFreePalletizeNum[%d]", __func__, __LINE__, axisNum, writeValue, m_curFreePalletizeNum);
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    DataFormat dataFormat = DataFormat::fromCode(m_axisInfoFreeDataFormat.at(axisIndex).value(3));
    int fraction = (dataFormat.fraction <= 0 ? 1 : dataFormat.fraction);
    int write_data = (int)(writeValue * pow(10.0, fraction));

    writeData.append((write_data & 0x0000FFFF));
    writeData.append((write_data & 0xFFFF0000) >> 16);
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            FREE_PALLETIZE_AXIS_CORRECT_VALUE_HR_ADDR_LIST.at(m_curFreePalletizeNum).value(axisNum),
            0x0002,
            writeData);
    setOneShotModbusCommand(writeCommand);
    modbusGetCorrectValue(m_curFreePalletizeNum, axisNum);
}
void TeachingStackPageModel::modbusSetFreePalletizeSpeed(int axisIndex, int axisNum, int writeValue)
{
    if (!m_communication) return;
    if (m_curFreePalletizeNum < 0 || m_curFreePalletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
    if (axisIndex < 0 || axisIndex >= 3){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return;
    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    //qDebug("%s:%d axisNum[%d] writeValue[%d] m_curFreePalletizeNum[%d]", __func__, __LINE__, axisNum, writeValue, m_curFreePalletizeNum);
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    writeData.append(writeValue);
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            FREE_PALLETIZE_AXIS_SPEED_HR_ADDR_LIST.at(m_curFreePalletizeNum).value(axisNum),
            0x0001,
            writeData);
    setOneShotModbusCommand(writeCommand);
    modbusGetStartSpeed(m_curFreePalletizeNum, 0, axisNum);
}

void TeachingStackPageModel::modbusSetFreePalletizePointPos(int index, int axisNum, quint32 writeValue)
{
    if (!m_communication) return;
    if (m_curFreePalletizeNum < 0 || m_curFreePalletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    //qDebug("%s:%d index[%d] axisNum[%d] writeValue[%u] m_curFreePalletizeNum[%d] mCurPage[%d]",
    //    __func__, __LINE__, index, axisNum, writeValue, m_curFreePalletizeNum, mCurPage);
    QList<int> writeData;
    quint16 writeAddress = 0;
    stModBusCommunicationData writeCommand;
    if (writeValue != PALLETIZE_TENKEY_DEFAULT_VALUE){
        writeValue *= 100;
    }
    writeData.append((int)(writeValue & 0x0000FFFF));
    writeData.append((int)((writeValue & 0xFFFF0000) >> 16));
    if (mCurPage == 1){
        if (index < 2){
            return;
        }
        ushort adjAddr = 0;
        ushort palleAdjValue = 0;
        if (index <= 5){
            palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
            adjAddr = ((index - 2) + palleAdjValue) * 24;
            writeAddress = FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST.at(0).value(axisNum) + adjAddr;
        } else {
            palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
            adjAddr = ((index - 6) + palleAdjValue) * 24;
            writeAddress = FREE_PALLETIZE_AXIS_POS_ADDR_LOW_LIST.at(0).value(axisNum) + adjAddr;
        }
        //qDebug("%s:%d writeAddress[%d]", __func__, __LINE__, writeAddress);
    } else {
        ushort adjAddr = 0;
        ushort palleAdjValue = 0;
        if (index <= 3){
            palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
            adjAddr = ((((mCurPage - 1) * 8) + index - 2 + palleAdjValue) * 24);
            writeAddress = FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST.at(0).value(axisNum) + adjAddr;
        } else {
            palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
            adjAddr = ((((mCurPage - 1) * 8) + index - 2 + palleAdjValue) * 24) - (4 * 24);
            writeAddress = FREE_PALLETIZE_AXIS_POS_ADDR_LOW_LIST.at(0).value(axisNum) + adjAddr;
        }
        //qDebug("%s:%d writeAddress[%d]", __func__, __LINE__, writeAddress);
    }
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            writeAddress,
            0x0002,
            writeData);
    setOneShotModbusCommand(writeCommand);
    QThread::msleep(50);                    // 50ms sleep
    modbusGetFreePalletizePointPos(index, axisNum);
}

void TeachingStackPageModel::modbusSetPalletizePointNumber(int palletizeNum)
{
    if (!m_communication) return;
    if (palletizeNum < 0 || palletizeNum >= PALLETIZE_ORDER_NUM_MAX){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (m_curTabIndex < 0 || m_curTabIndex > 1){
        qCritical("%s:%d m_curTabIndex[%d] is invalid", __func__, __LINE__, m_curTabIndex);
        return;
    }
    QList<int> writeData;
    writeData.append(PALLETIZE_POINT_NUMBER.at(m_curTabIndex).value(palletizeNum) - 1);
    stModBusCommunicationData writeCommand =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            PALLETIZE_POINT_NUMBER_HR,
            0x0001,
            writeData
        );
    m_communication->setModbusWriteCommand(writeCommand);
    setAxisOrder(m_standardAxisOrder.at(palletizeNum));
}

void TeachingStackPageModel::modbusSetFreePalletizePointNumber(int palletizeNum)
{
    if (!m_communication) return;
    if (palletizeNum < 0 || palletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    if (m_curTabIndex < 0 || m_curTabIndex > 1){
        qCritical("%s:%d m_curTabIndex[%d] is invalid", __func__, __LINE__, m_curTabIndex);
        return;
    }
    QList<int> writeData;
    writeData.append(PALLETIZE_POINT_NUMBER.at(m_curTabIndex).value(palletizeNum) - 1);
    stModBusCommunicationData writeCommand =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            PALLETIZE_POINT_NUMBER_HR,
            0x0001,
            writeData
        );
    m_communication->setModbusWriteCommand(writeCommand);
}

int TeachingStackPageModel::getDecelerateInfoDispFormat(int index)
{
    if (index < 0 || index > 3){
        return -1;
    }
    return (int)m_decelerateInfoDataFormat.at(0).value(index);
}

int TeachingStackPageModel::getAxisInfoDispFormat(int palletizeNum, int axisIndex, int itemIndex)
{
    int result = -1;
    if (palletizeNum < 0 || palletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return -1;
    }
    if (axisIndex < 0 || axisIndex > 2){
        return -1;
    }
    if (itemIndex < 0 || itemIndex > 5){
        return -1;
    }
    switch((PalletizeType)palletizeNum){
    case PalletizeType::PalletizeTypeSandard:
        result = (int)m_axisInfoDataFormat.at(axisIndex).value(itemIndex);
        break;
    case PalletizeType::PalletizeTypeFree:
        result = (int)m_axisInfoFreeDataFormat.at(axisIndex).value(itemIndex);
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

void TeachingStackPageModel::modbusGetStartPos(int palletizeNum, int index, int axisNum)
{
    //qDebug("%s:%d m_curTabIndex[%d] palletizeNum[%d] index[%d] axisNum[%d]",
    //    __func__, __LINE__, m_curTabIndex, palletizeNum, index, axisNum);
    if (!m_modbus || !m_axisInfo || !m_resource) return;
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        recieveStandardPalletizeStartPos(palletizeNum, index, axisNum);
        break;
    case PalletizeType::PalletizeTypeFree:
        break;
    default:
        break;
    }
}

void TeachingStackPageModel::modbusGetStartSpeed(int palletizeNum, int index, int axisNum)
{
    //qDebug("%s:%d m_curTabIndex[%d] palletizeNum[%d] index[%d] axisNum[%d]",
    //    __func__, __LINE__, m_curTabIndex, palletizeNum, index, axisNum);
    if (!m_modbus || !m_axisInfo || !m_resource) return;
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        recieveStandardPalletizeStartSpeed(palletizeNum, index, axisNum);
        break;
    case PalletizeType::PalletizeTypeFree:
        recieveFreePalletizeStartSpeed(palletizeNum, index, axisNum);
        break;
    default:
        break;
    }
}

void TeachingStackPageModel::modbusGetPointValue(int palletizeNum, int pointAxisNum)
{
    //qDebug("%s:%d m_curTabIndex[%d] palletizeNum[%d] pointAxisNum[%d]",
    //    __func__, __LINE__, m_curTabIndex, palletizeNum, pointAxisNum);
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        recieveStandardPalletizePointValue(palletizeNum, pointAxisNum);
        break;
    case PalletizeType::PalletizeTypeFree:
        recieveFreePalletizePointValue(palletizeNum, pointAxisNum);
        break;
    default:
        break;
    }
}

void TeachingStackPageModel::modbusGetPitch(int palletizeNum, int index, int axisNum)
{
    Q_UNUSED(index);
    if (!m_communication) return;
    //qDebug("%s:%d palletizeNum[%d] index[%d] axisNum[%d]", __func__, __LINE__, palletizeNum, index, axisNum);
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_PITCH_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
//    if (index < 0 || index >= 3){
//        qCritical("%s:%d index[%d] is invalid", __func__, __LINE__, index);
//        return;
//    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    //qDebug("%s:%d addr[%d]", __func__, __LINE__, PALLETIZE_PITCH_ADDR_LIST.at(palletizeNum).value(axisNum));
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, PALLETIZE_PITCH_ADDR_LIST.at(palletizeNum).value(axisNum), 0x0002);
    setOneShotModbusCommand(readCommand);
}

int TeachingStackPageModel::getFreePalletizeFPNumMax(int palletizeNum)
{
    if (palletizeNum < 0 || palletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return -1;
    }
    return m_settableFreeFPNumMax.at(palletizeNum).value(0);
}

void TeachingStackPageModel::modbusGetCorrectValue(int palletizeNum, int axisNum)
{
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        break;
    case PalletizeType::PalletizeTypeFree:
        recieveFreePalletizeCorrectValue(palletizeNum, axisNum);
        break;
    default:
        break;
    }
}

void TeachingStackPageModel::modbusGetFreePalletizePointPos(int index, int axisNum)
{
    if (!m_communication) return;
    if (m_curFreePalletizeNum < 0 || m_curFreePalletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    //qDebug("%s:%d index[%d] axisNum[%d] m_curFreePalletizeNum[%d] mCurPage[%d]", __func__, __LINE__, index, axisNum, m_curFreePalletizeNum, mCurPage);
    quint16 readAddress = 0;
    ushort palleAdjValue = 0;
    if (mCurPage == 1){
        if (index < 2){
            return;
        }
        ushort adjAddr = 0;
        if (index <= 5){
            palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
            adjAddr = ((index - 2) + palleAdjValue) * 24;
            readAddress = FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST.at(0).value(axisNum) + adjAddr;
        } else {
            palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
            adjAddr = ((index - 6) + palleAdjValue) * 24;
            readAddress = FREE_PALLETIZE_AXIS_POS_ADDR_LOW_LIST.at(0).value(axisNum) + adjAddr;
        }
        //qDebug("%s:%d readAddress[%d]", __func__, __LINE__, readAddress);
    } else {
        ushort adjAddr = 0;
        if (index <= 3){
            palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
            adjAddr = ((((mCurPage - 1) * 8) + index - 2 + palleAdjValue) * 24);
            readAddress = FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST.at(0).value(axisNum) + adjAddr;
        } else {
            palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
            adjAddr = ((((mCurPage - 1) * 8) + index - 2 + palleAdjValue) * 24) - (4 * 24);
            readAddress = FREE_PALLETIZE_AXIS_POS_ADDR_LOW_LIST.at(0).value(axisNum) + adjAddr;
        }
        //qDebug("%s:%d readAddress[%d]", __func__, __LINE__, readAddress);
    }
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, readAddress, 0x0002);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::modbusGetFreePalletizeUpperPagePointPos(int curPage)
{
    ushort palleAdjValue = 0;
    ushort adjAddr = 0;
    //qDebug("%s:%d curPage[%d]", __func__, __LINE__, curPage);
    if (curPage == 1){
        stModBusCommunicationData readCommand[2];
        quint16 upperReadAddress = 0;
        quint16 lowerReadAddress = 0;
        quint16 upperReadSize = 0;
        quint16 lowerReadSize = 0;
        palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
        adjAddr = palleAdjValue * 24;
        upperReadAddress = FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST.at(0).value(0) + adjAddr;
        upperReadSize = FREE_PALLETIZE_PAGE1_READ_SIZE_MAX / 2;
        lowerReadAddress = upperReadAddress + upperReadSize + 1;
        lowerReadSize = FREE_PALLETIZE_PAGE1_READ_SIZE_MAX / 2;
        readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, upperReadAddress, upperReadSize);
        readCommand[1] = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, lowerReadAddress, lowerReadSize);
        //qDebug("%s:%d upperReadAddress[%d] upperReadSize[%d] lowerReadAddress[%d] lowerReadSize[%d]",__func__, __LINE__, upperReadAddress, upperReadSize, lowerReadAddress, lowerReadSize);
        setOneShotModbusCommand(readCommand[0]);
        setOneShotModbusCommand(readCommand[1]);
    } else {
        stModBusCommunicationData readCommand;
        quint16 readAddress = 0;
        quint16 readSize = 0;
        palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
        adjAddr = ((curPage - 1) * 8 - 2 + palleAdjValue) * 24;
        readAddress = FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST.at(0).value(0) + adjAddr;
        readSize = FREE_PALLETIZE_ONE_PAGE_READ_SIZE_MAX + 1;
        //qDebug("%s:%d readAddress[%d] readSize[%d]", __func__, __LINE__, readAddress, readSize);
        readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, readAddress, readSize);
        setOneShotModbusCommand(readCommand);
    }
    return;
}

void TeachingStackPageModel::modbusGetFreePalletizeLowerPagePointPos(int curPage)
{
    quint16 readAddress = 0;
    quint16 readSize = 0;
    ushort palleAdjValue = 0;
    ushort adjAddr = 0;
    if (curPage == 1){
        // NOP
        qDebug("%s:%d curPage[%d]", __func__, __LINE__, curPage);
        return;
    } else {
        palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
        adjAddr = ((curPage - 1) * 8 - 2 + palleAdjValue) * 24;
        readAddress = FREE_PALLETIZE_AXIS_POS_ADDR_LOW_LIST.at(0).value(0) + adjAddr;
        readSize = FREE_PALLETIZE_ONE_PAGE_READ_SIZE_MAX + 1;
    }
    //qDebug("%s:%d curPage[%d] readAddress[%d] readSize[%d]", __func__, __LINE__, curPage, readAddress, readSize);
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, readAddress, readSize);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::modbusGetAxisOrderValue(int palletizeNum)
{
    if (palletizeNum < 0 || palletizeNum >= PALLETIZE_ORDER_NUM_MAX){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    //qDebug("%s:%d palletizeNum[%d] addr[%d]", __func__, __LINE__, palletizeNum, orderNumTable[0].hrNumber[palletizeNum]);
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, orderNumTable[0].hrNumber[palletizeNum], 0x0001);
    setOneShotModbusCommand(readCommand);
}

int TeachingStackPageModel::getPalletizePointNumber(int tabIndex, int palletizeNum)
{
    if (tabIndex < 0 || tabIndex >= 2){
        qCritical("%s:%d tabIndex[%d] is invalid", __func__, __LINE__, tabIndex);
        return -1;
    }
    if (palletizeNum < 0 || palletizeNum >= 8){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return -1;
    }
    //qDebug("%s:%d tabIndex[%d] palletizeNum[%d] pointNumber[%d]", __func__, __LINE__, tabIndex, palletizeNum, PALLETIZE_POINT_NUMBER.at(tabIndex).value(palletizeNum));
    return PALLETIZE_POINT_NUMBER.at(tabIndex).value(palletizeNum);
}

void TeachingStackPageModel::modbusGetDirection(int palletizeNum, int index, int axisNum)
{
    Q_UNUSED(index);
    if (!m_communication || !m_modbus) return;
    //qDebug("%s:%d palletizeNum[%d] index[%d] axisNum[%d]", __func__, __LINE__, palletizeNum, index, axisNum);
    if (palletizeNum < 0 || palletizeNum > PALLETIZE_PITCH_ADDR_LIST.count()){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
//    if (index < 0 || index >= 3){
//        qCritical("%s:%d index[%d] is invalid", __func__, __LINE__, index);
//        return;
//    }
    if (axisNum < 0 || axisNum >= 8){
        qCritical("%s:%d axisNum[%d] is invalid", __func__, __LINE__, axisNum);
        return;
    }
    //qDebug("%s:%d addr[%d]", __func__, __LINE__, (PALLETIZE_PITCH_ADDR_LIST.at(palletizeNum).value(axisNum) + 1));
    stModBusCommunicationData readCommand =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters,
            PALLETIZE_PITCH_ADDR_LIST.at(palletizeNum).value(axisNum) + 1,
            0x0001);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void TeachingStackPageModel::recordSetPalletizeNumber(int prevOrder, int currentOrder, int palletizeNum)
{
    if (prevOrder < PalletizeOrder::PalletizeOrderYMxMz || prevOrder > PalletizeOrder::PalletizeOrderMax){
        qCritical("%s:%d prevOrder[%d] is invalid", __func__, __LINE__, prevOrder);
        return;
    }
    if (currentOrder < PalletizeOrder::PalletizeOrderYMxMz || currentOrder > PalletizeOrder::PalletizeOrderMax){
        qCritical("%s:%d currentOrder[%d] is invalid", __func__, __LINE__, currentOrder);
        return;
    }
    if (palletizeNum < 0 || palletizeNum >= PALLETIZE_ORDER_NUM_MAX){
        qCritical("%s:%d palletizeNum[%d] is invalid", __func__, __LINE__, palletizeNum);
        return;
    }
    m_operationRecord->recordSetStdStackPalletizeNumber(palletizeNum, orderNumTable[prevOrder].orderValue[palletizeNum], orderNumTable[currentOrder].orderValue[palletizeNum]);
}

void TeachingStackPageModel::modbusSetStackType(int stackno, int type)
{
    int DO = 2464+ stackno * 2;
    //qDebug() << "stackno::::::" << stackno << "DO:::::" << DO;

    if(type == 0){
        QList<int> writeData;
        writeData.append(1);
        writeData.append(0);
        stModBusCommunicationData command =
                ModbusCommandGenerator::makeModbusWriteCommand(
                    QModbusPdu::FunctionCode::WriteMultipleCoils, DO, 0x0002, writeData);
        setOneShotModbusCommand(command);
    }else if(type == 1){
        QList<int> writeData;
        writeData.append(0);
        writeData.append(1);
        stModBusCommunicationData command =
                ModbusCommandGenerator::makeModbusWriteCommand(
                    QModbusPdu::FunctionCode::WriteMultipleCoils, DO, 0x0002, writeData);
        setOneShotModbusCommand(command);
    }else {
        QList<int> writeData;
        writeData.append(1);
        writeData.append(1);
        stModBusCommunicationData command =
                ModbusCommandGenerator::makeModbusWriteCommand(
                    QModbusPdu::FunctionCode::WriteMultipleCoils, DO, 0x0002, writeData);
        setOneShotModbusCommand(command);
    }
    modbusGetStackType(stackno);
}

void TeachingStackPageModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;

        emit dataLoadedChanged(m_dataLoaded);
    }
}

void TeachingStackPageModel::updatePointData()
{
    //7408 7432 7456 7480 7504 7528 7552 7576
    const QList<qint16> hrList = {7408, 7432, 7456, 7480, 7504, 7528, 7552, 7576};
    int hr = hrList.value(m_curStandardPalletizeNum);
    stModBusCommunicationData readCommand = ModbusCommandGenerator::makeModbusReadCommand(
                QModbusPdu::FunctionCode::ReadHoldingRegisters,
                static_cast<quint16>(hr),
                24);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::modBusReadStorageMethod()
{
    if (!m_communication) return;

    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadDiscreteInputs, PALLETIZE_STORAGE_METHOD_DI, 1);
    m_communication->setPageReadCommandList(Enums::Pages::TeachingStandardPalletize, readCommand);
}

void TeachingStackPageModel::modBusClearStorageMethod()
{
    if (!m_communication) return;
    m_communication->removePageReadCommandList(Enums::Pages::TeachingStandardPalletize, STANDARD_PALLETIZE_MONITOR_STORAGE_METHOD_INDEX);
}

void TeachingStackPageModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    if (!m_communication) return;
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerTeachingStackPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void TeachingStackPageModel::releaseModbusOneShotRead()
{
    if (!m_communication) return;
    m_communication->setOneShotModbusCaller(CommunicationThread::CallerDefault);
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &TeachingStackPageModel::onOneShotReadFinished);
}

void TeachingStackPageModel::updateFreePalletizePointNameAndColorList()
{
    if (!m_resource) return;
    if (m_curFreePalletizeNum < 0 || m_curFreePalletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
    int maxNum = FREE_PALLETIZE_POINT_INFO_LIST.at(m_curFreePalletizeNum).value(1);
    mTotalPage = (((maxNum + 2) % 8) ? maxNum / 8 + 1: maxNum / 8);
    int maxIndex = mTotalPage * 8 - 2;
    m_pointNameList.replace(0, m_resource->label(PEN_MSG_GROUP_PALLETIZE_PAGE, PALLETIZE_PITCH_PEN_MSG_INDEX));
    m_pointNameColorList.replace(0, m_resource->rgb(PEN_MSG_GROUP_PALLETIZE_PAGE, PALLETIZE_PITCH_PEN_MSG_INDEX));
    m_pointNameList.replace(1, m_resource->label(PEN_MSG_GROUP_PALLETIZE_PAGE, PALLETIZE_SPEED_PEN_MSG_INDEX));
    m_pointNameColorList.replace(1, m_resource->rgb(PEN_MSG_GROUP_PALLETIZE_PAGE, PALLETIZE_SPEED_PEN_MSG_INDEX));
    for(int i = 0; i < maxIndex; i++){
        if (i >= maxNum){
            m_pointNameList.replace(2 + i, " ");
        } else {
            int fpNum = FREE_PALLETIZE_POINT_INFO_LIST.at(m_curFreePalletizeNum).value(0) + i;
            QString pointName = "FP" + (fpNum >= 100 ? QString::number(fpNum) : (fpNum >= 10  ? "0" + QString::number(fpNum) : "00" + QString::number(fpNum)));
            m_pointNameList.replace(2 + i, pointName);
        }
        m_pointNameColorList.replace(2 + i, "#000000");
    }
    emit pointNameListChanged(m_pointNameList);
}

void TeachingStackPageModel::updateFreePalletizePointPos(int axis, int axisIndex)
{
    if (mCurPage <= 0){
        return;
    }
    ushort palleAdjValue = 0;
    ushort adjAddr = 0;
    //qDebug("%s:%d mCurPage[%d] axis[%d] axisIndex[%d]", __func__, __LINE__, mCurPage, axis, axisIndex);
    if (mCurPage == 1){
        palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
        for(int i = 2; i < 8; i++){
            PointInfo *pointInfo = static_cast<PointInfo *>(m_pointList.at(i));
            if (i <= 5){
                adjAddr = ((i - 2) + palleAdjValue) * 24;
                pointInfo->setPosition(axisIndex,
                    m_resource->formatData(m_modbus->getLongReceiveDataHR(FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST.at(0).value(axis) + adjAddr),
                        m_axisInfoFreeDataFormat.at(axisIndex).value(5), 2, true));
            } else {
                adjAddr = ((i - 6) + palleAdjValue) * 24;
                pointInfo->setPosition(axisIndex,
                    m_resource->formatData(m_modbus->getLongReceiveDataHR(FREE_PALLETIZE_AXIS_POS_ADDR_LOW_LIST.at(0).value(axis) + adjAddr),
                        m_axisInfoFreeDataFormat.at(axisIndex).value(5), 2, true));
            }
        }
    } else {
        palleAdjValue = (m_curFreePalletizeNum > 0 ? getFreePalletizeFPNumAccumulationValue(m_curFreePalletizeNum) : 0);
        for(int i = 0; i < 8; i++){
            PointInfo *pointInfo = static_cast<PointInfo *>(m_pointList.at(i));
            if (i <= 3){
                adjAddr = ((((mCurPage - 1) * 8) + i - 2 + palleAdjValue) * 24);
                pointInfo->setPosition(axisIndex,
                    m_resource->formatData(m_modbus->getLongReceiveDataHR(FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST.at(0).value(axis) + adjAddr),
                        m_axisInfoFreeDataFormat.at(axisIndex).value(5), 2, true));
            } else {
                adjAddr = ((((mCurPage - 1) * 8) + i - 2 + palleAdjValue) * 24) - (4 * 24);
                pointInfo->setPosition(axisIndex,
                    m_resource->formatData(m_modbus->getLongReceiveDataHR(FREE_PALLETIZE_AXIS_POS_ADDR_LOW_LIST.at(0).value(axis) + adjAddr),
                        m_axisInfoFreeDataFormat.at(axisIndex).value(5), 2, true));
            }
        }
    }
}

void TeachingStackPageModel::updateStandardPalletizePointData(int pointNum, int index, int writeValue)
{
    if (!m_communication) return;
//    if (index < 0 || index > 1){
//        return;
//    }
    quint16 writeSize = 0;
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
//    qDebug("%s:%d pointNum[%d] index[%d] writeValue[%d] addr1[%d] addr2[%d]",
//        __func__, __LINE__, pointNum, index, writeValue,
//        PALLETIZE_POINT_NUM_ADDR_LIST.at(0).value(index), PALLETIZE_POINT_NUM_ADDR_LIST.at(1).value(index));
    switch(pointNum){
    case DownDecelerationPointNum:
        writeSize = ((index == 0 || index == 2) ? 0x0002 : 0x0001);
        if (index == 0 || index == 2){
            writeValue *= 100;
            writeData.append((writeValue & 0x0000FFFF));
            writeData.append((writeValue & 0xFFFF0000) >> 16);
        } else {
            writeData.append(writeValue);
        }
        writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, PALLETIZE_POINT_NUM_ADDR_LIST.at(0).value(index), writeSize, writeData);
        break;
    case UpDecelerationPointNum:
        writeSize = ((index == 0 || index == 2) ? 0x0002 : 0x0001);
        if (index == 0 || index == 2){
            writeValue *= 100;
            writeData.append((writeValue & 0x0000FFFF));
            writeData.append((writeValue & 0xFFFF0000) >> 16);
        } else {
            writeData.append(writeValue);
        }
        writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, PALLETIZE_POINT_NUM_ADDR_LIST.at(1).value(index), writeSize, writeData);
        break;
    default:
        return;
    }
    setOneShotModbusCommand(writeCommand);
    modbusGetPointData(pointNum, index);
}
void TeachingStackPageModel::updateFreePalletizePointData(int index, int writeValue)
{
    if (!m_communication) return;
    if (m_curFreePalletizeNum < 0 || m_curFreePalletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    writeData.append(writeValue);
//    qDebug("%s:%d index[%d] writeValue[%d] m_curFreePalletizeNum[%d] addr[%d]",
//        __func__, __LINE__, index, writeValue, m_curFreePalletizeNum, FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(index + 1));
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(index + 1), 0x0001, writeData);
    setOneShotModbusCommand(writeCommand);
    //modbusGetPointData(-1, index);
}

void TeachingStackPageModel::recieveStandardPalletizePointData(int pointNum, int index)
{
    if (!m_communication) return;

    stModBusCommunicationData readCommand;
    quint16 readSize = 0;
//    qDebug("%s:%d index[%d] pointNum[%d] addr1[%d] addr2[%d]",
//        __func__, __LINE__, index, pointNum, PALLETIZE_POINT_NUM_ADDR_LIST.at(0).value(index), PALLETIZE_POINT_NUM_ADDR_LIST.at(1).value(index));
    switch(pointNum){
    case DownDecelerationPointNum:
        readSize = ((index == 0 || index == 2)? 0x0002 : 0x0001);
        readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, PALLETIZE_POINT_NUM_ADDR_LIST.at(0).value(index), readSize);
        break;
    case UpDecelerationPointNum:
        readSize = ((index == 0 || index == 2)? 0x0002 : 0x0001);
        readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, PALLETIZE_POINT_NUM_ADDR_LIST.at(1).value(index), readSize);
        break;
    default:
        return;
    }
    setOneShotModbusCommand(readCommand);
}
void TeachingStackPageModel::recieveFreePalletizePointData(int index)
{
    if (!m_communication) return;
    if (m_curFreePalletizeNum < 0 || m_curFreePalletizeNum >= FREE_PALLETIZE_NUM_MAX){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
//    qDebug("%s:%d index[%d] m_curFreePalletizeNum[%d] addr[%d]",
//        __func__, __LINE__, index, m_curFreePalletizeNum, FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(index));
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadInputRegisters, FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(index), 0x0001);
    setOneShotModbusCommand(readCommand);
}

void TeachingStackPageModel::updateStandardPalletizeAxisInfo()
{
    if (m_curStandardPalletizeNum < 0 || m_curStandardPalletizeNum > 7){
        qCritical("%s:%d m_curStandardPalletizeNum[%d]", __func__, __LINE__, m_curStandardPalletizeNum);
        return;
    }

    if(m_pageChanging == false && m_modbus->getIsPageChanging() == false){
        QList<int> axisOrder = m_standardAxisOrder.at(m_curStandardPalletizeNum);
        QString startPos;
        for(int i = 0; i < axisOrder.count(); i++){
            int axisIndex = axisOrder.at(i) - 1;
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(m_axisInfoDataFormat.at(axisIndex).value(6) - 1);
            if(i == 2){
                QString startPos1 = m_resource->formatData(
                            (int)m_modbus->getLongReceiveDataRG(PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.at(m_curStandardPalletizeNum).value(0 + ((int)axis * 2))),
                            m_axisInfoDataFormat.at(axisIndex).value(1), 2, true);
                startPos = QString::number(startPos1.toFloat() + m_axisMdownDeceleratePos.toFloat());
                int dotPos = (int)startPos.indexOf(".");
                int startPosStrCount = startPos.count();
                if (dotPos >= 0){
                    if ((startPosStrCount - dotPos) <= 2) { startPos += "0"; }
                } else {
                    startPos += ".00";
                }
            }else if(i == 4){
                QString startPos1 = m_resource->formatData(
                            (int)m_modbus->getLongReceiveDataRG(PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.at(m_curStandardPalletizeNum).value(0 + ((int)axis * 2))),
                            m_axisInfoDataFormat.at(axisIndex).value(1), 2, true);
                startPos = QString::number(startPos1.toFloat() + m_axisSdownDeceleratePos.toFloat());
                int dotPos = (int)startPos.indexOf(".");
                int startPosStrCount = startPos.count();
                if (dotPos >= 0){
                    if ((startPosStrCount - dotPos) <= 2) { startPos += "0"; }
                } else {
                    startPos += ".00";
                }
            }else {
                startPos = m_resource->formatData(
                            (int)m_modbus->getLongReceiveDataRG(PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.at(m_curStandardPalletizeNum).value(0 + ((int)axis * 2))),
                            m_axisInfoDataFormat.at(axisIndex).value(1), 2, true);
            }

            QString startSpeed = m_resource->formatData(
                        (short)m_modbus->getReceiveDataRG(PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST.at(m_curStandardPalletizeNum).value(1 + ((int)axis * 2))),
                        m_axisInfoDataFormat.at(axisIndex).value(2));
            QString pointCount = m_resource->formatData(
                        (short)m_modbus->getReceiveDataRG(PALLETIZE_COUNT_POINT_ADDR_LIST.at(m_curStandardPalletizeNum).value(0 + (i * 2))),
                        m_axisInfoDataFormat.at(axisIndex).value(3));
            QString pointValue = m_resource->formatData(
                        (short)m_modbus->getReceiveDataHR(PALLETIZE_COUNT_POINT_ADDR_LIST.at(m_curStandardPalletizeNum).value(1 + (i * 2))),
                        m_axisInfoDataFormat.at(axisIndex).value(4));
            int pitchDirData = m_modbus->getLongReceiveDataHR(PALLETIZE_PITCH_ADDR_LIST.at(m_curStandardPalletizeNum).value((int)axis));
            int pitchLower = pitchDirData & 0x0000FFFF;
            int pitchUpper = ((pitchDirData & 0xFF000000) >> 16);
            int pitchData = (pitchUpper << 16) | pitchLower;
            int dirData = (pitchDirData & 0x00FF0000) >> 16;
            QString pitch = m_resource->formatData(pitchData, m_axisInfoDataFormat.at(axisIndex).value(5));
            bool direction = ((((dirData & 0x00000080) >> 7) & 0x1) ? true : false );

            m_axisInfo->setStandarStackDatas(axis, startPos, startSpeed, pointCount, pointValue, pitch, direction);
        }
        setStorageMethod(m_modbus->getReceiveDataDI(PALLETIZE_STORAGE_METHOD_DI));
        setDataLoaded(true);
    }
}

void TeachingStackPageModel::updateFreePalletizeAxisInfo()
{
    if (m_curFreePalletizeNum < 0 || m_curFreePalletizeNum > 7){
        qCritical("%s:%d m_curFreePalletizeNum[%d] is invalid", __func__, __LINE__, m_curFreePalletizeNum);
        return;
    }
    if(m_pageChanging == false && m_modbus->getIsPageChanging() == false){
        for(int i = 0; i < m_axisName.count(); i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(m_axisName.at(i) - 1);
            QString pointCount = m_resource->formatData(
                    (short)m_modbus->getReceiveDataRG(FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(0)),
                    m_axisInfoFreeDataFormat.at(i).value(0));
            QString pointValue = m_resource->formatData(
                    (short)m_modbus->getReceiveDataHR(FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(1)),
                    m_axisInfoFreeDataFormat.at(i).value(1));
            QString correctValue = m_resource->formatData(
                    (int)m_modbus->getLongReceiveDataRG(FREE_PALLETIZE_AXIS_CORRECT_VALUE_SR_ADDR_LIST.at(m_curFreePalletizeNum).value((int)axis)),
                    m_axisInfoFreeDataFormat.at(i).value(3));
            QString speed = m_resource->formatData(
                    (short)m_modbus->getReceiveDataRG(FREE_PALLETIZE_AXIS_SPEED_SR_ADDR_LIST.at(m_curFreePalletizeNum).value((int)axis)),
                    m_axisInfoFreeDataFormat.at(i).value(4));
            m_axisInfo->setFreeStackDatas(axis, correctValue, speed, pointCount, pointValue);
            updateFreePalletizePointPos((int)axis, i);
        }
        setFreePointCount(QString::number(m_modbus->getReceiveDataRG(FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(0))));
        setDataLoaded(true);
    }
}

void TeachingStackPageModel::onFinished()
{
    if (!m_modbus || !m_axisInfo || !m_resource) return;
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        updateStandardPalletizeAxisInfo();
        break;
    case PalletizeType::PalletizeTypeFree:
        updateFreePalletizeAxisInfo();
        break;
    default:
        break;
    }
    updateAxis6_8Position();
}

void TeachingStackPageModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::OneShotReadModBusCaller::CallerTeachingStackPageModel){
        return;
    }
    switch(m_curTabIndex){
    case PalletizeType::PalletizeTypeSandard:
        switch(command.readAddress){
        case PALLETIZE_DOWN_DECELERATE_POS_HR_AXIS_M :
        case PALLETIZE_DOWN_DECELERATE_SPEED_HR_AXIS_M :
        case PALLETIZE_UP_DECELERATE_POS_HR_AXIS_M :
        case PALLETIZE_UP_DECELERATE_SPEED_HR_AXIS_M :
        case PALLETIZE_DOWN_DECELERATE_POS_HR_AXIS_S :
        case PALLETIZE_DOWN_DECELERATE_SPEED_HR_AXIS_S :
        case PALLETIZE_UP_DECELERATE_POS_HR_AXIS_S :
        case PALLETIZE_UP_DECELERATE_SPEED_HR_AXIS_S :
            updateDecelerateInfo(command);
            break;
        default:
            if (command.readAddress >= PALLETIZE_AXIS_ORDER_VALUE_TOP_HR
                && command.readAddress <= PALLETIZE_AXIS_ORDER_VALUE_BOTTOM_HR){
                updateStandardPalletizeAxisOrderInfo(command);
                updateStandardPalletizeAxisTypeInfo(command);
            } else {
                updateStandardPalletizeAxisInfo();
            }
            break;
        }
        break;
    case PalletizeType::PalletizeTypeFree:
    {
        ushort targetPointCountAddr = FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(0);
        ushort targetPointValueAddr = FREE_PALLETIZE_POINT_VALUE_ADDR_LIST.at(m_curFreePalletizeNum).value(0);
        if (command.readAddress == targetPointCountAddr
            || command.readAddress == targetPointValueAddr){
            updatePointDataInfo(command);
        } else {
            updateFreePalletizeAxisInfo();
        }
    }
        break;
    default:
        break;
    }
}

void TeachingStackPageModel::update(stModBusCommunicationData command)
{
    updateStorageMethod(command);
}

void TeachingStackPageModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::StackPage));
}

void TeachingStackPageModel::endModbusPageChange(int mainScreenId, int subScreenId)
{
    //qDebug("%s:%d mainScreenId[%d] subScreenId[%d] m_pageChanging[%d]", __func__, __LINE__, mainScreenId, subScreenId, m_pageChanging);
    if (m_curTabIndex == (int)PalletizeType::PalletizeTypeSandard){
        if(mainScreenId == 12 && subScreenId == m_curStandardPalletizeNum){
            m_pageChanging = false;
        }
    } else {
        if(mainScreenId == 12 && subScreenId == (m_curFreePalletizeNum + 8)){
            m_pageChanging = false;
        }
    }
}

