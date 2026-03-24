#ifndef AUTOIOVIEWMODEL_H
#define AUTOIOVIEWMODEL_H

#include "viewmodelbase.h"

class AutoIOData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index CONSTANT)
    Q_PROPERTY(bool value READ value NOTIFY valueChanged)

public:
    explicit AutoIOData(int index, QObject *parent = nullptr);
    ~AutoIOData();

    void setValue(bool value);

    int index() const { return m_index; }
    bool value() const { return m_value; }

private:
    int m_index;
    bool m_value;

signals:
    void valueChanged(bool value);
};

class AutoIOViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> ioDataList READ ioDataList WRITE setIoDataList NOTIFY ioDataListChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)

public:
    AutoIOViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~AutoIOViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void startPageChanging();

    void setIoDataList (QList<QObject *> list);
    QList<QObject *> ioDataList() const {return m_ioDataList;}
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }
    bool pageIng() {return m_pageIng;}

private:
    ModbusManager           *m_modbusManager;
    QList<QObject *>        m_ioDataList;
    bool                    m_pageChanging = false;
    bool                    m_dataLoaded = false;
    bool                    m_pageIng = false;

    void readPensetData();

private slots:
    void onFinished();
    void endModbusPageWriteReady(QList<int> writeData);

signals:
    void ioDataListChanged(QList<QObject *> list);
    void dataLoadedChanged(bool dataLoaded);
};

#endif // AUTOIOVIEWMODEL_H
