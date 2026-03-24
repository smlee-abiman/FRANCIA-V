#ifndef NCPCOMMANDMOVE_H
#define NCPCOMMANDMOVE_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandMove.
//------------------------------------------------------
class NCPCommandMove : public NCPCommand
{
    Q_OBJECT
public:
    typedef enum {
        Arbitrary,
        Fixed,
        Palletize,
        Unknown,
    } PointType;

    explicit NCPCommandMove(QObject *parent = nullptr);
    explicit NCPCommandMove(ResourceManager *resource, QObject *parent = nullptr);

    void appendAxis(int axis);
    void setRelative(bool relative);
    bool relative() const { return m_relative; }
    void setPointNo (int no) override;
    int pointNo() const override { return m_pointNo; }
    QList<int> axisList() const { return m_axisList; }
    virtual QString command() const { return "MOVE"; }
    QString getPointNoCode(int pointNo) const;
    int getPointNo(const QString &plt) const;

    // NCPCommand interface
    int type() const override { return Type::Move; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;

protected:
    QList<int>  m_axisList;
    bool        m_relative = false;
    int         m_pointNo = 0;
};

#endif // NCPCOMMANDMOVE_H
