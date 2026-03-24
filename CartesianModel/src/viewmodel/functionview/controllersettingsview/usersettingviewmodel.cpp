#include "usersettingviewmodel.h"
#include "corelibsdcontrol.h"

#include <bitset>

#define PENSET_GROUP_STR        ("17")
#define NUM_OF_USER_SETTINGS    (40)
#define USER_SETTINGS_OFFSET    (8)
#define SYSTEM_SETTINGS_DISPLAY_LEVEL_OFFSET (50)

#ifdef REAL_TARGET
#define ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS (1)
#else // REAL_TARGET
#define ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS (0)
#endif // REAL_TARGET

//------------------------------------------------------
// ScreenSettings.
//------------------------------------------------------
ScreenSettings::ScreenSettings(int index, ScreenDp *dp, QObject *parent)
    : QObject(parent), m_index(index), m_dp(dp)
#if ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS
    , m_visibles({false, false, false, false, false})
#else // ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS
    , m_visibles({true, true, true, true, true})
#endif // ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS
{
}
//------------------------------------------------------
ScreenSettings::~ScreenSettings()
{
}
//------------------------------------------------------
void ScreenSettings::setVisibles(const QList<bool> &visibles)
{
    if (m_visibles == visibles) { return; }
    m_visibles = visibles;
    emit visiblesChanged(m_visibles);
}
//------------------------------------------------------
void ScreenSettings::display()
{
    QString key((m_dp == nullptr) ? "" : m_dp->key());
    qDebug() << "    -"
            << key           << ","
            << m_visibles[0] << ","
            << m_visibles[1] << ","
            << m_visibles[2] << ","
            << m_visibles[3] << ","
            << m_visibles[4];
}
//------------------------------------------------------
// UserSettingViewModel.
//------------------------------------------------------
UserSettingViewModel::UserSettingViewModel(ResourceManager *resource, CommunicationThread *comm)
    : ViewModelBase(resource, comm)
    , mIsModbusConnectOneShot(false)
    , m_numOfRemainingInitialModbusReads(1)
    , m_systemSettingsDisplayLevel(4)
{
    readPenSetData();
    startModbusOneShot();
#if ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS
    modbusGetScreenSettings();
#endif // ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS
    // display();    // for debug.
}
//------------------------------------------------------
UserSettingViewModel::~UserSettingViewModel()
{
    qDeleteAll(m_screenSettingsTbl);
    m_screenSettingsTbl.clear();
    m_screenSettingsReferenceTbl.clear();
}
//------------------------------------------------------
QList<QObject *> UserSettingViewModel::screenSettingsTbl() const
{
    auto values = m_screenSettingsTbl.values();
    std::sort(values.begin(), values.end(), [](auto const& lhs, auto const& rhs) {
        auto* lSet = dynamic_cast<ScreenSettings*>(lhs);
        auto* rSet = dynamic_cast<ScreenSettings*>(rhs);
        return (lSet->index() < rSet->index());
    });
    return values;
}
//------------------------------------------------------
void UserSettingViewModel::display()
{
    qDebug("  - m_screenSettingsTbl");
    qDebug("    - size:%d", m_screenSettingsTbl.size());
    qDebug("    - system setting display level:%d", m_systemSettingsDisplayLevel);
    // auto values = m_screenSettingsTbl;
    auto values = screenSettingsTbl();
    for (auto& item : values) {
        auto* screenSettings = dynamic_cast<ScreenSettings*>(item);
        screenSettings->display();
    }
}
//------------------------------------------------------
void UserSettingViewModel::toggleScreenVisibility(QObject* object, int userLevel)
{
    if (object == nullptr) { return; }
    if (5 <= userLevel) { return; }
    auto* screenSettings = dynamic_cast<ScreenSettings*>(object);
    auto visibles = screenSettings->visibles();
    visibles[userLevel] = !visibles[userLevel];   // toggle visiblity.
    // QList<bool> -> std::bitset<5>
    std::bitset<5> bits;
    bits[0] = visibles[0];
    bits[1] = visibles[1];
    bits[2] = visibles[2];
    bits[3] = visibles[3];
    bits[4] = visibles[4];
    QList<int> writeData;
    writeData << (static_cast<int>(bits.to_ulong()) & 0x0000001F);
    writeModbusHR(SCREEN_VISIBILITY_HR + screenSettings->index(), writeData);
    modbusGetScreenSettings(screenSettings->index(), 1);
}
//------------------------------------------------------
QObject *UserSettingViewModel::getScreenSettings(int dp1, int dp2)
{
    ScreenDp dp(dp1,dp2);
    if (!m_screenSettingsReferenceTbl.contains(dp.key())) { return nullptr; }
    auto* screenSettings = dynamic_cast<ScreenSettings*>(m_screenSettingsReferenceTbl.value(dp.key()));
    ScreenSettings* ret = new ScreenSettings(-1, &dp);
    ret->setVisibles(screenSettings->visibles());
    return ret;
}
//------------------------------------------------------
void UserSettingViewModel::onActivate()
{
    startModbusPeriodic();
    startModbusOneShot();
#if ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS
    modbusGetScreenSettings();
#endif // ENABLE_READ_SCREEN_SETTINGS_FROM_MODBUS
}
//------------------------------------------------------
void UserSettingViewModel::onDeactivate()
{
    stopModbusPeriodic();
    stopModbusOneShot();
}
//------------------------------------------------------
void UserSettingViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (isViewActive()) {
        if (oneShotReadCaller != CommunicationThread::CallerFunctionControllerSettingUserSetPageModel) {
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
        // receive HR.
        if ((SCREEN_VISIBILITY_HR <= command.readAddress) &&
            (command.readAddress <= SCREEN_VISIBILITY_HR_END)) {
            updateScreenSettings(command);
            decrementNumOfRemainingInitialModbusReads();
        }
    } else if (command.funcCode == QModbusPdu::FunctionCode::ReadInputRegisters) {
        // receive SR.
    }
    if (!isViewActive() && (m_numOfRemainingInitialModbusReads <= 0)) {
        stopModbusOneShot();
    }
}

