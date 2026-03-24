#include "operationhistorylist.h"
#include "appenvironment.h"
#include <QFile>
#include <QDataStream>
#include "viewmodel/historyview/operationhistorydata.h"
#include <QDebug>

const QString OPERATION_HISTORY_DAT_FILE_NAME = "OPE2HIS.DAT";
const int OPERATION_HISTORY_DATA_RECORD_SIZE = 24;
const int OPERATION_HISTORY_MAX_DATA_NUM = 200;
const int OPERATION_HISTORY_MAX_DATA_SIZE = OPERATION_HISTORY_MAX_DATA_NUM * OPERATION_HISTORY_DATA_RECORD_SIZE;

OperationHistoryList::OperationHistoryList(QObject *parent) : QObject(parent)
{
    init();
}

void OperationHistoryList::init()
{
    QString filePath = AppEnvironment::getFtpFolderPath() + "/" + OPERATION_HISTORY_DAT_FILE_NAME;
    m_operationHistoryFile.setFileName(filePath);
    if (m_operationHistoryFile.open(QIODevice::ReadWrite)) {
        readData();
    }
}

int OperationHistoryList::readData()
{
    if (!m_operationHistoryFile.isOpen()) {
        return -1;
    }
    QByteArray arr = m_operationHistoryFile.readAll();
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int sec;
    QDate date;
    QTime time;
    QDateTime dateTime;
    QDateTime latestDateTime;
    int remain = arr.size();
    int i = 0;
    while (remain >= OPERATION_HISTORY_DATA_RECORD_SIZE) {
        year = 2000 + arr.at(i);
        month = arr.at(i + 1);
        day = arr.at(i + 2);
        hour = arr.at(i + 3);
        minute = arr.at(i + 4);
        sec = arr.at(i + 5);
        QByteArray array = arr.mid(i + 6, 18);

        remain -= OPERATION_HISTORY_DATA_RECORD_SIZE;
        i += OPERATION_HISTORY_DATA_RECORD_SIZE;

        date.setDate(year, month, day);
        time.setHMS(hour, minute, sec);
        dateTime.setDate(date);
        dateTime.setTime(time);
        if (!dateTime.isValid()) {
            qWarning() << __func__ << "Invalid date:" << year << month << day << hour << minute << sec;
            continue;
        }
        OperationHistoryData *data = new OperationHistoryData();
        if (data != nullptr) {
            data->setDateTime(dateTime);
            data->setData(array);
            if (isValid(data)) {
                m_operationList.append(data);
            } else {
                delete data;
                continue;
            }
        }

        if (latestDateTime <= dateTime) {
            latestDateTime = dateTime;
            m_offset = i;
        }
    }
    std::sort(m_operationList.begin(), m_operationList.end(), [] (QObject *left, QObject *right) {
        return static_cast<OperationHistoryData *>(left)->dateTime() > static_cast<OperationHistoryData *>(right)->dateTime();
    });
    emit operationListChanged();
    return 0;
}

int OperationHistoryList::add(OperationHistoryData *data)
{
    if (data == nullptr)
        return -1;
    if (!data->dateTime().isValid())
        return -1;
    if (!isValid(data))
        return -1;

    m_operationList.push_front(data);
    emit operationListChanged();
    writeData(data);
    return 0;
}

void OperationHistoryList::writeData(OperationHistoryData *data)
{
    m_operationHistoryFile.seek(m_offset);
    m_operationHistoryFile.write(data->toData());
    m_operationHistoryFile.flush();
    m_offset += OPERATION_HISTORY_DATA_RECORD_SIZE;
    if (m_offset >= OPERATION_HISTORY_MAX_DATA_SIZE) {
        m_offset = 0;
    }
}

bool OperationHistoryList::isValid(OperationHistoryData *data)
{
    if (data == nullptr)
        return false;

    bool ret = false;
    switch (data->d1()) {
    case 1:
        ret = isValidSetData(data->d2(), data->d3());
        break;
    case 2:
        ret = isValidOperationData(data->d2(), data->d3());
        break;
    case 3:
        ret = isValidDisplayData(data->d2(), data->d3());
        break;
    case 4:
        ret = isValidKeyCodeData(data->d2(), data->d3());
        break;
    case 5:
        ret = isValidViewSwitchingData(data->d2(), data->d3());
        break;
    }
    return ret;
}

bool OperationHistoryList::isValidSetData(int d2, int d3)
{
    bool ret = false;
    switch (d2) {
    case 0:
    case 15:
        ret = ((d3 >= 0) && (d3 <= 2));
        break;
    case 1:
    case 4:
    case 7:
    case 8:
    case 9:
    case 17:
        ret = (d3 == 0);
        break;
    case 5:
    case 14:
        ret = ((d3 >= 0) && (d3 <= 3));
        break;
    case 6:
        ret = ((d3 >= 0) && (d3 <= 12));
        break;
    case 10:
        ret = ((d3 >= 0) && (d3 <= 7));
        break;
    case 11:
    case 13:
        ret = ((d3 >= 0) && (d3 <= 4));
        break;
    case 12:
        ret = ((d3 >= 0) && (d3 <= 9));
        break;
    }
    return ret;
}

bool OperationHistoryList::isValidOperationData(int d2, int d3)
{
    bool ret = false;
    switch (d2) {
    case 40:
    case 41:
        ret = (d3 == 0);
        break;
    case 1:
        ret = ((d3 >= 0) && (d3 <= 4));
        break;
    case 3:
        ret = ((d3 >= 0) && (d3 <= 6));
        break;
    case 19:
        ret = ((d3 >= 0) && (d3 <= 2));
        break;
    }
    return ret;
}

bool OperationHistoryList::isValidDisplayData(int d2, int d3)
{
    bool ret = false;
    switch (d2) {
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
        ret = (d3 == 0);
        break;
    }
    return ret;
}

bool OperationHistoryList::isValidKeyCodeData(int d2, int d3)
{
    bool ret = false;
    switch (d2) {
    case 42:
        ret = ((d3 >= 201) && (d3 <= 218));
        break;
    case 0:
        ret = ((d3 >= 242) && (d3 <= 243));
        break;
    case 2:
        ret = ((d3 >= 240) && (d3 <= 246));
        break;
    case 6:
    case 7:
        ret = ((d3 >= 73) && (d3 <= 74));
        break;
    case 8:
        ret = (d3 == 0);
        break;
    case 16:
        ret = ((d3 >= 0) && (d3 <= 1));
        break;
    case 19:
    case 26:
        ret = (d3 == 247);
        break;
    }
    return ret;
}

bool OperationHistoryList::isValidViewSwitchingData(int d2, int d3)
{
    bool ret = false;
    if ((d2 >= 0 && d2 <= 3)) {
        ret = (d3 == (d2 + 5));
    } else if (d2 <= 19) {
        ret = (d3 == (d2 + 6));
    } else if (d2 == 20) {
        ret = false;
    } else if (d2 <= 21) {
        ret = (d3 == (d2 + 5));
    } else if (d2 <= 25) {
        ret = (d3 == (d2 + 6));
    }
    return ret;
}



