#ifndef PROGRAMMENU2_4_H
#define PROGRAMMENU2_4_H

#include "programmenu.h"

class ProgramMenu2_4 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> subMenuList MEMBER m_subMenuList CONSTANT)
public:
    explicit ProgramMenu2_4(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile);
    ~ProgramMenu2_4() override;
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;
    void setNcpFile(NCPFile *ncpFile);

private:
    int getMenuIndex(NCPCommand *command);

public slots:
    void onNcpCommandDataChanged() override;

private:
    QList<QObject *>    m_subMenuList;
};

#endif // PROGRAMMENU2_4_H
