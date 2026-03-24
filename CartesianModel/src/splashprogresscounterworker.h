#ifndef SPLASHPROGRESSCOUNTERWORKER_H
#define SPLASHPROGRESSCOUNTERWORKER_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class SplashProgressCounterWorker : public QObject
{
    Q_OBJECT
public:
    explicit SplashProgressCounterWorker(QObject *parent = nullptr);
    ~SplashProgressCounterWorker() override;
    int start(int maxTime);
    void stop();
    QString show();
private:
    void initData();
    void incrementCount();
    void setPercent(int percent);
    void setElapsedTime(int time);

signals:
    void countChanged(int count);
    void percentChanged(int percent);
    void elapsedTimeChanged(int time);

private slots:
    void updateCount();

private:
    QThread         *m_thread = nullptr;
    QElapsedTimer   *m_elapsedTimer = nullptr;
    QTimer          *m_timer = nullptr;
    int             m_basePercent = 10;
    int             m_interval = 100;
    int             m_maxTime = 3000;
    int             m_count = 0;
    int             m_percent = 0;
    int             m_elapsedTime = 0;
};

#endif // SPLASHPROGRESSCOUNTERWORKER_H
