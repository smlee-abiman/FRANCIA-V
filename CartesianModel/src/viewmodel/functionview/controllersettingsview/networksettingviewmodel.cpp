#include "networksettingviewmodel.h"
#include "corelibsdcontrol.h"

//------------------------------------------------------
// NetworkSetting.
//------------------------------------------------------
int NetworkSetting::g_counter = 1;  // 1-base.
//------------------------------------------------------
NetworkSetting::NetworkSetting(QObject *parent)
    : QObject(parent)
    , m_entryId("")
    , m_modbusId(0)
    , m_ipAddress({0,0,0,0})
    , m_subnetMask({0,0,0,0})
    , m_macAddress({0,0,0,0,0,0})
{
    if (2 <= g_counter) {
       m_entryId += QString::number(g_counter);
    }
    g_counter++;
}
//------------------------------------------------------
NetworkSetting::~NetworkSetting()
{
    m_ipAddress.clear();
    m_subnetMask.clear();
    m_macAddress.clear();
}
//------------------------------------------------------
void NetworkSetting::setModbusId(const quint16 &modbusId)
{
    if (m_modbusId == modbusId) {
        return;
    }
    m_modbusId = modbusId;
    emit modbusIdChanged(m_modbusId);
}
//------------------------------------------------------
void NetworkSetting::setIpAddress(const QList<int> &ipAddress)
{
    if (m_ipAddress.size() != ipAddress.size()) {
        return;
    }
    if (m_ipAddress == ipAddress) {
        return;
    }
    for (int octet = 0; octet < m_ipAddress.size(); octet++) {
        m_ipAddress[octet] = ipAddress[octet];
    }
    emit ipAddressChanged(m_ipAddress);
}
//------------------------------------------------------
void NetworkSetting::setSubnetMask(const QList<int> &subnetMask)
{
    if (m_subnetMask.size() != subnetMask.size()) {
        return;
    }
    if (m_subnetMask == subnetMask) {
        return;
    }
    m_subnetMask = subnetMask;
    emit subnetMaskChanged(m_subnetMask);
}
//------------------------------------------------------
void NetworkSetting::setMacAddress(const QList<int> &macAddress)
{
    if (m_macAddress == macAddress) {
        return;
    }
    m_macAddress = macAddress;
    emit macAddressChanged(m_macAddress);
}
//------------------------------------------------------
void NetworkSetting::editModbusId(const quint16 value)
{
    setModbusId(value);
}
//------------------------------------------------------
void NetworkSetting::editIpAddress(const quint16 index, const int value)
{
    if (m_ipAddress.size() <= index) {
        qWarning("array index out of bounds. index:%d, m_ipAddress.size:%d", index, m_ipAddress.size());
        return;
    }
    if (m_ipAddress[index] == value) {
        return;
    }
    m_ipAddress[index] = value;
    emit ipAddressChanged(m_ipAddress);
}
//------------------------------------------------------
void NetworkSetting::editSubnetMask(const quint16 index, const int value)
{
    if (m_subnetMask.size() <= index) {
        qWarning("array index out of bounds. index:%d, m_subnetMask.size:%d", index, m_subnetMask.size());
        return;
    }
    if (m_subnetMask[index] == value) {
        return;
    }
    m_subnetMask[index] = value;
    emit subnetMaskChanged(m_subnetMask);
}
//------------------------------------------------------
void NetworkSetting::saveToSdControl()
{
    saveIpAddressToControllerSettings();
    saveModbusIdToPendantSettings();
}
//------------------------------------------------------
void NetworkSetting::display()
{
    qDebug(" - [NetworkSetting[%s]:%d] -", qPrintable(m_entryId), m_modbusId);
    qDebug("   - ip(%d) :%d.%d.%d.%d", m_ipAddress.size(), m_ipAddress[0], m_ipAddress[1], m_ipAddress[2], m_ipAddress[3]);
    qDebug("   - sub(%d):%d.%d.%d.%d", m_subnetMask.size(), m_subnetMask[0], m_subnetMask[1], m_subnetMask[2], m_subnetMask[3]);
    qDebug("   - mac(%d):%02X:%02X:%02X:%02X:%02X:%02X", m_macAddress.size(), m_macAddress[0], m_macAddress[1], m_macAddress[2], m_macAddress[3], m_macAddress[4], m_macAddress[5]);
}
//------------------------------------------------------
void NetworkSetting::saveIpAddressToControllerSettings()
{
    QString entryStr = "IPAddress" + m_entryId;
    QString ipAddressStr = QString("%1.%2.%3.%4")
                            .arg(m_ipAddress[0]).arg(m_ipAddress[1])
                            .arg(m_ipAddress[2]).arg(m_ipAddress[3]);
    CoreLibSdControl m_sdControl;
    qInfo() << QString("%1(%2): [System] %3=%4").arg(__PRETTY_FUNCTION__).arg(__LINE__).arg(entryStr, ipAddressStr);
    m_sdControl.setControllerData("System", entryStr, ipAddressStr);
}
//------------------------------------------------------
void NetworkSetting::saveModbusIdToPendantSettings()
{
    QString sectionStr  = "Modbus" + m_entryId;
    QString modbusIdStr = QString::number(m_modbusId);
    CoreLibSdControl m_sdControl;
    qInfo() << QString("%1(%2): [%3] ServerAddress=%4").arg(__PRETTY_FUNCTION__).arg(__LINE__).arg(sectionStr, modbusIdStr);
    m_sdControl.setPendantData(sectionStr, "ServerAddress", modbusIdStr);
}
//------------------------------------------------------
// NetworkSettingViewModel.
//------------------------------------------------------
NetworkSettingViewModel::NetworkSettingViewModel(ResourceManager *resource, CommunicationThread *comm)
    : ViewModelBase(resource, comm)
    , mIsModbusConnectOneShot(false)
{
    init();
}
//------------------------------------------------------
NetworkSettingViewModel::~NetworkSettingViewModel()
{
    qDeleteAll(m_networkSettingTbl);
    m_networkSettingTbl.clear();
}
//------------------------------------------------------
void NetworkSettingViewModel::saveNetworkSettings()
{
    QList<int> ipAddressData;
    QList<int> subnetMaskData;
    QList<int> modbusIdData;
    for (auto& data : m_networkSettingTbl) {
        auto* networkSetting = dynamic_cast<NetworkSetting*>(data);
        ipAddressData  << networkSetting->ipAddress();
        subnetMaskData << networkSetting->subnetMask();
        modbusIdData   << networkSetting->modbusId();
        networkSetting->saveToSdControl();
    }
    modbusWriteIpAddress(ipAddressData);
    modbusWriteSubnetMask(subnetMaskData);
    modbusWriteModbusId(modbusIdData);
    modbusWriteApplyNetworkSetting();
    modbusGetIpAddress();
    modbusGetSubnetMask();
    modbusGetModbusId();
}
//------------------------------------------------------
void NetworkSettingViewModel::display()
{
    qDebug("[NetworkSettingViewModel]");
    for (auto& data : m_networkSettingTbl) {
        auto* networkSetting = dynamic_cast<NetworkSetting*>(data);
        networkSetting->display();
    }
}
//------------------------------------------------------
void NetworkSettingViewModel::onFinished()
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
void NetworkSettingViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerFunctionControllerSettingNetworkSetPageModel) {
        qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }
    if (command.comSize == 0) {
        qDebug("%s:%d comSize[%d]", __func__, __LINE__, command.comSize);
        return;
    }
    qDebug("addr:%d, comSize:%d", command.readAddress, command.comSize);
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        // recevie HR.
        switch (command.readAddress) {
        case IP_ADDRESS_HR:
            updateIpAddress(command);
            break;
        case SUBNET_MASK_HR:
            updateSubnetMask(command);
            break;
        case MAC_ADDRESS_HR:
            updateMacAddress(command);
            break;
        case MODBUS_ID_HR:
            updateModbusId(command);
            break;
        default:
            qDebug("unknown addr:%d", command.readAddress);
            break;
        }
    }
}
//------------------------------------------------------
void NetworkSettingViewModel::onActivate()
{
    startModbusOneShot();
    modbusGetIpAddress();
    modbusGetSubnetMask();
    modbusGetMacAddress();
    modbusGetModbusId();
}
//------------------------------------------------------
void NetworkSettingViewModel::onDeactivate()
{
    stopModbusOneShot();
}
//------------------------------------------------------
void NetworkSettingViewModel::init()
{
    m_networkSettingTbl << new NetworkSetting();
    m_networkSettingTbl << new NetworkSetting();
    // display();  // todo: debug.
}
//------------------------------------------------------
void NetworkSettingViewModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (mIsModbusConnectOneShot) return;
    connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &NetworkSettingViewModel::onOneShotReadFinished, Qt::QueuedConnection);
    mIsModbusConnectOneShot = true;
}
//------------------------------------------------------
void NetworkSettingViewModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (!mIsModbusConnectOneShot) return;
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
        this, &NetworkSettingViewModel::onOneShotReadFinished);
    mIsModbusConnectOneShot = false;
}
//------------------------------------------------------
void NetworkSettingViewModel::writeModbusHR(const quint16 hrAddress, const QList<int> &writeData)
{
    qDebug("called writeModbugHR hrAddress:%d range:%d.", hrAddress, writeData.size());
    for (auto& data : writeData) { qDebug(" - [%5d](0x%04x)", data, data); }
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, writeData.size(), writeData);
    m_communication->setModbusWriteCommand(command);
}
//------------------------------------------------------
void NetworkSettingViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    if (!m_communication) return;
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerFunctionControllerSettingNetworkSetPageModel);
    m_communication->setOneShotModbusCommand(data);
}
//------------------------------------------------------
void NetworkSettingViewModel::readModbusHR(const quint16 &hrAddress, const quint16 &range)
{
    qDebug("called readModbusHR hrAddress:%d range:%d.", hrAddress, range);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, hrAddress, range);
    setOneShotModbusCommand(command);
}
//------------------------------------------------------
void NetworkSettingViewModel::modbusWriteIpAddress(const QList<int> &writeData)
{
    writeModbusHR(IP_ADDRESS_HR, writeData);
}
//------------------------------------------------------
void NetworkSettingViewModel::modbusWriteSubnetMask(const QList<int> &writeData)
{
    writeModbusHR(SUBNET_MASK_HR, writeData);
}
//------------------------------------------------------
void NetworkSettingViewModel::modbusWriteModbusId(const QList<int> &writeData)
{
    writeModbusHR(MODBUS_ID_HR, writeData);
}
//------------------------------------------------------
void NetworkSettingViewModel::modbusWriteApplyNetworkSetting()
{
    QList<int> writeData;
    writeData << 1;
    writeModbusHR(APPLY_NETWORK_SETTING_HR, writeData);
}
//------------------------------------------------------
void NetworkSettingViewModel::modbusGetIpAddress()
{
    readModbusHR(IP_ADDRESS_HR, IP_ADDRESS_RANGE);
}
//------------------------------------------------------
void NetworkSettingViewModel::modbusGetSubnetMask()
{
    readModbusHR(SUBNET_MASK_HR, SUBNET_MASK_RANGE);
}
//------------------------------------------------------
void NetworkSettingViewModel::modbusGetMacAddress()
{
    readModbusHR(MAC_ADDRESS_HR, MAC_ADDRESS_RANGE);
}
//------------------------------------------------------
void NetworkSettingViewModel::modbusGetModbusId()
{
    readModbusHR(MODBUS_ID_HR, MODBUS_ID_RANGE);
}
//------------------------------------------------------
void NetworkSettingViewModel::updateIpAddress(const stModBusCommunicationData &command)
{
    if (command.comSize != IP_ADDRESS_RANGE) {
        qWarning("%s(%d): command.comSize:%d != IP_ADDRESS_RANGE(%d)", __PRETTY_FUNCTION__, __LINE__, command.comSize, IP_ADDRESS_RANGE);
        return;
    }
    int readAddress = command.readAddress;
    for (auto& data : m_networkSettingTbl) {
        QList<int> ipAddress;
        for (int i = 0; i < IP_ADDRESS_SIZE; ++i) {
            ipAddress << m_modbus->getReceiveDataHR(readAddress++);
        }
        auto* networkSetting = dynamic_cast<NetworkSetting*>(data);
        networkSetting->setIpAddress(ipAddress);
    }
}
//------------------------------------------------------
void NetworkSettingViewModel::updateSubnetMask(const stModBusCommunicationData &command)
{
    if (command.comSize != SUBNET_MASK_RANGE) {
        qWarning("%s(%d): command.comSize:%d != SUBNET_MASK_RANGE(%d)", __PRETTY_FUNCTION__, __LINE__, command.comSize, SUBNET_MASK_RANGE);
        return;
    }
    int readAddress = command.readAddress;
    for (auto& data : m_networkSettingTbl) {
        QList<int> subnetMask;
        for (int i = 0; i < SUBNET_MASK_SIZE; ++i) {
            subnetMask << m_modbus->getReceiveDataHR(readAddress++);
        }
        auto* networkSetting = dynamic_cast<NetworkSetting*>(data);
        networkSetting->setSubnetMask(subnetMask);
    }
}
//------------------------------------------------------
void NetworkSettingViewModel::updateMacAddress(const stModBusCommunicationData &command)
{
    if (command.comSize != MAC_ADDRESS_RANGE) {
        qWarning("%s(%d): command.comSize:%d != MAC_ADDRESS_RANGE(%d)", __PRETTY_FUNCTION__, __LINE__, command.comSize, MAC_ADDRESS_RANGE);
        return;
    }
    int readAddress = command.readAddress;
    for (auto& data : m_networkSettingTbl) {
        QList<int> macAddress;
        for (int i = 0; i < MAC_ADDRESS_SIZE; ++i) {
            macAddress << m_modbus->getReceiveDataHR(readAddress++);
        }
        auto* networkSetting = dynamic_cast<NetworkSetting*>(data);
        networkSetting->setMacAddress(macAddress);
    }
}
//------------------------------------------------------
void NetworkSettingViewModel::updateModbusId(const stModBusCommunicationData &command)
{
    if (command.comSize != MODBUS_ID_RANGE) {
        qWarning("%s(%d): command.comSize:%d != MODBUS_ID_RANGE(%d)", __PRETTY_FUNCTION__, __LINE__, command.comSize, MODBUS_ID_RANGE);
        return;
    }
    int readAddress = command.readAddress;
    for (auto& data : m_networkSettingTbl) {
        quint16 modbusId = static_cast<quint16>(m_modbus->getReceiveDataHR(readAddress++));
        auto* networkSetting = dynamic_cast<NetworkSetting*>(data);
        networkSetting->setModbusId(modbusId);
    }
}
