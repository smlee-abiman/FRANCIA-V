#include "ncplabelstorage.h"
#include "ncparbitrarypointstorage.h"
#include "ncpprogram.h"
#include "command/ncpcommandprog.h"
#include "command/ncpcommandmove.h"
#include "command/ncpcommandline.h"
#include "command/ncpcommandarc.h"

//------------------------------------------------------
// NCPProgram.
//------------------------------------------------------
NCPProgram::NCPProgram(NCPLabelStorage* ncpLabelStorage, NCPArbitraryPointStorage *arbitraryPointStorage, const NCPCommand& command, QObject *parent)
    : QObject(parent)
    , mNo(0)
    , mNcpLabelStorage(ncpLabelStorage)
    , mNcpArbitraryPointStorage(arbitraryPointStorage)
{
    auto* progCommand = dynamic_cast<const NCPCommandProg*>(&command);
    if (progCommand) {
        mNo = progCommand->no();
    }
}
//------------------------------------------------------
NCPProgram::NCPProgram(NCPLabelStorage* ncpLabelStorage, NCPArbitraryPointStorage *arbitraryPointStorage, int programNo, QObject *parent)
    : QObject(parent)
    , mNo(programNo)
    , mNcpLabelStorage(ncpLabelStorage)
    , mNcpArbitraryPointStorage(arbitraryPointStorage)
{
}
//------------------------------------------------------
NCPProgram::~NCPProgram()
{
    qDeleteAll(mSteps);
    mSteps.clear();
    for (auto& item : mLabels) {
        auto label = dynamic_cast<NCPLabel*>(item);
        mNcpLabelStorage->releaseLable(label);
    }
    mLabels.clear();
    mNcpLabelStorage = nullptr;
    mNcpArbitraryPointStorage = nullptr;
}
//------------------------------------------------------
void NCPProgram::display()
{
    if (!mLabels.isEmpty()) {
        qDebug() << "no:" << mNo;
        qDebug() << "labels:";
        for (auto& item : mLabels) {
            auto label = dynamic_cast<NCPLabel*>(item);
            qDebug() << " - no:" << label->no();
        }
    }
}
//------------------------------------------------------
QList<QObject*> NCPProgram::unUsedLabels() const
{
    QList<QObject*> labels;
    for (auto& item : mLabels ) {
        auto label = dynamic_cast<NCPLabel*>(item);
        if (label->inUse()) { continue; }
        labels.append(label);
    }
    return labels;
}
//------------------------------------------------------
NCPProgram* NCPProgram::clone(NCPLabelStorage* ncpLabelStorage, NCPArbitraryPointStorage *arbitraryPointStorage) const
{
    auto selfClone = new NCPProgram(ncpLabelStorage, arbitraryPointStorage, mNo);
    for (auto& item : mSteps) {
        auto step = dynamic_cast<NCPStep*>(item);
        auto stepClone = step->clone(true);
        selfClone->appendStep(stepClone);
    }
    return selfClone;
}
//------------------------------------------------------
QString NCPProgram::toString() const
{
    // todo:
    QString programString;
    programString.sprintf("%03d %s", mNo, qPrintable("todo:Not implemented"));
    return programString;
}
//------------------------------------------------------
void NCPProgram::writeStepCodes(QTextStream& out) const
{
    out << QString("\tPROG\t#%1").arg(mNo, 3, 10, QChar('0')) << QChar(0x0d) << QChar(0x0a) << flush;
    for (auto& item : mSteps) {
        auto step = dynamic_cast<NCPStep*>(item);
        out << step->toCode() << QChar(0x0d) << QChar(0x0a) << flush;
    }
}
//------------------------------------------------------
void NCPProgram::moveBackOneStep(int targetIndex)
{
    if (0 < targetIndex) {
        mSteps.move(targetIndex, targetIndex - 1);
        emit stepsChanged(mSteps);
    }
}
//------------------------------------------------------
void NCPProgram::moveForwardOneStep(int targetIndex)
{
    if (targetIndex < mSteps.size() - 1) {
        mSteps.move(targetIndex, targetIndex + 1);
        emit stepsChanged(mSteps);
    }
}
//------------------------------------------------------
NCPStep* NCPProgram::cloneNcpStep(int index)
{
    if ((index < 0) || (mSteps.size() <= index))
        return nullptr;
    auto step = dynamic_cast<NCPStep*>(mSteps.at(index));
    return step->clone();
}
//------------------------------------------------------
bool NCPProgram::insertStep(int index, NCPStep* step)
{
    if (step == nullptr)
        return false;
    if ((index == 0) && (mSteps.size() == 0)) {
        // step list is empty.
        // nop.
    } else if ((index < 0) || (mSteps.size() <= index)) {
        // out of range.
        return false;
    }
    appendLabel(step->command()->labelNo());
    assignLabel(step->labelNo());
    assignPoint(step->command());
    mSteps.insert(index, step);
    emit stepsChanged(mSteps);
    return true;
}
//------------------------------------------------------
void NCPProgram::appendStep(NCPStep *step)
{
    if (step == nullptr) {
        return;
    }
    appendLabel(step->command()->labelNo());
    assignLabel(step->labelNo());
    assignPoint(step->command());
    mSteps.append(step);
    emit stepsChanged(mSteps);
}
//------------------------------------------------------
bool NCPProgram::removeStep(int index)
{
    if ((index < 0) || (mSteps.size() <= index))
        return false;
    auto step = static_cast<NCPStep*>(mSteps.takeAt(index));
    removeLabelInNcpCommand(step->command());
    releaseLabelInNcpStep(step);
    releasePoint(step->command());
    emit stepsChanged(mSteps);
    delete step;
    return true;
}
//------------------------------------------------------
void NCPProgram::replaceCommand(int index, NCPCommand *command)
{
    if (command == nullptr)
        return;
    if (index >= mSteps.length())
        return;

    auto step = static_cast<NCPStep *>(mSteps.at(index));

    auto prevLabelNo = step->command()->labelNo();
    auto newLabelNo = command->labelNo();
    if (prevLabelNo != newLabelNo) {
        removeLabelInNcpCommand(step->command());
        appendLabel(newLabelNo);
    }

    releasePoint(step->command());

    step->setCommand(command);
}
//------------------------------------------------------
void NCPProgram::appendLabel(int labelNo)
{
    if (labelNo <= 0) { return; }
    if (mLabels.contains(labelNo)) { return; }
    auto label = mNcpLabelStorage->labelAt(labelNo);
    if (!label) {
        qWarning() << QString("not found, The label(No.%1) is out of range.").arg(labelNo);
        return;
    }
    appendLabel(label);
}
//------------------------------------------------------
void NCPProgram::appendLabel(NCPLabel* label)
{
    mLabels.insert(label->no(), label);
    emit labelsChanged(labels());
    emit unUsedLabelsChanged(unUsedLabels());
}
//------------------------------------------------------
void NCPProgram::removeLabel(int labelNo)
{
    if (!mLabels.contains(labelNo)) { return; }
    mLabels.remove(labelNo);
    emit labelsChanged(labels());
    emit unUsedLabelsChanged(unUsedLabels());
}
//------------------------------------------------------
void NCPProgram::assignLabel(int labelNo)
{
    if (labelNo <= 0) { return; }
    auto label = mNcpLabelStorage->assignLabel(labelNo, mNo);
    if (!label) {
        qWarning() << QString("assign label failed, The label(No.%1) has already been assigned.").arg(labelNo);
    }
    emit unUsedLabelsChanged(unUsedLabels());
}
//------------------------------------------------------
void NCPProgram::releaseLabel(int labelNo)
{
    if (labelNo <= 0) { return; }
    auto label = mNcpLabelStorage->labelAt(labelNo);
    mNcpLabelStorage->releaseLable(label);
    emit unUsedLabelsChanged(unUsedLabels());
}
//------------------------------------------------------
bool NCPProgram::isLabelReferencedbyCommand(int labelNo)
{
    for (auto& item : mSteps) {
        auto step = dynamic_cast<NCPStep*>(item);
        if (labelNo == step->command()->labelNo()) {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------
bool NCPProgram::assignLabelNoInNcpStep(int index, int labelNo)
{
    if ((index < 0) || (mSteps.size() <= index))
        return false;
    auto step = dynamic_cast<NCPStep*>(mSteps.at(index));
    step->setLabelNo(labelNo);
    assignLabel(labelNo);
    return true;
}
//------------------------------------------------------
bool NCPProgram::releaseLabelInNcpStep(int index)
{
    if ((index < 0) || (mSteps.size() <= index))
        return false;
    auto step = dynamic_cast<NCPStep*>(mSteps.at(index));
    releaseLabelInNcpStep(step);
    return true;
}
//------------------------------------------------------
void NCPProgram::releaseLabelInNcpStep(NCPStep* step)
{
    auto labelNo = step->labelNo();
    if (labelNo <= 0) { return; }
    step->setLabelNo(-1);
    releaseLabel(labelNo);
}
//------------------------------------------------------
void NCPProgram::removeLabelInNcpCommand(NCPCommand* command)
{
    if (!command) { return; }
    auto labelNo = command->labelNo();
    if (labelNo <= 0) { return; }
    command->setLabelNo(-1);
    auto isReferenced = isLabelReferencedbyCommand(labelNo);
    if (!isReferenced) {
        removeLabel(labelNo);
    }
}

void NCPProgram::assignPoint(NCPCommand *command)
{
    if (command == nullptr)
        return;

    int pointNo = -1;
    switch(command->type()) {
    case NCPCommand::Type::Move:
    case NCPCommand::Type::Movep:
        pointNo = dynamic_cast<NCPCommandMove *>(command)->pointNo();
        break;
    case NCPCommand::Type::Line:
    case NCPCommand::Type::Linec:
        pointNo = dynamic_cast<NCPCommandLine *>(command)->pointNo();
        break;
    case NCPCommand::Type::Arc:
    case NCPCommand::Type::Arcc:
        pointNo = dynamic_cast<NCPCommandArc *>(command)->pointNo();
        auto endPointNo = dynamic_cast<NCPCommandArc *>(command)->endPointNo();
        if (endPointNo >= 1) {
            mNcpArbitraryPointStorage->assignPoint(endPointNo);
        }
        break;
    }
    if (pointNo >= 1) {
        mNcpArbitraryPointStorage->assignPoint(pointNo);
    }
}

void NCPProgram::releasePoint(NCPCommand *command)
{
    if (command == nullptr)
        return;

    int pointNo = -1;
    switch(command->type()) {
    case NCPCommand::Type::Move:
    case NCPCommand::Type::Movep:
        pointNo = dynamic_cast<NCPCommandMove *>(command)->pointNo();
        break;
    case NCPCommand::Type::Line:
    case NCPCommand::Type::Linec:
        pointNo = dynamic_cast<NCPCommandLine *>(command)->pointNo();
        break;
    case NCPCommand::Type::Arc:
    case NCPCommand::Type::Arcc:
        pointNo = dynamic_cast<NCPCommandArc *>(command)->pointNo();
        auto endPointNo = dynamic_cast<NCPCommandArc *>(command)->endPointNo();
        if (endPointNo >= 1) {
            mNcpArbitraryPointStorage->releasePoint(endPointNo);
        }
        break;
    }
    if (pointNo >= 1) {
        mNcpArbitraryPointStorage->releasePoint(pointNo);
    }
}
