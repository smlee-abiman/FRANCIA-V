#ifndef CORELIBAPPLICATIONMANAGER_H
#define CORELIBAPPLICATIONMANAGER_H

#include <QFileInfo>
#include <QCoreApplication>
#include <QTextStream>
#include <QDir>
#include <QProcess>

class CoreLibApplicationManager: public QObject
{
    Q_OBJECT
public:
    CoreLibApplicationManager();
    ~CoreLibApplicationManager();
    QString getRunningAplInfo();
    Q_INVOKABLE void changeApplication(QString nextAppPath, QString args);
    Q_INVOKABLE void changeApplication(QString nextAppPath, QString nextArgs, QString afterNextAppPath, QString afterNextArgs);
    Q_INVOKABLE bool isLinux();

private:
    void applicationKill();
};

#endif // CORELIBAPPLICATIONMANAGER_H
