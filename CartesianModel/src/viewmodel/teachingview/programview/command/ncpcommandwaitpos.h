#ifndef NCPCOMMANDWAITPOS_H
#define NCPCOMMANDWAITPOS_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandWaitPOS.
//------------------------------------------------------
class NCPCommandWaitPOS : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandWaitPOS(QObject *parent = nullptr);
    explicit NCPCommandWaitPOS(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    int type() const override { return Type::WaitPos; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;
};

#endif // NCPCOMMANDWAITPOS_H
