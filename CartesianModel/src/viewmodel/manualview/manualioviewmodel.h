#ifndef MANUALIOVIEWMODEL_H
#define MANUALIOVIEWMODEL_H

#include "viewmodelbase.h"

class InputData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index CONSTANT)
    Q_PROPERTY(bool value READ value NOTIFY valueChanged)

public:
    explicit InputData(int index, QObject *parent = nullptr);
    ~InputData();

    void setValue(bool value);

    int index() const { return m_index; }
    bool value() const { return m_value; }

private:
    int m_index;
    bool m_value;

signals:
    void valueChanged(bool value);
};

class OutData : public QObject
{
    Q_OBJECT    
    Q_PROPERTY(int keyNo READ keyNo CONSTANT)

public:
    explicit OutData(int keyNo, QObject *parent = nullptr);
    ~OutData();

    void setValue(bool value);

    int keyNo() const { return m_keyNo; }

private:
    int m_keyNo;

signals:
};

class OutDataSignal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int di READ di CONSTANT)
    Q_PROPERTY(bool value READ value NOTIFY valueChanged)
    Q_PROPERTY(QString onImage READ onImage CONSTANT)
    Q_PROPERTY(QString offImage READ offImage CONSTANT)

public:
    explicit OutDataSignal(int di, QString onImage, QString offImage, QObject *parent = nullptr);
    ~OutDataSignal();

    void setValue(bool value);

    int di() const { return m_di; }
    bool value() const { return m_value; }
    QString onImage() const { return m_onImage; }
    QString offImage() const { return m_offImage; }

private:
    int m_di;
    bool m_value;
    QString m_onImage;
    QString m_offImage;

signals:
    void valueChanged(bool value);
};

class ManualIOViewModel : public ViewModelBase
{
    Q_OBJECT
    // IO
    Q_PROPERTY(int inPageNum READ inPageNum WRITE setInPageNum NOTIFY inPageNumChanged)
    Q_PROPERTY(int inPageMaxNum READ inPageMaxNum CONSTANT)
    Q_PROPERTY(int outPageNum READ outPageNum WRITE setOutPageNum NOTIFY outPageNumChanged)
    Q_PROPERTY(int outPageMaxNum READ outPageMaxNum CONSTANT)

public:
    explicit ManualIOViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~ManualIOViewModel();

    const QList<int> AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void pageInit();
    Q_INVOKABLE QList<QObject*> getInDataList(int page);
    Q_INVOKABLE QList<QObject*> getOutDataList(int page);
    Q_INVOKABLE QList<QObject*> getOutDataSignalList(int page);

    void setInPageNum(int inPageNum);
    void setOutPageNum(int outPageNum);
    void setLanguage(int language);

    int inPageNum() const { return m_inPageNum; }
    int inPageMaxNum() const { return m_inPageMaxNum; }
    int outPageNum() const { return m_outPageNum; }
    int outPageMaxNum() const { return m_outPageMaxNum; }

private:
    ModbusManager       *m_modbusManager = ModbusManager::get_instance();
    AxisInfo            *m_axisInfo = AxisInfo::get_instance();
    InputData           *m_inDataList[5][18];
    OutData             *m_outDataList[12][10];
    OutDataSignal*      m_outDataSignalList[12][5];

    int                 m_inPageNum;
    int                 m_inPageMaxNum;
    int                 m_outPageNum;
    int                 m_outPageMaxNum;

    void readPensetData();

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onFinished();

signals:
    void inPageNumChanged(int inPageNum);
    void outPageNumChanged(int outPageNum);
};

#endif // MANUALIOVIEWMODEL_H
