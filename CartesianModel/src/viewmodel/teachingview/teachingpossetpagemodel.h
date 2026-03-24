#ifndef TEACHINGPOSSETPAGEMODEL_H
#define TEACHINGPOSSETPAGEMODEL_H

#include <QObject>
#include <QHash>
#include "pagemodelbase.h"
#include "axisinfo.h"

#define POS_SET_MODE_NUM_MAX    64
#define POS_SET_POINT_NUMBER_INDEX_ADJUST_VALUE 121

const QList<QList<quint16>> POS_SET_ADDRESS_LIST = {
    // position(SR)    servo(DI)   servo(DO)
    {     2769,           2687,     1783  },     // Axis-1
    {     2771,           2686,     1782  },     // Axis-2
    {     2773,           2685,     1781  },     // Axis-3
    {     2775,           2684,     1780  },     // Axis-4
    {     2777,           2683,     1779  },     // Axis-5
    {     2779,           2682,     1778  },     // Axis-6
    {     2781,           2681,     1777  },     // Axis-7
    {     2783,           2680,     1776  },     // Axis-8
};

enum PosSetAddressType{
    position,
    servodi,
    servodo,
};

class TeachingModePageModel;
class AxisPointInfo;

class TeachingPosSetPageModel : public PageModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> pointList READ pointList CONSTANT)
    Q_PROPERTY(bool viewpoint READ viewpoint WRITE setViewpoint NOTIFY viewpointChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
    Q_PROPERTY(int numPoint READ numPoint NOTIFY numPointChanged)
	
public:
    explicit TeachingPosSetPageModel(ResourceManager *resource, CommunicationThread * comm, TeachingModePageModel* modePageModel,CommonViewModel *commonView);
    explicit TeachingPosSetPageModel(QObject *object = nullptr);
    ~TeachingPosSetPageModel() override;

    Q_INVOKABLE void moveSelectdPoint(int point, bool isRelease = false);
    Q_INVOKABLE void viewPointControl(bool reverse);
    Q_INVOKABLE int setPosition(int pointIndex, int axis, int position, bool fromPosSet = true);
    Q_INVOKABLE int setSpeed(int pointIndex, int axis, int speed, bool fromPosSet = true);
    Q_INVOKABLE QString formatCurrentPosition(int axis, int value);
    Q_INVOKABLE QString formatPosition(int axis, int value);
    Q_INVOKABLE QString formatPosAdujustSpeed(int axis, int value);
    Q_INVOKABLE QString formatPosition(int pointIndex, int axis, int value);
    Q_INVOKABLE QString formatPosAdujustSpeed(int pointIndex, int axis, int value);

    Q_INVOKABLE int getPointNo(int pageNum, int index);
    Q_INVOKABLE int getPointSettable(int pointIndex);
    Q_INVOKABLE void startPageChanging();
    Q_INVOKABLE void fileSwitchUpdatePoint();//輝俳算亀宛扮揖化point

    QList<QObject *> pointList() const { return m_pointList; }
    void setViewpoint(bool reverse);
    bool viewpoint() const { return m_viewPoint; }
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }
    int numPoint() { return m_numPoint; }

protected:
    void onActivate() override;
    void onDeactivate() override;

signals:
    void viewpointChanged(bool reverse);
    void dataLoadedChanged(bool dataLoaded);
    void numPointChanged(int numpoint);
public slots:
    void onLanguageChanged(int language);
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void endModbusPageChange(int mainScreenId, int subScreenId);

private:
    void init();
    void readDataFormatCsv();
    void readPenMsgData();
    void readPenSetData();
    void readPointAddress();
    void readPointStyle();
    void modbusGetMaxPosition();
    int updatePointData(int point);
    void updatePointData(int startPointNo, int endPointNo);
    void updateMaxPosition();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void readPointDispOrder();
    void readPointSettableAxisTable();

private:
    TeachingModePageModel       *m_modePageModel;
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();
    AxisPointInfo               *m_axisPointInfo = nullptr;
    CommonViewModel             *m_commonView;
    QList<QObject *>            m_pointList;
    int                         m_numPoint = 0;
    QHash<int, int>             m_startAddressPointConversionMap;
    QList<QList<int>>           m_pointDispOrderSetting;
    QList<QList<int>>           m_pointSettableAxisTable[POS_SET_MODE_NUM_MAX];
    bool                        m_viewPoint = false;    // true: reverse / false: normal
    bool                        m_pageChanging = false;
    bool                        m_dataLoaded = false;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2779, 2781, 2783};

};

#endif // TEACHINGPOSSETPAGEMODEL_H
