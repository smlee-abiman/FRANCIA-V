#include <QString>
#include "libdataformat.h"

/// Creates DataFormat from integer code.
LibDataFormat LibDataFormat::fromCode(int format)
{
    return {
        (format & 0xff00) >> 8, // unit
        (format & 0x0f0) >> 4, // real
        format & 0x0f, // fraction
    };
}

/// Converts data value to formatted string considering real/fraction part,
/// pre-calculation like 1/1000.
///
/// Both positive and negative numbers are rounded towards zero.
QString formatValue(int n, const LibDataFormat &fm, int shift)
{
    QString num = QString::number(n);

    // adjust precision of fraction part
    if (fm.fraction > shift) {
        num.append(QStringLiteral("0000000000").data(), fm.fraction - shift);
    } else if (fm.fraction < shift) {
        num.chop(shift - fm.fraction);
        if (num == QStringLiteral("-")) {
            num.clear(); // rounds towards zero
        }
    }

    if (fm.fraction <= 0) {
        if (num.isEmpty())
            return QStringLiteral("0");
        return num;
    } else {
        // insert decimal point and zeros as needed
        auto sigLen = static_cast<int>(num.startsWith('-'));
        if (num.length() <= sigLen + fm.fraction) {
            num.insert(sigLen, QStringLiteral("0.0000000000").data(),
                       2 + sigLen + fm.fraction - num.length());
        } else {
            num.insert(num.length() - fm.fraction, '.');
        }
        return num;
    }
}

QString formatValue(short n, const LibDataFormat &fm, int shift)
{
    QString num = QString::number(n);

    // adjust precision of fraction part
    if (fm.fraction > shift) {
        num.append(QStringLiteral("0000000000").data(), fm.fraction - shift);
    } else if (fm.fraction < shift) {
        num.chop(shift - fm.fraction);
        if (num == QStringLiteral("-")) {
            num.clear(); // rounds towards zero
        }
    }

    if (fm.fraction <= 0) {
        if (num.isEmpty())
            return QStringLiteral("0");
        return num;
    } else {
        // insert decimal point and zeros as needed
        auto sigLen = static_cast<int>(num.startsWith('-'));
        if (num.length() <= sigLen + fm.fraction) {
            num.insert(sigLen, QStringLiteral("0.0000000000").data(),
                       2 + sigLen + fm.fraction - num.length());
        } else {
            num.insert(num.length() - fm.fraction, '.');
        }
        return num;
    }
}
