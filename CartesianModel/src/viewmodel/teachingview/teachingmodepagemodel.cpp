#include "teachingmodepagemodel.h"
#include "teachingviewdef.h"
// ADD :  support animation behavior By.IJ.YI 25.07.29
#include <QDir>
#include <QCoreApplication>

TeachingModePageModel::TeachingModePageModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView)
    : PageModelBase(resource, comm)
{
    m_commonView = commonView;
    init();
}

TeachingModePageModel::TeachingModePageModel(QObject *object)
    : PageModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

TeachingModePageModel::~TeachingModePageModel()
{
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingModePageModel::onFinished);
    disconnect(m_resource, &ResourceManager::languageChanged, this, &TeachingModePageModel::onLanguageChanged);
    disconnect(m_commonView, &CommonViewModel::userLevelChanged, this, &TeachingModePageModel::onUserLevelChanged);
    qDeleteAll(m_modeDataList);
}

void TeachingModePageModel::init()
{
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::ModePage));
    setPageLabelColor(m_resource->rgb(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::ModePage));
    setActivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR + TEACHING_MODE_PAGE_ACTIVATE_ICON_NAME);
    setDeactivateIconPath(m_resource->imagePath() + TEACHING_VIEW_ICON_DIR +TEACHING_MODE_PAGE_DEACTIVATE_ICON_NAME);

    initList();
    readDataFormatCsv();
    updateCurrentModeList();
    // Detect language changes to update labels.
    connect(m_resource, &ResourceManager::languageChanged, this, &TeachingModePageModel::onLanguageChanged, Qt::QueuedConnection);
    connect(m_commonView, &CommonViewModel::userLevelChanged, this, &TeachingModePageModel::onUserLevelChanged, Qt::QueuedConnection);
}

void TeachingModePageModel::initList()
{
    getModeFormatByUserLevel();
    for (int i = 0; i < MODE_NUM; i++) {
        TeachingModeData *data = new TeachingModeData();
        m_modeDataList.append(data);
    }
}

void TeachingModePageModel::updatePenMsgModeData()
{
    TeachingModeData *data;
    int descriptionRow = PEN_MSG_DESCTIPTION_START_ROW;
    for (int i = 0; i < MODE_NUM_MAX; i++) {
        data = static_cast<TeachingModeData *>(m_modeDataList.at(i));
        int row = i * 3 + 1;
        data->setLabelData(m_resource->getMessageStringValue("1",data->di(),m_resource->language()) , m_resource->rgb(1,data->di()));
        data->setLabel1Data(m_resource->symbolAt(data->di()), m_resource->rgb(1,data->di()));
        //data->setLabelData(m_resource->label(PEN_MSG_GROUP_MODE_PAGE, row), m_resource->rgb(PEN_MSG_GROUP_MODE_PAGE, row));
        row++;
        data->setDescriptionData(m_resource->label(PEN_MSG_GROUP_MODE_PAGE, descriptionRow), m_resource->rgb(PEN_MSG_GROUP_MODE_PAGE, descriptionRow));
        descriptionRow++;
    }
}

void TeachingModePageModel::getModeFormatByUserLevel()
{
    if(!m_resource) return;
    QString pageNumber = PEN_SET_GROUP_MODE_PAGE;
    for(int i = 0; i < MAX_USER_LEVEL; i++){
        mCurModeCnt[i] = 0;
        //m_userLevelModeDisp[i] = m_resource->formatList(pageNumber.toInt(), modePenSetIndexByUserLevel[i], 8, true, false);
        m_userLevelModeDisp[i] = m_resource->formatList(pageNumber.toInt(), modePenSetIndexByUserLevel[i], 8, false);
        for(int j = 0; j < m_userLevelModeDisp[i].count(); j++){
            for(int k = 0; k < 8; k++){
                mCurModeCnt[i] += ((m_userLevelModeDisp[i].at(j).value(k) != INVALID_MODE_NUM_BY_USER_LEVEL
                                    && m_userLevelModeDisp[i].at(j).value(k) != 0) ? 1 : 0);
            }
        }
        //qDebug("%s:%d mCurModeCnt[%d]", __func__, __LINE__, mCurModeCnt[i]);
    }
}

void TeachingModePageModel::readPenMsgData()
{
    updatePenMsgModeData();
}

