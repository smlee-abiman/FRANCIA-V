#include <QString>
#include <gtest/gtest.h>
#include "symboltable.h"
#include "testhelpers.h"

TEST(SymbolTableTest, Empty)
{
    SymbolTable table;
    table.finalizeTable();
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.at(-1), QString());
    EXPECT_EQ(table.at(0), QString());
    EXPECT_EQ(table.at(1), QString());
}

TEST(SymbolTableTest, Hashable)
{
    SymbolTable table;
    table.addRange(1, 1, "Foo");
    table.addRange(3, 1, "Bar");
    table.finalizeTable();
    EXPECT_EQ(table.size(), 2);
    EXPECT_EQ(table.at(-1), QString());
    EXPECT_EQ(table.at(0), QString());
    EXPECT_EQ(table.at(1), QString("Foo"));
    EXPECT_EQ(table.at(2), QString());
    EXPECT_EQ(table.at(3), QString("Bar"));
    EXPECT_EQ(table.at(4), QString());
}

TEST(SymbolTableTest, Range)
{
    SymbolTable table;
    table.addRange(1, 2, "Foo?");
    table.addRange(3, 1, "Bar");
    table.addRange(4, 3, "?Baz");
    table.addRange(7, 2, "Quu?x");
    table.finalizeTable();
    EXPECT_EQ(table.size(), 8);
    EXPECT_EQ(table.at(-1), QString());
    EXPECT_EQ(table.at(0), QString());
    EXPECT_EQ(table.at(1), QString("Foo1"));
    EXPECT_EQ(table.at(2), QString("Foo2"));
    EXPECT_EQ(table.at(3), QString("Bar"));
    EXPECT_EQ(table.at(4), QString("1Baz"));
    EXPECT_EQ(table.at(5), QString("2Baz"));
    EXPECT_EQ(table.at(6), QString("3Baz"));
    EXPECT_EQ(table.at(7), QString("Quu1x"));
    EXPECT_EQ(table.at(8), QString("Quu2x"));
    EXPECT_EQ(table.at(9), QString());
}

TEST(SymbolTableTest, RangeSorting)
{
    SymbolTable table;
    table.addRange(7, 2, "Quu?x");
    table.addRange(4, 3, "?Baz");
    table.finalizeTable();
    EXPECT_EQ(table.at(4), QString("1Baz"));
    EXPECT_EQ(table.at(8), QString("Quu2x"));
    table.addRange(1, 2, "Foo?");
    table.finalizeTable();
    EXPECT_EQ(table.at(2), QString("Foo2"));
}

TEST(SymbolTableTest, BadRange)
{
    SymbolTable table;
    table.addRange(1, 0, "Foo");
    table.finalizeTable();
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.at(0), QString());
    EXPECT_EQ(table.at(1), QString());
}

TEST(SymbolTableTest, EmptyLabel)
{
    SymbolTable table;
    table.addRange(1, 2, "");
    table.addRange(3, 1, "");
    EXPECT_EQ(table.at(1), QString(""));
    EXPECT_EQ(table.at(3), QString(""));
    EXPECT_FALSE(table.at(1).isNull());
    EXPECT_FALSE(table.at(3).isNull());
    EXPECT_TRUE(table.at(4).isNull());
}
