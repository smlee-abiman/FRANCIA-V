#ifndef NCPCOMMANDMOVEP_H
#define NCPCOMMANDMOVEP_H

#include "ncpcommandmove.h"

//------------------------------------------------------
// NCPCommandMovep.
//------------------------------------------------------
class NCPCommandMovep : public NCPCommandMove
{
    Q_OBJECT
public:
    explicit NCPCommandMovep(QObject *parent = nullptr);
    explicit NCPCommandMovep(ResourceManager *resource, QObject *parent = nullptr);

public:
    // NCPCommand interface
    int type() const override { return Type::Movep; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    QString command() const override { return "MOVEP"; }
    NCPCommand* clone() const override;
};

#endif // NCPCOMMANDMOVEP_H
