#include "alarmhistoryviewmodel.h"
#include "alarmhistorydataparser.h"
#include "alarmhistorydata.h"
#include "enums.h"
#include <QTextCodec>

const QString ALARM_HISTORY_CSV_FILE_NAME = "ALMHIS.csv";
const QString ALARM_HISTORY_DAT_FILE_NAME = "ALMHIS.dat";
const int ALARM_HISTORY_MAX_DATA_NUM = 1000;

AlarmHistoryViewModel::AlarmHistoryViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : HistoryView(resource, comm)
{
    m_parser = new AlarmHistoryDataParser();
    m_commonView = commonView;
    int ret = readAlarmDataFileFromPendant();
    if (ret == 0) {
        setIsFtpFileAcquired(true);
    }
    initAxisLabelIndexList();
    if (m_isFtpFileAcquired) {
        connect(m_commonView, SIGNAL(alarmOccurred(const AlarmInfo &, const QDateTime &)), this, SLOT(onAlarmOccurred(const AlarmInfo &, const QDateTime &)));
    } else {
#ifndef REAL_TARGET
        QTimer::singleShot(0, this, &AlarmHistoryViewModel::onPseudoFtpGetFinished);
#endif
        connect(m_ftpManager, SIGNAL(ftpFileInfoChanged(int)), this, SLOT(onFtpFileInfoChanged(int)), Qt::DirectConnection);
    }
}

AlarmHistoryViewModel::AlarmHistoryViewModel(QObject *parent)
    : HistoryView(nullptr, nullptr)
{
    Q_UNUSED(parent)
}

AlarmHistoryViewModel::~AlarmHistoryViewModel()
{
    m_alarmHistoryFile.close();
    disconnect(m_commonView, &CommonViewModel::alarmOccurred, this, &AlarmHistoryViewModel::onAlarmOccurred);
    delete m_parser;
}

void AlarmHistoryViewModel::onActivate()
{
    startUsbMonitoring();
    connect(m_communication, &CommunicationThread::finish, this, &AlarmHistoryViewModel::onFinished);
}

void AlarmHistoryViewModel::onDeactivate()
{
    stopUsbMonitoring();
    disconnect(m_communication, &CommunicationThread::finish, this, &AlarmHistoryViewModel::onFinished);
}

