#include "mainwindowmanager.h"
#include <QQmlContext>

MainWindowManager::MainWindowManager(QObject *parent) : QObject(parent)
{

}

QString MainWindowManager::loaderSource() const
{
    return mLoaderSource;
}

void MainWindowManager::setLoaderSource(QString source)
{
    mLoaderSource = source;
    emit loaderSourceChanged();
}

void MainWindowManager::initialize(QQmlApplicationEngine *engine)
{
    mQmlApplicationEngine = engine;

    mSortFilterProxyModel.setSourceModel(&mFileListModel);

    mQmlApplicationEngine->rootContext()->setContextProperty("mainWindowManager", this);
    mQmlApplicationEngine->rootContext()->setContextProperty("systemSettingManager", &mSystemSettingManager);
    mQmlApplicationEngine->rootContext()->setContextProperty("testAndSetManager", &mTestAndSetManager);
    mQmlApplicationEngine->rootContext()->setContextProperty("folderListModel", &mFolderListModel);
    mQmlApplicationEngine->rootContext()->setContextProperty("sortFilterFileListModel", &mSortFilterProxyModel);
    mQmlApplicationEngine->rootContext()->setContextProperty("touchManager", &mTouchManager);

    mSystemSettingManager.registerFolderListModel(&mFolderListModel);
    mSystemSettingManager.registerFileListModel(&mFileListModel);
    mSystemSettingManager.registerProxyModel(&mSortFilterProxyModel);
}

void MainWindowManager::setFromAppName(QString fromAppName)
{
    mTestAndSetManager.setFromAppName(fromAppName);
    mSystemSettingManager.setFromAppName(fromAppName);
}

void MainWindowManager::onLoaderItemChanged(QString source)
{
    if((source == nullptr) || (source.length() == 0)) {
        return;
    }

    QObject* masterView = mQmlApplicationEngine->rootObjects().first()->findChild<QObject*>("MasterView");

    if(source.contains("TestAndSetPage.qml")) {
        // Start Test&Set
        QObject::connect(masterView, SIGNAL(inputTestExecute()), &mTestAndSetManager, SLOT(onInputTestExecute()));
        QObject::connect(masterView, SIGNAL(outputTestExecute()), &mTestAndSetManager, SLOT(outputTestExecute()));
        QObject::connect(masterView, SIGNAL(backLightCountDown()), &mTestAndSetManager, SLOT(onBackLightCountDown()));
        QObject::connect(masterView, SIGNAL(backLightCountUp()), &mTestAndSetManager, SLOT(onBackLightCountUp()));
        QObject::connect(masterView, SIGNAL(retClicked(QString, QString)), &mTestAndSetManager, SLOT(onRetClicked(QString, QString)));
        mTestAndSetManager.startTestAndSet();
    } else if(source.contains("SystemSettingPage.qml")) {
        // Start SystemSetting
        QObject::connect(masterView, SIGNAL(folderSelected(int)), &mSystemSettingManager, SLOT(folderSelected(int)));
        QObject::connect(masterView, SIGNAL(folderOpen(QString, int)), &mSystemSettingManager, SLOT(folderOpen(QString, int)));
        QObject::connect(masterView, SIGNAL(fileSelected(int)), &mSystemSettingManager, SLOT(onFileSelected(int)));
        QObject::connect(masterView, SIGNAL(childFolderOpened(int)), &mSystemSettingManager, SLOT(onChildFolderOpened(int)));
        QObject::connect(masterView, SIGNAL(calibrationClicked()), &mSystemSettingManager, SLOT(onCalibrationClicked()));
        QObject::connect(masterView, SIGNAL(executeClicked(QString)), &mSystemSettingManager, SLOT(onExecuteClicked(QString)));
        QObject::connect(masterView, SIGNAL(directionClicked()), &mSystemSettingManager, SLOT(onDirectionClicked()));
        QObject::connect(masterView, SIGNAL(createFolderClicked()), &mSystemSettingManager, SLOT(onCreateFolderClicked()));
        QObject::connect(masterView, SIGNAL(eraceFolderClicked()), &mSystemSettingManager, SLOT(onEraceFolderClicked()));
        QObject::connect(masterView, SIGNAL(displaySelectClicked()), &mSystemSettingManager, SLOT(onDisplaySelectClicked()));
        mSystemSettingManager.startSystemSetting();
    } else if(source.contains("Touch.qml")) {
        // Start Touch
        mTouchManager.startTouch();
    }
}

void MainWindowManager::onComponentDestructed(QString source)
{
    if((source == nullptr) || (source.length() == 0)) {
        return;
    }

    QObject* masterView = mQmlApplicationEngine->rootObjects().first()->findChild<QObject*>("MasterView");

    if(source.contains("TestAndSetPage.qml")) {
        // End Test&Set
        QObject::disconnect(masterView, SIGNAL(inputTestExecute()), &mTestAndSetManager, SLOT(onInputTestExecute()));
        QObject::disconnect(masterView, SIGNAL(outputTestExecute()), &mTestAndSetManager, SLOT(outputTestExecute()));
        QObject::disconnect(masterView, SIGNAL(backLightCountDown()), &mTestAndSetManager, SLOT(onBackLightCountDown()));
        QObject::disconnect(masterView, SIGNAL(backLightCountUp()), &mTestAndSetManager, SLOT(onBackLightCountUp()));
        QObject::disconnect(masterView, SIGNAL(retClicked(QString, QString)), &mTestAndSetManager, SLOT(onRetClicked(QString, QString)));
        mTestAndSetManager.endTestAndSet();
    } else if(source.contains("SystemSettingPage.qml")) {
        // End SystemSetting
        QObject::disconnect(masterView, SIGNAL(folderSelected(int)), &mSystemSettingManager, SLOT(folderSelected(int)));
        QObject::disconnect(masterView, SIGNAL(folderOpen(QString, int)), &mSystemSettingManager, SLOT(folderOpen(QString, int)));
        QObject::disconnect(masterView, SIGNAL(fileSelected(int)), &mSystemSettingManager, SLOT(onFileSelected(int)));
        QObject::disconnect(masterView, SIGNAL(childFolderOpened(int)), &mSystemSettingManager, SLOT(onChildFolderOpened(int)));
        QObject::disconnect(masterView, SIGNAL(calibrationClicked()), &mSystemSettingManager, SLOT(onCalibrationClicked()));
        QObject::disconnect(masterView, SIGNAL(executeClicked(QString)), &mSystemSettingManager, SLOT(onExecuteClicked(QString)));
        QObject::disconnect(masterView, SIGNAL(directionClicked()), &mSystemSettingManager, SLOT(onDirectionClicked()));
        QObject::disconnect(masterView, SIGNAL(createFolderClicked()), &mSystemSettingManager, SLOT(onCreateFolderClicked()));
        QObject::disconnect(masterView, SIGNAL(eraceFolderClicked()), &mSystemSettingManager, SLOT(onEraceFolderClicked()));
        QObject::disconnect(masterView, SIGNAL(displaySelectClicked()), &mSystemSettingManager, SLOT(onDisplaySelectClicked()));
        mSystemSettingManager.endSystemSetting();
    } else if(source.contains("Touch.qml")) {
        // End Touch
        mTouchManager.endTouch();
    }
}
