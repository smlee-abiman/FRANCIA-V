#ifndef NCPSTEP_H
#define NCPSTEP_H

#include "ncpcommand.h"
#include <QObject>

class NCPCommandFactory;

class NCPStep : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int labelNo READ labelNo NOTIFY labelNoChanged)
    Q_PROPERTY(QObject* command READ command NOTIFY commandChanged)
public:
    explicit NCPStep(NCPCommand *command, QObject *parent = nullptr);
    explicit NCPStep(QObject *parent = nullptr);
    virtual ~NCPStep();

    int labelNo() const { return mLabelNo; }
    NCPCommand* command() const { return mCommand; }
    void setLabelNo(int labelNo) {
        if (mLabelNo == labelNo) { return; }
        mLabelNo = labelNo;
        emit labelNoChanged(mLabelNo);
    }
    void setCommand(NCPCommand *command) {
        if (mCommand == command) { return; }
        if (mCommand != nullptr) { delete mCommand; }
        mCommand = command;
        emit commandChanged(mCommand);
    }    

    bool parse(NCPCommandFactory *factory, const QString& stepLine);
    NCPCommand* parseCommand(NCPCommandFactory *factory, const QString& commandLine);
    Q_INVOKABLE QString commandName() const;
    Q_INVOKABLE QString toTranslatedString(int language) const;
    QString toCode() const;
    NCPStep* clone(const bool copyLabelNo = false) const;

signals:
    void labelNoChanged(int);
    void commandChanged(QObject*);

private:
    bool parseLabel(const QString& labelLine);

private:
    int mLabelNo; // todo: Is it better to use NCPLabel instead of int?
    NCPCommand* mCommand;
};

#endif // NCPSTEP_H
