#ifndef PROGRAMMENU2_4_7_H
#define PROGRAMMENU2_4_7_H

#include "programmenu.h"

class ProgramMenu2_4_7 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(bool checked MEMBER m_checked NOTIFY checkedChanged)
public:
    explicit ProgramMenu2_4_7(ResourceManager *resource,CommunicationThread *comm);
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;

private:
    void onDeactivate() override;

signals:
    void checkedChanged();

public slots:
private:
    bool    m_checked = false;
};

#endif // PROGRAMMENU2_4_7_H
