#ifndef NCPCOMMANDSET_H
#define NCPCOMMANDSET_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandSet.
//------------------------------------------------------
class NCPCommandSet : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandSet(QObject *parent = nullptr);
    explicit NCPCommandSet(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Unknown; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::UnknownMenu; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
};

#endif // NCPCOMMANDSET_H
