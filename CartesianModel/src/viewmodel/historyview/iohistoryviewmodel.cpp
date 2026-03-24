#include "iohistoryviewmodel.h"
#include "iohistorydata.h"
#include "iostatus.h"
#include "iohistorydataparser.h"
#include <bitset>
#include <QTextCodec>
#include <QFileInfo>

const QString IO_HISTORY_CSV_FILE_NAME = "Iobank_his.CSV";
const QString IO_HISTORY_DAT_FILE_NAME = "IOHIS.DAT";

IoHistoryViewModel::IoHistoryViewModel(ResourceManager *resource, CommunicationThread * comm)
    : HistoryView(resource, comm)
{
    m_parser = new IoHistoryDataParser();
    readPensetData();
}

IoHistoryViewModel::IoHistoryViewModel(QObject *parent)
    : HistoryView(nullptr, nullptr)
{
    Q_UNUSED(parent)
}

IoHistoryViewModel::~IoHistoryViewModel()
{
    delete m_parser;
}

void IoHistoryViewModel::onActivate()
{
    setIsFtpFileAcquired(false);
    startFtpMonitoring();
    startUsbMonitoring();
#ifdef REAL_TARGET
    if (m_ftpManager->isStartupFtpGetFileList()) {
    getFtpFileInfo(FtpManager::FtpFileInfo::FTP_IO_HIS_DAT);
    } else {
        // Wait for completion of FTP data acquisition at startup.
        connect(m_ftpManager, SIGNAL(isStartupFtpGetFileListChanged(bool)), this, SLOT(onIsStartupFtpGetFileListChanged(bool)), Qt::DirectConnection);
    }
#else
    QTimer::singleShot(0, this, &IoHistoryViewModel::onPseudoFtpGetFinished);
#endif
    m_ftpGetTimeoutTimerId = startTimer(60000);
    connect(m_communication, &CommunicationThread::finish, this, &IoHistoryViewModel::onFinished);
}

void IoHistoryViewModel::onDeactivate()
{
    stopFtpMonitoring();
    stopUsbMonitoring();
    disconnect(m_communication, &CommunicationThread::finish, this, &IoHistoryViewModel::onFinished);
}

void IoHistoryViewModel::setIoList(QList<QObject *> &list)
{
    if(m_ioList != list){
        m_ioList = list;
        emit ioListChanged();
    }
}

void IoHistoryViewModel::readPensetData()
{
    m_bankDataType.clear();
    const int numBanks = 10;
    const int numRows = 16;
    const int startRow = 16;
    const int endRow = startRow + numRows;
    int index = -1;
    for (int i = 0; i < numBanks; i++) {
        for(int j = startRow; j < endRow; j++) {
            for (int k = 0; k < 2; k++) {
                index = m_resource->getPenSetIntValue("25", j, i * 2 + k);
                m_bankDataType.append(index);
            }
        }
    }
    emit bankDataTypeChanged();

    readPensetDisplayIoAddressList();
}

void IoHistoryViewModel::readPensetDisplayIoAddressList()
{
    const int numIoItem = 198;
    int startRow = 11;
    int endRow = startRow + numIoItem;
    int ioAddress;
    bool ok;
    for(int i = startRow; i < endRow; i++){
        QString indexStr = m_resource->getPenSetStringValue("8", i, 0);
        ioAddress = indexStr.toInt(&ok);
        if (ok){
            m_displayIoAddressList.append(ioAddress);
        }
    }
    emit displayIoAddressListChanged();
}

bool IoHistoryViewModel::isIoStatusOn(int ioHistoryIndex, int diAddress)
{
    if (ioHistoryIndex >= m_ioHistoryDataList.length()) {
        return false;
    }
    const int IO_NUM_PER_BANK = 256;
    int bankIndex = diAddress / IO_NUM_PER_BANK;
    int ioIndex = diAddress % IO_NUM_PER_BANK;
    auto ioHistoryData = static_cast<IoHistoryData *>(m_ioHistoryDataList.at(ioHistoryIndex));
    return ioHistoryData->getIoStatus(bankIndex, ioIndex);
}

