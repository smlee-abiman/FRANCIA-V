#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "corelibapplicationmanager.h"
#include <QQmlContext>
#include "corelibdebugdatamanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    /*added*/
    auto *context = engine.rootContext();	// QQmlContext *
    CoreLibApplicationManager *appMgr = new CoreLibApplicationManager();
    context->setContextProperty("appMgr",appMgr);
    //CoreLibDebugDataManager *manager = CoreLibDebugDataManager::get_instance();
    CoreLibDebugDataManager *dbgManager = CoreLibDebugDataManager::create_instance(app.applicationDirPath());
    context->setContextProperty("dbgManager", dbgManager);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

#include <QDir>

void aaa(){
    QDir dir("/tmp");
    dir.mkdir("a");
}
