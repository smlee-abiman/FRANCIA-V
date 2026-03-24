#ifndef NCPCOMMANDGOTO_H
#define NCPCOMMANDGOTO_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandGoTo.
//------------------------------------------------------
class NCPCommandGoTo : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandGoTo(QObject *parent = nullptr);
    explicit NCPCommandGoTo(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandGoTo(ResourceManager *resource, int labelNo, QObject *parent = nullptr);

    // gettter/setter.
    virtual void setLabelNo(int labelNo) override;
    virtual int labelNo() override { return m_labelNo; };

    // NCPCommand interface
    virtual int type() const override { return Type::GoTo; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    virtual NCPCommand* clone() const override;

private:
    int m_labelNo = -1;
};

#endif // NCPCOMMANDGOTO_H
