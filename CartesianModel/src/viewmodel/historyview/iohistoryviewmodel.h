#ifndef IOHISTORYVIEWMODEL_H
#define IOHISTORYVIEWMODEL_H

#include "historyview.h"

class IoHistoryDataParser;

class IoHistoryViewModel: public HistoryView
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> ioHistoryDataList MEMBER m_ioHistoryDataList NOTIFY ioHistoryDataListChanged)
    Q_PROPERTY(QList<int> displayIoAddressList MEMBER m_displayIoAddressList NOTIFY displayIoAddressListChanged)
    Q_PROPERTY(QList<QObject *> ioList READ ioList WRITE setIoList NOTIFY ioListChanged)
    Q_PROPERTY(int bankNo MEMBER m_bankNo NOTIFY bankNoChanged)
    Q_PROPERTY(int bankType MEMBER m_bankType NOTIFY bankTypeChanged)
    Q_PROPERTY(QList<bool> bankDataType MEMBER m_bankDataType NOTIFY bankDataTypeChanged)
    Q_PROPERTY(bool isFtpFileAcquired MEMBER m_isFtpFileAcquired NOTIFY isFtpFileAcquiredChanged)
public:
    explicit IoHistoryViewModel(ResourceManager *resource, CommunicationThread * comm);
    explicit IoHistoryViewModel(QObject *parent = nullptr);
    ~IoHistoryViewModel() override;
    Q_INVOKABLE bool isIoStatusOn(int ioHistoryIndex, int diAddress);
    Q_INVOKABLE QList<int> getDisplayIoAddressList(int page, int numItems);
    Q_INVOKABLE QString toIoChangeString(int index);
    QList<QObject *> ioList(){return m_ioList; }

private:
    void onActivate() override;
    void onDeactivate() override;
    void readPensetData();
    void readPensetDisplayIoAddressList();
    void createIoOnList(int index);
    void createIoChangeList(int index);
    void getFtpFileData();
    bool exportIoDataToCsvFile(const QString &filePath);
    void writeIoDataForCsv(QTextStream &out);
    void ftpGetFinished(const QString &filePath) override;
    int onExportDataProc() override;
    void setIsFtpFileAcquired(bool acquired);
    void timerEvent(QTimerEvent *timerEvent) override;

signals:
    void ioHistoryDataListChanged();
    void displayIoAddressListChanged();
    void ioListChanged();
    void bankDataTypeChanged();
    void bankNoChanged();
    void bankTypeChanged();
    void isFtpFileAcquiredChanged();

private slots:
    void setIoList(QList<QObject *> &list);
    void onIsStartupFtpGetFileListChanged(bool isStartupFtpGetFileListChanged);
    void onPseudoFtpGetFinished();
    void onFinished();

private:
    IoHistoryDataParser     *m_parser;
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();

    QList<QObject *>        m_ioHistoryDataList;
    QList<int>              m_displayIoAddressList;
    QList<QObject *>        m_ioList;
    QList<bool>             m_bankDataType;
    int                     m_bankNo = 0;
    int                     m_bankType = 0;
    bool                    m_isFtpFileAcquired = false;
    int                     m_ftpGetTimeoutTimerId = 0;
    const QList<int>        AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};
};

#endif // IOHISTORYVIEWMODEL_H
