#ifndef PROGRAMMENU4_H
#define PROGRAMMENU4_H

#include "programmenu.h"

class ProgramMenu4 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> subMenuList READ subMenuList CONSTANT)
public:
    explicit ProgramMenu4(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4();

    // getter.
    QList<QObject *> subMenuList() const { return m_subMenuList; }

    // ProgramMenu interface
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;
    virtual ProgramMenu::CreateMethod commandCreateMethod() override;
    virtual int insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;
    virtual int replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;

public slots:
    void onNcpCommandDataChanged() override;

private:
    QList<QObject*> m_subMenuList;
};



#endif // PROGRAMMENU4_H
