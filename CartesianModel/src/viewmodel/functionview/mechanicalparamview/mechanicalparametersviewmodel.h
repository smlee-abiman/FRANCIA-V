#ifndef MECHANICALPARAMETERSVIEWMODEL_H
#define MECHANICALPARAMETERSVIEWMODEL_H

#include "viewmodelbase.h"
#include "commonviewmodel.h"
#include <QQmlEngine>
#include <QtMath>
#include <cmath>

#define MC_MPVM_DATA1_HR            0xD05E
#define MC_MPVM_DATA2_HR            0xD0DB
#define MC_MPVM_DATA3_HR            0xD158
#define MC_MPVM_DATA4_HR            0xD1D5
#define MC_MPVM_DATA5_HR            0xD252
#define MC_MPVM_DATA_SIZE           0x007D
#define MC_MPVM_MODEL_SR_TOP        2761

#define MC_MPVM_SAVE_HR             1657

#define MC_MPVM_ROTETION_RANGE_TOP_HR                       53342
#define MC_MPVM_ROTETION_PULSE_TOP_HR                       53350
#define MC_MPVM_MAX_ROTETION_TOP_HR                         53486
#define MC_MPVM_FRONT_END_RANGE_TOP_HR                      53822
#define MC_MPVM_ORIGIN_CORRECTION_TOP_HR                    53742
#define MC_MPVM_BEFORE_ORIGIN_RETURN_SPEED_TOP_HR           53406
#define MC_MPVM_OPERATION_SPEED_L_TOP_HR                    53702
#define MC_MPVM_OPERATION_SPEED_M_TOP_HR                    53702
#define MC_MPVM_OPERATION_SPEED_H_TOP_HR                    53710
#define MC_MPVM_ACCELERATION_AND_DECELERATION_TIME_MIN_HR   53502
#define MC_MPVM_ACCELERATION_AND_DECELERATION_TIME_MAX_HR   53598
#define MC_MPVM_REFUELING_RANGE_TOP_HR                      53838

class MechanicalParam : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ushort rotationRange READ rotationRange NOTIFY rotationRangeChanged)
    Q_PROPERTY(bool isChangeRotationRange READ isChangeRotationRange NOTIFY isChangeRotationRangeChanged)
    Q_PROPERTY(ushort rotationPulse READ rotationPulse NOTIFY rotationPulseChanged)
    Q_PROPERTY(bool isChangeRotationPulse READ isChangeRotationPulse NOTIFY isChangeRotationPulseChanged)
    Q_PROPERTY(ushort maxRotation READ maxRotation NOTIFY maxRotationChanged)
    Q_PROPERTY(bool isChangeMaxRotation READ isChangeMaxRotation NOTIFY isChangeMaxRotationChanged)
    Q_PROPERTY(ushort frontEndRange READ frontEndRange  NOTIFY frontEndRangeChanged)
    Q_PROPERTY(bool isChangeFrontEndRange READ isChangeFrontEndRange NOTIFY isChangeFrontEndRangeChanged)
    Q_PROPERTY(short originCorrection READ originCorrection NOTIFY originCorrectionChanged)
    Q_PROPERTY(bool isChangeOriginCorrection READ isChangeOriginCorrection NOTIFY isChangeOriginCorrectionChanged)
    Q_PROPERTY(uchar beforeOriginReturnSpeed READ beforeOriginReturnSpeed NOTIFY beforeOriginReturnSpeedChanged)
    Q_PROPERTY(bool isChangeBeforeOriginReturnSpeed READ isChangeBeforeOriginReturnSpeed NOTIFY isChangeBeforeOriginReturnSpeedChanged)
    Q_PROPERTY(uchar freeOperationSpeedLow READ freeOperationSpeedLow NOTIFY freeOperationSpeedLowChanged)
    Q_PROPERTY(bool isChangeFreeOperationSpeedLow READ isChangeFreeOperationSpeedLow NOTIFY isChangeFreeOperationSpeedLowChanged)
    Q_PROPERTY(uchar freeOperationSpeedMedium READ freeOperationSpeedMedium NOTIFY freeOperationSpeedMediumChanged)
    Q_PROPERTY(bool isChangeFreeOperationSpeedMedium READ isChangeFreeOperationSpeedMedium NOTIFY isChangeFreeOperationSpeedMediumChanged)
    Q_PROPERTY(uchar freeOperationSpeedHigh READ freeOperationSpeedHigh NOTIFY freeOperationSpeedHighChanged)
    Q_PROPERTY(bool isChangeFreeOperationSpeedHigh READ isChangeFreeOperationSpeedHigh NOTIFY isChangeFreeOperationSpeedHighChanged)
    Q_PROPERTY(uchar accelerationAndDecelerationTimeMin READ accelerationAndDecelerationTimeMin NOTIFY accelerationAndDecelerationTimeMinChanged)
    Q_PROPERTY(bool isChangeAccelerationAndDecelerationTimeMin READ isChangeAccelerationAndDecelerationTimeMin NOTIFY isChangeAccelerationAndDecelerationTimeMinChanged)
    Q_PROPERTY(uchar accelerationAndDecelerationTimeMax READ accelerationAndDecelerationTimeMax NOTIFY accelerationAndDecelerationTimeMaxChanged)
    Q_PROPERTY(bool isChangeAccelerationAndDecelerationTimeMax READ isChangeAccelerationAndDecelerationTimeMax NOTIFY isChangeAccelerationAndDecelerationTimeMaxChanged)
    Q_PROPERTY(ulong refuelingRange READ refuelingRange NOTIFY refuelingRangeChanged)
    Q_PROPERTY(bool isChangeRefuelingRange READ isChangeRefuelingRange NOTIFY isChangeRefuelingRangeChanged)

