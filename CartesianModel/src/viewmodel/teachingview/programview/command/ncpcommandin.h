#ifndef NCPCOMMANDIN_H
#define NCPCOMMANDIN_H

#include "../ncpcommand.h"
#include "ncpvariable.h"

//------------------------------------------------------
// NCPCommandIn.
//------------------------------------------------------
class NCPCommandIn : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandIn(QObject *parent = nullptr);
    explicit NCPCommandIn(ResourceManager *resource, QObject *parent = nullptr);
    virtual ~NCPCommandIn();
    // NCPCommand interface
    virtual int type() const override { return Type::In; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu4; }
    virtual bool parse(const QString &constCommandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    virtual NCPCommand* clone() const override;
    // method.
    void append(NCPVariable* var);
    // setter/getter.
    NCPVariable* variableAt(int index) const;
signals:
public slots:

protected:
    virtual QString command() const { return "IN"; }
    virtual QString toAdditionalString() const { return ""; }
    virtual QString toAdditionalCode() const { return ""; }

private:
    QString toVariablesString() const;
    QString toVariablesCode() const;

protected:
    QList<NCPVariable*> m_variableList;
};

#endif // NCPCOMMANDIN_H
