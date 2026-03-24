#include "ncplabelstorage.h"

#include <QDebug>
#include <QQmlEngine>

//------------------------------------------------------
// NCPLabelStorage.
//------------------------------------------------------
NCPLabelStorage::NCPLabelStorage(QObject *parent)
    : QObject(parent)
{
    for (int i = 1; i < 1000; i++) {
        auto ncpLabel = new NCPLabel(i);
        QQmlEngine::setObjectOwnership(ncpLabel, QQmlEngine::CppOwnership);
        mLabels.insert(i, ncpLabel);
        mFreeLabels.insert(i, ncpLabel);
    }
}
//------------------------------------------------------
NCPLabelStorage::~NCPLabelStorage()
{
    qDeleteAll(mLabels);
    mLabels.clear();
    mFreeLabels.clear();
    mAssignedLabels.clear();
}
//------------------------------------------------------
NCPLabel* NCPLabelStorage::labelAt(int labelNo)
{
    if (!mLabels.contains(labelNo)) {
        return nullptr;
    }
    auto item = mLabels.value(labelNo);
    return dynamic_cast<NCPLabel*>(item);
}
//------------------------------------------------------
NCPLabel* NCPLabelStorage::findFreeLabel()
{
    if (mFreeLabels.isEmpty()) {
        return nullptr;
    }
    auto item = mFreeLabels.first();
    auto label = dynamic_cast<NCPLabel*>(item);
    return label;
}
//------------------------------------------------------
NCPLabel* NCPLabelStorage::findFreeLabel(QList<QObject*>& excludeLabels)
{
    if (mFreeLabels.isEmpty()) {
        return nullptr;
    }
    for (auto& item : mFreeLabels) {
        auto label = dynamic_cast<NCPLabel*>(item);
        auto isExcludeLabel = false;
        // exclude label?
        for (auto& item2 : excludeLabels) {
            auto label2 = dynamic_cast<NCPLabel*>(item2);
            if (label->no() == label2->no()) {
                isExcludeLabel = true;
                break;
            }
        }
        if (isExcludeLabel) {
            continue;
        }
        return label;
    }
    return nullptr;
}
//------------------------------------------------------
NCPLabel *NCPLabelStorage::assignLabel(int labelNo, int programNo)
{
    if (!mFreeLabels.contains(labelNo)) {
        return nullptr;
    }
    auto item = mFreeLabels.take(labelNo);
    auto label = dynamic_cast<NCPLabel*>(item);
    label->setProgramNo(programNo);
    mAssignedLabels.insert(labelNo, item);
    emit assignedLabelsChanged(assignedLabels());
    return label;
}
//------------------------------------------------------
void NCPLabelStorage::releaseLable(NCPLabel *label)
{
    if (!label) {
        return;
    }
    label->setProgramNo(0);
    mAssignedLabels.remove(label->no());
    emit assignedLabelsChanged(assignedLabels());
    mFreeLabels.insert(label->no(), label);
}
//------------------------------------------------------
void NCPLabelStorage::display()
{
    qDebug() << "free labels:";
    QMapIterator<int, QObject*> i(mFreeLabels);
    while (i.hasNext()) {
        auto label = dynamic_cast<NCPLabel*>(i.next().value());
        qDebug() << " - no:" << label->no() << "inUse:" << label->inUse();
    }
    qDebug() << "assinged labels:";
    QMapIterator<int, QObject*> ai(mAssignedLabels);
    while (ai.hasNext()) {
        auto label = dynamic_cast<NCPLabel*>(ai.next().value());
        qDebug() << " - no:" << label->no() << "inUse:" << label->inUse();
    }
    qDebug() << QString("free:%1, assign:%2, all:%3")
                    .arg(mFreeLabels.size())
                    .arg(mAssignedLabels.size())
                    .arg(mLabels.size());
}
//------------------------------------------------------
bool NCPLabelStorage::isEmptyFreeLabels()
{
    return mFreeLabels.isEmpty();
}
//------------------------------------------------------
bool NCPLabelStorage::isExistFreeLabels(int labelNo)
{
    return mFreeLabels.contains(labelNo);
}
