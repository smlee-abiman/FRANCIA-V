#include "productionviewmodel.h"

ProductionViewModel::ProductionViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : ViewModelBase(resource, comm)
{
    m_modbusManager = ModbusManager::get_instance();
    if (m_modbusManager){
        //qDebug() << "AutoAxisViewModel:m_modbusManager is share";
    }

    m_fetchTime = 0;
    m_cycleTime = 0;
    m_moldingTime = 0;
    m_oneFetchNum = 0;
    m_fetchNum = 0;
    m_prodNum = 0;
    m_progNum = 0;
    m_badRate = 0;
    m_badCount = 0;
    m_notificationTime = 0;
    m_prodEndYear = 0;
    m_prodEndMonth = 0;
    m_prodEndDay = 0;
    m_prodEndHour = 0;
    m_prodEndMin = 0;
    m_prodEndSec = 0;

    m_lineValuesDay.clear();
    m_lineValuesMonth.clear();

    m_dateTime = QDateTime::currentDateTime();

    m_commonView = commonView;
}

ProductionViewModel::~ProductionViewModel()
{

}

void ProductionViewModel::onActivate()
{
    startMonitoring();
}

void ProductionViewModel::onDeactivate()
{
    stopMonitoring();
}

void ProductionViewModel::startMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    connect(m_communication, &CommunicationThread::finish, this, &ProductionViewModel::onFinished);
    connect(m_modbusManager, &ModbusManager::endModbusPageChange, this, &ProductionViewModel::endModbusPageChange, Qt::DirectConnection);
}
void ProductionViewModel::stopMonitoring()
{
    if (!m_communication){
        qCritical("m_communication is Null");
        return;
    }
    disconnect(m_communication, &CommunicationThread::finish, this, &ProductionViewModel::onFinished);
    disconnect(m_modbusManager, &ModbusManager::endModbusPageChange, this, &ProductionViewModel::endModbusPageChange);
}

void ProductionViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void ProductionViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;

        emit dataLoadedChanged(m_dataLoaded);
    }
}

void ProductionViewModel::setFetchTime(int fetchTime)
{
    if(m_fetchTime != fetchTime){
        m_fetchTime = fetchTime;

        emit fetchTimeChanged(m_fetchTime);
    }
}

void ProductionViewModel::setCycleTime(int cycleTime)
{
    if(m_cycleTime != cycleTime){
        m_cycleTime = cycleTime;

        emit cycleTimeChanged(m_cycleTime);
    }
}

void ProductionViewModel::setMoldingTime(int moldingTime)
{
    if(m_moldingTime != moldingTime){
        m_moldingTime = moldingTime;

        emit moldingTimeChanged(m_moldingTime);
    }
}

void ProductionViewModel::setOneFetchNum(int oneFetchNum)
{
    if(m_oneFetchNum != oneFetchNum){
        m_oneFetchNum = oneFetchNum;

        emit oneFetchNumChanged(m_oneFetchNum);
    }
}

void ProductionViewModel::setFetchNum(int fetchNum)
{
    if(m_fetchNum != fetchNum){
        m_fetchNum = fetchNum;

        emit fetchNumChanged(m_fetchNum);
        emit achievementRateChanged(achievementRate());
    }
}

void ProductionViewModel::setProdNum(int prodNum)
{
    if(m_prodNum != prodNum){
        m_prodNum = prodNum;

        emit prodNumChanged(m_prodNum);
    }
}

void ProductionViewModel::setProgNum(int progNum)
{
    if(m_progNum != progNum){
        m_progNum = progNum;

        emit progNumChanged(m_progNum);
        emit achievementRateChanged(achievementRate());
    }
}

void ProductionViewModel::setBadRate(int badRate)
{
    if(m_badRate != badRate){
        m_badRate = badRate;

        emit badRateChanged(m_badRate);
    }
}

void ProductionViewModel::setBadCount(int badCount)
{
    if(m_badCount != badCount){
        m_badCount = badCount;

        emit badCountChanged(m_badCount);
    }
}

