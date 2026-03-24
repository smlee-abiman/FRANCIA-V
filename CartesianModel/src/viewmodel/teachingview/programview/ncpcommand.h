#ifndef NCPCOMMAND_H
#define NCPCOMMAND_H

#include <QObject>
#include "resourcemanager.h"

class NCPCommand : public QObject
{
    Q_OBJECT
public:
    enum Type {
        Unknown = -1,
        Prog,
        CInc,
        CClr,
        Inc,
        Dec,
        On,
        Off,
        In,
        Wait,
        WaitPos,
        Pause,
        GoSub,
        GoTo,
        IfGoSub,
        IfGoTo,
        Ret,
        End,
        Move,
        Movep,
        Home,
        Homep,
        Path,
        Break,
        Line,
        Linec,
        Arc,
        Arcc,
        Out,
        Outp,
        Acc,
        Alm,
        // Next,    // not supported.
        // Set,     // not supported.
        // Jog,     // not supported.
        // Srv,     // not supported.
        // Shift,   // not supported.
        // Aspd,    // not supported.
    };
    enum WizardMenuIndex {
        UnknownMenu = -1,
        Menu1,
        Menu2,
        Menu3,
        Menu4,
        Menu5,
    };
public:
    explicit NCPCommand(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommand(QObject *parent = nullptr);
    virtual ~NCPCommand();

    // getter/setter.
    virtual int type() const = 0;
    virtual int wizardMenuIndex() const = 0;
    virtual int labelNo() { return -1; }
    virtual void setLabelNo(int labelNo) { Q_UNUSED(labelNo); }

    // method. commandLine ... includes command & parameter.
    virtual bool parse(const QString& commandLine) = 0;
    virtual QString nameString() const = 0;
    virtual QString toString() const = 0;
    virtual NCPCommand* clone() const = 0;
    Q_INVOKABLE virtual QString toCode() const = 0;

    virtual int pointNo() const { return -1;}//筛选任意点指令粘贴
    virtual void setPointNo(int no){ Q_UNUSED(no);}//筛选任意点指令粘贴

protected:
    ResourceManager *m_resource = nullptr;
};

#endif // NCPCOMMAND_H
