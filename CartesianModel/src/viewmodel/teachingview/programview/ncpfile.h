#ifndef NCPFILE_H
#define NCPFILE_H

#include "ncplabelstorage.h"
#include "ncpprogram.h"
#include "../communication/communicationthread.h"

#include <QObject>
#include <QFile>
#include <QList>
#include <QMap>

class NCPCommandFactory;
class ResourceManager;
class NCPArbitraryPointStorage;
class NCPArbitraryPoint;

class NCPFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool edited READ edited NOTIFY editedChanged)
    Q_PROPERTY(QObject* mainProgram READ mainProgram NOTIFY mainProgramChanged)
    Q_PROPERTY(QList<QObject*> programs READ programs NOTIFY programsChanged)
    Q_PROPERTY(QList<QObject*> userPrograms READ userPrograms NOTIFY userProgramsChanged)
    Q_PROPERTY(QList<QObject*> manualPrograms READ manualPrograms NOTIFY manualProgramsChanged)
    Q_PROPERTY(QList<QObject*> systemPrograms READ systemPrograms NOTIFY systemProgramsChanged)
    Q_PROPERTY(QObject* labelStorage READ ncpLabelStorage CONSTANT)
public:
    explicit NCPFile(ResourceManager *resource, const QString& versionString = "", QObject *parent = nullptr);
    explicit NCPFile(QObject *parent = nullptr);
    virtual ~NCPFile();

    // getter/setter.
    bool edited() const { return mEdited; }
    void setEdited(bool edited);
    QObject* mainProgram() const { return mMainProgram; }
    QList<QObject*> programs() const { return mPrograms; }
    QList<QObject*> userPrograms() const { return mUserPrograms.values(); }
    QList<QObject*> manualPrograms() const { return mManualPrograms.values(); }
    QList<QObject*> systemPrograms() const { return mSystemPrograms.values(); }
    NCPLabelStorage* ncpLabelStorage() const { return mNcpLabelStorage; }
    NCPProgram *getProgram(int programNo);

    // method.
    NCPFile* clone() const;
    void load(const QString& filePath);
    void load(QFile& ncpFile); // QFile& ncpFile ... opend file.
    bool save(const QString& filePath);
    bool save(QFile& ncpFile); // QFile& ncpFile ... opend file.
    void appendProgram(NCPProgram* program);
    int getAvailableJigProgramNo();
    bool replaceNcpCommand(NCPProgram* program, int index, const QString& commandLine);
    NCPProgram* createProgram(int programNo);
    qint64 size(const QString& filePath);

    Q_INVOKABLE bool checkExistUserPrograms(int programNo){ return mUserPrograms.contains(programNo); }
    Q_INVOKABLE bool checkExistManualPrograms(int programNo){ return mManualPrograms.contains(programNo); }
    Q_INVOKABLE bool checkExistSystemPrograms(int programNo){ return mSystemPrograms.contains(programNo); }
    Q_INVOKABLE void createProgramForQml(int programNo); // NOTE: The created object is owned by C++.
    Q_INVOKABLE void deleteProgram(int programNo);
    Q_INVOKABLE int getCompileErrorStepIndex(int errLineNo);
    Q_INVOKABLE QObject* getProgramFromCompileErrorStepNoForQml(int errorStepNo);
    Q_INVOKABLE int getNcpStepNoInProgramFromCompileErrorStepNo(int errorStepNo);

    // for label.
    Q_INVOKABLE QObject* labelAt(int labelNo);
    Q_INVOKABLE QObject* findFreeLabel();
    Q_INVOKABLE QObject* findFreeLabel(QList<QObject*> excludeLabels);
    Q_INVOKABLE bool isEmptyFreeLabels();
    Q_INVOKABLE bool isExistFreeLabels(int labelNo);

    NCPArbitraryPoint *findFreeArbitraryPoint(int index = 0);
    NCPArbitraryPoint *assignPoint(int pointNo);
    void releasePoint(int pointNo);

private:
    // getter/setter.
    void setMainProgram(NCPProgram* mainProgram);
    void parseNCPVersion(QString readLine);
    void setModbusEdit();

signals:
    void mainProgramChanged(QObject*);
    void editedChanged(bool);
    void programsChanged(QList<QObject*>);
    void userProgramsChanged(QList<QObject*>);
    void manualProgramsChanged(QList<QObject*>);
    void systemProgramsChanged(QList<QObject*>);
    void labelStorageChanged(QObject*);

private:
    ResourceManager *mResource = nullptr;
    CommunicationThread *mModbus = CommunicationThread::get_instance();
    NCPCommandFactory *mCommandFactory = nullptr;
    NCPLabelStorage *mNcpLabelStorage = nullptr;
    NCPArbitraryPointStorage *mNcpArbitraryPointStorage = nullptr;
    bool mEdited;
    NCPProgram* mMainProgram = nullptr;
    QList<QObject*> mPrograms;           // QObject* -> NCPProgram*
    QMap<int, QObject*> mUserPrograms;   // QObject* -> NCPProgram*
    QMap<int, QObject*> mManualPrograms; // QObject* -> NCPProgram*
    QMap<int, QObject*> mSystemPrograms; // QObject* -> NCPProgram*
    QString mVersionString;
};

#endif // NCPFILE_H
