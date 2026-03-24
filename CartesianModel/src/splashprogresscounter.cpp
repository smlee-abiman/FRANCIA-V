#include "splashprogresscounter.h"
#include "splashprogresscounterworker.h"
#include <QThread>
#include <QDebug>

SplashProgressCounter::SplashProgressCounter(QObject *parent)
    : QObject{parent}
{
    m_worker = new SplashProgressCounterWorker();
    m_worker->moveToThread(&m_thread);
}

SplashProgressCounter::~SplashProgressCounter()
{
    disconnectSignal();
    delete m_worker;
    m_thread.quit();
    m_thread.wait();
}

void SplashProgressCounter::start(int maxTime)
{
    connectSignal();
    m_worker->start(maxTime);
    m_thread.start();
}

void SplashProgressCounter::stop()
{
    m_worker->stop();
    disconnectSignal();
    m_thread.quit();
    m_thread.wait();
}

QString SplashProgressCounter::show()
{
    return m_worker->show();
}

void SplashProgressCounter::connectSignal()
{
    connect(m_worker, SIGNAL(countChanged(int)), this, SLOT(onCountChanged(int)), Qt::QueuedConnection);
    connect(m_worker, SIGNAL(percentChanged(int)), this, SLOT(onPercentChanged(int)), Qt::QueuedConnection);
    connect(m_worker, SIGNAL(elapsedTimeChanged(int)), this, SLOT(onElapsedTimeChanged(int)), Qt::QueuedConnection);
}

void SplashProgressCounter::disconnectSignal()
{
    disconnect(m_worker, SIGNAL(countChanged(int)), this, SLOT(onCountChanged(int)));
    disconnect(m_worker, SIGNAL(percentChanged(int)), this, SLOT(onPercentChanged(int)));
    disconnect(m_worker, SIGNAL(elapsedTimeChanged(int)), this, SLOT(onElapsedTimeChanged(int)));
}

void SplashProgressCounter::setCount(int count)
{
    if (m_count != count) {
        m_count = count;
        emit countChanged();
    }
}

void SplashProgressCounter::setPercent(int percent)
{
    if (m_percent != percent) {
        m_percent = percent;
        emit percentChanged();
    }
}

void SplashProgressCounter::setElapsedTime(int time)
{
    if (m_elapsedTime != time) {
        m_elapsedTime = time;
        emit elapsedTimeChanged();
    }
}

void SplashProgressCounter::onCountChanged(int count)
{
    setCount(count);
}

void SplashProgressCounter::onPercentChanged(int percent)
{
    setPercent(percent);
}

void SplashProgressCounter::onElapsedTimeChanged(int time)
{
    setElapsedTime(time);
}

