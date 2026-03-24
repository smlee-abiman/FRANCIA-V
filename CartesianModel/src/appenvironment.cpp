#include "appenvironment.h"
#include <QDir>

// 버전명 변경 26.03.19 HMI Main version 1.6.9_260318
const QString AppEnvironment::m_mainAppVersion = "1.6.9_260318";
QString AppEnvironment::m_appDirPath;

AppEnvironment::AppEnvironment()
{
}

QString AppEnvironment::getSystemSettingAppDirPathNonRealTarget() {
    QString applicationPath = AppEnvironment::getApplicationFolderPath();
    QDir dir(applicationPath);
    dir.cdUp();
    dir.cdUp();
    dir.cd("bin");
    return dir.path();
};


