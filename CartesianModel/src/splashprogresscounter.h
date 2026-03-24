#ifndef SPLASHPROGRESSCOUNTER_H
#define SPLASHPROGRESSCOUNTER_H

#include <QObject>
#include <QThread>

class SplashProgressCounterWorker;

class SplashProgressCounter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count MEMBER m_count NOTIFY countChanged)
    Q_PROPERTY(int percent MEMBER m_percent NOTIFY percentChanged)
    Q_PROPERTY(int elapsedTime MEMBER m_elapsedTime NOTIFY elapsedTimeChanged)
public:
    explicit SplashProgressCounter(QObject *parent = nullptr);
    ~SplashProgressCounter() override;
    Q_INVOKABLE void start(int maxTime);
    Q_INVOKABLE void stop();
    Q_INVOKABLE QString show();
private:
    void connectSignal();
    void disconnectSignal();
    void setCount(int count);
    void setPercent(int percent);
    void setElapsedTime(int time);
signals:
    void countChanged();
    void percentChanged();
    void elapsedTimeChanged();

private slots:
    void onCountChanged(int count);
    void onPercentChanged(int percent);
    void onElapsedTimeChanged(int time);

private:
    SplashProgressCounterWorker     *m_worker;
    QThread                         m_thread;
    int                             m_count = 0;
    int                             m_percent = 0;
    int                             m_elapsedTime = 0;
};
#endif // SPLASHPROGRESSCOUNTER_H
