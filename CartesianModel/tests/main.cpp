#include <QCoreApplication>
#include <QLoggingCategory>
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    QCoreApplication app(argc, argv);
    QLoggingCategory::setFilterRules(QStringLiteral("*.debug=false"));
    return RUN_ALL_TESTS();
}
