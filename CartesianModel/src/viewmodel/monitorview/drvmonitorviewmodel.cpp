#include "drvmonitorviewmodel.h"
#include <QMutex>

DrvMonitorViewModel::DrvMonitorViewModel(ResourceManager *resource, CommunicationThread * comm, AxisInfo *axisInfo, CommonViewModel *commonView)
    : ViewModelBase(resource, comm),
      m_axisInfo(axisInfo),
      m_commonView(commonView),
      mTimerSettingValue{0, 0},
      mTimerSettingThreeAxisValue{0, 0, 0, 0},
      mTimerId{0, 0, 0, 0},
      mIsTimerStatus{false, false, false, false},
      mTimerCnt{0, 0, 0, 0}
{
    initSettingPenSetData();
    initAxisPeakData();
    mMutexTimerSet = new QMutex(QMutex::Recursive);
    mMutexGraphDraw = new QMutex(QMutex::Recursive);

    // init
    for(int axis = 0; axis < 8; axis++){
        m_axisSpeedPeakArray[axis] = 0;
        m_axisTorquePeakArray[axis] = 0;
        m_axisRotationMaxArray[axis] = 3000;
        for(int dataNum = 0; dataNum < GRAPH_SAMPLING_FULL; dataNum++){
            m_speedDataListArray[axis].enqueue(0);
            m_torqueDataListArray[axis].enqueue(0);
        }
        for(int dataNum = 0; dataNum < GRAPH_HISTORY_NUM; dataNum++){
            m_lastSpeedIndexes[axis].append(256);
            m_lastTorqueIndexes[axis].append(256);
        }
    }
}

DrvMonitorViewModel::~DrvMonitorViewModel()
{
    delete mMutexGraphDraw;
    mMutexGraphDraw = nullptr;
    delete mMutexTimerSet;
    mMutexTimerSet = nullptr;
}

void DrvMonitorViewModel::onActivate()
{
    startModbusMonitoring();
}

void DrvMonitorViewModel::onDeactivate()
{
    stopModbusMonitoring();
    initGraphDataAndNotify();
}

void DrvMonitorViewModel::initSettingPenSetData()
{
    if (!m_resource){
        return;
    }
    m_axisInfoDataFormat            = m_resource->formatList(8, 405, 8);    // ドライバモニタ、リアル 表示方式（速度、偏差、トルク、負荷率、回生抵抗率)
    mCmnAxisOrder                   = m_commonView->getAxisOrder();
}

void DrvMonitorViewModel::initAxisPeakData(bool isFirst)
{
    if (!isFirst) { stopModbusMonitoring(); }
    for(int i = 0; i < int(AxisInfo::Axis::Axis8 + 1); i++){
        AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
        prePeakData[(int)axis].speed = 0;
        prePeakData[(int)axis].deviation = 0;
        prePeakData[(int)axis].torque = 0;
        preMinData[(int)axis].speed = 0;
        preMinData[(int)axis].deviation = 0;
        preMinData[(int)axis].torque = 0;
        if (m_axisInfo && m_resource){
            m_axisInfo->setDatasPeak(axis,
                m_resource->formatData(prePeakData[(int)axis].speed, m_axisInfoDataFormat.at((int)axis).value(0)),
                m_resource->formatData(prePeakData[(int)axis].deviation, m_axisInfoDataFormat.at((int)axis).value(1)),
                m_resource->formatData(prePeakData[(int)axis].torque, m_axisInfoDataFormat.at((int)axis).value(2), 2)
            );
            m_axisInfo->setDatasMin(axis,
                m_resource->formatData(preMinData[(int)axis].speed, m_axisInfoDataFormat.at((int)axis).value(0)),
                m_resource->formatData(preMinData[(int)axis].deviation, m_axisInfoDataFormat.at((int)axis).value(1)),
                m_resource->formatData(preMinData[(int)axis].torque, m_axisInfoDataFormat.at((int)axis).value(2), 2)
            );
        }
    }
    for(int i = 0; i < GRAPH_AXIS_NUM_MAX; i++){
        m_axisSpeedPeak.append(0);
        m_axisTorquePeak.append(0);
        m_axisRotationMax.append(0);
    }
    if (!isFirst) { startModbusMonitoring(); }
}

void DrvMonitorViewModel::initPageCommand()
{
    if (!m_communication) {
        qCritical("%s:%d m_communication is invalid", __func__, __LINE__);
        return;
    }
    bool isPeriodicMonitor = ((mSubViewIndex == 0 || mSubViewIndex == 2) ? true : false);
    if (!isPeriodicMonitor) return;
    stModBusCommunicationData readCommand[DRIVER_REALTIME_PERIODIC_MONITOR_CMD_LIST_NUM];
    for(int i = 0; i < DRIVER_REALTIME_PERIODIC_MONITOR_CMD_LIST_NUM; i++){
        readCommand[i] = ModbusCommandGenerator::makeModbusReadCommand(
                            initModbusPageCmd[i].funcCode,
                            initModbusPageCmd[i].address,
                            initModbusPageCmd[i].size
                            );
        m_communication->setPageReadCommandList(Enums::Pages::MonitorDriver, readCommand[i]);
    }
}

void DrvMonitorViewModel::initGraphDataAndNotify()
{
    ctrlRTGraphDraw(false);
    mDetailGraphMonitorCnt = 0;
}

void DrvMonitorViewModel::deinitPageCommand()
{
    if (!m_communication) return;
    m_communication->resetPageReadCommandList(Enums::Pages::MonitorDriver);
}

