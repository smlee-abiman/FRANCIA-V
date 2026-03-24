#ifndef CORELIBEXTERNALMEMORYMANGER_H
#define CORELIBEXTERNALMEMORYMANGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>

class CoreLibExternalMemoryManger : public QObject
{
    Q_OBJECT
public:
    explicit CoreLibExternalMemoryManger(QObject *parent = nullptr);
    ~CoreLibExternalMemoryManger();
    bool createFolder(QString folderPath);
    bool deleteFolder(QString folderPath);
    bool copyFile(QString srcFilePath, QString dstFilePath, bool isOverWrite);
    QStringList readFile(QString filePath);
    bool writeFile(QString filePath, QStringList writeDataList);
    bool checkFolder(QString folderPath);
    bool deleteFileInFolder(QString folderPath);
    bool copyFileInFolder(QString srcFolderPath, QString dstFolderPath);
    bool renameFolder(QString folderPath, QString oldFolderName, QString newFolderName);

protected:
    QFile *fileControl;
    QDir  *dirControl;


signals:

};

#endif // CORELIBEXTERNALMEMORYMANGER_H
