#include "versionviewmodel.h"

#include "appenvironment.h" // AppEnvironment.

//------------------------------------------------------
// VersionViewModel.
//------------------------------------------------------
VersionViewModel::VersionViewModel(ResourceManager *resource, CommunicationThread *comm)
    : ViewModelBase(resource, comm)
    , mIsModbusConnectOneShot(false)
    , m_versionTbl({"","","","","","","","","","","","","","","","","","","",""})
{
}
//------------------------------------------------------
VersionViewModel::~VersionViewModel()
{
    m_versionTbl.clear();
}
//------------------------------------------------------
void VersionViewModel::display()
{
    qDebug("[VersionManagementViewModel]");
    qDebug("  - tbl size:[%d]", m_versionTbl.size());
    for (auto& version : m_versionTbl) {
        qDebug("  - version:[%s]", qUtf8Printable(version));
    }
}
//------------------------------------------------------
void VersionViewModel::onActivate()
{
    startModbusOneShot();
    modbusGetVersions();
    connect(m_communication, &CommunicationThread::finish, this, &VersionViewModel::onFinished);
}
//------------------------------------------------------
void VersionViewModel::onDeactivate()
{
    stopModbusOneShot();
    disconnect(m_communication, &CommunicationThread::finish, this, &VersionViewModel::onFinished);
}
//------------------------------------------------------
void VersionViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerFunctionVersionPageModel) {
        qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }
    if (command.comSize == 0) {
        qDebug("%s:%d comSize[%d]", __func__, __LINE__, command.comSize);
        return;
    }
    qDebug("addr:%d, comSize:%d", command.readAddress, command.comSize);
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        // receive HR.
    } else if (command.funcCode == QModbusPdu::FunctionCode::ReadInputRegisters) {
        // receive SR.
        switch (command.readAddress) {
        case VERSION_DATA_SR:
            updateVersions(command);
            break;
        default:
            // qDebug("unknown addr:%d", command.readAddress);  // for debug.
            return;
        }
    }
}

void VersionViewModel::onFinished()
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
void VersionViewModel::startModbusOneShot()
{
    if (!m_communication) { return; }
    if (mIsModbusConnectOneShot) { return; }
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &VersionViewModel::onOneShotReadFinished, Qt::QueuedConnection);
    mIsModbusConnectOneShot = true;
}
//------------------------------------------------------
void VersionViewModel::stopModbusOneShot()
{
    if (!m_communication) { return; }
    if (!mIsModbusConnectOneShot) { return; }
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
        this, &VersionViewModel::onOneShotReadFinished);
    mIsModbusConnectOneShot = false;
}
//------------------------------------------------------
void VersionViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    if (!m_communication) { return; }
    m_communication->setOneShotModbusCaller(CommunicationThread::CallerFunctionVersionPageModel);
    m_communication->setOneShotModbusCommand(data);
}
//------------------------------------------------------
void VersionViewModel::readModbus(const QModbusPdu::FunctionCode funcCode, const quint16 &address, const quint16 &range)
{
    stModBusCommunicationData command = ModbusCommandGenerator::makeModbusReadCommand(funcCode, address, range);
    setOneShotModbusCommand(command);
}
//------------------------------------------------------
void VersionViewModel::readModbusSR(const quint16 &srAddress, const quint16 &range)
{
    qDebug("called readModbusSR srAddress:%d range:%d.", srAddress, range);
    readModbus(QModbusPdu::FunctionCode::ReadInputRegisters, srAddress, range);
}
//------------------------------------------------------
void VersionViewModel::modbusGetVersions()
{
    readModbusSR(VERSION_DATA_SR, VERSION_DATA_RANGE);
}
//------------------------------------------------------
void VersionViewModel::updateVersions(const stModBusCommunicationData& command)
{
    if (command.comSize != VERSION_DATA_RANGE) {
        qWarning("%s(%d): command.comSize:%d != MACHINE_SETUP_DATE_SIZE(%d)", __PRETTY_FUNCTION__, __LINE__, command.comSize, VERSION_DATA_RANGE);
        return;
    }
    m_versionTbl.clear();
    appendVersionsCollectedFromModbus(command);
    appendIndividuallyCollectedVersions();
    emit versionTblChanged(m_versionTbl);
    // display();  // todo: debug.
}
//-----------------------------
void VersionViewModel::appendVersionsCollectedFromModbus(const stModBusCommunicationData& command)
{
    for (int i = 0; i < NUM_OF_VERSION; i++) {
        QString version("");
        for (int j = 0; j < VERSION_DATA_SIZE; j++) {
            quint16 offset = static_cast<quint16>((i * VERSION_DATA_SIZE) + j);
            const auto value = static_cast<quint16>(m_modbus->getWordReceiveDataRG(command.readAddress + offset));
            const char ch1 = static_cast<char>((value & 0xFF00) >> 8);
            const char ch2 = static_cast<char>((value & 0x00FF));
            version.append(ch1);
            version.append(ch2);
        }
        m_versionTbl << version;
    }
}
//-----------------------------
void VersionViewModel::appendIndividuallyCollectedVersions()
{
    m_versionTbl << AppEnvironment::getQtVersion();
    m_versionTbl << AppEnvironment::getMainAppVersion();
    m_versionTbl << m_resource->penMsgCsvVersion();
    m_versionTbl << m_resource->penSetCsvVersion();
}

