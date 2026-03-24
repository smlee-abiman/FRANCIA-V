#ifndef PROGRAMMENU5_H
#define PROGRAMMENU5_H

#include "programmenu.h"

class ProgramMenu5 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> subMenuList READ subMenuList CONSTANT)
    Q_PROPERTY(int Menu5_Index READ Menu5_Index WRITE setMenu5_Index NOTIFY Menu5_IndexChanged)

public:
    explicit ProgramMenu5(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu5();
    NCPCommand *createNcpCommand() override;
    virtual void show(NCPCommand *command) override;
    QList<QObject *> subMenuList() const { return m_subMenuList; }
    int Menu5_Index(){ return m_Menu5_Index; }

signals:
    void Menu5_IndexChanged(int Menu5_Index);

public slots:
    void setMenu5_Index(int Menu5_Index){
        if(m_Menu5_Index == Menu5_Index)
            return;
        m_Menu5_Index = Menu5_Index;
        emit Menu5_IndexChanged(Menu5_Index);
    }
    void onNcpCommandDataChanged() override;

private:
    QString m_menu5_1Command;//系统菜单生成的程序指令
    QString m_menu5_2Command;//内部输出菜单生成的程序指令
    int m_Menu5_Index = 0;
    QList<QObject *>    m_subMenuList;
};

#endif // PROGRAMMENU5_H
