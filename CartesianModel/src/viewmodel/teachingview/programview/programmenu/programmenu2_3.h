#ifndef PROGRAMMENU2_3_H
#define PROGRAMMENU2_3_H

#include "programmenu.h"

class ProgramMenu2_3 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(bool isFreeStack MEMBER m_isFreeStack NOTIFY isFreeStackChanged)//标准堆叠每个堆叠的方式
    Q_PROPERTY(QList<int> standardStackTypeList MEMBER m_standardStackTypeList NOTIFY standardStackTypeListChanged)//标准堆叠每个堆叠的方式

public:
    explicit ProgramMenu2_3(ResourceManager *resource,CommunicationThread *comm);
    NCPCommand *createNcpCommand() override;
    void show(NCPCommand *command) override;

    QList<int>  standardStackTypeList(){return m_standardStackTypeList;}
    int getCurrentStackType(int stack){return m_standardStackTypeList.value(stack);}

private:
    void onDeactivate() override;

signals:
    void isFreeStackChanged(bool isfreestack);
    void standardStackTypeListChanged(QList<int> type);

private:
    static const QList<int>     m_programNoList;
    bool                        m_isFreeStack = false;//true:is free stack; false:is standard stack;
    QList<int>                  m_standardStackTypeList = {0,0,0,0,0,0,0,0};

};

#endif // PROGRAMMENU2_3_H
