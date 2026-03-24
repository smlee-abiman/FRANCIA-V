#ifndef PROGRAMMENU3_H
#define PROGRAMMENU3_H

#include "programmenu.h"

#define TIMER_MAX_NUM       100
#define TIMER_ONE_PAGE_NUM  8
#define TIMER_PAGE_MAX_NUM  13
#define GET_TIMER_MAX_PAGE(onePage, maxPage) \
    ((maxPage % onePage == 0) ? maxPage / onePage : (onePage / onePage) + 1)

class ProgramMenu3 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> subMenuList READ subMenuList CONSTANT)
    Q_PROPERTY(int Menu3_Index READ Menu3_Index WRITE setMenu3_Index NOTIFY Menu3_IndexChanged)

public:
    explicit ProgramMenu3(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu3();

    void menu_init();
    NCPCommand *createNcpCommand() override;
    virtual void show(NCPCommand *command) override;
    int updateModbusData() override;
    QList<QObject *> subMenuList() const { return m_subMenuList; }
    int Menu3_Index(){ return m_Menu3_Index; }

signals:
    void Menu3_IndexChanged(int Menu3_Index);

public slots:
    void setMenu3_Index(int Menu3_Index){
        if(m_Menu3_Index == Menu3_Index)
            return;
        m_Menu3_Index = Menu3_Index;
        emit Menu3_IndexChanged(Menu3_Index);
    }
    void onNcpCommandDataChanged() override;

private:
    QList<QObject *>            m_subMenuList;
    int                         m_Menu3_Index = 0;
    QString                     m_menu3_1Command;//延时菜单生成的程序指令
    QString                     m_menu3_2Command;//计时器菜单生成的程序指令
};

#endif // PROGRAMMENU3_H
