#include <gtest/gtest.h>
#include <QCoreApplication>

#include "corelibmodbuscommunicationtest.h"
#include "corelibfiletransferprotocoltest.h"
#include "corelibjogcontroltest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
