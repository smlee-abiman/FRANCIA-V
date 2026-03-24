#ifndef OPERATIONHISTORYVIEWMODEL_H
#define OPERATIONHISTORYVIEWMODEL_H

#include "historyview.h"

class OperationHistoryString;
class OperationHistoryData;
class FunctionViewModel;

class OperationHistoryViewModel : public HistoryView
{
    Q_OBJECT
public:
    explicit OperationHistoryViewModel(ResourceManager *resource, CommunicationThread *comm, OperationHistoryList *operationHistoryList, FunctionViewModel *functionView);
    explicit OperationHistoryViewModel(QObject *parent = nullptr);
    ~OperationHistoryViewModel() override;
    Q_INVOKABLE QString toOperateString(OperationHistoryData *data, int language);

signals:
    void operationListChanged();

public slots:
    void onFinished();

private:
    void onActivate() override;
    void onDeactivate() override;
    bool exportOperationDataToCsvFile(const QString &filePath);
    void writeOperationDataForCsv(QTextStream &out);
    int onExportDataProc() override;

private:
    OperationHistoryList        *m_operationHistoryList = nullptr;
    OperationHistoryString      *m_string = nullptr;
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();

    const QList<int>            AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};
};

#endif // OPERATIONHISTORYVIEWMODEL_H
