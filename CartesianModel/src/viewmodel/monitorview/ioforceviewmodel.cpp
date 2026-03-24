#include "ioforceviewmodel.h"

#define mConvertIndexToHRBitOffset(index) (((index % 16) / 8) ? (index % 8) : ((index % 8) + 8))

//------------------------------------------------------
// IOExchangeData.
//------------------------------------------------------
IOSwapConfiguration::IOSwapConfiguration(QObject *parent)
    : IOSwapConfiguration(-1, -1, parent)
{
}
IOSwapConfiguration::IOSwapConfiguration(quint16 diAddress, quint16 bankNo, QObject *parent)
    : QObject(parent)
    , m_diAddress(diAddress)
    , m_bankNo(bankNo)
{
}
void IOSwapConfiguration::display()
{
    qDebug("diAddress:%3d, bankNo:%3d(%3X)", m_diAddress, m_bankNo, m_bankNo);
}

//------------------------------------------------------
// IOSwapData.
//------------------------------------------------------
IOSwapData::IOSwapData(QObject *parent)
    : IOSwapData(0, parent)
{
}
IOSwapData::IOSwapData(quint16 addressOffset, QObject *parent)
    : QObject(parent)
    , m_addressOffset(addressOffset)
    , m_fromBankNo(0)
    , m_toBankNo(0)
{
}
bool IOSwapData::setFromBankNo(const quint16 &fromBankNo, quint16 &prevBankNo)
{
    prevBankNo = m_fromBankNo;
    if (m_fromBankNo != fromBankNo) {
        m_fromBankNo = fromBankNo;
        return true;
    }
    return false;
}
bool IOSwapData::setToBankNo(const quint16 &toBankNo, quint16 &prevBankNo)
{
    prevBankNo = m_toBankNo;
    if (m_toBankNo != toBankNo) {
        m_toBankNo = toBankNo;
        return true;
    }
    return false;
}
void IOSwapData::display() {
    qDebug("offset:%d, fromBankNo:%d, toBankNo:%d", m_addressOffset, m_fromBankNo, m_toBankNo);
}

//------------------------------------------------------
// ForceIOData.
//------------------------------------------------------
ForceIOData::ForceIOData(QObject *parent)
    : ForceIOData(-1, -1, -1, -1, parent)
{
}
ForceIOData::ForceIOData(quint16 forcedDio, quint16 releasedDo, quint16 inversDo, quint16 bankNo, QObject *parent)
    : QObject(parent)
    , m_bits(0)
    , m_forcedDio(forcedDio)
    , m_releasedDo(releasedDo)
    , m_inversDo(inversDo)
    , m_bankNo(bankNo)
    , m_swapDi(-1)
{
}
void ForceIOData::display()
{
    qDebug("forcedOn:%s, forcedOff:%s, invers:%s, forcedDIO:%3d, releasedDO:%4d, inversDI:%4d, bankNo:%X, swapDi:%03d"
        , forcedOn()?"true":"false", forcedOff()?"true":"false", invers()?"true":"false"
        , m_forcedDio, m_releasedDo, m_inversDo, m_bankNo, m_swapDi
        );
}

//------------------------------------------------------
// IOForceViewModel.
//------------------------------------------------------
IOForceViewModel::IOForceViewModel(QObject *object)
    : IOForceViewModel(nullptr, nullptr)
{
    Q_UNUSED(object)
}

IOForceViewModel::IOForceViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
    , m_ioSwapConfigTblsIndex(0)
{
    init();
}

IOForceViewModel::~IOForceViewModel()
{
    stopModbusOneShot();
    stopModbusPeriodic();
}

void IOForceViewModel::forciblyTurnOn(quint16 index)
{
    if (m_forceIoDataTbl.size() <= index) {
        qWarning("array index out of bounds. index:%d, m_forceIoDataTbl.size:%d", index, m_forceIoDataTbl.size());
        return;
    }
    ForceIOData* iodata = dynamic_cast<ForceIOData*>(m_forceIoDataTbl[index]);
    if (iodata->forcedOn()) {
        return;
    }
    writeModbusDo(iodata->forcedDio(), 1);
    modbusGetForcedOn(iodata->forcedDio(), 1);
    modbusGetForcedOff(iodata->forcedDio(), 1);
    m_operationRecord->recordOpeMonitorForceIo(iodata->forcedDio(), 0, 1);
}

