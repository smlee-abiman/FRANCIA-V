#include "programmenu3_2.h"
#include "../command/ncpcommandwait.h"
#include <cmath>
#include <QRegularExpression>

const int PEN_SET_DELAY_TIMER_DIDO_ADDR_TOP_INDEX = 33;

ProgramMenu3_2::ProgramMenu3_2(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
    initSettingPenSetData();
    initTimerList();
    initTimerValueList();
    modbusReadTimerItemValues();
}

void ProgramMenu3_2::initSettingPenSetData()
{
    initTimerMenuList();
}

void ProgramMenu3_2::initTimerList()
{
    for(int i = 0; i < PROG_MENU3_2_ONE_PAGE_ITEM_MAX; i++){
        //m_timerDataList.append("");
        m_timerDataList.append(0);
    }
    emit timerDataListChanged(m_timerDataList);
}

void ProgramMenu3_2::onActivate()
{
    //qDebug("ProgramMenu3_2 %s:%d", __func__, __LINE__);
    startModbusMonitoring();
}

void ProgramMenu3_2::onDeactivate()
{
    //qDebug("ProgramMenu3_2 %s:%d", __func__, __LINE__);
    stopModbusMonitoring();
}

void ProgramMenu3_2::startModbusMonitoring()
{
    if (!m_communication) return;
    connect(m_communication, &CommunicationThread::feed,
        this, &ProgramMenu3_2::updateProgramMenu, Qt::QueuedConnection);
}

void ProgramMenu3_2::stopModbusMonitoring()
{
    if (!m_communication) return;
    disconnect(m_communication, &CommunicationThread::feed,
        this, &ProgramMenu3_2::updateProgramMenu);
}

void ProgramMenu3_2::modbusReadTimerItemValues()
{
    if (!m_communication) return;
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters,
        TIMER_TOP_ITEM_ADDR_LIST.at(0),
        PROG_MENU3_2_TIMER_ITEM_VALUE_MAX);
    m_communication->setOneShotModbusCommand(readCommand);
}

void ProgramMenu3_2::initTimerMenuList()
{
    if (!m_resource) return;
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 8; j++) {
            int index = m_resource->getPenSetStringValue("16", (j + PEN_SET_DELAY_TIMER_DIDO_ADDR_TOP_INDEX), i).toInt();
            if (index == 0){
                continue;
            }
            //qDebug() << "index:" << index << "symbolAt:" << m_resource->symbolAt(index);
            if(m_resource->getPenSetStringValue("16", (j + PEN_SET_DELAY_TIMER_DIDO_ADDR_TOP_INDEX), i) != NULL){
                m_timerAddr.append(index);
            }else {
                continue;
            }
        }
    }
#if 0
    qDebug("%s:%d m_timerAddr count[%d]", __func__, __LINE__, m_timerAddr.count());
    for(int i = 0; i < m_timerAddr.count(); i++){
        qDebug() << "m_timerAddr:" << m_timerAddr.at(i);
    }
#endif
}

void ProgramMenu3_2::initTimerValueList()
{
    for(int i = 0; i < PROG_MENU3_2_TIMER_ITEM_VALUE_MAX; i++){
        mTimerValues.append("");
        mTimerValueKeepStatus.append(false);
    }
}

void ProgramMenu3_2::clearTimerValueKeepStatus(int page)
{
    int startIndex = (page - 1) * PROG_MENU3_2_ONE_PAGE_ITEM_MAX;
    int endIndex = (page - 1) * PROG_MENU3_2_ONE_PAGE_ITEM_MAX + TIMER_ITEM_ADDR_DATA_SIZE_LIST.at(page - 1);
    //qDebug() << "ProgramMenu3_2::clearTimerValueKeepStatus startIndex:" << startIndex << "endIndex:" << endIndex;
    for(int i = startIndex; i < endIndex; i++){
        mTimerValues.append("");
        mTimerValueKeepStatus.append(false);
    }
}

