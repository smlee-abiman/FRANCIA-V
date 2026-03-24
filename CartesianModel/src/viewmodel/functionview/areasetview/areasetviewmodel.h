#ifndef AREASETVIEWMODEL_H
#define AREASETVIEWMODEL_H

#include "viewmodelbase.h"
#include "axisinfo.h"
#include "enums.h"
#include "../../commonviewmodel.h"

class AreaSetViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> dataList READ dataList WRITE setDataList NOTIFY dataListChanged)
    Q_PROPERTY(QList<bool> ioSignalList READ ioSignalList WRITE setIoSignalList NOTIFY ioSignalListChanged)
    Q_PROPERTY(int currentAreaType READ currentAreaType WRITE setCurrentAreaType NOTIFY currentAreaTypeChanged)
    Q_PROPERTY(int mxSxSpace READ mxSxSpace WRITE setMxSxSpace NOTIFY mxSxSpaceChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
public:
    explicit AreaSetViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    explicit AreaSetViewModel(QObject *object = nullptr);
    ~AreaSetViewModel() override;

    Q_INVOKABLE int setDataByAreaSet(int areaType, int axis, int value);
    Q_INVOKABLE int setDataByAreaFix(int areaType, int axis, int value);
    Q_INVOKABLE int setMemoryData(int areaType, int axis, int value);
    Q_INVOKABLE void setMxSxSpaceValue(int value);
    Q_INVOKABLE QString formatData(int areaType, int value);
    Q_INVOKABLE QString formatPosition(int value);
    Q_INVOKABLE QString formatAreaFixData(int areaType, int value);
    Q_INVOKABLE QString formatAreaFixPosition(int value);
    Q_INVOKABLE QString formatMxSxSpace(int value);
    Q_INVOKABLE QList<QObject *> getDataList(int areaType);
    Q_INVOKABLE int dataMagnification();
    Q_INVOKABLE void startPageChanging();

    void setDataList(QList<QObject *> list);
    QList<QObject *> dataList() const { return m_datas.at(m_currentAreaType); }
    void setCurrentAreaType(int type);
    int currentAreaType() const { return m_currentAreaType; }
    void setIoSignalList(QList<bool> ioSignalList);
    QList<bool> ioSignalList() const { return m_ioSignalList; }
    void setMxSxSpace(int value);
    int mxSxSpace() const { return m_mxSxSpace; }
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }
signals:
    void dataListChanged();
    void currentAreaTypeChanged();
    void ioSignalListChanged();
    void mxSxSpaceChanged();
    void dataLoadedChanged(bool dataLoaded);

public slots:
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void endModbusPageChange(int mainScreenId, int subScreenId);

private:
     void init();
     void initData();
     void onActivate() override;
     void onDeactivate() override;
     void readDataFormatCsv();
     void readPenSetData();
     void readPenSetAreaFixData();
     bool updateAxisData();
     bool updateIoSignal();
     void getAllAreaData();
     void startModbusOneShot();
     void stopModbusOneShot();
     void modbusSaveData(quint16 address, int value);
     void modbusGetData(quint16 address);
     void modbusGetData(quint16 address, quint16 size);
     void modbusDelayGetData(quint16 address, quint16 size);
     void modbusMxSxSpace(int value);
     void modbusGetMxSxSpace();
     void setOneShotModbusCommand(stModBusCommunicationData &data);
     int setData(int areaType, int axis, int value);
    int getPointNum(int areaType, int axis);

private:
     AxisInfo                   *m_axisInfo = AxisInfo::get_instance();
     CommonViewModel            *m_commonView;

     bool                       m_isModbusConnectOneShot = false;
     int                        m_positionFormat;
     QList<int>                 m_dataFormatList;
     int                        m_areaFixPositionFormat;
     QList<int>                 m_areaFixDataFormatList;
     int                        m_areaFixMxSxSpaceFormat;
     QList<QList<QObject *>>    m_datas;
     QList<bool>                m_ioSignalList;
     int                        m_mxSxSpace = 0;
     int                        m_currentAreaType = Enums::AreaTypeOutputSideMax;
     bool                       m_pageChanging = false;
     bool                       m_dataLoaded = false;
     int                        m_remainAllAreaDataNum = 0;
     bool                       m_delayReadingAllAreaData = false;
     const QList<int>           AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};
};

#endif // AREASETVIEWMODEL_H
