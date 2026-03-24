#ifndef FOLDERLISTMODEL_H
#define FOLDERLISTMODEL_H

#include <QAbstractListModel>

class DirInfo
{
public:
    DirInfo(const QString &dirName, const QString &dirPath, const int depth, const bool selected);

    QString dirName() const;
    QString dirPath() const;
    int depth() const;
    bool selected() const;

    void setSelected(bool selected);

private:
    QString m_dirName;
    QString m_dirPath;
    int m_depth;
    bool m_selected;
};

class FolderListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        DirNameRole = Qt::UserRole+1,
        DirPathRole,
        DepthRole,
        SelectedRole,
    };

    FolderListModel(QObject *parent = nullptr);
    virtual ~FolderListModel();

    // QAbstractListModel.
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    // method.
    void updateDirList(QString dirPath, int depth);
    void updateSelected(int index);
    void addDirInfo(const DirInfo &dirInfo);
    void addDirInfo(const QString &dirName, const QString &dirPath, const int depth, const bool selected);
    void removeDirInfo(const DirInfo &dirInfo);
    void sortDirList();
    DirInfo *getSelectedDirInfo();
    void clearSelectedDirInfo();
    void clearDirInfoList();

protected:
    // QAbstractListModel.
    QHash<int, QByteArray> roleNames() const;

private:
    QList<DirInfo*> m_dirInfos;
    DirInfo *m_selectedDirInfo = nullptr;

private:
    bool isContainsDirInfo(DirInfo &dirInfo);
};

#endif // FOLDERLISTMODEL_H
