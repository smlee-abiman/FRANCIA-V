#ifndef ALARMHISTORYVIEWMODEL_H
#define ALARMHISTORYVIEWMODEL_H

#include "historyview.h"
#include "ftpmanager.h"
#include "commonviewmodel.h"

class AlarmHistoryDataParser;
class AlarmHistoryData;

class AlarmHistoryViewModel : public HistoryView
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> normalAlarmList READ normalAlarmList WRITE setNormalAlarmList NOTIFY normalAlarmListChanged)
    Q_PROPERTY(QList<QObject *> systemAlarmList READ systemAlarmList WRITE setSystemAlarmList NOTIFY systemAlarmListChanged)
    Q_PROPERTY(QList<QObject *> axisAlarmList READ axisAlarmList WRITE setAxisAlarmList NOTIFY axisAlarmListChanged)
    Q_PROPERTY(bool isFtpFileAcquired MEMBER m_isFtpFileAcquired NOTIFY isFtpFileAcquiredChanged)

public:
    explicit AlarmHistoryViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    explicit AlarmHistoryViewModel(QObject *parent = nullptr);
    ~AlarmHistoryViewModel() override;
    Q_INVOKABLE QString getAxisName(int unitNo);
    QList<QObject *> normalAlarmList() { return m_normalAlarmList; }
    QList<QObject *> systemAlarmList() { return m_systemAlarmList; }
    QList<QObject *> axisAlarmList() { return m_axisAlarmList; }
    void setNormalAlarmList(QList<QObject *> &list);
    void setSystemAlarmList(QList<QObject *> &list);
    void setAxisAlarmList(QList<QObject *> &list);

signals:
    void normalAlarmListChanged();
    void systemAlarmListChanged();
    void axisAlarmListChanged();
    void isFtpFileAcquiredChanged();

private slots:
    void onAlarmOccurred(const AlarmInfo &alarmInfo, const QDateTime &dateTime);
    void onFtpFileInfoChanged(int ftpFileInfo);
    void onPseudoFtpGetFinished();
    void onFinished();

private:
    void onActivate() override;
    void onDeactivate() override;
    void getFtpFileList();
    void getFtpFileInfo(FtpManager::FtpFileInfo fileInfo);
    void getFtpFileData();
    void divideAlarmList();
    void divideAlarmData(AlarmHistoryData *data);
    void removeOldestAlarmData();
    bool exportAlarmDataToCsvFile(const QString &filePath);
    void writeAlarmDataForCsv(QTextStream &out);
    QString getAlarmDetailString(AlarmHistoryData *alarmData);
    void writeAllAlarmDataToFile();
    void writeAlarmDataToFile(AlarmHistoryData *alarmData);
    int readAlarmDataFileFromPendant();
    void initAxisLabelIndexList();
    int onExportDataProc() override;
    void setIsFtpFileAcquired(bool acquired);

private:
    AlarmHistoryDataParser      *m_parser;
    CommonViewModel             *m_commonView = nullptr;
    AxisInfo                    *m_axisInfo = AxisInfo::get_instance();

    QList<AlarmHistoryData *>   m_alarmList;
    QList<QObject *>            m_normalAlarmList;
    QList<QObject *>            m_systemAlarmList;
    QList<QObject *>            m_axisAlarmList;
    int                         m_getFileCount = 0;
    QFile                       m_alarmHistoryFile;
    QList<int>                  m_axisLabelIndexList = {-1,-1,-1,-1,-1,-1,-1,-1};
    bool                        m_isFtpFileAcquired = false;
    const QList<int>            AXIS_3_POS_ADDR_LIST = {2795, 2797, 2799};
};

#endif // ALARMHISTORYVIEWMODEL_H
