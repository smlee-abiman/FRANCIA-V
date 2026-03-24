#include "operationhistoryviewmodel.h"
#include "operationhistorydata.h"
#include "operationhistorystring.h"
#include "operationhistorylist.h"
#include <QTextCodec>
#include <QFileInfo>

const QString OPERATION_HISTORY_CSV_FILE_NAME = "OPEHIS.csv";

OperationHistoryViewModel::OperationHistoryViewModel(ResourceManager *resource, CommunicationThread *comm, OperationHistoryList *operationHistoryList, FunctionViewModel *functionView)
    : HistoryView(resource, comm),
      m_operationHistoryList(operationHistoryList)
{
    m_string = new OperationHistoryString(resource, functionView, this);
}

OperationHistoryViewModel::OperationHistoryViewModel(QObject *parent)
    : HistoryView(nullptr, nullptr)
{
    Q_UNUSED(parent)
}

OperationHistoryViewModel::~OperationHistoryViewModel()
{
}

QString OperationHistoryViewModel::toOperateString(OperationHistoryData *data, int language)
{
    return m_string->toString(data, language);
}

void OperationHistoryViewModel::onFinished()
{
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

bool OperationHistoryViewModel::exportOperationDataToCsvFile(const QString &filePath)
{
    QFile file(filePath);
    if (!isUSBConnected() || !file.open(QIODevice::WriteOnly)) {
        qDebug() << __func__ << "failed." << filePath << isUSBConnected();
        return false;
    }
    QTextStream out(&file);
    out.setGenerateByteOrderMark(true);
    out.setCodec(QTextCodec::codecForMib(1015));
    writeHeaderForCsv(out);
    writeOperationDataForCsv(out);
    out.flush();
    file.close();
#ifdef REAL_TARGET
    int ret = system("sync");
    (void)ret;
#endif // REAL_TARGET
    m_operationRecord->recordOpeExportOpeHistory();
    return true;
}

void OperationHistoryViewModel::writeOperationDataForCsv(QTextStream &out)
{
    int language = m_resource->language();
    int number = 1;
    auto operationList = m_operationHistoryList->operationList();
    for (auto it = operationList.begin(), end = operationList.end(); it != end; ++it) {
        auto operationData = static_cast<OperationHistoryData *>(*it);
        out << number++;
        out << "\t";
        out << operationData->dateTime().toString("yyyy/MM/dd hh:mm:ss");
        out << "\t";
        out << m_string->toString(operationData, language);
        out << "\n";
    }
}

void OperationHistoryViewModel::onActivate()
{
    startUsbMonitoring();
    connect(m_communication, &CommunicationThread::finish, this, &OperationHistoryViewModel::onFinished);
}

void OperationHistoryViewModel::onDeactivate()
{
    stopFtpMonitoring();
    stopUsbMonitoring();
    disconnect(m_communication, &CommunicationThread::finish, this, &OperationHistoryViewModel::onFinished);
}

int OperationHistoryViewModel::onExportDataProc()
{
    QString path = historyDirPath() + OPERATION_HISTORY_CSV_FILE_NAME;
    bool ret = exportOperationDataToCsvFile(path);
    return ret ? 0 : -1;
}
