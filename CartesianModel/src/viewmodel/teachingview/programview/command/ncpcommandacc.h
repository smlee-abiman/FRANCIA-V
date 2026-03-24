#ifndef NCPCOMMANDACC_H
#define NCPCOMMANDACC_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandAcc.
//------------------------------------------------------
class NCPCommandAcc : public NCPCommand
{
    Q_OBJECT
    const int ACCELERATION_MAX = 26;
public:
    class Data {
    public:
        Data() {}
        Data(int axis, int acceleration) {
            m_axis = axis;
            m_acceleration = acceleration;
        }
        int axis() const { return  m_axis; }
        void setAcceleration(int acceleration) { m_acceleration = acceleration; }
        int acceleration() const { return  m_acceleration; }
    private:
        int    m_axis = 0;
        int    m_acceleration = 0;
    };

    explicit NCPCommandAcc(QObject *parent = nullptr);
    explicit NCPCommandAcc(ResourceManager *resource, QObject *parent = nullptr);

    void setAcceleration(int axis, int acceleration);
    void setOn(bool on);
    bool on() const { return m_on; }
    QList<NCPCommandAcc::Data> accelerationList() const { return m_accelerationList; }
    // NCPCommand interface
    int type() const override { return Type::Acc; }
    int wizardMenuIndex() const override { return WizardMenuIndex::Menu2; }
    bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;
private:
    void setOn(const QString &str);

private:
    bool                        m_on = false;
    QList<NCPCommandAcc::Data>  m_accelerationList;
};

#endif // NCPCOMMANDACC_H
