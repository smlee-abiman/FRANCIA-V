#ifndef NCPCOMMANDNEXT_H
#define NCPCOMMANDNEXT_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandNext.
//------------------------------------------------------
class NCPCommandNext : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandNext(QObject *parent = nullptr);
    explicit NCPCommandNext(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Unknown; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::UnknownMenu; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
};

#endif // NCPCOMMANDNEXT_H
