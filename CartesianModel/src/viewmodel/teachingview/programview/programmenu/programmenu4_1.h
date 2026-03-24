#ifndef PROGRAMMENU4_1_H
#define PROGRAMMENU4_1_H

#include "programmenu.h"
#include "programmenu4_1item.h"

//------------------------------------------------------
// ProgramMenu4_1.
//------------------------------------------------------
class ProgramMenu4_1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(int overTime READ overTime NOTIFY overTimeChanged)
    Q_PROPERTY(QList<QObject *> itemList READ itemList NOTIFY itemListChanged)
    Q_PROPERTY(int editIndex MEMBER m_editIndex NOTIFY editIndexChanged)
public:
    explicit ProgramMenu4_1(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_1() override;

    // setter/getter.
    int overTime() const { return mOverTime; }
    void setOverTime(int overTime) {
        if (mOverTime == overTime) { return; }
        mOverTime = overTime;
        emit overTimeChanged(mOverTime);
    }
    QList<QObject *> itemList() const { return m_itemList; }
    void setEditIndex(int editIndex) {
        if (m_editIndex == editIndex) { return; }
        m_editIndex = editIndex;
        emit editIndexChanged();
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
    void overTimeChanged(int);
    void itemListChanged();
    void editIndexChanged();

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command,CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    void modbusGetOverTime();
    void updateOverTime(const stModBusCommunicationData &command);
    ProgramMenu4_1Item* itemAt(int index);

private:
    int                 mOverTime;
    QList<QObject *>    m_itemList;
    int                 m_editIndex = -1;//输入编辑索引
};

#endif // PROGRAMMENU4_1_H
