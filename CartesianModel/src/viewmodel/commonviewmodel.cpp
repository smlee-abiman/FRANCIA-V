#include "commonviewmodel.h"
#include "areasetpointparser.h"
#include "areasetpointinfo.h"

AlarmInfo::AlarmInfo(QObject *parent)
    :QObject(parent)
{

}

void AlarmInfo::setType(Type type)
{
    if (m_type == type) {
        return;
    }
    m_type = type;
    emit typeChanged();
}

void AlarmInfo::setAlarmNumber(const QString &alarmNumber)
{
    if(m_alarmNumber == alarmNumber) {
        return;
    }
    m_alarmNumber = alarmNumber;
    emit alarmNumberChanged(m_alarmNumber);
}

void AlarmInfo::setAlarmDetailsColor(const QString &alarmDetailsColor)
{
    if(m_alarmDetailsColor == alarmDetailsColor) {
        return;
    }
    m_alarmDetailsColor = alarmDetailsColor;
    emit alarmDetailsColorChanged(m_alarmDetailsColor);
}

void AlarmInfo::setMsgNumber(const int &msgNumber)
{
    if(m_msgNumber == msgNumber) {
        return;
    }
    m_msgNumber = msgNumber;
    emit msgNumberChanged(m_msgNumber);
}

void AlarmInfo::setAlarmIndex(const int index)
{
    if(m_alarmIndex == index) {
        return;
    }
    m_alarmIndex = index;
    emit alarmIndexChanged();
}

CommonViewModel::CommonViewModel(ResourceManager *resource, CommunicationThread * comm, ModbusManager *modbusMng)
    : ViewModelBase(resource, comm)
    , mPreAxisAlarmStauts{-1, -1, -1, -1, -1, -1, -1, -1}
{
    if (m_resource){
        mCsvReader = m_resource->getCsvReaderInstance();
    }
    if (modbusMng){
        mModbusMng = modbusMng;
        mModbusCom = mModbusMng->getSyncModbusCommunicationInstance();
    }
    m_areaSetPointParser = new AreaSetPointParser(this);
    mPopupDialogOrderStack.push(Enums::PopupDialogStatus::PopupDialogStatusNone);
    initSettingPenSetData();
    initModbusCommunication();
    initInternalInfo();
    initPageCommand();
    initJogInfo();
    modbusSetCurrentOperateAxis(0);
    modbusReadBuzzer();
}

CommonViewModel::~CommonViewModel()
{
    for (int i = 0; i < m_pointMaxMinList.length(); i++)
        qDeleteAll(m_pointMaxMinList.at(i));
    qDeleteAll(m_alarmList);
    if (mIsLedDeviceInitialized) {
        deinitLedDevice();
        mIsLedDeviceInitialized = false;
    }
    terminateModbusCommunication();
}

void CommonViewModel::initJogInfo()
{
   int zz = m_resource->getPenSetIntValue("1",22,0);
   bool speed1 = m_resource->getPenSetIntValue("1",22,1) == 1 ? true : false;
   bool speed2 = m_resource->getPenSetIntValue("1",22,2) == 1 ? true : false;
   bool speed3 = m_resource->getPenSetIntValue("1",22,3) == 1 ? true : false;
   bool speed4 = m_resource->getPenSetIntValue("1",22,4) == 1 ? true : false;
   setjogEnable(zz);
   m_JogSpeed.replace(0,speed1);
   m_JogSpeed.replace(1,speed2);
   m_JogSpeed.replace(2,speed3);
   m_JogSpeed.replace(3,speed4);
}

void CommonViewModel::initSettingPenSetData()
{
    if (!m_resource || !mCsvReader){
        return;
    }
    for(int i = 0; i < MAX_USER_LEVEL; i++){
        m_userLevelMenu[i] = getMenuList(i);
    }
    // note: Operation authority settings are not used.
    // However, we will leave the code as it may be used in the future.
    // m_userLevelOperationAuthority   = m_resource->getIntValueList(28, 6, 31);
    for (int i = 0; i < 31; i++) {
        QList<int> operationAuthority({1,1,1,1,1,1});
        m_userLevelOperationAuthority << operationAuthority;
    }
    m_manualSpeed0001Existence      = m_resource->getPenSetIntValue("30", 3, 0);
    m_manualSpeed001Existence       = m_resource->getPenSetIntValue("30", 4, 0);
    m_languageOrder                 = initLanguageOrder();
    initAxisOrder();
    emit languageOrderChanged();
#if 0
    for(int i = 0; i < 31; i++){
        qDebug() << "value(0):" << m_userLevelOperationAuthority.at(i).value(0) << "value(1):" << m_userLevelOperationAuthority.at(i).value(1)
                 << "value(2):" << m_userLevelOperationAuthority.at(i).value(2) << "value(3):" << m_userLevelOperationAuthority.at(i).value(3)
                 << "value(4):" << m_userLevelOperationAuthority.at(i).value(4) << "value(5):" << m_userLevelOperationAuthority.at(i).value(5);
    }
#endif
    m_manualSpeedSetting = (m_manualSpeed001Existence << 1) | m_manualSpeed0001Existence;
    emit manualSpeedSettingChanged();
    readPointMaxMinSetting();
}

void CommonViewModel::readPointMaxMinSetting()
{
    const int NUM_OF_ROW = 63;
    int row = 25;
    for (quint8 axis = 0; axis < Enums::AxisNum; axis++) {
        QList<AreaSetPointInfo *> pointMaxMinList;
        for (int i = 0; i < NUM_OF_ROW; i++) {
            QList<QList<int>> pointMaxMinSettngList = m_resource->getIntHexValueList(19, row, 1);
            for (QList<int> maxMinSetting: pointMaxMinSettngList) {
                for (int pointSetting : maxMinSetting) {
                    AreaSetPointInfo *info = new AreaSetPointInfo();
                    m_areaSetPointParser->parse(info, static_cast<quint8>(pointSetting), axis);
                    pointMaxMinList.append(info);
                }
            }
            row++;
        }
        m_pointMaxMinList.append(pointMaxMinList);
        row++;
    }
}

QList<int> CommonViewModel::initLanguageOrder()
{
    QList<int> languageOrderList;
    if (!m_resource){
        return languageOrderList;
    }
    QVector<int> languageOrderVector = m_resource->getIntRowArray("1", 9);
    for(int i = 0; i < languageOrderVector.size(); i++) {
        int order = languageOrderVector.at(i);
        if(order == 255) {
            break;
        }
        languageOrderList.append(order);
    }
    setLanguageCount(languageOrderList.count());
    return languageOrderList;
}

void CommonViewModel::initAxisOrder()
{
    if (!m_resource){
        return;
    }
    QVector<int> axisOrderVector = m_resource->getIntRowArray("1", 15);
    for(int i = 0; i < axisOrderVector.size(); i++) {
        int order = axisOrderVector.at(i);
        if ((order < 0) || (order > Enums::AxisNum)) {
            qWarning("%s:%d Invalid axis order. [%d][%d]", __func__, __LINE__, i, order);
            continue;
        }
        if (order == 0) {
            continue;
        }
        m_axisOrder.append(order - 1);
        m_axisTypeOrder.append(Enums::Axis::AxisY + i);
    }
    emit axisOrderChanged();
    emit axisTypeOrderChanged();
    initOperatedAxisAddr();
}

void CommonViewModel::initModbusCommunication()
{
    if (!m_communication || !m_communication->getPhyKeyManagerInstance()) return;
    connect(m_communication, &CommunicationThread::finish,
        this, &CommonViewModel::onFinished, Qt::QueuedConnection);
    connect(m_communication->getPhyKeyManagerInstance(), SIGNAL(updateSelectSwInfo(PhyKeyManager::SelectSwitchStatus)),
        this, SLOT(onUpdateSelectSwInfo(PhyKeyManager::SelectSwitchStatus)));
    connect(m_communication->getPhyKeyManagerInstance(), SIGNAL(updateMemblemSwInfo(PhyKeyManager::MemblemSwInfo)),
        this, SLOT(onUpdateMemblemSwInfo(PhyKeyManager::MemblemSwInfo)));
    connect(m_communication->getPhyKeyManagerInstance(), SIGNAL(updateReleaseMemblemSwInfo(int)),
        this, SLOT(onUpdateReleaseMemblemSwInfo(int)));
    connect(m_communication, &CommunicationThread::feed, this, &CommonViewModel::update, Qt::QueuedConnection);
    if(mModbusMng) { connect(mModbusMng, SIGNAL(isFirstReadCommonModbusChanged(bool)), this, SLOT(onIsFirstReadCommonModbusChanged(bool))); }
    connect(m_communication, SIGNAL(linkStateChange(bool)),this,SLOT(onlinkAlarmChanged(bool)));
}

void CommonViewModel::initAlarmList()
{
    qDeleteAll(m_alarmList);
    m_alarmList.clear();
    for (int i = 0; i < 10; i++) {
        AlarmInfo *alarmInfo = new AlarmInfo(this);
        m_alarmList.append(alarmInfo);
    }
    emit alarmListChanged(m_alarmList);
}

void CommonViewModel::initPageCommand()
{
    if (!m_communication) {
        qDebug("%s:%d m_communication", __func__, __LINE__);
        return;
    }
    stModBusCommunicationData readCommand[COMMON_MODBUS_MONITOR_CMD_LIST_NUM];
    for(int i = 0; i < COMMON_MODBUS_MONITOR_CMD_LIST_NUM; i++){
        readCommand[i] = ModbusCommandGenerator::makeModbusReadCommand(
                            initCommonModbusPageCmd[i].funcCode,
                            initCommonModbusPageCmd[i].address,
                            initCommonModbusPageCmd[i].size
                            );
        m_communication->setCommonPageReadCommandList(readCommand[i]);
    }
}

void CommonViewModel::terminateModbusCommunication()
{
    if (!m_communication || !m_communication->getPhyKeyManagerInstance()) return;
    if(mModbusMng) { disconnect(mModbusMng, SIGNAL(isFirstReadCommonModbusChanged(bool)), this, SLOT(onIsFirstReadCommonModbusChanged(bool))); }
    disconnect(m_communication, &CommunicationThread::finish,
        this, &CommonViewModel::onFinished);
    disconnect(m_communication->getPhyKeyManagerInstance(), SIGNAL(updateSelectSwInfo(PhyKeyManager::SelectSwitchStatus)),
        this, SLOT(onUpdateSelectSwInfo(PhyKeyManager::SelectSwitchStatus)));
    disconnect(m_communication->getPhyKeyManagerInstance(), SIGNAL(updateMemblemSwInfo(PhyKeyManager::MemblemSwInfo)),
        this, SLOT(onUpdateMemblemSwInfo(PhyKeyManager::MemblemSwInfo)));
    disconnect(m_communication->getPhyKeyManagerInstance(), SIGNAL(updateReleaseMemblemSwInfo(int)),
        this, SLOT(onUpdateReleaseMemblemSwInfo(int)));
}

void CommonViewModel::initInternalInfo()
{
#if 0   // TODO
    for(int i = 0; i < 3; i++){
        m_currentStatusList.append(0);
    }
#endif
    for(int i = 0; i < AXIS_ALARM_STATUS_NUM; i++){
        m_axisAlarmStatus.append(0);
    }
    for(int i = 0; i < MAX_USER_LEVEL; i++){
        m_userPassword.append("");
        mUserLevelPassword[i] = 0;
    }
    initAlarmList();
}

bool CommonViewModel::initLedDevice()
{
    if (!m_communication) { return false;}
    PhyKeyManager *phyKeyManager = m_communication->getPhyKeyManagerInstance();
    bool ret = false;
    if (phyKeyManager) {
        ret = phyKeyManager->initLedDevice();
    }
    return ret;
}

void CommonViewModel::deinitLedDevice()
{
    if (!m_communication) { return;}
    PhyKeyManager *phyKeyManager = m_communication->getPhyKeyManagerInstance();
    if (phyKeyManager) {
        phyKeyManager->deinitLedDevice();
    }
}

void CommonViewModel::initOperatedAxisAddr()
{
    const QList<int> DEFAULT_OPERATE_AXIS_DI = {2724,   2725,   2726,   2727,   2728,   2729,   2730,   2731};
    for(int i = 0; i < m_axisOrder.length(); i++){
        int di = DEFAULT_OPERATE_AXIS_DI.at(m_axisOrder.at(i));
        //qDebug() << ":::::::::::::::::::::::::" << di;
        m_currentOperateAxisDi.append(di);
    }
}

void CommonViewModel::modbusUpdateCurrentOperateAxis()
{
    for (int i = 0; i < m_axisOrder.length(); i++) {
        if(m_modbus->getReceiveDataDI(m_currentOperateAxisDi.at(i))){
            if(m_currentOperateAxisOrder != i){
                m_currentOperateAxisOrder = i;
                emit currentOperateAxisOrderChanged();
            }
        }
    }
}

void CommonViewModel::setLanguageCount(int languageCount) {
    if(m_languageCount == languageCount) {
        return;
    }
    m_languageCount = languageCount;
    emit languageCountChanged(m_languageCount);
}

QList<int> CommonViewModel::getMenuList(QVector<int> row1, QVector<int> row2, QVector<int> row3, QVector<int> row4, QVector<int> row5)
{
    QList<int> menuList;
    int useRow1Count = 1;
    int useRow2Count = 3;
    int useRow3Count = 3;
    int useRow4Count = 3;
    int useRow5Count = 0;

    foreach (int menuNo, row1) {
        if(useRow1Count > 0) {
            menuList.append(menuNo);
            useRow1Count--;
        } else {
            break;
        }
    }
    foreach (int menuNo, row2) {
        if(useRow2Count > 0) {
            menuList.append(menuNo);
            useRow2Count--;
        } else {
            break;
        }
    }
    foreach (int menuNo, row3) {
        if(useRow3Count > 0) {
            menuList.append(menuNo);
            useRow3Count--;
        } else {
            break;
        }
    }
    foreach (int menuNo, row4) {
        if(useRow4Count > 0) {
            menuList.append(menuNo);
            useRow4Count--;
        } else {
            break;
        }
    }
    foreach (int menuNo, row5) {
        if(useRow5Count > 0) {
            menuList.append(menuNo);
            useRow5Count--;
        } else {
            break;
        }
    }
    return menuList;
}

QList<int> CommonViewModel::getMenuList(int userLevel)
{
    QList<int> menuList;
    switch(userLevel) {
    case 0:
        menuList = menuListUserLevel01();
        break;
    case 1:
        menuList = menuListUserLevel02();
        break;
    case 2:
        menuList = menuListUserLevel03();
        break;
    case 3:
        menuList = menuListUserLevel04();
        break;
    case 4:
        menuList = menuListUserLevel05();
        break;
    default:
        // NOP
        break;
    }
    return menuList;
}

