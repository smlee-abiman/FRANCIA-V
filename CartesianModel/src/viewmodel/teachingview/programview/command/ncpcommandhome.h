#ifndef NCPCOMMANDHOME_H
#define NCPCOMMANDHOME_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandHome.
//------------------------------------------------------
class NCPCommandHome : public NCPCommand
{
    Q_OBJECT
public:
    typedef enum {
        ReturnPlusDirection,
        ReturnMinusDirection,
    } Option;
    explicit NCPCommandHome(QObject *parent = nullptr);
    explicit NCPCommandHome(ResourceManager *resource, QObject *parent = nullptr);

    void setOption(NCPCommandHome::Option option);
    NCPCommandHome::Option option() const { return m_option; }
    void appendAxis(int axis);
    QList<int> axisList() const { return m_axisList; }
    virtual QString command() const { return "HOME"; }
    // NCPCommand interface
    int type() const override { return Type::Home; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;
private:
    void setOption(const QString &str);

protected:
    QList<int>              m_axisList;
    NCPCommandHome::Option  m_option = ReturnMinusDirection;
};

#endif // NCPCOMMANDHOME_H
