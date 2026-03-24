#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include <QLoggingCategory>
#include <QStringLiteral>
#include <array>

#include "fileitemmodel.h"
#include "resourcemanager.h"
#include "debugtool.h"
#include "platformhelpers.h"
#include "editprogramviewmodel.h"
#include "appenvironment.h"
#include "corelibdebugdatamanager.h"
#include "controllerviewmodel.h"
#include "commonbarviewmodel.h"
#include "autooperationviewmodel.h"
#include "manualoperationviewmodel.h"
#include "manualautocommonviewmodel.h"
#include "commonbarviewmodel.h"
#include "programfilemanager.h"
#include "setupchangeoperationviewmodel.h"
#include "pointdataviewmodel.h"
#include "programvariableviewmodel.h"
#include "machineparamviewmodel.h"
#include "modbusmanager.h"
#include "ampmonitorviewmodel.h"

#define DEBUG
#define LOG_FILE_ENABLE

namespace {

#ifdef LOG_FILE_ENABLE
void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    static QString latestLogDate = "";
    QString currentDate = QDateTime::currentDateTime().toString("yyyy_MM_dd");

    //logを格納するフォルダーの存在確認
    QString path = "logs";
    QDir logDir(path);
    if(!logDir.exists()){
        QDir currentDir(".");
        currentDir.mkdir(path);
    }

    //コンソール・ファイル出力
    const auto &message = qFormatLogMessage(type, context, msg);
    qSetMessagePattern("%{time [yyyy/MM/dd hh:mm:ss.zzz]}[%{type}]%{function}(%{line}): %{message}");
    QTextStream cout(stdout);
    cout << message << endl;
    QFile file("logs/" + currentDate + ".log");
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

            for(int i = logFiles.length(); i > 10; i--){
                logDir.remove(logFiles[i - 1]);
            }
        }
        latestLogDate = currentDate;
    }
}
#endif

void loadFonts(QGuiApplication &app)
{
#ifdef REAL_TARGET
    // Do not load all fonts because it's unacceptably slow. Use fontconfig
    // instead.
    // TODO: select by locale
    // MODIFIED : font changed by IJ.IY 25.07.04
    app.setFont(QFont(QStringLiteral("NanumSquareB"), 8));
#else
    static const std::array<QString, 7> fileNames = {
        QStringLiteral("NotoSansJP-Regular.otf"), QStringLiteral("NanumSquareB.ttf"),
        QStringLiteral("NotoSans-Regular.ttf"),   QStringLiteral("NotoSansSC-Regular.otf"),
        QStringLiteral("NotoSansTC-Regular.otf"), QStringLiteral("Taviraj-Regular.ttf"),
        QStringLiteral("NotoSansMono-Regular.ttf"),
    };
    const QDir fontsDir(AppEnvironment::getFontFolderPath());
    QFontDatabase fdb;
    for (const auto &name : fileNames) {
        int id = fdb.addApplicationFont(fontsDir.filePath(name));
        if (id == -1) {
            qDebug() << "Error on loading" << name;
        } else {
            QString family = QFontDatabase::applicationFontFamilies(id).at(0);
            app.setFont(QFont(family, 8));
        }
    }
#endif // REAL_TARGET
}

} // namespace

