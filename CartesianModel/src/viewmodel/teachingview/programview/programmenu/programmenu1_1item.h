#ifndef PROGRAMMENU1_1ITEM_H
#define PROGRAMMENU1_1ITEM_H

#include <QObject>

class ProgramMenu1_1Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> timeList MEMBER m_timeList NOTIFY timeListChanged)
public:
    explicit ProgramMenu1_1Item(QObject *parent = nullptr);
    void appendTimerAddress(quint16 address);
    quint16 getTimerAddress(int index);
    int setTime(int index, int value);
    int getTime(int index);
    int length() { return m_timeList.length(); }

signals:
    void timeListChanged();

public slots:
private:
    QList<quint16>      m_timerAddressList;
    QList<int>          m_timeList;
};

#endif // PROGRAMMENU1_1ITEM_H
