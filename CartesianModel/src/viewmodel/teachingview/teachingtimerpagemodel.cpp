#include "teachingtimerpagemodel.h"
#include "teachingviewdef.h"
#include "dataformat.h"
#include <cmath>

TeachingTimerPageModel::TeachingTimerPageModel(ResourceManager *resource, CommunicationThread * comm)
    : PageModelBase(resource, comm)
{
    init();
}

TeachingTimerPageModel::TeachingTimerPageModel(QObject *object)
    : PageModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

TeachingTimerPageModel::~TeachingTimerPageModel()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingTimerPageModel::onFinished);
    stopModbusOneShot();
}

void TeachingTimerPageModel::onActivate()
{
    startModbusOneShot();
    connect(m_communication, &CommunicationThread::finish, this, &TeachingTimerPageModel::onFinished, Qt::QueuedConnection);
    modbusGetTime(TIMER_TIME_TOP_HR, TIMER_TIME_HR_NUM);
}

void TeachingTimerPageModel::onDeactivate()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingTimerPageModel::onFinished);
    stopModbusOneShot();
}

void TeachingTimerPageModel::startModbusOneShot()
{
    if (!m_communication) return;
    if (!mIsModbusConnectOneShot) {
        connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &TeachingTimerPageModel::onOneShotReadFinished, Qt::QueuedConnection);
        mIsModbusConnectOneShot = true;
    }
}
void TeachingTimerPageModel::stopModbusOneShot()
{
    if (!m_communication) return;
    if (mIsModbusConnectOneShot){
        disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal,
            this, &TeachingTimerPageModel::onOneShotReadFinished);
        mIsModbusConnectOneShot = false;
    }
}
void TeachingTimerPageModel::init()
{
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::TimerPage));
    setPageLabelColor(m_resource->rgb(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::TimerPage));
    setActivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_TIMER_PAGE_ACTIVATE_ICON_NAME);
    setDeactivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_TIMER_PAGE_DEACTIVATE_ICON_NAME);

    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = new TimerData(this);
        m_dataList.append(data);
        TimerAddress *address = new TimerAddress(this);
        m_addressList.append(address);
        m_timerListOrder.append(0);
    }

    readPenSetData();

    // Detect language changes to update labels.
    connect(m_resource, &ResourceManager::languageChanged, this, &TeachingTimerPageModel::onLanguageChanged, Qt::QueuedConnection);
}

void TeachingTimerPageModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::TimerPage));
    updateLanguageInfo();
}

void TeachingTimerPageModel::readPenSetData()
{
    readPenSetAddressData();
    readPenSetFormatData();
}

void TeachingTimerPageModel::readPenSetFormatData()
{
    TimerData *data;
    int row = PEN_SET_TIMER_FORMAT_START_ROW;
    int format = 0;
    int i = 0;
    int timerId = 0;
    TimerData *data1;
    QString markName;
    QString markColor;
    QString msgName;
    QString msgColor;

    while (i < TIMER_LIST_ITEM_NUM) {
        timerId = m_resource->getPenSetIntValue(PEN_SET_GROUP_TIMER_PAGE, row, 0) - 1;
        m_timerListOrder.replace(i, timerId);
        format = m_resource->getPenSetIntValue(PEN_SET_GROUP_TIMER_PAGE, row, 1);
        data = static_cast<TimerData *>(m_dataList.at(timerId));
        data->setFormat(format);
        i++;
        timerId = m_resource->getPenSetIntValue(PEN_SET_GROUP_TIMER_PAGE, row, 2) - 1;
        m_timerListOrder.replace(i, timerId);
        format = m_resource->getPenSetIntValue(PEN_SET_GROUP_TIMER_PAGE, row, 3);
        data = static_cast<TimerData *>(m_dataList.at(timerId));
        data->setFormat(format);
        i++;
        row++;
    }
    for (int j = 0; j < TIMER_LIST_ITEM_NUM; j++) {
        data1 = static_cast<TimerData *>(m_dataList.at(j));
        markName = m_resource->symbolAt(m_addressList.at(j)->upState());
        markColor =  m_resource->rgb(1,m_addressList.at(j)->upState());
        msgName = m_resource->getMessageStringValue("1", m_addressList.at(j)->upState(),m_resource->language());
        msgColor =  m_resource->rgb(1,m_addressList.at(j)->upState());
        int min = m_resource->getPenSetIntValue("13", j+63, 3);
        int max = m_resource->getPenSetIntValue("13", j+63, 4);
        data1->setMarklabelData(markName,markColor);
        data1->setMsglabelData(msgName,msgColor);
        data1->setMin(QString::number(min));
        data1->setMax(QString::number(max));
    }
}

