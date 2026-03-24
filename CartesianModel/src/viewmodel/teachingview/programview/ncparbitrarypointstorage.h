#ifndef NCPARBITRARYPOINTSTORAGE_H
#define NCPARBITRARYPOINTSTORAGE_H

#include <QMap>
#include <QObject>
#include "ncparbitrarypoint.h"

class NCPArbitraryPointStorage
{
public:
    NCPArbitraryPointStorage();
    virtual ~NCPArbitraryPointStorage();

    // method.
    NCPArbitraryPoint* pointAt(int pointNo);
    NCPArbitraryPoint* findFreePoint(int index = 0);
    NCPArbitraryPoint* assignPoint(int pointNo);
    void releasePoint(int pointNo);

    // for debug.
    void display();

private:
    QMap<int, NCPArbitraryPoint*> mPoints;              // QObject* -> NCPArbitraryPoint*
    QMap<int, NCPArbitraryPoint*> mFreePoints;          // QObject* -> NCPArbitraryPoint*
    QMap<int, NCPArbitraryPoint*> mAssignedPoints;      // QObject* -> NCPArbitraryPoint*
};

#endif // NCPARBITRARYPOINTSTORAGE_H
