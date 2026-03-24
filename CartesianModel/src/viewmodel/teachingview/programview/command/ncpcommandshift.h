#ifndef NCPCOMMANDSHIFT_H
#define NCPCOMMANDSHIFT_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandShift.
//------------------------------------------------------
class NCPCommandShift : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandShift(QObject *parent = nullptr);
    explicit NCPCommandShift(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Unknown; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::UnknownMenu; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
};

#endif // NCPCOMMANDSHIFT_H
