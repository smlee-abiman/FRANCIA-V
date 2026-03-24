#ifndef CORELIBTOUCHCONTROL_H
#define CORELIBTOUCHCONTROL_H

#include "corelibdrivermanager.h"

class CoreLibTouchControl : public CoreLibDriverManager
{
public:
    CoreLibTouchControl();
    ~CoreLibTouchControl();
    void executeCaribration(QString fromAppName);
};

#endif // CORELIBTOUCHCONTROL_H