void DrvMonitorViewModel::reinitPageCommand()
{
    deinitPageCommand();
    initPageCommand();
}

void DrvMonitorViewModel::startModbusMonitoring()
{
    if (!m_communication) return;
    if (!mIsModbusMonitor){
        connect(m_communication, &CommunicationThread::finish,
            this, &DrvMonitorViewModel::onFinished, Qt::QueuedConnection);
        mIsModbusMonitor = true;
    }
    connect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &DrvMonitorViewModel::endModbusPageChange, Qt::DirectConnection);
}

void DrvMonitorViewModel::stopModbusMonitoring()
{
    if (!m_communication) return;
    if (mIsModbusMonitor){
        disconnect(m_communication, &CommunicationThread::finish,
            this, &DrvMonitorViewModel::onFinished);
        mIsModbusMonitor = false;
    }
    disconnect(m_modbus, &ModbusManager::endModbusPageWriteReady, this, &DrvMonitorViewModel::endModbusPageChange);
}

void DrvMonitorViewModel::setSubViewIndex(int index)
{
    if (index < 0 || index > 2) return;
    mSubViewIndex = index;
    qDebug("%s:%d mSubViewIndex[%d]", __func__, __LINE__, mSubViewIndex);
    startModbusMonitoring();
}

int DrvMonitorViewModel::getAxisCount()
{
    return m_axisInfoDataFormat.count();
}

int DrvMonitorViewModel::getPeakTorqueData(int axisIndex)
{
    if (axisIndex < (int)AxisInfo::Axis::AxisY
        || axisIndex >= (int)AxisInfo::Axis::AxisNum){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return -1;
    }
    return (int)prePeakData[axisIndex].torque;
}

int DrvMonitorViewModel::getRealTimePeakTorqueData(int axisIndex)
{
    if (!m_modbus){
        return -1;
    }
    if (axisIndex < (int)AxisInfo::Axis::AxisY
        || axisIndex >= (int)AxisInfo::Axis::AxisNum){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return -1;
    }
    return (int)(m_modbus->getWordReceiveDataRG(REALTIME_AXIS_REAL_ADDRESS_LIST.at((int)axisIndex).value(2)));
}

int DrvMonitorViewModel::getPeakSpeedData(int axisIndex)
{
    if (axisIndex < (int)AxisInfo::Axis::AxisY
        || axisIndex >= (int)AxisInfo::Axis::AxisNum){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return -1;
    }
    return (int)prePeakData[axisIndex].speed;
}

int DrvMonitorViewModel::getMinSpeedData(int axisIndex)
{
    if (axisIndex < (int)AxisInfo::Axis::AxisY
        || axisIndex >= (int)AxisInfo::Axis::AxisNum){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return -1;
    }
    return (int)preMinData[axisIndex].speed;
}

int DrvMonitorViewModel::getMinTorqueData(int axisIndex)
{
    if (axisIndex < (int)AxisInfo::Axis::AxisY
        || axisIndex >= (int)AxisInfo::Axis::AxisNum){
        qCritical("%s:%d axisIndex[%d] is invalid", __func__, __LINE__, axisIndex);
        return -1;
    }
    return (int)preMinData[axisIndex].torque;
}

void DrvMonitorViewModel::setTimerValue(int graphOrder, int timerValue)
{
    qDebug("%s:%d graphOrder[%d] timerValue[%d]", __func__, __LINE__, graphOrder, timerValue);
    if (timerValue < (int)TimerSettingValue::TimerSettingValue200ms
        || timerValue >= (int)TimerSettingValue::TimerSettingValueMax){
        qCritical("%s:%d timerValue[%d] is invalid", __func__, __LINE__, timerValue);
        return;
    }
    if (mGraphDrawMode < (int)GraphDrawMode::GraphDrawModeOneAxis
        || mGraphDrawMode >= (int)GraphDrawMode::GraphDrawModeMax){
        qCritical("%s:%d mGraphDrawMode[%d] is invalid", __func__, __LINE__, mGraphDrawMode);
        return;
    }
    if (graphOrder < (int)GraphOrder::GraphOrderOneAxis
        || graphOrder >= (int)GraphOrder::GraphOrderMax){
        qCritical("%s:%d graphOrder[%d] is invalid", __func__, __LINE__, graphOrder);
        return;
    }
    lockTimerSetMutex();
    if (mGraphDrawMode == (int)GraphDrawMode::GraphDrawModeOneAxis){
        mTimerSettingValue[mGraphDrawMode] = timerValue;
    } else if (mGraphDrawMode == (int)GraphDrawMode::GraphDrawModeThreeAxis){
        mTimerSettingThreeAxisValue[graphOrder] = timerValue;
    }
    DispDataUpdate();

    unlockTimerSetMutex();
}

