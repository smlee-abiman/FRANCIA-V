#ifndef NCPCOMMANDLINE_H
#define NCPCOMMANDLINE_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandLine.
//------------------------------------------------------
class NCPCommandLine : public NCPCommand
{
    Q_OBJECT
public:
    explicit NCPCommandLine(QObject *parent = nullptr);
    explicit NCPCommandLine(ResourceManager *resource, QObject *parent = nullptr);
    void appendAxis(int axis);
    QList<int> axisList() const { return m_axisList; }
    void setPointNo(int no);
    int pointNo() const { return m_pointNo; }
    virtual QString command() const { return "LINE"; }
    // NCPCommand interface
    int type() const override { return Type::Line; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;
protected:
    QList<int>  m_axisList;
    int         m_pointNo = 0;
};

#endif // NCPCOMMANDLINE_H
