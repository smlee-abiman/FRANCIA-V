#ifndef IOMONITOR_H
#define IOMONITOR_H

#include "viewmodelbase.h"


class IOMonitorData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index CONSTANT)
    Q_PROPERTY(bool value READ value NOTIFY valueChanged)

public:
    explicit IOMonitorData(int index, QObject *parent = nullptr);
    ~IOMonitorData();

    void setValue(bool value);

    int index() const { return m_index; }
    bool value() const { return m_value; }

private:
    int m_index;
    bool m_value;

signals:
    void valueChanged(bool value);
};


class IOMonitorViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int pageNum READ pageNum WRITE setPageNum NOTIFY pageNumChanged)
    Q_PROPERTY(int pageMaxNum READ pageMaxNum CONSTANT)

public:
    IOMonitorViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~IOMonitorViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();

    Q_INVOKABLE QList<QObject*> getDataList(int page);

    void setPageNum(int pageNum);

    int pageNum() const { return m_pageNum; }
    int pageMaxNum() const { return m_pageMaxNum; }

private:
    ModbusManager       *m_modbusManager = ModbusManager::get_instance();
    AxisInfo            *m_axisInfo = AxisInfo::get_instance();
    IOMonitorData*      m_dataList[6][18];

    int                 m_pageNum;
    int                 m_pageMaxNum;
    const QList<int>    AXIS_3_POS_ADDR_LIST = {2813, 2815, 2817};

    void readPensetData();

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onFinished();

signals:
    void pageNumChanged(int pageNum);
};

#endif // IOMONITOR_H
