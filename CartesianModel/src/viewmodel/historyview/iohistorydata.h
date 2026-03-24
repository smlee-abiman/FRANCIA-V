#ifndef IOHISTORYDATA_H
#define IOHISTORYDATA_H

#include <QObject>
#include <QDateTime>
#include "iodata.h"

class IoStatus;

class IoHistoryData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(QList<QObject *> ioStatusList MEMBER m_ioStatusList NOTIFY ioStatusListChanged)
    Q_PROPERTY(QList<int> axisPositionList MEMBER m_axisPositionList NOTIFY axisPositionListChanged)
    Q_PROPERTY(QList<int> seqList MEMBER m_seqList NOTIFY seqListChanged)
    Q_PROPERTY(QList<int> stepList MEMBER m_stepList NOTIFY stepListChanged)

public:
    explicit IoHistoryData(QObject *parent = nullptr);
    void setDateTime(const QDateTime &dateTime);
    const QDateTime &dateTime() const { return m_dateTime; }
    bool getIoStatus(int bankIndex, int ioBankIndex);
    void appendIoStatus(IoStatus *status);
    void setSeq(int index, int value);
    void setStep(int index, int value);
    void setAxisPosition(int index, int value);
    void setIoChangeList(const QList<IoData> &ioChangeList);
    void setId(int id);
    int id() const { return m_id; }
    const QList<IoData> &ioChangeList() const { return m_ioChangeList; }
    const QList<QObject *> &ioStatusList() const { return m_ioStatusList; }
    bool ioChangeListInitialized() const { return m_ioChangeListInitialized; }

signals:
    void dateTimeChanged();
    void ioStatusListChanged();
    void axisPositionListChanged();
    void seqListChanged();
    void stepListChanged();
    void ioChangeListChanged();

public slots:
private:
    QDateTime           m_dateTime;
    QList<QObject *>    m_ioStatusList;
    QList<int>          m_axisPositionList;
    QList<int>          m_seqList;
    QList<int>          m_stepList;
    QList<IoData>       m_ioChangeList;
    bool                m_ioChangeListInitialized = false;
    int                 m_id = 0;
};

#endif // IOHISTORYDATA_H