public:
    explicit MechanicalParam(QObject *parent = nullptr);
    ~MechanicalParam();

    void setRotationRange(ushort rotationRange, bool firstDataLoaded);
    void setRotationPulse(ushort rotationPulse, bool firstDataLoaded);
    void setMaxRotation(ushort maxRotation, bool firstDataLoaded);
    void setfrontEndRange(ushort frontEndRange, bool firstDataLoaded);
    void setOriginCorrection(short originCorrection, bool firstDataLoaded);
    void setBeforeOriginReturnSpeed(uchar beforeOriginReturnSpeed, bool firstDataLoaded);
    void setFreeOperationSpeedLow(uchar freeOperationSpeedLow, bool firstDataLoaded);
    void setFreeOperationSpeedMedium(uchar freeOperationSpeedMedium, bool firstDataLoaded);
    void setFreeOperationSpeedHigh(uchar freeOperationSpeedHigh, bool firstDataLoaded);
    void setAccelerationAndDecelerationTimeMin(uchar accelerationAndDecelerationTimeMin, bool firstDataLoaded);
    void setAccelerationAndDecelerationTimeMax(uchar accelerationAndDecelerationTimeMax, bool firstDataLoaded);
    void setRefuelingRange(ulong refuelingRange, bool firstDataLoaded);

    void resetIsChange();

    ushort rotationRange() const { return m_rotationRange; }
    ushort rotationPulse() const { return m_rotationPulse; }
    ushort maxRotation() const { return m_maxRotation; }
    ushort frontEndRange() const { return m_frontEndRange; }
    short originCorrection() const { return m_originCorrection; }
    uchar beforeOriginReturnSpeed() const { return m_beforeOriginReturnSpeed; }
    uchar freeOperationSpeedLow() const { return m_freeOperationSpeedLow; }
    uchar freeOperationSpeedMedium() const { return m_freeOperationSpeedMedium; }
    uchar freeOperationSpeedHigh() const { return m_freeOperationSpeedHigh; }
    uchar accelerationAndDecelerationTimeMin() const { return m_accelerationAndDecelerationTimeMin; }
    uchar accelerationAndDecelerationTimeMax() const { return m_accelerationAndDecelerationTimeMax; }
    ulong refuelingRange() const { return m_refuelingRange; }
    bool isChangeRotationRange() const { return m_isChangeRotationRange; }
    bool isChangeRotationPulse() const { return m_isChangeRotationPulse; }
    bool isChangeMaxRotation() const { return m_isChangeMaxRotation; }
    bool isChangeFrontEndRange() const { return m_isChangeFrontEndRange; }
    bool isChangeOriginCorrection() const { return m_isChangeOriginCorrection; }
    bool isChangeBeforeOriginReturnSpeed() const { return m_isChangeBeforeOriginReturnSpeed; }
    bool isChangeFreeOperationSpeedLow() const { return m_isChangeFreeOperationSpeedLow; }
    bool isChangeFreeOperationSpeedMedium() const { return m_isChangeFreeOperationSpeedMedium; }
    bool isChangeFreeOperationSpeedHigh() const { return m_isChangeFreeOperationSpeedHigh; }
    bool isChangeAccelerationAndDecelerationTimeMin() const { return m_isChangeAccelerationAndDecelerationTimeMin; }
    bool isChangeAccelerationAndDecelerationTimeMax() const { return m_isChangeAccelerationAndDecelerationTimeMax; }
    bool isChangeRefuelingRange() const { return m_isChangeRefuelingRange; }

private:
    ushort m_rotationRange;
    ushort m_rotationPulse;
    ushort m_maxRotation;
    ushort m_frontEndRange;
    short m_originCorrection;
    uchar m_beforeOriginReturnSpeed;
    uchar m_freeOperationSpeedLow;
    uchar m_freeOperationSpeedMedium;
    uchar m_freeOperationSpeedHigh;
    uchar m_accelerationAndDecelerationTimeMin;
    uchar m_accelerationAndDecelerationTimeMax;
    ulong m_refuelingRange;
    bool m_isChangeRotationRange;
    bool m_isChangeRotationPulse;
    bool m_isChangeMaxRotation;
    bool m_isChangeFrontEndRange;
    bool m_isChangeOriginCorrection;
    bool m_isChangeBeforeOriginReturnSpeed;
    bool m_isChangeFreeOperationSpeedLow;
    bool m_isChangeFreeOperationSpeedMedium;
    bool m_isChangeFreeOperationSpeedHigh;
    bool m_isChangeAccelerationAndDecelerationTimeMin;
    bool m_isChangeAccelerationAndDecelerationTimeMax;
    bool m_isChangeRefuelingRange;

