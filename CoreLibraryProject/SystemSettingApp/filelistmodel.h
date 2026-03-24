#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QAbstractListModel>
#include <QDateTime>

struct stControllerInfo {
    QString filePath;
    QDateTime lastModified;
    qint64 size;
};

class FileInfo
{
public:
    FileInfo();

    FileInfo(const QString name, const bool isDirectory, const bool selected, const bool hasDiff, const bool nowCopying, const QString parent,
             const QString usbPath, const QDateTime usbLastModified, const qint64 usbSize,
             const QString controllerPath, const QDateTime controllerLastModified, const qint64 controllerSize);

    QString name() const;
    void setName(QString name);
    bool isDirectory() const;
    void setIsDirectory(bool isDirectory);
    bool selected() const;
    void setSelected(bool selected);
    bool hasDiff() const;
    void setHasDiff(bool hasDiff);
    bool nowCopying() const;
    void setNowCopying(bool nowCopying);
    QString parent() const;
    void setParent(QString parent);
    QString usbPath() const;
    void setUsbPath(QString usbPath);
    QDateTime usbLastModified() const;
    void setUsbLastModified(QDateTime usbLastModified);
    qint64 usbSize() const;
    void setUsbSize(qint64 usbSize);
    QString controllerPath() const;
    void setControllerPath(QString path);
    QDateTime controllerLastModified() const;
    void setControllerLastModified(QDateTime lastModified);
    qint64 controllerSize() const;
    void setControllerSize(qint64 size);

private:
    QString m_name = "";
    bool m_isDirectory = false;
    bool m_selected = false;
    bool m_hasDiff = false;
    bool m_nowCopying = false;
    QString m_parent = "";
    QString m_usbPath = "";
    QDateTime m_usbLastModified = QDateTime();
    qint64 m_usbSize = 0;
    QString m_controllerPath = "";
    QDateTime m_controllerLastModified = QDateTime();
    qint64 m_controllerSize = 0;
};

class FileListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole+1,
        IsDirectoryRole,
        SelectedRole,
        HasDiffRole,
        NowCopyingRole,
        ParentRole,
        UsbPathRole,
        UsbLastModifiedRoles,
        UsbSizeRole,
        ControllerPathRole,
        ControllerLastModifiedRoles,
        ControllerSizeRole
    };

    FileListModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void clearFileInfoList();
    void addUsbFileInfo(QString name, bool isDir, QString parent, bool selected, QString path, QDateTime lastModified, qint64 size, bool hasDiff);
    void addControllerFileInfo(QString name, bool isDir, QString parent, bool selected, QString path, QDateTime lastModified, qint64 size, bool hasDiff);
    void updateSelected(int index);
    void updateSelected(const QModelIndex &index);
    void resetSelectState();
    QList<int> getSelectedIndex();
    void updateControllerInfo(int index, stControllerInfo controllerInfo);
    void updateUsbLastModified(int index, QDateTime lastModified);
    FileInfo* getFileInfo(int index);
    FileInfo* getFileInfo(QString fileName);
    FileInfo* getFileInfo(const QModelIndex &index);
    bool removeChildFolder(QString parent);
    void beginReset();
    void endReset();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
     void updateHasDiff();

private:
    QList<FileInfo> m_fileInfos;
};

#endif // FILELISTMODEL_H
