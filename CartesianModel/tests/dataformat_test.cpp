#include <QString>
#include <gtest/gtest.h>
#include "dataformat.h"
#include "testhelpers.h"

TEST(DataFormatTest, FormatPositiveWithoutFraction)
{
    const DataFormat fm { 0, /*real=*/3, /*fraction=*/0 };
    EXPECT_EQ(formatValue(0, fm, 0), QString("0"));
    EXPECT_EQ(formatValue(1, fm, 0), QString("1"));
    EXPECT_EQ(formatValue(9, fm, 0), QString("9"));
    EXPECT_EQ(formatValue(10, fm, 0), QString("10"));
    EXPECT_EQ(formatValue(12, fm, 0), QString("12"));
    EXPECT_EQ(formatValue(99, fm, 0), QString("99"));
    EXPECT_EQ(formatValue(100, fm, 0), QString("100"));
    EXPECT_EQ(formatValue(123, fm, 0), QString("123"));
    EXPECT_EQ(formatValue(999, fm, 0), QString("999"));
    EXPECT_EQ(formatValue(1000, fm, 0), QString("1000"));
    EXPECT_EQ(formatValue(1234, fm, 0), QString("1234"));
    EXPECT_EQ(formatValue(9999, fm, 0), QString("9999"));
    EXPECT_EQ(formatValue(10000, fm, 0), QString("10000"));
}

TEST(DataFormatTest, FormatNegativeWithoutFraction)
{
    const DataFormat fm { 0, /*real=*/3, /*fraction=*/0 };
    EXPECT_EQ(formatValue(-1, fm, 0), QString("-1"));
    EXPECT_EQ(formatValue(-9, fm, 0), QString("-9"));
    EXPECT_EQ(formatValue(-10, fm, 0), QString("-10"));
    EXPECT_EQ(formatValue(-12, fm, 0), QString("-12"));
    EXPECT_EQ(formatValue(-99, fm, 0), QString("-99"));
    EXPECT_EQ(formatValue(-100, fm, 0), QString("-100"));
    EXPECT_EQ(formatValue(-123, fm, 0), QString("-123"));
    EXPECT_EQ(formatValue(-999, fm, 0), QString("-999"));
    EXPECT_EQ(formatValue(-1000, fm, 0), QString("-1000"));
    EXPECT_EQ(formatValue(-1234, fm, 0), QString("-1234"));
    EXPECT_EQ(formatValue(-9999, fm, 0), QString("-9999"));
    EXPECT_EQ(formatValue(-10000, fm, 0), QString("-10000"));
}

TEST(DataFormatTest, FormatPositiveWithoutFractionButShift)
{
    const DataFormat fm { 0, /*real=*/2, /*fraction=*/0 };
    EXPECT_EQ(formatValue(0, fm, 1), QString("0"));
    EXPECT_EQ(formatValue(1, fm, 1), QString("0"));
    EXPECT_EQ(formatValue(9, fm, 1), QString("0"));
    EXPECT_EQ(formatValue(10, fm, 1), QString("1"));
    EXPECT_EQ(formatValue(12, fm, 1), QString("1"));
    EXPECT_EQ(formatValue(99, fm, 1), QString("9"));
    EXPECT_EQ(formatValue(100, fm, 1), QString("10"));
    EXPECT_EQ(formatValue(123, fm, 1), QString("12"));
    EXPECT_EQ(formatValue(999, fm, 1), QString("99"));
    EXPECT_EQ(formatValue(1000, fm, 1), QString("100"));
    EXPECT_EQ(formatValue(1234, fm, 1), QString("123"));
    EXPECT_EQ(formatValue(9999, fm, 1), QString("999"));
    EXPECT_EQ(formatValue(10000, fm, 1), QString("1000"));
}

TEST(DataFormatTest, FormatNegativeWithoutFractionButShift)
{
    const DataFormat fm { 0, /*real=*/2, /*fraction=*/0 };
    EXPECT_EQ(formatValue(-1, fm, 1), QString("0"));
    EXPECT_EQ(formatValue(-9, fm, 1), QString("0"));
    EXPECT_EQ(formatValue(-10, fm, 1), QString("-1"));
    EXPECT_EQ(formatValue(-12, fm, 1), QString("-1"));
    EXPECT_EQ(formatValue(-99, fm, 1), QString("-9"));
    EXPECT_EQ(formatValue(-100, fm, 1), QString("-10"));
    EXPECT_EQ(formatValue(-123, fm, 1), QString("-12"));
    EXPECT_EQ(formatValue(-999, fm, 1), QString("-99"));
    EXPECT_EQ(formatValue(-1000, fm, 1), QString("-100"));
    EXPECT_EQ(formatValue(-1234, fm, 1), QString("-123"));
    EXPECT_EQ(formatValue(-9999, fm, 1), QString("-999"));
    EXPECT_EQ(formatValue(-10000, fm, 1), QString("-1000"));
}