void IOForceViewModel::forciblyTurnOff(quint16 index)
{
    if (m_forceIoDataTbl.size() <= index) {
        qWarning("array index out of bounds. index:%d, m_forceIoDataTbl.size:%d", index, m_forceIoDataTbl.size());
        return;
    }
    ForceIOData* iodata = dynamic_cast<ForceIOData*>(m_forceIoDataTbl[index]);
    if (iodata->forcedOff()) {
        return;
    }
    writeModbusDo(iodata->forcedDio(), 0);
    modbusGetForcedOn(iodata->forcedDio(), 1);
    modbusGetForcedOff(iodata->forcedDio(), 1);
    m_operationRecord->recordOpeMonitorForceIo(iodata->forcedDio(), 1, 0);
}

void IOForceViewModel::releaseForcedIO(quint16 index)
{
    if (m_forceIoDataTbl.size() <= index) {
        qWarning("array index out of bounds. index:%d, m_forceIoDataTbl.size:%d", index, m_forceIoDataTbl.size());
        return;
    }
    ForceIOData* iodata = dynamic_cast<ForceIOData*>(m_forceIoDataTbl[index]);
    if (!iodata->forcedOn() && !iodata->forcedOff()) {
        return;
    }
    writeModbusDo(iodata->releasedDo(), 1);
    modbusGetForcedOn(iodata->forcedDio(), 1);
    modbusGetForcedOff(iodata->forcedDio(), 1);
    m_operationRecord->recordOpeMonitorReleaseIo(iodata->forcedDio(), 0, 1);
}

void IOForceViewModel::inversTurnOn(quint16 index)
{
    if (m_forceIoDataTbl.size() <= index) {
        qWarning("array index out of bounds. index:%d, m_forceIoDataTbl.size:%d", index, m_forceIoDataTbl.size());
        return;
    }
    ForceIOData* iodata = dynamic_cast<ForceIOData*>(m_forceIoDataTbl[index]);
    if (iodata->invers()) {
        return;
    }
    writeModbusDo(iodata->inversDo(), 1);
    m_operationRecord->recordOpeMonitorInverseIo(iodata->forcedDio(), 0, 1);
}

void IOForceViewModel::inversTurnOff(quint16 index)
{
    if (m_forceIoDataTbl.size() <= index) {
        qWarning("array index out of bounds. index:%d, m_forceIoDataTbl.size:%d", index, m_forceIoDataTbl.size());
        return;
    }
    ForceIOData* iodata = dynamic_cast<ForceIOData*>(m_forceIoDataTbl[index]);
    if (!iodata->invers()) {
        return;
    }
    writeModbusDo(iodata->inversDo(), 0);
    m_operationRecord->recordOpeMonitorInverseIo(iodata->forcedDio(), 1, 0);
}

void IOForceViewModel::swapIO(QObject* forceIoPtr, QObject* ioSwapConfigPtr)
{
    if (!forceIoPtr || !ioSwapConfigPtr) {
        qWarning("either forceIoPtr:%p or ioSwapConfigPtr:%p is a nullPtr", forceIoPtr, ioSwapConfigPtr);
        return;
    }
    auto* forceIo = dynamic_cast<ForceIOData*>(forceIoPtr);
    auto* ioSwapConfig = dynamic_cast<IOSwapConfiguration*>(ioSwapConfigPtr);
    auto* ioSwapData = findIoSwapData(forceIo);
    if (!ioSwapData) {
        ioSwapData = freeIoSwapData();
        if (!ioSwapData) {
            qWarning("failed to swap IO. (because the number of cases was exceeded.)");
            return;
        }
    }
    modbusWriteIOSwap(ioSwapData->hrAddress(), forceIo->bankNo(), ioSwapConfig->bankNo());
    modbusGetIOSwap(ioSwapData->hrAddress(), 1);
    m_operationRecord->recordOpeMonitorSwapIo(forceIo->forcedDio(), ioSwapConfig->diAddress());
}