void DrvMonitorViewModel::setGraphDrawMode(int drawMode)
{
    qDebug("%s:%d drawMode[%d]", __func__, __LINE__, drawMode);
    if (drawMode < (int)GraphDrawMode::GraphDrawModeInvalid
        || drawMode >= (int)GraphDrawMode::GraphDrawModeMax){
        qCritical("%s:%d drawMode[%d] is invalid", __func__, __LINE__, drawMode);
        return;
    }
    mPreGraphDrawMode = mGraphDrawMode;
    mGraphDrawMode = drawMode;

    int curTopAxisIndex = 0;
    switch (mGraphDrawMode) {
    case (int)GraphDrawMode::GraphDrawModeOneAxis:
        curTopAxisIndex = mCurAxisIndex;
        break;
    case (int)GraphDrawMode::GraphDrawModeThreeAxis:
        curTopAxisIndex = (mCurAxisIndex / 3) * 3;
        break;
    default:
        qCritical("%s:%d mGraphDrawMode[%d] is invalid", __func__, __LINE__, mGraphDrawMode);
        return;
    }

    mCurTopAxis = mCmnAxisOrder.at(curTopAxisIndex);
    mCurTopAxisIndex = curTopAxisIndex;

    DispDataUpdate();
}

void DrvMonitorViewModel::setCurAxis(int curAxisIndex)
{
    if (mCmnAxisOrder.count() <= 0){
        qCritical("%s:%d mCmnAxisOrder[%d] is invalid", __func__, __LINE__, mCmnAxisOrder.count());
        return;
    }
    if(curAxisIndex < 0 || curAxisIndex >= mCmnAxisOrder.count()){
        qCritical("%s:%d curAxisIndex[%d] is invalid", __func__, __LINE__, curAxisIndex);
        return;
    }

    int curTopAxisIndex = 0;
    switch (mGraphDrawMode) {
    case (int)GraphDrawMode::GraphDrawModeOneAxis:
        curTopAxisIndex = curAxisIndex;
        break;
    case (int)GraphDrawMode::GraphDrawModeThreeAxis:
        curTopAxisIndex = (curAxisIndex / 3) * 3;
        break;
    default:
        qCritical("%s:%d mGraphDrawMode[%d] is invalid", __func__, __LINE__, mGraphDrawMode);
        return;
    }

    mCurTopAxis = mCmnAxisOrder.at(curTopAxisIndex);
    mCurTopAxisIndex = curTopAxisIndex;
    mCurAxisIndex = curAxisIndex;
    qDebug("%s:%d curTopAxisIndex[%d] mCurTopAxis[%d] mCurTopAxisIndex[%d] mCurAxisIndex[%d]", __func__, __LINE__,
        curTopAxisIndex, mCurTopAxis, mCurTopAxisIndex, mCurAxisIndex);
    curAxisIndexChanged(mCurAxisIndex);

    DispDataUpdate();
}

void DrvMonitorViewModel::DispDataUpdate()
{
    switch (mGraphDrawMode) {
    case (int)GraphDrawMode::GraphDrawModeOneAxis:
        {

            TimerSettingValue mode = (TimerSettingValue)mTimerSettingValue[GraphDrawMode::GraphDrawModeOneAxis];
            DispDataQueueCopy(&m_speedDispDataList_axis1, &m_speedDataListArray[mCurTopAxis], mode);
            DispDataQueueCopy(&m_torqueDispDataList_axis1, &m_torqueDataListArray[mCurTopAxis], mode);
            graphSpeedDataChanged(m_speedDispDataList_axis1);
            graphTorqueDataChanged(m_torqueDispDataList_axis1);
            m_axisSpeedPeak[0] = m_axisSpeedPeakArray[mCurTopAxis];
            m_axisTorquePeak[0] = m_axisTorquePeakArray[mCurTopAxis];
            m_axisRotationMax[0] = m_axisRotationMaxArray[mCurTopAxis];
            emit axisSpeedPeakChanged(m_axisSpeedPeak);
            emit axisTorquePeakChanged(m_axisTorquePeak);
            emit axisRotationMaxChanged(m_axisRotationMax);
        }
        break;
    case (int)GraphDrawMode::GraphDrawModeThreeAxis:
        {
            TimerSettingValue modeAxis1 = (TimerSettingValue)mTimerSettingThreeAxisValue[GraphOrder::GraphOrderThreeAxisFirst];
            TimerSettingValue modeAxis2 = (TimerSettingValue)mTimerSettingThreeAxisValue[GraphOrder::GraphOrderThreeAxisSecond];
            TimerSettingValue modeAxis3 = (TimerSettingValue)mTimerSettingThreeAxisValue[GraphOrder::GraphOrderThreeAxisThird];
            DispDataQueueCopy(&m_speedDispDataList_axis1, &m_speedDataListArray[mCurTopAxis], modeAxis1);
            DispDataQueueCopy(&m_torqueDispDataList_axis1, &m_torqueDataListArray[mCurTopAxis], modeAxis1);
            m_axisSpeedPeak[0] = m_axisSpeedPeakArray[mCurTopAxis];
            m_axisTorquePeak[0] = m_axisTorquePeakArray[mCurTopAxis];
            m_axisRotationMax[0] = m_axisRotationMaxArray[mCurTopAxis];

            if(mCurTopAxisIndex+1 < mCmnAxisOrder.count()){
                int axisOrder2 = mCmnAxisOrder.at(mCurTopAxisIndex + 1);
                DispDataQueueCopy(&m_speedDispDataList_axis2, &m_speedDataListArray[axisOrder2], modeAxis2);
                DispDataQueueCopy(&m_torqueDispDataList_axis2, &m_torqueDataListArray[axisOrder2], modeAxis2);
                m_axisSpeedPeak[1] = m_axisSpeedPeakArray[axisOrder2];
                m_axisTorquePeak[1] = m_axisTorquePeakArray[axisOrder2];
                m_axisRotationMax[1] = m_axisRotationMaxArray[axisOrder2];
            }
            else{
                m_speedDispDataList_axis2.clear();
                m_torqueDispDataList_axis2.clear();
            }
            if(mCurTopAxisIndex+2 < mCmnAxisOrder.count()){
                int axisOrder3 = mCmnAxisOrder.at(mCurTopAxisIndex + 2);
                DispDataQueueCopy(&m_speedDispDataList_axis3, &m_speedDataListArray[axisOrder3], modeAxis3);
                DispDataQueueCopy(&m_torqueDispDataList_axis3, &m_torqueDataListArray[axisOrder3], modeAxis3);
                m_axisSpeedPeak[2] = m_axisSpeedPeakArray[axisOrder3];
                m_axisTorquePeak[2] = m_axisTorquePeakArray[axisOrder3];
                m_axisRotationMax[2] = m_axisRotationMaxArray[axisOrder3];
            }
            else{
                m_speedDispDataList_axis3.clear();
                m_torqueDispDataList_axis3.clear();
            }
            graphSpeedDataFirstChanged(m_speedDispDataList_axis1);
            graphTorqueDataFirstChanged(m_torqueDispDataList_axis1);
            graphSpeedDataSecondChanged(m_speedDispDataList_axis2);
            graphTorqueDataSecondChanged(m_torqueDispDataList_axis2);
            graphSpeedDataThirdChanged(m_speedDispDataList_axis3);
            graphTorqueDataThirdChanged(m_torqueDispDataList_axis3);
            emit axisSpeedPeakChanged(m_axisSpeedPeak);
            emit axisTorquePeakChanged(m_axisTorquePeak);
            emit axisRotationMaxChanged(m_axisRotationMax);
        }
        break;
    default:
        break;
    }
}

