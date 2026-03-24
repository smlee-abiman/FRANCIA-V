#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "modbustest.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // Register signal and slot
    QObject *rootObject = engine.rootObjects().first();
    ModBusTest obj;
    QObject::connect(rootObject, SIGNAL(connectSignal()), &obj, SLOT(connectSlot()));
    QObject::connect(rootObject, SIGNAL(readBitSignal()), &obj, SLOT(readBitSlot()));
    QObject::connect(rootObject, SIGNAL(readByteSignal()), &obj, SLOT(readByteSlot()));
    QObject::connect(rootObject, SIGNAL(readWordSignal()), &obj, SLOT(readWordSlot()));
    QObject::connect(rootObject, SIGNAL(readLongSignal()), &obj, SLOT(readLongSlot()));
    QObject::connect(rootObject, SIGNAL(readStringSignal()), &obj, SLOT(readStringSlot()));
    QObject::connect(rootObject, SIGNAL(writeBitSignal()), &obj, SLOT(writeBitSlot()));
    QObject::connect(rootObject, SIGNAL(writeByteSignal()), &obj, SLOT(writeByteSlot()));
    QObject::connect(rootObject, SIGNAL(writeWordSignal()), &obj, SLOT(writeWordSlot()));
    QObject::connect(rootObject, SIGNAL(writeLongSignal()), &obj, SLOT(writeLongSlot()));
    QObject::connect(rootObject, SIGNAL(writeStringSignal()), &obj, SLOT(writeStringSlot()));
    QObject::connect(rootObject, SIGNAL(disconnectSignal()), &obj, SLOT(disconnectSlot()));
    QObject::connect(rootObject, SIGNAL(initializeSignal()), &obj, SLOT(initializeSlot()));
    QObject::connect(rootObject, SIGNAL(syncConnectSignal()), &obj, SLOT(syncConnectSlot()));
    QObject::connect(rootObject, SIGNAL(syncDisconnectSignal()), &obj, SLOT(syncDisconnectSlot()));
    return app.exec();
}
