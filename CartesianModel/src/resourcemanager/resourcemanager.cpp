/*
 * ResourceManager
 *
 * Pack label/message and data into one object and give a list of datas that UI requires
 *
 */

#include <QtDebug>
#include <QUrl>
#include "resourcemanager.h"
#include "corelibapplicationmanager.h"
#include "appenvironment.h"

namespace {
inline quint32 encodeLabelKey(int group, int id)
{
    Q_ASSERT(0 <= group && group < 0x10000);
    Q_ASSERT(0 <= id && id < 0x10000);
    return static_cast<quint32>(group) << 16 | static_cast<quint32>(id);
}
} // namespace

ResourceManager::ResourceManager(const QString &dataPath, QObject *parent)
    : QObject(parent),
      m_reader(),
      m_imagePath(QUrl::fromLocalFile(AppEnvironment::getImageFolderPath()).toString())
{
    Q_UNUSED(dataPath);

    m_labelsCache.reserve(100);

    initManualJogs();

    // Buttons on right side of Manual operation screen
    for (int i = 0; i < 12; ++i) {
        m_manualKeys.append(new PacketData());
    }

    prepareManualKeys();

    updateManualJogs(0);
    initAxisOrder();
}

ResourceManager::~ResourceManager()
{
    terminate();
    qDeleteAll(m_manualKeys);
    qDeleteAll(m_manualJogs);
}

/// Starts communication thread.
void ResourceManager::initialize()
{
    // NOP
}

/// Stops communication thread.
///
/// This blocks the current thread until the communication thread stops.
void ResourceManager::terminate()
{
    // NOP
}

CsvReader* ResourceManager::getCsvReaderInstance()
{
    return &m_reader;
}

void ResourceManager::updateMenuList(QList<int> receiveData)
{
    Q_ASSERT(receiveData.size() >= 70);
    QList<int> iconL1 = receiveData.mid(2, 12);
    QList<int> iconL2 = receiveData.mid(16, 12);
    QList<int> iconL3 = receiveData.mid(30, 12);
    QList<int> iconL4 = receiveData.mid(44, 12);
    QList<int> iconL5 = receiveData.mid(58, 12);

    QList<int> tmpIconAll = iconL1 + iconL2 + iconL3 + iconL4 + iconL5;

    QList<int> iconAll;
    for(int i = 0; i < tmpIconAll.size(); i++) {
        int value = tmpIconAll.at(i);
        iconAll.append(value & 0xFF);
        iconAll.append((value & 0xFF00) >> 8);
    }

    if (m_menu != iconAll) {
        m_menu = iconAll;
        emit menuChanged(m_menu);
    }
}

void ResourceManager::setLanguage(int language)
{
    if (m_language == language)
        return;
    m_language = language;
    m_labelsCache.clear();

    // retranslate labels embedded in models
    prepareManualKeys();
    updateManualJogs(m_manualSelectAxis);

    // Debug
    qDebug() << "ResourceManager::setLanguage m_language:" << m_language;
    emit languageChanged(m_language);
}

void ResourceManager::setMmPage(int mmPage)
{
    if (m_mmPage == mmPage)
        return;

    m_mmPage = mmPage;
    prepareManualKeys();
    emit mmPageChanged(m_mmPage);
}

void ResourceManager::setApplicationName(QString applicationName)
{
    if (m_applicationName == applicationName)
        return;

    m_applicationName = applicationName;
    emit applicationNameChanged(m_applicationName);
}

QString ResourceManager::unitFormat(int unit) const
{
    if (unit <= 0){
        return "";
    }
    return label(23, unit - 1);
}

/// Gives label from group(screen) ID and text ID cosidering current language setting.
QString ResourceManager::label(int group, int id) const
{
    auto k = encodeLabelKey(group, id);
    auto p = m_labelsCache.find(k);
    if (p == m_labelsCache.end()) {
        p = m_labelsCache.insert(k, m_reader.label(group, id, m_language));
    }
    return p.value();
}

QString ResourceManager::label(int group, int id, int language) const
{
    return m_reader.label(group, id, language);
}

QString ResourceManager::rgb(int group, int id) const
{
    auto k = encodeLabelKey(group, id);
    auto p = m_rgbCache.find(k);
    if (p == m_rgbCache.end()) {
        p = m_rgbCache.insert(k, m_reader.rgb(group, id));
    }
    return p.value();
}

QString ResourceManager::dataColor(int group, int id)
{
    auto k = encodeLabelKey(group, id);
    auto p = m_dataFieldColorCache.find(k);
    if (p == m_dataFieldColorCache.end()) {
        QString rgbStr;
        m_reader.parseRGB(&rgbStr, QString::number(group), id);
        p = m_dataFieldColorCache.insert(k, rgbStr);
    }
    return p.value();
}

