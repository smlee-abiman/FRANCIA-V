#ifndef NCPLABELSTORAGE_H
#define NCPLABELSTORAGE_H

#include "ncplabel.h"

#include <QMap>
#include <QObject>

//------------------------------------------------------
// NCPLabelStorage.
//------------------------------------------------------
class NCPLabelStorage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> assignedLabels READ assignedLabels NOTIFY assignedLabelsChanged)
public:
    explicit NCPLabelStorage(QObject *parent = nullptr);
    virtual ~NCPLabelStorage();

    // getter.
    QList<QObject*> assignedLabels() const { return mAssignedLabels.values(); }

    // method.
    NCPLabel* labelAt(int labelNo);
    NCPLabel* findFreeLabel();
    NCPLabel* findFreeLabel(QList<QObject*>& excludeLabels);
    NCPLabel* assignLabel(int labelNo, int programNo);
    void releaseLable(NCPLabel* label);
    bool isEmptyFreeLabels();
    bool isExistFreeLabels(int labelNo);

    // for debug.
    void display();

signals:
    void assignedLabelsChanged(QList<QObject*> assignedLabels);

private:
    QMap<int, QObject*> mLabels;         // QObject* -> NCPLabel*
    QMap<int, QObject*> mFreeLabels;     // QObject* -> NCPLabel*
    QMap<int, QObject*> mAssignedLabels; // QObject* -> NCPLabel*
};

#endif // NCPLABELSTORAGE_H
