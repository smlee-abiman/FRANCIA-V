#ifndef NCPCOMMANDPAUSE_H
#define NCPCOMMANDPAUSE_H

#include "../ncpcommand.h"

const int PEN_MSG_PROGRAM_WIZARD_PAUSE_ON   = 545;
const int PEN_MSG_PROGRAM_WIZARD_PAUSE_OFF  = 546;
//------------------------------------------------------
// NCPCommandPause.
//------------------------------------------------------
class NCPCommandPause : public NCPCommand
{
    Q_OBJECT
public:
    enum PauseCategory {
        PauseCategoryInvalid = -1,
        PauseCategoryOn,
        PauseCategoryOff,
    };
    explicit NCPCommandPause(QObject *parent = nullptr);
    explicit NCPCommandPause(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandPause(ResourceManager *resource, int category, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Pause; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu5; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    NCPCommand* clone() const override;

private:
    int mCurCategory = -1;
};

#endif // NCPCOMMANDPAUSE_H