NCPCommand *ProgramMenu3_2::createNcpCommand()
{
    //qDebug() << "ProgramMenu3_2::createNcpCommand m_selectIndexTimerDataList:" << m_selectIndexTimerDataList;
    if (!m_modbus || !m_communication) return nullptr;
    if (m_selectIndexTimerDataList <= 0) return nullptr;
    int curTimerValue = 0;
    double timerValue = 0.0;
    curTimerValue = m_modbus->getReceiveDataHR(TIMER_TOP_ITEM_ADDR_LIST.at(m_curPageTimerDataList - 1) + m_selectIndexTimerDataList - 1);
    timerValue = (double)((double)curTimerValue / 100.0);
    m_curTimerListIndex = (m_curPageTimerDataList - 1) * 8 + (m_selectIndexTimerDataList - 1);
    //qDebug("%s:%d m_curTimerListIndex[%d]", __func__, __LINE__, m_curTimerListIndex);
    m_curSymbolAtIndex = m_timerAddr.at(m_curTimerListIndex);
    //qDebug("%s:%d m_curSymbolAtIndex[%d]", __func__, __LINE__, m_curSymbolAtIndex);
    m_curTimerItemValue = QString::number(timerValue, 'f', 2);
    //qDebug() << "m_curTimerItemValue:" << m_curTimerItemValue;
    auto command = new NCPCommandWait(m_resource, m_curTimerItemValue, m_curTimerListIndex, m_curSymbolAtIndex, 1);
    return command;
}

void ProgramMenu3_2::show(NCPCommand *command)
{
    //qDebug() << "command type:" << command->type();
    auto waitCommand = dynamic_cast<NCPCommandWait*>(command);
    QString commandLine = waitCommand->toCode();
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    QStringList splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug() << "ProgramMenu3_1::show:" << "Syntax error commandLine:[" << commandLine << "]";
        return;
    }
    //qDebug() << "splitList is value : " << splitList[1];
    QString timerValue = "";
    int timerListIndex = 0;
    int symbolListIndex = 0;
    waitCommand->getTimerAndSymbolIndex(splitList[1], &timerValue, &timerListIndex, &symbolListIndex);
    //qDebug() << << "timerValue:" << timerValue << "timerListIndex:" << timerListIndex << "symbolListIndex:" << symbolListIndex;
    if (timerValue == "") {
        qDebug() << "ProgramMenu3_1::show timerValue is invalid:" << timerValue;
        return;
    }
    bool is_ok;
    double timerValueNum = timerValue.toDouble(&is_ok);
    if (!is_ok){
        qDebug() << "timerValue is not Numerical value : " << timerValue;
        return;
    }
    //qDebug("%s:%d timerValueNum[%f]", __func__, __LINE__, timerValueNum);
    int targetPage = (timerListIndex / PROG_MENU3_2_ONE_PAGE_ITEM_MAX) + 1;
    int targetTimerValue = (int)(timerValueNum * 100.0);
    int targetPageIndex = timerListIndex % PROG_MENU3_2_ONE_PAGE_ITEM_MAX;
    m_timerDataList.replace(targetPageIndex, targetTimerValue);
    setTimerDataList(m_timerDataList);
    setCurPageTimerDataList(targetPage);
    setTargetCoopPage(targetPage);
    setSelectIndexTimerDataList(targetPageIndex + 1);
    setTargetCoopSelectIndex(targetPageIndex + 1);
    setIsLinkedStartup(true);
}

int ProgramMenu3_2::updateModbusData()
{
    //qDebug() << "ProgramMenu3_2::updateModbusData m_selectIndexTimerDataList:" << m_selectIndexTimerDataList;
    if (!m_modbus || !m_communication) return -1;
    if (m_selectIndexTimerDataList <= 0) return -1;
    int startIndex = (m_curPageTimerDataList - 1) * PROG_MENU3_2_ONE_PAGE_ITEM_MAX;
    int endIndex = (m_curPageTimerDataList - 1) * PROG_MENU3_2_ONE_PAGE_ITEM_MAX + TIMER_ITEM_ADDR_DATA_SIZE_LIST.at(m_curPageTimerDataList - 1);
    //qDebug() << "ProgramMenu3_2::createNcpCommand startIndex:" << startIndex << "endIndex:" << endIndex;
    for(int i = startIndex; i < endIndex; i++){
        if (mTimerValueKeepStatus.at(i) == true){
            // MODBUS write
            double write_data = mTimerValues.at(i).toDouble();
            QList<int> writeData;
            stModBusCommunicationData writeCommand;
            writeData.append((int)lround(write_data * 100.0));
            int curPage = i / PROG_MENU3_2_ONE_PAGE_ITEM_MAX + 1;
            int curIndex = i % PROG_MENU3_2_ONE_PAGE_ITEM_MAX;
            writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
                    QModbusPdu::FunctionCode::WriteSingleRegister,
                    (TIMER_TOP_ITEM_ADDR_LIST.at(curPage - 1) + curIndex),
                    0x0001,
                    writeData);
            m_communication->setModbusWriteCommand(writeCommand);
            mTimerValueKeepStatus.replace(i, false);
            modbusGetValues(m_curPageTimerDataList);
        }
    }
    return 0;
}

