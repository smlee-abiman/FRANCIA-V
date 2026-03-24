#include <QDir>
#ifdef SPLASH_SCREEN_VALID
#include <QApplication>
#else // SPLASH_SCREEN_VALID
#include <QGuiApplication>
#endif // SPLASH_SCREEN_VALID
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include <QLoggingCategory>
#include <QStringLiteral>
#include <QPixmap>
#ifdef SPLASH_SCREEN_VALID
#include <QSplashScreen>
#include <QMainWindow>
#endif // SPLASH_SCREEN_VALID

#include <array>
#include <QCommandLineParser>

#include "pendantapplication.h"

#include "fileitemmodel.h"
#include "resourcemanager/resourcemanager.h"
#include "debugtool.h"
#include "platformhelpers.h"
#include "appenvironment.h"
#include "corelibdebugdatamanager.h"
#include "viewmodel/axisinfo.h"
#include "communication/programfilemanager.h"
#include "communication/ftpmanager.h"
#include "communication/usbmanager.h"
#include "communication/transferfilethread.h"

#include "viewmodel/originviewmodel.h"
#include "viewmodel/commonviewmodel.h"
#include "viewmodel/teachingview/teachingviewmodel.h"
#include "viewmodel/manualview/manualviewmodel.h"
#include "viewmodel/autoview/autoviewmodel.h"
#include "viewmodel/monitorview/monitorviewmodel.h"
#include "viewmodel/functionview/functionviewmodel.h"
#include "viewmodel/functionview/areasetview/areasetviewmodel.h"
#include "viewmodel/functionview/maintainview/maintainviewmodel.h"
#include "viewmodel/historyview/alarmhistoryviewmodel.h"
#include "viewmodel/historyview/operationhistoryviewmodel.h"
#include "viewmodel/historyview/operationhistorydata.h"
#include "viewmodel/historyview/iohistoryviewmodel.h"
#include "variableviewmodel.h"
#include "setupchangeviewmodel.h"
#include "externalmemoryviewmodel.h"

#include "memorylogger.h"
#include "viewmodel/monitorview/graphcanvas.h"
#include "components/linescanvas.h"
#include "operationhistorylist.h"
#include "operationrecord.h"
#include "axispointinfo.h"
#include "splashprogresscounter.h"

namespace {

#ifdef LOG_FILE_ENABLE
void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    static QString latestLogDate = "";
    QString currentDate = QDateTime::currentDateTime().toString("yyyy_MM_dd");

    //logを格納するフォルダーの存在確認
    QString applicationPath = QCoreApplication::applicationDirPath();
    QString logPath = applicationPath + "/logs";
    QDir logDir(logPath);
    if(!logDir.exists()){
        QDir applicationDir(applicationPath);
        applicationDir.mkdir(logPath);
    }

    //コンソール・ファイル出力
    qSetMessagePattern("%{time [yyyy/MM/dd hh:mm:ss.zzz]}[%{type}]%{function}(%{line}): %{message}");
    const auto &message = qFormatLogMessage(type, context, msg);
    QTextStream cout(stdout);
    cout << message << endl;
    QFile file(logPath + "/" + currentDate + ".log");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        cout << "Cannot open log file:" << file.fileName();
        return;
    }
    QTextStream(&file) << message << endl;

    //起動時と日付が変わったときにlogファイルが11件以上あったら古いものから消す
    if(latestLogDate != currentDate){
        if(logDir.exists()){
            QStringList filters;
            filters << "*.log";
            logDir.setNameFilters(filters);

            QList<QString> logFiles = logDir.entryList(QDir::Files, QDir::Time);

            for(int i = logFiles.length(); i > 6; i--){
                logDir.remove(logFiles[i - 1]);
            }
        }
        latestLogDate = currentDate;
    }
}
#endif

