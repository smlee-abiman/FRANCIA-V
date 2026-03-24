#ifndef NCPCOMMANDPROG_H
#define NCPCOMMANDPROG_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandProg.
//------------------------------------------------------
class NCPCommandProg : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandProg(QObject *parent = nullptr);
    explicit NCPCommandProg(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandProg(ResourceManager *resource, const QString& commandLine, QObject *parent = nullptr);
    virtual ~NCPCommandProg();

public:
    // setter/getter.
    int no() const { return mNo; }
    void setNo(int no) { mNo = no; }

    // NCPCommand interface
    virtual int type() const override { return Type::Prog; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::UnknownMenu; }
    bool parse(const QString& commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    virtual NCPCommand* clone() const override;

private:
    int mNo;
};

#endif // NCPCOMMANDPROG_H