void ProductionViewModel::setNotificationTime(int notificationTime)
{
    if(m_notificationTime != notificationTime){
        m_operationRecord->recordSetNotificationTime(m_notificationTime, notificationTime);
        m_notificationTime = notificationTime;

        emit notificationTimeChanged(m_notificationTime);
    }
}

void ProductionViewModel::setProdEnd(int year, int month, int day, int hour, int min, int sec)
{
    if(m_prodEndYear != year || m_prodEndMonth != month || m_prodEndDay != day || m_prodEndHour != hour || m_prodEndMin != min || m_prodEndSec != sec){
        m_prodEndYear = year;
        m_prodEndMonth = month;
        m_prodEndDay = day;
        m_prodEndHour = hour;
        m_prodEndMin = min;
        m_prodEndSec = sec;

        emit prodEndChanged(prodEnd());
    }
}

void ProductionViewModel::setLineValuesDay(QList<int> lineValues)
{
    if(m_lineValuesDay.count() != lineValues.count()){
        m_lineValuesDay = lineValues;

        emit lineValuesDayChanged(m_lineValuesDay);
    }
    else{
        for(int i = 0; i < m_lineValuesDay.count(); i++){
            if(m_lineValuesDay[i] != lineValues[i]){
                m_lineValuesDay = lineValues;

                emit lineValuesDayChanged(m_lineValuesDay);

                break;
            }
        }
    }
}

void ProductionViewModel::setLineValuesMonth(QList<int> lineValues)
{
    if(m_lineValuesMonth.count() != lineValues.count()){
        m_lineValuesMonth = lineValues;

        emit lineValuesMonthChanged(m_lineValuesMonth);
    }
    else{
        for(int i = 0; i < m_lineValuesMonth.count(); i++){
            if(m_lineValuesMonth[i] != lineValues[i]){
                m_lineValuesMonth = lineValues;

                emit lineValuesMonthChanged(m_lineValuesMonth);

                break;
            }
        }
    }
}

void ProductionViewModel::setLineValuesDayMax(int lineValuesDayMax)
{
    if(m_lineValuesDayMax != lineValuesDayMax){
        m_lineValuesDayMax = lineValuesDayMax;

        emit lineValuesDayMaxChanged(m_lineValuesDayMax);
    }
}

void ProductionViewModel::setLineValuesMonthMax(int lineValuesMonthMax)
{
    if(m_lineValuesMonthMax != lineValuesMonthMax){
        m_lineValuesMonthMax = lineValuesMonthMax;

        emit lineValuesMonthMaxChanged(m_lineValuesMonthMax);
    }
}

QString ProductionViewModel::prodEnd()
{
    QString s_year = QString::number(2000 + m_prodEndYear).rightJustified(4, '0');
    QString s_month = QString::number(m_prodEndMonth).rightJustified(2, '0');
    QString s_day = QString::number(m_prodEndDay).rightJustified(2, '0');
    QString s_hour = QString::number(m_prodEndHour).rightJustified(2, '0');
    QString s_min = QString::number(m_prodEndMin).rightJustified(2, '0');
    QString s_sec = QString::number(m_prodEndSec).rightJustified(2, '0');

    return s_year + "/" + s_month + "/" + s_day + " " + s_hour + ":" + s_min + ":" + s_sec;
}

void ProductionViewModel::writeProgNum(int value)
{
    stModBusCommunicationData writeCommand[1];
    QList<int> writeData;
    writeData.clear();
    short s_high_value = (value >> 16) & 0x0000FFFF;
    short s_low_value = value & 0x0000FFFF;
    writeData.append((int)s_low_value);
    writeData.append((int)s_high_value);


    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, 1955, writeData.length(), writeData);
    m_communication->setOneShotModbusCommand(writeCommand[0]);
    m_operationRecord->recordSetProductionProgNum(m_progNum, value);
}

void ProductionViewModel::writeOneFetchNum(int value)
{
    stModBusCommunicationData writeCommand[1];
    QList<int> writeData;
    writeData.clear();
    writeData.append(value);


    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, 1953, writeData.length(), writeData);

    m_communication->setOneShotModbusCommand(writeCommand[0]);
    m_operationRecord->recordSetOneFetchNum(m_fetchNum, value);
}

