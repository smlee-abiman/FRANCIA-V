#ifndef LIBCSVDOCUMENT_H
#define LIBCSVDOCUMENT_H

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QVector>

/// Thin wrapper around JSON object read from CSV/TSV spec file.
class LibCsvDocument
{
public:
    LibCsvDocument();
    explicit LibCsvDocument(const QJsonObject &object);

    QJsonObject toObject() const;
    void setObject(const QJsonObject &object);

    void parse(const QString &data, const QString &delimiter);

    QJsonArray rowArray(const QString &group, int row) const;
    QVector<int> intRowArray(const QString &group, int row) const;

    QString stringValue(const QString &group, int row, int column) const;
    int intValue(const QString &group, int row, int column) const;
    QJsonValue values(const QString &group) const;

    QStringList stringValues(const QString &group);
    QList<int> intValues(const QString &group);
    QList<uint> uintValues(const QString &group);
    QList<bool> boolValues(const QString &group);

private:
    QJsonObject m_jsonObject;
};

#endif // LIBCSVDOCUMENT_H
