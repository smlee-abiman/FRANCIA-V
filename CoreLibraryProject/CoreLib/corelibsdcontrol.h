#ifndef CORELIBSDCONTROL_H
#define CORELIBSDCONTROL_H

#include "corelibexternalmemorymanger.h"
#include <QCoreApplication>
#include <QDir>

class CoreLibSdControl : public CoreLibExternalMemoryManger
{
public:
    CoreLibSdControl(QString aplName = "itz3000-gui");
    ~CoreLibSdControl();
    QVariant getPendantData(QString section, QString key, QVariant defaultValue);
    void setPendantData(QString section, QString key, QVariant controllerData);
    QVariant getControllerData(QString section, QString key, QVariant defaultValue);
    void setControllerData(QString section, QString key, QVariant controllerData);
    QVariant getBackupData(QString section, QString key, QVariant defaultValue);
    void setBackupData(QString section, QString key, QVariant backupData);
    QString getStorageFolderPath();
    QString getProgramFolderPath();
    uint getFolderCount(QString folderPath);
    QFileInfoList getFolderList(QString folderPath);
    QDateTime getFolderLastModified(QFileInfoList folderList, uchar listIndex);
    QFile::Permissions getFolderPermissions(QFileInfoList folderList, uchar listIndex);
    qint64 getFolderSize(QFileInfoList folderList);
    bool setFolderPermissions(QString folderPath, QFlags<QFileDevice::Permission> permission);
    QFileInfoList getFileList(QString folderPath);

private:
#ifdef REAL_TARGET
    const QString mUserHomePath = "/home/root";
    const QString mCommonSettingDirPath = "/opt/pendant-gui/ini";
#else // REAL_TARGET
    const QString mUserHomePath = QDir::homePath();
    const QString mApplicationPath = QCoreApplication::applicationDirPath();
    const QString mCommonSettingDirPath = mApplicationPath + "/../ini";
#endif // REAL_TARGET
    const QString mPendantSettingFileName = "PendantSettings.ini";
#ifdef REAL_TARGET
    const QString mControllerSettingFileName = "ControllerSettings.ini";
#else // REAL_TARGET
    const QString mCommonSettingFileName = "CommonSettings.ini";
#endif // REAL_TARGET
    const QString mPendantBackupFileName = "PendantBackup.ini";
    const QString mPendantSettingFilePath = mCommonSettingDirPath + "/" + mPendantSettingFileName;
#ifdef REAL_TARGET
    const QString mControllerSettingFilePath = mCommonSettingDirPath + "/" + mControllerSettingFileName;
#else // REAL_TARGET
    const QString mCommonSettingFilePath = mCommonSettingDirPath + "/" + mCommonSettingFileName;
#endif // REAL_TARGET
    const QString mPendantBackupFilePath = mCommonSettingDirPath + "/" + mPendantBackupFileName;
    QString mAplName = "";
    QString mStorageFolderPath = "";
    QString mProgramFolderPath = "";
#ifdef REAL_TARGET
    const QString mAplExistPath = "/opt";
#else
    const QString mAplExistPath = mUserHomePath;
#endif
};

#endif // CORELIBSDCONTROL_H