TEST(DataFormatTest, FormatPositiveWithFractionLowerThanShift)
{
    const DataFormat fm { 0, /*real=*/2, /*fraction=*/1 };
    EXPECT_EQ(formatValue(0, fm, 3), QString("0.0"));
    EXPECT_EQ(formatValue(1, fm, 3), QString("0.0"));
    EXPECT_EQ(formatValue(10, fm, 3), QString("0.0"));
    EXPECT_EQ(formatValue(99, fm, 3), QString("0.0"));
    EXPECT_EQ(formatValue(100, fm, 3), QString("0.1"));
    EXPECT_EQ(formatValue(101, fm, 3), QString("0.1"));
    EXPECT_EQ(formatValue(123, fm, 3), QString("0.1"));
    EXPECT_EQ(formatValue(999, fm, 3), QString("0.9"));
    EXPECT_EQ(formatValue(1000, fm, 3), QString("1.0"));
    EXPECT_EQ(formatValue(1001, fm, 3), QString("1.0"));
    EXPECT_EQ(formatValue(1234, fm, 3), QString("1.2"));
    EXPECT_EQ(formatValue(9999, fm, 3), QString("9.9"));
    EXPECT_EQ(formatValue(10000, fm, 3), QString("10.0"));
    EXPECT_EQ(formatValue(10001, fm, 3), QString("10.0"));
    EXPECT_EQ(formatValue(10010, fm, 3), QString("10.0"));
    EXPECT_EQ(formatValue(10100, fm, 3), QString("10.1"));
    EXPECT_EQ(formatValue(11000, fm, 3), QString("11.0"));
    EXPECT_EQ(formatValue(12345, fm, 3), QString("12.3"));
    EXPECT_EQ(formatValue(100000, fm, 3), QString("100.0"));
    EXPECT_EQ(formatValue(123456, fm, 3), QString("123.4"));
    EXPECT_EQ(formatValue(999999, fm, 3), QString("999.9"));
}

TEST(DataFormatTest, FormatNegativeWithFractionLowerThanShift)
{
    const DataFormat fm { 0, /*real=*/2, /*fraction=*/1 };
    EXPECT_EQ(formatValue(-1, fm, 3), QString("0.0"));
    EXPECT_EQ(formatValue(-10, fm, 3), QString("0.0"));
    EXPECT_EQ(formatValue(-99, fm, 3), QString("0.0"));
    EXPECT_EQ(formatValue(-100, fm, 3), QString("-0.1"));
    EXPECT_EQ(formatValue(-101, fm, 3), QString("-0.1"));
    EXPECT_EQ(formatValue(-123, fm, 3), QString("-0.1"));
    EXPECT_EQ(formatValue(-999, fm, 3), QString("-0.9"));
    EXPECT_EQ(formatValue(-1000, fm, 3), QString("-1.0"));
    EXPECT_EQ(formatValue(-1001, fm, 3), QString("-1.0"));
    EXPECT_EQ(formatValue(-1234, fm, 3), QString("-1.2"));
    EXPECT_EQ(formatValue(-9999, fm, 3), QString("-9.9"));
    EXPECT_EQ(formatValue(-10000, fm, 3), QString("-10.0"));
    EXPECT_EQ(formatValue(-10001, fm, 3), QString("-10.0"));
    EXPECT_EQ(formatValue(-10010, fm, 3), QString("-10.0"));
    EXPECT_EQ(formatValue(-10100, fm, 3), QString("-10.1"));
    EXPECT_EQ(formatValue(-11000, fm, 3), QString("-11.0"));
    EXPECT_EQ(formatValue(-12345, fm, 3), QString("-12.3"));
    EXPECT_EQ(formatValue(-100000, fm, 3), QString("-100.0"));
    EXPECT_EQ(formatValue(-123456, fm, 3), QString("-123.4"));
    EXPECT_EQ(formatValue(-999999, fm, 3), QString("-999.9"));
}

