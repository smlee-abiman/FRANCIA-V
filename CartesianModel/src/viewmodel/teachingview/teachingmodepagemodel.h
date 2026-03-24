#ifndef TEACHINGMODEPAGEMODEL_H
#define TEACHINGMODEPAGEMODEL_H

#include <QObject>
#include "pagemodelbase.h"
#include "teachingmodedata.h"
#include "commonviewmodel.h"

#define GET_MODE_HR_NUMBER(mode)    \
    ((mode >= 1 && mode <= 16)  ? 2087 : \
     (mode >= 17 && mode <= 32) ? 2088 : \
     (mode >= 33 && mode <= 48) ? 2089 : \
     (mode >= 49 && mode <= 64) ? 2090 : -1)

#define GET_MODE_SR_NUMBER(mode)    \
    ((mode >= 1 && mode <= 16)  ? 2809 : \
     (mode >= 17 && mode <= 32) ? 2810 : \
     (mode >= 33 && mode <= 48) ? 2811 : \
     (mode >= 49 && mode <= 64) ? 2812 : -1)

#define MAX_USER_LEVEL      5
#define INVALID_MODE_NUM_BY_USER_LEVEL  255

class TeachingModePageModel : public PageModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> modeDataList READ modeDataList WRITE setModeDataList NOTIFY modeDataListChanged)
    Q_PROPERTY(QList<QObject *> currentModeList READ currentModeList WRITE setCurrentModeList NOTIFY currentModeListChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
    Q_PROPERTY(int mode64State READ mode64State WRITE setMode64State NOTIFY mode64StateChanged)
    Q_PROPERTY(int mode31State READ mode31State WRITE setMode31State NOTIFY mode31StateChanged)

public:
    // MODIFIED: number of items in the mode page list By IJ.YI 25.07.10
    const int MODE_PAGE_MAX = 11; //8->11
    const int MODE_NUM_PER_PAGE = 6; // 8-> 6
    const int MODE_NUM = MODE_PAGE_MAX * MODE_NUM_PER_PAGE;
    const int MODE_NUM_MAX = 66; //64->66
    const int modePenSetIndexByUserLevel[MAX_USER_LEVEL] = {72, 81, 90, 99, 108};

    explicit TeachingModePageModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    explicit TeachingModePageModel(QObject *object = nullptr);
    ~TeachingModePageModel() override;

    Q_INVOKABLE int getKeyNo(int modeDataListIndex);
    Q_INVOKABLE int modeNumPerPage() { return MODE_NUM_PER_PAGE; }
    Q_INVOKABLE void modbusUpdateModeStatus(bool modeStatus, int modeNo);
    Q_INVOKABLE void startPageChanging();
    // ADD :  support animation behavior By.IJ.YI 25.07.29
    Q_INVOKABLE int getFrameCount(int keyNo, const QString &dirName);

    void setModeDataList(const QList<QObject *> &list);
    void setCurrentModeList(const QList<QObject *> &list);
    void setCurrentPage(int page);
    QList<QObject *> modeDataList() const { return m_modeDataList; }
    QList<QObject *> currentModeList() const { return m_currentModeList; }
    int currentPage() const { return m_currentPage; }
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }
    int mode64State() {return m_mode64State; }
    int mode31State() {return m_mode31State; }

protected:
    void onActivate() override;
    void onDeactivate() override;

signals:
    void currentPageChanged(int page);
    void currentModeListChanged(const QList<QObject *>& list);
    void modeDataListChanged(const QList<QObject *>& list);
    void currentModeChanged(int mode);
    void dataLoadedChanged(bool dataLoaded);
    void mode64StateChanged(int state);
    void mode31StateChanged(int state);

public slots:
    void onFinished();
    void onLanguageChanged(int language);
    void onUserLevelChanged(int userLevel);
    void endModbusPageChange(int mainScreenId, int subScreenId);
    void setMode64State(int state);
    void setMode31State(int state);

private:
    void init();
    void initList();
    void readDataFormatCsv();
    void readPenMsgData();
    void readPenSetData();
    void updateCurrentModeList();
    void setTeachingModeStatus(int bitIndex, int status);
    void updatePenMsgModeData();
    void getModeFormatByUserLevel();

private:
    CommonViewModel*            m_commonView;
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();
    QList<QObject *>            m_currentModeList;
    QList<QObject *>            m_modeDataList;
    QHash<int, int>             m_diModeIndexConversionMap;
    int                         m_currentPage = 0;
    QList<QList<quint16>>       m_userLevelModeDisp[MAX_USER_LEVEL];
    int                         mUserLevel = 0;
    int                         mCurModeCnt[MAX_USER_LEVEL];
    bool                        m_pageChanging = false;
    bool                        m_dataLoaded = false;
    int                         m_mode64State = false;
    int                         m_mode31State;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2813, 2815, 2817};
};

#endif // TEACHINGMODEPAGEMODEL_H
