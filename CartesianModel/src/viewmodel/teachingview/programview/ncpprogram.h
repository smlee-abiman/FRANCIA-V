#ifndef NCPPROGRAM_H
#define NCPPROGRAM_H

#include <QMap>
#include <QList>
#include <QObject>
#include <QTextStream>

#include "ncpstep.h"
#include "ncplabel.h"
#include "ncpcommand.h"

class NCPLabelStorage;
class NCPArbitraryPointStorage;

class NCPProgram : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int no READ no CONSTANT)
    Q_PROPERTY(QList<QObject*> steps READ steps NOTIFY stepsChanged)
    Q_PROPERTY(QList<QObject*> labels READ labels NOTIFY labelsChanged)
    Q_PROPERTY(QList<QObject*> unUsedLabels READ unUsedLabels NOTIFY unUsedLabelsChanged)
public:
    explicit NCPProgram(NCPLabelStorage* ncpLabelStorage, NCPArbitraryPointStorage *arbitraryPointStorage, const NCPCommand& command, QObject *parent = nullptr);
    explicit NCPProgram(NCPLabelStorage* ncpLabelStorage, NCPArbitraryPointStorage *arbitraryPointStorage, int programNo, QObject *parent = nullptr);
    virtual ~NCPProgram();

    void display(); // for debug.

    // getter/setter.
    int no() const { return mNo; }
    int numSteps() const { return mSteps.length(); }
    QList<QObject *> steps() const { return mSteps; }
    QList<QObject *> labels() const { return mLabels.values(); }
    NCPStep* step(int index) const {
        if ((index < 0) || (mSteps.size() <= index)) { return nullptr; }
        return dynamic_cast<NCPStep*>(mSteps.at(index));
    }
    QList<QObject*> unUsedLabels() const;

    // method.
    NCPProgram* clone(NCPLabelStorage* ncpLabelStorage, NCPArbitraryPointStorage *arbitraryPointStorage) const;
    QString toString() const;
    void writeStepCodes(QTextStream& out) const;

    Q_INVOKABLE void moveBackOneStep(int targetIndex);
    Q_INVOKABLE void moveForwardOneStep(int targetIndex);

    NCPStep* cloneNcpStep(int index);
    bool insertStep(int index, NCPStep* step);
    void appendStep(NCPStep* step);
    bool removeStep(int index);
    void replaceCommand(int index, NCPCommand *command);
    void appendLabel(int labelNo);
    void appendLabel(NCPLabel *label);
    void removeLabel(int labelNo);
    void assignLabel(int labelNo);
    void releaseLabel(int labelNo);
    bool assignLabelNoInNcpStep(int index, int labelNo);
    bool releaseLabelInNcpStep(int index);
    void removeLabelInNcpCommand(NCPCommand* command);
    void assignPoint(NCPCommand *command);
    void releasePoint(NCPCommand *command);

signals:
    void stepsChanged(QList<QObject*>);
    void labelsChanged(QList<QObject*>);
    void unUsedLabelsChanged(QList<QObject*>);

private:
    bool isLabelReferencedbyCommand(int labelNo);
    void releaseLabelInNcpStep(NCPStep* step);

private:
    int mNo;
    NCPLabelStorage* mNcpLabelStorage;
    NCPArbitraryPointStorage *mNcpArbitraryPointStorage;
    QList<QObject*> mSteps;         // QObject* -> NCPStep*
    QMap<int, QObject*> mLabels;    // QObject* -> NCPLabel*
};

#endif // NCPPROGRAM_H