void AlarmHistoryViewModel::getFtpFileData()
{
    if (!m_ftpManager) {
        qDebug("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    // Parse the alarm history data in the controller.
    m_parser->parseAlarmHistory(m_ftpManager->getFtpFilePath(FtpManager::FtpFileInfo::FTP_ALM_HIS1_DAT), m_alarmList);
    m_parser->parseAlarmHistory(m_ftpManager->getFtpFilePath(FtpManager::FtpFileInfo::FTP_ALM_HIS2_DAT), m_alarmList);
    // Sort by date, oldest first.
    std::sort(m_alarmList.begin(), m_alarmList.end(), [] (AlarmHistoryData *left, AlarmHistoryData *right) {
        if (left->dateTime() == right->dateTime()) {
            return left->unitNo() < right->unitNo();
        }
        return left->dateTime() < right->dateTime();
    });
    // Delete duplicate data.
    m_alarmList.erase( std::unique(m_alarmList.begin(), m_alarmList.end(), [](AlarmHistoryData *d1, AlarmHistoryData *d2) -> bool { return *d1 == *d2; } ), m_alarmList.end() );
    // Delete data that exceeds the maximum number of histories.
    while (m_alarmList.length() > ALARM_HISTORY_MAX_DATA_NUM)
        m_alarmList.removeFirst();

    m_axisAlarmList.clear();
    m_systemAlarmList.clear();
    m_normalAlarmList.clear();

    // Sort the alarm history list into axis alarm history, system alarm history, and normal alarm history lists.
    divideAlarmList();
    emit normalAlarmListChanged();
    emit systemAlarmListChanged();
    emit axisAlarmListChanged();

    QString filePath = AppEnvironment::getFtpFolderPath() + "/" + ALARM_HISTORY_DAT_FILE_NAME;
    m_alarmHistoryFile.setFileName(filePath);
    if (m_alarmHistoryFile.open(QIODevice::WriteOnly)) {
        writeAllAlarmDataToFile();
    } else {
        qDebug() << __func__ << "ALMHIS.dat open failed." << filePath;
    }
}

void AlarmHistoryViewModel::divideAlarmList()
{
    for (auto *data : m_alarmList)
        divideAlarmData(data);
}

void AlarmHistoryViewModel::divideAlarmData(AlarmHistoryData *data)
{
    if (data->isAxisAlarm()) {
        m_axisAlarmList.insert(0, data);
    } else if (data->isSystemAlarm()) {
        m_systemAlarmList.insert(0, data);
    } else if (data->isNormalAlarm()) {
        m_normalAlarmList.insert(0, data);
    }
}

void AlarmHistoryViewModel::removeOldestAlarmData()
{
    while (m_alarmList.length() > ALARM_HISTORY_MAX_DATA_NUM) {
        auto removeData = m_alarmList.takeFirst();
        QList<QObject *>* list = nullptr;
        if (removeData->isAxisAlarm()) {
            list = &m_axisAlarmList;
        } else if (removeData->isSystemAlarm()) {
            list = &m_systemAlarmList;
        } else if (removeData->isNormalAlarm()) {
            list = &m_normalAlarmList;
        }
        if (list != nullptr) {
            int index = list->indexOf(removeData);
            list->takeAt(index);
        }
        delete removeData;
    }
}

bool AlarmHistoryViewModel::exportAlarmDataToCsvFile(const QString &filePath)
{
    QFile file(filePath);
    if (!isUSBConnected() || !file.open(QIODevice::WriteOnly)) {
        qDebug() << __func__ << "failed." << filePath << isUSBConnected();
        return false;
    }
    QTextStream out(&file);
    out.setGenerateByteOrderMark(true);
    out.setCodec(QTextCodec::codecForMib(1015));
    writeHeaderForCsv(out);
    writeAlarmDataForCsv(out);
    out.flush();
    file.close();
#ifdef REAL_TARGET
    int ret = system("sync");
    (void)ret;
#endif // REAL_TARGET
    m_operationRecord->recordOpeExportAlarmHistory();
    return true;
}

void AlarmHistoryViewModel::writeAlarmDataForCsv(QTextStream &out)
{
    int number = 1;
    for (auto it = m_alarmList.rbegin(), end = m_alarmList.rend(); it != end; ++it) {
        auto alarmData = *it;
        out << number++;
        out << "\t";
        out << alarmData->dateTime().toString("yyyy/MM/dd hh:mm:ss");
        out << "\t";
        out << getAlarmDetailString(alarmData);
        out << "\n";
    }
}

QString AlarmHistoryViewModel::getAlarmDetailString(AlarmHistoryData *alarmData)
{
    int alarmNoOffset = 0;
    if (alarmData->isNormalAlarm()) {
        alarmNoOffset = 0;
    } else if (alarmData->isAxisAlarm()) {
        alarmNoOffset = 1040;
    } else if (alarmData->isSystemAlarm()) {
        alarmNoOffset = 510;
    }
    int alarmNo = alarmNoOffset + alarmData->alarmNo() * 2 - 1;
    return m_resource->getMessageStringValue("12", alarmNo, m_resource->language());
}

void AlarmHistoryViewModel::setNormalAlarmList(QList<QObject *> &list)
{
    if (m_normalAlarmList != list) {
        m_normalAlarmList = list;
        emit normalAlarmListChanged();
    }
}

void AlarmHistoryViewModel::setSystemAlarmList(QList<QObject *> &list)
{
    if (m_systemAlarmList != list) {
        m_systemAlarmList = list;
        emit systemAlarmListChanged();
    }
}

void AlarmHistoryViewModel::setAxisAlarmList(QList<QObject *> &list)
{
    if (m_axisAlarmList != list) {
        m_axisAlarmList = list;
        emit axisAlarmListChanged();
    }
}

void AlarmHistoryViewModel::onAlarmOccurred(const AlarmInfo &alarmInfo, const QDateTime &dateTime)
{
    //qDebug() << __func__ << alarmInfo.type() << dateTime.toString("yyyy/MM/dd hh:mm:ss");
    auto alarmData = m_parser->parseAlarmInfo(alarmInfo, dateTime);
    if (alarmData != nullptr) {
        m_alarmList.append(alarmData);
        divideAlarmData(alarmData);
        if (m_alarmList.length() > ALARM_HISTORY_MAX_DATA_NUM) {
            removeOldestAlarmData();
            writeAllAlarmDataToFile();
            emit normalAlarmListChanged();
            emit systemAlarmListChanged();
            emit axisAlarmListChanged();
        } else {
            writeAlarmDataToFile(alarmData);
            if (alarmData->isNormalAlarm()) {
                emit normalAlarmListChanged();
            } else if (alarmData->isSystemAlarm()) {
                emit systemAlarmListChanged();
            } else if (alarmData->isAxisAlarm()) {
                emit axisAlarmListChanged();
            }
        }
    }
}

void AlarmHistoryViewModel::writeAllAlarmDataToFile()
{
    if (m_alarmHistoryFile.handle() < 0) {
        qDebug() << __func__ << "ALMHIS.dat is not open.";
        return;
    }
    m_alarmHistoryFile.resize(0);
    for (auto alarmData: m_alarmList) {
        //qDebug() << __func__ << alarmData->dateTime().toString("yyyy/MM/dd hh:mm:ss") << alarmData->unitNo() << alarmData->alarmNo();
        m_alarmHistoryFile.write(alarmData->toData());
    }
    m_alarmHistoryFile.flush();
}

void AlarmHistoryViewModel::writeAlarmDataToFile(AlarmHistoryData *alarmData)
{
    if (m_alarmHistoryFile.handle() < 0) {
        qDebug() << __func__ << "ALMHIS.dat is not open.";
        return;
    }
    //qDebug() << __func__ << alarmData->dateTime().toString("yyyy/MM/dd hh:mm:ss") << alarmData->unitNo() << alarmData->alarmNo();
    m_alarmHistoryFile.write(alarmData->toData());
    m_alarmHistoryFile.flush();
}

int AlarmHistoryViewModel::readAlarmDataFileFromPendant()
{
    QString filePath = AppEnvironment::getFtpFolderPath() + "/" + ALARM_HISTORY_DAT_FILE_NAME;
    // Parse the alarm history data in the pendant.
    int ret = m_parser->parseAlarmHistory(filePath, m_alarmList);
    if (ret != 0) {
        return ret;
    }

    // Sort by date, oldest first.
    std::sort(m_alarmList.begin(), m_alarmList.end(), [] (AlarmHistoryData *left, AlarmHistoryData *right) {
        if (left->dateTime() == right->dateTime()) {
            return left->unitNo() < right->unitNo();
        }
        return left->dateTime() < right->dateTime();
    });
    // Delete duplicate data.
    m_alarmList.erase( std::unique(m_alarmList.begin(), m_alarmList.end(), [](AlarmHistoryData *d1, AlarmHistoryData *d2) -> bool { return *d1 == *d2; } ), m_alarmList.end() );
    // Sort the alarm history list into axis alarm history, system alarm history, and normal alarm history lists.
    divideAlarmList();
    emit normalAlarmListChanged();
    emit systemAlarmListChanged();
    emit axisAlarmListChanged();
    // Write the data in m_alarmList to the alarm history file in the pendant.
    m_alarmHistoryFile.setFileName(filePath);
    if (m_alarmHistoryFile.open(QIODevice::WriteOnly)) {
        writeAllAlarmDataToFile();
    } else {
        qDebug() << __func__ << "ALMHIS.dat open failed." << filePath;
    }
    return 0;
}

QString AlarmHistoryViewModel::getAxisName(int unitNo)
{
    QString ret("");
    if (unitNo >= 0 && unitNo < m_axisLabelIndexList.length()) {
        int labelIndex = m_axisLabelIndexList.at(unitNo);
        if (labelIndex != -1)
            ret = m_resource->label(22, labelIndex);
    }
    return ret;
}

void AlarmHistoryViewModel::initAxisLabelIndexList()
{
    const QList<int> LABEL_INDEX_LIST = {
        0,  // Y軸
        1,  // MX軸
        3,  // MZ軸
        2,  // SX軸
        4,  // SZ軸
        5,  // 軸6
        6,  // 軸7
        7   // 軸8
    };

    QVector<int> axisOrderVector = m_resource->getIntRowArray("1", 15);
    for(int i = 0; i < axisOrderVector.size(); i++) {
        int axisNo = axisOrderVector.at(i);
        if ((axisNo <= 0) || (axisNo > Enums::AxisNum))
            continue;
        m_axisLabelIndexList.replace(axisNo - 1, LABEL_INDEX_LIST.at(i));
    }
}

int AlarmHistoryViewModel::onExportDataProc()
{
    QString path = historyDirPath() + ALARM_HISTORY_CSV_FILE_NAME;
    bool ret = exportAlarmDataToCsvFile(path);
    return ret ? 0 : -1;
}

void AlarmHistoryViewModel::onFtpFileInfoChanged(int ftpFileInfo)
{
    if (ftpFileInfo != FtpManager::FtpFileInfo::FTP_ALM_HIS2_DAT){
        return;
    }
    getFtpFileData();
    setIsFtpFileAcquired(true);
    disconnect(m_ftpManager, SIGNAL(ftpFileInfoChanged(int)), this, SLOT(onFtpFileInfoChanged(int)));
    connect(m_commonView, SIGNAL(alarmOccurred(const AlarmInfo &, const QDateTime &)), this, SLOT(onAlarmOccurred(const AlarmInfo &, const QDateTime &)));
}

void AlarmHistoryViewModel::onPseudoFtpGetFinished()
{
    onFtpFileInfoChanged(FtpManager::FtpFileInfo::FTP_ALM_HIS2_DAT);
}

void AlarmHistoryViewModel::onFinished()
{
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void AlarmHistoryViewModel::setIsFtpFileAcquired(bool acquired)
{
    if (m_isFtpFileAcquired != acquired) {
        m_isFtpFileAcquired = acquired;
        emit isFtpFileAcquiredChanged();
    }
}
