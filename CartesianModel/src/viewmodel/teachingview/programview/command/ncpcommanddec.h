#ifndef NCPCOMMANDDEC_H
#define NCPCOMMANDDEC_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandDec.
//------------------------------------------------------
class NCPCommandDec : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandDec(QObject *parent = nullptr);
    explicit NCPCommandDec(ResourceManager *resource, QObject *parent = nullptr);

    // gettter/setter.
    QString symbolStr() const { return m_symbolStr; }
    void setSymbolStr(const QString &symbolStr) { m_symbolStr = symbolStr; }

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Dec; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    virtual NCPCommand *clone() const override;

private:
    QString m_symbolStr;
};

#endif // NCPCOMMANDDEC_H
