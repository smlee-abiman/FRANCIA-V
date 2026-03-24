#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <iosfwd>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QtGlobal>

inline void PrintTo(const QString &s, std::ostream *os)
{
    *os << "QString(\"" << qUtf8Printable(s) << "\")";
}

inline void PrintTo(const QJsonObject &o, std::ostream *os)
{

    *os << "QJsonObject(" << QJsonDocument(o).toJson().constData() << ")";
}

#endif // TESTHELPERS_H
