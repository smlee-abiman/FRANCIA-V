#ifndef NCPCOMMANDON_H
#define NCPCOMMANDON_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandOn.
//------------------------------------------------------
class NCPCommandOn : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandOn(QObject *parent = nullptr);
    explicit NCPCommandOn(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandOn(ResourceManager *resource, int programNo, QObject *parent = nullptr);

    void setProgramNo(int no);
    int programNo() const { return m_programNo; }

    // NCPCommand interface
    virtual int type() const override { return Type::On; }
    virtual int wizardMenuIndex() const override;
    virtual bool parse(const QString& commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    virtual NCPCommand* clone() const override;

private:
    int             m_programNo = 0;
};

#endif // NCPCOMMANDON_H
