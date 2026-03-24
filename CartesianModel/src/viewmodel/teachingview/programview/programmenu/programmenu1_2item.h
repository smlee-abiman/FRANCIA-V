#ifndef PROGRAMMENU1_2ITEM_H
#define PROGRAMMENU1_2ITEM_H

#include <QObject>

class ProgramMenu1_2Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int programNo MEMBER m_programNo NOTIFY programNoChanged)
    Q_PROPERTY(QList<int> timeList MEMBER m_timeList NOTIFY timeListChanged)
    Q_PROPERTY(int signal MEMBER m_signal NOTIFY signalChanged)
    Q_PROPERTY(bool use MEMBER m_use NOTIFY useChanged)
    Q_PROPERTY(bool wait MEMBER m_wait NOTIFY waitChanged)
public:
    explicit ProgramMenu1_2Item(QObject *parent = nullptr);
    void resetData();
    void setProgramNo(int no);
    int programNo() const { return m_programNo; }
    int setTime(int index, int time);
    int getTime(int index);
    void emitTimeList();
    void setSignal(int signal);
    int signal() const { return m_signal; }
    void setUse(bool use);
    bool use() const { return m_use; }
    void setWait(bool wait);
    bool wait() const { return m_wait; }
    int getKey(int index);
    void appendTime(int time);
    void appendKey(int key);
    void appendDiAddress(quint16 address);
    void appendHrAddress(quint16 address);
    const QList<quint16> &diAddressList() { return m_diAddressList; }
    const QList<quint16> &hrAddressList() { return m_hrAddressList; }

signals:
    void programNoChanged();
    void timeListChanged();
    void signalChanged();
    void useChanged();
    void waitChanged();

public slots:
private:
    int                 m_programNo = 0;
    QList<int>          m_timeList;
    int                 m_signal = 0;
    bool                m_use = false;
    bool                m_wait = false;
    QList<int>          m_keyList;
    QList<quint16>      m_diAddressList;
    QList<quint16>      m_hrAddressList;
};

#endif // PROGRAMMENU1_2ITEM_H
