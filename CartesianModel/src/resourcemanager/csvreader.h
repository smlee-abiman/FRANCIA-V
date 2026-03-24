/***
 * Read the Csv file, and parse the language ,layout information, transfer to native data structure.
 *
 ***/

#ifndef CSVREADER_H
#define CSVREADER_H

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
#include "csvdocument.h"
#include "dataformat.h"
#include "memorymodel.h"
#include "symboltable.h"

Q_DECLARE_LOGGING_CATEGORY(csvreader)

class CsvReader : public QObject
{
    Q_OBJECT

public:
    explicit CsvReader(QObject *parent = nullptr);

    const QVector<DataFormat> &statusData() const { return m_statusData; }

    QList<int> menu() const { return m_menu; }

    const QJsonObject functionList() const { return m_functionList.toObject(); }

    // Get list of axis index that will be shown in origin screen.
    const QList<int> &axisOrder() const { return m_axisOrder; }

    // List of axis data from Read address 0000 - 0002
    const QVector<QVector<DataFormat>> &axisDataVec() { return m_axisDataVec; }

    // Get '生産予定数'
    int maxProduction() { return m_maxProduction; }
    // Get '1回取出数'
    int maxOnce() { return m_maxOnce; }

    QString label(int group, int id, int language) const;

    QString rgb(int group, int id) const;

    // Get Symbol name from symbol list from its ID
    Q_INVOKABLE QString symbolAt(int index) { return m_symTable.at(index); }

    // Get ID from symbol list from its symbol name
    Q_INVOKABLE int indexAt(const QString &symbol) { return m_symTable.indexAt(symbol); }

    // Get I/O name from symbol table
    int symIndexOfIO(int n);

    // Get modBus Address data from given group ID and row/column.
    int modBusAddress(int group, int row, int column);

    void parseRGB(QString *rgbStr, QString group, int line);
    void parseDataFormat(QList<QList<quint16>> *list, QString group, int line, bool isZeroValueSkip = true);
    void parseDataFormat(QList<QList<int>> *list, QString group, int line, bool isZeroValueSkip = true);
    void parseDataFormatHex(QList<QList<int>> *list, QString group, int line, bool isZeroValueSkip = true);

    // csv file version infomation
    QString getPenMsgCsvVersion();
    QString getPenSetCsvVersion();
    QString getMODBUSAddressCsvVersion();

    QString getPenSetStringValue(const QString &group, int row, int column);
    double getPenSetDoubleValue(const QString &group, int row, int column);
    QVector<int> getIntRowArray(const QString &group, int row);

    Q_INVOKABLE int getPenSetIntValue(const QString &group, int row, int column);
    Q_INVOKABLE QString getMessageStringValue(const QString &group, int row, int language);

    //For test
    void reload();

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

    CsvDocument m_language;
    CsvDocument m_layout;
    QVector<QVector<DataFormat>> m_axisDataVec;
    QVector<DataFormat> m_statusData;

    QList<int> m_menu;
    CsvDocument m_functionList;
    CsvDocument m_modbusAddress;

    QList<int> m_axisOrder;

    // Group 7
    int m_maxProduction = 0;
    int m_maxOnce = 0;

    SymbolTable m_symTable;
};

#endif // CSVREADER_H
