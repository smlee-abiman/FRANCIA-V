#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include "viewmodelbase.h"
#include "usbmanager.h"
#include "ftpmanager.h"

class HistoryView : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(bool isUSBConnected READ isUSBConnected WRITE setIsUSBConnected NOTIFY isUSBConnectedChanged)
    Q_PROPERTY(bool isExportProcessing MEMBER m_isExportProcessing NOTIFY isExportProcessingChanged)
    Q_PROPERTY(bool exportResult MEMBER m_exportResult NOTIFY exportResultChanged)

public:
    explicit HistoryView(ResourceManager *resource, CommunicationThread * comm);
    Q_INVOKABLE bool exportData();
    void setIsUSBConnected(bool isUSBConnected);
    bool isUSBConnected() const { return m_isUSBConnected; }

protected:
    void startUsbMonitoring();
    void stopUsbMonitoring();
    bool copyFileToUsb(QString srcFilePath, QString dstFilePath, bool isOverWrite);
    bool createFolder(const QString &folderPath);

    void startFtpMonitoring();
    void stopFtpMonitoring();
    virtual void startFtpCommunication();
    virtual void stopFtpCommunication();
    void getFtpFileList(const QString &path);
    void getFtpFileInfo(FtpManager::FtpFileInfo fileInfo);
    virtual void ftpStatusChanged(int ftpStatus);
    virtual void ftpGetFinished(const QString &filePath);
    QString usbMountPath() const;
    QString historyDirPath() const;
    void writeHeaderForCsv(QTextStream &out);
    void timerEvent(QTimerEvent *timerEvent) override;
    void setIsExportProcessing(bool status);
    void setExportResult(int result);
    virtual int onExportDataProc();

signals:
    void isUSBConnectedChanged(bool isUSBConnected);
    void isExportProcessingChanged();
    void exportResultChanged();

public slots:
    void onUsbMounted(QString usbMountPath);
    void onUsbUnmounted();
    void onFtpStatusChanged(int ftpStatus);
    void onFtpGetFinished(const QString &filePath);

protected:
    FtpManager              *m_ftpManager;
    UsbManager              *m_usbManager;
    bool                    m_isUsbMonitor = false;
    bool                    m_isUSBConnected = false;
    int                     m_timerId = 0;
    bool                    m_isExportProcessing = false;
    int                     m_exportResult = 0;
};

#endif // HISTORYVIEW_H
