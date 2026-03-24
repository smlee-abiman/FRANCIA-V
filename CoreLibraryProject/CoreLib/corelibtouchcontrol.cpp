#include "corelibapplicationmanager.h"
#include "corelibtouchcontrol.h"

CoreLibTouchControl::CoreLibTouchControl()
{

}

CoreLibTouchControl::~CoreLibTouchControl()
{

}

void CoreLibTouchControl::executeCaribration(QString fromAppName)
{
#ifdef Q_OS_LINUX
    CoreLibApplicationManager appMgr;
    appMgr.changeApplication("TSLIB_CALIBFILE=/etc/pointercal ts_calibrate", "", "/opt/SystemSettingApp/bin/SystemSettingApp", "--wakeUpPage test --fromAppName " + fromAppName);
#else
    (void)fromAppName;
#endif
}
