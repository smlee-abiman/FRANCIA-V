#include "folderlistmodel.h"
#include <QDir>
#include <QDirIterator>
#include <algorithm>

using namespace std;

bool dirPathLessThan(const DirInfo *d1, const DirInfo *d2)
{
    return d1->dirPath() < d2->dirPath();
}

DirInfo::DirInfo(const QString &dirName, const QString &dirPath, const int depth, const bool selected)
    : m_dirName(dirName), m_dirPath(dirPath), m_depth(depth), m_selected(selected)
{

}

QString DirInfo::dirName() const
{
    return m_dirName;
}

QString DirInfo::dirPath() const
{
    return m_dirPath;
}

int DirInfo::depth() const
{
    return m_depth;
}

bool DirInfo::selected() const
{
    return m_selected;
}

void DirInfo::setSelected(bool selected)
{
    m_selected = selected;
}

FolderListModel::FolderListModel(QObject *parent) : QAbstractListModel(parent)
{

}

FolderListModel::~FolderListModel()
{
    clearDirInfoList();
}

QHash<int, QByteArray> FolderListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DirNameRole] = "dirName";
    roles[DirPathRole] = "dirPath";
    roles[DepthRole] = "depth";
    roles[SelectedRole] = "selected";
    return roles;
}

QVariant FolderListModel::data(const QModelIndex &index, int role) const
{
    auto indexRow = index.row();
    if((indexRow < 0) || (indexRow >= m_dirInfos.count())) {
        return QVariant();
    }

    auto dirInfo = m_dirInfos[indexRow];

    if(role == DirNameRole) {
        return dirInfo->dirName();
    } else if(role == DirPathRole) {
        return dirInfo->dirPath();
    } else if(role == DepthRole) {
        return dirInfo->depth();
    } else if(role == SelectedRole) {
        return dirInfo->selected();
    }

    return QVariant();
}

int FolderListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dirInfos.count();
}

void FolderListModel::updateDirList(QString dirPath, int depth)
{
    QDir::Filters filters = QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot;
    QStringList nameFilters;
    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
    QDirIterator it(dirPath, nameFilters, filters, flags);
    while(it.hasNext()) {
        QString dir = it.next();
        DirInfo dirInfo = DirInfo(QDir(dir).dirName(), dir, depth, false);
        if(isContainsDirInfo(dirInfo)) {
            removeDirInfo(dirInfo);
        } else {
            addDirInfo(dirInfo);
        }
    }

    sortDirList();
}

void FolderListModel::updateSelected(int index)
{
    beginResetModel();
    if(m_selectedDirInfo != nullptr) {
        m_selectedDirInfo->setSelected(false);
    }
    auto dirInfo = m_dirInfos[index];
    dirInfo->setSelected(true);
    m_selectedDirInfo = dirInfo;
    endResetModel();
}

void FolderListModel::addDirInfo(const DirInfo &dirInfo)
{
    addDirInfo(dirInfo.dirName(), dirInfo.dirPath(), dirInfo.depth(), dirInfo.selected());
}

void FolderListModel::addDirInfo(const QString &dirName, const QString &dirPath, const int depth, const bool selected)
{
    auto newDirInfo = new DirInfo(dirName, dirPath, depth, selected);
    m_dirInfos << newDirInfo;
}

void FolderListModel::sortDirList()
{
    beginResetModel();
    std::sort(m_dirInfos.begin(), m_dirInfos.end(), dirPathLessThan);
    endResetModel();
}

void FolderListModel::removeDirInfo(const DirInfo &dirInfo)
{
    for(int i = 0; i < m_dirInfos.count(); i++) {
        if(m_dirInfos[i]->dirPath().contains(dirInfo.dirPath())) {
            m_dirInfos.removeAt(i);
            i--;
        }
    }
}

bool FolderListModel::isContainsDirInfo(DirInfo &dirInfo)
{
    for(int i = 0; i < m_dirInfos.count(); i++) {
        if(m_dirInfos[i]->dirPath() == dirInfo.dirPath()) {
            return true;
        }
    }
    return false;
}

DirInfo *FolderListModel::getSelectedDirInfo()
{
    return m_selectedDirInfo;
}

void FolderListModel::clearSelectedDirInfo()
{
    m_selectedDirInfo = nullptr;
}

void FolderListModel::clearDirInfoList()
{
    beginResetModel();
    for (auto dirInfo : m_dirInfos) {
        delete dirInfo;
    }
    m_dirInfos.clear();
    endResetModel();
}