QList<int> IoHistoryViewModel::getDisplayIoAddressList(int page, int numItems)
{
    QList<int> list;

    int startIndex = page * numItems;
    int endIndex = startIndex + numItems;

    if (endIndex >= m_displayIoAddressList.length()) {
        endIndex = m_displayIoAddressList.length() - 1;
    }

    for(int i = startIndex; i <= endIndex; i++){
        list.append(m_displayIoAddressList.at(i));
    }


    while (list.length() < numItems) {
        list.append(-1);
    }

    return list;
}

QString IoHistoryViewModel::toIoChangeString(int index)
{
    auto ioHistoryData = static_cast<IoHistoryData *>(m_ioHistoryDataList.at(index));
    if (!ioHistoryData->ioChangeListInitialized()) {
        bool isLastIndex = (index == m_ioHistoryDataList.length() - 1);
        if (isLastIndex) {
            createIoOnList(index);
        } else {
            createIoChangeList(index);
        }
    }

    QString ret("");
    QList<IoData> ioChangeDataList = ioHistoryData->ioChangeList();
    for (IoData data : ioChangeDataList) {
        if (data.type() == IoData::Type::Bit) {
            ret += QString("%1 %2,").arg(m_resource->symbolAt(data.ioAddress())).arg(data.value() == 0 ? "OFF" : "ON");
        } else {
            ret += QString("%1=%2,").arg(m_resource->symbolAt(data.ioAddress())).arg(data.value());
        }
    }
    ret.chop(1);
    return ret;
}

void IoHistoryViewModel::createIoOnList(int index)
{
    auto ioHistoryData = static_cast<IoHistoryData *>(m_ioHistoryDataList.at(index));
    auto ioStatusList = ioHistoryData->ioStatusList();
    QList<IoData> ioChangeDataList;
    int len = ioStatusList.length();
    for (int i = 0; i < len; i++) {
        auto statusList = static_cast<IoStatus *>(ioStatusList.at(i))->status();
        for (int j = 0; j < statusList.length(); j++) {
            int status = statusList.at(j);
            if (status != 0) {
                if (m_bankDataType.at(32 * i + j)) {
                    int ioAddress = 256 * i + j * 8;
                    IoData data(IoData::Type::Byte, ioAddress, status);
                    ioChangeDataList.append(data);
                } else {
                    std::bitset<8> bits(status);
                    for (int k = 0; k < 8; k++) {
                        if (bits[k] == 1) {
                            int ioAddress = 256 * i + j * 8 + k;
                            IoData data(IoData::Type::Bit, ioAddress, bits[k]);
                            ioChangeDataList.append(data);
                        }
                    }
                }
            }
        }
    }
    ioHistoryData->setIoChangeList(ioChangeDataList);
}

void IoHistoryViewModel::createIoChangeList(int index)
{
    auto currentIoHistoryData = static_cast<IoHistoryData *>(m_ioHistoryDataList.at(index));
    auto prevIoHistoryData = static_cast<IoHistoryData *>(m_ioHistoryDataList.at(index + 1));
    auto currentIoStatusList = currentIoHistoryData->ioStatusList();
    auto prevIoStatusList = prevIoHistoryData->ioStatusList();
    QList<IoData> ioChangeDataList;
    int len = prevIoStatusList.length();
    for (int i = 0; i < len; i++) {
        auto currentStatusList = static_cast<IoStatus *>(currentIoStatusList.at(i))->status();
        auto prevStatusList = static_cast<IoStatus *>(prevIoStatusList.at(i))->status();
        for (int j = 0; j < currentStatusList.length(); j++) {
            int currentStatus = currentStatusList.at(j);
            int prevStatus = prevStatusList.at(j);
            if (currentStatus != prevStatus) {
                if (m_bankDataType.at(32 * i + j)) {
                    int ioAddress = 256 * i + j * 8;
                    IoData data(IoData::Type::Byte, ioAddress, currentStatus);
                    ioChangeDataList.append(data);
                } else {
                    std::bitset<8> currentBits(currentStatus);
                    std::bitset<8> prevBits(prevStatus);
                    for (int k = 0; k < 8; k++) {
                        if (currentBits[k] != prevBits[k]) {
                            int ioAddress = 256 * i + j * 8 + k;
                            IoData data(IoData::Type::Bit, ioAddress, currentBits[k]);
                            ioChangeDataList.append(data);
                        }
                    }
                }
            }
        }
    }
    currentIoHistoryData->setIoChangeList(ioChangeDataList);
}

