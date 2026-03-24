#ifndef VERSIONVIEWMODEL_H
#define VERSIONVIEWMODEL_H

#include <QObject>
#include <QStringList>

#include "viewmodelbase.h"

#define NUM_OF_VERSION      (16)
#define VERSION_DATA_SR     (608)
#define VERSION_DATA_SIZE   (6)
#define VERSION_DATA_RANGE  (NUM_OF_VERSION * VERSION_DATA_SIZE)

//------------------------------------------------------
// VersionViewModel.
//------------------------------------------------------
class VersionViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QStringList versionTbl READ versionTbl  NOTIFY versionTblChanged)

public:
    explicit VersionViewModel(ResourceManager *resource, CommunicationThread * comm);
    virtual ~VersionViewModel() override;

    QStringList versionTbl() const { return m_versionTbl; }

    // for debug.
    void display();

signals:
    void versionTblChanged(const QStringList&);

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onOneShotReadFinished(stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void onFinished();

private:
    void startModbusOneShot();
    void stopModbusOneShot();
    void setOneShotModbusCommand(stModBusCommunicationData&);
    void readModbus(const QModbusPdu::FunctionCode, const quint16&, const quint16 &);
    void readModbusSR(const quint16&, const quint16&);
    void modbusGetVersions();
    void updateVersions(const stModBusCommunicationData&);
    void appendVersionsCollectedFromModbus(const stModBusCommunicationData&);
    void appendIndividuallyCollectedVersions();

private:
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();

    bool                    mIsModbusConnectOneShot;
    QStringList             m_versionTbl;
    const QList<int>        AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};
};

#endif // VERSIONVIEWMODEL_H