void DrvMonitorViewModel::DispDataQueueCopy(QQueue<int> *dest, QQueue<int> *src, TimerSettingValue mode)
{
    dest->clear();
    switch (mode) {
    case TimerSettingValue::TimerSettingValue200ms:
        dest->append(src->mid(GRAPH_SAMPLING_200MS_START_INDEX));
        break;
    case TimerSettingValue::TimerSettingValue500ms:
        dest->append(src->mid(GRAPH_SAMPLING_500MS_START_INDEX));
        break;
    case TimerSettingValue::TimerSettingValue1s:
        dest->append(src->mid(GRAPH_SAMPLING_1S_START_INDEX));
        break;
    case TimerSettingValue::TimerSettingValue2s:
        dest->append(src->mid(GRAPH_SAMPLING_2S_START_INDEX));
        break;
    default:
        break;
    }
}

void DrvMonitorViewModel::setIsRTGraphDraw(bool isRTGraphDraw)
{
    m_isRTGraphDraw = isRTGraphDraw;
    emit isRTGraphDrawChanged(m_isRTGraphDraw);
}

void DrvMonitorViewModel::ctrlRTGraphDraw(bool isRTDraw)
{
    lockGraphDrawMutex();
    setIsRTGraphDraw(isRTDraw);
    if(isRTDraw == true){
        // 開始時にピークデータを初期化する
        for(int i = 0; i < 8; i++){
            m_axisSpeedPeakArray[i] = 0;
            m_axisTorquePeakArray[i] = 0;
        }
        for(int i = 0; i < m_axisSpeedPeak.length(); i++){
            m_axisSpeedPeak[i] = 0;
        }
        for(int i = 0; i < m_axisTorquePeak.length(); i++){
            m_axisTorquePeak[i] = 0;
        }
    }
    for(int i = 0; i < int(AxisInfo::Axis::Axis8 + 1); i++){
        AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
        prePeakData[(int)axis].speed = 0;
        prePeakData[(int)axis].torque = 0;
    }
    if (mGraphDrawMode < (int)GraphDrawMode::GraphDrawModeOneAxis
        || mGraphDrawMode >= (int)GraphDrawMode::GraphDrawModeMax){
        qCritical("%s:%d mGraphDrawMode[%d] is invalid", __func__, __LINE__, mGraphDrawMode);
        unlockGraphDrawMutex();
        return;
    }
    qDebug("%s:%d timerSet[%d]", __func__, __LINE__, mTimerSettingValue[mGraphDrawMode]);
    unlockGraphDrawMutex();
}

void DrvMonitorViewModel::timerStart(int mode, int timeOut)
{
    if (!mIsTimerStatus[mode]) {
        mTimerId[mode] = startTimer(timeOut);
        mIsTimerStatus[mode] = true;
    }
}

void DrvMonitorViewModel::timerStop(int mode)
{
    if (mIsTimerStatus[mode]) {
        killTimer(mTimerId[mode]);
        mTimerCnt[mode] = 0;
        mIsTimerStatus[mode] = false;
    }
}

void DrvMonitorViewModel::timerEvent(QTimerEvent *timerEvent)
{
    if (!timerEvent) return;
    for(int i = 0; i < (int)GraphOrder::GraphOrderMax; i++){
        if(timerEvent->timerId() == mTimerId[i]){
            mTimerCnt[i]++;
            if (mTimerCnt[i] >= 4){
                mTimerCnt[i] = 0;
            }
        }
    }
}

void DrvMonitorViewModel::lockTimerSetMutex()
{
    if (mMutexTimerSet){
        mMutexTimerSet->lock();
        mIsMutexTimerSet = true;
    }
}

void DrvMonitorViewModel::unlockTimerSetMutex()
{
    if (mMutexTimerSet){
        if (mIsMutexTimerSet) {
            mMutexTimerSet->unlock();
        }
        mIsMutexTimerSet = false;
    }
}