Enums::Pages CommonViewModel::pageChange(Enums::Pages page, int subPage)
{
    qDebug("%s:%d page[%d] subPage[%d]", __func__, __LINE__, page, subPage);
    Enums::Pages oldPage = m_currentPage;
    m_prePage = m_currentPage;
    m_preSubPage = m_currentSubPage;
    switch(page){
    case Enums::Pages::Origin:
        m_curLeftPage = page;
        break;
    case Enums::Pages::ManualIO:
    case Enums::Pages::ManualAxisOpe:
    case Enums::Pages::ManualAxisExpress:
        m_curLeftPage = page;
        break;
    case Enums::Pages::AutoAxisTimer:
    case Enums::Pages::AutoAxisCounter:
    case Enums::Pages::AutoProductMngTimer:
    case Enums::Pages::AutoProductMngCounter:
    case Enums::Pages::AutoIOTimer:
    case Enums::Pages::AutoIOCounter:
        m_curLeftPage = page;
        break;
    default:
        break;
    }
    if (oldPage == Enums::Pages::Origin) { setDialogStatus(true); }
    m_currentPage = page;
    m_currentSubPage = subPage;
    updateTopTitleLocal(m_currentPage, m_curLeftPage);
    updateScreenId(m_currentPage, subPage);

    if(mModbusMng != nullptr) {
        if (page == Enums::Pages::MonitorInternal){
            mModbusMng->updatePageCommand(m_currentPage, (subPage + 1));
        } else {
            mModbusMng->updatePageCommand(m_currentPage);
        }
    }
    return oldPage;
}

void CommonViewModel::backPrePage()
{
    pageChange(m_prePage,m_preSubPage);
}

void CommonViewModel::backPrePage(Enums::Pages page, int subPage)
{
    pageChange(page, subPage);
}

void CommonViewModel::changeScreenId(Enums::MainScreenId mainScreenId, Enums::SubScreenId subScreenId)
{
    m_currentDp1 = mainScreenId;
    m_currentDp2 = subScreenId;
    emit currentDp1Changed(m_currentDp1);
    emit currentDp2Changed(m_currentDp2);

    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    if ((mainScreenId < 0 || mainScreenId > Enums::MainScreenId::MainScreenIdMax)
        || (subScreenId < 0 || subScreenId > Enums::SubScreenId::SubScreenIdMax)){
        qDebug("%s:%d ScreenId[%d][%d] is invalid", __func__, __LINE__, mainScreenId, subScreenId);
        return;
    }

    m_modbus->startIsPageChanging();

    //qDebug("%s:%d mainScreenId[%d] subScreenId[%d]", __func__, __LINE__, mainScreenId, subScreenId);
    QList<int> writeDataMain;
    QList<int> writeDataSub;
    m_curMainScreenId = mainScreenId;
    m_curSubScreenId = subScreenId;
    writeDataMain.clear();
    writeDataSub.clear();

    for(int i = 0; i < MAIN_SCREEN_ID_BIT_NUM; i++){
        writeDataMain.append(((int)mainScreenId >> i) & 0x01);
    }
    for(int i = 0; i < SUB_SCREEN_ID_BIT_NUM; i++){
        writeDataSub.append(((int)subScreenId >> i) & 0x01);
    }
    stModBusCommunicationData command1 =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleCoils, MAIN_SCREEN_ID_TOP_DO, MAIN_SCREEN_ID_BIT_NUM, writeDataMain);
    stModBusCommunicationData command2 =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleCoils, SUB_SCREEN_ID_TOP_DO, SUB_SCREEN_ID_BIT_NUM, writeDataSub);
    m_communication->setModbusWriteCommand(command1);
    m_communication->setModbusWriteCommand(command2);
    if (m_operationRecord) {
        if (mainScreenId != Enums::MainScreenId::MainScreenIdTeachingPalletize) {
            m_operationRecord->recordScreenSwitching(mainScreenId, subScreenId);
        }
    }
}

void CommonViewModel::changeMainScreenId(Enums::MainScreenId mainScreenId)
{
    if (!m_communication){
        qDebug("%s:%d m_communication is invalid", __func__, __LINE__);
        return;
    }
    if ((mainScreenId < 0 || mainScreenId > Enums::MainScreenId::MainScreenIdMax)){
        qDebug("%s:%d ScreenId[%d] is invalid", __func__, __LINE__, mainScreenId);
        return;
    }
    //qDebug("%s:%d mainScreenId[%d]", __func__, __LINE__, mainScreenId);
    QList<int> writeDataMain;
    m_curMainScreenId = mainScreenId;
    writeDataMain.clear();
    for(int i = 0; i < MAIN_SCREEN_ID_BIT_NUM; i++){
        writeDataMain.append(((int)mainScreenId >> i) & 0x01);
    }
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleCoils, MAIN_SCREEN_ID_TOP_DO, MAIN_SCREEN_ID_BIT_NUM, writeDataMain);
    m_communication->setModbusWriteCommand(command);
}

int CommonViewModel::getUserLevel()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return 0;
    }
#ifdef REAL_TARGET
    int userLevel = m_modbus->getUserLevel();
#else // REAL_TARGET
    int userLevel = m_userLevel;
#endif // REAL_TARGET
    setUserLevel(userLevel);
    return userLevel;
}

void CommonViewModel::modbusAutoPressed()
{
    writeManipulateMode((int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusAuto);
#ifndef REAL_TARGET
    setManipulateMode(2);
#endif
}

void CommonViewModel::modbusManualPressed()
{
    writeManipulateMode((int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusManual);
#ifndef REAL_TARGET
    setManipulateMode(1);
#endif
}

void CommonViewModel::modbusOriginPressed()
{
    writeManipulateMode((int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusOrigin);
#ifndef REAL_TARGET
    setManipulateMode(0);
#endif
}

void CommonViewModel::modbusReturnControl(bool isPress)
{
    if (!m_communication){
        qDebug("%s:%d m_communication is not share", __func__, __LINE__);
        return;
    }
    if (isPress){
        bool ret = false;
        int writeData = RETURN_CTRL_VALUE;
        ret = m_communication->writeModbusHR(RETURN_CTRL_HR, &writeData, 1);
        if (!ret){
            qDebug("%s:%d writeModbusHR is failed[%d]", __func__, __LINE__, ret);
            return;
        }
    }
    // isPress:trueの場合、keyNo=247　isPress:falseの場合、0xFF　
    modbusMemblemSwControl(isPress, Enums::MemblemKeyNo::ReturnCtrlkeyNo);
}

void CommonViewModel::modbusResetControl(bool isPress)
{
    if (!m_communication){
        qDebug("%s:%d m_communication is not share", __func__, __LINE__);
        return;
    }
    if (isPress){
        bool ret = false;
        int writeData = RESET_CTRL_VALUE;
        ret = m_communication->writeModbusHR(RESET_CTRL_HR, &writeData, 1);
        if (!ret){
            qDebug("%s:%d writeModbusHR is failed[%d]", __func__, __LINE__, ret);
            return;
        }
    }
    // isPress:trueの場合、keyNo=247　isPress:falseの場合、0xFF
    modbusMemblemSwControl(isPress, Enums::MemblemKeyNo::ReturnCtrlkeyNo);
}

void CommonViewModel::getModbusSystemTime()
{
    if(!m_communication) return;
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, COMMON_SYSTEM_TIME_ADDR_LIST.at(0).value(0), COMMON_SYSTEM_TIME_ADDR_LIST.at(0).count());
    m_communication->setOneShotModbusCommand(command);
    connect(m_communication, &CommunicationThread::oneShotReadFinished,
        this, &CommonViewModel::onOneShotReadFinished, Qt::QueuedConnection);
}

void CommonViewModel::modbusUpdateUserLevelPassword(int level, QString password)
{
    if(!m_communication) return;
    if (level < 0 || level > 4) return;
    int userLevelPassword = password.toInt();
    if (userLevelPassword < 0 || userLevelPassword > 9999){
        qDebug("%s:%d invalid password[%d]", __func__, __LINE__, userLevelPassword);
        return;
    }
    mUserLevelPassword[level] = userLevelPassword;
    stModBusCommunicationData writeCommand;
    QList<int> writeData;
    writeData.append(userLevelPassword);
    writeCommand = 
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister,
            USER_PASSWORD_TOP_HR + level,
            1,
            writeData);
    m_communication->setModbusWriteCommand(writeCommand);
    QThread::msleep(100);
}

void CommonViewModel::getUserLevelPassword(int level)
{
    if(!m_communication) return;
    if (level < 0 || level > 4) return;
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, USER_PASSWORD_TOP_HR + level, 1);
    m_communication->setOneShotReadCommand(command);
    connect(m_communication, &CommunicationThread::oneShotReadFinished,
        this, &CommonViewModel::onOneShotReadFinished, Qt::QueuedConnection);
}

QList<int> CommonViewModel::getOperationAuthorityByGroupId(int groupId)
{
    QList<int> dummyList;
    if (groupId < 1 || groupId > 31) {
        return dummyList;
    }
    if (m_userLevelOperationAuthority.count() <= 0 || groupId > m_userLevelOperationAuthority.count()){
        return dummyList;
    }
    return m_userLevelOperationAuthority.at(groupId - 1);
}

int CommonViewModel::getOperationAuthority(int groupId, int userLevel)
{
    if (groupId < 1 || groupId > 31) {
        return -1;
    }
    if (userLevel < 0 || userLevel > 4) {
        return -1;
    }
    if (m_userLevelOperationAuthority.count() <= 0 || groupId > m_userLevelOperationAuthority.count()){
        return -1;
    }
    return (int)(m_userLevelOperationAuthority.at(groupId - 1).value(userLevel + 1));
}

void CommonViewModel::modbusUpdateDateTime(int index, QString value)
{
    if (index < (int)Enums::DateTimeInfo::DateTimeYear || index > (int)Enums::DateTimeInfo::DateTimeSeconds){
        return;
    }
    stModBusCommunicationData writeCommand;
    QList<int> writeData;
    //QString adjValue = (index == (int)Enums::DateTimeInfo::DateTimeYear ? value.mid(2, 2) : value);
    QString adjValue = (index == (int)Enums::DateTimeInfo::DateTimeYear ? value : value);
    int write_data = adjValue.toInt();
    writeData.append(write_data);
    writeCommand =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister,
            DATE_TIME_TOP_HR + index,
            1,
            writeData);
    m_communication->setOneShotModbusCommand(writeCommand);
    getModbusSystemTime();
}

void CommonViewModel::modbusAxisStatus(bool isMainAxis, bool isSubAxis)
{
    if (!m_communication){
        qDebug("%s:%d m_communication is not share", __func__, __LINE__);
        return;
    }
    if ((isMainAxis && isSubAxis) || (!isMainAxis && !isSubAxis)){
        qDebug("%s:%d invalid status isMainAxis[%d] isSubAxis[%d]", __func__, __LINE__, isMainAxis, isSubAxis);
        return;
    }
    QList<int> writeData;
    for(int i = 0; i < 2; i++) {
        writeData.append(0);
    }
    qDebug("%s:%d isMainAxis[%d] isSubAxis[%d]", __func__, __LINE__, isMainAxis, isSubAxis);
    writeData.replace(0, isSubAxis);   // bit1674
    writeData.replace(1, isMainAxis);    // bit1675
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, AXIS_STATUS_TOP_DO, 0x0002, writeData);
    m_communication->setModbusWriteCommand(command);
}

QList<int> CommonViewModel::menuListUserLevel01()
{
    QList<int> dummyMenu;
    if (!m_resource){
        return dummyMenu;
    }
    QVector<int> menuRow01 = m_resource->getIntRowArray("8", 1);
    QVector<int> menuRow02 = m_resource->getIntRowArray("8", 2);
    QVector<int> menuRow03 = m_resource->getIntRowArray("8", 3);
    QVector<int> menuRow04 = m_resource->getIntRowArray("8", 4);
    QVector<int> menuRow05 = m_resource->getIntRowArray("8", 5);
    return getMenuList(menuRow01, menuRow02, menuRow03, menuRow04, menuRow05);
}

QList<int> CommonViewModel::menuListUserLevel02()
{
    QList<int> dummyMenu;
    if (!m_resource){
        return dummyMenu;
    }
    QVector<int> menuRow01 = m_resource->getIntRowArray("8", 6);
    QVector<int> menuRow02 = m_resource->getIntRowArray("8", 7);
    QVector<int> menuRow03 = m_resource->getIntRowArray("8", 8);
    QVector<int> menuRow04 = m_resource->getIntRowArray("8", 9);
    QVector<int> menuRow05 = m_resource->getIntRowArray("8", 10);
    return getMenuList(menuRow01, menuRow02, menuRow03, menuRow04, menuRow05);
}

QList<int> CommonViewModel::menuListUserLevel03()
{
    QList<int> dummyMenu;
    if (!m_resource){
        return dummyMenu;
    }
    QVector<int> menuRow01 = m_resource->getIntRowArray("8", 11);
    QVector<int> menuRow02 = m_resource->getIntRowArray("8", 12);
    QVector<int> menuRow03 = m_resource->getIntRowArray("8", 13);
    QVector<int> menuRow04 = m_resource->getIntRowArray("8", 14);
    QVector<int> menuRow05 = m_resource->getIntRowArray("8", 15);
    return getMenuList(menuRow01, menuRow02, menuRow03, menuRow04, menuRow05);
}

QList<int> CommonViewModel::menuListUserLevel04()
{
    QList<int> dummyMenu;
    if (!m_resource){
        return dummyMenu;
    }
    QVector<int> menuRow01 = m_resource->getIntRowArray("8", 16);
    QVector<int> menuRow02 = m_resource->getIntRowArray("8", 17);
    QVector<int> menuRow03 = m_resource->getIntRowArray("8", 18);
    QVector<int> menuRow04 = m_resource->getIntRowArray("8", 19);
    QVector<int> menuRow05 = m_resource->getIntRowArray("8", 20);
    return getMenuList(menuRow01, menuRow02, menuRow03, menuRow04, menuRow05);
}

QList<int> CommonViewModel::menuListUserLevel05()
{
    QList<int> dummyMenu;
    if (!m_resource){
        return dummyMenu;
    }
    QVector<int> menuRow01 = m_resource->getIntRowArray("8", 21);
    QVector<int> menuRow02 = m_resource->getIntRowArray("8", 22);
    QVector<int> menuRow03 = m_resource->getIntRowArray("8", 23);
    QVector<int> menuRow04 = m_resource->getIntRowArray("8", 24);
    QVector<int> menuRow05 = m_resource->getIntRowArray("8", 25);
    return getMenuList(menuRow01, menuRow02, menuRow03, menuRow04, menuRow05);
}

QString CommonViewModel::getScreenNameForMenu(int menuNo)
{
    QString screenName = "";
    switch (menuNo) {
    case 9:
        screenName = m_resource->label(33, 12);
        break;
    case 10:
        screenName = m_resource->label(33, 13);
        break;
    case 11:
        screenName = m_resource->label(33, 14);
        break;
    case 12:
        screenName = m_resource->label(33, 15);
        break;
    case 13:
        screenName = m_resource->label(33, 16);
        break;
    case 14:
        screenName = m_resource->label(33, 17);
        break;
    case 15:
        screenName = m_resource->label(33, 18);
        break;
    case 16:
        screenName = m_resource->label(33, 19);
        break;
    case 17:
        screenName = m_resource->label(33, 20);
        break;
    case 18:
        screenName = m_resource->label(33, 21);
        break;
    default:
        // NOP
        break;
    }
    return screenName;
}

