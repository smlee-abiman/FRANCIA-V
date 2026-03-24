#include <QAbstractItemModelTester>
#include <QString>
#include <gtest/gtest.h>
#include <memory>
#include "bitarraylistmodel.h"
#include "testhelpers.h"

class BitArrayListModelTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_tester = std::make_unique<QAbstractItemModelTester>(
                &m_model, QAbstractItemModelTester::FailureReportingMode::Warning);
    }

    BitArrayListModel m_model;
    std::unique_ptr<QAbstractItemModelTester> m_tester;

    constexpr static auto LabelRole = BitArrayListModel::LabelRole;
    constexpr static auto ValueRole = BitArrayListModel::ValueRole;
};

TEST_F(BitArrayListModelTest, Empty)
{
    EXPECT_EQ(m_model.rowCount(), 0);
    EXPECT_FALSE(m_model.index(0).data().isValid());
    m_model.setValues("\x55");
    EXPECT_EQ(m_model.rowCount(), 0);
}

TEST_F(BitArrayListModelTest, AddRemoveLabels)
{
    m_model.addLabels({ "Foo", "Bar", "Baz" });
    EXPECT_EQ(m_model.rowCount(), 3);
    EXPECT_EQ(m_model.index(0).data(LabelRole), QString("Foo"));
    EXPECT_EQ(m_model.index(1).data(LabelRole), QString("Bar"));
    EXPECT_EQ(m_model.index(2).data(LabelRole), QString("Baz"));
    m_model.addLabels({});
    EXPECT_EQ(m_model.rowCount(), 3);
    m_model.clearLabels();
    EXPECT_EQ(m_model.rowCount(), 0);
    m_model.addLabels({ "Quux" });
    EXPECT_EQ(m_model.rowCount(), 1);
    EXPECT_EQ(m_model.index(0).data(LabelRole), QString("Quux"));
}

