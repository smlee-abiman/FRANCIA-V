#ifndef MODELSELECTVIEWMODEL_H
#define MODELSELECTVIEWMODEL_H

#include "viewmodelbase.h"

#define MC_MSVM_MODEL_SR            0xF0A6
#define MC_MSVM_MODEL_SIZE          0x0100
#define MC_MSVM_MAKER_SR            0xF05E
#define MC_MSVM_MAKER_SIZE          0x0040
#define MC_MSVM_CAPACITY_SR_TOP     2785
#define MC_MSVM_ENCODERTYPE_SR_TOP  2793
#define MC_MSVM_STROKE_SR_TOP       2769
#define MC_MSVM_MODEL_SR_TOP        2761

#define MC_MSVM_MODEL_SET1_HR       1952
#define MC_MSVM_MODEL_SET2_HR       1657

class ModelAxisData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString makerName READ makerName WRITE setMakerName NOTIFY makerNameChanged)
    Q_PROPERTY(int capacity READ capacity WRITE setCapacity NOTIFY capacityChanged)
    Q_PROPERTY(int encoderType READ encoderType WRITE setEncoderType NOTIFY encoderTypeChanged)
    Q_PROPERTY(long stroke READ stroke WRITE setStroke NOTIFY strokeChanged)

public:
    explicit ModelAxisData(QObject *parent = nullptr);
    ~ModelAxisData();

    void setMakerName(QString makerName);
    void setCapacity(int capacity);
    void setEncoderType(int encoderType);
    void setStroke(long stroke);

    QString makerName() const { return m_makerName; }
    int capacity() const { return m_capacity; }
    int encoderType() const { return m_encoderType; }
    long stroke() const { return m_stroke; }

private:
    QString m_makerName;
    int m_capacity;
    int m_encoderType;
    long m_stroke;

signals:
    void makerNameChanged(QString makerName);
    void capacityChanged(int capacity);
    void encoderTypeChanged(int encoderType);
    void strokeChanged(long long stroke);
};

class ModelSelectViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int pageNum READ pageNum WRITE setPageNum NOTIFY pageNumChanged)
    Q_PROPERTY(int pageMaxNum READ pageMaxNum CONSTANT)

    Q_PROPERTY(QString modelName READ modelName NOTIFY modelNameChanged)
    Q_PROPERTY(int selectedModelIdx READ selectedModelIdx NOTIFY selectedModelIdxChanged)

    Q_PROPERTY(QStringList modelList READ modelList NOTIFY modelListChanged)
    Q_PROPERTY(int saveStatus READ saveStatus WRITE setSaveStatus NOTIFY saveStatusChanged)

    Q_PROPERTY(ModelAxisData* axisData1 READ axisData1 CONSTANT)
    Q_PROPERTY(ModelAxisData* axisData2 READ axisData2 CONSTANT)
    Q_PROPERTY(ModelAxisData* axisData3 READ axisData3 CONSTANT)
    Q_PROPERTY(ModelAxisData* axisData4 READ axisData4 CONSTANT)
    Q_PROPERTY(ModelAxisData* axisData5 READ axisData5 CONSTANT)
    Q_PROPERTY(ModelAxisData* axisData6 READ axisData6 CONSTANT)
    Q_PROPERTY(ModelAxisData* axisData7 READ axisData7 CONSTANT)
    Q_PROPERTY(ModelAxisData* axisData8 READ axisData8 CONSTANT)

    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)

public:
    explicit ModelSelectViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~ModelSelectViewModel();

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void startPageChanging();
    Q_INVOKABLE void pageInit();
    Q_INVOKABLE void saveModelType(int modelIdx);
    Q_INVOKABLE QObject* getData(int index) { return m_dataList[index]; };

    void setPageNum(int pageNum);
    void setSaveStatus(int saveStatus);
    void setModelName(QString modelName);
    void setSelectedModelIdx(int selectedModelIdx);
    void setDataLoaded(bool dataLoaded);

    int pageNum() const { return m_pageNum; }
    int pageMaxNum() const { return m_pageMaxNum; }
    QStringList modelList() const { return m_modelList; }
    int saveStatus() const { return m_saveStatus; }
    QString modelName() const { return m_modelName; }
    int selectedModelIdx() const { return m_selectedModelIdx; }

    ModelAxisData* axisData1() const { return m_dataList[0]; }
    ModelAxisData* axisData2() const { return m_dataList[1]; }
    ModelAxisData* axisData3() const { return m_dataList[2]; }
    ModelAxisData* axisData4() const { return m_dataList[3]; }
    ModelAxisData* axisData5() const { return m_dataList[4]; }
    ModelAxisData* axisData6() const { return m_dataList[5]; }
    ModelAxisData* axisData7() const { return m_dataList[6]; }
    ModelAxisData* axisData8() const { return m_dataList[7]; }

    bool dataLoaded() { return m_dataLoaded; }

private:
    ModbusManager               *m_modbusManager;
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();

    int                         m_pageNum;
    int                         m_pageMaxNum;
    QString                     m_modelName;
    int                         m_selectedModelIdx;
    int                         m_saveStatus;
    QStringList                 m_modelList;
    ModelAxisData               *m_dataList[8];
    bool                        m_pageChanging = false;
    bool                        m_dataLoaded = false;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2811, 2813, 2815};

protected:
    void onActivate() override;
    void onDeactivate() override;

private slots:
    void onFinished();
    void onModelListFinished();
    void onModelChangeFinished();
    void endModbusPageChange(int mainScreenId, int subScreenId);

signals:
    void pageNumChanged(int pageNum);
    void saveStatusChanged(int saveStatus);
    void modelListChanged(QStringList modelList);
    void modelNameChanged(QString modelName);
    void selectedModelIdxChanged(int selectedModelIdx);
    void dataLoadedChanged(bool dataLoaded);
};

#endif // MODELSELECTVIEWMODEL_H
