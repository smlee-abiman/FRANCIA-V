#ifndef APPENVIRONMENT_H
#define APPENVIRONMENT_H

#include <QString>
#include <QSysInfo>
#include <QObject>

#include "corelibsysteminformation.h"

class AppEnvironment : public QObject
{
    Q_OBJECT

public:
    AppEnvironment();

    inline static void initialize(QString appDirPath){
        m_appDirPath = appDirPath;
    }

    inline static QString getApplicationFolderPath(){
        return m_appDirPath;
    }

    inline static QString getImageFolderPath(){
        return m_appDirPath + QStringLiteral("/../images");
    }

    inline static QString getFontFolderPath(){
        return m_appDirPath + QStringLiteral("/../fonts");
    }

    inline static QString getWorkProgFolderPath(){
        return m_appDirPath + QStringLiteral("/../wprog_folder");
    }

    inline static QString getKeybordLayoutFolderPath(){
        return m_appDirPath + QStringLiteral("/../keybord/layout");
    }

    inline static QString getKeybordStyleFolderPath(){
        return m_appDirPath + QStringLiteral("/../keybord/style");
    }

    inline static QString getSettingsFolderPath(){
        return m_appDirPath + QStringLiteral("/../settings");
    }

    inline static QString getProgFolderPath(){
        return getSettingsFolderPath() + QStringLiteral("/prog_folder");
    }

    inline static QString getSetfolderPath(){
        return getSettingsFolderPath() + QStringLiteral("/set_folder");
    }

    inline static QString getMsgFolderPath(){
        return getSettingsFolderPath() + QStringLiteral("/msg_folder");
    }

    inline static QString getStorageFolderPath(){
        return getSettingsFolderPath() + QStringLiteral("/storage_folder");
    }

    inline static QString getIniFolderPath(){
        return m_appDirPath + QStringLiteral("/../ini");
    }

    inline static QString getNcpFolderPath(){
        return m_appDirPath + QStringLiteral("/../ncp");
    }

    inline static QString getFtpFolderPath(){
        return m_appDirPath + QStringLiteral("/../ftp");
    }

    inline static QString getExeFolderPath(){
        return m_appDirPath + QStringLiteral("/../exe");
    }

    Q_INVOKABLE inline static QString getProductVersion(){
        return QSysInfo::prettyProductName();
    }

    Q_INVOKABLE inline static QString getQtVersion(){
        return "Qt " + QString(qVersion());
    }

    Q_INVOKABLE inline static QString getCoreAppVersion(){
        return "CoreApp " + CorelibSystemInformation::getCoreAppVersion();
    }

    Q_INVOKABLE inline static QString getMainAppVersion(){
        return "MainApp " + m_mainAppVersion;
    }

    Q_INVOKABLE static QString getSystemSettingAppDirPathNonRealTarget();


signals:

private:
    const static QString m_mainAppVersion;
    static QString m_appDirPath;
};

#endif // APPENVIRONMENT_H
