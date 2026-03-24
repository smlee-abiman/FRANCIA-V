#include "corelibdisplaycontrol.h"

CoreLibDisPlayControl::CoreLibDisPlayControl()
{

}

CoreLibDisPlayControl::~CoreLibDisPlayControl()
{

}

bool CoreLibDisPlayControl::setLcdBackLightBrightness(QString value)
{
#ifdef REAL_TARGET
    QFile *file = mDevFile[DriverControlType::DriverControlTypeDisplayBrightness];
    if (file == nullptr){
        qCritical() << "CoreLibDisPlayControl::setLcdBackLightBrightness file is null ";
        return false;
    }
    QTextStream stream(file);
    stream.seek(0);
    stream << value;
    stream.flush();
#else // REAL_TARGET
    (void)value;
#endif
    return true;
}

bool CoreLibDisPlayControl::getLcdBackLightBrightness(QString *value)
{
    if (value == nullptr){
        qCritical() << "CoreLibDisPlayControl::getLcdBackLightBrightness value is null ";
        return false;
    }
    QFile *file = mDevFile[DriverControlType::DriverControlTypeDisplayBrightness];
    if (file == nullptr){
        qCritical() << "CoreLibDisPlayControl::getLcdBackLightBrightness file is null ";
        return false;
    }
    QTextStream stream(file);
    *value = stream.readLine(0);
    return true;
}
