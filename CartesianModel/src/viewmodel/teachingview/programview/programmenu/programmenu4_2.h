#ifndef PROGRAMMENU4_2_H
#define PROGRAMMENU4_2_H

#include "programmenu.h"

//------------------------------------------------------
// ProgramMenu4_2
//------------------------------------------------------
class ProgramMenu4_2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> subMenuList READ subMenuList CONSTANT)

public:
    explicit ProgramMenu4_2(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_2() override;

    // ProgramMenu interface.
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;

    QList<QObject*> subMenuList() const { return m_subMenuList; }

public slots:
    void onNcpCommandDataChanged() override;

private:
    QList<QObject*>    m_subMenuList;
};

#endif // PROGRAMMENU4_2_H
