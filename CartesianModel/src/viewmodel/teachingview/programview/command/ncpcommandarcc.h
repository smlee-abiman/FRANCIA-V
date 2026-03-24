#ifndef NCPCOMMANDARCC_H
#define NCPCOMMANDARCC_H

#include "ncpcommandarc.h"

//------------------------------------------------------
// NCPCommandArcc.
//------------------------------------------------------
class NCPCommandArcc : public NCPCommandArc
{
    Q_OBJECT
public:
    explicit NCPCommandArcc(QObject *parent = nullptr);
    explicit NCPCommandArcc(ResourceManager *resource, QObject *parent = nullptr);

    QString command() const override { return "ARCC"; }
    // NCPCommand interface
    virtual int type() const override { return Type::Arcc; }
    NCPCommand* clone() const override;
};

#endif // NCPCOMMANDARCC_H
