#ifndef NCPCOMMANDHOMEP_H
#define NCPCOMMANDHOMEP_H

#include "ncpcommandhome.h"

//------------------------------------------------------
// NCPCommandHomep.
//------------------------------------------------------
class NCPCommandHomep : public NCPCommandHome
{
    Q_OBJECT
public:
    explicit NCPCommandHomep(QObject *parent = nullptr);
    explicit NCPCommandHomep(ResourceManager *resource, QObject *parent = nullptr);

    QString command() const override { return "HOMEP"; }
    // NCPCommand interface
    int type() const override { return Type::Homep; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    NCPCommand* clone() const override;
};

#endif // NCPCOMMANDHOMEP_H
