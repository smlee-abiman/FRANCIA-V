#include "operationhistorydataparser.h"
#include <QFile>
#include <QDataStream>
#include "operationhistorydata.h"
#include <QDebug>

const int OPERATION_HISTORY_DATA_RECORD_SIZE = 24;

OperationHistoryDataParser::OperationHistoryDataParser()
{

}

int OperationHistoryDataParser::parseOperationHistory(const QString &filePath, QList<QObject *> &operationDataList)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return -1;
    }
    QByteArray arr = file.readAll();
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int sec;
    QDate date;
    QTime time;
    QDateTime dateTime;
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
        OperationHistoryData *operationHistoryData = new OperationHistoryData();
        if (operationHistoryData != nullptr) {
            operationHistoryData->setDateTime(dateTime);
            operationHistoryData->setData(array);
            operationDataList.append(operationHistoryData);
        }
    }
    file.close();
    return 0;
}