#ifdef LOG_MEMORY_ENABLE
void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    qSetMessagePattern("%{time [yyyy/MM/dd hh:mm:ss.zzz]}[%{type}]%{function}(%{line}): %{message}");
    const auto &message = qFormatLogMessage(type, context, msg);
    MemoryLogger::instance()->addLog(message);
}
#endif //LOG_MEMORY_ENABLE

void readCommandLineArguments(int argc, char *argv[], int *startupDlgStatus)
{
    QStringList qArgv;
    for(int i = 0; i < argc; i++){
        QString temp(argv[i]);
        if((temp.left(1) == "-")
                && !((temp.mid(1,1) == "L") || (temp.mid(1,1) == "D"))){
            continue;
        }
        qArgv.append(argv[i]);
    }

    QCommandLineParser parser;
    //set log level option
    QCommandLineOption logOpt("L", "Specify log output level.", "debug|info|warning|critical|none", "none");
    parser.addOption(logOpt);
    QCommandLineOption dlgOpt("D", "Specify show dialog.", "restart|none", "none");
    parser.addOption(dlgOpt);
    if(!parser.parse(qArgv)){
        return;
    }
    parser.process(qArgv);

    //log出力レベル設定オプション -L
    //debug: 全てのログを出力
    //info: info, warning, criticalのログを出力
    //warning: warning, criticalのログを出力
    //critical: criticalのログを出力
    //none: 全て出力しない
    //デフォルト値: 何も設定しない
    if(parser.isSet(logOpt)){
        QString logopt = parser.value(logOpt);
        if(logopt == "none"){
            qputenv("QT_LOGGING_RULES", "*=false");
        }else if(logopt == "debug"){
            qputenv("QT_LOGGING_RULES", "");
        }else if(logopt == "info"){
            qputenv("QT_LOGGING_RULES", "*.debug=false");
        }else if(logopt == "warning"){
            qputenv("QT_LOGGING_RULES", "*.debug=false;*.info=false");
        }else if(logopt == "critical"){
            qputenv("QT_LOGGING_RULES", "*.debug=false;*.info=false;*.warning=false");
        }else{
            QTextStream(stderr) << "-L option <" << logopt << "> is illegal." << endl;
        }
    }

    //起動時dialog表示オプション -D
    //restart: 再起動を促すdialog表示
    //none: 全て出力しない
    //デフォルト値: 何も設定しない
    if(parser.isSet(dlgOpt)){
        QString dlgopt = parser.value(dlgOpt);
        if(dlgopt == "none"){
            *startupDlgStatus = 0;
        }else if(dlgopt == "restart"){
            *startupDlgStatus = 1;
        }
    }
}

void loadFonts(QGuiApplication &app)
{
#ifdef REAL_TARGET
    // Do not load all fonts because it's unacceptably slow. Use fontconfig
    // instead.
    // TODO: select by locale
    app.setFont(QFont(QStringLiteral("NanumSquare Bold"), 8));
#else
    static const std::array<QString, 7> fileNames = {
        QStringLiteral("NotoSansJP-Regular.otf"), QStringLiteral("NanumSquare Bold.ttf"),
        QStringLiteral("NotoSans-Regular.ttf"),   QStringLiteral("NotoSansSC-Regular.otf"),
        QStringLiteral("NotoSansTC-Regular.otf"), QStringLiteral("Taviraj-Regular.ttf"),
        QStringLiteral("NotoSansMyanmar-Regular.ttf"),
    };
    const QDir fontsDir(AppEnvironment::getFontFolderPath());
    QFontDatabase fdb;
    for (const auto &name : fileNames) {
        int id = fdb.addApplicationFont(fontsDir.filePath(name));
        if (id == -1) {
            qDebug() << "Error on loading" << name;
        }
    }
    //中国語フォントをデフォルトにする
    // korea font change -> 25.6.18 IJ.YI
    app.setFont(QFont(QStringLiteral("NanumSquare Bold"), 8));

#endif // REAL_TARGET
}
} // namespace

