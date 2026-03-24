#include <QDebug>
#include "tst_teachingtimerpagemodel.h"
#include "tst_teachingcounterpagemodel.h"
#include "tst_ioforceviewmodel.h"

int main(int argc, char** argv)
{
    int status = 0;
    {
        TeachingTimerPageModelTest tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        TeachingCounterPageModelTest tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        IOForceViewModelTest tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
//    {
//      ControllerViewModelTest tc;
//      status |= QTest::qExec(&tc, argc, argv);
//    }
//    {
//      AmpMonitorViewModelTest tc;
//      status |= QTest::qExec(&tc, argc, argv);
//    }
    return status;
}
