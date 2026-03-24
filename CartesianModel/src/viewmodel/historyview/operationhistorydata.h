#ifndef OPERATIONHISTORYDATA_H
#define OPERATIONHISTORYDATA_H

#include <QObject>
#include <QDateTime>

class OperationHistoryData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
public:
    explicit OperationHistoryData(QObject *parent = nullptr);
    void setDateTime(const QDateTime &dateTime);
    void setData(const QByteArray &array);
    int setData(int index, int value);
    const QDateTime &dateTime() const { return m_dateTime; }
    int at(int index) { return m_data.at(index); }
    int d1() { return m_data.at(0); }
    int d2() { return m_data.at(1); }
    int d3() { return m_data.at(2); }
    int d4() { return m_data.at(3); }
    int d5() { return m_data.at(4); }
    int d6() { return m_data.at(5); }
    int d7() { return m_data.at(6); }
    void setD1(int value) { m_data.replace(0, value); }
    void setD2(int value) { m_data.replace(1, value); }
    void setD3(int value) { m_data.replace(2, value); }
    void setD4(int value) { m_data.replace(3, value); }
    void setD5(int value) { m_data.replace(4, value); }
    void setD6(int value) { m_data.replace(5, value); }
    void setD7(int value) { m_data.replace(6, value); }
    QByteArray toData();

private:
    short toShort(const QByteArray &array, int index);
    int toInt(const QByteArray &array, int index);

signals:
    void dateTimeChanged();
    void dataChanged();

private:
    QDateTime   m_dateTime;
    QList<int>  m_data;
};

#endif // OPERATIONHISTORYDATA_H
