#ifndef NCPCOMMANDASPD_H
#define NCPCOMMANDASPD_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandAspd.
//------------------------------------------------------
class NCPCommandAspd : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandAspd(QObject *parent = nullptr);
    explicit NCPCommandAspd(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Unknown; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::UnknownMenu; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
};

#endif // NCPCOMMANDASPD_H
