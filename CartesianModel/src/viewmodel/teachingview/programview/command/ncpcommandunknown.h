#ifndef NCPCOMMANDUNKNOWN_H
#define NCPCOMMANDUNKNOWN_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandUnknown.
//------------------------------------------------------
class NCPCommandUnknown : public NCPCommand
{
public:
    explicit NCPCommandUnknown(QObject *parent = nullptr);
    explicit NCPCommandUnknown(const QString& commandLine, QObject *parent = nullptr);
    virtual ~NCPCommandUnknown();

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Unknown; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::UnknownMenu; }
    bool parse(const QString& commandLine) override;
    virtual QString nameString() const override { return "Unknown"; };
    QString toString() const override { return mCommandLine; }
    QString toCode() const override { return mCommandLine; }
    virtual NCPCommand* clone() const override;

private:
    QString mCommandLine;
};

#endif // NCPCOMMANDUNKNOWN_H
