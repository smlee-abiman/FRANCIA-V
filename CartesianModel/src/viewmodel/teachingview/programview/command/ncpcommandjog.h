#ifndef NCPCOMMANDJOG_H
#define NCPCOMMANDJOG_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandJog.
//------------------------------------------------------
class NCPCommandJog : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandJog(QObject *parent = nullptr);
    explicit NCPCommandJog(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Unknown; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::UnknownMenu; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
};

#endif // NCPCOMMANDJOG_H
