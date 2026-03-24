#ifndef NCPCOMMANDARC_H
#define NCPCOMMANDARC_H

#include "ncpcommandline.h"

//------------------------------------------------------
// NCPCommandArc.
//------------------------------------------------------
class NCPCommandArc : public NCPCommandLine
{
    Q_OBJECT
public:
    explicit NCPCommandArc(QObject *parent = nullptr);
    explicit NCPCommandArc(ResourceManager *resource, QObject *parent = nullptr);

    void setEndPointNo(int no);
    int endPointNo() const { return m_endPointNo; }
    QString command() const override { return "ARC"; }
    // NCPCommand interface
    int type() const override { return Type::Arc; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;

protected:
    int         m_endPointNo = 0;
};

#endif // NCPCOMMANDARC_H
