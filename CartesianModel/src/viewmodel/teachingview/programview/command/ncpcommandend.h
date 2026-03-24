#ifndef NCPCOMMANDEND_H
#define NCPCOMMANDEND_H

#include "../ncpcommand.h"

const int PEN_MSG_PROGRAM_WIZARD_PROG_END = 547;
//------------------------------------------------------
// NCPCommandEnd.
//------------------------------------------------------
class NCPCommandEnd : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandEnd(QObject *parent = nullptr);
    explicit NCPCommandEnd(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::End; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu5; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    NCPCommand* clone() const override;
};

#endif // NCPCOMMANDEND_H
