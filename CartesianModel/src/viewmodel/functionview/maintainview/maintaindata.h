#ifndef MAINTAINDATA_H
#define MAINTAINDATA_H

#include <QObject>

class MaintainData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int refuelingDistance READ refuelingDistance WRITE setRefuelingDistance NOTIFY refuelingDistanceChanged)
    Q_PROPERTY(int cumulativeDistance READ cumulativeDistance WRITE setCumulativeDistance NOTIFY cumulativeDistanceChanged)
    Q_PROPERTY(QString lastRefuelingDate READ lastRefuelingDate WRITE setLastRefuelingDate NOTIFY lastRefuelingDateChanged)
public:
    explicit MaintainData(QObject *parent = nullptr);
    void setRefuelingDistance(int value);
    int refuelingDistance() const { return m_refuelingDistance; }
    void setCumulativeDistance(int value);
    int cumulativeDistance() const { return m_cumulativeDistance; }
    void setLastRefuelingDate(const QString &date);
    QString lastRefuelingDate() const { return m_lastRefuelingDate; }

signals:
    void refuelingDistanceChanged();
    void cumulativeDistanceChanged();
    void lastRefuelingDateChanged();

private:
    int         m_refuelingDistance = 0;    // cm ( 97056 =  0.97Km)
    int         m_cumulativeDistance = 0;   // 0.01km(70087 = 700.87Km)
    QString     m_lastRefuelingDate = "";
};

#endif // MAINTAINDATA_H