QString ResourceManager::getSystemSettingAppDirPathNonRealTarget() {
    QString applicationPath = AppEnvironment::getApplicationFolderPath();
    QDir dir(applicationPath);
    dir.cdUp();
    dir.cdUp();
    dir.cd("bin");
    return dir.path();
};

void ResourceManager::changeApplication(QString nextAppPath, QString args) const
{
    CoreLibApplicationManager appManager;
    appManager.changeApplication(nextAppPath, args);
}

void ResourceManager::changeApplication(QString nextAppPath, QString args, QString afterNextAppPath, QString afterNextArgs) const
{
    CoreLibApplicationManager appManager;
    appManager.changeApplication(nextAppPath, args, afterNextAppPath, afterNextArgs);
}

bool ResourceManager::isLinux() const
{
    CoreLibApplicationManager appManager;
    return appManager.isLinux();
}

int ResourceManager::maxProduction() {
    return m_reader.maxProduction();
}

int ResourceManager::maxOnce() {
    return m_reader.maxOnce();
}

QString ResourceManager::getTopIconFilename(QString qmlFile)
{
    QString icon;
    auto object = functionList();
    auto array = object.value("1").toArray();
    foreach(QJsonValue value, array){
        if((value[1].toString().length() > 0) &&
           (qmlFile.contains(value[1].toString())))
        {
            icon = imagePath() + "/" + value[2].toString();
            break;
        }
    }
    return icon;
}

QString ResourceManager::getUnitFromFormat(int unit)
{
    QString unitStr = "";
    switch(unit) {
    case 1:
        unitStr = label(23, 0);
        break;
    case 2:
        unitStr = label(23, 1);
        break;
    case 3:
        unitStr = label(23, 2);
        break;
    case 4:
        unitStr = label(23, 3);
        break;
    case 5:
        unitStr = label(23, 4);
        break;
    case 6:
        unitStr = label(23, 5);
        break;
    case 7:
        unitStr = label(23, 6);
        break;
    case 8:
        unitStr = label(23, 7);
        break;
    case 9:
        unitStr = label(23, 8);
        break;
    case 10:
        unitStr = label(23, 22);
        break;
    default:
        // NOP
        break;
    }
    return unitStr;
}

void ResourceManager::prepareManualKeys()
{
    if (m_mmPage >= 0) {
        for (int i = 0; i < 12; ++i) {
            PacketData *p = reinterpret_cast<PacketData *>(m_manualKeys.at(i));
            p->setLabel(label(7, m_mmPage * 10 + i));
            p->setValue(QString::number(20 + m_mmPage * 10 + i));
        }
    }
}

void ResourceManager::initManualJogs()
{
    for(int i = 0; i < 8; i++) {
        m_manualJogs.append(new PacketData());
    }
}

void ResourceManager::initAxisOrder()
{
    QVector<int> axisOrderVector = getIntRowArray("1", 15);
    for(int i = 0; i < axisOrderVector.size(); i++) {
        int order = axisOrderVector.at(i);
        if ((order < 0) || (order > Enums::AxisNum)) {
            qWarning("%s:%d Invalid axis order. [%d][%d]", __func__, __LINE__, i, order);
            continue;
        }
        if (order == 0) {
            continue;
        }
        m_AxisOrder.append(order - 1);
    }
}

