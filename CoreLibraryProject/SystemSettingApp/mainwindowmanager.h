#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include "folderlistmodel.h"
#include "filelistmodel.h"
#include "systemsettingmanager.h"
#include "testandsetmanager.h"
#include <QSortFilterProxyModel>
#include "touchmanager.h"

class MainWindowManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString loaderSource READ loaderSource WRITE setLoaderSource NOTIFY loaderSourceChanged)
public:
    explicit MainWindowManager(QObject *parent = nullptr);
    void initialize(QQmlApplicationEngine *engine);
    void setFromAppName(QString fromAppName);

    QString loaderSource() const;
    void setLoaderSource(QString);

signals:
    void loaderSourceChanged();

public slots:
    void onLoaderItemChanged(QString source);
    void onComponentDestructed(QString source);

private:
    QQmlApplicationEngine *mQmlApplicationEngine;
    SystemSettingManager mSystemSettingManager;
    TestAndSetManager mTestAndSetManager;
    TouchManager mTouchManager;
    FolderListModel mFolderListModel;
    FileListModel mFileListModel;
    QString mLoaderSource = "";
    QSortFilterProxyModel mSortFilterProxyModel;

};

#endif // MAINWINDOWMANAGER_H