QString CommonViewModel::getCurrentState(bool di001, bool di002, bool di003)
{
    QString state = "- - - -";
    if(di001 && !di002 && !di003) {
        state = m_resource->label(20, 4);
    } else if(!di001 && di002 && !di003) {
        state = m_resource->label(20, 5);
    } else if(!di001 && !di002 && di003) {
        state = m_resource->label(20, 6);
    }
    return state;
}

void CommonViewModel::writeManipulateMode(int manipMode)
{
    if (!m_communication){
        qDebug("%s:%d m_communication is not share", __func__, __LINE__);
        return;
    }
    if (m_operationMode){
        writeManipulateModeExistModeSw(manipMode);
    } else {
        writeManipulateModeUnExistModeSw(manipMode);
    }
}

void CommonViewModel::writeManipulateModeExistModeSw(int manipMode)
{
    QList<int> writeData1;
    QList<int> writeData2;

    writeData1.append(0);
    for(int i = 0; i < 2; i++) {
        writeData2.append(0);
    }
    if (manipMode == (int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusOrigin){
        writeData1.replace(0, 1);   // bit28 1 on
    } else if (manipMode == (int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusManual){
        writeData2.replace(1, 1);  // bit31 1 on
    } else if (manipMode == (int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusAuto){
        writeData2.replace(0, 1);  // bit30 1 on
    } else {
        qDebug("%s:%d manipMode[%d] is invalid", __func__, __LINE__, manipMode);
        return;
    }
    qInfo("%s:%d writeData1 count[%d] writeData1[0]=[%d]", __func__, __LINE__, writeData1.count(), writeData1.at(0));
    qInfo("%s:%d writeData2 count[%d] writeData2[0]=[%d] writeData2[1]=[%d]",
        __func__, __LINE__, writeData2.count(), writeData2.at(0), writeData2.at(1));
    stModBusCommunicationData command1 =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, MANIP_MODE1_TOP_DO, 0x0001, writeData1);
    m_communication->setModbusWriteCommand(command1);
    stModBusCommunicationData command2 =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, MANIP_MODE2_TOP_DO, 0x0002, writeData2);
    m_communication->setModbusWriteCommand(command2);
}

void CommonViewModel::writeManipulateModeUnExistModeSw(int manipMode)
{
    QList<int> writeData;

    for(int i = 0; i < 3; i++) {
        writeData.append(0);
    }
    if (manipMode == (int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusOrigin){
        writeData.replace(1, 1);   // bit2297 1 on
    } else if (manipMode == (int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusManual){
        writeData.replace(2, 1);  // bit2298 1 on
    } else if (manipMode == (int)PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusAuto){
        writeData.replace(0, 1);  // bit2296 1 on
    } else {
        qDebug("%s:%d manipMode[%d] is invalid", __func__, __LINE__, manipMode);
        return;
    }
    qInfo("%s:%d writeData count[%d] writeData[0]=[%d] writeData[1]=[%d] writeData[2]=[%d]",
        __func__, __LINE__, writeData.count(), writeData.at(0), writeData.at(1), writeData.at(2));
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, OPE_MODE_TOP_DO, 0x0003, writeData);
    m_communication->setModbusWriteCommand(command);
}

void CommonViewModel::writeOperationMode()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    bool isManual = m_modbus->getReceiveDataDI(31);
    bool isOrigin = m_modbus->getReceiveDataDI(28);
    bool isAuto = m_modbus->getReceiveDataDI(30);
    QList<int> writeData;
    OperationViewMode opeViewMode = OperationViewModeInvalid;

    if (isManual && !isOrigin && !isAuto){
        opeViewMode = OperationViewModeManual;
    } else if (!isManual && isOrigin && !isAuto){
        opeViewMode = OperationViewModeOrigin;
    } else if (!isManual && !isOrigin && isAuto){
        opeViewMode = OperationViewModeAuto;
    } else {
        opeViewMode = OperationViewModeManual;
    }
    for(int i = 0; i < 3; i++) {
        writeData.append(0);    // bit2296-2298 0 off
    }
    if(opeViewMode == OperationViewModeManual){
        writeData.replace(2, 1);  // bit2298 1 on
    } else if (opeViewMode == OperationViewModeOrigin){
        writeData.replace(1, 1);  // bit2297 1 on
    } else if (opeViewMode == OperationViewModeAuto){
        writeData.replace(0, 1);  // bit2296 1 on
    } else {
        writeData.replace(2, 1);  // bit2298 1 on
    }
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleCoils, OPE_MODE_TOP_DO, 0x0003, writeData);
    if(m_communication != nullptr) m_communication->setModbusWriteCommand(command);
}

bool CommonViewModel::judgeOperationMode()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return false;
    }
    bool isManual = m_modbus->getReceiveDataDI(31);
    bool isOrigin = m_modbus->getReceiveDataDI(28);
    bool isAuto = m_modbus->getReceiveDataDI(30);
    if(!isManual && !isOrigin && !isAuto){
        return false;
    } else {
        return true;
    }
}

void CommonViewModel::judgeManipulateMode()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    bool isManual = false, isOrigin = false, isAuto = false;
    if (m_operationMode) {
        isManual = m_modbus->getReceiveDataDI(31);
        isOrigin = m_modbus->getReceiveDataDI(28);
        isAuto = m_modbus->getReceiveDataDI(30);
    } else {
        isManual = m_modbus->getReceiveDataDI(2658);
        isOrigin = m_modbus->getReceiveDataDI(2657);
        isAuto = m_modbus->getReceiveDataDI(2656);
    }

    if(isManual && !isOrigin && !isAuto){
        setManipulateMode(1);
    } else if (!isManual && isOrigin && !isAuto){
        setManipulateMode(0);
    } else if (!isManual && !isOrigin && isAuto){
        setManipulateMode(2);
    } else if (!isManual && !isOrigin && !isAuto){
        //setManipulateMode(1);
    } else {
        qWarning("%s:%d Unknown mode", __func__, __LINE__);
        setManipulateMode(-1);
    }
}

bool CommonViewModel::judgeAxisAlarmStatus(QList<int> curAxisAlarm)
{
    if (curAxisAlarm.count() < AXIS_ALARM_STATUS_NUM){
        return false;
    }
    if (m_axisAlarmStatus.at(0) == curAxisAlarm.at(0) &&
        m_axisAlarmStatus.at(1) == curAxisAlarm.at(1) &&
        m_axisAlarmStatus.at(2) == curAxisAlarm.at(2) &&
        m_axisAlarmStatus.at(3) == curAxisAlarm.at(3) &&
        m_axisAlarmStatus.at(4) == curAxisAlarm.at(4) &&
        m_axisAlarmStatus.at(5) == curAxisAlarm.at(5) &&
        m_axisAlarmStatus.at(6) == curAxisAlarm.at(6) &&
        m_axisAlarmStatus.at(7) == curAxisAlarm.at(7)){
        return false;
    }
    return true;
}

bool CommonViewModel::judgeUpdateUserPassword(QList<QString> curUserPassword)
{
    if (curUserPassword.count() < 0 || curUserPassword.count() > MAX_USER_LEVEL){
        return false;
    }
    if (m_userPassword.at(0) == curUserPassword.at(0) &&
        m_userPassword.at(1) == curUserPassword.at(1) &&
        m_userPassword.at(2) == curUserPassword.at(2) &&
        m_userPassword.at(3) == curUserPassword.at(3) &&
        m_userPassword.at(4) == curUserPassword.at(3)){
        return false;
    }
    return true;
}

void CommonViewModel::updateAxisStatus()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    bool isMainAxis = m_modbus->getReceiveDataDI(2563);
    bool isSubAxis = m_modbus->getReceiveDataDI(2562);
    if ((!isMainAxis && !isSubAxis) || (isMainAxis && isSubAxis)){
        //qDebug("%s:%d invalid status isMainAxis[%d] isSubAxis[%d]", __func__, __LINE__, isMainAxis, isSubAxis);
        return;
    }
    setAxisStatus(isMainAxis);
}

void CommonViewModel::updateStepOpeStatus()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    bool isStepOpe = m_modbus->getReceiveDataDI(2613);
    setStepOpeStatus(isStepOpe);
}

void CommonViewModel::updateCurrentStatus()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    quint8 alarmStatus = 0;
    quint8 errorStatus = 0;
    qint16 systemAlarmStatus = 0;
    int shift = 0;
    int currentStatus = 0;
    mIsNotifyAlarm = false;
    shift = 0;
    for(int i = ALARM_STATUS_TOP_DI; i < (ALARM_STATUS_TOP_DI + ALARM_STATUS_BIT_NUM) && shift < ALARM_STATUS_BIT_NUM; i++){
        alarmStatus |= (m_modbus->getReceiveDataDI(i) << shift);
        shift++;
    }
    if (mPreAlarmStatus != alarmStatus
        && alarmStatus != 0){
        updateAlarmList(AlertMsgType::AlertMsgTypeAlarm, 0, (quint16)alarmStatus);
        if (mPreAlarmStatus <= 0){
            mInternalAlarmCount++;
        }
        mIsNotifyAlarm = true;
        mPreAlarmStatus = alarmStatus;
        mIsAlarmOccurred = true;
    } else {
        if (mPreAlarmStatus != 0 && alarmStatus == 0 && mInternalAlarmCount > 0){
            mInternalAlarmCount--;
            removeAlarmList(AlertMsgType::AlertMsgTypeAlarm, 0);
            mIsNotifyAlarm = true;
            mIsAlarmOccurred = false;
        }
        mPreAlarmStatus = alarmStatus;
    }
    shift = 0;
    for(int i = ERROR_STATUS_TOP_DI; i < (ERROR_STATUS_TOP_DI + ERROR_STATUS_BIT_NUM) && shift < ERROR_STATUS_BIT_NUM; i++){
        errorStatus |= (m_modbus->getReceiveDataDI(i) << shift);
        shift++;
    }
    if (errorStatus != 0){
        if (m_errorStatus != errorStatus)
            mIsNotifyAlarm = true;
        updateErrorStatus(errorStatus);
    }
    systemAlarmStatus = (quint16)(m_modbus->getWordReceiveDataRG(SYSTEM_ALARM_STATUS_SR));
    if (mPreSystemAlarmStatus != systemAlarmStatus
        && systemAlarmStatus != 0){
        updateAlarmList(AlertMsgType::AlertMsgTypeSystem, 0, systemAlarmStatus);
        if (mPreSystemAlarmStatus <= 0){
            mInternalAlarmCount++;
        }
        mPreSystemAlarmStatus = systemAlarmStatus;
        mIsNotifyAlarm = true;
    } else {
        if (mPreSystemAlarmStatus != 0 && systemAlarmStatus == 0 && mInternalAlarmCount > 0){
            mInternalAlarmCount--;
            removeAlarmList(AlertMsgType::AlertMsgTypeSystem, 0);
            mIsNotifyAlarm = true;
        }
        mPreSystemAlarmStatus = systemAlarmStatus;
    }
    currentStatus = (systemAlarmStatus << 16) | (errorStatus << 8) | alarmStatus;
    setCurAlarmCount(mInternalAlarmCount);
    setCurrentStatus(currentStatus);
}

void CommonViewModel::updateAxisAlarmStatus()
{
    if (!mIsLedDeviceInitialized) {
        mIsLedDeviceInitialized = initLedDevice();
        if (mIsLedDeviceInitialized) {
            controlLedDevice(false, (int)CoreLibDriverManager::LedType::LedTypeRed);
        }
    }
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    QList<int> axisAlarmStatusList;
    int dataRg = 0;
    bool alarmListStatusChange = true;
    axisAlarmStatusList.clear();
    bool axisAlarmStatusUpdated = false;
    for(int i = AXIS_ALARM_STATUS_TOP_SR; i < (AXIS_ALARM_STATUS_TOP_SR + AXIS_ALARM_STATUS_NUM); i++){
        dataRg = m_modbus->getReceiveDataRG(i);
        if (mPreAxisAlarmStauts[i - AXIS_ALARM_STATUS_TOP_SR] != dataRg
            && dataRg != 0) {
            if (dataRg < 0 || dataRg > 128){
                qDebug("%s:%d invalid axis[%d] alarm number [%d]", __func__, __LINE__, i - AXIS_ALARM_STATUS_TOP_SR + 1, dataRg);
                continue;
            }
            updateAlarmList(AlertMsgType::AlertMsgTypeAxisAlarm, i - AXIS_ALARM_STATUS_TOP_SR, (quint16)dataRg);
            if (mPreAxisAlarmStauts[i - AXIS_ALARM_STATUS_TOP_SR] <= 0){
                mInternalAlarmCount++;
                axisAlarmStatusUpdated = true;
            }
            mPreAxisAlarmStauts[i - AXIS_ALARM_STATUS_TOP_SR] = dataRg;
            mIsNotifyAlarm = true;
        } else {
            if (mPreAxisAlarmStauts[i - AXIS_ALARM_STATUS_TOP_SR] > 0 && dataRg == 0){
                removeAlarmList(AlertMsgType::AlertMsgTypeAxisAlarm, i - AXIS_ALARM_STATUS_TOP_SR);
                mPreAxisAlarmStauts[i - AXIS_ALARM_STATUS_TOP_SR] = dataRg;
                alarmListStatusChange = false;
            }
        }
        axisAlarmStatusList.append(m_modbus->getReceiveDataRG(i));
    }
    setCurAlarmCount(mInternalAlarmCount, alarmListStatusChange);
    if (axisAlarmStatusUpdated)
        setAxisAlarmStatus(axisAlarmStatusList);
}

void CommonViewModel::updateOpeCmdStatus()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    quint8 opeCmdStatus = 0;
    int shift = 0;

    mIsNotifyOpeCmd = false;
    for(int i = OPE_CMD_STATUS_TOP_DI; i < (OPE_CMD_STATUS_TOP_DI + OPE_CMD_STATUS_BIT_NUM) && shift < OPE_CMD_STATUS_BIT_NUM; i++){
        opeCmdStatus |= (m_modbus->getReceiveDataDI(i) << shift);
        shift++;
    }
    if (opeCmdStatus != 0){
        m_preOpeCmdCount = 1;
        mIsNotifyOpeCmd = true;
        if (m_opeCmdValue != opeCmdStatus) {
            m_operationRecord->recordDispOperation(opeCmdStatus);
        }
        m_opeCmdStatusMsg = m_resource->getMessageStringValue("14", (int)opeCmdStatus, m_resource->language());
        emit opeCmdStatusMsgChanged(m_opeCmdStatusMsg);
        m_opeCmdStatusColor = m_resource->rgb(14, (int)opeCmdStatus);
        emit opeCmdStatusColorChanged(m_opeCmdStatusColor);
        setOpeCmdStatus(true);
    } else {
        setOpeCmdStatus(false);
    }
    m_opeCmdValue = opeCmdStatus;
}

void CommonViewModel::updateUserLevel()
{
    int userlevel = getUserLevel();
    (void)userlevel;
    //qDebug("%s:%d userlevel[%d]", __func__, __LINE__, userlevel);
}

void CommonViewModel::updateManualOpeSpeedStatus()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    int manualSpeed = m_modbus->getReceiveDataRG(MANUAL_SPEED_STATUS_SR);
    setManualSpeedStatus(manualSpeed);
}