void TeachingModePageModel::readPenSetData()
{
    TeachingModeData *data;
    int row = 0;
    int keyNo = 0;
    int bitIndex = 0;
    int bitIndex1 = 0;
    int key_on = 0;
    int key_off = 0;
    for (int i = 0; i < MODE_NUM; i++) {
        if(i >= 64){
            return;
        }
        row = PEN_SET_KEY_DI_START_ROW + i;
        keyNo = m_resource->getPenSetIntValue(PEN_SET_GROUP_MODE_PAGE, row, PEN_SET_COLUMN_KEY_NO);
        bitIndex = m_resource->getPenSetIntValue(PEN_SET_GROUP_MODE_PAGE, row, PEN_SET_COLUMN_KEY_NO) - 1;
        bitIndex1 = m_resource->getPenSetIntValue(PEN_SET_GROUP_MODE_PAGE, row, PEN_SET_COLUMN_KEY_NO + 1);
        key_on = m_resource->getPenSetIntValue(PEN_SET_GROUP_MODE_PAGE, row, PEN_SET_COLUMN_KEY_NO + 3);
        key_off = m_resource->getPenSetIntValue(PEN_SET_GROUP_MODE_PAGE, row, PEN_SET_COLUMN_KEY_NO + 4);

        data = static_cast<TeachingModeData *>(m_modeDataList.at(i));
        data->setKeyNo(keyNo);
        data->setDi(bitIndex1);
        data->setKey_on(key_on);
        data->setKey_off(key_off);
        m_diModeIndexConversionMap.insert(bitIndex, i);
    }
}

void TeachingModePageModel::readDataFormatCsv()
{
    readPenSetData();
    readPenMsgData();
}

void TeachingModePageModel::onActivate()
{
    connect(m_communication, &CommunicationThread::finish, this, &TeachingModePageModel::onFinished, Qt::QueuedConnection);
    connect(m_modbus, &ModbusManager::endModbusPageChange, this, &TeachingModePageModel::endModbusPageChange, Qt::DirectConnection);
}

void TeachingModePageModel::onDeactivate()
{
    disconnect(m_modbus, &ModbusManager::endModbusPageChange, this, &TeachingModePageModel::endModbusPageChange);
    disconnect(m_communication, &CommunicationThread::finish, this, &TeachingModePageModel::onFinished);
}

int TeachingModePageModel::getKeyNo(int modeDataListIndex)
{
    if (modeDataListIndex >= m_currentModeList.length()) {
        return -1;
    }
    TeachingModeData *data = static_cast<TeachingModeData *>(m_currentModeList.at(modeDataListIndex));
    return data->keyNo();
}

void TeachingModePageModel::setModeDataList(const QList<QObject *> &list)
{
    if (m_modeDataList != list) {
        m_modeDataList = list;
        emit modeDataListChanged(m_modeDataList);
    }
}

void TeachingModePageModel::setCurrentModeList(const QList<QObject *>& list)
{
    if (m_currentModeList != list) {
        m_currentModeList = list;
        emit currentModeListChanged(m_currentModeList);
    }
}

void TeachingModePageModel::setCurrentPage(int page)
{
    if ((page < 0) || (page > MODE_PAGE_MAX))
        return;

    if (m_currentPage != page) {
        m_currentPage = page;
        emit currentPageChanged(m_currentPage);
    }
}

void TeachingModePageModel::updateCurrentModeList()
{
    if (mUserLevel < 0 || mUserLevel >= MAX_USER_LEVEL){
        return;
    }

    m_currentModeList.clear();
    const int modeNum = m_modeDataList.length();
    int end = mCurModeCnt[mUserLevel];
    for (int i = 0; i < end; i++) {
        int modeKeyNo = m_userLevelModeDisp[mUserLevel].at(i / 8).value(i % 8);
        if ((modeKeyNo == INVALID_MODE_NUM_BY_USER_LEVEL) || (modeKeyNo == 0) || (modeKeyNo > modeNum)) continue;
        int modeKeyIndex = modeKeyNo - 1;
        m_currentModeList.append(m_modeDataList.at(modeKeyIndex));
    }
    emit currentModeListChanged(m_currentModeList);
}

void TeachingModePageModel::setTeachingModeStatus(int bitIndex, int status)
{
    int modeListIndex;
    if (m_diModeIndexConversionMap.contains(bitIndex)) {
        modeListIndex = m_diModeIndexConversionMap.value(bitIndex);
        static_cast<TeachingModeData *>(m_modeDataList.at(modeListIndex))->setStatus(status);
    }
}