void DrvMonitorViewModel::lockGraphDrawMutex()
{
    if (mMutexGraphDraw){
        mMutexGraphDraw->lock();
        mIsMutexGraphDraw = true;
    }
}

void DrvMonitorViewModel::unlockGraphDrawMutex()
{
    if (mMutexGraphDraw){
        if (mIsMutexGraphDraw) {
            mMutexGraphDraw->unlock();
        }
        mIsMutexGraphDraw = false;
    }
}

void DrvMonitorViewModel::updateOneAxisData(int axis)
{
    int curSpeedValue = 0;
    int curTorqueValue = 0;
    QList<int> axisSpeed[GRAPH_HISTORY_NUM];
    QList<int> axisTorque[GRAPH_HISTORY_NUM];
    for(int i = 0; i <= (int)AxisInfo::Axis::Axis8; i++){
        axisSpeed[0].append(0);
        axisTorque[0].append(0);
        axisSpeed[1].append(0);
        axisTorque[1].append(0);
        axisSpeed[2].append(0);
        axisTorque[2].append(0);
        axisSpeed[3].append(0);
        axisTorque[3].append(0);
        axisSpeed[4].append(0);
        axisTorque[4].append(0);
    }
    for(int i = 0; i < GRAPH_HISTORY_NUM; i++){
        curSpeedValue = m_modbus->getWordReceiveDataRG(WAVEFORM_AXIS_ADDRESS_LIST.at(axis).value(0 + i * 2));
        curTorqueValue = (m_modbus->getWordReceiveDataRG(WAVEFORM_AXIS_ADDRESS_LIST.at(axis).value(1 + i * 2))/100);
#ifdef GRAPH_DRAW_SPEED_DEBUG
        debugDrawSpeedGraph(axis, &curSpeedValue);
#endif // GRAPH_DRAW_SPEED_DEBUG
#ifdef GRAPH_DRAW_TORQUE_DEBUG
        debugDrawTorqueGraph(axis, &curTorqueValue);
#endif // GRAPH_DRAW_TORQUE_DEBU
        axisSpeed[i].replace(axis, curSpeedValue);
        axisTorque[i].replace(axis, curTorqueValue);
        judgePeakDataNotify(axis, axisSpeed[i].at(axis), axisTorque[i].at(axis));
    }
}

void DrvMonitorViewModel::updateThreeAxisData(int axis)
{
    int curSpeedValue = 0;
    int curTorqueValue = 0;
    QList<int> axisSpeed[GRAPH_HISTORY_NUM];
    QList<int> axisTorque[GRAPH_HISTORY_NUM];
    for(int i = 0; i <= (int)AxisInfo::Axis::Axis8; i++){
        axisSpeed[0].append(0);
        axisTorque[0].append(0);
        axisSpeed[1].append(0);
        axisTorque[1].append(0);
        axisSpeed[2].append(0);
        axisTorque[2].append(0);
        axisSpeed[3].append(0);
        axisTorque[3].append(0);
        axisSpeed[4].append(0);
        axisTorque[4].append(0);
    }
    for(int i = 0; i < GRAPH_HISTORY_NUM; i++){
        curSpeedValue = m_modbus->getWordReceiveDataRG(WAVEFORM_AXIS_ADDRESS_LIST.at(axis).value(0 + i * 2));
        curTorqueValue = (m_modbus->getWordReceiveDataRG(WAVEFORM_AXIS_ADDRESS_LIST.at(axis).value(1 + i * 2))/100);
#ifdef GRAPH_DRAW_SPEED_DEBUG
        debugDrawSpeedGraph(axis, &curSpeedValue);
#endif // GRAPH_DRAW_SPEED_DEBUG
#ifdef GRAPH_DRAW_TORQUE_DEBUG
        debugDrawTorqueGraph(axis, &curTorqueValue);
#endif // GRAPH_DRAW_TORQUE_DEBUG
        axisSpeed[i].replace(axis, curSpeedValue);
        axisTorque[i].replace(axis, curTorqueValue);
        judgePeakDataNotify(axis, axisSpeed[i].at(axis), axisTorque[i].at(axis));
    }
}

void DrvMonitorViewModel::drawAxisGraph(int axis)
{
    if (!m_modbus || !m_axisInfo) return;
    if (axis < 0 || axis > 7) return;
    if (axis >= (int)AxisInfo::Axis6) return;
    switch((GraphDrawMode)mGraphDrawMode){
    case GraphDrawMode::GraphDrawModeOneAxis:
        if(axis == mCurTopAxis){
            // 速度、トルクデータ通知(1軸)
            if (mDetailGraphMonitorCnt > 1){
                updateOneAxisData(axis);
            }
        }
        break;
    case GraphDrawMode::GraphDrawModeThreeAxis:
        // 速度、トルクデータ通知(3軸)
        if (mCurTopAxisIndex <= mCmnAxisOrder.count() - 3){
            if (mDetailGraphMonitorCnt > 1){
                if (axis == mCurTopAxis){               // First Axis
                    updateThreeAxisData(axis);
                } else if (axis == mCmnAxisOrder.at(mCurTopAxisIndex + 1)){    // Second Axis
                    updateThreeAxisData(axis);
                } else if (axis == mCmnAxisOrder.at(mCurTopAxisIndex + 2)){    // Third Axis
                    updateThreeAxisData(axis);
                }
            }
        } else if (mCurTopAxisIndex <= mCmnAxisOrder.count() - 2){
            if (mDetailGraphMonitorCnt > 1){
                if (axis == mCurTopAxis){               // First Axis
                    updateThreeAxisData(axis);
                } else if (axis == mCmnAxisOrder.at(mCurTopAxisIndex + 1)){    // Second Axis
                    updateThreeAxisData(axis);
                }
            }
        } else if (mCurTopAxisIndex <= mCmnAxisOrder.count() - 1){
            if (mDetailGraphMonitorCnt > 1){
                if (axis == mCurTopAxis){               // First Axis
                    updateThreeAxisData(axis);
                }
            }
        }
        break;
    default:
        break;
    }
}

