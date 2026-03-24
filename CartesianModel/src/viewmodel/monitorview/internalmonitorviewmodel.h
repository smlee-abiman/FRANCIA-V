#ifndef INTERNALMONITORVIEWMODEL_H
#define INTERNALMONITORVIEWMODEL_H

#include "viewmodelbase.h"

#define INTERNAL_PERIODIC_MONITOR_CMD_LIST_NUM                 1
#define INTERNAL_MONITOR_BANK_NO_NUM                           5
#define INTERNAL_MONITOR_BANK_CONTENTS_NUM                     2
#define INTERNAL_BANK_OFFSET_NUMBER_HR                      2526
#define INTERNAL_BANK_IO_TOP_SR                             2761

class InternalMonitorData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool value READ value NOTIFY valueChanged)

public:
    explicit InternalMonitorData(QObject *parent = nullptr);
    ~InternalMonitorData();

    void setValue(bool value);

    bool value() const { return m_value; }

private:
    bool m_value;

signals:
    void valueChanged(bool value);
};

class InternalMonitorViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int curBankNo READ curBankNo WRITE setCurBankNo NOTIFY curBankNoChanged)
    Q_PROPERTY(int curBankContents READ curBankContents WRITE setCurBankContents NOTIFY curBankContentsChanged)
    Q_PROPERTY(QList<QObject *> bankIOList READ bankIOList WRITE setBankIOList NOTIFY bankIOListChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
    Q_PROPERTY(QList<int> bankIOByteList READ bankIOByteList NOTIFY bankIOByteListChanged)

public:
    InternalMonitorViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~InternalMonitorViewModel();

    Q_INVOKABLE void startPageChanging();
    Q_INVOKABLE int getBankOffsetNum();

    void setCurBankNo(int curBankNo);
    int curBankNo() const { return m_curBankNo;}
    void setCurBankContents(int curBankContents);
    int curBankContents() const { return m_curBankContents;}
    void setBankIOList(QList<QObject *>& bankIOList);
    QList<QObject *> bankIOList() const { return m_bankIOList; }
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }
    QList<int> bankIOByteList(){ return m_bankIOByteList; }

protected:
    void onActivate() override;
    void onDeactivate() override;

private:
    AxisInfo            *m_axisInfo = AxisInfo::get_instance();
    int                 m_curBankNo = 0;
    int                 m_curBankContents = 0;
    QList<QObject *>    m_bankIOList;
    QList<int>          m_bankIOByteList = {1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9};
    bool                m_pageChanging = false;
    bool                m_dataLoaded = false;
    const QList<int>    AXIS_3_POS_ADDR_LIST = {2813, 2815, 2817};

    stModbusPeridiocMonitor initModbusPageCmd[INTERNAL_PERIODIC_MONITOR_CMD_LIST_NUM] = {
        { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0000, 0x0100 },        // Bank0-IO    FuncCode:0x02 addr:DI000 size:256
    };
    stModbusPeridiocMonitor bankIOModbusPageCmd[INTERNAL_MONITOR_BANK_NO_NUM][INTERNAL_MONITOR_BANK_CONTENTS_NUM] = {
        //  Bank0-IO FuncCode:0x02 addr:DI000 size:256                        Bank0-M FuncCode:0x02 addr:DI256 size:256
        { { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0000, 0x0100 }, { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0100, 0x0100 }},
        //  Bank1-IO FuncCode:0x02 addr:DI512 size:256                        Bank1-M FuncCode:0x02 addr:DI768 size:256
        { { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0200, 0x0100 }, { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0300, 0x0100 }},
        //  Bank2-IO FuncCode:0x02 addr:DI1024 size:256                       Bank2-M FuncCode:0x02 addr:DI1280 size:256
        { { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0400, 0x0100 }, { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0500, 0x0100 }},
        //  Bank3-IO FuncCode:0x02 addr:DI1536 size:256                       Bank3-M FuncCode:0x02 addr:DI1792 size:256
        { { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0600, 0x0100 }, { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0700, 0x0100 }},
        //  Bank4-IO FuncCode:0x02 addr:DI2048 size:256                       Bank4-M FuncCode:0x02 addr:DI2304 size:256
        { { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0800, 0x0100 }, { QModbusPdu::FunctionCode::ReadDiscreteInputs, 0x0900, 0x0100 }},
    };
    int bankOffsetNumber[INTERNAL_MONITOR_BANK_NO_NUM][INTERNAL_MONITOR_BANK_CONTENTS_NUM] = {
        {   0,      1 },
        {   2,      3 },
        {   4,      5 },
        {   6,      7 },
        {   8,      9 },
    };

    void initMonitorData();
    void initPageCommand();
    void deinitPageCommand();
    void updatePageCommand();
    void startModbusMonitoring();
    void stopModbusMonitoring();

public slots:
    void onFinished();
    void onEndModbusPageWriteReady(QList<int> writeData);

signals:
    void curBankNoChanged(int curBankNo);
    void curBankContentsChanged(int curBankContents);
    void bankIOListChanged(QList<QObject *> bankIOList);
    void dataLoadedChanged(bool dataLoaded);
    void bankIOByteListChanged(QList<int> data);
};

#endif // INTERNALMONITORVIEWMODEL_H