signals:
    void rotationRangeChanged(ushort rotationRange);
    void rotationPulseChanged(ushort rotationPulse);
    void maxRotationChanged(ushort maxRotation);
    void frontEndRangeChanged(ushort frontEndRange);
    void originCorrectionChanged(short originCorrection);
    void beforeOriginReturnSpeedChanged(uchar beforeOriginReturnSpeed);
    void freeOperationSpeedLowChanged(uchar freeOperationSpeedLow);
    void freeOperationSpeedMediumChanged(uchar freeOperationSpeedMedium);
    void freeOperationSpeedHighChanged(uchar freeOperationSpeedHigh);
    void accelerationAndDecelerationTimeMinChanged(uchar accelerationAndDecelerationTimeMin);
    void accelerationAndDecelerationTimeMaxChanged(uchar accelerationAndDecelerationTimeMax);
    void refuelingRangeChanged(ulong refuelingRange);
    void isChangeRotationRangeChanged(bool isChangeRotationRange);
    void isChangeRotationPulseChanged(bool isChangeRotationPulse);
    void isChangeMaxRotationChanged(bool isChangeMaxRotation);
    void isChangeFrontEndRangeChanged(bool isChangeFrontEndRange);
    void isChangeOriginCorrectionChanged(bool isChangeOriginCorrection);
    void isChangeBeforeOriginReturnSpeedChanged(bool isChangeBeforeOriginReturnSpeed);
    void isChangeFreeOperationSpeedLowChanged(bool isChangeFreeOperationSpeedLow);
    void isChangeFreeOperationSpeedMediumChanged(bool isChangeFreeOperationSpeedMedium);
    void isChangeFreeOperationSpeedHighChanged(bool isChangeFreeOperationSpeedHigh);
    void isChangeAccelerationAndDecelerationTimeMinChanged(bool isChangeAccelerationAndDecelerationTimeMin);
    void isChangeAccelerationAndDecelerationTimeMaxChanged(bool isChangeAccelerationAndDecelerationTimeMax);
    void isChangeRefuelingRangeChanged(bool isChangeRefuelingRange);
};

class MechanicalParametersViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int pageNum READ pageNum WRITE setPageNum NOTIFY pageNumChanged)
    Q_PROPERTY(int pageMaxNum READ pageMaxNum CONSTANT)
    Q_PROPERTY(int axisPageNum READ axisPageNum WRITE setAxisPageNum NOTIFY axisPageNumChanged)
    Q_PROPERTY(int axisPageMaxNum READ axisPageMaxNum CONSTANT)

    Q_PROPERTY(QString modelName READ modelName NOTIFY modelNameChanged)
    Q_PROPERTY(int saveStatus READ saveStatus WRITE setSaveStatus NOTIFY saveStatusChanged)

public:
    explicit MechanicalParametersViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~MechanicalParametersViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();

    Q_INVOKABLE void pageInit();

    Q_INVOKABLE int getIndex(int axisNum, int dataNum);
    Q_INVOKABLE void setValue(int axisNum, int dataNum, double value);
    Q_INVOKABLE void initParameter();
    Q_INVOKABLE void saveParameter();

    Q_INVOKABLE QList<QObject*> getDataList(int axisPage);

    void setPageNum(int pageNum);
    void setAxisPageNum(int axisPageNum);
    void setSaveStatus(int saveStatus);
    void setModelName(QString modelName);

    int pageNum() const { return m_pageNum; }
    int pageMaxNum() const { return m_pageMaxNum; }
    int axisPageNum() const { return m_axisPageNum; }
    int axisPageMaxNum() const { return m_axisPageMaxNum; }
    int saveStatus() const { return m_saveStatus; }
    QString modelName() const { return m_modelName; }

private:
    ModbusManager               *m_modbusManager;
    CommonViewModel             *m_commonView;
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();
    MechanicalParam             *m_dataList[8];

    int                         m_pageNum;
    int                         m_pageMaxNum;
    int                         m_axisPageNum;
    int                         m_axisPageMaxNum;
    int                         m_saveStatus;
    QString                     m_modelName;
    bool                        m_firstDataLoadedFlag;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2819, 2821, 2823};

    void pageCommandInit();
    void readParameter();
    int getValue(int axisNum, int dataNum);

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onFinished();
    void onReadParameterFinished();
    void onDataInitFinished();
    void onDataSaveFinished();

signals:
    void pageNumChanged(int pageNum);
    void axisPageNumChanged(int pageNum);
    void saveStatusChanged(int saveStatus);
    void modelNameChanged(QString modelName);
};

#endif // MECHANICALPARAMETERSVIEWMODEL_H
