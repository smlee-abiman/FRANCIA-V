#ifndef TEACHINGPASSPAGEMODEL_H
#define TEACHINGPASSPAGEMODEL_H

#include <QObject>
#include "pagemodelbase.h"

class AreaSetPointParser;
class AreaSetPointInfo;

class TeachingPassPageModel : public PageModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<int> timeList READ timeList WRITE setTimeList NOTIFY timeListChanged)
    Q_PROPERTY(QList<int> distanceList READ distanceList WRITE setDistanceList NOTIFY distanceListChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
public:
    explicit TeachingPassPageModel(ResourceManager *resource, CommunicationThread * comm);
    explicit TeachingPassPageModel(QObject *object = nullptr);
    ~TeachingPassPageModel() override;

    Q_INVOKABLE int setDistance(int index, int value);
    Q_INVOKABLE QString formatData(int point, int value);
    Q_INVOKABLE QString formatPointMaxValue(int index);
    Q_INVOKABLE QString formatPointMinValue(int index);
    Q_INVOKABLE void startPageChanging();

    void setTimeList(QList<int> &list);
    void setDistanceList(QList<int> &list);
    QList<int> timeList() const { return m_timeList; }
    QList<int> distanceList() const { return m_distanceList; }
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }

signals:
    void timeListChanged(QList<int> list);
    void distanceListChanged(QList<int> list);
    void dataLoadedChanged(bool dataLoaded);

public slots:
    void onLanguageChanged(int language);
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void endModbusPageChange(int mainScreenId, int subScreenId);

private:
    void init();
    void readPenSetData();
    void readDataFormat();
    void readMaxMinSetting();
    void startModbusOneShot();
    void stopModbusOneShot();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void onActivate() override;
    void onDeactivate() override;
    void modbusSaveDistance(quint16 hrAddress, int value);
    int modbusGetDistance(int index);
    void updateDistanceData(quint16 hrAddress);
    int roundOff(int value, int fraction);
    void modbusGetData(quint16 address);

private:
    AreaSetPointParser          *m_areaSetPointParser;
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();
    bool                        mIsModbusConnectOneShot = false;
    QList<int>                  m_timeList;
    QList<int>                  m_distanceList;
    QList<int>                  m_dataFormatList;
    QList<AreaSetPointInfo *>   m_pointInfoList;
    bool                        m_pageChanging = false;
    bool                        m_dataLoaded = false;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2819, 2821, 2823};
};

#endif // TEACHINGPASSPAGEMODEL_H
