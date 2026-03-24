#ifndef NCPCOMMANDCCLR_H
#define NCPCOMMANDCCLR_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandCClr.
//------------------------------------------------------
class NCPCommandCClr : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandCClr(QObject *parent = nullptr);
    explicit NCPCommandCClr(ResourceManager *resource, QObject *parent = nullptr);

    // gettter/setter.
    QString symbolStr() const { return m_symbolStr; }
    void setSymbolStr(const QString &symbolStr) { m_symbolStr = symbolStr; }

    // NCPCommand interface
    virtual int type() const override { return Type::CClr; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    virtual NCPCommand *clone() const override;

private:
    QString m_symbolStr;
};

#endif // NCPCOMMANDCCLR_H