void IOForceViewModel::releaseSwappedIO(QObject *forceIoPtr)
{
    if (!forceIoPtr) {
        qWarning("failed to release the swapped IO. forceIoPtr:%p is a nullPtr", forceIoPtr);
        return;
    }
    auto* forceIo = dynamic_cast<ForceIOData*>(forceIoPtr);
    auto* ioSwapData = findIoSwapData(forceIo);
    if (!ioSwapData) {
        qWarning("failed to release the swapped IO. bankNo:%d is not IO-swap.", forceIo->bankNo());
        return;
    }
    modbusWriteIOSwap(ioSwapData->hrAddress(), 0, 0);
    modbusGetIOSwap(ioSwapData->hrAddress(), 1);
}

QList<bool> IOForceViewModel::getIoSwapConfigTblsStatus(QObject *forceIoPtr)
{
    QList<bool> ret;
    for (int i = 0; i < m_ioSwapConfigTbls.size(); i++) {
        ret << false;
    }
    if (!forceIoPtr) {
        qWarning("%s(%d): failed to get io swap configuration tables status. forceIoPtr:%p is a nullPtr", __PRETTY_FUNCTION__, __LINE__, forceIoPtr);
        return ret;
    }
    auto* forceIo = dynamic_cast<ForceIOData*>(forceIoPtr);
    for (int i = 0; i < m_ioSwapConfigTbls.size(); i++) {
        for (auto& data : m_ioSwapConfigTbls[i]) {
            IOSwapConfiguration* item = dynamic_cast<IOSwapConfiguration*>(data);
            if (item->bankNo() == forceIo->bankNo()) {
                qDebug("found bankNo:%d. io swap configuration table num:%d.", forceIo->bankNo(), i);
                ret[i] = true;
                break;
            }
        }
    }
    return ret;
}

void IOForceViewModel::onLanguageChanged(int language)
{
    // TODO:
    Q_UNUSED(language)
}

void IOForceViewModel::onFinished()
{
    updatePeriodicData();
}

void IOForceViewModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerMonitorIOForcePageModel) {
        qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }
    if (command.comSize == 0) {
        qDebug("%s:%d comSize[%d]", __func__, __LINE__, command.comSize);
        return;
    }
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        if (/*FORCED_ON_HR <= command.readAddress && */ // warning avoidance : warning: comparison is always true due to limited range of data type.
            command.readAddress <= FORCED_ON_HR_END) {
            updateForcedOn(command);
        } else if (FORCED_OFF_HR <= command.readAddress && command.readAddress <= FORCED_OFF_HR_END) {
            updateForcedOff(command);
        } else if (SWAP_INSTRUCTION_HR <= command.readAddress && command.readAddress <= SWAP_INSTRUCTION_HR_END) {
            updateSwapData(command);
        }
    }
}

void IOForceViewModel::onActivate()
{
    startModbusPeriodic();
    startModbusOneShot();
    modbusGetIOSwap();
    modbusGetForcedOn();
    modbusGetForcedOff();
}

void IOForceViewModel::onDeactivate()
{
    stopModbusOneShot();
    stopModbusPeriodic();
}

void IOForceViewModel::init()
{
    // create instance.
    for (int i = 0; i < NUM_OF_SWAP_DATA; i++) {
        auto* ioSwapData = new IOSwapData(i * 2);
        m_ioSwapDataTbl << ioSwapData;
    }
    //
    readPenSetData();
}

void IOForceViewModel::readPenSetData()
{
    m_category1StartPage = m_resource->getPenSetIntValue(PENSET_GROUP, 6, 0) - 1;  // 0-based.
    m_category2StartPage = m_resource->getPenSetIntValue(PENSET_GROUP, 7, 0) - 1;  // 0-based.
    m_category3StartPage = m_resource->getPenSetIntValue(PENSET_GROUP, 8, 0) - 1;  // 0-based.
    m_ioNameTextColorRBG = m_resource->dataColor(PENSET_GROUP_NUM, 210);
    m_pageTextColorRGB   = m_resource->dataColor(PENSET_GROUP_NUM, 211);
    readPenSetForceIoDataTbl();
    readPenSetIoSwapConfigurationTbls();
}

