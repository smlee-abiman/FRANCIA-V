#ifndef NCPCOMMANDOFF_H
#define NCPCOMMANDOFF_H

#include "../ncpcommand.h"
#include "ncpcommandon.h"

//------------------------------------------------------
// NCPCommandOff.
//------------------------------------------------------
class NCPCommandOff : public NCPCommandOn
{
    Q_OBJECT
public:
    explicit NCPCommandOff(QObject *parent = nullptr);
    explicit NCPCommandOff(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandOff(ResourceManager *resource, int programNo, QObject *parent = nullptr);

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Off; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    virtual NCPCommand *clone() const override;
};

#endif // NCPCOMMANDOFF_H
