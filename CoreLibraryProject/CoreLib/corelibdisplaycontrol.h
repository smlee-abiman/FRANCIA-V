#ifndef CORELIBDISPLAYCONTROL_H
#define CORELIBDISPLAYCONTROL_H

#include "corelibdrivermanager.h"

#include <QFileInfo>
#include <QCoreApplication>
#include <QTextStream>
#include <QDir>

class CoreLibDisPlayControl : public CoreLibDriverManager
{
public:
    CoreLibDisPlayControl();
    ~CoreLibDisPlayControl();
    bool setLcdBackLightBrightness(QString value);
    bool getLcdBackLightBrightness(QString *value);
};

#endif // CORELIBDISPLAYCONTROL_H
