#ifndef AUTOVIEWMODEL_H
#define AUTOVIEWMODEL_H

#include "autoaxisviewmodel.h"
#include "autocounterviewmodel.h"
#include "autoioviewmodel.h"
#include "autoprodviewmodel.h"
#include "autotimerviewmodel.h"
#include "viewmodelbase.h"
#include "commonviewmodel.h"
#include "axisinfo.h"

class AutoViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(AutoAxisViewModel* axisView READ axisView CONSTANT)
    Q_PROPERTY(AutoProdViewModel* prodView READ prodView CONSTANT)
    Q_PROPERTY(AutoIOViewModel* ioView READ ioView CONSTANT)
    Q_PROPERTY(AutoTimerViewModel* timerView READ timerView CONSTANT)
    Q_PROPERTY(AutoCounterViewModel* counterView READ counterView CONSTANT)

    Q_PROPERTY(int selectTab READ selectTab WRITE setSelectTab NOTIFY selectTabChanged)
    Q_PROPERTY(bool oneCycle READ oneCycle NOTIFY oneCycleChanged)
    Q_PROPERTY(bool cycle READ cycle NOTIFY cycleChanged)
    Q_PROPERTY(bool isRun READ isRun NOTIFY isRunChanged)
    Q_PROPERTY(bool step READ step NOTIFY stepChanged)
    Q_PROPERTY(bool isFront READ isFront NOTIFY isFrontChanged)
    Q_PROPERTY(bool isBack READ isBack NOTIFY isBackChanged)
    Q_PROPERTY(bool autoRunning READ autoRunning NOTIFY autoRunningChanged)

public:
    explicit AutoViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~AutoViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();

    AutoAxisViewModel* axisView() const { return (AutoAxisViewModel*)m_panelModel.at(0); }
    AutoProdViewModel* prodView() const { return (AutoProdViewModel*)m_panelModel.at(1); }
    AutoIOViewModel* ioView() const { return (AutoIOViewModel*)m_panelModel.at(2); }
    AutoTimerViewModel* timerView() const { return (AutoTimerViewModel*)m_panelModel.at(3); }
    AutoCounterViewModel* counterView() const { return (AutoCounterViewModel*)m_panelModel.at(4); }

    void setSelectTab(int selectTab);
    void setOneCycle(bool oneCycle);
    void setCycle(bool cycle);
    void setIsRun(bool isRun);
    void setStep(bool step);
    void setIsFront(bool isFront);
    void setIsBack(bool isBack);
    void setautoRunning(bool autoRunning);

    int selectTab() { return m_selectTab; }
    bool oneCycle() { return m_oneCycle; }
    bool cycle() { return m_cycle; }
    bool isRun() { return m_isRun; }
    bool step() { return m_step; }
    bool isFront() { return m_isFront; }
    bool isBack() { return m_isBack; }
    bool autoRunning() { return m_autoRunning; }

private:
    ModbusManager       *m_modbusManager;
    CommonViewModel     *m_commonView;
    AxisInfo            *m_axisInfo = AxisInfo::get_instance();

    QList<QObject *>    m_panelModel;
    int                 m_selectTab = 0;
    bool                m_oneCycle;
    bool                m_cycle;
    bool                m_isRun;
    bool                m_step;
    bool                m_isFront;
    bool                m_isBack;
    bool                m_autoRunning;
    const QList<int>    AXIS_3_POS_ADDR_LIST_1 = {2787, 2789, 2791};
    const QList<int>    AXIS_3_POS_ADDR_LIST_2 = {2813, 2815, 2817};
    const QList<int>    AXIS_3_POS_ADDR_LIST_3 = {2819, 2821, 2823};

    void initPanel();

signals:
    void selectTabChanged(int selectTab);
    void oneCycleChanged(bool oneCycle);
    void cycleChanged(bool cycle);
    void isRunChanged(bool isRun);
    void stepChanged(bool step);
    void isFrontChanged(bool isFront);
    void isBackChanged(bool isBack);
    void autoRunningChanged(bool autoRunning);

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onFinished();
};

#endif // AUTOVIEWMODEL_H
