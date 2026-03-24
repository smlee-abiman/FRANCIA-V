#include "fileitemobject.h"

FileItemObject::FileItemObject(QObject *parent) : QObject(parent)
{
    setValid(false);
}

FileItemObject::FileItemObject(QString name, bool hasChild, QString absolute)
{
    setName(name);
    setHasChild(hasChild);
    setAbsolute(absolute);
    setValid(true);
}
