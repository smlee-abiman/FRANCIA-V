#ifndef NETWORKSETTINGVIEWMODEL_H
#define NETWORKSETTINGVIEWMODEL_H

#include <QObject>
#include <viewmodelbase.h>


#define IP_ADDRESS_HR            (52655)
#define IP_ADDRESS_SIZE          (4)
#define IP_ADDRESS_RANGE         (IP_ADDRESS_SIZE * 2)
#define SUBNET_MASK_HR           (52663)
#define SUBNET_MASK_SIZE         (4)
#define SUBNET_MASK_RANGE        (SUBNET_MASK_SIZE * 2)
#define MAC_ADDRESS_HR           (52671)
#define MAC_ADDRESS_SIZE         (6)
#define MAC_ADDRESS_RANGE        (MAC_ADDRESS_SIZE * 2)
#define MODBUS_ID_HR             (52653)
#define MODBUS_ID_RANGE          (2)     // modbus id(1) * 2.
#define APPLY_NETWORK_SETTING_HR (2533)

//------------------------------------------------------
// NetworkSetting.
//------------------------------------------------------
class NetworkSetting : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<int> ipAddress READ ipAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(QList<int> subnetMask READ subnetMask NOTIFY subnetMaskChanged)
    Q_PROPERTY(QList<int> macAddress READ macAddress NOTIFY macAddressChanged)
    Q_PROPERTY(quint16 modbusId READ modbusId NOTIFY modbusIdChanged)

public:
    explicit NetworkSetting(QObject *parent = nullptr);
    virtual ~NetworkSetting() override;

    quint16 modbusId() const { return m_modbusId; }
    QList<int> ipAddress() const { return m_ipAddress; }
    QList<int> subnetMask() const { return m_subnetMask; }
    QList<int> macAddress() const { return m_macAddress; }

    void setModbusId(const quint16 &modbusId);
    void setIpAddress(const QList<int> &ipAddress);
    void setSubnetMask(const QList<int> &subnetMask);
    void setMacAddress(const QList<int> &macAddress);

    Q_INVOKABLE void editModbusId(const quint16 value);
    Q_INVOKABLE void editIpAddress(const quint16 index, const int value);
    Q_INVOKABLE void editSubnetMask(const quint16 index, const int value);

    void saveToSdControl();

    // for debug.
    void display();

signals:
    void ipAddressChanged(QList<int>);
    void subnetMaskChanged(QList<int>);
    void macAddressChanged(QList<int>);
    void modbusIdChanged(quint16);

private:
    void saveIpAddressToControllerSettings();
    void saveModbusIdToPendantSettings();

private:
    static int g_counter;
    QString m_entryId;
    quint16 m_modbusId;
    QList<int> m_ipAddress;
    QList<int> m_subnetMask;
    QList<int> m_macAddress;
};
//------------------------------------------------------
// NetworkSettingViewModel.
//------------------------------------------------------
class NetworkSettingViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> networkSettingTbl READ networkSettingTbl CONSTANT)

public:
    explicit NetworkSettingViewModel(ResourceManager *resource, CommunicationThread * comm);
    virtual ~NetworkSettingViewModel() override;

    QList<QObject*> networkSettingTbl() const { return m_networkSettingTbl; }

    Q_INVOKABLE void saveNetworkSettings();

    // for debug.
    void display();

public slots:
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

protected:
    void onActivate() override;
    void onDeactivate() override;

private:
    void init();
    void startModbusOneShot();
    void stopModbusOneShot();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void writeModbusHR(const quint16 hrAddress, const QList<int> &writeData);
    void readModbusHR(const quint16 &hrAddress, const quint16 &range);
    void modbusWriteIpAddress(const QList<int> &writeData);
    void modbusWriteSubnetMask(const QList<int> &writeData);
    void modbusWriteModbusId(const QList<int> &writeData);
    void modbusWriteApplyNetworkSetting();
    void modbusGetIpAddress();
    void modbusGetSubnetMask();
    void modbusGetMacAddress();
    void modbusGetModbusId();
    void updateIpAddress(const stModBusCommunicationData &command);
    void updateSubnetMask(const stModBusCommunicationData &command);
    void updateMacAddress(const stModBusCommunicationData &command);
    void updateModbusId(const stModBusCommunicationData &command);

private:
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();

    bool                        mIsModbusConnectOneShot;
    QList<QObject*>             m_networkSettingTbl;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2761, 2763, 2765};
};

#endif // NETWORKSETTINGVIEWMODEL_H
