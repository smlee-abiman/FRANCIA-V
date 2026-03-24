#ifndef DRIVERPARAMETERSVIEWMODEL_H
#define DRIVERPARAMETERSVIEWMODEL_H

#include "viewmodelbase.h"
#include <QQmlEngine>
#include <QtMath>

#define MC_DPVM_DATA_TOP            54878
#define MC_DPVM_DATA_END            59677
#define MC_DPVM_DATA_SIZE           125
#define MC_DPVM_MODEL_SR_TOP        2761

#define MC_DPVM_SAVE_HR             1657

#define MC_DPVM_PAGE_MAX    11
#define MC_DPVM_ROW_MAX     7
#define MC_DPVM_AXIS_MAX    8

class DrvPrmData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long index READ index CONSTANT)
    Q_PROPERTY(QString min READ min CONSTANT)
    Q_PROPERTY(QString max READ max CONSTANT)
    Q_PROPERTY(short scale READ scale CONSTANT)
    Q_PROPERTY(long format READ format CONSTANT)
    Q_PROPERTY(short tenkeyType READ tenkeyType CONSTANT)
    Q_PROPERTY(long defValue READ defValue  NOTIFY defValueChanged)
    Q_PROPERTY(long value READ value NOTIFY valueChanged)
    Q_PROPERTY(bool isChange READ isChange NOTIFY isChangeChanged)

public:
    explicit DrvPrmData(long index, QString min, QString max, short scale, long format, short tenkeyType, QObject *parent = nullptr);
    ~DrvPrmData();

    void setDefValue(long defValue);
    void setValue(long value, bool firstDataLoaded);
    void setIsChange(bool isChange);

    long index() const { return m_index; }
    QString min() const { return m_min; }
    QString max() const { return m_max; }
    short scale() const { return m_scale; }
    long format() const { return m_format; }
    short tenkeyType() const { return m_tenkeyType; }
    long defValue() const { return m_defValue; }
    long value() const { return m_value; }
    bool isChange() const { return m_isChange; }

private:
    long m_index;
    QString m_min;
    QString m_max;
    short m_scale;
    long m_format;
    short m_tenkeyType;
    long m_defValue;
    long m_value;
    bool m_isChange;

signals:
    void defValueChanged(long defValue);
    void valueChanged(long value);
    void isChangeChanged(bool isChange);
};

class DriverParametersViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int pageNum READ pageNum WRITE setPageNum NOTIFY pageNumChanged)
    Q_PROPERTY(int pageMaxNum READ pageMaxNum CONSTANT)
    Q_PROPERTY(int axisPageNum READ axisPageNum WRITE setAxisPageNum NOTIFY axisPageNumChanged)
    Q_PROPERTY(int axisPageMaxNum READ axisPageMaxNum CONSTANT)

    Q_PROPERTY(QString modelName READ modelName NOTIFY modelNameChanged)
    Q_PROPERTY(int saveStatus READ saveStatus WRITE setSaveStatus NOTIFY saveStatusChanged)

public:
    explicit DriverParametersViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~DriverParametersViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void pageInit();
    Q_INVOKABLE void setValue(int row, int axis, double value);
    Q_INVOKABLE void initParameter();
    Q_INVOKABLE void saveParameter();
    Q_INVOKABLE QObject* getData(int page, int row, int axis);

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
    DrvPrmData                  *m_dataList[MC_DPVM_PAGE_MAX][MC_DPVM_ROW_MAX][MC_DPVM_AXIS_MAX];
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();

    int                         m_pageNum;
    int                         m_pageMaxNum;
    int                         m_axisPageNum;
    int                         m_axisPageMaxNum;
    int                         m_saveStatus;
    QString                     m_modelName;
    bool                        m_firstDataLoadedFlag;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2819, 2821, 2823};

    void readParameter();
    void readPensetData();
    void firstDataLoad();
    void updateData();

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onFinished();
    void onReadParameterFinished();
    void onDataSaveFinished();
    void onDataInitFinished();

signals:
    void pageNumChanged(int pageNum);
    void axisPageNumChanged(int pageNum);
    void saveStatusChanged(int saveStatus);
    void modelNameChanged(QString modelName);
};

#endif // DRIVERPARAMETERSVIEWMODEL_H