int main(int argc, char *argv[])
{
    // virtualkeyboard
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#if defined(LOG_FILE_ENABLE) || defined(LOG_MEMORY_ENABLE)
    qInstallMessageHandler(logHandler);
#endif

    int startupDlgStatus = -1;

    readCommandLineArguments(argc, argv, &startupDlgStatus);

    PendantApplication* pApp = PendantApplication::initializeInstance(argc, argv);
    PendantApplication& app = *pApp;
    qDebug("%s:%d App Start:", __func__, __LINE__);
    app.setOrganizationName("TIETECH");
    app.setOrganizationDomain("tietech.co.jp");
    app.setApplicationName("Pendant");

#ifdef SPLASH_SCREEN_VALID
    // Show Splash Screen
    QPixmap pixmap(":/images/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();
#endif // SPLASH_SCREEN_VALID

    AppEnvironment::initialize(app.applicationDirPath());

    // virtualkeyboard layout path
    qputenv("QT_VIRTUALKEYBOARD_LAYOUT_PATH",
            qPrintable(AppEnvironment::getKeybordLayoutFolderPath()));
    qputenv("QML2_IMPORT_PATH",
            qPrintable(AppEnvironment::getKeybordStyleFolderPath()));
    qputenv("QT_VIRTUALKEYBOARD_STYLE",
            qPrintable("3GPendant"));

    // Filter out noisy messages by default
    QLoggingCategory::setFilterRules(QStringLiteral("sharedcontrol.debug=false"));

    loadFonts(app);
    qmlRegisterType<FileItemModel>("FileItem", 1, 0, "FileItemModel");
    qmlRegisterType<AlarmInfo>("AlarmInfo", 1, 0, "AlarmInfo");
    qRegisterMetaType<AlarmInfo::Type>("AlarmInfo::Type");
    qmlRegisterType<Enums>("Enums", 1, 0, "Enums");
    qRegisterMetaType<Enums::Pages>("Enums::Pages");
    qRegisterMetaType<Enums::MainScreenId>("Enums::MainScreenId");
    qRegisterMetaType<Enums::SubScreenId>("Enums::SubScreenId");
    qRegisterMetaType<Enums::MemblemKeyNo>("Enums::MemblemKeyNo");
    qRegisterMetaType<stModBusCommunicationData>("stModBusCommunicationData");
    qRegisterMetaType<CommunicationThread::OneShotReadModBusCaller>("CommunicationThread::OneShotReadModBusCaller");
    qRegisterMetaType<Enums::LedInfo>("Enums::LedInfo");
    qRegisterMetaType<Enums::AuthorityInfo>("Enums::AuthorityInfo");
    qRegisterMetaType<Enums::DateTimeInfo>("Enums::DateTimeInfo");
    qRegisterMetaType<Enums::ManualSpeedValue>("Enums::ManualSpeedValue");
    qRegisterMetaType<Enums::AutoSpeedValue>("Enums::AutoSpeedValue");
    qRegisterMetaType<Enums::Axis>("Enums::Axis");
    qRegisterMetaType<Enums::AreaType>("Enums::AreaType");
    qRegisterMetaType<Enums::FtpCommand>("Enums::FtpCommand");
    qRegisterMetaType<Enums::VersionNo>("Enums::VersionNo");
    qRegisterMetaType<Enums::PopupDialogStatus>("Enums::PopupDialogStatus");
    qmlRegisterType<GraphCanvas>("GraphCanvas", 1, 0, "GraphCanvas");
    qmlRegisterType<LinesCanvas>("LinesCanvas", 1, 0, "LinesCanvas");
    qRegisterMetaType<Enums::UsbDownloadType>("Enums::UsbDownloadType");

#ifdef REAL_TARGET
    const QUrl url(QStringLiteral("qrc:/qml/RealTargetMain.qml"));
#else
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
#endif

    // service objects should live slightly longer than the engine; otherwise
    // the QML would be notified when the objects get deleted and nullified.
    qDebug("%s:%d Resource Mng Start:", __func__, __LINE__);
    auto resourceManager = new ResourceManager(app.applicationDirPath() + QStringLiteral("/.."));
    resourceManager->setApplicationName(app.applicationName());
    CommunicationThread *communicationThread = nullptr;
    qDebug("%s:%d Modbus Mng Start:", __func__, __LINE__);
    auto *modbusManager = ModbusManager::create_instance();
    qDebug("%s:%d Ftp Mng Start:", __func__, __LINE__);
    auto *ftpManager = FtpManager::create_instance();
    qDebug("%s:%d Ftp Com Start:", __func__, __LINE__);
    if (ftpManager){
#ifdef ENABLE_FTP_COM_ASYNC
        ftpManager->startFtpCommunicationThread();
#else // ENABLE_FTP_COM_ASYNC
        ftpManager->setFtpTimeOut(resourceManager);
        ftpManager->startFtpCommunication();
#endif // ENABLE_FTP_COM_ASYNC
    }
    qDebug("%s:%d Ftp Com-start End:", __func__, __LINE__);
    if (modbusManager){
        modbusManager->setTimeoutAndRetry(resourceManager);
        qDebug("%s:%d Modbus Com Start:", __func__, __LINE__);
        modbusManager->startCommunication(app.applicationName());             // async modbus com thread start
        qDebug("%s:%d Modbus Com-start End:", __func__, __LINE__);
        communicationThread = CommunicationThread::get_instance();
    }
    auto *usbManager = UsbManager::create_instance();
    if(usbManager) { usbManager->initialize(); }
    auto *progfileManager = ProgramFileManager::create_instance();
    auto *transferFileThread = TransferFileThread::create_instance();
    if (communicationThread && communicationThread->getPhyKeyManagerInstance()){
        communicationThread->getPhyKeyManagerInstance()->initJogSetting(resourceManager);
        communicationThread->getPhyKeyManagerInstance()->initResourceManager(resourceManager);
    }
    auto platformHelpersObj = new PlatformHelpers();
    auto appEnvironmentObj = new AppEnvironment();

    QQmlApplicationEngine engine;
    auto *context = engine.rootContext();

    resourceManager->initialize();

    // PhyKeyManager::SelectSwitchStatus register
    qRegisterMetaType<PhyKeyManager::SelectSwitchStatus>("PhyKeyManager::SelectSwitchStatus");
    // PhyKeyManager::MemblemSwInfo register
    qRegisterMetaType<PhyKeyManager::MemblemSwInfo>("PhyKeyManager::MemblemSwInfo");
    qRegisterMetaType<stUpdateJogInfo>("stUpdateJogInfo");
    qRegisterMetaType<ProgCompileErrInfo>("ProgCompileErrInfo");
    qDebug("%s:%d ViewModel Create Start:", __func__, __LINE__);

    auto axisinfo = AxisInfo::create_instance();
    auto axisPointInfo = AxisPointInfo::create_instance(resourceManager);
    axisPointInfo->init();
    auto operationHistoryList = new OperationHistoryList();
    context->setContextProperty(QStringLiteral("operationHistoryList"), operationHistoryList);

    auto operationRecord = OperationRecord::create_instance(operationHistoryList);
    context->setContextProperty(QStringLiteral("operationRecord"), operationRecord);

    auto commonViewModel = new CommonViewModel(resourceManager, communicationThread, modbusManager);
    context->setContextProperty(QStringLiteral("commonViewModel"), commonViewModel);
    if(0 < startupDlgStatus){
        commonViewModel->setStartupStatus(startupDlgStatus);
    }

    operationRecord->setCommonView(commonViewModel);

    auto originViewModel = new OriginViewModel(resourceManager, communicationThread);
    context->setContextProperty(QStringLiteral("originViewModel"), originViewModel);

    auto externalMemoryViewModel = new ExternalMemoryViewModel(resourceManager, communicationThread, commonViewModel);
    context->setContextProperty(QStringLiteral("externalMemoryViewModel"), externalMemoryViewModel);

    auto teachingViewModel = new TeachingViewModel(resourceManager, communicationThread, commonViewModel, externalMemoryViewModel);
    context->setContextProperty(QStringLiteral("teachingViewModel"), teachingViewModel);

    auto manualViewModel = new ManualViewModel(resourceManager, communicationThread, commonViewModel);
    context->setContextProperty(QStringLiteral("manualViewModel"), manualViewModel);

    auto autoViewModel = new AutoViewModel(resourceManager, communicationThread, commonViewModel);
    context->setContextProperty(QStringLiteral("autoViewModel"), autoViewModel);

    auto monitorViewModel = new MonitorViewModel(resourceManager, communicationThread, commonViewModel);
    context->setContextProperty(QStringLiteral("monitorViewModel"), monitorViewModel);

    auto functionViewModel = new FunctionViewModel(resourceManager, communicationThread, commonViewModel);
    context->setContextProperty(QStringLiteral("functionViewModel"), functionViewModel);

    auto areaSetViewModel = new AreaSetViewModel(resourceManager, communicationThread, commonViewModel);
    context->setContextProperty(QStringLiteral("areaSetViewModel"), areaSetViewModel);

    auto maintainViewModel = new MaintainViewModel(resourceManager, communicationThread);
    context->setContextProperty(QStringLiteral("maintainViewModel"), maintainViewModel);

    auto variableViewmodel = new VariableViewModel(resourceManager,communicationThread);
    context->setContextProperty(QStringLiteral("variableViewmodel"),variableViewmodel);

    auto setupChangeViewModel = new SetupChangeViewModel(resourceManager, communicationThread, commonViewModel);
    context->setContextProperty(QStringLiteral("setupChangeViewModel"), setupChangeViewModel);

    auto alarmHistoryViewModel = new AlarmHistoryViewModel(resourceManager, communicationThread, commonViewModel);
    context->setContextProperty(QStringLiteral("alarmHistoryViewModel"), alarmHistoryViewModel);

    auto operationHistoryViewModel = new OperationHistoryViewModel(resourceManager, communicationThread, operationHistoryList, functionViewModel);
    context->setContextProperty(QStringLiteral("operationHistoryViewModel"), operationHistoryViewModel);

    auto ioHistoryViewModel = new IoHistoryViewModel(resourceManager, communicationThread);
    context->setContextProperty(QStringLiteral("ioHistoryViewModel"), ioHistoryViewModel);

    auto splashScreenCounter = new SplashProgressCounter();
    context->setContextProperty(QStringLiteral("splashScreenCounter"), splashScreenCounter);

    qmlRegisterType<AxisOperateViewModel>("AxisOperateViewModel", 1, 0, "AxisOperateViewModel");
    qmlRegisterType<OperationHistoryData>("OperationHistoryData", 1, 0, "OperationHistoryData");

    if (modbusManager) { context->setContextProperty(QStringLiteral("modbusManager"), modbusManager); }
    if (ftpManager) { context->setContextProperty(QStringLiteral("ftpManager"), ftpManager); }
    if (usbManager) { context->setContextProperty(QStringLiteral("usbManager"), usbManager); }
    if (transferFileThread) { context->setContextProperty(QStringLiteral("transferFileThread"), transferFileThread); }
    context->setContextProperty(QStringLiteral("resourceManager"), resourceManager);
#ifndef REAL_TARGET
    CoreLibDebugDataManager *dbgManager = CoreLibDebugDataManager::create_instance(app.applicationDirPath() + QStringLiteral("/.."));
    if (dbgManager){
        context->setContextProperty(QStringLiteral("dbgManager"), dbgManager);
        QObject::connect(dbgManager, SIGNAL(memoryDataIsUpdated()), originViewModel, SLOT(update()));
        QObject::connect(dbgManager, SIGNAL(memoryDataIsUpdated()), commonViewModel, SLOT(update()));
    }
#endif
    context->setContextProperty(QStringLiteral("platformHelpers"), platformHelpersObj);
    context->setContextProperty(QStringLiteral("appEnvironment"), appEnvironmentObj);
    if (communicationThread){
        PhyKeyManager *phyKeyManager = communicationThread->getPhyKeyManagerInstance();
        if (phyKeyManager){
            context->setContextProperty(QStringLiteral("phyKeyManager"), phyKeyManager);
            app.startPhyKeyMonitoring(*phyKeyManager);
        }
        app.startCommunicationMonitoring(*communicationThread);
    }
    context->setContextProperty(QStringLiteral("progfilemanager"), progfileManager);

#ifdef DEBUG
    DebugTool debugtool;
    context->setContextProperty(QStringLiteral("debugtool"), &debugtool);
#endif
#ifdef REAL_TARGET
    engine.rootContext()->setContextProperty("REAL_TARGET", QVariant(true));
#else
    engine.rootContext()->setContextProperty("REAL_TARGET", QVariant(false));
#endif
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    engine.rootContext()->setContextProperty("DRV_MONITOR_GRAPH_10MS_CYCLE", QVariant(true));
#else // DRV_MONITOR_GRAPH_10MS_CYCLE
    engine.rootContext()->setContextProperty("DRV_MONITOR_GRAPH_10MS_CYCLE", QVariant(false));
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    engine.rootContext()->setContextProperty("ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC", QVariant(true));
#else // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    engine.rootContext()->setContextProperty("ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC", QVariant(false));
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
#ifdef ENABLE_FTP_COM_ASYNC
    engine.rootContext()->setContextProperty("ENABLE_FTP_COM_ASYNC", QVariant(true));
#else // ENABLE_FTP_COM_ASYNC
    engine.rootContext()->setContextProperty("ENABLE_FTP_COM_ASYNC", QVariant(false));
#endif // ENABLE_FTP_COM_ASYNC

    QObject::connect(
            &engine, &QQmlApplicationEngine::objectCreated, &app,
            [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            },
            Qt::QueuedConnection);

    qDebug("%s:%d Qml Load Start:", __func__, __LINE__);
    engine.load(url);
#ifdef SPLASH_SCREEN_VALID
    // Close splash
    QMainWindow window;
    window.show();
    splash.finish(&window);
    window.close();
#endif // SPLASH_SCREEN_VALID
    qDebug("%s:%d Qml Load End:", __func__, __LINE__);
    if(usbManager) { usbManager->start(); }
    int ret = app.exec();
    engine.quit();
    delete context;
    delete externalMemoryViewModel;
    delete setupChangeViewModel;
    delete variableViewmodel;
    delete maintainViewModel;
    delete areaSetViewModel;
    delete functionViewModel;
    delete monitorViewModel;
    delete autoViewModel;
    delete manualViewModel;
    delete teachingViewModel;
    delete commonViewModel;
    delete originViewModel;
    delete axisPointInfo;
    axisinfo->destroy_instance();
    delete appEnvironmentObj;
    delete platformHelpersObj;
    delete resourceManager;
    splashScreenCounter->deleteLater();
    TransferFileThread::destroy_instance();
    ProgramFileManager::destroy_instance();
    ftpManager->stopFtpCommunication();
    ftpManager->terminate();
    FtpManager::destroy_instance();
    if (usbManager) {
        usbManager->quit();
        usbManager->wait();
        usbManager->terminate();
    }
    UsbManager::destroy_instance();
    modbusManager->stopCommunication();
    ModbusManager::destroy_instance();
    return ret;
}