void DrvMonitorViewModel::judgePeakDataNotify(int axis, int speedValue, int torqueValue)
{
    if (!m_axisInfo || !m_resource) return;

    switch((GraphDrawMode)mGraphDrawMode){
    case GraphDrawMode::GraphDrawModeOneAxis:
        if((short)speedValue > prePeakData[(int)axis].speed){
            prePeakData[(int)axis].speed = (short)speedValue;
        }
        if((short)torqueValue > prePeakData[(int)axis].torque){
            prePeakData[(int)axis].torque = (short)torqueValue;
        }
        break;
    case GraphDrawMode::GraphDrawModeThreeAxis:
        if (axis >= mCurTopAxis && axis <= mCurTopAxis + 2){
            if (axis == mCurTopAxis){               // First Axis
                if((short)speedValue > prePeakData[(int)axis].speed){
                    prePeakData[(int)axis].speed = (short)speedValue;
                }
                if((short)torqueValue > prePeakData[(int)axis].torque){
                    prePeakData[(int)axis].torque = (short)torqueValue;
                }
            } else if (axis == mCurTopAxis + 1){    // Second Axis
                if((short)speedValue > prePeakData[(int)axis].speed){
                    prePeakData[(int)axis].speed = (short)speedValue;
                }
                if((short)torqueValue > prePeakData[(int)axis].torque){
                    prePeakData[(int)axis].torque = (short)torqueValue;
                }
            } else if (axis == mCurTopAxis + 2){    // Third Axis
                if((short)speedValue > prePeakData[(int)axis].speed){
                    prePeakData[(int)axis].speed = (short)speedValue;
                }
                if((short)torqueValue > prePeakData[(int)axis].torque){
                    prePeakData[(int)axis].torque = (short)torqueValue;
                }
            }
        }
        break;
    default:
        break;
    }
}

void DrvMonitorViewModel::onFinished()
{
    if(m_pageChanging == true){
        //qDebug("onFinished: skip.");
        return;
    }

    if (!m_modbus || !m_axisInfo || !m_resource) return;

    mDetailGraphMonitorCnt++;

    if (mGraphDrawMode >= 0){        // グラフ表示モード(波形)の場合
        dataUpdateWave();
    }else{
        dataUpdate();
    }

    if (mDetailGraphMonitorCnt >= 2){
        mDetailGraphMonitorCnt = 0;
    }

    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void DrvMonitorViewModel::dataUpdate()
{
    //qDebug("dataUpdate: start.");
    for(int i = 0; i < getAxisCount(); i++){
        AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);

        if(m_modbus->getIsPageWriteReady() == false){
            bool updatePeak = false;
            bool updateMin = false;
            short speedValue = (short)m_modbus->getWordReceiveDataRG(REALTIME_AXIS_REAL_ADDRESS_LIST.at((int)axis).value(0));
            int deviationValue = (int)m_modbus->getLongReceiveDataRG(REALTIME_AXIS_REAL_ADDRESS_LIST.at((int)axis).value(1));
            short torqueValue = m_modbus->getWordReceiveDataRG(REALTIME_AXIS_REAL_ADDRESS_LIST.at((int)axis).value(2));
            QString speed =  m_resource->formatData(speedValue, m_axisInfoDataFormat.at((int)axis).value(0));
            QString deviation = m_resource->formatData(deviationValue, m_axisInfoDataFormat.at((int)axis).value(1));
            QString torque = m_resource->formatData(torqueValue, m_axisInfoDataFormat.at((int)axis).value(2), 2);
            QString loadRate =  m_resource->formatData(
                    (short)m_modbus->getWordReceiveDataRG(REALTIME_AXIS_REAL_ADDRESS_LIST.at((int)axis).value(3)),
                    m_axisInfoDataFormat.at((int)axis).value(3));
            QString resurrectionResistorRate =  m_resource->formatData(
                    (short)m_modbus->getWordReceiveDataRG(REALTIME_AXIS_REAL_ADDRESS_LIST.at((int)axis).value(4)),
                    m_axisInfoDataFormat.at((int)axis).value(4));
            m_axisInfo->setDatasRT(axis, speed, deviation, torque, loadRate, resurrectionResistorRate);
            if(speedValue > prePeakData[(int)axis].speed){
                prePeakData[(int)axis].speed = speedValue;
                updatePeak = true;
            }
            if(speedValue < preMinData[(int)axis].speed){
                preMinData[(int)axis].speed = speedValue;
                updateMin = true;
            }
            if(deviationValue > prePeakData[(int)axis].deviation){
                prePeakData[(int)axis].deviation = deviationValue;
                updatePeak = true;
            }
            if(deviationValue < preMinData[(int)axis].deviation){
                preMinData[(int)axis].deviation = deviationValue;
                updateMin = true;
            }
            if(torqueValue > prePeakData[(int)axis].torque){
                prePeakData[(int)axis].torque = torqueValue;
                updatePeak = true;
            }
            if(torqueValue < preMinData[(int)axis].torque){
                preMinData[(int)axis].torque = torqueValue;
                updateMin = true;
            }
            if(updatePeak){
                m_axisInfo->setDatasPeak(axis,
                    m_resource->formatData(prePeakData[(int)axis].speed, m_axisInfoDataFormat.at((int)axis).value(0)),
                    m_resource->formatData(prePeakData[(int)axis].deviation, m_axisInfoDataFormat.at((int)axis).value(1)),
                    m_resource->formatData((prePeakData[(int)axis].torque / 100), m_axisInfoDataFormat.at((int)axis).value(2))
                );
            }
            if(updateMin){
                m_axisInfo->setDatasMin(axis,
                    m_resource->formatData(preMinData[(int)axis].speed, m_axisInfoDataFormat.at((int)axis).value(0)),
                    m_resource->formatData(preMinData[(int)axis].deviation, m_axisInfoDataFormat.at((int)axis).value(1)),
                    m_resource->formatData((preMinData[(int)axis].torque / 100), m_axisInfoDataFormat.at((int)axis).value(2))
                );
            }
            setDataLoaded(true);
        }
    }
}

