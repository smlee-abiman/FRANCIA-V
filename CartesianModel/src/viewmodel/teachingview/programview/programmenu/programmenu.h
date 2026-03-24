#ifndef PROGRAMMENU_H
#define PROGRAMMENU_H

#include "viewmodelbase.h"
#include "../ncpcommand.h"

class NCPFile;
class NCPProgram;

class ProgramMenu : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int menuIndex MEMBER m_menuIndex NOTIFY menuIndexChanged)
public:
    typedef enum {
        SimpleNcpCommand,
        ComplexNcpCommand
    } CreateMethod;
    explicit ProgramMenu(ResourceManager *resource,CommunicationThread *comm);
    virtual void initData();
    virtual void show(NCPCommand *command);
    virtual int updateModbusData();
    virtual NCPCommand *createNcpCommand();
    virtual int insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex);
    virtual int replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex);
    virtual ProgramMenu::CreateMethod commandCreateMethod();
    void setMenuIndex(int index);
    int menuIndex() const { return m_menuIndex; }
signals:
    void menuIndexChanged();
    void ncpCommandDataChanged();
public slots:
    virtual void onNcpCommandDataChanged();
protected:
    int             m_menuIndex = 0;
};

#endif // PROGRAMMENU_H
