#ifndef NCPCOMMANDCINC_H
#define NCPCOMMANDCINC_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandCInc.
//------------------------------------------------------
class NCPCommandCInc : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandCInc(QObject *parent = nullptr);
    explicit NCPCommandCInc(ResourceManager *resource, QObject *parent = nullptr);

    // gettter/setter.
    QString symbolStr() const { return m_symbolStr; }
    void setSymbolStr(const QString &symbolStr) { m_symbolStr = symbolStr; }

public:
    // NCPCommand interface
    virtual int type() const override { return Type::CInc; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    virtual NCPCommand *clone() const override;

private:
    QString m_symbolStr;
};

#endif // NCPCOMMANDCINC_H
