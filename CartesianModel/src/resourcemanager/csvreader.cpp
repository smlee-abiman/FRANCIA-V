#include "csvreader.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QTextCodec>
#include <algorithm>
#include "appenvironment.h"

Q_LOGGING_CATEGORY(csvreader, "csvreader")

namespace {
QString readFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(csvreader) << "read error file name : " << filename
                             << "error info:" << file.errorString();
        return {};
    }
    QByteArray tst = file.read(2);
    file.seek(0);
    bool isUTF16 = (tst == QByteArrayLiteral("\xff\xfe"));
    if (isUTF16) {
        QByteArray ba = file.readAll();
        return QTextCodec::codecForMib(1015)->toUnicode(ba);
    } else {
        return QString::fromUtf8(file.readAll());
    }
}
} // namespace

CsvReader::CsvReader(QObject *parent)
    : QObject(parent)
{
    parseLanguage();
    parseLayout();
    parseModbusAddress();
    parseFunctionList();
    parseReturnOrigin();
    parseSymbolTable();
}

void CsvReader::parseLanguage()
{
    QDir dir(AppEnvironment::getMsgFolderPath());
    QStringList filefilter;
    filefilter << "*.txt";
    QStringList fileL = dir.entryList(filefilter, QDir::Files);

    // not building hash table here because that would be expensive
    for (auto filename : fileL) {
        m_language.parse(readFile(dir.filePath(filename)), "\t");
    }
}

void CsvReader::parseLayout()
{
    m_layout.parse(readFile(AppEnvironment::getSetfolderPath() + QStringLiteral("/pen_set.csv")), ",");
}

void CsvReader::parseModbusAddress()
{
    m_modbusAddress.parse(
            readFile(AppEnvironment::getSetfolderPath()+ QStringLiteral("/MODBUS_address.csv")), ",");
}

void CsvReader::parseFunctionList()
{
    m_functionList.parse(readFile(AppEnvironment::getSettingsFolderPath() + QStringLiteral("/function_list.txt")), "\t");
}

void CsvReader::parseSymbolTable()
{
    auto data = readFile(AppEnvironment::getMsgFolderPath() + QStringLiteral("/symbol_msg.csv"));
    QStringList datalist = data.split("\n", QString::SkipEmptyParts);
    for (QString line : datalist) {
        if (line.startsWith(";"))
            continue; // comment, ignore
        QStringList data = line.split(',');
        m_symTable.addRange(data.at(2).toInt(), data.at(1).toInt(), data.at(0));
    }
    m_symTable.finalizeTable();
}

void CsvReader::parseReturnOrigin()
{
    // the group 1 ,line 12
    // the layout data, which defined which line shoud show
    m_axisOrder.clear();
    int ix = 0;
    for (auto index : m_layout.intRowArray("1", 1)) {
        if (index == 0)
            continue;
        m_axisOrder.append(index - 1);
        // find data in group22

        QVector<DataFormat> ax;
        for (int i = 6; i < 11; i++) { // layout data is from 8 to 12 in group 1
            ax.push_back(DataFormat::fromCode(m_layout.intValue("1", i, ix)));
        }
        ++ix;
        m_axisDataVec.push_back(std::move(ax));
    }

    // Set maximum

    m_maxProduction = m_layout.intValue("7", 1, 0);
    m_maxOnce = m_layout.intValue("7", 3, 0);

    // Read Menu Table Layout
    {
        for (int ul = 0; ul < 5; ++ul) {
            for (int v = 0; v < 4; ++v) {
                auto formats = m_layout.intRowArray("8", ul * 5 + v + 1 + 1);
                for (int h = 0; h < std::min(6, formats.size()); ++h) {
                    m_menu.append(formats.at(h));
                }
            }
        }
    }

    {
        // Status Table units and format
        // find data in group32
        int step = 0;
        for (int i = 12; i < 21; i++) { // layout data is from 14 to 21 in group 1
            m_statusData.push_back(DataFormat::fromCode(m_layout.intValue("1", i + step, 0)));
        }
    }
}

int CsvReader::modBusAddress(int group, int row, int column)
{
    return m_modbusAddress.intValue(QString::number(group), row, column);
}

int CsvReader::symIndexOfIO(int n)
{
    int ix = (n % 4);
    int iy = n / 4 + 31;
    return m_modbusAddress.intValue("1", iy, ix);
}

QString CsvReader::label(int group, int id, int language) const
{    
    return m_language.stringValue(QString::number(group), id, language + 2);    // +2して先頭のRGBカラム、次のFontサイズカラムを読み飛ばす
}

QString CsvReader::rgb(int group, int id) const
{
    QString rbg = m_language.stringValue(QString::number(group), id, 0);
    if(rbg.length() == 0){
        return "#FFFFFF";
    }else{
        return "#" + rbg;
    }
}

void CsvReader::parseRGB(QString *rgbStr, QString group, int line)
{
    if(rgbStr == nullptr) return;
    *rgbStr = "#";
    for (int rgb : m_layout.intRowArray(group, line)) {
        *rgbStr += QString::number(rgb, 16).rightJustified(2, '0', true);
    }
}

void CsvReader::parseDataFormat(QList<QList<quint16>> *list, QString group, int line, bool isZeroValueSkip)
{
    if(list == nullptr) return;
    QList<quint16> dataFormatList;
    for (int dataFormat : m_layout.intRowArray(group, line)) {
        if(isZeroValueSkip && dataFormat == 0) continue;
        dataFormatList.append(dataFormat);
    }
    list->append(dataFormatList);
}

void CsvReader::parseDataFormat(QList<QList<int>> *list, QString group, int line, bool isZeroValueSkip)
{
    if(list == nullptr) return;
    QList<int> dataFormatList;
    for (int dataFormat : m_layout.intRowArray(group, line)) {
        if(isZeroValueSkip && dataFormat == 0) continue;
        dataFormatList.append(dataFormat);
    }
    list->append(dataFormatList);
}

void CsvReader::parseDataFormatHex(QList<QList<int>> *list, QString group, int line, bool isZeroValueSkip)
{
    if(list == nullptr) return;
    QList<int> dataFormatList;
    for (QString dataFormat : m_layout.stringRowArray(group, line)) {
        if(isZeroValueSkip && dataFormat == "") continue;
        bool isOk;
        int hex = dataFormat.toInt(&isOk, 16);
        if (!isOk) continue;
        dataFormatList.append(hex);
    }
    list->append(dataFormatList);
}

QString CsvReader::getPenMsgCsvVersion()
{
    return  m_language.getSettingVersion();
}

QString CsvReader::getPenSetCsvVersion()
{
    return  m_layout.getSettingVersion();
}

QString CsvReader::getMODBUSAddressCsvVersion()
{
    return  m_modbusAddress.getSettingVersion();
}

QString CsvReader::getPenSetStringValue(const QString &group, int row, int column)
{
    return m_layout.stringValue(group, row, column);
}

int CsvReader::getPenSetIntValue(const QString &group, int row, int column)
{
    return m_layout.intValue(group, row, column);
}

double CsvReader::getPenSetDoubleValue(const QString &group, int row, int column)
{
    return m_layout.doubleValue(group, row, column);
}

QString CsvReader::getMessageStringValue(const QString &group, int row, int language)
{
    return m_language.stringValue(group, row, language + 1);    // +1して先頭のRGBカラムを読み飛ばす
}

QVector<int> CsvReader::getIntRowArray(const QString &group, int row)
{
    return m_layout.intRowArray(group, row);
}

//For test
void CsvReader::reload()
{
    m_layout.clear();
    m_language.clear();
    parseLayout();
    parseLanguage();
}
