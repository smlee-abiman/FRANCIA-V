#ifndef PROGRAMMENU3_1_H
#define PROGRAMMENU3_1_H

#include "programmenu.h"

class ProgramMenu3_1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QString curDelayTimeValue READ curDelayTimeValue WRITE setCurDelayTimeValue NOTIFY curDelayTimeValueChanged)
public:
    explicit ProgramMenu3_1(ResourceManager *resource,CommunicationThread *comm);
    NCPCommand *createNcpCommand() override;
    virtual void show(NCPCommand *command) override;
    void setCurDelayTimeValue(QString curDelayTimeValue);
    QString curDelayTimeValue() const { return m_curDelayTimeValue; }
private:
    QString m_curDelayTimeValue = "";

protected:
    void onActivate() override;
    void onDeactivate() override;

signals:
    void curDelayTimeValueChanged(QString timerDataList);
};

#endif // PROGRAMMENU3_1_H
