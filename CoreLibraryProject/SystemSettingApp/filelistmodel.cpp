#include "filelistmodel.h"
#include <QDir>
#include <QDirIterator>

using namespace std;

bool nameLessThan(const FileInfo &d1, const FileInfo &d2)
{
    QString d1Name;
    QString d2Name;

    if(d1.parent().length() != 0) {
        d1Name = d1.parent() + " " + d1.name();  // space is smaller than [0-9|a-z|A-Z|etc]
    } else {
        d1Name = d1.name();
    }

    if(d2.parent().length() != 0) {
        d2Name = d2.parent() + " " + d2.name();  // space is smaller than [0-9|a-z|A-Z|etc]
    } else {
        d2Name = d2.name();
    }

    return d1Name < d2Name;
}

FileInfo::FileInfo()
{

}

FileInfo::FileInfo(const QString name, const bool isDirectory, const bool selected, const bool hasDiff, const bool nowCopying, const QString parent,
                   const QString usbPath, const QDateTime usbLastModified, const qint64 usbSize,
                   const QString controllerPath, const QDateTime controllerLastModified, const qint64 controllerSize)
    : m_name(name), m_isDirectory(isDirectory), m_selected(selected), m_hasDiff(hasDiff), m_nowCopying(nowCopying), m_parent(parent),
      m_usbPath(usbPath), m_usbLastModified(usbLastModified), m_usbSize(usbSize),
      m_controllerPath(controllerPath), m_controllerLastModified(controllerLastModified), m_controllerSize(controllerSize)
{

}

QString FileInfo::name() const
{
    return m_name;
}

bool FileInfo::isDirectory() const
{
    return m_isDirectory;
}

bool FileInfo::selected() const
{
    return m_selected;
}

bool FileInfo::hasDiff() const
{
    return m_hasDiff;
}

bool FileInfo::nowCopying() const
{
    return m_nowCopying;
}

QString FileInfo::parent() const
{
    return m_parent;
}

QString FileInfo::usbPath() const
{
    return m_usbPath;
}

void FileInfo::setUsbPath(QString usbPath)
{
    m_usbPath = usbPath;
}

QDateTime FileInfo::usbLastModified() const
{
    return m_usbLastModified;
}

qint64 FileInfo::usbSize() const
{
    return m_usbSize;
}

void FileInfo::setUsbSize(qint64 usbSize)
{
    m_usbSize = usbSize;
}

QString FileInfo::controllerPath() const
{
    return m_controllerPath;
}

QDateTime FileInfo::controllerLastModified() const
{
    return m_controllerLastModified;
}

qint64 FileInfo::controllerSize() const
{
    return m_controllerSize;
}

void FileInfo::setName(QString name)
{
    m_name = name;
}

void FileInfo::setIsDirectory(bool isDirectory)
{
    m_isDirectory = isDirectory;
}

void FileInfo::setSelected(bool selected)
{
    m_selected = selected;
}

void FileInfo::setHasDiff(bool hasDiff)
{
    m_hasDiff = hasDiff;
}

void FileInfo::setNowCopying(bool nowCopying)
{
    m_nowCopying = nowCopying;
}

void FileInfo::setParent(QString parent)
{
    m_parent = parent;
}

void FileInfo::setControllerLastModified(QDateTime lastModified)
{
    m_controllerLastModified.swap(lastModified);
}

void FileInfo::setControllerSize(qint64 size)
{
    m_controllerSize = size;
}

void FileInfo::setControllerPath(QString path)
{
    m_controllerPath = path;
}

void FileInfo::setUsbLastModified(QDateTime usbLastModified)
{
    m_usbLastModified.swap(usbLastModified);
}

FileListModel::FileListModel(QObject *parent) : QAbstractListModel(parent)
{

}

QHash<int, QByteArray> FileListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IsDirectoryRole] = "isDirectory";
    roles[SelectedRole] = "selected";
    roles[HasDiffRole] = "hasDiff";
    roles[NowCopyingRole] = "nowCopying";
    roles[ParentRole] = "parent";
    roles[UsbPathRole] = "usbPath";
    roles[UsbLastModifiedRoles] = "usbLastModified";
    roles[UsbSizeRole] = "usbSize";
    roles[ControllerPathRole] = "controllerPath";
    roles[ControllerLastModifiedRoles] = "controllerLastModified";
    roles[ControllerSizeRole] = "controllerSize";
    return roles;
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if((index.row() < 0) || (index.row() >= m_fileInfos.count())) {
        return QVariant();
    }

    const FileInfo &fileInfo = m_fileInfos[index.row()];

    if(role == NameRole) {
        return fileInfo.name();
    } else if(role == IsDirectoryRole) {
        return fileInfo.isDirectory();
    } else if(role == SelectedRole) {
        return fileInfo.selected();
    } else if(role == HasDiffRole) {
        return fileInfo.hasDiff();
    } else if(role == NowCopyingRole) {
        return fileInfo.nowCopying();
    } else if(role == ParentRole) {
        return fileInfo.parent();
    } else if(role == UsbPathRole) {
        return fileInfo.usbPath();
    } else if(role == UsbLastModifiedRoles) {
        return fileInfo.usbLastModified();
    } else if(role == UsbSizeRole) {
        return fileInfo.usbSize();
    } else if(role == ControllerPathRole) {
        return fileInfo.controllerPath();
    } else if(role == ControllerLastModifiedRoles) {
        return fileInfo.controllerLastModified();
    } else if(role == ControllerSizeRole) {
        return fileInfo.controllerSize();
    }

    return QVariant();
}

int FileListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_fileInfos.count();
}

void FileListModel::clearFileInfoList()
{
    m_fileInfos.clear();
}

void FileListModel::addUsbFileInfo(QString name, bool isDir, QString parent, bool selected, QString path, QDateTime lastModified, qint64 size, bool hasDiff)
{
    for(int i = 0; i < m_fileInfos.count(); i++) {
        FileInfo &fileInfo = m_fileInfos[i];
        if((fileInfo.name().compare(name) != 0) || (fileInfo.parent().compare(parent) != 0)) {
            continue;
        }
        // already exists in controller
        fileInfo.setUsbPath(path);
        fileInfo.setUsbSize(size);
        fileInfo.setUsbLastModified(lastModified);
        return;
    }
    FileInfo fileInfo;
    fileInfo.setName(name);
    fileInfo.setParent(parent);
    fileInfo.setIsDirectory(isDir);
    fileInfo.setSelected(selected);
    fileInfo.setUsbPath(path);
    fileInfo.setUsbSize(size);
    fileInfo.setUsbLastModified(lastModified);
    fileInfo.setHasDiff(hasDiff);
    m_fileInfos << fileInfo;

    std::sort(m_fileInfos.begin(), m_fileInfos.end(), nameLessThan);
}

void FileListModel::addControllerFileInfo(QString name, bool isDir, QString parent, bool selected, QString path, QDateTime lastModified, qint64 size, bool hasDiff)
{
    for(int i = 0; i < m_fileInfos.count(); i++) {
        FileInfo &fileInfo = m_fileInfos[i];
        if((fileInfo.name().compare(name) != 0) || (fileInfo.parent().compare(parent) != 0)) {
            continue;
        }
        // already exists in usb
        fileInfo.setControllerPath(path);
        fileInfo.setControllerSize(size);
        fileInfo.setControllerLastModified(lastModified);
        return;
    }
    FileInfo fileInfo;
    fileInfo.setName(name);
    fileInfo.setParent(parent);
    fileInfo.setHasDiff(hasDiff);
    fileInfo.setControllerPath(path);
    fileInfo.setControllerSize(size);
    fileInfo.setControllerLastModified(lastModified);
    fileInfo.setIsDirectory(isDir);
    fileInfo.setSelected(selected);
    m_fileInfos << fileInfo;

    std::sort(m_fileInfos.begin(), m_fileInfos.end(), nameLessThan);
}

void FileListModel::beginReset()
{
    beginResetModel();
}

void FileListModel::endReset()
{
    endResetModel();
}

void FileListModel::updateSelected(int index)
{
    FileInfo &fileInfo = m_fileInfos[index];
    if(fileInfo.selected()) {
        fileInfo.setSelected(false);
    } else {
        fileInfo.setSelected(true);
    }
}

void FileListModel::updateSelected(const QModelIndex &index)
{
    if((index.row() < 0) || (index.row() >= m_fileInfos.count())) {
        return;
    }
    updateSelected(index.row());
}

void FileListModel::resetSelectState()
{
    for(int i = 0; i < m_fileInfos.count(); i++) {
        FileInfo &fileInfo = m_fileInfos[i];
        fileInfo.setSelected(false);
    }
}

QList<int> FileListModel::getSelectedIndex()
{
    QList<int> selectedIndexList;

    for(int i = 0; i < m_fileInfos.count(); i++) {
        FileInfo fileInfo = m_fileInfos.at(i);
        if(fileInfo.selected()) {
            selectedIndexList << i;
        }
    }

    return selectedIndexList;
}

void FileListModel::updateControllerInfo(int index, stControllerInfo controllerInfo)
{
    FileInfo &fileInfo = m_fileInfos[index];
    fileInfo.setControllerPath(controllerInfo.filePath);
    fileInfo.setControllerLastModified(controllerInfo.lastModified);
    fileInfo.setControllerSize(controllerInfo.size);
}

void FileListModel::updateUsbLastModified(int index, QDateTime lastModified)
{
    FileInfo &fileInfo = m_fileInfos[index];
    fileInfo.setUsbLastModified(lastModified);
}

FileInfo* FileListModel::getFileInfo(int index)
{
    return &m_fileInfos[index];
}

FileInfo* FileListModel::getFileInfo(QString fileName)
{
    if((fileName == nullptr) || (fileName.length() == 0)) {
        return nullptr;
    }

    for(int i = 0; i < m_fileInfos.count(); i++) {
        FileInfo &fileInfo = m_fileInfos[i];
        if(fileName.compare(fileInfo.name()) == 0) {
            return &m_fileInfos[i];
        }
    }

    return nullptr;  // not match
}

FileInfo* FileListModel::getFileInfo(const QModelIndex &index)
{
    if((index.row() < 0) || (index.row() >= m_fileInfos.count())) {
        return nullptr;
    }
    return getFileInfo(index.row());
}

bool FileListModel::removeChildFolder(QString parent)
{
    bool isRemoved = false;

    if((parent == nullptr) || (parent.length() == 0)) {
        return isRemoved;
    }

    for(int i = 0; i < m_fileInfos.count(); i++) {
        FileInfo &fileInfo = m_fileInfos[i];
        if(parent.compare(fileInfo.parent()) == 0) {
            m_fileInfos.removeAt(i);
            i--;
            isRemoved = true;
        }
    }

    return isRemoved;
}
