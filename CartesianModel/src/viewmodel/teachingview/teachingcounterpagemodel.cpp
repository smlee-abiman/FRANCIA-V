#include "teachingcounterpagemodel.h"
#include "teachingviewdef.h"

TeachingCounterPageModel::TeachingCounterPageModel(ResourceManager *resource, CommunicationThread * comm)
    : PageModelBase(resource, comm)
    , mIsModbusConnectOneShot(false)
{
    init();
}

TeachingCounterPageModel::TeachingCounterPageModel(QObject *object)
    : PageModelBase(nullptr, nullptr)
    , mIsModbusConnectOneShot(false)
{
    Q_UNUSED(object)
}

void TeachingCounterPageModel::init()
{
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::CounterPage));
    setPageLabelColor(m_resource->rgb(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::CounterPage));
    setActivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_COUNTER_PAGE_ACTIVATE_ICON_NAME);
    setDeactivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_COUNTER_PAGE_DEACTIVATE_ICON_NAME);
    // Detect language changes to update labels.
    connect(m_resource, &ResourceManager::languageChanged, this, &TeachingCounterPageModel::onLanguageChanged, Qt::QueuedConnection);

    readPenSetData();
    initcounterMsgIndex();
    for (int i = 0; i < m_numOfCounter; i++) {
        m_currentValueTbl.append(0);
        m_settingValueTbl.append(0);
    }
}

void TeachingCounterPageModel::readPenSetData()
{
    int numOfCounter = m_resource->getPenSetIntValue(PENSET_GROUP, 2, 0);
    setNumOfCounter(numOfCounter);

    m_numericTextColorRGB = m_resource->dataColor(PENSET_GROUP_NUM, 1);
    m_numOfCountersDisplayed = m_resource->getPenSetIntValue(PENSET_GROUP, 2, 0);
    m_currentValueDisplayFormatNo = m_resource->getPenSetIntValue(PENSET_GROUP, 8, 0);
    m_settingValueDisplayFormatNo = m_resource->getPenSetIntValue(PENSET_GROUP, 8, 1);
}

void TeachingCounterPageModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::CounterPage));
}

void TeachingCounterPageModel::onFinished()
{
    updateCurrentValueTbl();
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void TeachingCounterPageModel::onOneShotReadFinished(
        stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller
        )
{
    if (oneShotReadCaller != CommunicationThread::CallerTeachingCounterPageModel){
        qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }

    if ((SETTINGVALUE_HR <= command.readAddress) && (command.readAddress <= SETTINGVALUE_HR_END)) {
        if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
            updateSettingValueTbl(command);
        }
    }
}

void TeachingCounterPageModel::onActivate()
{
    startModbusPeriodic();
    startModbusOneShot();
    requestToReadSettingValues(0, m_settingValueTbl.size());
}

void TeachingCounterPageModel::onDeactivate()
{
    stopModbusOneShot();
    stopModbusPeriodic();
}

void TeachingCounterPageModel::startModbusPeriodic()
{
    connect(m_communication, &CommunicationThread::finish, this, &TeachingCounterPageModel::onFinished, Qt::QueuedConnection);
}

void TeachingCounterPageModel::stopModbusPeriodic()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingCounterPageModel::onFinished);
}

void TeachingCounterPageModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (!mIsModbusConnectOneShot) {
        connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
                this, &TeachingCounterPageModel::onOneShotReadFinished, Qt::QueuedConnection);
        mIsModbusConnectOneShot = true;
    }
}

void TeachingCounterPageModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (mIsModbusConnectOneShot){
        disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &TeachingCounterPageModel::onOneShotReadFinished);
        mIsModbusConnectOneShot = false;
    }
}

void TeachingCounterPageModel::editSettingValue(int index, int value)
{
    if ((index < 0) || (m_settingValueTbl.size() <= index)) {
        qWarning("%s:%d index:%d, value:%d", __func__, __LINE__, index, value);
        return;
    }
    requestToWriteSettingValue(index, value);
    requestToReadSettingValues(index, 1);
}

void TeachingCounterPageModel::initcounterMsgIndex()
{
    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            if(m_resource->getPenSetIntValue(PENSET_GROUP, j+9, i) != 0){
                m_counterMsgIndex.append(m_resource->getPenSetIntValue(PENSET_GROUP, j+9, i));
            }
        }
    }
}

void TeachingCounterPageModel::requestToWriteSettingValue(int index, int value)
{
    QList<int> writeData;
    writeData << 0 << 0;
    writeData[0] = (value & 0x0000FFFF);
    writeData[1] = (value & 0xFFFF0000) >> 16;
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters,
            SETTINGVALUE_HR + (index * 2),
            0x0002,
            writeData
            );
    m_communication->setModbusWriteCommand(command);
    m_operationRecord->recordSetCounter(index, m_settingValueTbl[index], value);
}

void TeachingCounterPageModel::requestToReadSettingValues(uint index, uint range)
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters,
            SETTINGVALUE_HR + (index * 2),
            range * 2
            );
    setOneShotModbusCommand(command);
}

void TeachingCounterPageModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
        CommunicationThread::CallerTeachingCounterPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void TeachingCounterPageModel::updateCurrentValueTbl()
{
    quint16 readAddress = CURRENTVALUE_SR;
    for (int i = 0; i < m_currentValueTbl.size(); i++) {
        int value = m_modbus->getLongReceiveDataRG(readAddress);
        m_currentValueTbl[i] = value;
        readAddress += 2;
    }
    emit currentValueTblChanged(m_currentValueTbl);
}

void TeachingCounterPageModel::updateSettingValueTbl(stModBusCommunicationData &command)
{
    const quint16 endHrAddress = command.readAddress + command.comSize - 1;
    quint16 readAddress = command.readAddress;
    while (readAddress < endHrAddress) {
        int index = (readAddress - SETTINGVALUE_HR) / 2;
        if (index < m_settingValueTbl.size()) {
            int value = m_modbus->getLongReceiveDataHR(readAddress);
            m_settingValueTbl[index] = value;
        }
        readAddress += 2;
    }
    emit settingValueTblChanged(m_settingValueTbl);
}