void IOForceViewModel::readPenSetForceIoDataTbl()
{
    int numOfPage = m_resource->getPenSetIntValue(PENSET_GROUP, 5, 0);
    for (int i = 0; i < numOfPage * NUM_OF_IO_DATA_PAR_PAGE; i++) {
        auto penSetRow = PENSET_OFFSET_IO_ADDRESS + i;
        auto forcedDioStr = m_resource->getPenSetStringValue(PENSET_GROUP, penSetRow, 0);
        auto releasedDoStr = m_resource->getPenSetStringValue(PENSET_GROUP, penSetRow, 1);
        auto inversDoStr = m_resource->getPenSetStringValue(PENSET_GROUP, penSetRow, 2);
        auto bankNoStr = m_resource->getPenSetStringValue(PENSET_GROUP, penSetRow, 3);
        bool ok;
        auto forcedDio = static_cast<quint16>(forcedDioStr.toUInt(&ok));
        if (!ok) {
            forcedDio = -1;
        }
        auto releasedDo = static_cast<quint16>(releasedDoStr.toUInt(&ok));
        if (!ok) {
            releasedDo = -1;
        }
        auto inversDo = static_cast<quint16>(inversDoStr.toUInt(&ok));
        if (!ok) {
            inversDo = -1;
        }
        auto bankNo = static_cast<quint16>(bankNoStr.toUInt(&ok, 16));
        if (!ok) {
            bankNo = -1;
        }
        auto* item = new ForceIOData(forcedDio, releasedDo, inversDo, bankNo);
        // item->display();    // todo: debug
        m_forceIoDataTbl.append(item);
    }
}

void IOForceViewModel::readPenSetIoSwapConfigurationTbls()
{
    struct { // Line number of pen_set.csv to read.
        int numOfPageRow;
        int startRow;
    } penSetConfigurationTbl[] = {
        {212, 216}, // input.
        {213, 313}, // output.
        // todo:
    };
    for (auto& penSetConfiguration : penSetConfigurationTbl) {
        QList<QObject*> tempSwapTbl;
        readPenSetIoSwapConfigurationTbl(tempSwapTbl, penSetConfiguration.numOfPageRow, penSetConfiguration.startRow);
        m_ioSwapConfigTbls << tempSwapTbl;
    }
}

void IOForceViewModel::readPenSetIoSwapConfigurationTbl(QList<QObject*>& dataTbl, const int numOfPageRow, const int startRow)
{
    int numOfPage = m_resource->getPenSetIntValue(PENSET_GROUP, numOfPageRow, 0);
    for (int i = 0; i < numOfPage * NUM_OF_SWAP_CONFIG_PAR_PAGE; i++) {
        auto penSetRow = startRow + i;
        auto diAddressStr = m_resource->getPenSetStringValue(PENSET_GROUP, penSetRow, 0);
        auto bankNoStr = m_resource->getPenSetStringValue(PENSET_GROUP, penSetRow, 1);
        if (diAddressStr.isEmpty() || bankNoStr.isEmpty()) {
            continue;
        }
        bool ok;
        auto diAddress = static_cast<quint16>(diAddressStr.toUInt(&ok));
        if (!ok) {
            continue;
        }
        auto bankNo = static_cast<quint16>(bankNoStr.toUInt(&ok, 16));
        if (!ok) {
            continue;
        }
        auto* item = new IOSwapConfiguration(diAddress, bankNo);
//        item->display();  // todo: debug
        dataTbl.append(item);
    }
    // Adjust the size of dataTbl to a multiple of the number of pieces per page.
    if (dataTbl.size() % NUM_OF_SWAP_CONFIG_PAR_PAGE) {
        int remain = NUM_OF_SWAP_CONFIG_PAR_PAGE - (dataTbl.size() % NUM_OF_SWAP_CONFIG_PAR_PAGE);
        while (0 < remain) {
            // append dummy data.
            auto* item = new IOSwapConfiguration();
            dataTbl.append(item);
            remain--;
        }
    }
}

