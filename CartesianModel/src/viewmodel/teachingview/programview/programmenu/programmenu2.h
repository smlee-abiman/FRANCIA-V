#ifndef PROGRAMMENU2_H
#define PROGRAMMENU2_H

#include "viewmodelbase.h"
#include "axisinfo.h"
#include "src/viewmodel/teachingview/pointinfo.h"
#include "src/viewmodel/teachingview/pagemodelbase.h"
#include "src/viewmodel/teachingview/teachingviewdef.h"
#include "programmenu.h"

#define POINT_NUMBER_INDEX_ADJUST_VALUE 121

class AxisPointInfo;

class ProgramMenu2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> subMenuList READ subMenuList CONSTANT)
public:
    explicit ProgramMenu2(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile, AxisInfo *axisInfo);
    ~ProgramMenu2() override;

    void setNcpFile(NCPFile *ncpFile);
    QList<QObject *> subMenuList() const { return m_subMenuList; }
    ProgramMenu::CreateMethod commandCreateMethod() override;
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;
    int insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;
    int replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;
    ProgramMenu *getProgMenu(int menuIndex)  { return static_cast<ProgramMenu *>(m_subMenuList.at(menuIndex)); }

private:
    int getMenuIndex(NCPCommand *command);
    int getMoveMenuIndex(NCPCommand *command);

signals:

public slots:
    void onNcpCommandDataChanged() override;

private:
    AxisPointInfo       *m_axisPointInfo = nullptr;
    QList<QObject *>    m_subMenuList;
    AxisInfo            *m_axisInfo;
};

#endif // PROGRAMMENU2_H
