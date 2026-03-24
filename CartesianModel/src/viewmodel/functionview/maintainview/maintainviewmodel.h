#ifndef MAINTAINVIEWMODEL_H
#define MAINTAINVIEWMODEL_H

#include <QObject>
#include "viewmodelbase.h"

class MaintainData;

class MaintainViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<bool> maintainCompleteList READ maintainCompleteList WRITE setMaintainCompleteList NOTIFY maintainCompleteListChanged)
    Q_PROPERTY(QList<QString> lastInspectionDateList READ lastInspectionDateList WRITE setLastInspectionDateList NOTIFY lastInspectionDateListChanged)
    Q_PROPERTY(QList<bool> refuelingCompleteList READ refuelingCompleteList WRITE setRefuelingCompleteList NOTIFY refuelingCompleteListChanged)
    Q_PROPERTY(QList<QObject *> maintainDataList READ maintainDataList WRITE setMaintainDataList NOTIFY maintainDataListChanged)

public:
    explicit MaintainViewModel(ResourceManager *resource, CommunicationThread * comm);
    explicit MaintainViewModel(QObject *object = nullptr);

    Q_INVOKABLE void saveLastInspectionDate(const QString& date);
    Q_INVOKABLE void saveLastRefuelingDate(const QString& date);
    Q_INVOKABLE QString formatRefuelingDistance(int value);
    Q_INVOKABLE QString formatCumulativeDistance(int value);

    void setMaintainCompleteList(QList<bool>& list);
    QList<bool> maintainCompleteList() const { return m_maintainCompleteList; }
    void setLastInspectionDateList(QList<QString>& list);
    QList<QString> lastInspectionDateList() const { return m_lastInspectionDateList; }
    void setRefuelingCompleteList(QList<bool>& list);
    QList<bool> refuelingCompleteList() const { return m_refuelingCompleteList; }
    void setMaintainDataList(QList<QObject *>& list);
    QList<QObject *> maintainDataList() const { return m_maintainDataList; }

signals:
    void maintainCompleteListChanged();
    void lastInspectionDateListChanged();
    void refuelingCompleteListChanged();
    void maintainDataListChanged();

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void onFinished();

private:
    void init();
    void initData();
    void readDataFormatCsv();
    void readPenSetData();
    void onActivate() override;
    void onDeactivate() override;
    int modbusSetLastInspectionDate(int dataIndex, int year, int month, int day);
    int modbusSetLastRefuelingDate(int dataIndex, int year, int month, int day);
    int modbusSaveMaintainData();
    void modbusGetLastInspectionDate();
    void modbusGetDistanceData();
    void modbusGetLastRefuelingDate();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void startModbusOneShot();
    void stopModbusOneShot();
    void updateAllLastInspectionDate();
    void updateAllDistanceData();
    void updateAllLastRefuelingDate();
    void getDate(const QString &dateString, int &year, int &month, int &day);

private:
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();

    bool                        m_isModbusConnectOneShot = false;
    QList<bool>                 m_maintainCompleteList;
    QList<bool>                 m_refuelingCompleteList;
    QList<QString>              m_lastInspectionDateList;
    QList<QObject *>            m_maintainDataList;
    int                         m_refuelingDistanceFormat;
    int                         m_cumulativeDistanceFormat;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};
};

#endif // MAINTAINVIEWMODEL_H