void IOForceViewModel::startModbusPeriodic()
{
    connect(m_communication, &CommunicationThread::finish, this, &IOForceViewModel::onFinished, Qt::QueuedConnection);
}

void IOForceViewModel::stopModbusPeriodic()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &IOForceViewModel::onFinished);
}

void IOForceViewModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (!mIsModbusConnectOneShot) {
        connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
                this, &IOForceViewModel::onOneShotReadFinished, Qt::QueuedConnection);
        mIsModbusConnectOneShot = true;
    }
}

void IOForceViewModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (mIsModbusConnectOneShot){
        disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &IOForceViewModel::onOneShotReadFinished);
        mIsModbusConnectOneShot = false;
    }
}

void IOForceViewModel::writeModbusDo(quint16 dio, uchar writeData)
{
    qDebug("called writeModbusDO doNo:%d anyValue:%d.", dio, writeData);
    m_communication->writeModbusDO(static_cast<int>(dio), true, writeData);
}

void IOForceViewModel::writeModbusHR(quint16 hrAddress, QList<int>& writeData)
{
    qDebug("called writeModbugHR hrAddress:%d range:%d.", hrAddress, writeData.size());
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, writeData.size(), writeData);
    m_communication->setModbusWriteCommand(command);
}

void IOForceViewModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerMonitorIOForcePageModel);
    m_communication->setOneShotModbusCommand(data);
}

void IOForceViewModel::readModbusHR(quint16 hrAddress, quint16 range)
{
    qDebug("called readModbusHR hrAddress:%d range:%d.", hrAddress, range);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, hrAddress, range);
    setOneShotModbusCommand(command);
}

void IOForceViewModel::modbusGetForcedOn(quint16 index, quint16 range)
{
    quint16 hrAddress = FORCED_ON_HR + (index / 16);
    readModbusHR(hrAddress, range);
}

void IOForceViewModel::modbusGetForcedOff(quint16 index, quint16 range)
{
    quint16 hrAddress = FORCED_OFF_HR + (index / 16);
    readModbusHR(hrAddress, range);
}

void IOForceViewModel::modbusGetIOSwap(quint16 hrAddress, quint16 range)
{
    readModbusHR(hrAddress, range * 2);
}

void IOForceViewModel::modbusWriteIOSwap(quint16 hrAddress, quint16 fromBankNo, quint16 toBankNo)
{
    QList<int> writeData;
    writeData << 0 << 0;
    writeData[0] = fromBankNo;
    writeData[1] = toBankNo;
    qDebug("write IO-Swap, hrAddress:%d bankNo(from:%d to:%d).", hrAddress, writeData[0], writeData[1]);
    writeModbusHR(hrAddress, writeData);
}

void IOForceViewModel::updatePeriodicData()
{
    for (auto& data : m_forceIoDataTbl) {
        auto* item = dynamic_cast<ForceIOData*>(data);
        item->setValue(m_modbus->getReceiveDataDI(item->forcedDio()));
        item->setInvers(m_modbus->getReceiveDataDO(item->inversDo()));
    }
}

void IOForceViewModel::updateForcedOn(stModBusCommunicationData &command)
{
    updateStatus(command, FORCED_ON_HR, &ForceIOData::setForcedOn);
}

void IOForceViewModel::updateForcedOff(stModBusCommunicationData &command)
{
    updateStatus(command, FORCED_OFF_HR, &ForceIOData::setForcedOff);
}