void CommonViewModel::updateAutoOpeSpeedStatus()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    int autoSpeed = m_modbus->getReceiveDataRG(AUTO_SPEED_STATUS_SR);
    setAutoSpeedStatus(autoSpeed);
}

void CommonViewModel::updateTopTitleLocal(Enums::Pages currentPage, Enums::Pages curLeftPage, bool isForce)
{
    if (!m_resource){
        return;
    }
    QString topTitle = "";
    switch(currentPage) {
    case Enums::Pages::Origin:
        topTitle = m_resource->label(20, 0);
        break;
    case Enums::Pages::ManualIO:
    case Enums::Pages::ManualAxisOpe:
    case Enums::Pages::ManualAxisExpress:
        topTitle = m_resource->label(20, 1);
        break;
    case Enums::Pages::AutoAxisTimer:
    case Enums::Pages::AutoAxisCounter:
    case Enums::Pages::AutoProductMngTimer:
    case Enums::Pages::AutoProductMngCounter:
    case Enums::Pages::AutoIOTimer:
    case Enums::Pages::AutoIOCounter:
        topTitle = m_resource->label(20, 2);
        break;
    default:
        //NOP
        break;
    }
    if(topTitle.length() > 0){
        setTopTitle(topTitle);
    }
    if (isForce){
        QString forceTopTitle = "";
        switch(curLeftPage) {
        case Enums::Pages::Origin:
            forceTopTitle = m_resource->label(20, 0);
            break;
        case Enums::Pages::ManualIO:
        case Enums::Pages::ManualAxisOpe:
        case Enums::Pages::ManualAxisExpress:
            forceTopTitle = m_resource->label(20, 1);
            break;
        case Enums::Pages::AutoAxisTimer:
        case Enums::Pages::AutoAxisCounter:
        case Enums::Pages::AutoProductMngTimer:
        case Enums::Pages::AutoProductMngCounter:
        case Enums::Pages::AutoIOTimer:
        case Enums::Pages::AutoIOCounter:
            forceTopTitle = m_resource->label(20, 2);
            break;
        default:
            //NOP
            break;
        }
        if(forceTopTitle.length() > 0){
            setTopTitle(forceTopTitle);
        }
    }
}

void CommonViewModel::updateOperatingStatus()
{
    if (!mIsLedDeviceInitialized) {
        mIsLedDeviceInitialized = initLedDevice();
        if (mIsLedDeviceInitialized) {
            controlLedDevice(false, (int)CoreLibDriverManager::LedType::LedTypeRed);
        }
    }
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    bool isManualOpe = m_modbus->getReceiveDataDI(2614);
    bool isOriginOpe = m_modbus->getReceiveDataDI(2657);
    bool isAutoOpe = m_modbus->getReceiveDataDI(2615);
    bool isStepOpe = m_modbus->getReceiveDataDI(2613);

    if (isManualOpe && !isOriginOpe && !isAutoOpe){
        controlLedDevice(false, (int)CoreLibDriverManager::LedType::LedTypeBlue);
        setOperatingMode(1);
    } else if (!isManualOpe && isOriginOpe && !isAutoOpe){
        controlLedDevice(false, (int)CoreLibDriverManager::LedType::LedTypeBlue);
        setOperatingMode(2);

    } else if (!isManualOpe && !isOriginOpe && isAutoOpe){
        if (isStepOpe){
            controlLedDevice(false, (int)CoreLibDriverManager::LedType::LedTypeBlue);
            setOperatingMode(4);
        } else {
            controlLedDevice(true, (int)CoreLibDriverManager::LedType::LedTypeBlue);
            setOperatingMode(3);
        }
    } else {
        controlLedDevice(false, (int)CoreLibDriverManager::LedType::LedTypeBlue);
        setOperatingMode(0);
    }
}

void CommonViewModel::updateEmergencyStopStatus()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    bool isEmergency = m_modbus->getReceiveDataDI(2662);
    setEmergencyStatus(isEmergency);
}

void CommonViewModel::updateCurFileInfo()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    int setupFileNum = m_modbus->getReceiveDataRG(CURRENT_FILE_NUMBER_SR);
    QString curFileNumber = (setupFileNum < 10 ? "00" + QString::number(setupFileNum, 10) : \
                    (setupFileNum < 100 ? "0" + QString::number(setupFileNum, 10) : \
                    (setupFileNum < 1000 ? QString::number(setupFileNum, 10) : \
                    QString::number(setupFileNum, 10))));
    setCurFileNumber(curFileNumber);
    QString curFileName = "";
    QChar str[2];
    bool findTerminate[2];
    for(int i = 0; i < CURRENT_FILE_NAME_SIZE; i++){
        findTerminate[0] = false;
        findTerminate[1] = false;
        int setupFileNameInfo = m_modbus->getReceiveDataRG(CURRENT_FILE_NAME_SR + i);
        str[0] = (QChar)((setupFileNameInfo & 0xFF00) >> 8);
        str[1] = (QChar)(setupFileNameInfo & 0x00FF);
        if (str[0].toLatin1() == '\x0'){
            findTerminate[0] = true;
        }
        if (str[1].toLatin1() == '\x0'){
            findTerminate[1] = true;
        }
        curFileName.append(str[0].toLatin1());
        if (findTerminate[0] == true){
            break;
        }
        curFileName.append(str[1].toLatin1());
        if (findTerminate[1] == true){
            break;
        }
    }
    setCurFileName(curFileName);
}

void CommonViewModel::updateLanguageInfo()
{
    setLanguage(getLanguage());
}

void CommonViewModel::updateScreenTitle(Enums::Pages currentPage)
{
    //qDebug("%s:%d currentPage[%d]", __func__, __LINE__, currentPage);
    if (!m_resource){
        return;
    }
    QString screenTitle = "";
    switch(currentPage) {
    case Enums::Pages::Origin:
        screenTitle = m_resource->label(20, 4);
        break;
    case Enums::Pages::ManualIO:
    case Enums::Pages::ManualAxisOpe:
    case Enums::Pages::ManualAxisExpress:
        screenTitle = m_resource->label(20, 5);
        break;
    case Enums::Pages::AutoAxisTimer:
    case Enums::Pages::AutoAxisCounter:
    case Enums::Pages::AutoProductMngTimer:
    case Enums::Pages::AutoProductMngCounter:
    case Enums::Pages::AutoIOTimer:
    case Enums::Pages::AutoIOCounter:
        screenTitle = m_resource->label(20, 6);
        break;
    case Enums::Pages::Monitor:
    case Enums::Pages::MonitorIOExpress:
    case Enums::Pages::MonitorIOForced:
    case Enums::Pages::MonitorDriver:
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    case Enums::Pages::MonitorDriverWaveform:
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
    case Enums::Pages::MonitorInternal:
    case Enums::Pages::FunctionDriverMonitor:
        screenTitle = m_resource->label(20, 7);
        break;
    case Enums::Pages::TeachingMode:
        screenTitle = m_resource->label(20, 31);
        break;
    case Enums::Pages::TeachingPosSet:
        screenTitle = m_resource->label(20, 32);
        break;
    case Enums::Pages::TeachingPosFix:
        screenTitle = m_resource->label(20, 32);
        break;
    case Enums::Pages::TeachingStandardPalletize:
        screenTitle = m_resource->label(20, 33);
        break;
    case Enums::Pages::TeachingFreePalletize:
        screenTitle = m_resource->label(20, 33);
        break;
    case Enums::Pages::TeachingTimer:
        screenTitle = m_resource->label(20, 34);
        break;
    case Enums::Pages::TeachingCounter:
        screenTitle = m_resource->label(20, 35);
        break;
    case Enums::Pages::TeachingPass:
        screenTitle = m_resource->label(20, 36);
        break;
    case Enums::Pages::TeachingProgram:
        screenTitle = m_resource->label(20, 37);
        break;
    case Enums::Pages::FunctionControllerSetSystem:
    case Enums::Pages::FunctionControllerSetUser:
    case Enums::Pages::FunctionControllerSetNetwork:
        screenTitle = m_resource->label(20, 27);
        break;
    case Enums::Pages::FunctionMechanicalParamModelSelect:
    case Enums::Pages::FunctionMechanicalParam:
    case Enums::Pages::FunctionMechanicalParamDrvParam:
    case Enums::Pages::FunctionMPDrvParamSimpleSet:
        screenTitle = m_resource->label(20, 11);
        break;
    case Enums::Pages::FunctionAreaSet:
    case Enums::Pages::FunctionAreaFix:
        screenTitle = m_resource->label(20, 12);
        break;
    case Enums::Pages::FunctionMaintainCheck:
    case Enums::Pages::FunctionMaintain:
        screenTitle = m_resource->label(20, 13);
        break;
    case Enums::Pages::FunctionProductionMng:
        screenTitle = m_resource->label(20, 14);
        break;
    case Enums::Pages::FunctionAbsSet:
        screenTitle = m_resource->label(20, 15);
        break;
    case Enums::Pages::FunctionDeceleration:
        screenTitle = m_resource->label(20, 16);
        break;
    case Enums::Pages::FunctionVersionMng:
        screenTitle = m_resource->label(20, 17);
        break;
    case Enums::Pages::History:
    case Enums::Pages::HistoryAlarm:
    case Enums::Pages::HistoryOpe:
    case Enums::Pages::HistoryIO:
        screenTitle = m_resource->label(20, 28);
        break;
    case Enums::Pages::SetupChgFileMng:
    case Enums::Pages::SetupChgExportUsb:
    case Enums::Pages::SetupChgImportUsb:
        screenTitle = updateSpecialScreenTitle(m_prePage);
        break;
    default:
        break;
    }
    if(screenTitle.length() > 0){
        m_resource->setTitleString(screenTitle);
    }
}

QString CommonViewModel::updateSpecialScreenTitle(Enums::Pages prePage)
{
    QString screenTitle = "";
    qDebug("%s:%d prePage[%d] m_curLeftPage[%d]", __func__, __LINE__, prePage, m_curLeftPage);
    if (!m_resource){
        return screenTitle;
    }
    switch(prePage) {
    case Enums::Pages::Origin:
        screenTitle = m_resource->label(20, 4, m_resource->language());
        break;
    case Enums::Pages::ManualIO:
    case Enums::Pages::ManualAxisOpe:
    case Enums::Pages::ManualAxisExpress:
        screenTitle = m_resource->label(20, 5, m_resource->language());
        break;
    case Enums::Pages::AutoAxisTimer:
    case Enums::Pages::AutoAxisCounter:
    case Enums::Pages::AutoProductMngTimer:
    case Enums::Pages::AutoProductMngCounter:
    case Enums::Pages::AutoIOTimer:
    case Enums::Pages::AutoIOCounter:
        screenTitle = m_resource->label(20, 6, m_resource->language());
        break;
    case Enums::Pages::Monitor:
    case Enums::Pages::MonitorIOExpress:
    case Enums::Pages::MonitorIOForced:
    case Enums::Pages::MonitorDriver:
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    case Enums::Pages::MonitorDriverWaveform:
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
    case Enums::Pages::MonitorInternal:
    case Enums::Pages::FunctionDriverMonitor:
        screenTitle = m_resource->label(20, 7, m_resource->language());
        break;
    case Enums::Pages::TeachingMode:
        screenTitle = m_resource->label(20, 31, m_resource->language());
        break;
    case Enums::Pages::TeachingPosSet:
        screenTitle = m_resource->label(20, 32, m_resource->language());
        break;
    case Enums::Pages::TeachingPosFix:
        screenTitle = m_resource->label(20, 32, m_resource->language());
        break;
    case Enums::Pages::TeachingStandardPalletize:
        screenTitle = m_resource->label(20, 33, m_resource->language());
        break;
    case Enums::Pages::TeachingFreePalletize:
        screenTitle = m_resource->label(20, 33, m_resource->language());
        break;
    case Enums::Pages::TeachingTimer:
        screenTitle = m_resource->label(20, 34, m_resource->language());
        break;
    case Enums::Pages::TeachingCounter:
        screenTitle = m_resource->label(20, 35, m_resource->language());
        break;
    case Enums::Pages::TeachingPass:
        screenTitle = m_resource->label(20, 36, m_resource->language());
        break;
    case Enums::Pages::TeachingProgram:
        screenTitle = m_resource->label(20, 37, m_resource->language());
        break;
    case Enums::Pages::FunctionControllerSetSystem:
    case Enums::Pages::FunctionControllerSetUser:
    case Enums::Pages::FunctionControllerSetNetwork:
        screenTitle = m_resource->label(20, 27, m_resource->language());
        break;
    case Enums::Pages::FunctionMechanicalParamModelSelect:
    case Enums::Pages::FunctionMechanicalParam:
    case Enums::Pages::FunctionMechanicalParamDrvParam:
    case Enums::Pages::FunctionMPDrvParamSimpleSet:
        screenTitle = m_resource->label(20, 11, m_resource->language());
        break;
    case Enums::Pages::FunctionAreaSet:
    case Enums::Pages::FunctionAreaFix:
        screenTitle = m_resource->label(20, 12, m_resource->language());
        break;
    case Enums::Pages::FunctionMaintainCheck:
    case Enums::Pages::FunctionMaintain:
        screenTitle = m_resource->label(20, 13, m_resource->language());
        break;
    case Enums::Pages::FunctionProductionMng:
        screenTitle = m_resource->label(20, 14, m_resource->language());
        break;
    case Enums::Pages::FunctionAbsSet:
        screenTitle = m_resource->label(20, 15, m_resource->language());
        break;
    case Enums::Pages::FunctionDeceleration:
        screenTitle = m_resource->label(20, 16, m_resource->language());
        break;
    case Enums::Pages::FunctionVersionMng:
        screenTitle = m_resource->label(20, 17, m_resource->language());
        break;
    case Enums::Pages::History:
    case Enums::Pages::HistoryAlarm:
    case Enums::Pages::HistoryOpe:
    case Enums::Pages::HistoryIO:
        screenTitle = m_resource->label(20, 28, m_resource->language());
        break;
    default:
        break;
    }
    return screenTitle;
}

void CommonViewModel::setCurFileNumber(QString curFileNumber)
{
    if (m_curFileNumber == curFileNumber){
        return;
    }
    m_curFileNumber = curFileNumber;
    emit curFileNumberChanged(m_curFileNumber);
}

void CommonViewModel::setCurFileName(QString curFileName)
{
    if (m_curFileName == curFileName){
        return;
    }
    m_curFileName = curFileName;
    emit curFileNameChanged(m_curFileName);
}

void CommonViewModel::setLanguage(int language)
{
    if (m_language == language){
        return;
    }
    if (m_resource) {
#ifdef REAL_TARGET
#else // REAL_TARGET
        if (!mIsAlreadyLanguage){
            m_language = 1;         // default:1
            m_resource->setLanguage(m_language);
            mIsAlreadyLanguage = true;
        } else {
#endif // REAL_TARGET
            m_resource->setLanguage(language);
#ifdef REAL_TARGET
#else // REAL_TARGET
        }
#endif // REAL_TARGET
    }
    updateTopTitleLocal(m_currentPage, m_curLeftPage, true);
    updateScreenTitle(m_currentPage);
    m_language = language;
    emit languageChanged(m_language);
}

