#ifndef IOSTATUS_H
#define IOSTATUS_H

#include <QObject>

class IoStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(int bankNo MEMBER m_bankNo NOTIFY bankNoChanged)
    Q_PROPERTY(bool isIoBank MEMBER m_isIoBank NOTIFY isIoBankChanged)
public:
    explicit IoStatus(int bankNo, bool isIoBank, QObject *parent = nullptr);
    explicit IoStatus(QObject *parent = nullptr);
    void setStatus(const QList<int> &statuslist);
    const QList<int> &status() const { return m_status; }
signals:
    void statusChanged();
    void bankNoChanged();
    void isIoBankChanged();
public slots:
private:
    QList<int>  m_status;
    int         m_bankNo = 0;
    bool        m_isIoBank = false;
};

#endif // IOSTATUS_H