TEST(DataFormatTest, FormatPositiveWithSameFractionAndShift)
{
    const DataFormat fm { 0, /*real=*/1, /*fraction=*/2 };
    EXPECT_EQ(formatValue(0, fm, 2), QString("0.00"));
    EXPECT_EQ(formatValue(1, fm, 2), QString("0.01"));
    EXPECT_EQ(formatValue(9, fm, 2), QString("0.09"));
    EXPECT_EQ(formatValue(10, fm, 2), QString("0.10"));
    EXPECT_EQ(formatValue(12, fm, 2), QString("0.12"));
    EXPECT_EQ(formatValue(99, fm, 2), QString("0.99"));
    EXPECT_EQ(formatValue(100, fm, 2), QString("1.00"));
    EXPECT_EQ(formatValue(101, fm, 2), QString("1.01"));
    EXPECT_EQ(formatValue(123, fm, 2), QString("1.23"));
    EXPECT_EQ(formatValue(999, fm, 2), QString("9.99"));
    EXPECT_EQ(formatValue(1000, fm, 2), QString("10.00"));
    EXPECT_EQ(formatValue(1001, fm, 2), QString("10.01"));
    EXPECT_EQ(formatValue(1010, fm, 2), QString("10.10"));
    EXPECT_EQ(formatValue(1100, fm, 2), QString("11.00"));
    EXPECT_EQ(formatValue(1234, fm, 2), QString("12.34"));
}

TEST(DataFormatTest, FormatNegativeWithSameFractionAndShift)
{
    const DataFormat fm { 0, /*real=*/1, /*fraction=*/2 };
    EXPECT_EQ(formatValue(-1, fm, 2), QString("-0.01"));
    EXPECT_EQ(formatValue(-9, fm, 2), QString("-0.09"));
    EXPECT_EQ(formatValue(-10, fm, 2), QString("-0.10"));
    EXPECT_EQ(formatValue(-12, fm, 2), QString("-0.12"));
    EXPECT_EQ(formatValue(-99, fm, 2), QString("-0.99"));
    EXPECT_EQ(formatValue(-100, fm, 2), QString("-1.00"));
    EXPECT_EQ(formatValue(-101, fm, 2), QString("-1.01"));
    EXPECT_EQ(formatValue(-123, fm, 2), QString("-1.23"));
    EXPECT_EQ(formatValue(-999, fm, 2), QString("-9.99"));
    EXPECT_EQ(formatValue(-1000, fm, 2), QString("-10.00"));
    EXPECT_EQ(formatValue(-1001, fm, 2), QString("-10.01"));
    EXPECT_EQ(formatValue(-1010, fm, 2), QString("-10.10"));
    EXPECT_EQ(formatValue(-1100, fm, 2), QString("-11.00"));
    EXPECT_EQ(formatValue(-1234, fm, 2), QString("-12.34"));
}

TEST(DataFormatTest, FormatPositiveWithFractionHigherThanShift)
{
    const DataFormat fm { 0, /*real=*/1, /*fraction=*/3 };
    EXPECT_EQ(formatValue(0, fm, 2), QString("0.000"));
    EXPECT_EQ(formatValue(1, fm, 2), QString("0.010"));
    EXPECT_EQ(formatValue(9, fm, 2), QString("0.090"));
    EXPECT_EQ(formatValue(10, fm, 2), QString("0.100"));
    EXPECT_EQ(formatValue(12, fm, 2), QString("0.120"));
    EXPECT_EQ(formatValue(99, fm, 2), QString("0.990"));
    EXPECT_EQ(formatValue(100, fm, 2), QString("1.000"));
    EXPECT_EQ(formatValue(101, fm, 2), QString("1.010"));
    EXPECT_EQ(formatValue(123, fm, 2), QString("1.230"));
    EXPECT_EQ(formatValue(999, fm, 2), QString("9.990"));
    EXPECT_EQ(formatValue(1000, fm, 2), QString("10.000"));
    EXPECT_EQ(formatValue(1001, fm, 2), QString("10.010"));
    EXPECT_EQ(formatValue(1010, fm, 2), QString("10.100"));
    EXPECT_EQ(formatValue(1100, fm, 2), QString("11.000"));
    EXPECT_EQ(formatValue(1234, fm, 2), QString("12.340"));
}

