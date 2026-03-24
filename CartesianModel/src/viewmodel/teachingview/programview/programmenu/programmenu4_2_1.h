#ifndef PROGRAMMENU4_2_1_H
#define PROGRAMMENU4_2_1_H

#include "programmenu.h"
#include "programmenu4_1item.h"
#include "../command/ncpcommandgosub.h"
#include "../command/ncpcommandifgosub.h"

typedef ProgramMenu4_1Item ProgramMenu4_2_1Item;

//------------------------------------------------------
// ProgramMenu4_2_1
//------------------------------------------------------
class ProgramMenu4_2_1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList READ itemList NOTIFY itemListChanged)
    Q_PROPERTY(int editIndex MEMBER m_editIndex NOTIFY editIndexChanged)
    Q_PROPERTY(int programNo READ programNo NOTIFY programNoChanged)
    Q_PROPERTY(bool openProgramSelectionDialog MEMBER m_openProgramSelectionDialog NOTIFY openProgramSelectionDialogChanged)
public:
    explicit ProgramMenu4_2_1(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_2_1() override;

    // setter/getter.
    QList<QObject *> itemList() const { return m_itemList; }
    void setEditIndex(int editIndex) {
        if (m_editIndex == editIndex) { return; }
        m_editIndex = editIndex;
        emit editIndexChanged();
    }
    int programNo() const { return m_programNo; }
    Q_INVOKABLE void setProgramNo(int programNo) {
        if (m_programNo == programNo) { return; }
        m_programNo = programNo;
        emit programNoChanged(m_programNo);
    }

    // ProgramMenu interface.
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;

    Q_INVOKABLE void clearItems();
    Q_INVOKABLE void selectItem(int index);
    Q_INVOKABLE void deselectItem(int index);
    Q_INVOKABLE void setCondition(int index, const QString &symbol, const QString &opeStr, int value, int ioAddress);
    Q_INVOKABLE void setVariableOk(int index);

signals:
    void itemListChanged();
    void editIndexChanged();
    void programNoChanged(int);
    void openProgramSelectionDialogChanged(bool);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    void show(NCPCommandGoSub* command);
    void show(NCPCommandIfGoSub* command);
    ProgramMenu4_2_1Item* itemAt(int index);
    NCPCommand* createNcpCommandGoSub();
    NCPCommand* createNcpCommandIfGoSub();

private:
    QList<QObject *>    m_itemList;
    int                 m_editIndex = -1;//输入编辑索引
    int                 m_programNo;
    bool                m_openProgramSelectionDialog;
};

#endif // PROGRAMMENU4_2_1_H