void DrvMonitorViewModel::dataUpdateWave()
{
    //qDebug("dataUpdateWave: start.");
    for(int axis = 0; axis < 8; axis++){
        short rotationMax = m_modbus->getWordReceiveDataRG(2761+axis);
        m_axisRotationMaxArray[axis] = rotationMax;
    }
    int axisOrder2 = -1;
    int axisOrder3 = -1;
    if(mCurTopAxisIndex+1 < mCmnAxisOrder.count()){
        axisOrder2 = mCmnAxisOrder.at(mCurTopAxisIndex + 1);
    }
    if(mCurTopAxisIndex+2 < mCmnAxisOrder.count()){
        axisOrder3 = mCmnAxisOrder.at(mCurTopAxisIndex + 2);
    }

    m_axisRotationMax[0] = m_axisRotationMaxArray[mCurTopAxis];
    if(axisOrder2 != -1){
        m_axisRotationMax[1] = m_axisRotationMaxArray[axisOrder2];
    }
    if(axisOrder3 != -1){
        m_axisRotationMax[2] = m_axisRotationMaxArray[axisOrder3];
    }
    emit axisRotationMaxChanged(m_axisRotationMax);

    if (m_isRTGraphDraw){        // グラフ表示モード(波形)の場合
        //qDebug("dataUpdateWave: dataList enqueue & dequeue start.");
        for(int axis = 0; axis < 5; axis++){
            short rotationMax =  m_axisRotationMaxArray[axis];
            bool speedDataIsNew = false;
            bool torqueDataIsNew = false;

            for(int dataNum = 0; dataNum < 5; dataNum++){
                int speedIndex = WAVEFORM_AXIS_ADDRESS_LIST[axis][dataNum*2];
                int torqueIndex = WAVEFORM_AXIS_ADDRESS_LIST[axis][dataNum*2+1];

                ushort speedData = m_modbus->getWordReceiveDataRG(speedIndex);
                ushort torqueData = m_modbus->getWordReceiveDataRG(torqueIndex);

                uchar speedIdx = (speedData >> 8) & 0xFF;
                uchar c_speedData = speedData & 0xFF;
                short speed = (((short)c_speedData) - 128) * rotationMax / 115;

                uchar torqueIdx = (torqueData >> 8) & 0xFF;
                uchar c_torqueData = torqueData & 0xFF;
                short torque = (((short)c_torqueData) - 128) * 400 / 128;

                if(speedDataIsNew == true || m_lastSpeedIndexes[axis].contains(speedIdx) == false){
                    speedDataIsNew = true;
                    // FIFO
                    m_speedDataListArray[axis].enqueue(speed);
                    m_speedDataListArray[axis].dequeue();

                    if(m_axisSpeedPeakArray[axis] < speed){
                        m_axisSpeedPeakArray[axis] = speed;
                    }

                    switch (mGraphDrawMode) {
                    case (int)GraphDrawMode::GraphDrawModeOneAxis:
                        if(axis == mCurTopAxis){
                            m_speedDispDataList_axis1.enqueue(speed);
                            m_speedDispDataList_axis1.dequeue();
                            if(m_axisSpeedPeak[0] < speed){
                                m_axisSpeedPeak[0] = speed;
                                emit axisSpeedPeakChanged(m_axisSpeedPeak);
                            }
                        }
                        break;
                    case (int)GraphDrawMode::GraphDrawModeThreeAxis:
                        if(axis == mCurTopAxis){
                            m_speedDispDataList_axis1.enqueue(speed);
                            m_speedDispDataList_axis1.dequeue();
                            if(m_axisSpeedPeak[0] < speed){
                                m_axisSpeedPeak[0] = speed;
                                emit axisSpeedPeakChanged(m_axisSpeedPeak);
                            }
                        }
                        else if(axis == axisOrder2){
                            m_speedDispDataList_axis2.enqueue(speed);
                            m_speedDispDataList_axis2.dequeue();
                            if(m_axisSpeedPeak[1] < speed){
                                m_axisSpeedPeak[1] = speed;
                                emit axisSpeedPeakChanged(m_axisSpeedPeak);
                            }
                        }
                        else if(axis == axisOrder3){
                            m_speedDispDataList_axis3.enqueue(speed);
                            m_speedDispDataList_axis3.dequeue();
                            if(m_axisSpeedPeak[2] < speed){
                                m_axisSpeedPeak[2] = speed;
                                emit axisSpeedPeakChanged(m_axisSpeedPeak);
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }

                if(torqueDataIsNew == true || m_lastTorqueIndexes[axis].contains(torqueIdx) == false){
                    torqueDataIsNew = true;
                    // FIFO
                    m_torqueDataListArray[axis].enqueue(torque);
                    m_torqueDataListArray[axis].dequeue();
                    if(m_axisTorquePeakArray[axis] < torque){
                        m_axisTorquePeakArray[axis] = torque;
                    }

                    switch (mGraphDrawMode) {
                    case (int)GraphDrawMode::GraphDrawModeOneAxis:
                        if(axis == mCurTopAxis){
                            m_torqueDispDataList_axis1.enqueue(torque);
                            m_torqueDispDataList_axis1.dequeue();
                            if(m_axisTorquePeak[0] < torque){
                                m_axisTorquePeak[0] = torque;
                                emit axisTorquePeakChanged(m_axisTorquePeak);
                            }
                        }
                        break;
                    case (int)GraphDrawMode::GraphDrawModeThreeAxis:
                        if(axis == mCurTopAxis){
                            m_torqueDispDataList_axis1.enqueue(torque);
                            m_torqueDispDataList_axis1.dequeue();
                            if(m_axisTorquePeak[0] < torque){
                                m_axisTorquePeak[0] = torque;
                                emit axisTorquePeakChanged(m_axisTorquePeak);
                            }
                        }
                        else if(axis == axisOrder2){
                            m_torqueDispDataList_axis2.enqueue(torque);
                            m_torqueDispDataList_axis2.dequeue();
                            if(m_axisTorquePeak[1] < torque){
                                m_axisTorquePeak[1] = torque;
                                emit axisTorquePeakChanged(m_axisTorquePeak);
                            }
                        }
                        else if(axis == axisOrder3){
                            m_torqueDispDataList_axis3.enqueue(torque);
                            m_torqueDispDataList_axis3.dequeue();
                            if(m_axisTorquePeak[2] < torque){
                                m_axisTorquePeak[2] = torque;
                                emit axisTorquePeakChanged(m_axisTorquePeak);
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }

                m_lastSpeedIndexes[axis][dataNum] = speedIdx;
                m_lastTorqueIndexes[axis][dataNum] = torqueIdx;
            }

            // 必要かわからないが、一旦callしておく。
            drawAxisGraph((int)axis);
        }

        switch (mGraphDrawMode) {
        case (int)GraphDrawMode::GraphDrawModeOneAxis:
            graphSpeedDataChanged(m_speedDispDataList_axis1);
            graphTorqueDataChanged(m_torqueDispDataList_axis1);
            break;
        case (int)GraphDrawMode::GraphDrawModeThreeAxis:
            graphSpeedDataFirstChanged(m_speedDispDataList_axis1);
            graphTorqueDataFirstChanged(m_torqueDispDataList_axis1);
            graphSpeedDataSecondChanged(m_speedDispDataList_axis2);
            graphTorqueDataSecondChanged(m_torqueDispDataList_axis2);
            graphSpeedDataThirdChanged(m_speedDispDataList_axis3);
            graphTorqueDataThirdChanged(m_torqueDispDataList_axis3);
            break;
        default:
            break;
        }
        //qDebug("dataUpdateWave: dataList enqueue & dequeue end.");
    }
}

#ifdef GRAPH_DRAW_SPEED_DEBUG
void DrvMonitorViewModel::debugDrawSpeedGraph(int axis, int *curSpeedValue)
{
    (void)axis;
    (void)curSpeedValue;
}
#endif // GRAPH_DRAW_SPEED_DEBUG
#ifdef GRAPH_DRAW_TORQUE_DEBUG
void DrvMonitorViewModel::debugDrawTorqueGraph(int axis, int *curTorqueValue)
{
    //static int cnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (!curTorqueValue){
        return;
    }
    (void)axis;
}
#endif // GRAPH_DRAW_TORQUE_DEBUG

void DrvMonitorViewModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
    qDebug("%s:%d m_pageChanging[%d]", __func__, __LINE__, m_pageChanging);
}

void DrvMonitorViewModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;

        emit dataLoadedChanged(m_dataLoaded);
    }
}

void DrvMonitorViewModel::endModbusPageChange(QList<int> writeData)
{
    qDebug("%s:%d mSubViewIndex[%d] writeData[0]=[%d] writeData[1]=[%d] writeData[2]=[%d]",
        __func__, __LINE__, mSubViewIndex, writeData[0], writeData[1], writeData[2]);
    if (mSubViewIndex == 2){
        if(writeData.length() == 3
                && (writeData[0] == 12 && writeData[1] == 0 && writeData[2] == 0)){
            m_pageChanging = false;
            qDebug("%s:%d m_pageChanging[%d]", __func__, __LINE__, m_pageChanging);
        }
    } else {
        if(writeData.length() == 3
                && (writeData[0] == 6 && writeData[1] == 0 && writeData[2] == 0)){
            m_pageChanging = false;
            qDebug("%s:%d m_pageChanging[%d]", __func__, __LINE__, m_pageChanging);
        }
    }
}
