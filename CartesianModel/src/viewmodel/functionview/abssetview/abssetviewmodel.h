#ifndef ABSSETVIEWMODEL_H
#define ABSSETVIEWMODEL_H

#include "viewmodelbase.h"
#include "commonviewmodel.h"
#include <QQmlEngine>

class AbsData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int encoder READ encoder NOTIFY encoderChanged)
    Q_PROPERTY(bool bm READ bm NOTIFY bmChanged)
    Q_PROPERTY(bool alert READ alert NOTIFY alertChanged)
    Q_PROPERTY(short resetKeyNo READ resetKeyNo CONSTANT)
    Q_PROPERTY(short setKeyNo READ setKeyNo CONSTANT)

public:
    explicit AbsData(short resetKeyNo, short setKeyNo, QObject *parent = nullptr);
    ~AbsData();

    void setEncoder(int encoder);
    void setBm(bool bm);
    void setAlert(bool alert);

    int encoder() const { return m_encoder; }
    bool bm() const { return m_bm; }
    bool alert() const { return m_alert; }
    short resetKeyNo() const { return m_resetKeyNo; }
    short setKeyNo() const { return m_setKeyNo; }

private:
    int m_encoder;
    bool m_bm;
    bool m_alert;
    short m_resetKeyNo;
    short m_setKeyNo;

signals:
    void encoderChanged(int encoder);
    void bmChanged(bool bm);
    void alertChanged(bool alert);
};

class AbsSetViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int pageNum READ pageNum WRITE setPageNum NOTIFY pageNumChanged)
    Q_PROPERTY(int pageMaxNum READ pageMaxNum CONSTANT)

public:
    explicit AbsSetViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    ~AbsSetViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE QList<QObject*> getDataList(int page);
    Q_INVOKABLE QList<QObject*> getDataAllList();

    void setPageNum(int pageNum);
    void setAxisPageNum(int axisPageNum);

    int pageNum() const { return m_pageNum; }
    int pageMaxNum() const { return m_pageMaxNum; }

private:
    ModbusManager           *m_modbusManager;
    CommonViewModel         *m_commonView;
    AbsData                 *m_dataList[8];
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();

    int                     m_pageNum;
    int                     m_pageMaxNum;
    const QList<quint16>    abs_reset_key = {81, 82, 84, 83, 85, 86, 87, 88};
    const QList<quint16>    abs_set_key = {89, 90, 92, 91, 93, 94, 95, 96};
    const QList<int>        AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onFinished();

signals:
    void pageNumChanged(int pageNum);
};

#endif // ABSSETVIEWMODEL_H
