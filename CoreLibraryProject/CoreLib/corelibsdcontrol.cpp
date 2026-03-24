#include "corelibsdcontrol.h"
#include <QSettings>

CoreLibSdControl::CoreLibSdControl(QString aplName)
{
    mAplName = aplName;
    mStorageFolderPath = mAplExistPath + "/" + mAplName + "/settings" + "/" + "storage_folder";
    mProgramFolderPath = mAplExistPath + "/" + mAplName + "/settings" + "/" + "prog_folder";
}

CoreLibSdControl::~CoreLibSdControl()
{

}

QVariant CoreLibSdControl::getPendantData(QString section, QString key, QVariant defaultValue)
{
    QSettings pendantDataFile(mPendantSettingFilePath, QSettings::IniFormat);
    QVariant value;

    pendantDataFile.setIniCodec("UTF-8");
    pendantDataFile.beginGroup(section);
    value = pendantDataFile.value(key, defaultValue);
    pendantDataFile.endGroup();

    return value;
}

void CoreLibSdControl::setPendantData(QString section, QString key, QVariant controllerData)
{
   // qDebug() << controllerData;  // todo: debug.
   QSettings pendantDataFile(mPendantSettingFilePath, QSettings::IniFormat);
   pendantDataFile.setIniCodec("UTF-8");
   pendantDataFile.beginGroup(section);
   pendantDataFile.setValue(key, controllerData);
   pendantDataFile.endGroup();
}

QVariant CoreLibSdControl::getControllerData(QString section, QString key, QVariant defaultValue)
{
#ifdef REAL_TARGET
    QSettings controllerDataFile(mControllerSettingFilePath, QSettings::IniFormat);
#else // REAL_TARGET
    QSettings controllerDataFile(mCommonSettingFilePath, QSettings::IniFormat);
#endif // REAL_TARGET
    QVariant value;

    controllerDataFile.setIniCodec("UTF-8");
    controllerDataFile.beginGroup(section);
    value = controllerDataFile.value(key, defaultValue);
    controllerDataFile.endGroup();

    return value;
}

void CoreLibSdControl::setControllerData(QString section, QString key, QVariant controllerData)
{
    //qDebug() << controllerData;
#ifdef REAL_TARGET
    //qDebug() << mControllerSettingFilePath;
    QSettings controllerDataFile(mControllerSettingFilePath, QSettings::IniFormat);
#else // REAL_TARGET
    //qDebug() << mCommonSettingFilePath;
    QSettings controllerDataFile(mCommonSettingFilePath, QSettings::IniFormat);
#endif // REAL_TARGET
    controllerDataFile.setIniCodec("UTF-8");

    controllerDataFile.beginGroup(section);
    controllerDataFile.setValue(key, controllerData);
    controllerDataFile.endGroup();
}

QVariant CoreLibSdControl::getBackupData(QString section, QString key, QVariant defaultValue)
{
    QSettings backupFile(mPendantBackupFilePath, QSettings::IniFormat);
    QVariant value;

    backupFile.setIniCodec("UTF-8");
    backupFile.beginGroup(section);
    value = backupFile.value(key, defaultValue);
    backupFile.endGroup();

    return value;
}

void CoreLibSdControl::setBackupData(QString section, QString key, QVariant backupData)
{
    QSettings backupFile(mPendantBackupFilePath, QSettings::IniFormat);
    backupFile.setIniCodec("UTF-8");

    backupFile.beginGroup(section);
    backupFile.setValue(key, backupData);
    backupFile.endGroup();
}

QString CoreLibSdControl::getStorageFolderPath()
{
    return mStorageFolderPath;
}

QString CoreLibSdControl::getProgramFolderPath()
{
    return mProgramFolderPath;
}

uint CoreLibSdControl::getFolderCount(QString folderPath)
{
    uint dirCount = 0;
    QDir dir(folderPath);
    dir.setFilter(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
    dirCount = dir.count();
    return dirCount;
}

QFileInfoList CoreLibSdControl::getFolderList(QString folderPath)
{
    QDir dir(folderPath);
    return dir.entryInfoList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
}

QDateTime CoreLibSdControl::getFolderLastModified(QFileInfoList folderList, uchar listIndex)
{
    return folderList.at((int)listIndex).lastModified();
}

QFile::Permissions CoreLibSdControl::getFolderPermissions(QFileInfoList folderList, uchar listIndex)
{
    //return folderList.at((int)listIndex).isWritable();
    return folderList.at((int)listIndex).permissions();
}

qint64 CoreLibSdControl::getFolderSize(QFileInfoList folderList)
{
    qint64 folderCapacity = 0;
    for (int i = 0; i < folderList.count(); i++){
        folderCapacity += folderList.at(i).size();
    }
    return folderCapacity;
}

bool CoreLibSdControl::setFolderPermissions(QString folderPath, QFlags<QFileDevice::Permission> permission)
{
    bool ret = false;
    QFile file(folderPath);
    ret = file.setPermissions(permission);
    return ret;
}

QFileInfoList CoreLibSdControl::getFileList(QString folderPath)
{
    QDir dir(folderPath);
    return dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);
}
