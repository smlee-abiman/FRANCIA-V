#ifndef PROGRAMMENU4_2_3_H
#define PROGRAMMENU4_2_3_H

#include "programmenu.h"

//------------------------------------------------------
// ProgramMenu4_2_3
//------------------------------------------------------
class ProgramMenu4_2_3 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<int> programNoList MEMBER mProgramNoList NOTIFY programNoListChanged)
    Q_PROPERTY(int selectedIndex MEMBER mSelectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(bool openProgramSelectionDialog MEMBER mOpenProgramSelectionDialog NOTIFY openProgramSelectionDialogChanged)
public:
    explicit ProgramMenu4_2_3(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_2_3() override;

    // gettter/setter.
    void setSelectedIndex(int selectedIndex) {
        if (mSelectedIndex == selectedIndex) { return; }
        mSelectedIndex = selectedIndex;
        emit selectedIndexChanged(mSelectedIndex);
    }
    Q_INVOKABLE void setProgramNo(int index, int programNo) {
        if (index < 0 || mProgramNoList.size() <= index) { return; }
        mProgramNoList[index] = programNo;
        emit programNoListChanged(mProgramNoList);
    }

    // ProgramMenu interface.
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;

signals:
    void programNoListChanged(QList<int>);
    void selectedIndexChanged(int);
    void openProgramSelectionDialogChanged(bool);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    NCPCommand* createNcpOnCommand();
    NCPCommand* createNcpOffCommand();

private:
    QList<int> mProgramNoList;
    int mSelectedIndex;
    bool mOpenProgramSelectionDialog;
};

#endif // PROGRAMMENU4_2_3_H
