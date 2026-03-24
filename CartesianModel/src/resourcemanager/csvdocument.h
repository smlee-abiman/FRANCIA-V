#ifndef CSVDOCUMENT_H
#define CSVDOCUMENT_H

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QVector>

/// Thin wrapper around JSON object read from CSV/TSV spec file.
class CsvDocument
{
public:
    CsvDocument();
    explicit CsvDocument(const QJsonObject &object);

    QJsonObject toObject() const;
    void setObject(const QJsonObject &object);

    void parse(const QString &data, const QString &delimiter);

    QJsonArray rowArray(const QString &group, int row) const;
    QVector<int> intRowArray(const QString &group, int row) const;
    QVector<QString> stringRowArray(const QString &group, int row) const;

    QString stringValue(const QString &group, int row, int column) const;
    int intValue(const QString &group, int row, int column) const;
    double doubleValue(const QString &group, int row, int column) const;

    QString getSettingVersion();

    //For Test
    void clear() { m_jsonObject = QJsonObject(); m_settingVersion = ""; }

private:
    QJsonObject m_jsonObject;
    QString m_settingVersion;
};

#endif // CSVDOCUMENT_H
