#ifndef NCPCOMMANDOUTP_H
#define NCPCOMMANDOUTP_H

#include "ncpcommandout.h"

//------------------------------------------------------
// NCPCommandOutp.
//------------------------------------------------------
class NCPCommandOutp : public NCPCommandOut
{
    Q_OBJECT
public:
    explicit NCPCommandOutp(QObject *parent = nullptr);
    explicit NCPCommandOutp(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Outp; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu1; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    QString command() const override{ return "OUTP"; }
    NCPCommand* clone() const override;
};

#endif // NCPCOMMANDOUTP_H
