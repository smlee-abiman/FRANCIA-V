#include <QQmlEngine>
#include "modepageviewmodel.h"

Label::Label(QObject *parent)
    : QObject(parent)
{

}

void Label::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged(m_name);
    }
}

void Label::setColor(const QString &color)
{
    if (m_color != color) {
        m_color = color;
        emit colorChanged(m_color);
    }
}

ModeData::ModeData(QObject *parent)
    : QObject(parent)
{
    m_label = new Label(this);
}

ModeData::ModeData(int choiceNum, QObject *parent)
    : QObject(parent)
{
    m_label = new Label(this);

    for (int i = 0; i < choiceNum; i++) {
        m_choices.append(new Label(this));
    }
}

void ModeData::setLabelName(const QString &name)
{
    m_label->setName(name);
}

void ModeData::setLabelData(const QString &name, const QString &color)
{
    m_label->setName(name);
    m_label->setColor(color);
}

int ModeData::setChoiceName(int index, const QString &name)
{
    if (m_choices.length() <= index)
        return -1;

    m_choices.at(index)->setName(name);
    return 0;
}

int ModeData::setChoiceData(int index, const QString &name, const QString &color)
{
    m_choices.at(index)->setName(name);
    m_choices.at(index)->setColor(color);
    return 0;
}

void ModeData::setLabel(Label *label)
{
    if (m_label != label) {
        m_label = label;
        emit labelChanged(label);
    }
}

void ModeData::setChoices(const QList<Label *> &choices)
{
    if (m_choices != choices) {
        m_choices = choices;
        emit choicesChanged(m_choices);
    }
}

void ModeData::setStatus(int status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged(m_status);
    }
}

void ModeData::setKeyNo(int no)
{
    m_keyNo = no;
}

void ModeData::setDi(int di)
{
    m_di = di;
}

ModePageViewModel::ModePageViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource, comm)
{
    init();
}

ModePageViewModel::~ModePageViewModel()
{
    disconnect(m_resource, &ResourceManager::languageChanged, this, &ModePageViewModel::onLanguageChanged);
}

void ModePageViewModel::init()
{
    initList();
    readDataFormatCsv();
    // Detect language changes to rename labels.
    connect(m_resource, &ResourceManager::languageChanged, this, &ModePageViewModel::onLanguageChanged, Qt::QueuedConnection);
}

void ModePageViewModel::initList()
{
    for (int i = 0; i < MODE_NUM; i++) {
        ModeData *data = new ModeData(MODE_CHOICE_NUM, this);
        m_currentModeList.append(data);
    }
}

void ModePageViewModel::readPenMsgData()
{
    ModeData *data;
    int id = PEN_SET_START_ID;
    for (int i = 0; i < MODE_NUM; i++) {
        data = m_currentModeList.at(i);
        data->setLabelData(m_resource->label(PEN_MSG_GROUP_MODE_PAGE, id), m_resource->rgb(PEN_MSG_GROUP_MODE_PAGE, id));
        id++;
        for (int j = 0; j < MODE_CHOICE_NUM; j++) {
            data->setChoiceData(i, m_resource->label(PEN_MSG_GROUP_MODE_PAGE, id), m_resource->rgb(PEN_MSG_GROUP_MODE_PAGE, id));
            id++;
        }
    }
}

void ModePageViewModel::readPenMsgNameData()
{
    ModeData *data;
    int id = PEN_SET_START_ID;
    for (int i = 0; i < MODE_NUM; i++) {
        data = m_currentModeList.at(i);
        data->setLabelName(m_resource->label(PEN_MSG_GROUP_MODE_PAGE, id++));
        for (int j = 0; j < MODE_CHOICE_NUM; j++) {
            data->setChoiceName(i, m_resource->label(PEN_MSG_GROUP_MODE_PAGE, id++));
        }
    }
}

void ModePageViewModel::readPenSetData()
{
    ModeData *data;
    int row = 0;
    int keyNo = 0;
    int di = 0;
    for (int i = 0; i < MODE_NUM; i++) {
        row = PEN_SET_START_ROW + i;
//        keyNo = m_resource->getPenSetIntValue(PEN_SET_GROUP_MODE_PAGE, row, PEN_SET_COLUMN_KEY_NO);
//        di = m_resource->getPenSetIntValue(PEN_SET_GROUP_MODE_PAGE, row, PEN_SET_COLUMN_DI);
        data = m_currentModeList.at(i);
        data->setKeyNo(keyNo);
        data->setDi(di);
        m_diModeIndexConversionMap.insert(di, i);
    }
}

void ModePageViewModel::readDataFormatCsv()
{
    readPenMsgData();
    readPenSetData();
}

void ModePageViewModel::activateView()
{
    connect(m_communication, &CommunicationThread::feed, this, &ModePageViewModel::onFeed, Qt::QueuedConnection);
}

void ModePageViewModel::deactivateView()
{
    disconnect(m_communication, &CommunicationThread::feed, this, &ModePageViewModel::onFeed);
}

int ModePageViewModel::setMode(int page, int index, int status)
{
    Q_UNUSED(page)
    Q_UNUSED(index)
    Q_UNUSED(status)

    return 0;
}

void ModePageViewModel::setCurrentModeList(const QList<ModeData *>& list)
{
    if (m_currentModeList != list) {
        m_currentModeList = list;
        emit currentModeListChanged(m_currentModeList);
    }
}

void ModePageViewModel::setCurrentPage(int page)
{
    if ((page < 0) || (page > MODE_PAGE_MAX))
        return;

    if (m_currentPage != page) {
        m_currentPage = page;
        emit currentPageChanged(m_currentPage);
        upateCurrentModeList();
    }
}

void ModePageViewModel::upateCurrentModeList()
{
    m_currentModeList.clear();
    int start = m_currentPage * MODE_NUM_PER_PAGE;
    int end = start + MODE_NUM_PER_PAGE;
    for (int i = start; i < end; i++) {
        m_currentModeList.append(m_modeDataList.at(i));
    }
    emit currentModeListChanged(m_currentModeList);
}

void ModePageViewModel::receiveModeData(quint16 *readBuf)
{
    int di = MODE_DI_START;
    int modeListIndex = 0;
    int status;
    for (int i = 0; i < MODE_NUM / 2; i++) {
        status = readBuf[i] >> 8;
        if (m_diModeIndexConversionMap.contains(di)) {
            modeListIndex = m_diModeIndexConversionMap.value(di);
            m_modeDataList.at(modeListIndex)->setStatus(status);
        }
        di++;
        status = (readBuf[i] & 0x00FF);
        if (m_diModeIndexConversionMap.contains(di)) {
            modeListIndex = m_diModeIndexConversionMap.value(di);
            m_modeDataList.at(modeListIndex)->setStatus(status);
        }
        di++;
    }
}

void ModePageViewModel::onFeed(stModBusCommunicationData command)
{
    if (command.readAddress == 0x0600) {
        receiveModeData(command.readBuf);
    }
}

void ModePageViewModel::onLanguageChanged(int language)
{
    Q_UNUSED(language)
    readPenMsgNameData();
}