void ProductionViewModel::writeNotificationTime(int value)
{
    stModBusCommunicationData writeCommand[1];
    QList<int> writeData;
    writeData.clear();
    writeData.append(value);

    writeCommand[0] = ModbusCommandGenerator::makeModbusWriteCommand(
        QModbusPdu::FunctionCode::WriteMultipleRegisters, 1954, writeData.length(), writeData);

    m_communication->setOneShotModbusCommand(writeCommand[0]);
    m_operationRecord->recordSetOneFetchNum(m_notificationTime, value);
}

void ProductionViewModel::readDayData()
{
    stModBusCommunicationData readCommand[1];


    readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadInputRegisters, 835, 24);

    m_communication->setOneShotModbusCommand(readCommand[0]);
}

void ProductionViewModel::readMonthData()
{
    stModBusCommunicationData readCommand[1];


    readCommand[0] = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadInputRegisters, 859, 32);

    m_communication->setOneShotModbusCommand(readCommand[0]);
}

void ProductionViewModel::readDateTime()
{
#ifdef REAL_TARGET
    m_dateTime = m_commonView->getDateTime();
#else
    m_dateTime = QDateTime::currentDateTime();
#endif
}

QString ProductionViewModel::getHourStr(int sub)
{
    int hour = m_dateTime.time().hour();

    int val = hour - sub;

    if(val < 0){
        return QString::number(24 + val);
    }
    else{
        return QString::number(val);
    }
}

QString ProductionViewModel::getDayStr(int sub)
{
    QDate date = m_dateTime.date();

    int val = date.addDays((sub*-1)).day();

    if(val % 10 == 0 || sub == 0){
        return QString::number(val);
    }
    else{
        return "";
    }
}

void ProductionViewModel::onFinished(){
    if(m_pageChanging == false && m_modbusManager->getIsPageChanging() == false){
        setFetchTime(m_modbusManager->getWordReceiveDataRG(2761));
        setCycleTime(m_modbusManager->getWordReceiveDataRG(2762));
        setMoldingTime(m_modbusManager->getWordReceiveDataRG(2763));
        setOneFetchNum(m_modbusManager->getWordReceiveDataRG(2777));
        setFetchNum(m_modbusManager->getLongReceiveDataRG(2775));
        setProdNum(m_modbusManager->getLongReceiveDataRG(2764));
        setProgNum(m_modbusManager->getLongReceiveDataRG(2779));
        setBadRate(m_modbusManager->getWordReceiveDataRG(2768));
        setBadCount(m_modbusManager->getLongReceiveDataRG(2766));
        setNotificationTime(m_modbusManager->getWordReceiveDataRG(2778));
        setProdEnd(m_modbusManager->getReceiveDataRG(2769), m_modbusManager->getReceiveDataRG(2770), m_modbusManager->getReceiveDataRG(2771), m_modbusManager->getReceiveDataRG(2772), m_modbusManager->getReceiveDataRG(2773), m_modbusManager->getReceiveDataRG(2774));

        QList<int> lineValuesDay;
        int lineValuesDayMax = 0;
        for(int i = 23; i <= 0; i--){
            int value = m_modbusManager->getReceiveDataRG(835+i);
            lineValuesDayMax = qMax(lineValuesDayMax, value);
            lineValuesDay.append(value);
        }
        setLineValuesDay(lineValuesDay);
        setLineValuesDayMax(lineValuesDayMax);

        QList<int> lineValuesMonth;
        int lineValuesMonthMax = 0;
        for(int i = 31; i <= 0; i--){
            int value = m_modbusManager->getReceiveDataRG(859+i);
            lineValuesMonthMax = qMax(lineValuesMonthMax, value);
            lineValuesMonth.append(value);
        }
        setLineValuesMonth(lineValuesMonth);
        setLineValuesMonthMax(lineValuesMonthMax);

        setDataLoaded(true);
    }

    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void ProductionViewModel::endModbusPageChange(int mainScreenId, int subScreenId){
    if(mainScreenId == 21 && subScreenId == 0){
        m_pageChanging = false;
    }
}