QString CommonViewModel::topTitle() const
{
    return m_pageTitle;
}

void CommonViewModel::setTopTitle(QString& newValue)
{
    if (m_pageTitle == newValue)
        return;

    m_pageTitle = newValue;
    emit topTitleChanged(m_pageTitle);
}

void CommonViewModel::setFileChange(bool change)
{
    if (m_fileChange == change)
        return;

    m_fileChange = change;
    emit fileChangeChanged(m_fileChange);
}

void CommonViewModel::updateScreenId(Enums::Pages currentPage, int subPage)
{
    Enums::MainScreenId mainScreenId = Enums::MainScreenId::MainScreenIdInvalid;
    Enums::SubScreenId subScreenId = Enums::SubScreenId::SubScreenIdInvalid;
    switch(currentPage) {
    case Enums::Pages::Origin:
        mainScreenId = Enums::MainScreenId::MainScreenIdOrigin;
        subScreenId = Enums::SubScreenId::SubScreenIdOrigin;
        break;
    case Enums::Pages::ManualIO:
        mainScreenId = Enums::MainScreenId::MainScreenIdManual;
        subScreenId = Enums::SubScreenId::SubScreenIdManualIO1;
        break;
    case Enums::Pages::ManualAxisOpe:
        mainScreenId = Enums::MainScreenId::MainScreenIdManual;
        subScreenId = Enums::SubScreenId::SubScreenIdManualAxisOpe;
        break;
    case Enums::Pages::ManualAxisExpress:
        mainScreenId = Enums::MainScreenId::MainScreenIdManual;
        subScreenId = Enums::SubScreenId::SubScreenIdManualAxisExpress;
        break;
    case Enums::Pages::AutoAxisTimer:
        mainScreenId = Enums::MainScreenId::MainScreenIdAuto;
        subScreenId = Enums::SubScreenId::SubScreenIdAutoAxisTimer;
        break;
    case Enums::Pages::AutoAxisCounter:
        mainScreenId = Enums::MainScreenId::MainScreenIdAuto;
        subScreenId = Enums::SubScreenId::SubScreenIdAutoAxisCounter;
        break;
    case Enums::Pages::AutoProductMngTimer:
        mainScreenId = Enums::MainScreenId::MainScreenIdAuto;
        subScreenId = Enums::SubScreenId::SubScreenIdAutoProductMngTimer;
        break;
    case Enums::Pages::AutoProductMngCounter:
        mainScreenId = Enums::MainScreenId::MainScreenIdAuto;
        subScreenId = Enums::SubScreenId::SubScreenIdAutoProductMngCounter;
        break;
    case Enums::Pages::AutoIOTimer:
        mainScreenId = Enums::MainScreenId::MainScreenIdAuto;
        subScreenId = Enums::SubScreenId::SubScreenIdAutoIOTimer;
        break;
    case Enums::Pages::AutoIOCounter:
        mainScreenId = Enums::MainScreenId::MainScreenIdAuto;
        subScreenId = Enums::SubScreenId::SubScreenIdAutoIOCounter;
        break;
    case Enums::Pages::TeachingMode:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingMode;
        subScreenId = Enums::SubScreenId::SubScreenIdTeachingDefault;
        break;
    case Enums::Pages::TeachingPosSet:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingPosSet;
        subScreenId = Enums::SubScreenId::SubScreenIdTeachingPosSet;
        break;
    case Enums::Pages::TeachingPosFix:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingPosFix;
        subScreenId = Enums::SubScreenId::SubScreenIdTeachingPosFix;
        break;
    case Enums::Pages::TeachingStandardPalletize:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingPalletize;
        subScreenId = (Enums::SubScreenId)subPage;
        break;
    case Enums::Pages::TeachingFreePalletize:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingPalletize;
        subScreenId = (Enums::SubScreenId)subPage;
        break;
    case Enums::Pages::TeachingTimer:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingTimer;
        subScreenId = Enums::SubScreenId::SubScreenIdTeachingTimerDefault;
        break;
    case Enums::Pages::TeachingCounter:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingCounter;
        subScreenId = GET_COUNTER_VIEW_SUB_SCREEN_ID(subPage);
        break;
    case Enums::Pages::TeachingPass:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingPass;
        subScreenId = Enums::SubScreenId::SubScreenIdTeachingPassDefault;
        break;
    case Enums::Pages::TeachingProgram:
        mainScreenId = Enums::MainScreenId::MainScreenIdTeachingProgram;
        subScreenId = (Enums::SubScreenId)subPage;
        break;
    case Enums::Pages::MonitorIOExpress:
        mainScreenId = Enums::MainScreenId::MainScreenIdMonitor;
        subScreenId = Enums::SubScreenId::SubScreenIdMonitorIOExpress;
        break;
    case Enums::Pages::MonitorIOForced:
        mainScreenId = Enums::MainScreenId::MainScreenIdMonitor;
        subScreenId = Enums::SubScreenId::SubScreenIdMonitorIOForced;
        break;
    case Enums::Pages::MonitorDriver:
        mainScreenId = Enums::MainScreenId::MainScreenIdMonitor;
        subScreenId = Enums::SubScreenId::SubScreenIdMonitorRealTime;
        break;
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    case Enums::Pages::MonitorDriverWaveform:
        mainScreenId = Enums::MainScreenId::MainScreenIdMonitor;
        subScreenId = Enums::SubScreenId::SubScreenIdMonitorWaveform;
        break;
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
    case Enums::Pages::MonitorInternal:
        mainScreenId = Enums::MainScreenId::MainScreenIdMonitor;
        subScreenId = Enums::SubScreenId::SubScreenIdMonitorInternal;
        break;
    case Enums::Pages::FunctionMechanicalParamModelSelect:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionMachineParam;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionMachineParamModelSelect;
        break;
    case Enums::Pages::FunctionMechanicalParam:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionMachineParam;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionMachineParam;
        break;
    case Enums::Pages::FunctionMechanicalParamDrvParam:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionMachineParam;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionMachineParamDriverParam;
        break;
    case Enums::Pages::FunctionMPDrvParamSimpleSet:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionMachineParam;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionMPDriverParamSimpleSet;
        break;
    case Enums::Pages::FunctionDriverMonitor:
        mainScreenId = Enums::MainScreenId::MainScreenIdMonitor;
        subScreenId = Enums::SubScreenId::SubScreenIdMonitorRealTime;
        break;
    case Enums::Pages::FunctionControllerSetSystem:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionControllerSetSystem;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionControllerSetDefault;
        break;
    case Enums::Pages::FunctionControllerSetUser:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionControllerSetSystem;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionControllerSetUser;
        break;
    case Enums::Pages::FunctionControllerSetNetwork:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionControllerSetSystem;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionControllerSetNetwork;
        break;
    case Enums::Pages::FunctionAreaSet:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionAreaSet;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionAreaSetDefault;
        break;
    case Enums::Pages::FunctionAreaFix:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionAreaSet;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionAreaFix;
        break;
    case Enums::Pages::FunctionMaintainCheck:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionMaintain;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionMaintainDefault;
        break;
    case Enums::Pages::FunctionMaintain:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionMaintain;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionMaintain;
        break;
    case Enums::Pages::FunctionProductionMng:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionProductionMng;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionProductionMngDefault;
        break;
    case Enums::Pages::FunctionAbsSet:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionAbsSet;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionAbsSetDefault;
        break;
    case Enums::Pages::FunctionDeceleration:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionDeceleration;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionDecelerationDefault;
        break;
    case Enums::Pages::FunctionVersionMng:
        mainScreenId = Enums::MainScreenId::MainScreenIdFunctionVersionMng;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionVersionMngDefault;
        break;
    case Enums::Pages::HistoryAlarm:
        mainScreenId = Enums::MainScreenId::MainScreenIdHistory;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionHistoryAlarm;
        break;
    case Enums::Pages::HistoryOpe:
        mainScreenId = Enums::MainScreenId::MainScreenIdHistory;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionHistoryOperation;
        break;
    case Enums::Pages::HistoryIO:
        mainScreenId = Enums::MainScreenId::MainScreenIdHistory;
        subScreenId = Enums::SubScreenId::SubScreenIdFunctionHistoryIO;
        break;
    case Enums::Pages::SetupChgFileMng:
        mainScreenId = Enums::MainScreenId::MainScreenIdSetupChgExtMem;
        subScreenId = Enums::SubScreenId::SubScreenIdSetupChgDefault;
        break;
    case Enums::Pages::SetupChgExportUsb:
        mainScreenId = Enums::MainScreenId::MainScreenIdSetupChgExtMem;
        subScreenId = Enums::SubScreenId::SubScreenIdSetupChgUsbEntry;
        break;
    case Enums::Pages::SetupChgImportUsb:
        mainScreenId = Enums::MainScreenId::MainScreenIdSetupChgExtMem;
        subScreenId = Enums::SubScreenId::SubScreenIdSetupChgUsbImport;
        break;
    default:
        mainScreenId = Enums::MainScreenId::MainScreenIdInvalid;                // TODO
        subScreenId = Enums::SubScreenId::SubScreenIdInvalid;                   // TODO
        break;
    }
    //qDebug("%s:%d mainScreenId[%d] subScreenId[%d]",
    //    __func__, __LINE__, mainScreenId, subScreenId);
    if (m_curMainScreenId != mainScreenId || m_curSubScreenId != subScreenId){
        changeScreenId(mainScreenId, subScreenId);
    }
}

void CommonViewModel::updateDateTime()
{
    if (!m_modbus) return;
    QString dateStr = "";
    QString timeStr = "";
    int year = m_modbus->getReceiveDataRG(2723);
    int month = m_modbus->getReceiveDataRG(2724);
    int day = m_modbus->getReceiveDataRG(2725);
    int hour = m_modbus->getReceiveDataRG(2726);
    int minutes = m_modbus->getReceiveDataRG(2727);
    int seconds = m_modbus->getReceiveDataRG(2728);

    if(m_permissionTimeUpdate && mPreDay != day){
        //qDebug("%s:%d mPreDay[%d] day[%d]", __func__, __LINE__, mPreDay, day);
        dateStr.append(QString::asprintf("%04d", year));
        dateStr.append("/");
        dateStr.append(QString::asprintf("%02d", month));
        dateStr.append("/");
        dateStr.append(QString::asprintf("%02d", day));
        setCurDate(dateStr);
#ifdef REAL_TARGET
        QString yearStr = "";
        yearStr.append(QString::asprintf("%04d", year));
        m_year = yearStr;
        emit yearChanged();
        QString monthStr = "";
        monthStr.append(QString::asprintf("%02d", month));
        m_month = monthStr;
        emit monthChanged();
        QString dayStr = "";
        dayStr.append(QString::asprintf("%02d", day));
        m_day = dayStr;
        emit dayChanged();
#endif // REAL_TARGET
        mPreDay = day;
    }
    if(m_permissionTimeUpdate && mPreSeconds != seconds){
        //qDebug("%s:%d mPreSeconds[%d] seconds[%d]", __func__, __LINE__, mPreSeconds, seconds);
        timeStr.append(QString::asprintf("%02d", hour));
        timeStr.append(":");
        timeStr.append(QString::asprintf("%02d", minutes));
        setCurTimeUptoMinutes(timeStr);
        timeStr.append(":");
        timeStr.append(QString::asprintf("%02d", seconds));
        setCurTime(timeStr);
#ifdef REAL_TARGET
        QString hourStr = "";
        hourStr.append(QString::asprintf("%02d", hour));
        m_hour = hourStr;
        emit hourChanged();
        QString minutesStr = "";
        minutesStr.append(QString::asprintf("%02d", minutes));
        m_minutes = minutesStr;
        emit minutesChanged();
        QString secondsStr = "";
        secondsStr.append(QString::asprintf("%02d", seconds));
        m_seconds = secondsStr;
        emit secondsChanged();
#endif // REAL_TARGET
        mPreSeconds = seconds;
    }
}

void CommonViewModel::controlLedDevice(bool isOnOff, int ledType)
{
    if (!m_communication){
        qDebug("%s:%d m_communication is not share", __func__, __LINE__);
        return;
    }
    PhyKeyManager *phyKeyManager = m_communication->getPhyKeyManagerInstance();
    if (!phyKeyManager){
        qDebug("%s:%d phyKeyManager is not share", __func__, __LINE__);
        return;
    }
    if (!mIsLedDeviceInitialized){
        qDebug("%s:%d mIsLedDeviceInitialized is not open", __func__, __LINE__);
        return;
    }
    switch(ledType){
    case CoreLibDriverManager::LedType::LedTypeBlue:
        phyKeyManager->ctrlLed(isOnOff, CoreLibDriverManager::LedType::LedTypeBlue);
        mIsBlueLed = isOnOff;
        break;
    case CoreLibDriverManager::LedType::LedTypeGreen:
        phyKeyManager->ctrlLed(isOnOff, CoreLibDriverManager::LedType::LedTypeGreen);
        break;
    case CoreLibDriverManager::LedType::LedTypeRed:
        phyKeyManager->ctrlLed(isOnOff, CoreLibDriverManager::LedType::LedTypeRed);
        break;
    default:
        break;
    }
}

void CommonViewModel::updateSystemTime(stModBusCommunicationData command)
{
    quint16 year = command.readBuf[0];
    quint16 month = command.readBuf[1];
    quint16 day = command.readBuf[2];
    quint16 hour = command.readBuf[3];
    quint16 minutes = command.readBuf[4];
    quint16 seconds = command.readBuf[5];
#ifdef REAL_TARGET
    QString dateStr = "";
    QString timeStr = "";
#endif
    m_year = QString::asprintf("%04d", year);
    emit yearChanged();
    m_month = QString::asprintf("%02d", month);
    emit monthChanged();
    m_day = QString::asprintf("%02d", day);
    emit dayChanged();
#ifdef REAL_TARGET
    dateStr.append(m_year);
    dateStr.append("/");
    dateStr.append(m_month);
    dateStr.append("/");
    dateStr.append(m_day);
    setCurDate(dateStr);
#endif
    m_hour = QString::asprintf("%02d", hour);
    emit hourChanged();
    m_minutes = QString::asprintf("%02d", minutes);
    emit minutesChanged();
    m_seconds = QString::asprintf("%02d", seconds);
    emit secondsChanged();
#ifdef REAL_TARGET
    timeStr.append(m_hour);
    timeStr.append(":");
    timeStr.append(m_minutes);
    timeStr.append(":");
    timeStr.append(m_seconds);
    setCurTime(timeStr);
#endif
    m_systemTimeStatus = true;
    emit systemTimeStatusChanged(m_systemTimeStatus);
}

