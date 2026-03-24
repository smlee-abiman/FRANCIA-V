#include "splashprogresscounterworker.h"
#include <QThread>
#include <QDebug>

SplashProgressCounterWorker::SplashProgressCounterWorker(QObject *parent)
    : QObject{parent}
{
    m_timer = new QTimer();
    m_timer->setInterval(m_interval);
}

SplashProgressCounterWorker::~SplashProgressCounterWorker()
{
    stop();
    delete m_timer;
}

int SplashProgressCounterWorker::start(int maxTime)
{
    delete m_elapsedTimer;
    m_elapsedTimer = nullptr;
    m_elapsedTimer = new QElapsedTimer();
    if (m_elapsedTimer == nullptr) {
        qWarning() << __func__ << "Generation of m_elapsedTimer failed.";
        return -1;
    }
    initData();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateCount()));
    m_maxTime = maxTime;
    m_timer->start();
    m_elapsedTimer->start();
    return 0;
}

void SplashProgressCounterWorker::stop()
{
    disconnect(m_timer, SIGNAL(timeout()), this, SLOT(updateCount()));
    if (m_timer != nullptr) {
        m_timer->stop();
    }
    delete m_elapsedTimer;
    m_elapsedTimer = nullptr;
    initData();
}

void SplashProgressCounterWorker::updateCount()
{
    int percent;
    int elapsedTime = static_cast<int>(m_elapsedTimer->elapsed());
    percent = elapsedTime * 100 / m_maxTime;
//    percent = percent / 10 * 10;
    if (percent == 0) {
        percent = m_basePercent;
    } else {
        percent += m_basePercent;
        if (percent > 100) {
            percent = 100;
        }
    }
    setPercent(percent);
    setElapsedTime(elapsedTime);
    incrementCount();
}

void SplashProgressCounterWorker::initData()
{
    m_count = 0;
    m_percent = 0;
    m_elapsedTime = 0;
}

void SplashProgressCounterWorker::incrementCount()
{
    m_count++;
    emit countChanged(m_count);
}

void SplashProgressCounterWorker::setPercent(int percent)
{
    if (m_percent != percent) {
        m_percent = percent;
        emit percentChanged(m_percent);
    }
}

void SplashProgressCounterWorker::setElapsedTime(int time)
{
    if (m_elapsedTime != time) {
        m_elapsedTime = time;
        emit elapsedTimeChanged(m_elapsedTime);
    }
}

QString SplashProgressCounterWorker::show()
{
    return QString("count:%1 %2% %3msec").arg(m_count).arg(m_percent).arg(m_elapsedTime);
}
