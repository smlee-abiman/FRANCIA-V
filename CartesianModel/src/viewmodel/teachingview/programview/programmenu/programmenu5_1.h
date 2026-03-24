#ifndef PROGRAMMENU5_1_H
#define PROGRAMMENU5_1_H

#include "programmenu.h"

class ProgramMenu5_1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(int curSelectIndex READ curSelectIndex WRITE setCurSelectIndex NOTIFY curSelectIndexChanged)
    Q_PROPERTY(int curAlarmSelectIndex READ curAlarmSelectIndex WRITE setCurAlarmSelectIndex NOTIFY curAlarmSelectIndexChanged)
    Q_PROPERTY(int itemSelect READ itemSelect WRITE setItemSelect NOTIFY itemSelectChanged)
public:
    enum SystemItem {
        SystemItemApplyStop   = 0,
        SystemItemCancelStop,
        SystemItemProgEnd,
        SystemItemAlarm,
        SystemItemMax,
    };
    explicit ProgramMenu5_1(ResourceManager *resource,CommunicationThread *comm);
    NCPCommand *createNcpCommand() override;
    virtual void show(NCPCommand *command) override;
    void setCurSelectIndex(int curSelectIndex);
    int curSelectIndex() const { return m_curSelectIndex; }
    void setCurAlarmSelectIndex(int curAlarmSelectIndex);
    int curAlarmSelectIndex() const { return m_curAlarmSelectIndex; }
    void setItemSelect(int itemSelect);
    int itemSelect() const { return m_itemSelect; }

    int m_curSelectIndex = -1;
    int m_curAlarmSelectIndex = -1;
    int m_itemSelect = 0;

protected:
    void onActivate() override;
    void onDeactivate() override;

signals:
    void curSelectIndexChanged(int curSelectIndex);
    void curAlarmSelectIndexChanged(int curAlarmSelectIndex);
    void itemSelectChanged(int itemSelect);
};

#endif // PROGRAMMENU5_1_H
