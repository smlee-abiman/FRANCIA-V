#ifndef OPERATIONHISTORYLIST_H
#define OPERATIONHISTORYLIST_H

#include <QObject>
#include <QFile>

class OperationHistoryData;

class OperationHistoryList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> operationList MEMBER m_operationList NOTIFY operationListChanged)
public:
    explicit OperationHistoryList(QObject *parent = nullptr);
    int add(OperationHistoryData *data);
    QList<QObject *> &operationList() { return m_operationList; }
    bool isValid(OperationHistoryData *data);

private:
    void init();
    int readData();
    void writeData(OperationHistoryData *data);
    bool isValidSetData(int d2, int d3);
    bool isValidOperationData(int d2, int d3);
    bool isValidDisplayData(int d2, int d3);
    bool isValidKeyCodeData(int d2, int d3);
    bool isValidViewSwitchingData(int d2, int d3);

signals:
    void operationListChanged();

public slots:
private:
    QFile               m_operationHistoryFile;
    QList<QObject *>    m_operationList;
    int                 m_offset = 0;
};

#endif // OPERATIONHISTORYLIST_H