void IOForceViewModel::updateStatus(
        stModBusCommunicationData &command,
        quint16 baseHrAddress,
        void(ForceIOData::*setMethod)(bool)
        )
{
    QList<std::bitset<16>> bitsTbl;
    for (int i = 0; i < command.comSize; i++) {
        std::bitset<16> bits(m_modbus->getReceiveDataHR(command.readAddress + i));
        bitsTbl << bits;
    }
    quint16 tblBaseIndex = command.readAddress - baseHrAddress;
    for (auto& item : m_forceIoDataTbl) {
        auto* iodata = dynamic_cast<ForceIOData*>(item);
        auto index = iodata->forcedDio();
        auto tblIndex = (index / 16) - tblBaseIndex;
        if (tblIndex < 0 || bitsTbl.size() <= tblIndex) {
            continue;
        }
        int bitOffset = mConvertIndexToHRBitOffset(index);
        std::bitset<16>& bits = bitsTbl[tblIndex];
        (iodata->*setMethod)(bits[bitOffset]);
    }
}

void IOForceViewModel::updateSwapData(stModBusCommunicationData &command)
{
    if ((command.comSize % 2) != 0) {
        qWarning("%s(%d): command.comSize:%d is not an even number.", __PRETTY_FUNCTION__, __LINE__, command.comSize);
        return;
    }
    for (int i = 0; i < command.comSize; i += 2) {
        auto readAddress = command.readAddress + i;
        auto fromBankNo = m_modbus->getReceiveDataHR(readAddress);
        auto toBankNo = m_modbus->getReceiveDataHR(readAddress + 1);
        auto tblIndex = (readAddress - SWAP_INSTRUCTION_HR) / 2;
        auto* ioSwapData = dynamic_cast<IOSwapData*>(m_ioSwapDataTbl[tblIndex]);
        quint16 prevBankNo;
        if (ioSwapData->setFromBankNo(fromBankNo, prevBankNo)) {
            auto* forceIo = findForceIoData(prevBankNo);
            if (forceIo) {
                forceIo->setSwapDi(-1);
            }
        }
        if (ioSwapData->setToBankNo(toBankNo, prevBankNo)) {
            auto* forceIo = findForceIoData(fromBankNo);
            if (forceIo) {
                auto* ioSwapConfig = findIoSwapConfiguration(toBankNo);
                if (toBankNo == 0) {
                    forceIo->setSwapDi(-1);
                } else if (ioSwapConfig) {
                    forceIo->setSwapDi(ioSwapConfig->diAddress());
                }
            }
        }
    }
    setCanSwapIO(freeIoSwapData() != nullptr);
}

IOSwapData* IOForceViewModel::findIoSwapData(ForceIOData* forceIo)
{
    if (forceIo->bankNo() == 0) {
        return nullptr;
    }
    for (auto& data : m_ioSwapDataTbl) {
        IOSwapData* item = dynamic_cast<IOSwapData*>(data);
        if (item->isSameFromBankNo(forceIo->bankNo())) {
            return item;
        }
    }
    return nullptr;
}

IOSwapData* IOForceViewModel::freeIoSwapData()
{
    for (auto& data : m_ioSwapDataTbl) {
        IOSwapData* item = dynamic_cast<IOSwapData*>(data);
        if (item->unUsed()) {
            return item;
        }
    }
    qDebug("not found free io swap area.");
    return nullptr;
}

ForceIOData* IOForceViewModel::findForceIoData(quint16 bankNo) {
    if (bankNo == 0) {
        return nullptr;
    }
    for (auto& data : m_forceIoDataTbl) {
        auto* item = dynamic_cast<ForceIOData*>(data);
        if (item->bankNo() == bankNo) {
            return item;
        }
    }
    qWarning("not found force io. bankNo:%d.", bankNo);
    return nullptr;
}

IOSwapConfiguration* IOForceViewModel::findIoSwapConfiguration(quint16 bankNo) {
    if (bankNo == 0) {
        return nullptr;
    }
    for (auto& ioSwapTbl : m_ioSwapConfigTbls) {
        for (auto& data : ioSwapTbl) {
            IOSwapConfiguration* item = dynamic_cast<IOSwapConfiguration*>(data);
            if (item->bankNo() == bankNo) {
                return item;
            }
        }
    }
    qWarning("not found io swap configuration. bankNo:%d.", bankNo);
    return nullptr;
}
