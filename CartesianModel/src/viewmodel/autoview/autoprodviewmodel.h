#ifndef AUTOPRODVIEWMODEL_H
#define AUTOPRODVIEWMODEL_H

#include "viewmodelbase.h"

class CommonViewModel;

class AutoProdViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int fetchTime READ fetchTime NOTIFY fetchTimeChanged)
    Q_PROPERTY(int cycleTime READ cycleTime NOTIFY cycleTimeChanged)
    Q_PROPERTY(int moldingTime READ moldingTime NOTIFY moldingTimeChanged)
    Q_PROPERTY(int oneFetchNum READ oneFetchNum NOTIFY oneFetchNumChanged)
    Q_PROPERTY(int fetchNum READ fetchNum NOTIFY fetchNumChanged)
    Q_PROPERTY(int prodNum READ prodNum NOTIFY prodNumChanged)
    Q_PROPERTY(int progNum READ progNum NOTIFY progNumChanged)
    Q_PROPERTY(int badRate READ badRate NOTIFY badRateChanged)
    Q_PROPERTY(int achievementRate READ achievementRate NOTIFY achievementRateChanged)
    Q_PROPERTY(int notificationTime READ notificationTime NOTIFY notificationTimeChanged)
    Q_PROPERTY(int badCount READ badCount NOTIFY badCountChanged)
    Q_PROPERTY(QString prodEnd READ prodEnd NOTIFY prodEndChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)

public:
    AutoProdViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~AutoProdViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void startPageChanging();

    void setFetchTime(int fetchTime);
    void setCycleTime(int cycleTime);
    void setMoldingTime(int moldingTime);
    void setOneFetchNum(int oneFetchNum);
    void setFetchNum(int fetchNum);
    void setProdNum(int prodNum);
    void setProgNum(int progNum);
    void setBadRate(int badRate);
    void setBadCount(int badCount);
    void setNotificationTime(int notificationTime);
    void setProdEnd(int year, int month, int day, int hour, int min, int sec);
    void setDataLoaded(bool dataLoaded);


    int fetchTime() const { return m_fetchTime; }
    int cycleTime() const { return m_cycleTime; }
    int moldingTime() const { return m_moldingTime; }
    int oneFetchNum() const { return m_oneFetchNum; }
    int fetchNum() const { return m_fetchNum; }
    int prodNum() const { return m_prodNum; }
    int progNum() const { return m_progNum; }
    int badRate() const { return m_badRate; }
    int badCount() const { return m_badCount; }
    int achievementRate() { return (m_progNum != 0 ? (int)(((double)m_prodNum / (double)m_progNum) * 100) : 0); }
    int notificationTime() const { return m_notificationTime; }
    QString prodEnd();
    bool dataLoaded() { return m_dataLoaded; }
    bool pageIng() {return m_pageIng;}


private:
    CommonViewModel *m_commonView;
    ModbusManager *m_modbusManager;

    int m_fetchTime;         // 取出時間(1/100s)
    int m_cycleTime;         // サイクル時間(1/100s)
    int m_moldingTime;       // 成型時間(1/100s)
    int m_oneFetchNum;          // 1回取出数（1-255）
    int m_fetchNum;             // 取出数
    int m_prodNum;              // 生産数
    int m_progNum;              // 予定数(0-9999999)
    int m_badRate;              // 不良率(100%)
    int m_badCount;             // 不良品数
    int m_notificationTime;     // 告知時間(1-99)
    int m_prodEndYear;          // 生産終了予定(年)
    int m_prodEndMonth;         // 生産終了予定(月)
    int m_prodEndDay;           // 生産終了予定(日)
    int m_prodEndHour;          // 生産終了予定(時)
    int m_prodEndMin;           // 生産終了予定(分)
    int m_prodEndSec;           // 生産終了予定(秒)

    bool m_pageChanging = false;
    bool m_dataLoaded = false;
    bool m_pageIng = false;

signals:
    void fetchTimeChanged(int fetchTime);
    void cycleTimeChanged(int cycleTime);
    void moldingTimeChanged(int moldingTime);
    void oneFetchNumChanged(int oneFetchNum);
    void fetchNumChanged(int fetchNum);
    void prodNumChanged(int prodNum);
    void progNumChanged(int progNum);
    void badRateChanged(int badRate);
    void badCountChanged(int badCount);
    void achievementRateChanged(int achievementRate);
    void notificationTimeChanged(int notificationTime);
    void prodEndChanged(QString torque);
    void dataLoadedChanged(bool dataLoaded);

private slots:
    void onFinished();
    void endModbusPageWriteReady(QList<int> writeData);
};

#endif // AUTOPRODVIEWMODEL_H
