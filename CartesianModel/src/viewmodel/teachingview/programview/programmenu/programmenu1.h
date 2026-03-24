#ifndef PROGRAMMENU1_H
#define PROGRAMMENU1_H

#include "viewmodelbase.h"
#include "programmenu.h"

class ProgramMenu1_3;

class ProgramMenu1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> subMenuList READ subMenuList CONSTANT)

public:
    explicit ProgramMenu1(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu1() override;
    void initData() override;
    NCPCommand *createNcpCommand() override;
    ProgramMenu::CreateMethod commandCreateMethod() override;
    void show(NCPCommand *command) override;
    int insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;
    int replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;

    //接口
    QList<QObject *> subMenuList() const { return m_subMenuList; }

private:
    int getMenuIndex(NCPCommand *command);
    int getIndex(int programNo);

public slots:
    void onNcpCommandDataChanged() override;

private:
    QList<QObject *>    m_subMenuList;
};

#endif // PROGRAMMENU1_H