void ProgramMenu3_2::modbusReadTimerValues()
{
    if (!m_modbus || !m_communication) return;
    if (m_curPageTimerDataList <= 0 || m_curPageTimerDataList > PROG_MENU3_2_TIMER_PAGE_MAX){
        qDebug("%s:%d m_curPageTimerDataList[%d] is invalid", __func__, __LINE__, m_curPageTimerDataList);
        return;
    }
    stModBusCommunicationData readCommand;
    readCommand = ModbusCommandGenerator::makeModbusReadCommand(
        QModbusPdu::FunctionCode::ReadHoldingRegisters,
        TIMER_TOP_ITEM_ADDR_LIST.at(m_curPageTimerDataList - 1),
        TIMER_ITEM_ADDR_DATA_SIZE_LIST.at(m_curPageTimerDataList - 1));
    m_communication->setOneShotModbusCommand(readCommand);
}

void ProgramMenu3_2::modbusGetValues(int pageNum)
{
    //qDebug() << "ProgramMenu3_2::modbusGetValues pageNum:" << pageNum;
    if (pageNum <= 0 || pageNum > PROG_MENU3_2_TIMER_PAGE_MAX){
        qDebug("%s:%d pageNum[%d] is invalid", __func__, __LINE__, pageNum);
        return;
    }
    m_curPageTimerDataList = pageNum;
    modbusReadTimerValues();
}

void ProgramMenu3_2::setTimerDataList(QList<int> timerDataList)
{
    if (m_isLinkedStartup){
        return;
    }
    m_timerDataList = timerDataList;
    emit timerDataListChanged(m_timerDataList);
}

void ProgramMenu3_2::setCurPageTimerDataList(int curPageTimerDataList)
{
    // if (m_curPageTimerDataList == curPageTimerDataList){
    //     return;
    // }
    m_curPageTimerDataList = curPageTimerDataList;
    emit curPageTimerDataListChanged(m_curPageTimerDataList);
}

void ProgramMenu3_2::setSelectIndexTimerDataList(int selectIndexTimerDataList)
{
    // if (m_selectIndexTimerDataList == selectIndexTimerDataList){
    //     return;
    // }
    m_selectIndexTimerDataList = selectIndexTimerDataList;
    emit selectIndexTimerDataListChanged(m_selectIndexTimerDataList);
}

void ProgramMenu3_2::setTargetCoopPage(int targetCoopPage)
{
    m_targetCoopPage = targetCoopPage;
    emit targetCoopPageChanged(m_targetCoopPage);
}

void ProgramMenu3_2::setTargetCoopSelectIndex(int targetCoopSelectIndex)
{
    m_targetCoopSelectIndex = targetCoopSelectIndex;
    emit targetCoopSelectIndexChanged(m_targetCoopSelectIndex);
}

void ProgramMenu3_2::setIsLinkedStartup(bool isLinkedStartup)
{
    if (m_isLinkedStartup == isLinkedStartup){
        return;
    }
    m_isLinkedStartup = isLinkedStartup;
    emit isLinkedStartupChanged(m_isLinkedStartup);
}

void ProgramMenu3_2::setIsHoldingTimerValue(bool isHoldingTimerValue)
{
    if (m_isHoldingTimerValue == isHoldingTimerValue){
        return;
    }
    m_isHoldingTimerValue = isHoldingTimerValue;
    emit isHoldingTimerValueChanged(m_isHoldingTimerValue);
}

void ProgramMenu3_2::setHoldingTimerValue(QString holdingTimerValue)
{
    // if (m_holdingTimerValue == holdingTimerValue){
    //     return;
    // }
    m_holdingTimerValue = holdingTimerValue;
    emit holdingTimerValueChanged(m_holdingTimerValue);
}

void ProgramMenu3_2::setHoldingTimerValueIndex(int holdingTimerValueIndex)
{
    m_holdingTimerValueIndex = holdingTimerValueIndex;
    emit holdingTimerValueIndexChanged(m_holdingTimerValueIndex);
}