void TeachingModePageModel::modbusUpdateModeStatus(bool modeStatus, int modeNo)
{
    if (!m_modbus || !m_communication) return;
    short hr = GET_MODE_HR_NUMBER(modeNo);
    if (hr < 0){
        qDebug("%s:%d modeNo[%d] is invalid", __func__, __LINE__, modeNo);
        return;
    }
    short sr = GET_MODE_SR_NUMBER(modeNo);
    if (sr < 0){
        qDebug("%s:%d modeNo[%d] is invalid", __func__, __LINE__, modeNo);
        return;
    }

    int modeState = m_modbus->getReceiveDataRG((quint16)sr);

    int modeNoIndex = ((modeNo - 1 ) % 16);
    if (modeStatus){
        modeState = (modeState | (1 << (15 - modeNoIndex)));
    } else {
        modeState = (modeState & ~(1 << (15 - modeNoIndex)));
    }
//    qDebug("%s:%d hr[%d] modeState[%d][0x%04x]", __func__, __LINE__, hr, modeState, modeState);
    QList<int> writeData;
    writeData.append((int)modeState);
    stModBusCommunicationData command =
        ModbusCommandGenerator::makeModbusWriteCommand(
            QModbusPdu::FunctionCode::WriteSingleRegister, (quint16)hr, 1, writeData);
    m_communication->setModbusWriteCommand(command);
#ifndef REAL_TARGET
    {
        QList<int> writeData;
        writeData.append((int)modeState);
        stModBusCommunicationData command =
            ModbusCommandGenerator::makeModbusWriteCommand(
                QModbusPdu::FunctionCode::WriteSingleRegister, (quint16)sr, 1, writeData);
        m_communication->setModbusWriteCommand(command);
    }
#endif
}

void TeachingModePageModel::onFinished()
{

    if (m_pageChanging == false && m_modbus->getIsPageChanging() == false) {
        quint16 sr = MODBUS_ADDRESS_MODE_STATUS;
        for (int i = 0; i < MODBUS_NOD_MODE_STATUS; i++) {
            for(int j = 0 ; j < 16; j++){
                setTeachingModeStatus(i * 16 + j, ((m_modbus->getReceiveDataRG(sr) >> (16 - (j + 1))) & 0x01));
            }
            sr++;
        }
        setDataLoaded(true);
    }
    setMode64State(static_cast<TeachingModeData *>(m_modeDataList.at(63))->status());
    setMode31State(static_cast<TeachingModeData *>(m_modeDataList.at(30))->status());
    if(m_resource->axisorder().size() > 5) {
        for(int i = 5; i < 8; i++){
            AxisInfo::Axis axis = static_cast<AxisInfo::Axis>(i);
            QString position = m_resource->formatData((int)m_modbus->getLongReceiveDataRG(AXIS_3_POS_ADDR_LIST.at(i-5)),66, 2);
            m_axisInfo->setPosition(axis,position);
        }
    }
}

void TeachingModePageModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    setPageLabelName(m_resource->label(PEN_MSG_GROUP_TEACHING_VIEW, PenMsgTeachingGroupRow::ModePage));
    updatePenMsgModeData();
    emit currentModeListChanged(m_currentModeList);
}

void TeachingModePageModel::onUserLevelChanged(int userLevel)
{
    bool userLevelChanged = mUserLevel != userLevel;
    mUserLevel = userLevel;
    if (userLevelChanged) {
        setCurrentPage(0);
    }
    updateCurrentModeList();
    if(userLevel < m_resource->getPenSetIntValue("1",27,0)){
        setMode64State(0);
    }
}

void TeachingModePageModel::startPageChanging()
{
    setDataLoaded(false);
    m_pageChanging = true;
}

void TeachingModePageModel::setDataLoaded(bool dataLoaded)
{
    if(m_dataLoaded != dataLoaded){
        m_dataLoaded = dataLoaded;
        emit dataLoadedChanged(m_dataLoaded);
    }
}

void TeachingModePageModel::endModbusPageChange(int mainScreenId, int subScreenId)
{
    if ((mainScreenId == 10) && (subScreenId == 0)) {
        m_pageChanging = false;
    }
}

void TeachingModePageModel::setMode64State(int state)
{
    if(m_mode64State != state){
        m_mode64State = state;
        emit mode64StateChanged(m_mode64State);
    }
}

void TeachingModePageModel::setMode31State(int state)
{
    if(m_mode31State != state){
        m_mode31State = state;
        emit mode31StateChanged(m_mode31State);
    }
}

// ADD :  support animation behavior By.IJ.YI 25.07.29
int TeachingModePageModel::getFrameCount(int keyNo, const QString &dirName)
{
    if (keyNo <= 0 || keyNo > MODE_NUM_MAX)
        return 0;
    QDir baseDir(QCoreApplication::applicationDirPath());
    baseDir.cdUp();
    QString animPath = baseDir.absoluteFilePath("images/37_Mode_Animation/" + dirName);
    QDir dir(animPath);
    if (!dir.exists()) {
        qWarning() << "[getFrameCount] Animation folder not found:" << animPath;
        return 0;
    }
    dir.setNameFilters(QStringList() << "md*.png");
    int fileCount = dir.entryList().count();
    return fileCount > 0 ? fileCount - 1 : 0;  // 0~65 사용
}
