#include "fileitemmodel.h"
#include <QDebug>

Q_LOGGING_CATEGORY(filemgr, "filemgr")

FileItemModel::FileItemModel(QObject *parent) : QObject(parent)
{
    m_row = 0;
    setNumOfRows(10);
    m_cur.setFilter(QDir::AllDirs | QDir::NoDot | QDir::Files);
    m_cur.setSorting(QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);
    setHeader("Files");
    setValid(false);
    setSuppressDoubleDot(false);
}

void FileItemModel::setRoothPath(QString path)
{
    m_cur.cd(path);
    setAbsolute(m_cur.absolutePath());
    m_row = 0;
    setValid(true);
    qCDebug(filemgr) << "SetRootPath " << symbol() << path;
    setList(makeList());
}

QList<QObject *> FileItemModel::makeList()
{
    QList<QObject *> list;

    // Create all of rows as dummy
    for (int i = 0; i < numOfRows(); ++i) {
        FileItemObject *obj = new FileItemObject();
        list.append(obj);
    }

    reinterpret_cast<FileItemObject *>(list.at(0))->setName(header());

    if (valid()) {
        int count = m_cur.entryInfoList().count();
        int limit = ((m_row + numOfRows()) > (count - 1)) ? (count - 1) : (m_row + numOfRows()) - 1;
        int j = 1;
        for (int i = m_row; i < limit; ++i, ++j) {
            QFileInfo info = m_cur.entryInfoList().at(i);
            FileItemObject *obj = reinterpret_cast<FileItemObject *>(list.at(j));
            obj->setName(info.fileName());
            obj->setAbsolute(info.absolutePath());
            obj->setHasChild(info.isDir());
            obj->setValid(true);
            obj->setIndex(i);
        }
        for (; j < numOfRows(); ++j) {
            FileItemObject *obj = reinterpret_cast<FileItemObject *>(list.at(j));
            obj->setHasChild(false);
            obj->setValid(false);
            obj->setIndex(-1);
        }
    }
    return list;
}
