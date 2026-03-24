#ifndef PROGRAMMENU2_4_5_H
#define PROGRAMMENU2_4_5_H

#include "programmenu.h"

class ProgramMenu2_4_5 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<bool> selectList MEMBER m_selectList NOTIFY selectListChanged)
public:
    explicit ProgramMenu2_4_5(ResourceManager *resource,CommunicationThread *comm);
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;
    void clearSelect();

private:
    void onDeactivate() override;

signals:
    void selectListChanged();

public slots:

private:
    QList<bool>    m_selectList;
};

#endif // PROGRAMMENU2_4_5_H
