#include "operationhistorydata.h"
#include <QDebug>
#include <QDataStream>

const int OPERATION_HISTORY_DATA_NUM = 7;

OperationHistoryData::OperationHistoryData(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < OPERATION_HISTORY_DATA_NUM; i++)
        m_data.append(0);
}

void OperationHistoryData::setDateTime(const QDateTime &dateTime)
{
    if (m_dateTime != dateTime) {
        m_dateTime = dateTime;
        emit dateTimeChanged();
    }
}

void OperationHistoryData::setData(const QByteArray &array)
{
    m_data.clear();
    m_data.append(array.at(0));
    m_data.append(array.at(1));
    m_data.append(toShort(array, 2));
    m_data.append(toShort(array, 4));
    m_data.append(toInt(array, 6));
    m_data.append(toInt(array, 10));
    m_data.append(toInt(array, 14));
}

int OperationHistoryData::setData(int index, int value)
{
    if (index < 0 || index >= m_data.length()) {
        return -1;
    }
    m_data.replace(index, value);
    return 0;
}

QByteArray OperationHistoryData::toData()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QDate date = m_dateTime.date();
    stream << static_cast<qint8>(date.year() - 2000);
    stream << static_cast<qint8>(date.month());
    stream << static_cast<qint8>((date.day()));
    QTime time = m_dateTime.time();
    stream << static_cast<qint8>((time.hour()));
    stream << static_cast<qint8>((time.minute()));
    stream << static_cast<qint8>((time.second()));
    stream << static_cast<qint8>((d1()));
    stream << static_cast<qint8>((d2()));
    stream << static_cast<qint16>((d3()));
    stream << static_cast<qint16>((d4()));
    stream << d5();
    stream << d6();
    stream << d7();
    return data;
}


short OperationHistoryData::toShort(const QByteArray &array, int index)
{
    short ret = 0;
    QDataStream stream(array.mid(index, 2));
    stream >> ret;
    return ret;
}

int OperationHistoryData::toInt(const QByteArray &array, int index)
{
    int ret = 0;
    QDataStream stream(array.mid(index, 4));
    stream >> ret;
    return ret;
}
