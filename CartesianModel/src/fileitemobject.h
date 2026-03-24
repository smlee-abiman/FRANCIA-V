/*
 * Each data (file/directory) model for file browser
 */

#ifndef FILEITEMOBJECT_H
#define FILEITEMOBJECT_H

#include <QObject>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(filemgr)

class FileItemObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool hasChild READ hasChild WRITE setHasChild NOTIFY hasChildChanged)
    Q_PROPERTY(QString absolute READ absolute WRITE setAbsolute NOTIFY absoluteChanged)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)

public:
    explicit FileItemObject(QObject *parent = nullptr);
    FileItemObject(QString name, bool hasChild, QString absolute);

    QString name() const { return m_name; }

    bool hasChild() const { return m_hasChild; }

    QString absolute() const { return m_absolute; }

    bool valid() const { return m_valid; }

    int index() const { return m_index; }

signals:

    void nameChanged(QString name);

    void hasChildChanged(bool hasChild);

    void absoluteChanged(QString absolute);

    void validChanged(bool valid);

    void indexChanged(int index);

public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }
    void setHasChild(bool hasChild)
    {
        if (m_hasChild == hasChild)
            return;

        m_hasChild = hasChild;
        emit hasChildChanged(m_hasChild);
    }
    void setAbsolute(QString absolute)
    {
        if (m_absolute == absolute)
            return;

        m_absolute = absolute;
        emit absoluteChanged(m_absolute);
    }

    void setValid(bool valid)
    {
        if (m_valid == valid)
            return;

        m_valid = valid;
        emit validChanged(m_valid);
    }

    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

private:
    QString m_name;
    bool m_hasChild = false;
    QString m_absolute;
    bool m_valid = false;
    int m_index = -1;
};

#endif // FILEITEMOBJECT_H
