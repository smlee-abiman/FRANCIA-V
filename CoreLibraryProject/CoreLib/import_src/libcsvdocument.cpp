#include <QStringList>
#include "libcsvdocument.h"

namespace {
QString detectLineEnding(const QString &data)
{
    int index = data.indexOf('\n');
    if (index <= 0) {
        return QStringLiteral("\n");
    } else if (data.at(index - 1) == '\r') {
        return QStringLiteral("\r\n");
    } else {
        return QStringLiteral("\n");
    }
}
} // namespace

LibCsvDocument::LibCsvDocument() : m_jsonObject() {}

LibCsvDocument::LibCsvDocument(const QJsonObject &object) : m_jsonObject(object) {}

QJsonObject LibCsvDocument::toObject() const
{
    return m_jsonObject;
}

void LibCsvDocument::setObject(const QJsonObject &object)
{
    m_jsonObject = object;
}

/// Parses CSV/TSV-like data and appends rows.
void LibCsvDocument::parse(const QString &data, const QString &delimiter)
{
    // We can't simply split the data by LF and chop CR because multi-line CSV
    // cell may contain LF, and each line will be terminated with CR+LF in that
    // case.
    QStringList datalist = data.split(detectLineEnding(data), QString::SkipEmptyParts);
    QJsonArray groupRows;
    QString currentGroupIndex;
    for (const auto &line : datalist) {
        auto row = line.split(delimiter);
        for (auto &s : row) {
            if (s.size() >= 2 && s.startsWith('"') && s.endsWith('"')) {
                s = s.mid(1, s.size() - 2); // strip quotes
            }
        }

        // [";group", index]
        if (row.size() >= 2 && row.at(0) == ";group") {
            if (!currentGroupIndex.isNull()) {
                m_jsonObject.insert(currentGroupIndex, groupRows);
                groupRows = {};
            }
            currentGroupIndex = row.at(1);
            continue;
        }

        if (currentGroupIndex.isNull())
            continue;

        // Strip comments: [..., ";...", ...]
        auto p = std::find_if(row.begin(), row.end(), [](auto &s) { return s.startsWith(';'); });
        if (p != row.end()) {
            row.erase(p, row.end());
            // Omit [";...", ...] row at all, but an empty row [] (with no
            // comment) should be preserved.
            if (row.empty())
                continue;
        }

        groupRows.append(QJsonArray::fromStringList(row));
    }

    // insert the last group in a file.
    if (!currentGroupIndex.isNull()) {
        m_jsonObject.insert(currentGroupIndex, groupRows);
    }
}

/// Returns data of the specified row; or empty QJsonArray if out of range.
QJsonArray LibCsvDocument::rowArray(const QString &group, int row) const
{
    return m_jsonObject.value(group).toArray().at(row).toArray();
}

/// Returns data of the specified row as ints; or empty QVector if the index
/// is out of range.
///
/// Unparsable value will be mapped to 0.
QVector<int> LibCsvDocument::intRowArray(const QString &group, int row) const
{
    auto array = rowArray(group, row);
    QVector<int> values;
    values.reserve(array.size());
    for (auto v : array) {
        values.push_back(v.toString().toInt());
    }
    return values;
}

/// Returns value in the specified cell; or null QString if out of range.
QString LibCsvDocument::stringValue(const QString &group, int row, int column) const
{
    return rowArray(group, row).at(column).toString();
}

/// Returns integer value in the specified cell; or 0 if the index is out of
/// range or the value can't be parsed as integer.
int LibCsvDocument::intValue(const QString &group, int row, int column) const
{
    return stringValue(group, row, column).toInt();
}


QJsonValue LibCsvDocument::values(const QString &group) const
{
    return m_jsonObject.value(group);
}

QStringList LibCsvDocument::stringValues(const QString &group){
    QStringList ret;

    foreach (QJsonValue rows, m_jsonObject.value(group).toArray()) {
        foreach(QJsonValue col, rows.toArray()){
            ret.append(col.toString());
        }
    }

    return ret;
}

QList<int> LibCsvDocument::intValues(const QString &group){
    QList<int> ret;

    foreach (QJsonValue rows, m_jsonObject.value(group).toArray()) {
        foreach(QJsonValue col, rows.toArray()){
            ret.append(col.toString().toInt());
        }
    }

    return ret;
}

QList<uint> LibCsvDocument::uintValues(const QString &group){
    QList<uint> ret;

    foreach (QJsonValue rows, m_jsonObject.value(group).toArray()) {
        foreach(QJsonValue col, rows.toArray()){
            ret.append(col.toString().toInt());
        }
    }

    return ret;
}

QList<bool> LibCsvDocument::boolValues(const QString &group){
    QList<bool> ret;

    foreach (QJsonValue rows, m_jsonObject.value(group).toArray()) {
        foreach(QJsonValue col, rows.toArray()){
            QString str = col.toString().trimmed();
            if(str == "true"){
                ret.append(true);
            }
            else{
                ret.append(false);
            }
        }
    }

    return ret;
}
