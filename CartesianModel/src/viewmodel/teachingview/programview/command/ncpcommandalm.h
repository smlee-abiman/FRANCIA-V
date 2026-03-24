#ifndef NCPCOMMANDALM_H
#define NCPCOMMANDALM_H

#include "../ncpcommand.h"

const int PEN_MSG_PROGRAM_WIZARD_ALARM = 548;
const int PEN_MSG_PROGRAM_WIZARD_USER_ALARM = 668;
//------------------------------------------------------
// NCPCommandAlm.
//------------------------------------------------------
class NCPCommandAlm : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandAlm(QObject *parent = nullptr);
    explicit NCPCommandAlm(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandAlm(ResourceManager *resource, int alarmIndex, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Alm; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu5; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    NCPCommand* clone() const override;

private:
    int mCurAlarmIndex = -1;
};

#endif // NCPCOMMANDALM_H