int main(int argc, char *argv[])
{
    // virtualkeyboard
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef LOG_FILE_ENABLE
    qInstallMessageHandler(logHandler);
#endif

    QGuiApplication app(argc, argv);
    app.setOrganizationName("TIETECH");
    app.setOrganizationDomain("tietech.co.jp");
    app.setApplicationName("ITZ-3000");

    AppEnvironment::initialize(app.applicationDirPath());

    // virtualkeyboard
    qputenv("QT_VIRTUALKEYBOARD_LAYOUT_PATH",
            qPrintable(AppEnvironment::getKeybordFolderPath()));

    // Filter out noisy messages by default
    QLoggingCategory::setFilterRules(QStringLiteral("sharedcontrol.debug=false"));

    loadFonts(app);
    qmlRegisterType<FileItemModel>("FileItem", 1, 0, "FileItemModel");
    qmlRegisterType<Enums>("Enums", 1, 0, "Enums");
    qRegisterMetaType<Enums::Pages>("Enums::Pages");
    qRegisterMetaType<stModBusCommunicationData>("stModBusCommunicationData");

#ifdef REAL_TARGET
    const QUrl url(QStringLiteral("qrc:/qml/RealTargetMain.qml"));
#else
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
#endif

    // service objects should live slightly longer than the engine; otherwise
    // the QML would be notified when the objects get deleted and nullified.
    CommunicationThread *communicationThread = nullptr;
    ModbusManager *modbusManager = ModbusManager::create_instance();
    if (modbusManager){
        modbusManager->startCommunication(app.applicationName());
        communicationThread = CommunicationThread::get_instance();
    }
    ProgramFileManager *progfileManager = ProgramFileManager::create_instance();
    if (progfileManager){
        // NOP
    }
    ResourceManager resourceManager(app.applicationDirPath() + QStringLiteral("/.."));
    resourceManager.setApplicationName(app.applicationName());
    // SetupChangeOperation
    SetupChangeOperationViewModel setupChgOpeViewModelObj;
    setupChgOpeViewModelObj.initSetupChangeOperation(resourceManager.getCsvReaderInstance(), resourceManager.language());
    // PointData
    PointDataViewModel pointDataViewModelObj;
    pointDataViewModelObj.initPointData(resourceManager.getCsvReaderInstance(), resourceManager.language());
    // ProgramVariable
    ProgramVariableViewModel progVarViewModel;
    progVarViewModel.initProgVarData(resourceManager.getCsvReaderInstance(), resourceManager.language());
    // MachineParam
    MachineParamViewModel machineParamViewModel;
    machineParamViewModel.initMachineParam(resourceManager.getCsvReaderInstance(), resourceManager.language());
    CommonBarViewModel commonBarViewModelObj(&resourceManager, &setupChgOpeViewModelObj);
    AutoOperationViewModel autoOpeViewModelObj(&resourceManager);
    ManualOperationViewModel manualOpeViewModelObj(&resourceManager);
    ManualAutoCommonViewModel manualAutoCommonViewModelObj(&resourceManager);
    PlatformHelpers platformHelpersObj;
    QString imgPath = resourceManager.imagePath();
    AppEnvironment appEnvironmentObj;
    ControllerViewModel controllerViewModel(&commonBarViewModelObj);
    EditProgramViewModel editProgramViewModel;
    AmpMonitorViewModel ampMonitorViewModel;

    QQmlApplicationEngine engine;
    auto *context = engine.rootContext();

    resourceManager.initialize();

    // PhyKeyManager::SelectSwitchStatus register
    qRegisterMetaType<PhyKeyManager::SelectSwitchStatus>("PhyKeyManager::SelectSwitchStatus");
    // PhyKeyManager::MemblemSwInfo register
    qRegisterMetaType<PhyKeyManager::MemblemSwInfo>("PhyKeyManager::MemblemSwInfo");
    qRegisterMetaType<stUpdateJogInfo>("stUpdateJogInfo");

    if (modbusManager) { context->setContextProperty(QStringLiteral("modbusManager"), modbusManager); }
    context->setContextProperty(QStringLiteral("commonBarViewModel"), &commonBarViewModelObj);
    context->setContextProperty(QStringLiteral("autoOpeViewModel"), &autoOpeViewModelObj);
    context->setContextProperty(QStringLiteral("manualOpeViewModel"), &manualOpeViewModelObj);
    context->setContextProperty(QStringLiteral("manualAutoCommonViewModel"), &manualAutoCommonViewModelObj);
    context->setContextProperty(QStringLiteral("resourceManager"), &resourceManager);
#ifndef REAL_TARGET
    CoreLibDebugDataManager *dbgManager = CoreLibDebugDataManager::create_instance(app.applicationDirPath() + QStringLiteral("/.."));
    if (dbgManager){
        context->setContextProperty(QStringLiteral("dbgManager"), dbgManager);
    }
#endif
    context->setContextProperty(QStringLiteral("platformHelpers"), &platformHelpersObj); 
    context->setContextProperty(QStringLiteral("editProgramViewModel"), &editProgramViewModel);
    context->setContextProperty(QStringLiteral("controllerViewModel"), &controllerViewModel);
    context->setContextProperty(QStringLiteral("appEnvironment"), &appEnvironmentObj);
    if (communicationThread){
        PhyKeyManager *phyKeyManager = communicationThread->getPhyKeyManagerInstance();
        if (phyKeyManager){
            context->setContextProperty(QStringLiteral("phykeymanager"), phyKeyManager);
        }
    }
    context->setContextProperty(QStringLiteral("setupChgOpeViewModel"), &setupChgOpeViewModelObj);
    context->setContextProperty(QStringLiteral("pointDataViewModel"), &pointDataViewModelObj);
    context->setContextProperty(QStringLiteral("progVarViewModel"), &progVarViewModel);
    context->setContextProperty(QStringLiteral("progfilemanager"), progfileManager);
    context->setContextProperty(QStringLiteral("machineParamViewModel"), &machineParamViewModel);
    context->setContextProperty(QStringLiteral("ampMonitorViewModel"), &ampMonitorViewModel);



#ifdef DEBUG
    DebugTool debugtool;
    context->setContextProperty(QStringLiteral("debugtool"), &debugtool);
#endif
#ifdef REAL_TARGET
    engine.rootContext()->setContextProperty("REAL_TARGET", QVariant(true));
#else
    engine.rootContext()->setContextProperty("REAL_TARGET", QVariant(false));
#endif

    QObject::connect(
            &engine, &QQmlApplicationEngine::objectCreated, &app,
            [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            },
            Qt::QueuedConnection);


    
    engine.load(url);

    return app.exec();
}
