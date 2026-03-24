#ifndef PROGRAMMENU5_2_H
#define PROGRAMMENU5_2_H

#include "programmenu.h"
#include "programmenu5_2item.h"

class ProgramMenu5_2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList MEMBER m_itemList NOTIFY itemListChanged)
    Q_PROPERTY(int editIndex MEMBER m_editIndex NOTIFY editIndexChanged)
public:
    explicit ProgramMenu5_2(ResourceManager *resource,CommunicationThread *comm);
    Q_INVOKABLE void setCondition(int index, const QString &mark, const QString &opeStr, const QString &msg, int value);
    Q_INVOKABLE void clearSelect();
    Q_INVOKABLE void selectItem(int index);
    Q_INVOKABLE void deselectItem(int index);
    Q_INVOKABLE void setVariableOk(int editIndex);
    Q_INVOKABLE void clearItems();
    NCPCommand *createNcpCommand() override;
    virtual void show(NCPCommand *command) override;
    int editIndex() const { return m_editIndex; }
    void menu_init();

private:
    QList<QObject *>    m_itemList;
    int                 m_editIndex = -1;

    ProgramMenu5_2Item *itemAt(int index);

protected:
    void onActivate() override;
    void onDeactivate() override;

public slots:
    void setEditIndex(int index){
        if(m_editIndex == index)
            return;
        m_editIndex = index;
        emit editIndexChanged();
    }

signals:
    void itemListChanged();
    void editIndexChanged();
};

#endif // PROGRAMMENU5_2_H