void CommonViewModel::updateAlarmList(int alarmType, int axisNumber, quint16 msgNo)
{
//    qDebug("%s:%d alarmType[%d] axisNumber[%d] msgNo[%d] mInternalAlarmCount[%d]",
//        __func__, __LINE__, alarmType, axisNumber, msgNo, mInternalAlarmCount);
    if (!m_resource) return;
    if (mInternalAlarmCount < 0 || mInternalAlarmCount >= 10){
        return;
    }
    if (axisNumber < 0 || axisNumber >= 8){
        return;
    }
    if (msgNo <= 0 || msgNo > 256){
        return;
    }
    switch(alarmType){
    case AlertMsgType::AlertMsgTypeAlarm:{
        AlarmInfo *alarmInfo = new  AlarmInfo();
        alarmInfo->setType(AlarmInfo::Type::Normal);
        alarmInfo->setMsgNumber((int)msgNo);
        alarmInfo->setAlarmNumber((msgNo < 100 ? (msgNo < 10 ? "00" + QString::number((int)msgNo)
                                    : "0" + QString::number((int)msgNo)) : QString::number((int)msgNo)));
        alarmInfo->setAlarmDetailsColor(m_resource->rgb(
            12, ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2)));
        alarmInfo->setAlarmIndex(ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2));
        if (mInternalAlarmCount == 0){
            delete m_alarmList.takeFirst();
            m_alarmList.insert(0, alarmInfo);
        } else {
            if (isAlarmExist(AlarmInfo::Type::Normal)) {
                delete m_alarmList.takeFirst();
                m_alarmList.insert(0, alarmInfo);
            } else {
                delete m_alarmList.takeLast();
                m_alarmList.insert(0, alarmInfo);
            }
        }
        m_operationRecord->recordDispNormalAlarm(msgNo);
        notifyAlarm(*alarmInfo);
        break;
    }
    case AlertMsgType::AlertMsgTypeSystem:{
        AlarmInfo *alarmInfo = new  AlarmInfo();
        alarmInfo->setType(AlarmInfo::Type::System);
        alarmInfo->setMsgNumber((int)msgNo);
        alarmInfo->setAlarmNumber((msgNo < 100 ? (msgNo < 10 ? "00" + QString::number((int)msgNo)
                                    : "0" + QString::number((int)msgNo)) : QString::number((int)msgNo)));
        alarmInfo->setAlarmDetailsColor(m_resource->rgb(
            12, SYSTEM_ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2)));
        alarmInfo->setAlarmIndex(SYSTEM_ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2));
        if (mInternalAlarmCount == 0){
            delete m_alarmList.takeFirst();
            m_alarmList.insert(0, alarmInfo);
        } else {
            auto index = mIsAlarmOccurred ? 1 : 0;
            if (isAlarmExist(AlarmInfo::Type::System)) {
                delete m_alarmList.takeAt(index);
                m_alarmList.insert(index, alarmInfo);
            } else {
                delete m_alarmList.takeLast();
                m_alarmList.insert(index, alarmInfo);
            }
        }
        m_operationRecord->recordDispSystemAlarm(msgNo);
        notifyAlarm(*alarmInfo);
        break;
    }
    case AlertMsgType::AlertMsgTypeAxisAlarm:{
        AlarmInfo *alarmInfo = new  AlarmInfo();
        alarmInfo->setType(static_cast<AlarmInfo::Type>(axisNumber));
        alarmInfo->setMsgNumber((int)msgNo);
        if (msgNo >= 1 && msgNo <= 64){
            alarmInfo->setAlarmNumber((msgNo < 100 ? (msgNo < 10 ? "00" + QString::number((int)msgNo)
                                    : "0" + QString::number((int)msgNo)) : QString::number((int)msgNo)));
            alarmInfo->setAlarmDetailsColor(m_resource->rgb(
                12, AXIS_ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2)));
            alarmInfo->setAlarmIndex(AXIS_ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2));
        } else {
            alarmInfo->setAlarmNumber("12-" + (msgNo - 64 < 100 ? (msgNo - 64 < 10 ? "00" + QString::number((int)(msgNo - 64))
                                            : "0" + QString::number((int)(msgNo - 64))) : QString::number((int)(msgNo - 64))));
            alarmInfo->setAlarmDetailsColor(m_resource->rgb(
                12, AXIS_ALARM_MSG_TYPE2_START_INDEX + (int)((msgNo - 65) * 2)));
            alarmInfo->setAlarmIndex(AXIS_ALARM_MSG_TYPE2_START_INDEX + (int)((msgNo - 65) * 2));
        }
        if (mInternalAlarmCount > 0){
            delete m_alarmList.takeLast();
            int insertIndex = checkAlarmOrder(alarmType, axisNumber);
            if(insertIndex < 0){
                insertIndex = 0;
            }
            m_alarmList.insert(insertIndex, alarmInfo);
        } else {
            delete m_alarmList.takeFirst();
            m_alarmList.insert(0, alarmInfo);
        }
        m_operationRecord->recordDispAxisAlarm(msgNo, axisNumber);
        notifyAlarm(*alarmInfo);
        break;
    }
    default:
        break;
    }
}

void CommonViewModel::removeAlarmList(int alarmType, int axisNumber)
{
    if (!m_resource) return;
    if (mInternalAlarmCount < 0 || mInternalAlarmCount > 10){
        return;
    }
    if (axisNumber < 0 || axisNumber >= 8){
        return;
    }
    if (mInternalAlarmCount <= 0){
        return;
    }
    switch(alarmType){
    case AlertMsgType::AlertMsgTypeAxisAlarm:{
        int len = m_alarmList.length();
        AlarmInfo::Type removeAlarmType = static_cast<AlarmInfo::Type>(AlarmInfo::Type::Axis1 + axisNumber);
        for (int i = 0; i < len; i++) {
            auto alarm = dynamic_cast<AlarmInfo *>(m_alarmList.at(i));
            if (alarm->type() == removeAlarmType) {
                delete m_alarmList.takeAt(i);
                mInternalAlarmCount--;
                mIsNotifyAlarm = true;
                AlarmInfo *alarmInfo = new AlarmInfo(this);
                m_alarmList.append(alarmInfo);
            }
        }
        break;
    }
    case AlertMsgType::AlertMsgTypeAlarm:{
        int len = m_alarmList.length();
        for (int i = 0; i < len; i++) {
            auto alarm = dynamic_cast<AlarmInfo *>(m_alarmList.at(i));
            if (alarm->type() == AlarmInfo::Type::Normal) {
                delete m_alarmList.takeAt(i);
                AlarmInfo *alarmInfo = new AlarmInfo(this);
                m_alarmList.append(alarmInfo);
            }
        }
        break;
    }
    case AlertMsgType::AlertMsgTypeSystem:{
        int len = m_alarmList.length();
        for (int i = 0; i < len; i++) {
            auto alarm = dynamic_cast<AlarmInfo *>(m_alarmList.at(i));
            if (alarm->type() == AlarmInfo::Type::System) {
                delete m_alarmList.takeAt(i);
                AlarmInfo *alarmInfo = new AlarmInfo(this);
                m_alarmList.append(alarmInfo);
            }
        }
        break;
    }
    default:
        break;
    }
}

int CommonViewModel::getAxisAlarmRemoveIndex(int axisNumber)
{
    int removeIndex = -1;
    int alarmCnt = 0;
    int sysAlarmCnt = 0;
    bool findRemoveIndex = false;
    bool alarmListTopSearch = false;

    for(int i = 0; i < mInternalAlarmCount; i++){
        AlarmInfo *alarmInfo = (AlarmInfo *)(m_alarmList.at(i));
        if(alarmInfo->type() == AlarmInfo::Type::Normal){
            alarmCnt++;
            continue;
        } else if (alarmInfo->type() == AlarmInfo::Type::System){
            sysAlarmCnt++;
            continue;
        } else {
            for(int j = 0; j < 8; j++){
                if(alarmInfo->type() == j){
                    if (j == axisNumber){
                        removeIndex = i;
                        findRemoveIndex = true;
                        break;
                    }
                } else {
                    if (j == 7){
                        alarmListTopSearch = true;
                    }
                }
            }
            if (findRemoveIndex){
                alarmListTopSearch = false;
                break;
            }
        }
        if (alarmListTopSearch) {
            alarmListTopSearch = false;
            continue;
        }
    }
    //qDebug("%s:%d removeIndex[%d]", __func__, __LINE__, removeIndex);
    return removeIndex;
}

void CommonViewModel::updateAlarmMsgList()
{
    emit alarmListChanged(m_alarmList);
}

void CommonViewModel::updateErrorStatus(quint16 msgNo)
{
    if (msgNo <= 0 || msgNo > 256){
        qDebug("%s:%d msgNo[%d]", __func__, __LINE__, msgNo);
        return;
    }
    if (m_errorStatus != msgNo) {
        m_errorStatus = msgNo;
        emit errorStatusChanged();
    }
}

void CommonViewModel::updateUserPassword(stModBusCommunicationData command)
{
    if (mIsUpdateUserPassword || !m_communication){
        return;
    }
    if (command.funcCode != QModbusPdu::FunctionCode::ReadHoldingRegisters &&
        command.readAddress != USER_PASSWORD_TOP_HR &&
        command.comSize != 5){
        return;
    }
    QList<QString> curUserPassword;
    for(int i = 0; i < MAX_USER_LEVEL; i++){
        QString password =
            (command.readBuf[i] < 10 ? "000" + QString::number((int)command.readBuf[i]) :
             command.readBuf[i] < 100 ? "00" + QString::number((int)command.readBuf[i]) :
             command.readBuf[i] < 1000 ? "0" + QString::number((int)command.readBuf[i]) :
                                               QString::number((int)command.readBuf[i]));
        curUserPassword.append(password);
    }
    setUserPassword(curUserPassword);
}

void CommonViewModel::updateUserPasswordIndividual(stModBusCommunicationData command)
{
//    qDebug("%s:%d readAddress[%d] m_userLevel[%d] mUserLevelPassword[%d] readBuf[%d]", __func__, __LINE__,
//        command.readAddress, m_userLevel, mUserLevelPassword[m_userLevel], command.readBuf[0]);
    if(command.readAddress == USER_PASSWORD_TOP_HR + m_userLevel &&
        mUserLevelPassword[m_userLevel] == command.readBuf[0]){
        m_userLevelPassword = true;
        emit userLevelPasswordChanged(m_userLevelPassword);
        updateUserPassword(command.readBuf[0]);
    } else {
        m_userLevelPassword = false;
        emit userLevelPasswordChanged(m_userLevelPassword);
    }
}

void CommonViewModel::updateUserPassword(quint16 userPassword)
{
    if (m_userLevel < 0 || m_userLevel > 4){
        return;
    }
    QString updatePassword = (userPassword < 10 ? "000" + QString::number((int)userPassword) :
                              userPassword < 100 ? "00" + QString::number((int)userPassword) :
                              userPassword < 1000 ? "0" + QString::number((int)userPassword) : QString::number((int)userPassword));
    //qDebug() << "updatePassword:" << updatePassword;
    m_userPassword.replace(m_userLevel, updatePassword);
    emit userPasswordChanged(m_userPassword);
}

void CommonViewModel::updateFileChange()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return;
    }
    setFileChange(m_modbus->getReceiveDataDI(2690));
}

void CommonViewModel::updateBuzzerInfo(stModBusCommunicationData command)
{
    quint16 data = command.readBuf[0];
    quint8 isonff = (data >>8) & 0xFF;
    quint8 buzzerType = data & 0xFF;
    auto phyKeyManager = m_communication->getPhyKeyManagerInstance();

    if(phyKeyManager){
        phyKeyManager->setCurrentBuzzerState(isonff);
        phyKeyManager->setCurrentBuzzerIndex(buzzerType);
    }
}

void CommonViewModel::updateAxisCurrentPosition()
{
    for(int i = 0; i < 5; i++){
        AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
        QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_5_POS_ADDR_LIST.at(i)),66, 2);
        m_axisInfo->setPosition(axis,position);
    }
}

void CommonViewModel::updateJogRatioSpeedInfo(stModBusCommunicationData command)
{
    for (int i = 0; i < command.comSize ; i++) {
        if(i >= 0 && i <= 2){
           m_JogLimitspeed.replace(i, command.readBuf[i]);
        }else {
           m_JogRatio.replace(i-3,command.readBuf[i]);
        }
    }
    emit jogRatioChanged(m_JogRatio);
    emit jogLimitspeedChanged(m_JogLimitspeed);
}

int CommonViewModel::checkAlarmOrder(int alarmType, int axisNumber)
{
    int insertIndex = -1;
    int i = 0, j = 0;
    int alarmCnt = 0;
    int sysAlarmCnt = 0;
    bool isFindIndex = false;
    bool alarmListTopSearch = false;
    if (!m_resource) return insertIndex;
    switch(alarmType){
    case AlertMsgType::AlertMsgTypeAlarm:{
        insertIndex = 0;
        break;
    }
    case AlertMsgType::AlertMsgTypeSystem:{
        for(i = 0; i < mInternalAlarmCount; i++){
            AlarmInfo *alarmInfo = (AlarmInfo *)(m_alarmList.at(i));
            if(alarmInfo->type() == AlarmInfo::Type::Normal) {
                continue;
            } else {
                insertIndex = i;
                break;
            }
        }
        if (i == mInternalAlarmCount){
            insertIndex = i;
        }
        break;
    }
    case AlertMsgType::AlertMsgTypeAxisAlarm:{
        for(i = 0; i < mInternalAlarmCount; i++){
            AlarmInfo *alarmInfo = (AlarmInfo *)(m_alarmList.at(i));
            if(alarmInfo->type() == AlarmInfo::Type::Normal) {
                alarmCnt++;
                continue;
            } else if (alarmInfo->type() == AlarmInfo::Type::System) {
                sysAlarmCnt++;
                continue;
            } else {
                for(j = 0; j < 8; j++){
                    //qDebug() << "alarmType:" << alarmInfo->type() << "resource alarm:" << m_resource->label(52, 5 + j, m_resource->language());
                    if(alarmInfo->type() == j) {
                        if (j == axisNumber){
                            insertIndex = alarmCnt + sysAlarmCnt + j + 1;
                            isFindIndex = true;
                            alarmListTopSearch = false;
                            break;
                        }
                    } else {
                        if (j == 7){
                            alarmListTopSearch = true;
                        }
                    }
                }
                if (alarmListTopSearch) {
                    alarmListTopSearch = false;
                    continue;
                }
            }
        }
        if (isFindIndex == false){
            insertIndex = mInternalAlarmCount;
        }
        break;
    }
    default:
        break;
    }
    return insertIndex;
}

bool CommonViewModel::isAlarmExist(AlarmInfo::Type type)
{
    bool ret = false;
    const auto len = m_alarmList.length();
    AlarmInfo *alarm;
    for (auto i = 0; i < len; i++) {
        alarm = static_cast<AlarmInfo *>(m_alarmList.at(i));
        if (alarm->type() == type) {
            ret = true;
            break;
        }
    }
    return ret;
}

void CommonViewModel::modbusUpdateLanguageCode(int code)
{
    QList<int> writeData;
    writeData.append(code);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister, LANGUAGE_CODE_HR, 1, writeData);
    m_communication->setModbusWriteCommand(command);
#ifdef REAL_TARGET
    setLanguage(code);
#endif // REAL_TARGET
}