void TeachingTimerPageModel::readPenSetAddressData()
{
    TimerAddress *address;
    int row = PEN_SET_TIMER_ADDRESS_START_ROW;
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_TIMER_PAGE, row);
        address = m_addressList.at(i);
        address->setTime(static_cast<quint16>(rowData.at(0)));
        address->setStartState(static_cast<quint16>(rowData.at(1)));
        address->setUpState(static_cast<quint16>(rowData.at(2)));
        row++;
    }
}

int TeachingTimerPageModel::getIntegerDigits(int timerIndex)
{
    if ((timerIndex <= -1) || (timerIndex >= m_dataList.length())) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid argument." << timerIndex;
        return -1;
    }

    int format = static_cast<TimerData *>(m_dataList.at(timerIndex))->format();
    DataFormat df = DataFormat::fromCode(format);
    return df.real;
}

QString TeachingTimerPageModel::getUnit(int timerIndex)
{
    if ((timerIndex <= -1) || (timerIndex >= m_dataList.length())) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid argument." << timerIndex;
        return "";
    }

    int format = static_cast<TimerData *>(m_dataList.at(timerIndex))->format();
    DataFormat df = DataFormat::fromCode(format);
    return m_resource->getUnitFromFormat(df.unit);
}

int TeachingTimerPageModel::setTime(int timerIndex, double time)
{
    if ((timerIndex <= -1) || (timerIndex >= m_addressList.length())) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid argument." << timerIndex << time;
        return -1;
    }

    double d_value = time * TIMER_TIME_MAGNIFICATION;
    long correct_value = qMin(lround(d_value), static_cast<long>(TIMER_MAX_TIME));
    TimerAddress *address = m_addressList.at(timerIndex);
    modbusSaveTime(address->time(), correct_value);
    int prevTime = static_cast<TimerData *>(m_dataList.at(timerIndex))->time();
    m_operationRecord->recordSetTimer(timerIndex, prevTime, correct_value);
    modbusGetTime(address->time(), 1);
    return 0;
}

QString TeachingTimerPageModel::formatSpeed(int timerIndex, int time)
{
    if ((timerIndex <= -1) || (timerIndex >= m_dataList.length())) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid argument." << timerIndex << time;
        return "";
    }

    int format = static_cast<TimerData *>(m_dataList.at(timerIndex))->format();
    DataFormat df = DataFormat::fromCode(format);
    return formatValue(time, df, 2);
}

void TeachingTimerPageModel::modbusGetTime(quint16 hrAddress, quint16 size)
{
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusReadCommand(
            QModbusPdu::FunctionCode::ReadHoldingRegisters, hrAddress, size);
    setOneShotModbusCommand(command);
}

void TeachingTimerPageModel::modbusSaveTime(quint16 hrAddress, int time)
{
    QList<int> writeData;
    writeData.append(time);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, hrAddress, 0x0001, writeData);
    m_communication->setModbusWriteCommand(command);
}

void TeachingTimerPageModel::setOneShotModbusCommand(stModBusCommunicationData &data)
{
    m_communication->setOneShotModbusCaller(
                CommunicationThread::CallerTeachingTimerPageModel);
    m_communication->setOneShotModbusCommand(data);
}

void TeachingTimerPageModel::onFinished()
{
    for (int i = 0; i < m_addressList.length(); i++) {
        TimerAddress *address = m_addressList.at(i);
        TimerData *data = static_cast<TimerData *>(m_dataList.at(i));
        data->setStartState(m_modbus->getReceiveDataDI(address->startState()));
        data->setUpState(m_modbus->getReceiveDataDI(address->upState()));
    }
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void TeachingTimerPageModel::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerTeachingTimerPageModel){
        qDebug("%s:%d oneShotReadCaller[%d]", __func__, __LINE__, oneShotReadCaller);
        return;
    }
    if (command.readAddress >= TIMER_TIME_TOP_HR && command.readAddress <= TIMER_TIME_BOTTOM_HR) {
        if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
            updateTimeData(command.readAddress, command.comSize);
        }
    }
}

void TeachingTimerPageModel::updateTimeData(quint16 startHrAddress, quint16 size)
{
    quint16 endHrAddress = startHrAddress + size - 1;
    for (int i = 0; i < m_addressList.length(); i++) {
        quint16 timeAddress = m_addressList.at(i)->time();
        if ((timeAddress >= startHrAddress) && (timeAddress <= endHrAddress)) {
            int time = m_modbus->getReceiveDataHR(timeAddress);
            TimerData *data = static_cast<TimerData *>(m_dataList.at(i));
            data->setTime(time);
        }
    }
}

void TeachingTimerPageModel::updateLanguageInfo()
{
    TimerData *data1;
    QString msgName;
    QString msgColor;
    for (int j = 0; j < TIMER_LIST_ITEM_NUM; j++) {
        data1 = static_cast<TimerData *>(m_dataList.at(j));
        msgName = m_resource->getMessageStringValue("1", m_addressList.at(j)->upState(),m_resource->language());
        msgColor =  m_resource->rgb(1,m_addressList.at(j)->upState());
        data1->setMsglabelData(msgName,msgColor);
    }
}
