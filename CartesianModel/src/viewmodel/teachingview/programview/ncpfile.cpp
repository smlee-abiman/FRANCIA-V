#include <QTextStream>
#include <QDebug>
#include <QQmlEngine>
#include <QRegularExpressionMatch>

#include "ncpfile.h"
#include "ncpprogram.h"
#include "ncpstep.h"
#include "ncpcommandfactory.h"
#include "ncplabelstorage.h"
#include "ncparbitrarypointstorage.h"

const int NCPFILE_JIG_PROGRAM_NO_START  = 50;
const int NCPFILE_JIG_PROGRAM_NO_END    = 99;

//------------------------------------------------------
// NCPFile.
//------------------------------------------------------
NCPFile::NCPFile(ResourceManager *resource, const QString &versionString, QObject *parent)
    : QObject(parent)
    , mResource(resource)
    , mEdited(false)
    , mVersionString(versionString)
{
    mCommandFactory = new NCPCommandFactory(resource);
    mNcpLabelStorage = new NCPLabelStorage();
    mNcpArbitraryPointStorage = new NCPArbitraryPointStorage();
}
//------------------------------------------------------
NCPFile::NCPFile(QObject *parent)
    : QObject(parent)
    , mResource(nullptr)
    , mEdited(false)
    , mVersionString("")
{
}
//------------------------------------------------------
NCPFile::~NCPFile()
{
    qDeleteAll(mPrograms);
    mPrograms.clear();
    mUserPrograms.clear();
    mManualPrograms.clear();
    mSystemPrograms.clear();
    delete mCommandFactory;
    delete mNcpLabelStorage;
    delete mNcpArbitraryPointStorage;
}
//------------------------------------------------------
void NCPFile::setEdited(bool edited)
{
    if(edited == true && mEdited != edited){
        setModbusEdit();
    }
    if (mEdited == edited) { return; }
    mEdited = edited;
    emit editedChanged(mEdited);
}
//------------------------------------------------------
void NCPFile::setMainProgram(NCPProgram *mainProgram)
{
    if (mMainProgram == mainProgram) { return; }
    mMainProgram = mainProgram;
    emit mainProgramChanged(mMainProgram);
}
//------------------------------------------------------
NCPProgram *NCPFile::getProgram(int programNo)
{
    NCPProgram *program = nullptr;
    if (mUserPrograms.contains(programNo)) {
        program = dynamic_cast<NCPProgram *>(mUserPrograms.value(programNo));
    } else if (mManualPrograms.contains(programNo)) {
        program = dynamic_cast<NCPProgram *>(mManualPrograms.value(programNo));
    } else if(mSystemPrograms.contains(programNo)) {
        program = dynamic_cast<NCPProgram *>(mSystemPrograms.value(programNo));
    }
    return program;
}

