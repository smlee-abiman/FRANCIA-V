#ifndef NCPCOMMANDLINEC_H
#define NCPCOMMANDLINEC_H

#include "ncpcommandline.h"

//------------------------------------------------------
// NCPCommandLinec.
//------------------------------------------------------
class NCPCommandLinec : public NCPCommandLine
{
    Q_OBJECT
public:
    explicit NCPCommandLinec(QObject *parent = nullptr);
    explicit NCPCommandLinec(ResourceManager *resource, QObject *parent = nullptr);

    QString command() const override { return "LINEC"; }
    // NCPCommand interface
    virtual int type() const override { return Type::Linec; }
    NCPCommand* clone() const override;
};

#endif // NCPCOMMANDLINEC_H
