/*
 * Used on file browser. Resonsible to manage data model
 */

#ifndef FILEITEMMODEL_H
#define FILEITEMMODEL_H

#include <QObject>
#include <QDir>
#include "fileitemobject.h"

class FileItemModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QList<QObject *> list READ list WRITE setList NOTIFY listChanged)
    Q_PROPERTY(int numOfRows READ numOfRows WRITE setNumOfRows NOTIFY numOfRowsChanged)
    Q_PROPERTY(QString header READ header WRITE setHeader NOTIFY headerChanged)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)
    Q_PROPERTY(bool suppressDoubleDot READ suppressDoubleDot WRITE setSuppressDoubleDot NOTIFY
                       suppressDoubleDotChanged)
    Q_PROPERTY(QString absolute READ absolute WRITE setAbsolute NOTIFY absoluteChanged)
    Q_PROPERTY(int select READ select WRITE setSelect NOTIFY selectChanged)

    // Debug
    Q_PROPERTY(QString symbol READ symbol WRITE setSymbol NOTIFY symbolChanged)

public:
    explicit FileItemModel(QObject *parent = nullptr);
    void setRoothPath(QString path);
    Q_INVOKABLE void up()
    {
        m_row = (m_row - 1) < 0 ? 0 : m_row - 1;
        setList(makeList());
    }
    Q_INVOKABLE void down()
    {
        int count = static_cast<int>(m_cur.count());
        m_row = ((m_row + 1) > (count - 1)) ? (count - 1) : (m_row + 1);
        setList(makeList());
    }

    Q_INVOKABLE QString getFile(int index)
    {
        QFileInfo info = m_cur.entryInfoList().at(index);
        return info.absoluteFilePath() + "/" + info.fileName();
    }

    Q_INVOKABLE bool hasChild(int index)
    {
        QFileInfo info = m_cur.entryInfoList().at(index);
        return info.isDir();
    }

    QString path() const { return m_path; }

    QList<QObject *> list() const { return m_list; }

    int numOfRows() const { return m_numOfRows; }

    QString header() const { return m_header; }

    bool valid() const { return m_valid; }

    bool suppressDoubleDot() const { return m_suppressDoubleDot; }

    QString symbol() const { return m_symbol; }

    QString absolute() const { return m_absolute; }

    int select() const { return m_select; }

signals:

    void pathChanged(QString path);

    void listChanged(QList<QObject *> list);

    void numOfRowsChanged(int numOfRows);

    void headerChanged(QString header);

    void validChanged(bool valid);

    void suppressDoubleDotChanged(bool suppressDoubleDot);

    void symbolChanged(QString symbol);

    void absoluteChanged(QString absolute);

    void selectChanged(int select);

public slots:

    void setPath(QString path)
    {
        if (m_path == path)
            return;

        m_path = path;
        setRoothPath(m_path);
        emit pathChanged(m_path);
    }

    void setList(QList<QObject *> list)
    {
        if (m_list == list)
            return;

        for (int i = 0; i < m_list.count(); ++i) {
            delete m_list.last();
            m_list.removeLast();
        }
        m_list = list;
        emit listChanged(m_list);
    }

    void setNumOfRows(int numOfRows)
    {
        if (m_numOfRows == numOfRows)
            return;

        if (numOfRows < 2) {
            return;
        }

        m_numOfRows = numOfRows;
        setList(makeList());
        emit numOfRowsChanged(m_numOfRows);
    }

    void setHeader(QString header)
    {
        if (m_header == header)
            return;

        m_header = header;
        emit headerChanged(m_header);
    }

    void setValid(bool valid)
    {
        if (m_valid == valid)
            return;

        m_valid = valid;

        setList(makeList());
        emit validChanged(m_valid);
    }

    void setSuppressDoubleDot(bool suppressDoubleDot)
    {
        if (m_suppressDoubleDot == suppressDoubleDot)
            return;

        m_suppressDoubleDot = suppressDoubleDot;
        if (m_suppressDoubleDot) {
            m_cur.setFilter(QDir::AllDirs | QDir::NoDot | QDir::Files | QDir::NoDotDot);
        } else {
            m_cur.setFilter(QDir::AllDirs | QDir::NoDot | QDir::Files);
        }

        emit suppressDoubleDotChanged(m_suppressDoubleDot);
    }

    void setSymbol(QString symbol)
    {
        if (m_symbol == symbol)
            return;

        m_symbol = symbol;
        emit symbolChanged(m_symbol);
    }

    void setAbsolute(QString absolute)
    {
        if (m_absolute == absolute)
            return;

        m_absolute = absolute;
        emit absoluteChanged(m_absolute);
    }

    void setSelect(int select)
    {
        if (m_select == select)
            return;

        m_select = select;
        setList(makeList());
        emit selectChanged(m_select);
    }

private:
    int m_row = 0;
    QDir m_cur;

    QString m_path;
    QList<QObject *> m_list;

    QList<QObject *> makeList();

    int m_numOfRows;
    QString m_header;
    bool m_valid;
    bool m_suppressDoubleDot;
    QString m_symbol;
    QString m_absolute;
    int m_select = -1;
};

#endif // FILEITEMMODEL_H