//------------------------------------------------------
NCPFile* NCPFile::clone() const
{
qInfo() << "clone start.";     // todo:debug.
    auto selfClone = new NCPFile(mResource, mVersionString);
    for (auto& item : mPrograms) {
        auto program = dynamic_cast<NCPProgram*>(item);
        auto programClone = program->clone(selfClone->mNcpLabelStorage, selfClone->mNcpArbitraryPointStorage);
        selfClone->appendProgram(programClone);
        if (programClone->no() == 1) {
            selfClone->setMainProgram(programClone);
        }
    }
qInfo() << "clone end.";       // todo: debug.
    return selfClone;
}
//------------------------------------------------------
void NCPFile::load(const QString &filePath)
{
qInfo() << "load:" << filePath;    // todo: debug.
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << QString("%1(%2): load failed : Failed to open the file:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << filePath;
        return;
    }
    load(file);
    file.close();
qInfo() << "load end.";            // todo: debug.
}
//------------------------------------------------------
// ncpFile ... opend file.
void NCPFile::load(QFile &ncpFile)
{
    QTextStream input(&ncpFile);
    input.seek(0);

    NCPProgram* program = nullptr;
    NCPProgram* mainProgram = nullptr;
    while (!input.atEnd()) {
        auto readLine = input.readLine();
        if (readLine.contains(":NCPVER")) {
            parseNCPVersion(readLine);
            continue;
        }
        auto step = new NCPStep();
        step->parse(mCommandFactory, readLine);
        if (step->command()->type() == NCPCommand::Type::Prog) {
            program = new NCPProgram(mNcpLabelStorage, mNcpArbitraryPointStorage, *step->command());
            appendProgram(program);
            if (program->no() == 1) {
                mainProgram = program;
            }
            delete step; // Do not include a "PROG" step.
            continue;
        } else if (step->command()->type() == NCPCommand::Type::Unknown) {
            delete step; // Do not include a "UnKnown" step.
            continue;
        }
        if (!program) {
            delete step;
            continue;
        }
        program->appendStep(step);
    }
    // If program 001 does not exist, create a new one.
    if (!mainProgram) {
        mainProgram = createProgram(1);
    }
    setMainProgram(mainProgram);
    emit programsChanged(mPrograms);
    emit userProgramsChanged(userPrograms());
    emit manualProgramsChanged(manualPrograms());
    emit systemProgramsChanged(systemPrograms());
}
//------------------------------------------------------
void NCPFile::parseNCPVersion(QString readLine)
{
    QRegularExpression re("(\\s+|\\t+)");
    auto splitList = readLine.split(re);
    mVersionString = "";
    if (2 <= splitList.size()) {
        if (splitList[0] == ":NCPVER") {
            mVersionString = splitList[1];
        }
    }
}
//------------------------------------------------------
void NCPFile::setModbusEdit()
{
    QList<int> writeData;
    writeData.append(1);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteMultipleRegisters, 2539, 0x0001, writeData);
    mModbus->setModbusWriteCommand(command);
}
//------------------------------------------------------
// Note:
//   output file codec : utf-8 with bom.
//   If the file exists, it will be overwritten and saved.
bool NCPFile::save(const QString &filePath)
{
qInfo() << "save:" << filePath;    // todo: debug.
    bool ret;
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << QString("%1(%2): save failed:Failed to open the file:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << filePath;
        return false;
    }
    ret = save(file);
    file.close();
qInfo() << "save end:" << ret;     // todo: debug.
    return ret;
}
//------------------------------------------------------
// ncpFile ... opend file.
bool NCPFile::save(QFile &ncpFile)
{
    QTextStream out(&ncpFile);
    out.setCodec("UTF-8");
    out << ":NCPVER " << mVersionString << QChar(0x0d) << QChar(0x0a) << flush;
    for (auto& item : mPrograms) {
        auto program = dynamic_cast<NCPProgram*>(item);
        program->writeStepCodes(out);
    }
    out << QChar(0x1a) << flush;    // write control character 0x1A(SUB/EOF).
    return true;
}
//------------------------------------------------------
// Note:
//   The created object is owned by C++.
void NCPFile::createProgramForQml(int programNo)
{
    auto program = createProgram(programNo);
    QQmlEngine::setObjectOwnership(program, QQmlEngine::CppOwnership);
}
//------------------------------------------------------
NCPProgram* NCPFile::createProgram(int programNo)
{
qDebug() << "create program:" << programNo; // todo: debug.
    auto program = new NCPProgram(mNcpLabelStorage, mNcpArbitraryPointStorage, programNo);
    appendProgram(program);

    auto command = mCommandFactory->createCommand("RET");
    program->appendStep(new NCPStep(command));
    return program;
}
//------------------------------------------------------
qint64 NCPFile::size(const QString& filePath)
{
qDebug() << "NCPFile::size:" << filePath;    // todo: debug.
    QFile file(filePath);
    qint64 fileSize = file.size();
qDebug() << "NCPFile::size end:" << fileSize;     // todo: debug.
    return fileSize;
}
//------------------------------------------------------
void NCPFile::appendProgram(NCPProgram* program)
{
    auto programNo = program->no();
    mPrograms.append(program);

    if (programNo <= 0 || 255 < programNo) {
        // todo:
        qWarning() << QString("%1(%2): out of range program number:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << programNo;
        return;
    }

    QMap<int, QObject*>* destPrograms;
    if (1 <= programNo && programNo <= 99) {
        destPrograms = &mUserPrograms;
    } else if (178 <= programNo && programNo <= 249) {
        destPrograms = &mManualPrograms;
    } else {
        destPrograms = &mSystemPrograms;
    }
    if (destPrograms->contains(programNo)) {
        qWarning() << QString("%1(%2): duplicate program number:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << programNo;
        return;
    }
    destPrograms->insert(programNo, program);

    if (1 <= programNo && programNo <= 99) {
        emit userProgramsChanged(userPrograms());
    } else if (178 <= programNo && programNo <= 249) {
        emit manualProgramsChanged(manualPrograms());
    } else {
        emit systemProgramsChanged(systemPrograms());
    }
}
//------------------------------------------------------
void NCPFile::deleteProgram(int programNo)
{
    QMutableListIterator<QObject*> iterator(mPrograms);
    while(iterator.hasNext()) {
        auto program = dynamic_cast<NCPProgram*>(iterator.next());
        if (program->no() == programNo) {
            iterator.remove();
            delete program;
        }
    }

    if (programNo <= 0 || 255 < programNo) {
        qWarning() << QString("%1(%2): out of range program number:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << programNo;
        return;
    }

    if(checkExistUserPrograms(programNo)) {
        mUserPrograms.remove(programNo);
        emit userProgramsChanged(userPrograms());
    } else if(checkExistManualPrograms(programNo)) {
        mManualPrograms.remove(programNo);
        emit manualProgramsChanged(manualPrograms());
    } else if(checkExistSystemPrograms(programNo)) {
        mSystemPrograms.remove(programNo);
        emit systemProgramsChanged(systemPrograms());
    } else {
        qWarning() << QString("%1(%2): not find delete program:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << programNo;
        return;
    }
}
//------------------------------------------------------
int NCPFile::getAvailableJigProgramNo()
{
    int programNo = -1;
    for (int key = NCPFILE_JIG_PROGRAM_NO_START; key <= NCPFILE_JIG_PROGRAM_NO_END; key++) {
        if (!mUserPrograms.contains(key)) {
            programNo = key;
            break;
        }
    }
    return programNo;
}
//------------------------------------------------------
bool NCPFile::replaceNcpCommand(NCPProgram* program, int index, const QString& commandLine)
{
    auto step = program->step(index);
    if (!step) {
        return false;
    }
    auto command = step->parseCommand(mCommandFactory, commandLine);
    if (command->type() == NCPCommand::Type::Unknown) {
        delete command;     // "UnKnown" command cannot be replaced.
        return false;
    }
    program->replaceCommand(index, command);
    return true;
}
//------------------------------------------------------
QObject* NCPFile::labelAt(int labelNo)
{
    return mNcpLabelStorage->labelAt(labelNo);
}
//------------------------------------------------------
QObject* NCPFile::findFreeLabel()
{
    return mNcpLabelStorage->findFreeLabel();
}
//------------------------------------------------------
QObject* NCPFile::findFreeLabel(QList<QObject*> excludeLabels)
{
    return mNcpLabelStorage->findFreeLabel(excludeLabels);
}
//------------------------------------------------------
Q_INVOKABLE bool NCPFile::isEmptyFreeLabels()
{
    return mNcpLabelStorage->isEmptyFreeLabels();
}
//------------------------------------------------------
Q_INVOKABLE bool NCPFile::isExistFreeLabels(int labelNo)
{
    return mNcpLabelStorage->isExistFreeLabels(labelNo);
}

NCPArbitraryPoint *NCPFile::findFreeArbitraryPoint(int index)
{
    return mNcpArbitraryPointStorage->findFreePoint(index);
}

NCPArbitraryPoint *NCPFile::assignPoint(int pointNo)
{
    return mNcpArbitraryPointStorage->assignPoint(pointNo);
}

void NCPFile::releasePoint(int pointNo)
{
    mNcpArbitraryPointStorage->releasePoint(pointNo);
}

//------------------------------------------------------
Q_INVOKABLE int NCPFile::getCompileErrorStepIndex(int errLineNo)
{
    //qDebug("%s:%d errLineNo[%d]",__func__, __LINE__, errLineNo);
    int total = 0;
    int stepIndex = 0;
    NCPProgram *program = nullptr;
    for (auto &item : mPrograms) {
        program = dynamic_cast<NCPProgram*>(item);
        if (program){
            total += program->steps().size() + 1;
            if ((errLineNo + 1) < total){
                stepIndex = program->steps().size() - (total - (errLineNo + 1));
                break;
            }
        }
    }
    //qDebug("%s:%d stepIndex[%d]",__func__, __LINE__, stepIndex);
    if (program) {
        //qDebug("%s:%d no[%d] steps[%d] labels[%d]",__func__, __LINE__, program->no(), program->steps().count(), program->labels().count());
        //setMainProgram(program);
    }
    return stepIndex;
}
//------------------------------------------------------
QObject *NCPFile::getProgramFromCompileErrorStepNoForQml(int errorStepNo)
{
    auto totalStep = 1;
    for (auto& item : mPrograms) {
        auto program = dynamic_cast<NCPProgram*>(item);
        auto numOfStep = program->steps().size() + 1;   // +1 ... Including PROG step.
        if (errorStepNo < (totalStep + numOfStep)) {
            QQmlEngine::setObjectOwnership(program, QQmlEngine::CppOwnership);
            return program;
        }
        totalStep += numOfStep;
    }
    return nullptr;
}
//------------------------------------------------------
int NCPFile::getNcpStepNoInProgramFromCompileErrorStepNo(int errorStepNo)
{
    auto totalStep = 1;
    for (auto& item : mPrograms) {
        auto program = dynamic_cast<NCPProgram*>(item);
        auto numOfStep = program->steps().size() + 1;   // +1 ... Including PROG steps.
        if (errorStepNo < (totalStep + numOfStep)) {
            return errorStepNo - totalStep - 1;         // -1 ... Excluding PROG steps.
        }
        totalStep += numOfStep;
    }
    return -1;
}

