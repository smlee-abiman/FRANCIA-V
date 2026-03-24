/***
 * Read the Csv file, and parse the language ,layout information, transfer to native data structure.
 *
 ***/

#ifndef LIBCSVREADER_H
#define LIBCSVREADER_H

#include <QObject>
#include <QFile>
#include <QMap>
#include <QStringList>
#include <QVariantMap>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QTimer>
#include <QDateTime>
#include <QVector>
#include "libcsvdocument.h"
#include "libdataformat.h"
#include "libsymboltable.h"

Q_DECLARE_LOGGING_CATEGORY(csvreader)

class LibCSVReader : public QObject
{
    Q_OBJECT

public:
    explicit LibCSVReader(const QString &settingsPath, QObject *parent = nullptr);

    const QVector<LibDataFormat> &statusData() const { return m_statusData; }

    QList<int> menu() const { return m_menu; }

    const QJsonObject functionList() const { return m_functionList.toObject(); }

    // Get list of axis index that will be shown in origin screen.
    const QList<int> &axisOrder() const { return m_axisOrder; }

    // List of axis data from Read address 0000 - 0002
    const QVector<QVector<LibDataFormat>> &axisDataVec() { return m_axisDataVec; }

    // Get list of languages that will be shown in Menu screen.
    QList<int> languageOrder() { return m_languageOrder; }

    // Get '生産予定数'
    int maxProduction() { return m_maxProduction; }
    // Get '1回取出数'
    int maxOnce() { return m_maxOnce; }

    QString label(int group, int id, int language) const;

    // Get Symbol name from symbol list from its ID
    Q_INVOKABLE QString symbolAt(int index) { return m_symTable.at(index); }

    // Get I/O name from symbol table
    int symIndexOfIO(int n);

    // Get modBus Address data from given group ID and row/column.
    int modBusAddress(int group, int row, int column);
    QJsonArray modBusAddressList(int group);

    // Whole speed parameters will be given layout sheet
    int stepWholeSpeed();
    int minWholeSpeed();
    int maxWholeSpeed();

private:
    // Read msg data and store JSON object
    void parseLanguage();
    // Read set file and store JSON object
    void parseLayout();
    // Read finction_list.csv file and store JSON object
    void parseFunctionList();
    // Read symbol_msg.csv file and store JSON object
    void parseSymbolTable();
    // Read MODBUS_address.csv file and store into JSON object
    void parseModbusAddress();

    void parseReturnOrigin();

    const QString m_settingsPath;
    LibCsvDocument m_language;
    LibCsvDocument m_layout;
    QVector<QVector<LibDataFormat>> m_axisDataVec;
    QVector<LibDataFormat> m_statusData;

    QList<int> m_menu;
    LibCsvDocument m_functionList;
    LibCsvDocument m_modbusAddress;

    QList<int> m_axisOrder;

    // Group 7
    int m_maxProduction = 0;
    int m_maxOnce = 0;

    QList<int> m_languageOrder;
    LibSymbolTable m_symTable;
};

#endif // LIBCSVREADER_H