TEST(DataFormatTest, FormatNegativeWithFractionHigherThanShift)
{
    const DataFormat fm { 0, /*real=*/1, /*fraction=*/3 };
    EXPECT_EQ(formatValue(-1, fm, 2), QString("-0.010"));
    EXPECT_EQ(formatValue(-9, fm, 2), QString("-0.090"));
    EXPECT_EQ(formatValue(-10, fm, 2), QString("-0.100"));
    EXPECT_EQ(formatValue(-12, fm, 2), QString("-0.120"));
    EXPECT_EQ(formatValue(-99, fm, 2), QString("-0.990"));
    EXPECT_EQ(formatValue(-100, fm, 2), QString("-1.000"));
    EXPECT_EQ(formatValue(-101, fm, 2), QString("-1.010"));
    EXPECT_EQ(formatValue(-123, fm, 2), QString("-1.230"));
    EXPECT_EQ(formatValue(-999, fm, 2), QString("-9.990"));
    EXPECT_EQ(formatValue(-1000, fm, 2), QString("-10.000"));
    EXPECT_EQ(formatValue(-1001, fm, 2), QString("-10.010"));
    EXPECT_EQ(formatValue(-1010, fm, 2), QString("-10.100"));
    EXPECT_EQ(formatValue(-1100, fm, 2), QString("-11.000"));
    EXPECT_EQ(formatValue(-1234, fm, 2), QString("-12.340"));
}

TEST(DataFormatTest, FormatPositiveZeroPadding)
{
    const DataFormat fm { 0, /*real=*/1, /*fraction=*/5 };
    EXPECT_EQ(formatValue(0, fm, 0), QString("0.00000"));
    EXPECT_EQ(formatValue(1, fm, 0), QString("1.00000"));
    EXPECT_EQ(formatValue(1, fm, 1), QString("0.10000"));
    EXPECT_EQ(formatValue(1, fm, 2), QString("0.01000"));
    EXPECT_EQ(formatValue(1, fm, 3), QString("0.00100"));
    EXPECT_EQ(formatValue(1, fm, 4), QString("0.00010"));
    EXPECT_EQ(formatValue(1, fm, 5), QString("0.00001"));
    EXPECT_EQ(formatValue(1, fm, 6), QString("0.00000"));
    EXPECT_EQ(formatValue(10, fm, 0), QString("10.00000"));
    EXPECT_EQ(formatValue(10, fm, 1), QString("1.00000"));
    EXPECT_EQ(formatValue(10, fm, 2), QString("0.10000"));
    EXPECT_EQ(formatValue(10, fm, 3), QString("0.01000"));
    EXPECT_EQ(formatValue(10, fm, 4), QString("0.00100"));
    EXPECT_EQ(formatValue(10, fm, 5), QString("0.00010"));
    EXPECT_EQ(formatValue(10, fm, 6), QString("0.00001"));
    EXPECT_EQ(formatValue(10, fm, 7), QString("0.00000"));
}

TEST(DataFormatTest, FormatNegativeZeroPadding)
{
    const DataFormat fm { 0, /*real=*/1, /*fraction=*/5 };
    EXPECT_EQ(formatValue(-1, fm, 0), QString("-1.00000"));
    EXPECT_EQ(formatValue(-1, fm, 1), QString("-0.10000"));
    EXPECT_EQ(formatValue(-1, fm, 2), QString("-0.01000"));
    EXPECT_EQ(formatValue(-1, fm, 3), QString("-0.00100"));
    EXPECT_EQ(formatValue(-1, fm, 4), QString("-0.00010"));
    EXPECT_EQ(formatValue(-1, fm, 5), QString("-0.00001"));
    EXPECT_EQ(formatValue(-1, fm, 6), QString("0.00000"));
    EXPECT_EQ(formatValue(-10, fm, 0), QString("-10.00000"));
    EXPECT_EQ(formatValue(-10, fm, 1), QString("-1.00000"));
    EXPECT_EQ(formatValue(-10, fm, 2), QString("-0.10000"));
    EXPECT_EQ(formatValue(-10, fm, 3), QString("-0.01000"));
    EXPECT_EQ(formatValue(-10, fm, 4), QString("-0.00100"));
    EXPECT_EQ(formatValue(-10, fm, 5), QString("-0.00010"));
    EXPECT_EQ(formatValue(-10, fm, 6), QString("-0.00001"));
    EXPECT_EQ(formatValue(-10, fm, 7), QString("0.00000"));
}
