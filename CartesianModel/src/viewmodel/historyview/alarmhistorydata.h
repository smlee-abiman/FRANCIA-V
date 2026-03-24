#ifndef ALARMHISTORYDATA_H
#define ALARMHISTORYDATA_H

#include <QObject>
#include <QDateTime>

class AlarmHistoryData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(int unitNo READ unitNo WRITE setUnitNo NOTIFY unitNoChanged)
    Q_PROPERTY(int alarmNo READ alarmNo WRITE setAlarmNo NOTIFY alarmNoChanged)
public:
    explicit AlarmHistoryData(QObject *parent = nullptr);
    void setDateTime(const QDateTime &dateTime);
    void setUnitNo(int no);
    void setAlarmNo(int no);
    const QDateTime &dateTime() const { return m_dateTime; }
    int unitNo() const { return m_unitNo; }
    int alarmNo() const { return m_alarmNo; }
    bool isAxisAlarm() { return (m_unitNo >= 0) && (m_unitNo <= 7);}
    bool isSystemAlarm() { return (m_unitNo == 8);}
    bool isNormalAlarm() { return (m_unitNo == 9);}
    QByteArray toData();
    bool operator==(const AlarmHistoryData &data) const;
signals:
    void dateTimeChanged();
    void unitNoChanged();
    void alarmNoChanged();
private:
    QDateTime   m_dateTime;
    int         m_unitNo = 0;
    int         m_alarmNo = 0;
};

#endif // ALARMHISTORYDATA_H
