#ifndef TEACHINGTIMERPAGEMODEL_H
#define TEACHINGTIMERPAGEMODEL_H

#include <QObject>
#include "pagemodelbase.h"
#include "timerdata.h"
#include "timeraddress.h"

#define TIMER_LIST_ITEM_NUM         100
#define TIMER_MAX_TIME              65535
#define TIMER_TIME_MAGNIFICATION    100
#define TIMER_TIME_TOP_HR           2125
#define TIMER_TIME_BOTTOM_HR        2224
#define TIMER_TIME_HR_NUM           (TIMER_TIME_BOTTOM_HR - TIMER_TIME_TOP_HR + 1)

class TeachingTimerPageModel : public PageModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> dataList READ dataList CONSTANT)
    Q_PROPERTY(QList<int> timerListOrder READ timerListOrder CONSTANT)
public:
    explicit TeachingTimerPageModel(ResourceManager *resource, CommunicationThread * comm);
    explicit TeachingTimerPageModel(QObject *object = nullptr);
    ~TeachingTimerPageModel() override;

    Q_INVOKABLE int getIntegerDigits(int timerIndex);
    Q_INVOKABLE QString getUnit(int timerIndex);
    Q_INVOKABLE int setTime(int timerIndex, double time);
    Q_INVOKABLE QString formatSpeed(int timerIndex, int time);

    QList<QObject *> dataList() const { return m_dataList; }
    QList<int> timerListOrder() const { return m_timerListOrder; }

public slots:
    void onLanguageChanged(int language);
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

private:
    void init();
    void onActivate() override;
    void onDeactivate() override;
    void readPenSetData();
    void readPenSetFormatData();
    void readPenSetAddressData();
    void startModbusOneShot();
    void stopModbusOneShot();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void modbusGetTime(quint16 hrAddress, quint16 size);
    void modbusSaveTime(quint16 hrAddress, int time);
    void updateTimeData(quint16 startHrAddress, quint16 size);
    void updateLanguageInfo();

private:
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();
    QList<int>                  m_timerListOrder;
    QList<QObject *>            m_dataList;
    QList<TimerAddress *>       m_addressList;
    bool                        mIsModbusConnectOneShot = false;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2819, 2821, 2823};

};

#endif // TEACHINGTIMERPAGEMODEL_H
