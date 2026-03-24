#include <QtDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <gtest/gtest.h>
#include "csvdocument.h"
#include "testhelpers.h"

TEST(CsvReaderTest, ParseUtf8LfCsv)
{
    const auto *const data = ";group,6,;画面2　手動操作,,\n"
                             ";group,7,;画面3　自動運転,,\n"
                             "long,,,\n"
                             "999999999,;生産数設定　最大値,,\n"
                             "word,,,\n"
                             "999,;1回取出数　最大値,,\n"
                             ";group,8,;画面4　メニュー,,\n"
                             "byte,,,\n"
                             "1,0,0,0\n"
                             "32,29,0,0\n";
    const QJsonObject expectedObj = {
        { "6", QJsonArray {} },
        { "7",
          QJsonArray {
                  QJsonArray { "long", "", "", "" },
                  QJsonArray { "999999999" },
                  QJsonArray { "word", "", "", "" },
                  QJsonArray { "999" },
          } },
        { "8",
          QJsonArray {
                  QJsonArray { "byte", "", "", "" },
                  QJsonArray { "1", "0", "0", "0" },
                  QJsonArray { "32", "29", "0", "0" },
          } },
    };
    CsvDocument doc;
    doc.parse(data, ",");
    EXPECT_EQ(doc.toObject(), expectedObj);
}

TEST(CsvReaderTest, ParseUtf8LfTsv)
{
    const auto *const data = ";group\t1\t\t\n"
                             "08_MENU/d080_12.png\tdummyFeature.qml\t;メニューなし\n"
                             "08_MENU/d080_12.png\tdummyFeature.qml\t;段取換\n"
                             "08_MENU/d080_12.png\tdummyFeature.qml\t;\n"
                             "10_MODE/d100\tSelectFunctionPage.qml\t;機能選択\n"
                             ";group\t2\t\n";
    const QJsonObject expectedObj = {
        { "1",
          QJsonArray {
                  QJsonArray { "08_MENU/d080_12.png", "dummyFeature.qml" },
                  QJsonArray { "08_MENU/d080_12.png", "dummyFeature.qml" },
                  QJsonArray { "08_MENU/d080_12.png", "dummyFeature.qml" },
                  QJsonArray { "10_MODE/d100", "SelectFunctionPage.qml" },
          } },
        { "2", QJsonArray {} },
    };
    CsvDocument doc;
    doc.parse(data, "\t");
    EXPECT_EQ(doc.toObject(), expectedObj);
}

TEST(CsvReaderTest, ParseUtf8CrlfTsv) // TODO: Utf16
{
    const auto *const data = ";group\t21\t\t\t\t\t;全画面共通下バー\r\n"
                             "\"原点\n復帰\"\t\"返回\n原点\"\t\"返回\n原點\"\t\"원점\n복귀\"\tORG\t"
                             "\t;原点復帰ボタン名称\r\n"
                             "\"手動\n操作\"\t\"手动\n操作\"\t\"手動\n操作\"\t\"수동\n조작\"\tMANU"
                             "\t\t;手動操作ボタン名称\r\n"
                             ";group\t22\t\t\t\t\t;全画面共通　軸名称\r\n"
                             "走行\t\t\t\t\t\t\r\n";
    const QJsonObject expectedObj = {
        { "21",
          QJsonArray {
                  QJsonArray { "原点\n復帰", "返回\n原点", "返回\n原點", "원점\n복귀", "ORG", "" },
                  QJsonArray { "手動\n操作", "手动\n操作", "手動\n操作", "수동\n조작", "MANU", "" },
          } },
        { "22",
          QJsonArray {
                  QJsonArray { "走行", "", "", "", "", "", "" },
          } },
    };
    CsvDocument doc;
    doc.parse(data, "\t");
    EXPECT_EQ(doc.toObject(), expectedObj);
}

TEST(CsvReaderTest, ParseInvalidGroupLine)
{
    const auto *const data = ";group\n" // should not crash because of .at(1)
                             ";group,1\n"
                             "foo\n"
                             ";group\n" // should not crash because of .at(1)
                             "bar\n"
                             ";group,2\n"
                             "baz\n";
    const QJsonObject expectedObj = {
        { "1",
          QJsonArray {
                  QJsonArray { "foo" },
                  QJsonArray { "bar" },
          } },
        { "2",
          QJsonArray {
                  QJsonArray { "baz" },
          } },
    };
    CsvDocument doc;
    doc.parse(data, ",");
    EXPECT_EQ(doc.toObject(), expectedObj);
}

TEST(CsvReaderTest, ParseEmptyContent)
{
    const QJsonObject expectedObj = {};
    CsvDocument doc;
    doc.parse("", ",");
    EXPECT_EQ(doc.toObject(), expectedObj);
}
