#ifndef NCPCOMMANDSRV_H
#define NCPCOMMANDSRV_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandSrv.
//------------------------------------------------------
class NCPCommandSrv : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandSrv(QObject *parent = nullptr);
    explicit NCPCommandSrv(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Unknown; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::UnknownMenu; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
};

#endif // NCPCOMMANDSRV_H
