#ifndef AUTOTIMERVIEWMODEL_H
#define AUTOTIMERVIEWMODEL_H

#include "viewmodelbase.h"
#include "commonviewmodel.h"

class AutoTimerData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int timer READ timer NOTIFY timerChanged)
    Q_PROPERTY(bool start READ start NOTIFY startChanged)
    Q_PROPERTY(bool up READ up NOTIFY upChanged)
    Q_PROPERTY(QString mark READ mark WRITE setMark NOTIFY markChanged)
    Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged)

public:
    explicit AutoTimerData(QObject *parent = nullptr);
    ~AutoTimerData();

    void setTimer(int timer);
    void setStart(bool start);
    void setUp(bool up);
    void setMark(QString mark);
    void setMsg(QString msg);

    int timer() const { return m_timer; }
    bool start() const { return m_start; }
    bool up() const { return m_up; }
    QString mark() const {return m_mark; }
    QString msg() const {return m_msg; }

signals:
    void timerChanged(int timer);
    void startChanged(bool start);
    void upChanged(bool up);
    void markChanged(QString mark);
    void msgChanged(QString msg);

private:
    int m_timer;
    bool m_start;
    bool m_up;
    QString m_mark;
    QString m_msg;
};

class AutoTimerViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)

public:
    AutoTimerViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~AutoTimerViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void startPageChanging();
    Q_INVOKABLE QList<QObject*> getDataList(int page);

    void initTimerMsg();
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }

private:
    ModbusManager       *m_modbusManager;
    CommonViewModel     *m_commonView;

    AutoTimerData       m_dataList[100];
    bool                m_pageChanging = false;
    bool                m_dataLoaded = false;

private slots:
    void onFinished();
    void endModbusPageWriteReady(QList<int> writeData);
    void onLanguageChanged(int language);

signals:
    void dataLoadedChanged(bool dataLoaded);

};

#endif // AUTOTIMERVIEWMODEL_H
