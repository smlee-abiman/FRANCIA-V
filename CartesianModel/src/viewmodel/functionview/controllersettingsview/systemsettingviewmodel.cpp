#include "systemsettingviewmodel.h"

#include <bitset>

#define PENSET_GROUP_STR    ("17")
#define SCREEN_BRIGHT_MIN   (1)
#define SCREEN_BRIGHT_MAX   (8)
#define SCREEN_OFF_TIME_MIN (1)
#define SCREEN_OFF_TIME_MAX (99)

#define mConvertIndexToHRBitOffset(index) (((index % 16) / 8) ? (index % 8) : ((index % 8) + 8))

//------------------------------------------------------
// SystemSettingViewModel.
//------------------------------------------------------
SystemSettingViewModel::SystemSettingViewModel(ResourceManager *resource, CommunicationThread *comm)
    : ViewModelBase(resource, comm)
    , m_application(PendantApplication::getInstance())
    , m_isReadyDisplayControl(false)
    , mIsModbusConnectOneShot(false)
    , m_numOfRemainingInitialModbusReads(2) // screen bright, off time.
    , m_screenBright(3)
    , m_screenOffTime(10)
    , m_systemProgram(0)
    , m_takeUpType(0)
    , m_machineName("")
    , m_acceptanceRegistrationCode(0)
    , m_acceptanceRemainingTime(0)
    , m_systemMode(0)
    , m_screenOffTimeFormatNo(0)
    , m_machineSetupDatePressAndHoldInterval_msec(0)
    , m_editedScreenBright(false)
    , m_editedScreenOffTime(false)
    , m_editedSystemProgram(false)
    , m_editedTakeUpType(false)
    , m_editedMachineName(false)
    , m_editedMachineSetupDate(false)
    , m_editedAcceptanceRegistrationCode(false)
    , m_editedSystemModeTbl({false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false})
{
    init();
    startModbusOneShot();
    modbusGetScreenBright();
    modbusGetScreenOffTime();
}
//------------------------------------------------------
SystemSettingViewModel::~SystemSettingViewModel()
{
    disconnectToOperationNotification();
    closeDisplayControl();
    m_editedSystemModeTbl.clear();
}
//------------------------------------------------------
QList<bool> SystemSettingViewModel::systemModeTbl() const
{
    std::bitset<16> bits(m_systemMode);
    QVector<bool> systemModeTbl;
    systemModeTbl.resize(bits.size());
    for (std::size_t index = 0; index < bits.size(); index++) {
        auto bitOffset = mConvertIndexToHRBitOffset(index);
        systemModeTbl[index] = bits[bitOffset];
    }
    return systemModeTbl.toList();
}
//------------------------------------------------------
void SystemSettingViewModel::editScreenBright(const int screenBright)
{
    if (m_screenBright == screenBright) {
        return;
    }
    if (!validScreenBright(static_cast<quint16>(screenBright))) {
        qWarning("edit value:%d is out range. range:[%d-%d]", screenBright, SCREEN_BRIGHT_MIN, SCREEN_BRIGHT_MAX);
        return;
    }
    QList<int> writeData;
    writeData << screenBright;
    writeModbusHR(SCREEN_BRIGHT_HR, writeData);
    m_operationRecord->recordSetScreenBright(m_screenBright, screenBright);
    setLcdBackLightBrightness(screenBright);
    modbusGetScreenBright();
    setEditedScreenBright(true);
}
//------------------------------------------------------
void SystemSettingViewModel::editScreenOffTime(const int screenOffTime)
{
    if (m_screenOffTime == screenOffTime) {
        return;
    }
    if (!validScreenOffTime(static_cast<quint16>(screenOffTime))) {
        qWarning("edit value:%d is out range. range:[%d-%d]", screenOffTime, SCREEN_OFF_TIME_MIN, SCREEN_OFF_TIME_MAX);
        return;
    }
    QList<int> writeData;
    writeData << screenOffTime;
    writeModbusHR(SCREEN_OFF_TIME_HR, writeData);
    m_operationRecord->recordSetScreenOffTime(m_screenOffTime, screenOffTime);
    modbusGetScreenOffTime();
    setEditedScreenOffTime(true);
}
//------------------------------------------------------
void SystemSettingViewModel::editSystemProgram(const int systemProgram)
{
    if (m_systemProgram == systemProgram) {
        return;
    }
    if (!validSystemProgram(static_cast<quint16>(systemProgram))) {
        qWarning("%s(%d): unknown value:%d", __PRETTY_FUNCTION__, __LINE__, systemProgram);
        return;
    }
    QList<int> writeData;
    writeData << systemProgram;
    writeModbusHR(SYSTEM_PROGRAM_HR, writeData);
    m_operationRecord->recordSetSystemProgram(m_systemProgram, systemProgram);
    modbusGetSystemProgram();
    setEditedSystemProgram(true);
}
//------------------------------------------------------
void SystemSettingViewModel::editTakeUpType(const int takeUpType)
{
    if (m_takeUpType == takeUpType) {
        return;
    }
    if (!validTakeUpType(static_cast<quint16>(takeUpType))) {
        qWarning("%s(%d): unknown value:%d", __PRETTY_FUNCTION__, __LINE__, takeUpType);
        return;
    }
    QList<int> writeData;
    writeData << takeUpType;
    writeModbusHR(TAKE_UP_TYPE_HR, writeData);
    m_operationRecord->recordSetTakeUpType(m_takeUpType, takeUpType);
    modbusGetTakeUpType();
    setEditedTakeUpType(true);
}
//------------------------------------------------------
void SystemSettingViewModel::editMachineName(const QString &machineName)
{
    if (this->machineName() == machineName) {
        return;
    }
    if (MACHINE_NAME_LENGTH < machineName.size()) {
        qWarning("length:%d of the machine name has been exceeded.", machineName.size());
        return;
    }
    QVector<int> writeData;
    for (int i = 0; i < machineName.size(); ++i) {
        const char ch1 = machineName.at(i).toLatin1();
        const char ch2 = (i == (machineName.size() - 1)) ? 0 : machineName.at(++i).toLatin1();
        const auto value = static_cast<quint16>((ch1 << 8) | ch2);
        writeData.append(value);
    }
    writeData.resize(MACHINE_NAME_SIZE);
    writeModbusHR(MACHINE_NAME_HR, writeData.toList());
    modbusGetMachineName();
    setEditedMachineName(true);
}
//------------------------------------------------------
void SystemSettingViewModel::resetMachineSetupDateToCurrentDate()
{
    auto currentDate = QDate::currentDate();
    if (m_machineSetupDate.date() == currentDate) {
        return;
    }
    QList<int> writeData;
    writeData << currentDate.year() % 100;
    writeData << currentDate.month();
    writeData << currentDate.day();
    writeData << 0; // hour.
    writeData << 0; // minute.
    writeData << 0; // second.
    writeModbusHR(MACHINE_SETUP_DATE_HR, writeData);
    modbusGetMachineSetupDate();
    setEditedMachineSetupDate(true);
}
//------------------------------------------------------
void SystemSettingViewModel::editAcceptanceRegistrationCode(const int acceptanceRegistrationCode)
{
    if (m_acceptanceRegistrationCode == acceptanceRegistrationCode) {
        return;
    }
    QList<int> writeData;
    writeData << acceptanceRegistrationCode;
    writeModbusHR(ACCEPTANCE_REGISTRATIONCODE_HR, writeData);
    modbusGetAcceptanceRegistrationCode();
    setEditedAcceptanceRegistrationCode(true);
}
//------------------------------------------------------
void SystemSettingViewModel::editSystemMode(int index, bool systemMode)
{
    if (16 <= index) {
        qWarning("index:%d is out range. range:0-15.", index);
    }
    auto bitOffset = mConvertIndexToHRBitOffset(index);
    std::bitset<16> bits(m_systemMode);
    bool currentSystemMode = bits[bitOffset];
    if (currentSystemMode == systemMode) {
        return;
    }
    bits[bitOffset] = systemMode;
    QList<int> writeData;
    writeData << static_cast<quint16>(bits.to_ulong());
    writeModbusHR(SYSTEM_MODE_HR, writeData);
    m_operationRecord->recordSetSystemMode(index, currentSystemMode, systemMode);
    modbusGetSystemMode();
    setEditedSystemMode(index, true);
}
//------------------------------------------------------
void SystemSettingViewModel::saveSystemSettings()
{
    QList<int> writeData({2});
    writeModbusHR(SAVE_SYSTEM_SETTING_HR, writeData);
    resetAllEdited();
}
//------------------------------------------------------
void SystemSettingViewModel::display()
{
    qDebug("[SystemSettingViewModel]");
    qDebug("  - m_screenOffTimeFormatNo:%d", m_screenOffTimeFormatNo);
    qDebug("  - m_machineSetupDatePressAndHoldInterval_msec:%d", m_machineSetupDatePressAndHoldInterval_msec);
    {
        qDebug("  - m_systemModeDisplay");
        auto size = m_systemModeDisplay.size();
        for (int i = 0; i < size; i++) {
            qDebug("    [%d]:%s", i, (m_systemModeDisplay[i]?"true":"false"));
        }
    }
}
//------------------------------------------------------
bool SystemSettingViewModel::validScreenBright(const quint16 &screenBright)
{
    if ((screenBright < SCREEN_BRIGHT_MIN) ||
        (SCREEN_BRIGHT_MAX < screenBright)) {
        return false;
    }
    return true;
}
//----------------------------------------
void SystemSettingViewModel::setScreenBright(const quint16 &screenBright)
{
    if (m_screenBright == screenBright) {
        return;
    }
    if (!validScreenBright(screenBright)) {
        return;
    }
    m_screenBright = screenBright;
    setLcdBackLightBrightness(screenBright);
    emit screenBrightChanged(screenBright);
}
//------------------------------------------------------
bool SystemSettingViewModel::validScreenOffTime(const quint16 &screenOffTime)
{
    if ((screenOffTime < SCREEN_OFF_TIME_MIN) ||
        (SCREEN_OFF_TIME_MAX < screenOffTime)) {
        return false;
    }
    return true;
}
//----------------------------------------
void SystemSettingViewModel::setScreenOffTime(const quint16 &screenOffTime)
{
    if (m_screenOffTime == screenOffTime) {
        return;
    }
    if (!validScreenOffTime(screenOffTime)) {
        return;
    }
    m_screenOffTime = screenOffTime;
    startOperationMonitoring(screenOffTime);
    emit screenOffTimeChanged(screenOffTime);
}
//------------------------------------------------------
bool SystemSettingViewModel::validSystemProgram(const quint16 &systemProgram)
{
    switch (systemProgram) {
    case 0:
    case 1:
        return true;
    }
    return false;
}
//----------------------------------------
void SystemSettingViewModel::setSystemProgram(const quint16 &systemProgram)
{
    if (m_systemProgram == systemProgram) {
        return;
    }
    if (!validSystemProgram(systemProgram)) {
        qDebug("unknown value:%d", systemProgram);
        return;
    }
    m_systemProgram = systemProgram;
    emit systemProgramChanged(systemProgram);
}
//------------------------------------------------------
bool SystemSettingViewModel::validTakeUpType(const quint16 &takeUpType)
{
    switch (takeUpType) {
    case 0:
    case 1:
    case 2:
        return true;
    }
    return false;
}
//----------------------------------------
void SystemSettingViewModel::setTakeUpType(const quint16 &takeUpType)
{
    if (m_takeUpType == takeUpType) {
        return;
    }
    if (!validTakeUpType(takeUpType)) {
        qDebug("unknown value:%d", takeUpType);
        return;
    }
    m_takeUpType = takeUpType;
    emit takeUpTypeChanged(takeUpType);
}
//------------------------------------------------------
void SystemSettingViewModel::setMachineName(const QByteArray &machineName)
{
    if (m_machineName == machineName) {
        return;
    }
    m_machineName = machineName;
    emit machineNameChanged(QString(machineName));
}
//------------------------------------------------------
void SystemSettingViewModel::setMachineSetupDate(const QDateTime &machineSetupDate)
{
    if (m_machineSetupDate == machineSetupDate) {
        return;
    }
    m_machineSetupDate = machineSetupDate;
    emit machineSetupDateChanged(machineSetupDate);
}
//------------------------------------------------------
void SystemSettingViewModel::setAcceptanceRegistrationCode(const quint16 &acceptanceRegistrationCode)
{
    if (m_acceptanceRegistrationCode == acceptanceRegistrationCode) {
        return;
    }
    m_acceptanceRegistrationCode = acceptanceRegistrationCode;
    emit acceptanceRegistrationCodeChanged(acceptanceRegistrationCode);
}
//------------------------------------------------------
void SystemSettingViewModel::setAcceptanceRemainingTime(const quint16 &acceptanceRemainingTime)
{
    if (m_acceptanceRemainingTime == acceptanceRemainingTime) {
        return;
    }
    m_acceptanceRemainingTime = acceptanceRemainingTime;
    emit acceptanceRemainingTimeChanged(acceptanceRemainingTime);
}
//------------------------------------------------------
void SystemSettingViewModel::setSystemMode(quint16 systemMode)
{
    if (m_systemMode == systemMode) {
        return;
    }
    m_systemMode = systemMode;
    emit systemModeTblChanged(systemModeTbl());
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedScreenBright(bool editedScreenBright)
{
    if (m_editedScreenBright == editedScreenBright) { return; }
    m_editedScreenBright = editedScreenBright;
    emit editedScreenBrightChanged(editedScreenBright);
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedScreenOffTime(bool editedScreenOffTime)
{
    if (m_editedScreenOffTime == editedScreenOffTime) { return; }
    m_editedScreenOffTime = editedScreenOffTime;
    emit editedScreenOffTimeChanged(editedScreenOffTime);
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedSystemProgram(bool editedSystemProgram)
{
    if (m_editedSystemProgram == editedSystemProgram) { return; }
    m_editedSystemProgram = editedSystemProgram;
    emit editedSystemProgramChanged(editedSystemProgram);
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedTakeUpType(bool editedTakeUpType)
{
    if (m_editedTakeUpType == editedTakeUpType) { return; }
    m_editedTakeUpType = editedTakeUpType;
    emit editedTakeUpTypeChanged(editedTakeUpType);
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedMachineName(bool editedMachineName)
{
    if (m_editedMachineName == editedMachineName) { return; }
    m_editedMachineName = editedMachineName;
    emit editedMachineNameChanged(editedMachineName);
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedMachineSetupDate(bool editedMachineSetupDate)
{
    if (m_editedMachineSetupDate == editedMachineSetupDate) { return; }
    m_editedMachineSetupDate = editedMachineSetupDate;
    emit editedMachineSetupDateChanged(editedMachineSetupDate);
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedAcceptanceRegistrationCode(bool editedAcceptanceRegistrationCode)
{
    if (m_editedAcceptanceRegistrationCode == editedAcceptanceRegistrationCode) { return; }
    m_editedAcceptanceRegistrationCode = editedAcceptanceRegistrationCode;
    emit editedAcceptanceRegistrationCodeChanged(editedAcceptanceRegistrationCode);
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedSystemMode(int index, bool editedSystemMode)
{
    if (index < 0 || m_editedSystemModeTbl.size() <= index) { return; }
    if (m_editedSystemModeTbl[index] == editedSystemMode) { return; }
    m_editedSystemModeTbl[index] = editedSystemMode;
    emit editedSystemModeTblChanged(m_editedSystemModeTbl);
}
//------------------------------------------------------
void SystemSettingViewModel::setEditedSystemModeAll(bool editedSystemMode)
{
    for (auto& value : m_editedSystemModeTbl) {
        value = editedSystemMode;
    }
    emit editedSystemModeTblChanged(m_editedSystemModeTbl);
}
//------------------------------------------------------
void SystemSettingViewModel::resetAllEdited()
{
    setEditedScreenBright(false);
    setEditedScreenOffTime(false);
    setEditedSystemProgram(false);
    setEditedTakeUpType(false);
    setEditedMachineName(false);
    setEditedMachineSetupDate(false);
    setEditedAcceptanceRegistrationCode(false);
    setEditedSystemModeAll(false);
}
//------------------------------------------------------
void SystemSettingViewModel::onOperationStarted()
{
    setLcdBackLightBrightness(m_screenBright);
}
//------------------------------------------------------
void SystemSettingViewModel::onOperationStopped()
{
    setLcdBackLightBrightness(0);
}
//------------------------------------------------------
void SystemSettingViewModel::onFinished()
{
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}
//------------------------------------------------------
void SystemSettingViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (isViewActive()) {
        if (oneShotReadCaller != CommunicationThread::CallerFunctionControllerSettingSystemSetPageModel) {
            qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
            return;
        }
    }
    if (command.comSize == 0) {
        qDebug("%s:%d comSize[%d]", __func__, __LINE__, command.comSize);
        return;
    }
    // qDebug("addr:%d, comSize:%d", command.readAddress, command.comSize); // for debug.
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        switch (command.readAddress) {
        case SCREEN_BRIGHT_HR:
            updateScreenBright(command);
            decrementNumOfRemainingInitialModbusReads();
            break;
        case SCREEN_OFF_TIME_HR:
            updateScreenOffTime(command);
            decrementNumOfRemainingInitialModbusReads();
            break;
        case SYSTEM_PROGRAM_HR:
            updateSystemProgram(command);
            break;
        case TAKE_UP_TYPE_HR:
            updateTakeUpType(command);
            break;
        case MACHINE_NAME_HR:
            updateMachineName(command);
            break;
        case MACHINE_SETUP_DATE_HR:
            updateMachineSetupDate(command);
            break;
        case ACCEPTANCE_REGISTRATIONCODE_HR:
            updateAcceptanceRegistrationCode(command);
            break;
        case SYSTEM_MODE_HR:
            updateSystemMode(command);
            break;
        default:
            // qDebug("unknown addr:%d", command.readAddress);  // for debug.
            return;
        }
    } else if (command.funcCode == QModbusPdu::FunctionCode::ReadInputRegisters) {
        switch (command.readAddress) {
        case ACCEPTANCE_REMAINING_TIME_SR:
            updateAcceptanceRemainingTime(command);
            break;
        default:
            // qDebug("unknown addr:%d", command.readAddress);  // for debug.
            return;
        }
    }
    if (!isViewActive() && (m_numOfRemainingInitialModbusReads <= 0)) {
        stopModbusOneShot();
    }
}
//------------------------------------------------------
void SystemSettingViewModel::onActivate()
{
    startModbusPeriodic();
    startModbusOneShot();
    modbusGetScreenBright();
    modbusGetScreenOffTime();
    modbusGetSystemProgram();
    modbusGetTakeUpType();
    modbusGetMachineName();
    modbusGetMachineSetupDate();
    modbusGetAcceptanceRegistrationCode();
    modbusGetAcceptanceRemainingTime();
    modbusGetSystemMode();
}
//------------------------------------------------------
void SystemSettingViewModel::onDeactivate()
{
    stopModbusOneShot();
    stopModbusPeriodic();
}
//------------------------------------------------------
void SystemSettingViewModel::init()
{
    openDisplayControl();
    readPenSetData();
    readPenSetSystemModeDisplay();
    connectToOperationNotification();
    startOperationMonitoring(m_screenOffTime);
    // display();  // todo: debug.
}
//------------------------------------------------------
void SystemSettingViewModel::connectToOperationNotification()
{
    if (!m_application) {
        qWarning("couldn't connect, application instance is nullptr.");
        return;
    }
    connect(m_application, &PendantApplication::operationStarted, this, &SystemSettingViewModel::onOperationStarted, Qt::QueuedConnection);
    connect(m_application, &PendantApplication::operationStopped, this, &SystemSettingViewModel::onOperationStopped, Qt::QueuedConnection);
}
//------------------------------------------------------
void SystemSettingViewModel::disconnectToOperationNotification()
{
    if (!m_application) {
        qDebug("couldn't disconnect, application instance is nullptr.");
        return;
    }
    disconnect(m_application, &PendantApplication::operationStarted, this, &SystemSettingViewModel::onOperationStarted);
    disconnect(m_application, &PendantApplication::operationStopped, this, &SystemSettingViewModel::onOperationStopped);
}
//------------------------------------------------------
void SystemSettingViewModel::startOperationMonitoring(const int screenOffTime)
{
    if (!m_application) {
        qWarning("couldn't set opration moniterting timeout, application instance is nullptr.");
        return;
    }
    m_application->startOperationMonitoring(screenOffTime * 60 * 1000);
}
//------------------------------------------------------
void SystemSettingViewModel::decrementNumOfRemainingInitialModbusReads()
{
    if (m_numOfRemainingInitialModbusReads <= 0) { return; }
    m_numOfRemainingInitialModbusReads--;
}
//------------------------------------------------------
void SystemSettingViewModel::openDisplayControl()
{
    if (m_isReadyDisplayControl) {
        return;
    }
    m_isReadyDisplayControl = m_displayControl.openDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeDisplayBrightness);
    if (!m_isReadyDisplayControl) {
        qWarning("display control open failed.");
        return;
    }
}
//------------------------------------------------------
void SystemSettingViewModel::closeDisplayControl()
{
    if (!m_isReadyDisplayControl) {
        return;
    }
    m_displayControl.closeDevice(CoreLibDriverManager::DriverControlType::DriverControlTypeDisplayBrightness);
}
//------------------------------------------------------
void SystemSettingViewModel::setLcdBackLightBrightness(const int screenBright)
{
    if (!m_isReadyDisplayControl) {
        qWarning("lcd backlight brightness control failed (set bright:%d). because the display control has not been opened.", screenBright);
        return;
    }
    m_displayControl.setLcdBackLightBrightness(QString::number(screenBright));
}
//------------------------------------------------------
void SystemSettingViewModel::readPenSetData()
{
    m_screenOffTimeFormatNo = m_resource->getPenSetIntValue(PENSET_GROUP_STR, 6, 0);
    m_machineSetupDatePressAndHoldInterval_msec = m_resource->getPenSetIntValue(PENSET_GROUP_STR, 2, 0) * 1000;
}
//------------------------------------------------------
void SystemSettingViewModel::readPenSetSystemModeDisplay()
{
    int row = 55;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 2; j++){
            int data = m_resource->getPenSetIntValue(PENSET_GROUP_STR, row,j);
            auto visible = (data == 255);
            m_systemModeDisplay.append(visible);
        }
        row++;
    }
}
//------------------------------------------------------
void SystemSettingViewModel::startModbusPeriodic()
{
    if (!m_communication) return;
    connect(m_communication, &CommunicationThread::finish, this, &SystemSettingViewModel::onFinished, Qt::QueuedConnection);
}
//------------------------------------------------------
void SystemSettingViewModel::stopModbusPeriodic()
{
    if (!m_communication) return;
    disconnect(m_communication, &CommunicationThread::finish, this, &SystemSettingViewModel::onFinished);
}
//------------------------------------------------------
void SystemSettingViewModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (mIsModbusConnectOneShot) return;
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &SystemSettingViewModel::onOneShotReadFinished, Qt::QueuedConnection);
    mIsModbusConnectOneShot = true;
}
//------------------------------------------------------
void SystemSettingViewModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (!mIsModbusConnectOneShot) return;
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
        this, &SystemSettingViewModel::onOneShotReadFinished);
    mIsModbusConnectOneShot = false;
}
//------------------------------------------------------
void SystemSettingViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    if (!m_communication) return;
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerFunctionControllerSettingSystemSetPageModel);
    m_communication->setOneShotModbusCommand(data);
}
//------------------------------------------------------
void SystemSettingViewModel::writeModbusHR(quint16 hrAddress, const QList<int> &writeData)
{
    qDebug("called writeModbugHR hrAddress:%d range:%d.", hrAddress, writeData.size());
    // for (auto& data : writeData) { qDebug(" - [%5d](0x%04x)", data, data); }
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, writeData.size(), writeData);
    m_communication->setModbusWriteCommand(command);
}
//------------------------------------------------------
void SystemSettingViewModel::readModbus(const QModbusPdu::FunctionCode funcCode, const quint16 &address, const quint16 &range)
{
    stModBusCommunicationData command = ModbusCommandGenerator::makeModbusReadCommand(funcCode, address, range);
    setOneShotModbusCommand(command);
}
//------------------------------------------------------
void SystemSettingViewModel::readModbusHR(const quint16 &hrAddress, const quint16 &range)
{
    qDebug("called readModbusHR hrAddress:%d range:%d.", hrAddress, range);
    readModbus(QModbusPdu::FunctionCode::ReadHoldingRegisters, hrAddress, range);
}
//------------------------------------------------------
void SystemSettingViewModel::readModbusSR(const quint16 &srAddress, const quint16 &range)
{
    qDebug("called readModbusSR srAddress:%d range:%d.", srAddress, range);
    readModbus(QModbusPdu::FunctionCode::ReadInputRegisters, srAddress, range);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetScreenBright()
{
    readModbusHR(SCREEN_BRIGHT_HR, 1);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetScreenOffTime()
{
    readModbusHR(SCREEN_OFF_TIME_HR, 1);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetSystemProgram()
{
    readModbusHR(SYSTEM_PROGRAM_HR, 1);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetTakeUpType()
{
    readModbusHR(TAKE_UP_TYPE_HR, 1);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetMachineName()
{
    readModbusHR(MACHINE_NAME_HR, MACHINE_NAME_SIZE);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetMachineSetupDate()
{
    readModbusHR(MACHINE_SETUP_DATE_HR, MACHINE_SETUP_DATE_SIZE);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetAcceptanceRegistrationCode()
{
    readModbusHR(ACCEPTANCE_REGISTRATIONCODE_HR, 1);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetAcceptanceRemainingTime()
{
    readModbusSR(ACCEPTANCE_REMAINING_TIME_SR, 1);
}
//------------------------------------------------------
void SystemSettingViewModel::modbusGetSystemMode()
{
    readModbusHR(SYSTEM_MODE_HR, 1);
}
//------------------------------------------------------
void SystemSettingViewModel::updateScreenBright(const stModBusCommunicationData &command)
{
    setScreenBright(m_modbus->getReceiveDataHR(command.readAddress));
}
//------------------------------------------------------
void SystemSettingViewModel::updateScreenOffTime(const stModBusCommunicationData &command)
{
    setScreenOffTime(m_modbus->getReceiveDataHR(command.readAddress));
}
//------------------------------------------------------
void SystemSettingViewModel::updateSystemProgram(const stModBusCommunicationData &command)
{
    setSystemProgram(m_modbus->getReceiveDataHR(command.readAddress));
}
//------------------------------------------------------
void SystemSettingViewModel::updateTakeUpType(const stModBusCommunicationData &command)
{
    setTakeUpType(m_modbus->getReceiveDataHR(command.readAddress));
}
//------------------------------------------------------
void SystemSettingViewModel::updateMachineName(const stModBusCommunicationData &command)
{
    if (command.comSize != MACHINE_NAME_SIZE) {
        qWarning("%s(%d): command.comSize:%d != MACHINE_NAME_SIZE(%d)", __PRETTY_FUNCTION__, __LINE__, command.comSize, MACHINE_NAME_SIZE);
        return;
    }
    QByteArray machineName;
    for (int i = 0; i < MACHINE_NAME_SIZE; i++) {
        const auto value = static_cast<quint16>(m_modbus->getReceiveDataHR(command.readAddress + i));
        const char ch1 = static_cast<char>((value & 0xFF00) >> 8);
        const char ch2 = static_cast<char>((value & 0x00FF));
        machineName.append(ch1);
        // qDebug("received:[%c](%d)", machineName.at(machineName.size()-1), machineName.at(machineName.size()-1));  // for debug.
        machineName.append(ch2);
        // qDebug("received:[%c](%d)", machineName.at(machineName.size()-1), machineName.at(machineName.size()-1));  // for debug.
    }
    setMachineName(machineName);
}
//------------------------------------------------------
void SystemSettingViewModel::updateMachineSetupDate(const stModBusCommunicationData &command)
{
    if (command.comSize != MACHINE_SETUP_DATE_SIZE) {
        qWarning("%s(%d): command.comSize:%d != MACHINE_SETUP_DATE_SIZE(%d)", __PRETTY_FUNCTION__, __LINE__, command.comSize, MACHINE_SETUP_DATE_SIZE);
        return;
    }
    int year   = m_modbus->getReceiveDataHR(command.readAddress + 0);
    int month  = m_modbus->getReceiveDataHR(command.readAddress + 1);
    int day    = m_modbus->getReceiveDataHR(command.readAddress + 2);
    int hour   = m_modbus->getReceiveDataHR(command.readAddress + 3);
    int minute = m_modbus->getReceiveDataHR(command.readAddress + 4);
    int second = m_modbus->getReceiveDataHR(command.readAddress + 5);
    // qDebug("received:%d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minute, second);   // for debug.
    QDate date(2000 + (year % 100), month, day);
    QTime time(hour, minute, second);
    QDateTime dateTime(date, time);
    setMachineSetupDate(dateTime);
}
//------------------------------------------------------
void SystemSettingViewModel::updateAcceptanceRegistrationCode(const stModBusCommunicationData &command)
{
    setAcceptanceRegistrationCode(m_modbus->getReceiveDataHR(command.readAddress));
}
//------------------------------------------------------
void SystemSettingViewModel::updateAcceptanceRemainingTime(const stModBusCommunicationData &command)
{
    setAcceptanceRemainingTime(m_modbus->getWordReceiveDataRG(command.readAddress));
}
//------------------------------------------------------
void SystemSettingViewModel::updateSystemMode(const stModBusCommunicationData &command)
{
    setSystemMode(m_modbus->getReceiveDataHR(command.readAddress));
}
