#ifndef NCPCOMMANDRET_H
#define NCPCOMMANDRET_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandRet.
//------------------------------------------------------
class NCPCommandRet : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandRet(QObject *parent = nullptr);
    explicit NCPCommandRet(ResourceManager *resource, QObject *parent = nullptr);
    virtual ~NCPCommandRet();

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Ret; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual bool parse(const QString &/*commandLine*/) override { return true; }
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    virtual NCPCommand* clone() const override;
};

#endif // NCPCOMMANDRET_H
