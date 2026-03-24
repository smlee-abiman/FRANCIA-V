#include "libcsvreader.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QTextCodec>
#include <algorithm>

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

LibCSVReader::LibCSVReader(const QString &settingsPath, QObject *parent)
    : QObject(parent), m_settingsPath(settingsPath)
{
    parseLanguage();
    parseLayout();
    parseModbusAddress();
    parseFunctionList();
    parseReturnOrigin();
    parseSymbolTable();
}

void LibCSVReader::parseLanguage()
{
    QDir dir(m_settingsPath + QStringLiteral("/msg_folder"));
    QStringList filefilter;
    filefilter << "*.txt";
    QStringList fileL = dir.entryList(filefilter, QDir::Files);

    // not building hash table here because that would be expensive
    for (auto filename : fileL) {
        m_language.parse(readFile(dir.filePath(filename)), "\t");
    }
}

void LibCSVReader::parseLayout()
{
    m_layout.parse(readFile(m_settingsPath + QStringLiteral("/set_folder/pen_set.csv")), ",");
}

void LibCSVReader::parseModbusAddress()
{
    m_modbusAddress.parse(
            readFile(m_settingsPath + QStringLiteral("/set_folder/MODBUS_address.csv")), ",");
}

void LibCSVReader::parseFunctionList()
{
    m_functionList.parse(readFile(m_settingsPath + QStringLiteral("/function_list.txt")), "\t");
}

void LibCSVReader::parseSymbolTable()
{
    auto data = readFile(m_settingsPath + QStringLiteral("/msg_folder/symbol_msg.csv"));
    QStringList datalist = data.split("\n", QString::SkipEmptyParts);
    for (QString line : datalist) {
        if (line.startsWith(";"))
            continue; // comment, ignore
        QStringList data = line.split(',');
        m_symTable.addRange(data.at(2).toInt(), data.at(1).toInt(), data.at(0));
    }
    m_symTable.finalizeTable();
}

void LibCSVReader::parseReturnOrigin()
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

        QVector<LibDataFormat> ax;
        for (int i = 6; i < 11; i++) { // layout data is from 8 to 12 in group 1
            ax.push_back(LibDataFormat::fromCode(m_layout.intValue("1", i, ix)));
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

    // Read Menu language order
    {
        for (auto idx : m_layout.intRowArray("8", 27)) {
            if (idx == 0)
                break;
            m_languageOrder.append(idx);
        }
    }

    {
        // Status Table units and format
        // find data in group32
        int step = 0;
        for (int i = 12; i < 21; i++) { // layout data is from 14 to 21 in group 1
            m_statusData.push_back(LibDataFormat::fromCode(m_layout.intValue("1", i + step, 0)));
        }
    }
}

int LibCSVReader::modBusAddress(int group, int row, int column)
{
    return m_modbusAddress.intValue(QString::number(group), row, column);
}

QJsonArray LibCSVReader::modBusAddressList(int group)
{
    return m_modbusAddress.values(QString::number(group)).toArray();
}

int LibCSVReader::stepWholeSpeed()
{
    return m_layout.intValue("1", 22, 0);
}

int LibCSVReader::minWholeSpeed()
{
    return m_layout.intValue("1", 23, 0);
}

int LibCSVReader::maxWholeSpeed()
{
    return m_layout.intValue("1", 24, 0);
}

int LibCSVReader::symIndexOfIO(int n)
{
    int ix = (n % 4);
    int iy = n / 4 + 31;
    return m_modbusAddress.intValue("1", iy, ix);
}

QString LibCSVReader::label(int group, int id, int language) const
{
    return m_language.stringValue(QString::number(group), id, language);
}
