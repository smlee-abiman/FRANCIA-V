#ifndef NCPCOMMANDPATH_H
#define NCPCOMMANDPATH_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandPath.
//------------------------------------------------------
class NCPCommandPath : public NCPCommand
{
    Q_OBJECT
public:
    class Data {
    public:
        Data() {}
        Data(int axis, int position) {
            m_axis = axis;
            m_position = position;
        }
        int axis() const { return  m_axis; }
        void setPosition(int position) { m_position = position; }
        int position() const { return  m_position; }
    private:
        int    m_axis = 0;
        int    m_position = 0;
    };
    explicit NCPCommandPath(QObject *parent = nullptr);
    explicit NCPCommandPath(ResourceManager *resource, QObject *parent = nullptr);
    void setPosition(int axis, int position);
    void setOn(bool on);
    bool on() const { return m_on; }
    QList<NCPCommandPath::Data> positionList() const { return m_positionList; }

    // NCPCommand interface
    int type() const override { return Type::Path; }
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
    QList<NCPCommandPath::Data> m_positionList;
};

#endif // NCPCOMMANDPATH_H
