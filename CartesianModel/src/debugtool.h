/*
 * This class is used for just debugging, feed test data to the system
 */

#ifndef DEBUGTOOL_H
#define DEBUGTOOL_H

#include <QObject>
#include "resourcemanager/memorymodel.h"

class DebugTool : public QObject
{
    Q_OBJECT
public:
    explicit DebugTool(QObject *parent = nullptr);
    Q_INVOKABLE bool feedTestData(QString url);
signals:

public slots:

private:
};

#endif // DEBUGTOOL_H
