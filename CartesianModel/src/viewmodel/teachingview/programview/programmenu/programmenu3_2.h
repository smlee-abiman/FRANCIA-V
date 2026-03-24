#ifndef PROGRAMMENU3_2_H
#define PROGRAMMENU3_2_H

#include "programmenu.h"

#define PROG_MENU3_2_TIMER_PAGE_MAX         13
#define PROG_MENU3_2_ONE_PAGE_ITEM_MAX      8
#define PROG_MENU3_2_TIMER_ITEM_VALUE_MAX   100

const QList<quint16> TIMER_TOP_ITEM_ADDR_LIST = {
    //1-8   //9-16  //17-24 //25-32 //33-40 //41-48 //49-56 //57-64 //65-72 //73-80 //81-88 //89-96 //97-100
    2125,   2133,   2141,   2149,   2157,   2165,   2173,   2181,   2189,   2197,   2205,   2213,   2221        // HRアドレス
};

const QList<quint16> TIMER_ITEM_ADDR_DATA_SIZE_LIST = {
    //1-8   //9-16  //17-24 //25-32 //33-40 //41-48 //49-56 //57-64 //65-72 //73-80 //81-88 //89-96 //97-100
    8,      8,      8,      8,      8,      8,      8,      8,      8,      8,      8,     8,       4
};

class ProgramMenu3_2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<int> timerAddr READ timerAddr CONSTANT)
    Q_PROPERTY(QList<int> timerDataList READ timerDataList WRITE setTimerDataList NOTIFY timerDataListChanged)
    Q_PROPERTY(int curPageTimerDataList READ curPageTimerDataList WRITE setCurPageTimerDataList NOTIFY curPageTimerDataListChanged)
    Q_PROPERTY(int selectIndexTimerDataList READ selectIndexTimerDataList WRITE setSelectIndexTimerDataList NOTIFY selectIndexTimerDataListChanged)
    Q_PROPERTY(int targetCoopPage READ targetCoopPage WRITE setTargetCoopPage NOTIFY targetCoopPageChanged)
    Q_PROPERTY(int targetCoopSelectIndex READ targetCoopSelectIndex WRITE setTargetCoopSelectIndex NOTIFY targetCoopSelectIndexChanged)
    Q_PROPERTY(bool isLinkedStartup READ isLinkedStartup WRITE setIsLinkedStartup NOTIFY isLinkedStartupChanged)
    Q_PROPERTY(bool isHoldingTimerValue READ isHoldingTimerValue WRITE setIsHoldingTimerValue NOTIFY isHoldingTimerValueChanged)
    Q_PROPERTY(QString holdingTimerValue READ holdingTimerValue WRITE setHoldingTimerValue NOTIFY holdingTimerValueChanged)
    Q_PROPERTY(int holdingTimerValueIndex READ holdingTimerValueIndex WRITE setHoldingTimerValueIndex NOTIFY holdingTimerValueIndexChanged)
    Q_PROPERTY(bool isReleaseHoldTimerValue READ isReleaseHoldTimerValue WRITE setIsReleaseHoldTimerValue NOTIFY isReleaseHoldTimerValueChanged)
public:
    explicit ProgramMenu3_2(ResourceManager *resource,CommunicationThread *comm);

    Q_INVOKABLE int gettimerNum(){ return m_timerAddr.length();}
    Q_INVOKABLE void modbusGetValues(int pageNum);
    QList<int> timerAddr(){ return m_timerAddr; }
    void setTimerDataList(QList<int> timerDataList);
    QList<int> timerDataList() const { return m_timerDataList; }
    NCPCommand *createNcpCommand() override;
    virtual void show(NCPCommand *command) override;
    int updateModbusData() override;
    Q_INVOKABLE void setTimerValue(int page, int index, QString timerValue);
    Q_INVOKABLE void keepTimerValue(int page, int index, QString timerValue);
    Q_INVOKABLE void setCurSelectInfo(int page, int selectIndex);
    void setCurPageTimerDataList(int curPageTimerDataList);
    int curPageTimerDataList() const { return m_curPageTimerDataList; }
    void setSelectIndexTimerDataList(int selectIndexTimerDataList);
    int selectIndexTimerDataList() const { return m_selectIndexTimerDataList; }
    void setTargetCoopPage(int targetCoopPage);
    int targetCoopPage() const { return m_targetCoopPage; }
    void setTargetCoopSelectIndex(int targetCoopSelectIndex);
    int targetCoopSelectIndex() const { return m_targetCoopSelectIndex; }
    void setIsLinkedStartup(bool isLinkedStartup);
    int isLinkedStartup() const { return m_isLinkedStartup; }
    Q_INVOKABLE void clearTimerValueKeepStatus(int page);
    void setIsHoldingTimerValue(bool isHoldingTimerValue);
    int isHoldingTimerValue() const { return m_isHoldingTimerValue; }
    void setHoldingTimerValue(QString holdingTimerValue);
    QString holdingTimerValue() const { return m_holdingTimerValue; }
    void setHoldingTimerValueIndex(int holdingTimerValueIndex);
    int holdingTimerValueIndex() const { return m_holdingTimerValueIndex; }
    void setIsReleaseHoldTimerValue(bool isReleaseHoldTimerValue);
    int isReleaseHoldTimerValue() const { return m_isReleaseHoldTimerValue; }

private:
    QList<int>      m_timerAddr;
    QList<int>      m_timerDataList;
    int             mCurSelectIndex = 0;
    QString         m_curTimerItemValue = "";
    int             m_curTimerListIndex = 0;
    int             m_curSymbolAtIndex = 0;
    QList<bool>     mTimerValueKeepStatus;
    QList<QString>  mTimerValues;
    int             m_curPageTimerDataList = 1;
    int             m_selectIndexTimerDataList = 0;
    int             m_targetCoopPage = 0;
    int             m_targetCoopSelectIndex = 0;
    bool            m_isLinkedStartup = false;
    bool            m_isHoldingTimerValue = false;
    QString         m_holdingTimerValue = "";
    int             m_holdingTimerValueIndex = -1;
    bool            m_isReleaseHoldTimerValue = false;

    void initSettingPenSetData();
    void initTimerList();
    void initTimerMenuList();
    void initTimerValueList();
    void startModbusMonitoring();
    void stopModbusMonitoring();
    void modbusReadTimerValues();
    void modbusReadTimerItemValues();

protected:
    void onActivate() override;
    void onDeactivate() override;

public slots:
    void updateProgramMenu(stModBusCommunicationData command);

signals:
    void timerDataListChanged(QList<int> timerDataList);
    void curPageTimerDataListChanged(int curPageTimerDataList);
    void selectIndexTimerDataListChanged(int selectIndexTimerDataList);
    void targetCoopPageChanged(int targetCoopPage);
    void targetCoopSelectIndexChanged(int targetCoopSelectIndex);
    void isLinkedStartupChanged(bool isLinkedStartup);
    void isHoldingTimerValueChanged(bool isHoldingTimerValue);
    void holdingTimerValueChanged(QString holdingTimerValue);
    void holdingTimerValueIndexChanged(int holdingTimerValueIndex);
    void isReleaseHoldTimerValueChanged(bool isReleaseHoldTimerValue);
};

#endif // PROGRAMMENU3_2_H
