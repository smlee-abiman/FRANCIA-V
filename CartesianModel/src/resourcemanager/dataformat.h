/*
 * This class handles value's representation. Like how many digits on real part, fraction part and
 * also its unit. Will be used when the value is displayed. The detail of this is comming from
 * layout data file (pen_set.csv)
 */

#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <QMetaType>
#include <QString>

struct DataFormat
{
    int unit;
    int real;
    int fraction;

    static DataFormat fromCode(int format);
};

Q_DECLARE_METATYPE(DataFormat)

QString formatValue(int n, const DataFormat &fm, int shift);

#endif // DATAFORMAT_H