void ResourceManager::updateManualJogs(int axisSelectNo)
{
    m_manualSelectAxis = axisSelectNo;

    for(int i = 0; i < 8; i++) {
        PacketData *p = reinterpret_cast<PacketData *>(m_manualJogs.at(i));

        switch(i) {
        case 0:
            switch(axisSelectNo) {
            case 0: // joint
                p->setLabel(label(31, 5));
                p->setColor(rgb(31, 5));
                break;
            case 1: // xyz
                p->setLabel(label(31, 10));
                p->setColor(rgb(31, 10));
                break;
            case 2: // cylinder
                p->setLabel(label(31, 15));
                p->setColor(rgb(31, 15));
                break;
            case 3: // tool
                p->setLabel(label(31, 20));
                p->setColor(rgb(31, 20));
                break;
            }
            break;
        case 1:
            switch(axisSelectNo) {
            case 0: // joint
                p->setLabel(label(31, 6));
                p->setColor(rgb(31, 6));
                break;
            case 1: // xyz
                p->setLabel(label(31, 11));
                p->setColor(rgb(31, 11));
                break;
            case 2: // cylinder
                p->setLabel(label(31, 16));
                p->setColor(rgb(31, 16));
                break;
            case 3: // tool
                p->setLabel(label(31, 21));
                p->setColor(rgb(31, 21));
                break;
            }
            break;
        case 2:
            switch(axisSelectNo) {
            case 0: // joint
                p->setLabel(label(31, 7));
                p->setColor(rgb(31, 7));
                break;
            case 1: // xyz
                p->setLabel(label(31, 12));
                p->setColor(rgb(31, 12));
                break;
            case 2: // cylinder
                p->setLabel(label(31, 17));
                p->setColor(rgb(31, 17));
                break;
            case 3: // tool
                p->setLabel(label(31, 22));
                p->setColor(rgb(31, 22));
                break;
            }
            break;
        case 3:
            switch(axisSelectNo) {
            case 0: // joint
                p->setLabel(label(31, 8));
                p->setColor(rgb(31, 8));
                break;
            case 1: // xyz
                p->setLabel(label(31, 13));
                p->setColor(rgb(31, 13));
                break;
            case 2: // cylinder
                p->setLabel(label(31, 18));
                p->setColor(rgb(31, 18));
                break;
            case 3: // tool
                p->setLabel(label(31, 23));
                p->setColor(rgb(31, 23));
                break;
            }
            break;
        case 4:
            switch(axisSelectNo) {
            case 0: // joint
                p->setLabel(label(31, 9));
                p->setColor(rgb(31, 9));
                break;
            case 1: // xyz
                p->setLabel(label(31, 14));
                p->setColor(rgb(31, 14));
                break;
            case 2: // cylinder
                p->setLabel(label(31, 19));
                p->setColor(rgb(31, 19));
                break;
            case 3: // tool
                p->setLabel(label(31, 24));
                p->setColor(rgb(31, 24));
                break;
            }
            break;
        default:
            break;
        }
    }
}

QList<QList<quint16>> ResourceManager::formatList(int group, int startId, int count, bool isZeroValueSkip)
{
    QList<QList<quint16>> list;
    QString g = QString::number(group);
    for(int i = 0; i < count; i++) {
        m_reader.parseDataFormat(&list, g, startId + i, isZeroValueSkip);
    }
    return list;
}

QList<QList<int>> ResourceManager::getIntValueList(int group, int startId, int count, bool isZeroValueSkip)
{
    QList<QList<int>> list;
    QString g = QString::number(group);
    for(int i = 0; i < count; i++) {
        m_reader.parseDataFormat(&list, g, startId + i, isZeroValueSkip);
    }
    return list;
}

QList<QList<int>> ResourceManager::getIntHexValueList(int group, int startId, int count, bool isZeroValueSkip)
{
    QList<QList<int>> list;
    QString g = QString::number(group);
    for(int i = 0; i < count; i++) {
        m_reader.parseDataFormatHex(&list, g, startId + i, isZeroValueSkip);
    }
    return list;
}

QString ResourceManager::doFormatData(int data, DataFormat dataFormat, int shift, bool isDefaultDisp)
{
    if (isDefaultDisp && data == mDefaultValuePtn2){
        return mDefaultValueStr;
    }
    if (data == mDefaultValuePtn1){
        return mDefaultValueStr;
    }
    QString value = formatValue(data, dataFormat, shift);
    QString unit = getUnitFromFormat(dataFormat.unit);
    return unit.isEmpty() ? value : (value + " " + unit);
}

QString ResourceManager::formatData(int data, int format)
{
    if(format == 0) {
        return "";
    }
    DataFormat dataFormat = DataFormat::fromCode(format);
    return doFormatData(data, dataFormat, dataFormat.fraction);
}

QString ResourceManager::formatData(int data, int format, int shift, bool isDefaultDisp)
{
    if(format == 0) {
        return "";
    }
    if(format == 255){
        format = 66;
    }
    DataFormat dataFormat = DataFormat::fromCode(format);
    return doFormatData(data, dataFormat, shift, isDefaultDisp);
}

QString ResourceManager::formatData(short data, int format, int shift)
{
    if(format == 0) {
        return "";
    }
    DataFormat dataFormat = DataFormat::fromCode(format);
    QString value = formatValue(data, dataFormat, shift);
    QString unit = getUnitFromFormat(dataFormat.unit);
    return unit.isEmpty() ? value : (value + " " + unit);
}

bool ResourceManager::formatIsDecimals(int format)
{
    if(format == 0) {
        return false;
    }
    DataFormat dataFormat = DataFormat::fromCode(format);

    return (0 < dataFormat.fraction ? true : false);
}

//テスト用 csvを読み込み直す処理
void ResourceManager::reloadCsvReader(){
    m_reader.reload();

    m_dataFieldColorCache.clear();
    m_labelsCache.clear();
    m_rgbCache.clear();
}