TEST_F(BitArrayListModelTest, ShortValues)
{
    m_model.addLabels({ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" });
    EXPECT_TRUE(m_model.index(0).data(ValueRole).isNull());
    EXPECT_TRUE(m_model.index(7).data(ValueRole).isNull());
    EXPECT_TRUE(m_model.index(8).data(ValueRole).isNull());
    EXPECT_TRUE(m_model.index(9).data(ValueRole).isNull());
    m_model.setValues("\xaa");
    EXPECT_EQ(m_model.index(0).data(ValueRole), false);
    EXPECT_EQ(m_model.index(1).data(ValueRole), true);
    EXPECT_EQ(m_model.index(7).data(ValueRole), true);
    EXPECT_TRUE(m_model.index(8).data(ValueRole).isNull());
}

TEST_F(BitArrayListModelTest, ValueChanges)
{
    m_model.addLabels({ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" });
    m_model.setValues("\xaa\x55");
    EXPECT_EQ(m_model.index(0).data(ValueRole), false);
    EXPECT_EQ(m_model.index(7).data(ValueRole), true);
    EXPECT_EQ(m_model.index(8).data(ValueRole), true);
    EXPECT_EQ(m_model.index(9).data(ValueRole), false);
    m_model.setValues("\x55\xaa");
    EXPECT_EQ(m_model.index(0).data(ValueRole), true);
    EXPECT_EQ(m_model.index(7).data(ValueRole), false);
    EXPECT_EQ(m_model.index(8).data(ValueRole), false);
    EXPECT_EQ(m_model.index(9).data(ValueRole), true);
    m_model.setValues("");
    EXPECT_TRUE(m_model.index(0).data(ValueRole).isNull());
}

TEST_F(BitArrayListModelTest, MirrorEmpty)
{
    m_model.setMirrorPages(2);
    EXPECT_EQ(m_model.rowCount(), 0);
    m_model.setMirrorPages(1);
    EXPECT_EQ(m_model.rowCount(), 0);
}

TEST_F(BitArrayListModelTest, MirrorChanges)
{
    m_model.addLabels({ "Foo", "Bar", "Baz" });
    EXPECT_EQ(m_model.rowCount(), 3);
    m_model.setMirrorPages(2);
    EXPECT_EQ(m_model.rowCount(), 3 * 2);
    m_model.setMirrorPages(1);
    EXPECT_EQ(m_model.rowCount(), 3);
}

TEST_F(BitArrayListModelTest, MirrorAddRemoveLabels)
{
    m_model.setMirrorPages(4);
    m_model.addLabels({ "Foo", "Bar", "Baz" });
    EXPECT_EQ(m_model.rowCount(), 3 * 4);
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(m_model.index(i * 3 + 0).data(LabelRole), QString("Foo"));
        EXPECT_EQ(m_model.index(i * 3 + 1).data(LabelRole), QString("Bar"));
        EXPECT_EQ(m_model.index(i * 3 + 2).data(LabelRole), QString("Baz"));
    }
    EXPECT_FALSE(m_model.index(3 * 4).data().isValid());
    m_model.addLabels({});
    EXPECT_EQ(m_model.rowCount(), 3 * 4);
    m_model.clearLabels();
    EXPECT_EQ(m_model.rowCount(), 0);
    m_model.addLabels({ "Quux" });
    EXPECT_EQ(m_model.rowCount(), 1 * 4);
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(m_model.index(i).data(LabelRole), QString("Quux"));
    }
}

TEST_F(BitArrayListModelTest, MirrorValueChanges)
{
    m_model.addLabels({ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" });
    m_model.setMirrorPages(2);
    m_model.setValues("\xaa\x55");
    for (int i = 0; i < 2; ++i) {
        EXPECT_EQ(m_model.index(i * 10 + 0).data(ValueRole), false);
        EXPECT_EQ(m_model.index(i * 10 + 7).data(ValueRole), true);
        EXPECT_EQ(m_model.index(i * 10 + 8).data(ValueRole), true);
        EXPECT_EQ(m_model.index(i * 10 + 9).data(ValueRole), false);
    }
    m_model.setValues("\x55\xaa");
    for (int i = 0; i < 2; ++i) {
        EXPECT_EQ(m_model.index(i * 10 + 0).data(ValueRole), true);
        EXPECT_EQ(m_model.index(i * 10 + 7).data(ValueRole), false);
        EXPECT_EQ(m_model.index(i * 10 + 8).data(ValueRole), false);
        EXPECT_EQ(m_model.index(i * 10 + 9).data(ValueRole), true);
    }
    m_model.setValues("");
    EXPECT_TRUE(m_model.index(0).data(ValueRole).isNull());
}

TEST_F(BitArrayListModelTest, BlockLayout)
{
    m_model.setValueBlockLayout(13, 2); // 13 bits per 2 byte
    m_model.addLabels(
            { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "1", "2", "3", "4",
              "5", "6", "7", "8", "9", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" });
    m_model.setValues("\xaa\xaa");
    EXPECT_EQ(m_model.index(0).data(ValueRole), false);
    EXPECT_EQ(m_model.index(1).data(ValueRole), true);
    EXPECT_EQ(m_model.index(12).data(ValueRole), false);
    EXPECT_TRUE(m_model.index(13).data(ValueRole).isNull());
    m_model.setValues("\xaa\xaa\x01");
    EXPECT_EQ(m_model.index(13).data(ValueRole), true);
    EXPECT_EQ(m_model.index(14).data(ValueRole), false);
    EXPECT_EQ(m_model.index(20).data(ValueRole), false);
    EXPECT_TRUE(m_model.index(21).data(ValueRole).isNull());
    m_model.setValues("\xaa\xaa\x01\x55");
    EXPECT_EQ(m_model.index(21).data(ValueRole), true);
    EXPECT_EQ(m_model.index(22).data(ValueRole), false);
    EXPECT_EQ(m_model.index(23).data(ValueRole), true);
    EXPECT_EQ(m_model.index(24).data(ValueRole), false);
    EXPECT_EQ(m_model.index(25).data(ValueRole), true);
    EXPECT_TRUE(m_model.index(26).data(ValueRole).isNull());
    m_model.setValueBlockLayout(8, 3); // 8 bits per 3 byte
    EXPECT_EQ(m_model.index(0).data(ValueRole), false);
    EXPECT_EQ(m_model.index(7).data(ValueRole), true);
    EXPECT_EQ(m_model.index(8).data(ValueRole), true);
    EXPECT_EQ(m_model.index(15).data(ValueRole), false);
    EXPECT_TRUE(m_model.index(16).data(ValueRole).isNull());
}