void UserSettingViewModel::onFinished()
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
void UserSettingViewModel::readPenSetData()
{
    m_screenSettingsTbl.clear();
    m_screenSettingsReferenceTbl.clear();
    for (int screenIndex = 0; screenIndex < 40; screenIndex++) {
        const auto dp1 = m_resource->getPenSetIntValue(PENSET_GROUP_STR, USER_SETTINGS_OFFSET + screenIndex, 0);
        const auto dp2 = m_resource->getPenSetIntValue(PENSET_GROUP_STR, USER_SETTINGS_OFFSET + screenIndex, 1);
        if (dp1 == 0) { continue; } // skip.
        auto* screenDp = new ScreenDp(dp1, dp2);
        auto* screenSettings = new ScreenSettings(m_screenSettingsTbl.count(), screenDp);
        m_screenSettingsTbl.insert(m_screenSettingsTbl.count(), screenSettings);
        m_screenSettingsReferenceTbl.insert(screenDp->key(), screenSettings);
    }
    m_systemSettingsDisplayLevel = m_resource->getPenSetIntValue(PENSET_GROUP_STR, SYSTEM_SETTINGS_DISPLAY_LEVEL_OFFSET, 0);
}

void UserSettingViewModel::startModbusPeriodic()
{
    if (!m_communication) return;
    connect(m_communication, &CommunicationThread::finish, this, &UserSettingViewModel::onFinished, Qt::QueuedConnection);
}

void UserSettingViewModel::stopModbusPeriodic()
{
    if (!m_communication) return;
    disconnect(m_communication, &CommunicationThread::finish, this, &UserSettingViewModel::onFinished);
}
//------------------------------------------------------
void UserSettingViewModel::startModbusOneShot()
{
    if (!m_communication) { return; }
    if (mIsModbusConnectOneShot) { return; }
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &UserSettingViewModel::onOneShotReadFinished, Qt::QueuedConnection);
    mIsModbusConnectOneShot = true;
}
//------------------------------------------------------
void UserSettingViewModel::stopModbusOneShot()
{
    if (!m_communication) { return; }
    if (!mIsModbusConnectOneShot) { return; }
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
        this, &UserSettingViewModel::onOneShotReadFinished);
    mIsModbusConnectOneShot = false;
}
//------------------------------------------------------
void UserSettingViewModel::decrementNumOfRemainingInitialModbusReads()
{
    if (m_numOfRemainingInitialModbusReads <= 0) { return; }
    m_numOfRemainingInitialModbusReads--;
}
//------------------------------------------------------
void UserSettingViewModel::writeModbusHR(quint16 hrAddress, const QList<int> &writeData)
{
    qDebug("called writeModbugHR hrAddress:%d range:%d.", hrAddress, writeData.size());
    for (auto& data : writeData) { qDebug(" - [%u](0x%08x)", data, data); }
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, writeData.size(), writeData);
    m_communication->setModbusWriteCommand(command);
}
//------------------------------------------------------
void UserSettingViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    if (!m_communication) { return; }
    m_communication->setOneShotModbusCaller(CommunicationThread::CallerFunctionControllerSettingUserSetPageModel);
    m_communication->setOneShotModbusCommand(data);
}
//------------------------------------------------------
void UserSettingViewModel::readModbus(const QModbusPdu::FunctionCode funcCode, const quint16 &address, const quint16 &range)
{
    stModBusCommunicationData command = ModbusCommandGenerator::makeModbusReadCommand(funcCode, address, range);
    setOneShotModbusCommand(command);
}
//------------------------------------------------------
void UserSettingViewModel::readModbusHR(const quint16 &hrAddress, const quint16 &range)
{
    qDebug("called readModbusHR srAddress:%d range:%d.", hrAddress, range);
    readModbus(QModbusPdu::FunctionCode::ReadHoldingRegisters, hrAddress, range);
}
//------------------------------------------------------
void UserSettingViewModel::modbusGetScreenSettings(const quint16& index, const quint16& range)
{
    readModbusHR(SCREEN_VISIBILITY_HR + index, range);
}
//------------------------------------------------------
void UserSettingViewModel::updateScreenSettings(const stModBusCommunicationData& command)
{
    if (SCREEN_VISIBILITY_RANGE < command.comSize) {
        qWarning("%s(%d): SCREEN_VISIBILITY_RANGE:%d < command.comSize:%d", __PRETTY_FUNCTION__, __LINE__, SCREEN_VISIBILITY_RANGE, command.comSize);
        return;
    }
    for (int i = 0; i < command.comSize; i++) {
        auto readAddress = command.readAddress + i;
        auto tblIndex = readAddress - SCREEN_VISIBILITY_HR;
        if (!m_screenSettingsTbl.contains(tblIndex)) {
            continue;
        }
        auto* screenSettings = dynamic_cast<ScreenSettings*>(m_screenSettingsTbl.value(tblIndex));
        auto value = m_modbus->getReceiveDataHR(readAddress);
        // std::bitset<5> -> QList<bool>
        std::bitset<5> bits(value & 0x0000001F);
        QList<bool> visibles;
        visibles << bits[0] << bits[1] << bits[2] << bits[3] << bits[4];
        screenSettings->setVisibles(visibles);
        screenSettings->display();  // for debug.
    }
}
