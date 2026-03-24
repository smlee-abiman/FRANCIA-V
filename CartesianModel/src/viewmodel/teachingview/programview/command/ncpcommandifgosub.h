#ifndef NCPCOMMANDIFGOSUB_H
#define NCPCOMMANDIFGOSUB_H

#include "ncpcommandin.h"

//------------------------------------------------------
// NCPCommandIfGoSub.
//------------------------------------------------------
class NCPCommandIfGoSub : public NCPCommandIn
{
    Q_OBJECT
public:
    explicit NCPCommandIfGoSub(QObject *parent = nullptr);
    explicit NCPCommandIfGoSub(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandIfGoSub(ResourceManager *resource, int programNo, QObject *parent = nullptr);

    // gettter/setter.
    void setProgramNo(int no);
    int programNo() const { return m_programNo; }

    // NCPCommand interface
    virtual int type() const override { return Type::IfGoSub; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual bool parse(const QString &constCommandLine) override;
    virtual QString nameString() const override;
    virtual NCPCommand* clone() const override;

protected:
    // for toCode().
    virtual QString command() const override { return "IF"; }
    virtual QString toAdditionalString() const override;
    virtual QString toAdditionalCode() const override;

private:
    bool parseProgramNo(const QString &programNoStr);

private:
    int m_programNo = 0;
};

#endif // NCPCOMMANDIFGOSUB_H
