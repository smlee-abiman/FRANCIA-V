#ifndef AUTOCOUNTERVIEWMODEL_H
#define AUTOCOUNTERVIEWMODEL_H

#include "viewmodelbase.h"
#include "commonviewmodel.h"

class AutoCounterData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int now READ now NOTIFY nowChanged)
    Q_PROPERTY(int setting READ setting NOTIFY settingChanged)

public:
    explicit AutoCounterData(QObject *parent = nullptr);
    ~AutoCounterData();

    void setNow(int now);
    void setSetting(int setting);

    int now() const { return m_now; }
    int setting() const { return m_setting; }

private:
    int     m_now;
    int     m_setting;

signals:
    void nowChanged(int now);
    void settingChanged(int setting);
};

class AutoCounterViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<int> counterMsgIndex READ counterMsgIndex CONSTANT)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)

public:
    AutoCounterViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~AutoCounterViewModel();

    QList<int> counterMsgIndex() { return m_counterMsgIndex; }
    bool pageIng() {return m_pageIng;}

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void startPageChanging();
    Q_INVOKABLE QList<QObject*> getDataList(int page);
    void initcounterMsgIndex();
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }

private:
    ModbusManager       *m_modbusManager;
    CommonViewModel     *m_commonView;
    AutoCounterData     m_autoCounterDataAry[8];
    bool                m_pageChanging = false;
    QList<int>          m_counterMsgIndex;
    bool                m_dataLoaded = false;
    bool                m_pageIng = false;

private slots:
    void onFinished();
    void endModbusPageWriteReady(QList<int> writeData);

signals:
    void dataLoadedChanged(bool dataLoaded);

};

#endif // AUTOCOUNTERVIEWMODEL_H
