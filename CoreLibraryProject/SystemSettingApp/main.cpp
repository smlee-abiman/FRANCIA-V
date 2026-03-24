#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "mainwindowmanager.h"
#include <QCommandLineParser>
#include <QDebug>

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDate>

namespace {

QString loggingRootPath() {
#ifdef REAL_TARGET
    auto pendantApplicationDirPath = QString("/opt/pendant-gui/bin");
#else // REAL_TARGET
    auto pendantApplicationDirPath = QString(".");
#endif // REAL_TARGET
    return QString("%1/logs").arg(pendantApplicationDirPath);
}
QString loggingFilePath() {
    auto currentDate = QDate::currentDate().toString("yyyy_MM_dd");
    return QString("%1/%2.log").arg(loggingRootPath(), currentDate);
}

bool makeLoggingDir() {
    auto logDir = QDir(loggingRootPath());
    auto ret = (logDir.exists() || logDir.mkpath("."));
    if (!ret) {
        qWarning() << "** Log directory not found. Output log messages to console only." << logDir.absolutePath();
    }
    return ret;
}

void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    const auto& message = qFormatLogMessage(type, context, msg);

    // output message to console(stdout) & file.
    QTextStream cout(stdout);
    cout << message << endl;
    //
    QFile logFile(loggingFilePath());
    auto ret = logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    if (!ret) {
        cout << "Cannot open log file:" << logFile.fileName() << endl;
        return;
    }
    QTextStream(&logFile) << message << endl;
}

void ConfigureLogging() {
    qSetMessagePattern("%{time [yyyy/MM/dd hh:mm:ss.zzz]}[SysSet][%{type}]%{function}(%{line}): %{message}");
    auto ret = makeLoggingDir();
    if (ret) {
        qInstallMessageHandler(logHandler);
    }
    qDebug() << "** [System Setting App] **";
}

} // namespace

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    ConfigureLogging();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
#ifdef REAL_TARGET
    const QUrl url(QStringLiteral("qrc:/RealTargetMain.qml"));
#else
    const QUrl url(QStringLiteral("qrc:/main.qml"));
#endif
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

#ifdef REAL_TARGET
    engine.rootContext()->setContextProperty("REAL_TARGET", QVariant(true));
#else
    engine.rootContext()->setContextProperty("REAL_TARGET", QVariant(false));
#endif

    // add start
    MainWindowManager mainWindowManager;
    mainWindowManager.initialize(&engine);

    QObject* masterView = engine.rootObjects().first()->findChild<QObject*>("MasterView");
    QObject::connect(masterView, SIGNAL(loaderItemChanged(QString)), &mainWindowManager, SLOT(onLoaderItemChanged(QString)));
    QObject::connect(masterView, SIGNAL(componentDestructed(QString)), &mainWindowManager, SLOT(onComponentDestructed(QString)));

    // parse argument
    QStringList arguments = QCoreApplication::arguments();

    QCommandLineParser parser;
    QString wakeUpPage = "wakeUpPage";
    QString fromAppName = "fromAppName";

    QCommandLineOption wakeUpPageOpt(QStringList() << wakeUpPage, "Wake up page.(Default: settings)", "settings|test", "settings");
    QCommandLineOption fromAppNameOpt(QStringList() << fromAppName, "The name of the app that called this screen.(Default: \"\")", "\"\"|app_name...", "\"\"");

    parser.addHelpOption();
    parser.addOption(wakeUpPageOpt);
    parser.addOption(fromAppNameOpt);

    if(!parser.parse(arguments)){
        // unknown option set.
        qDebug() << parser.errorText();
    }

    QString argFromAppName = parser.value(fromAppName);
    mainWindowManager.setFromAppName(argFromAppName);

    QString argWakeUpPage = parser.value(wakeUpPage);
    QString qmlName;
    if(argWakeUpPage.compare("settings") == 0) {
        qmlName = "SystemSettingPage.qml";
    } else if(argWakeUpPage.compare("test") == 0) {
        qmlName = "TestAndSetPage.qml";
    } else {
        qmlName = "SystemSettingPage.qml";
    }
    mainWindowManager.setLoaderSource(qmlName);
    // add end

    return app.exec();
}
