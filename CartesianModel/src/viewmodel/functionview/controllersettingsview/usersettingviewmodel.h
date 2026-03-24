#ifndef USERSETTINGVIEWMODEL_H
#define USERSETTINGVIEWMODEL_H

#include <algorithm>

#include <QObject>
#include <QList>
#include <viewmodelbase.h>

#define SCREEN_VISIBILITY_RANGE  (40)
#define SCREEN_VISIBILITY_HR     (52513)
#define SCREEN_VISIBILITY_HR_END (SCREEN_VISIBILITY_HR + SCREEN_VISIBILITY_RANGE - 1)

//------------------------------------------------------
// ScreenDp.
//------------------------------------------------------
class ScreenDp : public QObject
{
    Q_OBJECT

public:
    explicit ScreenDp(int dp1, int dp2, QObject* parent = nullptr)
        : QObject(parent), m_dp1(dp1), m_dp2(dp2) {}
    virtual ~ScreenDp() {}

    int dp1() const { return m_dp1; }
    int dp2() const { return m_dp2; }
    QString key() const
    {
        QString str;
        str.sprintf("%02d,%02d",m_dp1, m_dp2);
        return str;
    }

    bool operator<(const ScreenDp& other) const {
        bool ret = (m_dp1 < other.dp1());
        return (ret || (m_dp2 < other.dp2()));
    }

private:
    int m_dp1;
    int m_dp2;
};
//------------------------------------------------------
// ScreenSettings.
//------------------------------------------------------
class ScreenSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<bool> visibles READ visibles NOTIFY visiblesChanged)

public:
    explicit ScreenSettings(int index, ScreenDp* dp, QObject* parent = nullptr);
    virtual ~ScreenSettings();

    int index() const { return m_index; }
    QList<bool> visibles() const { return m_visibles; }
    void setVisibles(const QList<bool> &visibles);

    // for debug.
    void display();

signals:
    void visiblesChanged(QList<bool>&);

private:
    int m_index;
    ScreenDp* m_dp;
    QList<bool> m_visibles;
};

//------------------------------------------------------
// UserSettingViewModel.
//------------------------------------------------------
class UserSettingViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> screenSettingsTbl READ screenSettingsTbl NOTIFY screenSettingsTblChanged)
    Q_PROPERTY(quint16 numOfScreens READ numOfScreens CONSTANT)
    Q_PROPERTY(int systemSettingsDisplayLevel READ systemSettingsDisplayLevel CONSTANT)
public:
    explicit UserSettingViewModel(ResourceManager *resource, CommunicationThread * comm);
    virtual ~UserSettingViewModel() override;

    QList<QObject*> screenSettingsTbl() const;
    quint16 numOfScreens() const { return m_screenSettingsTbl.size(); }
    int systemSettingsDisplayLevel() const { return m_systemSettingsDisplayLevel; }

    Q_INVOKABLE void toggleScreenVisibility(QObject*, int userLevel);
    Q_INVOKABLE QObject* getScreenSettings(int dp1, int dp2);

    // for debug.
    void display();

signals:
    void screenSettingsTblChanged(QList<QObject*>&);

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onOneShotReadFinished(stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void onFinished();

private:
    void readPenSetData();
    void startModbusPeriodic();
    void stopModbusPeriodic();
    void startModbusOneShot();
    void stopModbusOneShot();
    void decrementNumOfRemainingInitialModbusReads();
    void writeModbusHR(quint16 hrAddress, const QList<int> &writeData);
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void readModbus(const QModbusPdu::FunctionCode funcCode, const quint16 &address, const quint16 &range);
    void readModbusHR(const quint16 &hrAddress, const quint16 &range);
    void modbusGetScreenSettings(const quint16& index = 0, const quint16& range = SCREEN_VISIBILITY_RANGE);
    void updateScreenSettings(const stModBusCommunicationData& command);

private:
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();
    bool                        mIsModbusConnectOneShot;
    quint16                     m_numOfRemainingInitialModbusReads;
    QHash<int, QObject*>        m_screenSettingsTbl;
    QHash<QString, QObject*>    m_screenSettingsReferenceTbl;
    int                         m_systemSettingsDisplayLevel;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2761, 2763, 2765};
};

#endif // USERSETTINGVIEWMODEL_H
