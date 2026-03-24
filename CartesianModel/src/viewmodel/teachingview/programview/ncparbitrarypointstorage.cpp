#include "ncparbitrarypointstorage.h"
#include <QDebug>

const int NCP_ARBITRARY_POINT1_START = 101;
const int NCP_ARBITRARY_POINT1_END = 200;
const int NCP_ARBITRARY_POINT2_START = 256;
const int NCP_ARBITRARY_POINT2_END = 999;

NCPArbitraryPointStorage::NCPArbitraryPointStorage()
{
    for (int i = NCP_ARBITRARY_POINT1_START; i <= NCP_ARBITRARY_POINT1_END; i++) {
        auto point = new NCPArbitraryPoint(i);
        mPoints.insert(i, point);
        mFreePoints.insert(i, point);
    }

    for (int i = NCP_ARBITRARY_POINT2_START; i <= NCP_ARBITRARY_POINT2_END; i++) {
        auto point = new NCPArbitraryPoint(i);
        mPoints.insert(i, point);
        mFreePoints.insert(i, point);
    }
}

NCPArbitraryPointStorage::~NCPArbitraryPointStorage()
{
    qDeleteAll(mPoints);
    mPoints.clear();
    mFreePoints.clear();
    mAssignedPoints.clear();
}

NCPArbitraryPoint *NCPArbitraryPointStorage::pointAt(int pointNo)
{
    if (!mPoints.contains(pointNo)) {
        return nullptr;
    }
    return mPoints.value(pointNo);
}

NCPArbitraryPoint* NCPArbitraryPointStorage::findFreePoint(int index)
{
    if (mFreePoints.isEmpty()) {
        return nullptr;
    }

    auto keys = mFreePoints.keys();
    if ((index < 0) || (index >= keys.length())) {
        return nullptr;
    }

    auto key = keys.at(index);
    return mFreePoints.value(key);
}

NCPArbitraryPoint *NCPArbitraryPointStorage::assignPoint(int pointNo)
{
    if (!mFreePoints.contains(pointNo)) {
        return nullptr;
    }
    auto item = mFreePoints.take(pointNo);
    item->setInUse(true);
    mAssignedPoints.insert(pointNo, item);
    return item;
}

void NCPArbitraryPointStorage::releasePoint(int pointNo)
{
    if (!mAssignedPoints.contains(pointNo)) {
        return;
    }
    auto item = mAssignedPoints.take(pointNo);
    item->setInUse(false);
    mFreePoints.insert(pointNo, item);
}

void NCPArbitraryPointStorage::display()
{
    qDebug() << "free points:";
    QMapIterator<int, NCPArbitraryPoint *> i(mFreePoints);
    while (i.hasNext()) {
        auto point = i.next().value();
        qDebug() << " - no:" << point->no() << "inUse:" << point->inUse();
    }
    qDebug() << "assinged labels:";
    QMapIterator<int, NCPArbitraryPoint *> ai(mAssignedPoints);
    while (ai.hasNext()) {
        auto point = ai.next().value();
        qDebug() << " - no:" << point->no() << "inUse:" << point->inUse();
    }
}
