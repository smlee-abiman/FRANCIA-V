#include "corelibexternalmemorymanger.h"
#include <QTextStream>

CoreLibExternalMemoryManger::CoreLibExternalMemoryManger(QObject *parent) : QObject(parent)
{

}

CoreLibExternalMemoryManger::~CoreLibExternalMemoryManger()
{

}

bool CoreLibExternalMemoryManger::createFolder(QString folderPath)
{
    QDir targetFolder(folderPath);
    return targetFolder.mkpath("."); // If the path already exists when this function is called, it will return true.
}

bool CoreLibExternalMemoryManger::deleteFolder(QString folderPath)
{
    QDir targetFolder(folderPath);
    return targetFolder.removeRecursively(); // If the directory was already removed, the method returns true (expected result already reached).
}

bool CoreLibExternalMemoryManger::copyFile(QString srcFilePath, QString dstFilePath, bool isOverWrite)
{
    QFile srcFile(srcFilePath);
    if(!srcFile.exists()) {
        return false;  // srcFile not found.
    }

    QFile dstFile(dstFilePath);
    if(dstFile.exists()) {
        if(!isOverWrite) {
            return false;  // dstFile already exists.
        }

        dstFile.remove();
    }

    return QFile::copy(srcFilePath, dstFilePath);
}

QStringList CoreLibExternalMemoryManger::readFile(QString filePath)
{
    QStringList readDataList;
    QFile targetFile(filePath);
    if(!targetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return readDataList;
    }

    QTextStream in(&targetFile);
    while(!in.atEnd()) {
        QString line = in.readLine();
        readDataList.append(line);
    }

    targetFile.close();

    return readDataList;
}

bool CoreLibExternalMemoryManger::writeFile(QString filePath, QStringList writeDataList)
{
    QFile targetFile(filePath);
    if(!targetFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&targetFile);
    foreach (QString line, writeDataList) {
        out << line;
    }

    targetFile.close();
    return true;
}

bool CoreLibExternalMemoryManger::checkFolder(QString folderPath)
{
    QDir targetDir(folderPath);
    return targetDir.exists();
}

bool CoreLibExternalMemoryManger::deleteFileInFolder(QString folderPath)
{
    bool ret = true;

    QDir dir(folderPath);
    // get filelist in directory
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot );
    QFileInfoList list = dir.entryInfoList();
    // Delete the acquired files one by one
    foreach (QFileInfo finfo, list) {
        ret = QFile::remove(finfo.filePath());
    }
    return ret;
}

bool CoreLibExternalMemoryManger::copyFileInFolder(QString srcFolderPath, QString dstFolderPath)
{
    bool ret = false;

    QDir dir(srcFolderPath);
    // get filelist in directory
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);
    for (int i = 0; i < list.count(); i++){
        ret = QFile::copy(srcFolderPath + QDir::separator() + list.at(i).fileName(),
                            dstFolderPath + QDir::separator() + list.at(i).fileName());
    }
    return ret;
}

bool CoreLibExternalMemoryManger::renameFolder(QString folderPath, QString oldFolderName, QString newFolderName)
{
    bool ret = false;

    QDir dir(folderPath);
    ret = dir.rename(oldFolderName, newFolderName);
    if (!ret){
        qCritical() << "renameFolder ret:" << ret;
    }
    return ret;
}
