#ifndef PROGRAMMENU1_4_H
#define PROGRAMMENU1_4_H

#include "programmenu.h"
#include "programmenu1_4item.h"

class ProgramMenu1_4 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList MEMBER m_itemList NOTIFY itemListChanged)
    Q_PROPERTY(int editIndex MEMBER m_editIndex NOTIFY editIndexChanged)

public:
    explicit ProgramMenu1_4(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu1_4() override;
    //函数
    Q_INVOKABLE void setCondition(int index, const QString &mark, const QString &msg, int value);
    Q_INVOKABLE void clearSelect();
    Q_INVOKABLE void selectItem(int index);
    Q_INVOKABLE void deselectItem(int index);
    Q_INVOKABLE void setVariableOk(int editIndex);
    NCPCommand *createNcpCommand() override;
    void show(NCPCommand *command) override;
    int editIndex() const { return m_editIndex; }
private:
    void init();
    void onActivate() override;
    void onDeactivate() override;
    ProgramMenu1_4Item *itemAt(int index);
signals:
    void itemListChanged();
    void editIndexChanged();
public slots:
    void setEditIndex(int index){
        if(m_editIndex == index)
            return;
        m_editIndex = index;
        emit editIndexChanged();
    }
private:
    QList<QObject *>    m_itemList;
    int                 m_editIndex = -1;//输入编辑索引
};

#endif // PROGRAMMENU1_4_H