int CommonViewModel::getPointMaxValue(int axisIndex, int pointNo)
{
    if (axisIndex < 0 || axisIndex >= Enums::AxisNum) {
        return -1;
    }
    int pointIndex = pointNo - 1;
    int ret;
    QList<AreaSetPointInfo *> pointInfoList = m_pointMaxMinList.at(axisIndex);
    if ((pointIndex < 0) || (pointIndex >= pointInfoList.length())) {
        return -1;
    }
    ret = m_areaSetPointParser->getMaxValue(pointInfoList.at(pointIndex));
    if(ret < 0){
        ret = m_areaSetPointParser->getMaxValue(pointInfoList.at(221));
    }
    return ret;
}

int CommonViewModel::getPointMinValue(int axisIndex, int pointNo)
{
    if (axisIndex < 0 || axisIndex >= Enums::AxisNum) {
        return -1;
    }
    int pointIndex = pointNo - 1;
    QList<AreaSetPointInfo *> pointInfoList = m_pointMaxMinList.at(axisIndex);
    if ((pointIndex < 0) || (pointIndex >= pointInfoList.length())) {
        return -1;
    }
    return m_areaSetPointParser->getMinValue(pointInfoList.at(pointIndex));
}

QString CommonViewModel::formatPointMaxValue(int axisIndex, int pointNo)
{
    int value = getPointMaxValue(axisIndex, pointNo);
    return m_resource->formatData(value, 242, 2, true);
}

QString CommonViewModel::formatPointMinValue(int axisIndex, int pointNo)
{
    int value = getPointMinValue(axisIndex, pointNo);
    QString valueStr = m_resource->formatData(value, 242, 2, true);
    if (valueStr == "****.**") {
        valueStr = "0.00";
    }
    return valueStr;
}

void CommonViewModel::setPopupDialogStatus(const int id, const bool status)
{
    if ((id < 0) || (Enums::PopupDialogStatus::PopupDialogStatusNum <= id)) {
        return;
    }
    if (status) {
        openPopupDialog(id);
    } else {
        closePopupDialog(id);
    }
}

bool CommonViewModel::isPopupDialogOpened(const int id)
{
    return mPopupDialogOrderStack.contains(id);
}

void CommonViewModel::updateTitle(int curPage)
{
    updateTopTitleLocal((Enums::Pages)curPage, m_curLeftPage, true);
    updateScreenTitle((Enums::Pages)curPage);
}

void CommonViewModel::updateOpeCmdDisplay()
{
    emit opeCmdStatusChanged(m_opeCmdStatus);
}

void CommonViewModel::modbusSetCurrentOperateAxis(int number)
{
    quint16 axislength = m_axisOrder.length();
    QList<int> writeData;
    for (int i = 0; i < m_axisOrder.length(); i++) {
        if(i != number){
            writeData.append(0);
        }else {
            writeData.append(1);
        }
    }
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteMultipleCoils, 2200, axislength, writeData);
    if(m_communication != nullptr) m_communication->setModbusWriteCommand(command);
}

void CommonViewModel::modbusSetBuzzer(bool isonoff, int buzzerindex)
{
    //qDebug("%s:%d,modbusSetbuzzer::%d,%d",__func__,__LINE__,isonoff,buzzerindex);

    quint8 state = isonoff ? 1 : 0;
    quint8 index = buzzerindex;
    int data = (state << 8) | index;
    QList<int> writeData;
    writeData.append(data);
    stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteSingleRegister, BUZZER_INFO_HR, 2, writeData);
    if(m_communication != nullptr) m_communication->setModbusWriteCommand(command);
    modbusReadBuzzer();
}

void CommonViewModel::modbusReadBuzzer()
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, BUZZER_INFO_HR, 1);
    m_communication->setOneShotModbusCommand(command);
    connect(m_communication, &CommunicationThread::oneShotReadFinished, this, &CommonViewModel::onOneShotReadFinished, Qt::QueuedConnection);
}

void CommonViewModel::modbusSetJogratio(int index, int value)
{
    QList<int> writeData;
    writeData.append(value);
    stModBusCommunicationData command = ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteSingleRegister, 2546 + index, 1, writeData);
    if(m_communication != nullptr) m_communication->setModbusWriteCommand(command);
    modbusReadJogratioSpeed();
}

void CommonViewModel::modbusSetJoglimitspeed(int index, int value)
{
    QList<int> writeData;
    writeData.append(value);
    stModBusCommunicationData command = ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteSingleRegister, 2543 + index, 1, writeData);
    if(m_communication != nullptr) m_communication->setModbusWriteCommand(command);
    modbusReadJogratioSpeed();
}

void CommonViewModel::modbusReadJogratioSpeed()
{
    stModBusCommunicationData command = ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, 2543, 6);
    m_communication->setOneShotModbusCommand(command);
    connect(m_communication, &CommunicationThread::oneShotReadFinished, this, &CommonViewModel::onOneShotReadFinished, Qt::QueuedConnection);
}

void CommonViewModel::selectPSaxis()
{
    if( m_resource->axis_select() == 5){
        m_resource->setaxis_select(6);
        QList<int> bitWrite = {1,0};
        stModBusCommunicationData command1 = ModbusCommandGenerator::makeModbusWriteCommand(QModbusPdu::FunctionCode::WriteMultipleCoils, 1674, bitWrite.size(), bitWrite);
        m_communication->setOneShotModbusCommand(command1);
    }else{
        m_resource->setaxis_select(5);
        QList<int> bitWrite = {0,1};
        stModBusCommunicationData command1 = ModbusCommandGenerator::makeModbusWriteCommand(QModbusPdu::FunctionCode::WriteMultipleCoils, 1674, bitWrite.size(), bitWrite);
        m_communication->setOneShotModbusCommand(command1);
    }
}

void CommonViewModel::openPopupDialog(const int id)
{
    if (mPopupDialogOrderStack.contains(id)) {
        return;
    }
    mPopupDialogOrderStack.push(id);
    emit topPopupDialogChanged(topPopupDialog());
}

void CommonViewModel::closePopupDialog(const int id)
{
    if (!mPopupDialogOrderStack.contains(id)) {
        return;
    }
    mPopupDialogOrderStack.removeOne(id);
    emit topPopupDialogChanged(topPopupDialog());
}

void CommonViewModel::setManipulateMode(int manipulateMode)
{
    if (m_manipulateMode == manipulateMode)
        return;

    m_manipulateMode = manipulateMode;
    emit manipulateModeChanged(m_manipulateMode);
}

void CommonViewModel::setOperationMode(bool operationMode)
{
    if (m_operationMode == operationMode)
        return;

    m_operationMode = operationMode;
    emit operationModeChanged(m_operationMode);
}

void CommonViewModel::setAxisStatus(bool axisStatus)
{
    if (m_axisStatus == axisStatus)
        return;

    m_axisStatus = axisStatus;
    emit axisStatusChanged(m_axisStatus);
}

void CommonViewModel::setStepOpeStatus(bool stepOpeStatus)
{
    if (m_stepOpeStatus == stepOpeStatus)
        return;

    m_stepOpeStatus = stepOpeStatus;
    emit stepOpeStatusChanged(m_stepOpeStatus);
}

void CommonViewModel::setCurrentStatus(int currentStatus)
{
    if (m_currentStatus == currentStatus){
        return;
    }

    m_currentStatus = currentStatus;
    emit currentStatusChanged(m_currentStatus);
}
void CommonViewModel::setStartupStatus(int startupStatus)
{
    if (m_startupStatus == startupStatus){
        return;
    }

    m_startupStatus = startupStatus;
    emit startupStatusChanged(m_startupStatus);
}

void CommonViewModel::setOpeCmdStatus(bool opeCmdStatus)
{
    if (mIsNotifyOpeCmd == false){
        if (m_preOpeCmdCount != 0){
            m_preOpeCmdCount = 0;
            m_opeCmdStatus = false;
            emit opeCmdStatusChanged(m_opeCmdStatus);
        }
    }
    if (m_opeCmdStatus == opeCmdStatus){
        return;
    }
    m_opeCmdStatus = opeCmdStatus;
    emit opeCmdStatusChanged(m_opeCmdStatus);
}

void CommonViewModel::setDialogStatus(bool dialogStatus)
{
    m_dialogStatus = dialogStatus;
    emit dialogStatusChanged(m_dialogStatus);
}

void CommonViewModel::setPermissionTimeUpdate(bool permissionTimeUpdate)
{
    m_permissionTimeUpdate = permissionTimeUpdate;
    emit permissionTimeUpdateChanged(m_permissionTimeUpdate);
}

void CommonViewModel::setjogEnable(int jogEnable)
{
    if(m_jogEnable != jogEnable){
        m_jogEnable = jogEnable;

        emit jogEnableChanged(jogEnable);
    }
}

#if 0   // TODO
void CommonViewModel::setCurrentStatusList(QList<int> currentStatusList)
{
    if (m_currentStatusList.at(0) == currentStatusList.at(0) &&
        m_currentStatusList.at(1) == currentStatusList.at(1) &&
        m_currentStatusList.at(2) == currentStatusList.at(2)){
        return;
    }
    m_currentStatusList = currentStatusList;
    emit currentStatusListChanged(m_currentStatusList);
}
#endif

void CommonViewModel::setAxisAlarmStatus(QList<int> axisAlarmStatus)
{
    m_axisAlarmStatus = axisAlarmStatus;
    emit axisAlarmStatusChanged(m_axisAlarmStatus);
}

void CommonViewModel::setUserLevel(int userLevel)
{
    if (m_userLevel == userLevel)
        return;

    m_userLevel = userLevel;
    emit userLevelChanged(m_userLevel);
}

void CommonViewModel::setManualSpeedStatus(int manualSpeedStatus)
{
    if (m_manualSpeedStatus == manualSpeedStatus)
        return;

    m_manualSpeedStatus = manualSpeedStatus;
    emit manualSpeedStatusChanged(m_manualSpeedStatus);
}

void CommonViewModel::setAutoSpeedStatus(int autoSpeedStatus)
{
    if (m_autoSpeedStatus == autoSpeedStatus)
        return;

    m_autoSpeedStatus = autoSpeedStatus;
    emit autoSpeedStatusChanged(m_autoSpeedStatus);
}

void CommonViewModel::setOperatingMode(int operatingMode)
{
    if (m_operatingMode == operatingMode)
        return;

    m_operatingMode = operatingMode;
    emit operatingModeChanged(m_operatingMode);
}

void CommonViewModel::setEmergencyStatus(bool emergencyStatus)
{
    if (m_emergencyStatus == emergencyStatus)
        return;

    m_operationRecord->recordDispEmergencyStop(m_emergencyStatus, emergencyStatus);
    m_emergencyStatus = emergencyStatus;
    emit emergencyStatusChanged(m_emergencyStatus);
}

void CommonViewModel::setCurDate(QString curDate)
{
    m_curDate = curDate;
    emit curDateChanged(m_curDate);
}
void CommonViewModel::setCurTime(QString curTime)
{
    m_curTime = curTime;
    emit curTimeChanged(m_curTime);
}

void CommonViewModel::setCurTimeUptoMinutes(QString curTimeUptoMinutes)
{
    m_curTimeUptoMinutes = curTimeUptoMinutes;
    emit curTimeUptoMinutesChanged(m_curTimeUptoMinutes);
}

void CommonViewModel::setCurAlarmCount(int curAlarmCount)
{
    setCurAlarmCount(curAlarmCount, true);
}

void CommonViewModel::setCurAlarmCount(int curAlarmCount, bool alarmListStatusChange)
{
    if (mIsNotifyAlarm == false
        && mPreAlarmCount == curAlarmCount){
        return;
    }

    if (m_curAlarmCount != curAlarmCount) {
        m_curAlarmCount = curAlarmCount;
        emit curAlarmCountChanged(m_curAlarmCount);
        if (curAlarmCount <= 0){
            initAlarmList();
        }
    }
    mPreAlarmCount = curAlarmCount;
    updateAlarmMsgList();
    emit alarmListStatusChanged(alarmListStatusChange);
    if (m_curAlarmCount > 0){
        controlLedDevice(true, (int)CoreLibDriverManager::LedType::LedTypeRed);
    } else {
        controlLedDevice(false, (int)CoreLibDriverManager::LedType::LedTypeRed);
    }
}

void CommonViewModel::setUserPassword(QList<QString> userPassword)
{
    if (!m_communication){
        return;
    }
    if (!judgeUpdateUserPassword(userPassword)){
        return;
    }
    m_userPassword = userPassword;
    emit userPasswordChanged(m_userPassword);
    m_communication->resetCommonPageReadCommandList();
    disconnect(m_communication, &CommunicationThread::feed, this, &CommonViewModel::update);
    mIsUpdateUserPassword = true;
}

void CommonViewModel::modbusMemblemSwControl(bool isPress, uchar keyNo)
{
    bool ret = false;
    if (!mModbusCom || !m_communication){
        qDebug("%s:%d mModbusCom or m_communication is not share ret[%d]", __func__, __LINE__, ret);
        return;
    }
    qDebug("%s:%d isPress[%d] keyNo[%d]", __func__, __LINE__, isPress, keyNo);
    if (isPress){
        if (mModbusMng->getModbusComConnectStatus() != QModbusDevice::ConnectedState){
            m_communication->writeModbusDO(MEMBLEM_SW_STATUS_DO, keyNo, MEMBLEM_SW_STATUS_BIT_NUM);
        } else {
            qDebug("%s:%d isPress[%d] keyNo[%d] ModbusCom::Connect", __func__, __LINE__, isPress, keyNo);
            ret = mModbusCom->asyncModbusWriteDO(MEMBLEM_SW_STATUS_DO, keyNo, MEMBLEM_SW_STATUS_BIT_NUM);
            //qDebug("%s:%d asyncModbusWriteDO ret[%d]", __func__, __LINE__, ret);
        }
        m_operationRecord->recordKeyMembrem(keyNo);
    } else {
        if (mModbusMng->getModbusComConnectStatus() != QModbusDevice::ConnectedState){
            m_communication->writeModbusDO(MEMBLEM_SW_RELEASE_DO, 0xFF, MEMBLEM_SW_RELEASE_BIT_NUM);             // writeModbusDO()で0xFF書込み
        } else {
            qDebug("%s:%d isPress[%d] keyNo[%d] ModbusCom::Connect", __func__, __LINE__, isPress, keyNo);
            ret = mModbusCom->asyncModbusWriteDO(MEMBLEM_SW_RELEASE_DO, 0xFF, MEMBLEM_SW_RELEASE_BIT_NUM);             // writeModbusDO()で0xFF書込み
            //qDebug("%s:%d asyncModbusWriteDO ret[%d]", __func__, __LINE__, ret);
        }
    }

#ifndef REAL_TARGET
    if(isPress){
        m_currentKeyNo = keyNo;
        emit currentKeyNoChanged(m_currentKeyNo);
    }else {
        m_currentKeyNo = -1;
        emit currentKeyNoChanged(m_currentKeyNo);
    }
#endif
}

void CommonViewModel::modbusUpdateUserLevel(int userLevel)
{
    if (!m_communication){
        qDebug("%s:%d m_communication is not share", __func__, __LINE__);
        return;
    }
    QList<int> writeData;
    writeData.append(userLevel);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, USERLEVEL_TOP_HR, USERLEVEL_SIZE_NUM, writeData);
    m_communication->setModbusWriteCommand(command);
}

