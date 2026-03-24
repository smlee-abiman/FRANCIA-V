#ifndef PROGRAMMENU4_2_2_H
#define PROGRAMMENU4_2_2_H

#include "programmenu.h"
#include "programmenu4_1item.h"
#include "../command/ncpcommandgoto.h"
#include "../command/ncpcommandifgoto.h"

typedef ProgramMenu4_1Item ProgramMenu4_2_2Item;

//------------------------------------------------------
// ProgramMenu4_2_2
//------------------------------------------------------
class ProgramMenu4_2_2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList READ itemList NOTIFY itemListChanged)
    Q_PROPERTY(int editIndex MEMBER m_editIndex NOTIFY editIndexChanged)
    Q_PROPERTY(int labelNo READ labelNo NOTIFY labelNoChanged)
    Q_PROPERTY(bool openLabelSelectionDialog MEMBER m_openLabelSelectionDialog NOTIFY openLabelSelectionDialogChanged)
public:
    explicit ProgramMenu4_2_2(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_2_2() override;

    // setter/getter.
    QList<QObject *> itemList() const { return m_itemList; }
    void setEditIndex(int editIndex) {
        if (m_editIndex == editIndex) { return; }
        m_editIndex = editIndex;
        emit editIndexChanged();
    }
    int labelNo() const { return m_labelNo; }
    Q_INVOKABLE void setLabelNo(int labelNo) {
        if (m_labelNo == labelNo) { return; }
        m_labelNo = labelNo;
        emit labelNoChanged(m_labelNo);
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
    void labelNoChanged(int);
    void openLabelSelectionDialogChanged(bool);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    void show(NCPCommandGoTo* command);
    void show(NCPCommandIfGoTo* command);
    ProgramMenu4_2_2Item* itemAt(int index);
    NCPCommand* createNcpCommandGoTo();
    NCPCommand* createNcpCommandIfGoTo();

private:
    QList<QObject *>    m_itemList;
    int                 m_editIndex = -1;//输入编辑索引
    int                 m_labelNo;
    bool                m_openLabelSelectionDialog;
};

#endif // PROGRAMMENU4_2_2_H