void IoHistoryViewModel::getFtpFileData()
{
    if (!m_ftpManager) {
        qDebug("%s:%d mFtpManager is not share", __func__, __LINE__);
        return;
    }
    int len = m_ioHistoryDataList.length();
    m_parser->parseIoHistory(m_ftpManager->getFtpFilePath(FtpManager::FtpFileInfo::FTP_IO_HIS_DAT), m_ioHistoryDataList);
    for (int i = 0; i < len; i++) {
        m_ioHistoryDataList.removeFirst();
    }
    std::sort(m_ioHistoryDataList.begin(), m_ioHistoryDataList.end(), [] (QObject *left, QObject *right) {
        auto l = static_cast<IoHistoryData *>(left);
        auto r = static_cast<IoHistoryData *>(right);
        if (l->dateTime() == r->dateTime()) {
            return l->id() < r->id();
        } else {
            return l->dateTime() > r->dateTime();
        }
    });

    emit ioHistoryDataListChanged();
    setIsFtpFileAcquired(true);
    killTimer(m_ftpGetTimeoutTimerId);
    m_ftpGetTimeoutTimerId = -1;
}

void IoHistoryViewModel::ftpGetFinished(const QString &filePath)
{
    if (QFileInfo(filePath).fileName().compare(IO_HISTORY_DAT_FILE_NAME, Qt::CaseInsensitive) == 0 ) {
        getFtpFileData();
    }
}

bool IoHistoryViewModel::exportIoDataToCsvFile(const QString &filePath)
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
    writeIoDataForCsv(out);
    out.flush();
    file.close();
#ifdef REAL_TARGET
    int ret = system("sync");
    (void)ret;
#endif // REAL_TARGET
    m_operationRecord->recordOpeExportIoHistory();
    return true;
}

void IoHistoryViewModel::writeIoDataForCsv(QTextStream &out)
{
    int number = 1;
    int len = m_ioHistoryDataList.length();
    for (int i = 0; i < len; i++) {
        auto ioData = static_cast<IoHistoryData *>(m_ioHistoryDataList.at(i));
        out << number++;
        out << "\t";
        out << ioData->dateTime().toString("yyyy/MM/dd hh:mm:ss");
        out << "\t";
        out << toIoChangeString(i);
        out << "\n";
    }
}

int IoHistoryViewModel::onExportDataProc()
{
    QString path = historyDirPath() + IO_HISTORY_CSV_FILE_NAME;
    bool ret = exportIoDataToCsvFile(path);
    return ret ? 0 : -1;
}

void IoHistoryViewModel::onPseudoFtpGetFinished()
{
    getFtpFileData();
}

void IoHistoryViewModel::onFinished()
{
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void IoHistoryViewModel::setIsFtpFileAcquired(bool acquired)
{
    if (m_isFtpFileAcquired != acquired) {
        m_isFtpFileAcquired = acquired;
        emit isFtpFileAcquiredChanged();
    }
}

void IoHistoryViewModel::timerEvent(QTimerEvent *timerEvent)
{
    HistoryView::timerEvent(timerEvent);
    if (timerEvent->timerId() == m_ftpGetTimeoutTimerId) {
        qWarning() << "IO history data acquisition timed out.";
        setIsFtpFileAcquired(true);
        killTimer(m_ftpGetTimeoutTimerId);
        m_ftpGetTimeoutTimerId = -1;
    }
}

void IoHistoryViewModel::onIsStartupFtpGetFileListChanged(bool isStartupFtpGetFileListChanged)
{
    if (isStartupFtpGetFileListChanged) {
        if (isViewActive()) {
            getFtpFileInfo(FtpManager::FtpFileInfo::FTP_IO_HIS_DAT);
        }
        disconnect(m_ftpManager, SIGNAL(isStartupFtpGetFileListChanged(bool)), this, SLOT(onIsStartupFtpGetFileListChanged(bool)));
    }
}

