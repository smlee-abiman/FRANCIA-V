#include "iohistorydata.h"
#include "iostatus.h"
#include "enums.h"

IoHistoryData::IoHistoryData(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < Enums::AxisNum; i++) {
        m_axisPositionList.append(0);
        m_seqList.append(0);
        m_stepList.append(0);
    }
}

void IoHistoryData::setDateTime(const QDateTime &dateTime)
{
    if (m_dateTime != dateTime) {
        m_dateTime = dateTime;
        emit dateTimeChanged();
    }
}

bool IoHistoryData::getIoStatus(int bankIndex, int ioBankIndex)
{
    if (bankIndex >= m_ioStatusList.length()) {
        return false;
    }
    auto ioStatusObj = static_cast<IoStatus *>(m_ioStatusList.at(bankIndex));
    int ioStatusIndex = ioBankIndex / 8;
    int ioStatusBit = ioBankIndex % 8;
    int ioStatus = ioStatusObj->status().at(ioStatusIndex);
    return (ioStatus & (1 << ioStatusBit)) != 0;
}

void IoHistoryData::appendIoStatus(IoStatus *status)
{
    m_ioStatusList.append(status);
    emit ioStatusListChanged();
}

void IoHistoryData::setSeq(int index, int value)
{
    if (index >= m_seqList.length()) {
        return;
    }
    m_seqList.replace(index, value);
    emit seqListChanged();
}

void IoHistoryData::setStep(int index, int value)
{
    if (index >= m_stepList.length()) {
        return;
    }
    m_stepList.replace(index, value);
    emit stepListChanged();
}

void IoHistoryData::setAxisPosition(int index, int value)
{
    if (index >= m_axisPositionList.length()) {
        return;
    }
    m_axisPositionList.replace(index, value);
    emit axisPositionListChanged();
}

void IoHistoryData::setIoChangeList(const QList<IoData> &ioChangeList)
{
    m_ioChangeList = ioChangeList;
    m_ioChangeListInitialized = true;
}

void IoHistoryData::setId(int id)
{
    m_id = id;
}

