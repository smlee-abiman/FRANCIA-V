#include "alarmhistorydataparser.h"
#include <QFile>
#include <QDataStream>
#include "alarmhistorydata.h"

const int ALARM_HISTORY_DATA_RECORD_SIZE = 8;

AlarmHistoryDataParser::AlarmHistoryDataParser()
{

}

int AlarmHistoryDataParser::parseAlarmHistory(const QString &filePath, QList<AlarmHistoryData *> &alarmDataList)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << __func__ << "Open failed." << filePath;
        return -1;
    }
    if (file.size() == 0) {
        qDebug() << __func__ << "File is empty." << filePath;
        file.close();
        return -2;
    }
    QByteArray arr = file.readAll();
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int sec;
    int unitNo;
    int alarmNo;
    QDate date;
    QTime time;
    QDateTime dateTime;
    int remain = arr.size();
    int i = 0;
    while (remain >= ALARM_HISTORY_DATA_RECORD_SIZE) {
        year = 2000 + arr.at(i);
        month = arr.at(i + 1);
        day = arr.at(i + 2);
        hour = arr.at(i + 3);
        minute = arr.at(i + 4);
        unitNo = arr.at(i + 5);
        sec = arr.at(i + 6);
        alarmNo = static_cast<unsigned char>(arr.at(i + 7));
        remain -= ALARM_HISTORY_DATA_RECORD_SIZE;
        i += ALARM_HISTORY_DATA_RECORD_SIZE;

        date.setDate(year, month, day);
        time.setHMS(hour, minute, sec);
        dateTime.setDate(date);
        dateTime.setTime(time);
        if (!dateTime.isValid()) {
            qWarning() << __func__ << "Invalid date:" << year << month << day << hour << minute << sec << unitNo << alarmNo;
            continue;
        }
        AlarmHistoryData *data = new AlarmHistoryData();
        if (data != nullptr) {
            data->setUnitNo(unitNo);
            data->setAlarmNo(alarmNo);
            data->setDateTime(dateTime);
            alarmDataList.append(data);
        }
    }
    file.close();
    return 0;
}

AlarmHistoryData *AlarmHistoryDataParser::parseAlarmInfo(const AlarmInfo &alarmInfo, const QDateTime &dateTime)
{
    auto *data = new AlarmHistoryData();
    if (data != nullptr) {
        data->setUnitNo(alarmInfo.type());
        data->setAlarmNo(alarmInfo.msgNumber());
        data->setDateTime(dateTime);
    }
    return data;
}
