/*
 * This class handles value's representation. Like how many digits on real part, fraction part and
 * also its unit. Will be used when the value is displayed. The detail of this is comming from
 * layout data file (pen_set.csv)
 */

#ifndef LIBDATAFORMAT_H
#define LIBDATAFORMAT_H

#include <QMetaType>
#include <QString>

struct LibDataFormat
{
    int unit;
    int real;
    int fraction;

    static LibDataFormat fromCode(int format);
};

Q_DECLARE_METATYPE(LibDataFormat)

QString formatValue(int n, const LibDataFormat &fm, int shift);
QString formatValue(short n, const LibDataFormat &fm, int shift);

#endif // LIBDATAFORMAT_H
