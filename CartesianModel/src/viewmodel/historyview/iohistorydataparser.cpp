#include "iohistorydataparser.h"
#include <QFile>
#include <QDataStream>
#include "iohistorydata.h"
#include "iostatus.h"

const int IO_HISTORY_DATA_RECORD_SIZE = 612;


IoHistoryDataParser::IoHistoryDataParser()
{

}

int IoHistoryDataParser::parseIoHistory(const QString &filePath, QList<QObject *> &ioDataList)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return -1;
    }
    QByteArray arr = file.readAll();
    QDateTime dateTime;
    int pos;
    int remain = arr.size();
    int i = 0;
    int count = 1;
    while (remain >= IO_HISTORY_DATA_RECORD_SIZE) {
        if (static_cast<quint8>(arr.at(i + 600)) == 0xFF && static_cast<quint8>(arr.at(i+601)) == 0xFF) {
            remain -= IO_HISTORY_DATA_RECORD_SIZE;
            i += IO_HISTORY_DATA_RECORD_SIZE;
            continue;
        }

        auto ioHistoryData = new IoHistoryData();
        for (int j = 0; j < 10; j++) {
            auto ioStatus = new IoStatus(j / 2, j%2 == 0);
            QList<int> dataList;
            for (int l = 0; l < 32; l++) {
                dataList.append(arr.at(i));
                i++;
            }
            ioStatus->setStatus(dataList);
            ioHistoryData->appendIoStatus(ioStatus);
        }

        i += 192;
        i += 32; // reserve

        for (int j = 0; j < 8; j++) {
            ioHistoryData->setSeq(j, toShort(arr, i));
            i += 2;
            ioHistoryData->setStep(j, toShort(arr, i));
            i += 2;
        }

        for (int j = 0; j < 8; j++) {
            pos = static_cast<quint8>(arr.at(i)) | (static_cast<quint8>(arr.at(i + 1)) << 8) | (static_cast<quint8>(arr.at(i + 2))  << 16);
            if ((pos & 0x00800000) != 0) {
                pos |= 0xFF000000;
            }
            ioHistoryData->setAxisPosition(j, pos);
            i += 3;
        }
        getDateTime(arr, i, dateTime);
        i += 8;
        ioHistoryData->setDateTime(dateTime);
        i += 4; // reserve
        ioHistoryData->setId(count);
        ioDataList.append(ioHistoryData);

        remain -= IO_HISTORY_DATA_RECORD_SIZE;
        count++;
    }
    file.close();
    return 0;
}

short IoHistoryDataParser::toShort(const QByteArray &array, int index)
{
    short ret = 0;
    QDataStream stream(array.mid(index, 2));
    stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);
    stream >> ret;
    return ret;
}

void IoHistoryDataParser::getDateTime(const QByteArray &array, int index, QDateTime &dateTime)
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int sec;
    QDate date;
    QTime time;

    year = toShort(array, index);
    month = array.at(index + 2);
    day = array.at(index + 3);
    hour = array.at(index + 5);
    minute = array.at(index + 6);
    sec = array.at(index + 7);
    date.setDate(year, month, day);
    time.setHMS(hour, minute, sec);
    dateTime.setDate(date);
    dateTime.setTime(time);

}
