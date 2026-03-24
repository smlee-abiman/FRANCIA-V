#include "maintaindata.h"

MaintainData::MaintainData(QObject *parent) : QObject(parent)
{

}

void MaintainData::setRefuelingDistance(int value)
{
    if (m_refuelingDistance != value) {
        m_refuelingDistance = value;
        emit refuelingDistanceChanged();
    }
}

void MaintainData::setCumulativeDistance(int value)
{
    if (m_cumulativeDistance != value) {
        m_cumulativeDistance = value;
        emit cumulativeDistanceChanged();
    }
}

void MaintainData::setLastRefuelingDate(const QString &date)
{
    if (m_lastRefuelingDate != date) {
        m_lastRefuelingDate = date;
        emit lastRefuelingDateChanged();
    }
}
