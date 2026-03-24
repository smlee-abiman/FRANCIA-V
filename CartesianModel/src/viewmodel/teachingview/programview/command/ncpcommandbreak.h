#ifndef NCPCOMMANDBREAK_H
#define NCPCOMMANDBREAK_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandBreak.
//------------------------------------------------------
class NCPCommandBreak : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandBreak(QObject *parent = nullptr);
    explicit NCPCommandBreak(ResourceManager *resource, QObject *parent = nullptr);

    void appendAxis(int axis);
    QList<int> axisList() const { return m_axisList; }
    // NCPCommand interface
    int type() const override { return Type::Break; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;
private:
    QList<int>  m_axisList;
};

#endif // NCPCOMMANDBREAK_H