void CommonViewModel::modbusSpeedControl(int speed)
{
    QList<int> writeData;
    writeData.append(speed);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister, MANUAL_SPEED_STATUS_HR, 1, writeData);
    m_communication->setModbusWriteCommand(command);
}

void CommonViewModel::modbusAutoSpeedControl(int speed)
{
    QList<int> writeData;
    writeData.append(speed);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister, AUTO_SPEED_STATUS_HR, 1, writeData);
    m_communication->setModbusWriteCommand(command);
}

void CommonViewModel::modbusFreeControl(bool free)
{
    QList<int> writeData;
    int value = free ? 1 : 0;
    writeData.append(value);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleCoils, MFREE_DO, 1, writeData);
    m_communication->setModbusWriteCommand(command);
    m_freeOperate = free;
    emit freeOperateChanged(m_freeOperate);
}

void CommonViewModel::modbusJogMagnificationControl(int jogMagnificationIndex)
{
    auto phyKeyManager = m_communication->getPhyKeyManagerInstance();
    if (phyKeyManager) {
        phyKeyManager->writeJogMagnification(jogMagnificationIndex);
        m_communication->writeModbusHR(JOG_DIAL_MAGNIFICATION_HR, &jogMagnificationIndex, 1);
    }
}

QDateTime CommonViewModel::getDateTime(int dateMode)
{
    QDateTime dateTime;
    if (!m_modbus){
        return dateTime;
    }
    int year = m_modbus->getReceiveDataRG(2723);
    int month = m_modbus->getReceiveDataRG(2724);
    int day= m_modbus->getReceiveDataRG(2725);
    int hour= m_modbus->getReceiveDataRG(2726);
    int minute= m_modbus->getReceiveDataRG(2727);
    int second= m_modbus->getReceiveDataRG(2728);
    //qDebug("%s:%d year[%d] month[%d] day[%d] hour[%d] minute[%d] second[%d]",
    //    __func__, __LINE__, year, month, day, hour, minute, second);
    QString dateStr = "";
    QString timeStr = "";
    QString dateTimeStr = "";
    dateStr.append(QString::asprintf("%04d", year));
    dateStr.append("/");
    dateStr.append(QString::asprintf("%02d", month));
    dateStr.append("/");
    dateStr.append(QString::asprintf("%02d", day));
    timeStr.append(QString::asprintf("%02d", hour));
    timeStr.append(":");
    timeStr.append(QString::asprintf("%02d", minute));
    timeStr.append(":");
    timeStr.append(QString::asprintf("%02d", second));
    timeStr.append(".000");
    if (dateMode == DateModeDateTime){
        dateTimeStr = dateStr + " " + timeStr;
        dateTime = QDateTime::fromString(dateTimeStr, "yyyy/MM/dd HH:mm:ss.zzz");
    } else if (dateMode == DateModeDateOnly) {
        dateTimeStr = dateStr;
        dateTime = QDateTime::fromString(dateTimeStr, "yyyy/MM/dd");
    } else if (dateMode == DateModeTimeOnly) {
        dateTimeStr = timeStr;
        dateTime = QDateTime::fromString(dateTimeStr, "HH:mm:ss.zzz");
    } else {
        dateTimeStr = dateStr + " " + timeStr;
        dateTime = QDateTime::fromString(dateTimeStr, "yyyy/MM/dd HH:mm:ss.zzz");
    }
    return dateTime;
}

int CommonViewModel::getLanguage()
{
    if (!m_modbus){
        qDebug("%s:%d m_modbusManager is not share", __func__, __LINE__);
        return 0;
    }
    int language = m_modbus->getLanguage();
    return language;
}

void CommonViewModel::onFinished()
{
    if (!mModbusConnectFlag){
        if (m_communication) {
            if(m_communication->getModbusConnectStatus() == QModbusDevice::ConnectedState){
                mModbusConnectFlag = true;
            }
        } else {
            mModbusConnectFlag = true;
        }
        return;
    }

    judgeManipulateMode();
    updateDateTime();
    if (!mIsSettingSystemData) {
        int result = 0;
        QDateTime dateTime = getDateTime((int)DateMode::DateModeDateTime);
        QString systemTime = dateTime.toString("yyyy/MM/dd hh:mm:ss").replace("/", "-");
        result = mCoreLibSystemControl.setSystemDateTime(systemTime);
        if (result < 0){
            qDebug("%s:%d setSystemDateTime failed[%d]", __func__, __LINE__, result);
        } else {
            mIsSettingSystemData = true;
        }
    }
    updateAxisCurrentPosition();
    updateAxisStatus();
    updateStepOpeStatus();
    updateEmergencyStopStatus();
    updateCurrentStatus();
    updateAxisAlarmStatus();
    updateOpeCmdStatus();
    updateUserLevel();
    updateManualOpeSpeedStatus();
    updateAutoOpeSpeedStatus();
    updateOperatingStatus();
    updateCurFileInfo();
	updateFileChange();
    modbusUpdateCurrentOperateAxis();
#ifdef REAL_TARGET
    if (m_isFirstReadCommonModbus) {
        if (!mIsAlreadyCommonModbus){
            updateLanguageInfo();
            mIsAlreadyCommonModbus = true;
        }
    }
#else
    updateLanguageInfo();
#endif // REAL_TARGET
}

void CommonViewModel::onOneShotReadFinished(stModBusCommunicationData command)
{
    if (command.readAddress == COMMON_SYSTEM_TIME_ADDR_LIST.at(0).value(0)
        && command.comSize == COMMON_SYSTEM_TIME_ADDR_LIST.at(0).count()){
        updateSystemTime(command);
        goto oneshot_disconnect;
    } else if ((command.readAddress >= USER_PASSWORD_TOP_HR && command.readAddress <= USER_PASSWORD_TOP_HR + 4)
        && command.comSize == 1){
        updateUserPasswordIndividual(command);
        goto oneshot_disconnect;
    }else if(command.readAddress == BUZZER_INFO_HR){
        updateBuzzerInfo(command);
        goto oneshot_disconnect;
    }else if(command.readAddress >= 2543 && command.readAddress <= 2548){
         updateJogRatioSpeedInfo(command);
        goto oneshot_disconnect;
    }
    return;
oneshot_disconnect:
    disconnect(m_communication, &CommunicationThread::oneShotReadFinished,
            this, &CommonViewModel::onOneShotReadFinished);
}

void CommonViewModel::update(stModBusCommunicationData command)
{
    if(command.readAddress >= USER_PASSWORD_TOP_HR && command.readAddress <= USER_PASSWORD_TOP_HR + 4){
        updateUserPassword(command);
    }
}

void CommonViewModel::onUpdateSelectSwInfo(PhyKeyManager::SelectSwitchStatus selectSwStatus)
{
//    qDebug("%s:%d selectSwStatus[%d]", __func__, __LINE__, selectSwStatus);
    switch(selectSwStatus){
    case PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusAuto:
    case PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusManual:
    case PhyKeyManager::SelectSwitchStatus::SelectSwitchStatusOrigin:
        writeManipulateMode((int)selectSwStatus);
        break;
    default:
        break;
    }
}

void CommonViewModel::onUpdateMemblemSwInfo(PhyKeyManager::MemblemSwInfo memblemSwInfo)
{
    if (!m_bootCompleted) {
        qDebug("%s:%d m_bootCompleted[%d]", __func__, __LINE__, m_bootCompleted);
        return;
    } else {
        if (memblemSwInfo.key01 || memblemSwInfo.key18){
            //qDebug("%s:%d m_bootCompleted[%d] is true, key01[%d] or key18[%d] is on", __func__, __LINE__, m_bootCompleted, memblemSwInfo.key01, memblemSwInfo.key18);
        }
    }
    if (memblemSwInfo.key01) { modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey1); }
    if (memblemSwInfo.key02) { modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey2); }
    if (memblemSwInfo.key03) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey3);
        modbusSetCurrentOperateAxis(0);
    }
    if (memblemSwInfo.key04) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey4);
        modbusSetCurrentOperateAxis(0);
    }
    if (memblemSwInfo.key05) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey5);
        modbusSetCurrentOperateAxis(1);
    }
    if (memblemSwInfo.key06) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey6);
        modbusSetCurrentOperateAxis(2);
    }
    if (memblemSwInfo.key07) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey7);
        modbusSetCurrentOperateAxis(2);
    }
    if (memblemSwInfo.key08) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey8);
        modbusSetCurrentOperateAxis(1);
    }
    if (memblemSwInfo.key09) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey9);
        modbusSetCurrentOperateAxis(3);
    }
    if (memblemSwInfo.key10) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey10);
        modbusSetCurrentOperateAxis(4);
    }
    if (memblemSwInfo.key11) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey11);
        modbusSetCurrentOperateAxis(4);
    }
    if (memblemSwInfo.key12) {
        modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey12);
        modbusSetCurrentOperateAxis(3);
    }
    if (memblemSwInfo.key13) { modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey13);}
    if (memblemSwInfo.key14) { modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey14);}
    if (memblemSwInfo.key15) { modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey15);}
    if (memblemSwInfo.key16) { modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey16);}
    if (memblemSwInfo.key17) { modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey17);}
    if (memblemSwInfo.key18) { modbusMemblemSwControl(true, Enums::MemblemKeyNo::MemblemKeyNoKey18);}
}

void CommonViewModel::onUpdateReleaseMemblemSwInfo(int memblemSwReleaseInfo)
{
    if (!m_bootCompleted) {
        qDebug("%s:%d m_bootCompleted[%d]", __func__, __LINE__, m_bootCompleted);
        return;
    }
    if ((memblemSwReleaseInfo & 0x00000001) == 0x00000001) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey1); }
    if ((memblemSwReleaseInfo & 0x00000002) == 0x00000002) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey2); }
    if ((memblemSwReleaseInfo & 0x00000004) == 0x00000004) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey3); }
    if ((memblemSwReleaseInfo & 0x00000008) == 0x00000008) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey4); }
    if ((memblemSwReleaseInfo & 0x00000010) == 0x00000010) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey5); }
    if ((memblemSwReleaseInfo & 0x00000020) == 0x00000020) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey6); }
    if ((memblemSwReleaseInfo & 0x00000040) == 0x00000040) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey7); }
    if ((memblemSwReleaseInfo & 0x00000080) == 0x00000080) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey8); }
    if ((memblemSwReleaseInfo & 0x00000100) == 0x00000100) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey9); }
    if ((memblemSwReleaseInfo & 0x00000200) == 0x00000200) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey10); }
    if ((memblemSwReleaseInfo & 0x00000400) == 0x00000400) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey11); }
    if ((memblemSwReleaseInfo & 0x00000800) == 0x00000800) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey12); }
    if ((memblemSwReleaseInfo & 0x00001000) == 0x00001000) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey13); }
    if ((memblemSwReleaseInfo & 0x00002000) == 0x00002000) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey14); }
    if ((memblemSwReleaseInfo & 0x00004000) == 0x00004000) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey15); }
    if ((memblemSwReleaseInfo & 0x00008000) == 0x00008000) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey16); }
    if ((memblemSwReleaseInfo & 0x00010000) == 0x00010000) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey17); }
    if ((memblemSwReleaseInfo & 0x00020000) == 0x00020000) { modbusMemblemSwControl(false, Enums::MemblemKeyNo::MemblemKeyNoKey18); }
}
void CommonViewModel::notifyAlarm(const AlarmInfo &alarmInfo)
{
#ifdef REAL_TARGET
    QString curDateTimeStr = curDate() + " " + curTime();
    QDateTime curDateTime = QDateTime::fromString(curDateTimeStr, "yyyy/MM/dd HH:mm:ss");
#else // REAL_TARGET
    QDateTime curDateTime = QDateTime::currentDateTime();
#endif // REAL_TARGET
    //qDebug() << __func__ << "type:" << alarmInfo.type() << "dateTime:" << curDateTime.toString("yyyy/MM/dd hh:mm:ss");
    if (!curDateTime.isValid()) {
        qWarning() << __func__ << "Invalid date:" << curDateTime.toString("yyyy/MM/dd hh:mm:ss") << alarmInfo.type();
        return;
    }
    emit alarmOccurred(alarmInfo, curDateTime);
}

void CommonViewModel::onIsFirstReadCommonModbusChanged(bool isFirstReadCommonModbus)
{
    m_isFirstReadCommonModbus = isFirstReadCommonModbus;
}
void CommonViewModel::onlinkAlarmChanged(bool isonoff)
{
    if(isonoff){
        int msgNo = 20;
        AlarmInfo *alarmInfo = new  AlarmInfo();
        alarmInfo->setType(AlarmInfo::Type::System);
        alarmInfo->setMsgNumber(msgNo);
        alarmInfo->setAlarmNumber((msgNo < 100 ? (msgNo < 10 ? "00" + QString::number((int)msgNo)
                                                             : "0" + QString::number((int)msgNo)) : QString::number((int)msgNo)));
        alarmInfo->setAlarmDetailsColor(m_resource->rgb(12, SYSTEM_ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2)));
        alarmInfo->setAlarmIndex(SYSTEM_ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2));
        if (mInternalAlarmCount == 0){
            delete m_alarmList.takeFirst();
            m_alarmList.insert(0, alarmInfo);
        } else {
            auto index = mIsAlarmOccurred ? 1 : 0;
            if (isAlarmExist(AlarmInfo::Type::System)) {
                delete m_alarmList.takeAt(index);
                m_alarmList.insert(index, alarmInfo);
            } else {
                delete m_alarmList.takeLast();
                m_alarmList.insert(index, alarmInfo);
            }
        }
        notifyAlarm(*alarmInfo);
        m_operationRecord->recordDispSystemAlarm(msgNo);
    }else {
        QTimer::singleShot(500, this, &CommonViewModel::onUpdateSingleTimer);
    }
}

void CommonViewModel::onUpdateSingleTimer()
{   int msgNo = 21;
    AlarmInfo *alarmInfo = new  AlarmInfo();
    alarmInfo->setType(AlarmInfo::Type::System);
    alarmInfo->setMsgNumber(msgNo);
    alarmInfo->setAlarmNumber((msgNo < 100 ? (msgNo < 10 ? "00" + QString::number((int)msgNo)
                                                         : "0" + QString::number((int)msgNo)) : QString::number((int)msgNo)));
    alarmInfo->setAlarmDetailsColor(m_resource->rgb(12, SYSTEM_ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2)));
    alarmInfo->setAlarmIndex(SYSTEM_ALARM_MSG_TYPE_START_INDEX + (int)(msgNo * 2));
    if (mInternalAlarmCount == 0){
        delete m_alarmList.takeFirst();
        m_alarmList.insert(0, alarmInfo);
    } else {
        auto index = mIsAlarmOccurred ? 1 : 0;
        if (isAlarmExist(AlarmInfo::Type::System)) {
            delete m_alarmList.takeAt(index);
            m_alarmList.insert(index, alarmInfo);
        } else {
            delete m_alarmList.takeLast();
            m_alarmList.insert(index, alarmInfo);
        }
    }
    notifyAlarm(*alarmInfo);
    m_operationRecord->recordDispSystemAlarm(msgNo);
}
