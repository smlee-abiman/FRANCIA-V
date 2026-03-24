#ifndef NCPCOMMANDIFGOTO_H
#define NCPCOMMANDIFGOTO_H

#include "ncpcommandin.h"

//------------------------------------------------------
// NCPCommandIfGoTo.
//------------------------------------------------------
class NCPCommandIfGoTo : public NCPCommandIn
{
    Q_OBJECT
public:
    explicit NCPCommandIfGoTo(QObject *parent = nullptr);
    explicit NCPCommandIfGoTo(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandIfGoTo(ResourceManager *resource, int labelNo, QObject *parent = nullptr);

    // gettter/setter.
    virtual void setLabelNo(int labelNo) override;
    virtual int labelNo() override { return m_labelNo; };

    // NCPCommand interface
    virtual int type() const override { return Type::IfGoTo; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual NCPCommand* clone() const override;

protected:
    // for toCode().
    virtual QString command() const override { return "IF"; }
    virtual QString toAdditionalString() const override;
    virtual QString toAdditionalCode() const override;

private:
    bool parseLabelNo(const QString &labelNoStr);

private:
    int m_labelNo = -1;
};

#endif // NCPCOMMANDIFGOTO_H
