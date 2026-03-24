#include "corelibapplicationmanager.h"

#include <QTimer>

CoreLibApplicationManager::CoreLibApplicationManager()
{

}

CoreLibApplicationManager::~CoreLibApplicationManager()
{

}

QString CoreLibApplicationManager::getRunningAplInfo()
{
    return QFileInfo(QCoreApplication::applicationFilePath()).fileName();
}

Q_INVOKABLE void CoreLibApplicationManager::changeApplication(QString nextAppPath, QString args)
{
#ifdef REAL_TARGET
    QString batPath = "/opt/itz3000-gui.sh";

    // .sh編集
    QString batStr =
            "#!/bin/bash\n"
            "export LD_LIBRARY_PATH=/opt/SystemSettingApp/CoreLib:$LD_LIBRARY_PATH\n" +
            nextAppPath;
    if(0 < args.length()){
        batStr += " " + args;
    }
    batStr += "&";

    if (!batPath.isEmpty()) {
        QFile file(batPath);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
            QTextStream stream(&file);
            stream << batStr;
            stream.flush();
            file.close();
        }
    }

    // Application終了
    applicationKill();
#else
    if(QProcess::startDetached(nextAppPath, args.split(' '))){
        // Application終了
        applicationKill();
    }
#endif
}

// 次のアプリ終了時に実行するアプリも指定する（上書きされない前提）
Q_INVOKABLE void CoreLibApplicationManager::changeApplication(QString nextAppPath, QString nextArgs, QString afterNextAppPath, QString afterNextArgs)
{
#ifdef REAL_TARGET
    QString batPath = "/opt/itz3000-gui.sh";

    QFile notFirstFile("/opt/notFirst");
    if(notFirstFile.exists()){
        notFirstFile.remove();
    }

    // .sh編集
    QString batStr =
            "#!/bin/bash\n"
            "export LD_LIBRARY_PATH=/opt/SystemSettingApp/CoreLib:$LD_LIBRARY_PATH\n"
            "FILE=\"/opt/notFirst\"\n"
            "if [ ! -e $FILE ];then\n"
            "   " + nextAppPath;
    if(0 < nextArgs.length()){
        batStr += " " + nextArgs;
    }
    batStr += "&\n";
    batStr +=
            "   touch $FILE\n"
            "else\n"
            "   " + afterNextAppPath;
    if(0 < afterNextArgs.length()){
        batStr += " " + afterNextArgs;
    }
    batStr += "&\n";
    batStr +=
            "fi";

    if (!batPath.isEmpty()) {
        QFile file(batPath);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
            QTextStream stream(&file);
            stream << batStr;
            stream.flush();
            file.close();
        }
    }

    // Application終了
    applicationKill();
#else
    (void)nextAppPath;
    (void)nextArgs;
    (void)afterNextAppPath;
    (void)afterNextArgs;

    applicationKill();
#endif
}

// タイミングによってquitだと終了しないことがあるのでkillする
void CoreLibApplicationManager::applicationKill()
{
    int pid = QCoreApplication::applicationPid();
    qDebug("pid:%d", pid);
    QString cmd = QString("kill %1").arg(pid);
    QProcess process;
    process.startDetached(cmd);
}

Q_INVOKABLE bool CoreLibApplicationManager::isLinux(){

#ifdef Q_OS_LINUX
    return true;
#else
    return false;
#endif
}