void ProgramMenu3_2::setIsReleaseHoldTimerValue(bool isReleaseHoldTimerValue)
{
    m_isReleaseHoldTimerValue = isReleaseHoldTimerValue;
    emit isReleaseHoldTimerValueChanged(m_isReleaseHoldTimerValue);
}

void ProgramMenu3_2::setTimerValue(int page, int index, QString timerValue)
{
    //qDebug() << "ProgramMenu3_2::setTimerValue page:" << page << "index:" << index << "timerValue:" << timerValue;
    if (!m_communication) return;
    if (page <= 0 || page > PROG_MENU3_2_TIMER_PAGE_MAX){
        qDebug("%s:%d page[%d] is invalid", __func__, __LINE__, page);
        return;
    }
    if (index < 0 || index > 7){
        qDebug("%s:%d index[%d] is invalid", __func__, __LINE__, index);
        return;
    }
    double write_data = timerValue.toDouble();
    QList<int> writeData;
    stModBusCommunicationData writeCommand;
    writeData.append((int)lround(write_data * 100.0));
    writeCommand = ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister,
            (TIMER_TOP_ITEM_ADDR_LIST.at(page - 1) + index),
            0x0001,
            writeData);
    m_communication->setModbusWriteCommand(writeCommand);
    modbusGetValues(page);
}

void ProgramMenu3_2::keepTimerValue(int page, int index, QString timerValue)
{
    //qDebug() << "ProgramMenu3_2::keepTimerValue page:" << page << "index:" << index << "timerValue:" << timerValue;
    if (page <= 0 || page > PROG_MENU3_2_TIMER_PAGE_MAX){
        qDebug("%s:%d page[%d] is invalid", __func__, __LINE__, page);
        return;
    }
    if (index < 0 || index > 7){
        qDebug("%s:%d index[%d] is invalid", __func__, __LINE__, index);
        return;
    }
    if (mTimerValues.count() <= 0){
        qDebug("%s:%d mTimerValues count[%d] is not init", __func__, __LINE__, mTimerValues.count());
        return;
    }
    int timerValueIndex = 0;
    for(int i = 0; i < page; i++){
        timerValueIndex += ((i < page - 1) ? TIMER_ITEM_ADDR_DATA_SIZE_LIST.at(i) : 0) + ((i == page - 1) ? index : 0);
    }
    //qDebug("%s:%d timerValueIndex[%d]", __func__, __LINE__, timerValueIndex);
    if (timerValueIndex < mTimerValueKeepStatus.count()){
        mTimerValueKeepStatus.replace(timerValueIndex, true);
        mTimerValues.replace(timerValueIndex, timerValue);
    }
    setHoldingTimerValue(timerValue);
    setHoldingTimerValueIndex(index);
    setIsHoldingTimerValue(true);
}

void ProgramMenu3_2::setCurSelectInfo(int page, int selectIndex)
{
    //qDebug("%s:%d page[%d] selectIndex[%d]", __func__, __LINE__, page, selectIndex);
    m_curPageTimerDataList = page;
    m_selectIndexTimerDataList = selectIndex;
}

void ProgramMenu3_2::updateProgramMenu(stModBusCommunicationData command)
{
    if (!m_modbus || !m_communication || !m_resource) return;
    if (m_curPageTimerDataList <= 0 || m_curPageTimerDataList > PROG_MENU3_2_TIMER_PAGE_MAX){
        qDebug("%s:%d m_curPageTimerDataList[%d] is invalid", __func__, __LINE__, m_curPageTimerDataList);
        return;
    }
    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters
        && command.readAddress == TIMER_TOP_ITEM_ADDR_LIST.at(m_curPageTimerDataList - 1)){
        //qDebug("%s:%d funcCode[%d] readAddress[%d] size[%d]", __func__, __LINE__, command.funcCode, command.readAddress, command.comSize);
        for(int i = 0; i < (int)TIMER_ITEM_ADDR_DATA_SIZE_LIST.at(m_curPageTimerDataList - 1); i++){
            m_timerDataList.replace(i, (int)command.readBuf[i]);
        }
        setTimerDataList(m_timerDataList);
        if (m_isHoldingTimerValue) {
            setIsReleaseHoldTimerValue(true);
        }
        emit ncpCommandDataChanged();
    }
}
