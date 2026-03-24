#ifndef TEACHINGCOUNTERPAGEMODEL_H
#define TEACHINGCOUNTERPAGEMODEL_H

#include <QObject>
#include "pagemodelbase.h"
#include "resourcemanager.h"

#define PENSET_GROUP        ("14")
#define PENSET_GROUP_NUM    (14)
#define CURRENTVALUE_SR     (2777)      // SR2777
#define SETTINGVALUE_HR     (2293)      // HR2293
#define SETTINGVALUE_HR_END (2339 + 1)  // HR2340

class TeachingCounterPageModel : public PageModelBase
{
    Q_OBJECT
    Q_PROPERTY(int numOfCounter READ numOfCounter NOTIFY numOfCounterChanged)
    Q_PROPERTY(QList<int> currentValueTbl READ currentValueTbl NOTIFY currentValueTblChanged)
    Q_PROPERTY(QList<int> settingValueTbl READ settingValueTbl NOTIFY settingValueTblChanged)
    Q_PROPERTY(QString numericTextColorRGB READ numericTextColorRGB CONSTANT)
    Q_PROPERTY(int numOfCountersDisplayed READ numOfCountersDisplayed CONSTANT)
    Q_PROPERTY(int currentValueDisplayFormatNo READ currentValueDisplayFormatNo CONSTANT)
    Q_PROPERTY(int settingValueDisplayFormatNo READ settingValueDisplayFormatNo CONSTANT)
    Q_PROPERTY(QList<int> counterMsgIndex READ counterMsgIndex CONSTANT)

public:
    explicit TeachingCounterPageModel(ResourceManager *resource, CommunicationThread * comm);
    explicit TeachingCounterPageModel(QObject *object = nullptr);

    int numOfCounter() { return m_numOfCounter; }
    QList<int> currentValueTbl() { return m_currentValueTbl; }
    QList<int> settingValueTbl() { return m_settingValueTbl; }
    QString numericTextColorRGB() const { return m_numericTextColorRGB; }
    int numOfCountersDisplayed() const { return m_numOfCountersDisplayed; }
    int currentValueDisplayFormatNo() const { return m_currentValueDisplayFormatNo; }
    int settingValueDisplayFormatNo() const { return m_settingValueDisplayFormatNo; }
    QList<int> counterMsgIndex() { return m_counterMsgIndex; }

    Q_INVOKABLE void editSettingValue(int index, int value);
    void initcounterMsgIndex();

signals:
    void numOfCounterChanged(int numOfCounter);
    void currentValueTblChanged(QList<int> currentValue);
    void settingValueTblChanged(QList<int> settingValue);
    void counterMsgIndexChanged(QList<int> settingValue);

public slots:
    void onLanguageChanged(int language);
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

    // ViewModelBase interface
protected:
    void onActivate() override;
    void onDeactivate() override;

private:
    void init();
    void readPenSetData();
    void startModbusPeriodic();
    void stopModbusPeriodic();
    void startModbusOneShot();
    void stopModbusOneShot();
    void requestToWriteSettingValue(int index, int value);
    void requestToReadSettingValues(uint index, uint range);
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void updateCurrentValueTbl();
    void updateSettingValueTbl(stModBusCommunicationData &command);
    void setNumOfCounter(int numOfCounter) {
        if (numOfCounter != m_numOfCounter) {
            m_numOfCounter = numOfCounter;
            emit numOfCounterChanged(m_numOfCounter);
        }
    }

private:
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();
    int                     m_numOfCounter;
    QList<int>              m_currentValueTbl;
    QList<int>              m_settingValueTbl;
    bool                    mIsModbusConnectOneShot;
    QString                 m_numericTextColorRGB;
    int                     m_numOfCountersDisplayed;
    int                     m_currentValueDisplayFormatNo;
    int                     m_settingValueDisplayFormatNo;
    QList<int>              m_counterMsgIndex;
    const QList<int>        AXIS_3_POS_ADDR_LIST = {2819, 2821, 2823};
};

#endif // TEACHINGCOUNTERPAGEMODEL_H
