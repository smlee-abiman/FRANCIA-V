#ifndef PROGRAMMENU1_3ITEM_H
#define PROGRAMMENU1_3ITEM_H

#include <QObject>

class ProgramMenu1_3Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int titleIndex MEMBER m_titleIndex NOTIFY titleIndexChanged)
    Q_PROPERTY(bool state MEMBER m_state NOTIFY stateChanged)
    Q_PROPERTY(bool detect MEMBER m_detect NOTIFY detectChanged)
    Q_PROPERTY(bool signal MEMBER m_signal NOTIFY signalChanged)
    Q_PROPERTY(QList<int> timeList MEMBER m_timeList NOTIFY timeListChanged)
public:
    enum Type {
        Absorption,
        Hold,
        S_ArmClamp,
        Unknown
    };
    explicit ProgramMenu1_3Item(QObject *parent = nullptr);
    void resetData();
    void setTitleIndex(int index);
    int titleIndex() const { return m_titleIndex; }
    void setState(bool state);
    bool state() const { return m_state; }
    void setDetect(bool detect);
    bool detect() const { return m_detect; }
    void setSignal(bool signal);
    bool signal() const { return m_signal; }
    void appendTime(int time);
    int setTime(int index, int time);
    int getTime(int index);
    void emitTimeList();
    void appendKey(int key);
    int getKey(int index);
    void setDiAddress(quint16 address);
    quint16 diAddress() { return m_diAddress; }
    void appendHrAddress(quint16 address);
    const QList<quint16> &hrAddressList() { return m_hrAddressList; }
    void setType(Type type) { m_type = type; }
    Type type() const { return m_type; }
    void setNo(int no) { m_no = no; }
    int no() const { return m_no; }

signals:
    void titleIndexChanged();
    void stateChanged();
    void detectChanged();
    void signalChanged();
    void timeListChanged();

public slots:
private:
    int                 m_titleIndex = 0;
    bool                m_state = false;
    bool                m_detect = false;
    bool                m_signal = false;
    Type                m_type = Unknown;
    int                 m_no = 0;
    QList<int>          m_timeList;
    QList<int>          m_keyList;
    QList<quint16>      m_hrAddressList;
    quint16             m_diAddress = 0;
};

#endif // PROGRAMMENU1_3ITEM_H
