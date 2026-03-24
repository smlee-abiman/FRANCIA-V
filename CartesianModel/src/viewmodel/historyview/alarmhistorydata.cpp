#include "alarmhistorydata.h"
#include <QDebug>

AlarmHistoryData::AlarmHistoryData(QObject *parent) : QObject(parent)
{

}

void AlarmHistoryData::setDateTime(const QDateTime &dateTime)
{
    if (m_dateTime != dateTime) {
        m_dateTime = dateTime;
        emit dateTimeChanged();
    }
}

void AlarmHistoryData::setUnitNo(int no)
{
    if (m_unitNo != no) {
        m_unitNo = no;
        emit unitNoChanged();
    }
}

void AlarmHistoryData::setAlarmNo(int no)
{
    if (m_alarmNo != no) {
        m_alarmNo = no;
        emit alarmNoChanged();
    }
}

QByteArray AlarmHistoryData::toData()
{
    QByteArray data;
    QDate date = m_dateTime.date();
    data.append(static_cast<qint8>(date.year() - 2000));
    data.append(static_cast<qint8>(date.month()));
    data.append(static_cast<qint8>((date.day())));
    QTime time = m_dateTime.time();
    data.append(static_cast<qint8>((time.hour())));
    data.append(static_cast<qint8>((time.minute())));
    data.append(static_cast<qint8>((unitNo())));
    data.append(static_cast<qint8>((time.second())));
    data.append(static_cast<qint8>((alarmNo())));
//    qDebug() << m_dateTime.toString("yyyy/MM/dd hh:mm:ss") << unitNo() << alarmNo();
    return data;
}

bool AlarmHistoryData::operator==(const AlarmHistoryData &data) const
{
    return (m_dateTime == data.m_dateTime) &&
           (m_unitNo == data.m_unitNo) &&
           (m_alarmNo == data.m_alarmNo);
}
