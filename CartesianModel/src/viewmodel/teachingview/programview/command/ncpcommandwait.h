#ifndef NCPCOMMANDWAIT_H
#define NCPCOMMANDWAIT_H

#include "../ncpcommand.h"
#include "modbusmanager.h"

const int PEN_MSG_PROGRAM_WIZARD_DELAY_TIME = 10;
const int PEN_MSG_PROGRAM_WIZARD_TIMER_ITEM = 22;
//------------------------------------------------------
// NCPCommandWait.
//------------------------------------------------------
class NCPCommandWait : public NCPCommand
{
    Q_OBJECT
    Q_PROPERTY(QList<int> symbolIndex READ symbolIndex CONSTANT)
    Q_PROPERTY(QList<QString> symbolTitle READ symbolTitle CONSTANT)
public:
    explicit NCPCommandWait(QObject *parent = nullptr);
    explicit NCPCommandWait(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandWait(ResourceManager *resource, QString timerValue, QObject *parent = nullptr);
    explicit NCPCommandWait(ResourceManager *resource, QString timerValue, int timerListIndex, int symbolListIndex,
        int category, QObject *parent = nullptr);
    QList<int> symbolIndex(){ return m_symbolIndex; }
    QList<QString> symbolTitle(){ return m_symbolTitle; }

public:
    // NCPCommand interface
    virtual int type() const override { return Type::Wait; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu3; }
    virtual bool parse(const QString &commandLine) override;
    virtual QString nameString() const override;
    virtual QString toString() const override;
    virtual QString toCode() const override;
    NCPCommand* clone() const override;
    int getCmdCategory() { return mCurCategory; }
    void getTimerAndSymbolIndex(QString &symbolTitle, QString *timerValue, int *timerIndex, int *symbolIndex);

private:
    QString         mTimerValue = "";
    int             mCurCategory = -1;
    int             mTimerListIndex = -1;
    int             mSymbolListIndex = -1;
    QList<int>      m_symbolIndex;
    QList<QString>  m_symbolTitle;
    ModbusManager   *m_modbus = ModbusManager::get_instance();
    void            initSymbolListPenSetData();
};

#endif // NCPCOMMANDWAIT_H
