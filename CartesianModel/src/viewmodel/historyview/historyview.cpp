#include "historyview.h"

HistoryView::HistoryView(ResourceManager *resource, CommunicationThread * comm) :
    ViewModelBase(resource, comm)
{
    m_ftpManager = FtpManager::get_instance();
    m_usbManager = UsbManager::get_instance();
}

bool HistoryView::exportData()
{
    bool ret = createFolder(historyDirPath());
    if (ret) {
        m_timerId = startTimer(1);
        setIsExportProcessing(true);
    }
    return ret;
}

void HistoryView::startFtpMonitoring()
{
    connect(m_ftpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)), Qt::QueuedConnection);
    connect(m_ftpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)), Qt::QueuedConnection);
}

void HistoryView::stopFtpMonitoring()
{
    disconnect(m_ftpManager, SIGNAL(ftpGetFinished(const QString &)), this, SLOT(onFtpGetFinished(const QString &)));
    disconnect(m_ftpManager, SIGNAL(ftpStatusChanged(int)), this, SLOT(onFtpStatusChanged(int)));
}


void HistoryView::startFtpCommunication()
{
    if (!m_ftpManager) {
        qDebug("%s:%d m_ftpManager is not share", __func__, __LINE__);
        return;
    }
    m_ftpManager->startFtpCommunication();
}

void HistoryView::stopFtpCommunication()
{
    if (!m_ftpManager) {
        qDebug("%s:%d m_ftpManager is not share", __func__, __LINE__);
        return;
    }
    m_ftpManager->stopFtpCommunication();
}

void HistoryView::getFtpFileList(const QString &path)
{
    if (!m_ftpManager) {
        qDebug("%s:%d m_ftpManager is not share", __func__, __LINE__);
        return;
    }
    m_ftpManager->requestFtpListCommand(path);
}

void HistoryView::getFtpFileInfo(FtpManager::FtpFileInfo fileInfo)
{
    if (!m_ftpManager) {
        qDebug("%s:%d m_ftpManager is not share", __func__, __LINE__);
        return;
    }
    m_ftpManager->getFtpFileInfo(fileInfo);
}

void HistoryView::onFtpStatusChanged(int ftpStatus)
{
    ftpStatusChanged(ftpStatus);
}

void HistoryView::onFtpGetFinished(const QString &filePath)
{
    ftpGetFinished(filePath);
}

void HistoryView::ftpGetFinished(const QString &filePath)
{
    Q_UNUSED(filePath)
}

void HistoryView::ftpStatusChanged(int ftpStatus)
{
    Q_UNUSED(ftpStatus)
}

void HistoryView::startUsbMonitoring()
{
    if (!m_usbManager){
        qDebug("%s:%d UsbManager is invalid", __func__, __LINE__);
        return;
    }
    setIsUSBConnected(m_usbManager->getCurUsbMountStatus());
    connect(m_usbManager, SIGNAL(usbMounted(QString)), this, SLOT(onUsbMounted(QString)));
    connect(m_usbManager, SIGNAL(usbUnmounted()), this, SLOT(onUsbUnmounted()));
    m_isUsbMonitor = true;
    m_usbManager->timerStart(100);
}

void HistoryView::stopUsbMonitoring()
{
    m_usbManager->timerStop();
    m_isUsbMonitor = false;
    disconnect(m_usbManager, SIGNAL(usbMounted(QString)), this, SLOT(onUsbMounted(QString)));
    disconnect(m_usbManager, SIGNAL(usbUnmounted()), this, SLOT(onUsbUnmounted()));
    setIsUSBConnected(false);
}

bool HistoryView::copyFileToUsb(QString srcFilePath, QString dstFilePath, bool isOverWrite)
{
    if (!isUSBConnected()) {
        qWarning() << __func__ << "USB is not connected." << srcFilePath << dstFilePath;
        return false;
    }
    bool ret = m_usbManager->copyFile(srcFilePath, dstFilePath, isOverWrite);
    if (!ret) {
        qWarning() << __func__ << "Failed to copy the file." << srcFilePath << dstFilePath;
    }
    return ret;
}

bool HistoryView::createFolder(const QString &folderPath)
{
    if (!isUSBConnected()) {
        qWarning() << __func__ << "USB is not connected." << folderPath << m_usbManager->getCurUsbMountStatus();
        return false;
    }
    return m_usbManager->createFolder(folderPath);
}

void HistoryView::onUsbMounted(QString usbMountPath)
{
    (void)usbMountPath;
    setIsUSBConnected(true);
}

void HistoryView::onUsbUnmounted()
{
    setIsUSBConnected(false);
}

void HistoryView::setIsUSBConnected(bool isUSBConnected)
{
    if (m_isUSBConnected != isUSBConnected) {
        m_isUSBConnected = isUSBConnected;
        emit isUSBConnectedChanged(m_isUSBConnected);
    }
}

QString HistoryView::usbMountPath() const
{
    if (!m_usbManager) {
        return "";
    }
    return m_usbManager->mountPath();
}

QString HistoryView::historyDirPath() const
{
    return usbMountPath() + "/Rireki/";
}

void HistoryView::writeHeaderForCsv(QTextStream &out)
{
    for (int id = 4; id <= 6; id++) {
        out << m_resource->label(50, id);
        out << "\t";
    }
    out << "\n";
}

void HistoryView::timerEvent(QTimerEvent *timerEvent)
{
    if (timerEvent->timerId() == m_timerId) {
        setExportResult(onExportDataProc());
        setIsExportProcessing(false);
        killTimer(m_timerId);
    }
}

int HistoryView::onExportDataProc()
{
    return 0;
}

void HistoryView::setIsExportProcessing(bool status)
{
    if (m_isExportProcessing != status) {
        m_isExportProcessing = status;
        emit isExportProcessingChanged();
    }
}

void HistoryView::setExportResult(int result)
{
    if (m_exportResult != result) {
        m_exportResult = result;
        emit exportResultChanged();
    }
}
