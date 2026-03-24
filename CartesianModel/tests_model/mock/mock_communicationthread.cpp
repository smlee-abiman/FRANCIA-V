#include "mock_communicationthread.h"
#include <QMutexLocker>

Q_LOGGING_CATEGORY(sharedcontrol, "sharedcontrol")

CommunicationThread::CommunicationThread(QObject* parent) : QThread(parent)
{
    Q_UNUSED(parent)
}

void CommunicationThread::initialize()
{
}

void CommunicationThread::setModbusInfo(QString aplInfo, int portNo, int serverAddress, QString ipAddr)
{
    Q_UNUSED(aplInfo)
    Q_UNUSED(portNo)
    Q_UNUSED(serverAddress)
    Q_UNUSED(ipAddr)
}

void CommunicationThread::setIntervalMiliSec(int msec)
{
    Q_UNUSED(msec)
}

void CommunicationThread::setRequestPage(Enums::Pages page)
{
    Q_UNUSED(page)
}

void CommunicationThread::setModbusWriteCommand(stModBusCommunicationData &command)
{
    m_modbusWriteCommandList.append(command);
}

void CommunicationThread::setOneShotReadCommand(stModBusCommunicationData &command)
{
    m_oneShotReadCommandList.append(command);
}

void CommunicationThread::setOneShotModbusCaller(OneShotReadModBusCaller caller)
{
    oneShotReadCaller = caller;
}
void CommunicationThread::setOneShotModbusCommand(stModBusCommunicationData &command)
{
    m_oneShotModbusCommandList.append(command);
}

void CommunicationThread::run()
{
}

void CommunicationThread::onTimeout()
{
}

void CommunicationThread::appendCommonModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    Q_UNUSED(commandList)
}

void CommunicationThread::appendPageByPageModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    Q_UNUSED(commandList)
}

void CommunicationThread::appendOneShotReadCommand(QList<stModBusCommunicationData> *commandList)
{
    while(m_oneShotReadCommandList.size() > 0) {
        commandList->append(m_oneShotReadCommandList.takeFirst());
    }
}

void CommunicationThread::appendWriteModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    while(m_modbusWriteCommandList.size() > 0) {
        commandList->append(m_modbusWriteCommandList.takeFirst());
    }
}

void CommunicationThread::notifyOneShotModbusRead(stModBusCommunicationData command)
{
    Q_UNUSED(command)
}
void CommunicationThread::appendOneShotModbusCommand(QList<stModBusCommunicationData> *commandList)
{
    Q_UNUSED(commandList)
}

void CommunicationThread::onModBusComConnectStatusChanged(int state)
{
    Q_UNUSED(state)
}

void CommunicationThread::writeModbusDO(int doNumber, bool isOn)
{
    Q_UNUSED(doNumber)
    Q_UNUSED(isOn)
    m_writeModbusDOMap.insert(doNumber, isOn);
}

void CommunicationThread::writeModbusDO(int doNumber, bool isOn, uchar anyValue)
{
    Q_UNUSED(doNumber)
    Q_UNUSED(isOn)
    Q_UNUSED(anyValue)
    m_writeModbusDOucharMap.insert(doNumber, anyValue);
}

bool CommunicationThread::writeModbusHR(int HrNumber, int *write_data, int write_size)
{
    Q_UNUSED(HrNumber)
    Q_UNUSED(write_data)
    Q_UNUSED(write_size)
        return true;
}
bool CommunicationThread::writeModbusHR(int HrNumber, QString writeDataStr)
{
    Q_UNUSED(HrNumber)
    Q_UNUSED(writeDataStr)
    return true;
}

int CommunicationThread::getModbusConnectStatus()
{
    return m_modBusComConnectStatus;
}

void CommunicationThread::setPageReadCommandList(Enums::Pages page,
                                                 stModBusCommunicationData &command)
{
    Q_UNUSED(page);    
    m_pageReadCommandList.append(command);
}

void CommunicationThread::resetPageReadCommandList(Enums::Pages page)
{
    Q_UNUSED(page);
    m_pageReadCommandList.clear();
}


CommunicationThread* CommunicationThread::instance = nullptr;
