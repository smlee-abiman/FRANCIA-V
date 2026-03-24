#include "appmain.h"

AppMain::AppMain(QObject *parent, QCoreApplication *coreApp) : QObject(parent) ,app(coreApp)
{
    m_wdtMgr = new WatchDogManager();

    m_timer.setInterval(100);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()), Qt::DirectConnection);
}

AppMain::~AppMain()
{
    m_timer.stop();

    disconnect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    app->quit();
}

void AppMain::run()
{
    m_timer.start();
}

void AppMain::onTimeout()
{
    m_wdtMgr->wdtProcess();
}
