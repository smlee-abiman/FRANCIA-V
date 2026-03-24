#ifndef NCPCOMMANDSTRING_H
#define NCPCOMMANDSTRING_H

#include <QObject>
#include "ncpcommand.h"
#include "modbusmanager.h"

class TeachingProgramPageModel;
class NCPStep;
class NCPCommandLine;
class AxisPointInfo;

class NCPCommandString : public QObject
{
    Q_OBJECT
public:
    explicit NCPCommandString(TeachingProgramPageModel *programPageModel, ResourceManager *resource, QObject *parent = nullptr);
    QString toTranslatedString(NCPStep *step, int language);

    QString toStringGoSub(NCPStep *step, int language);
    QString toStringMove(NCPStep *step, int language);
    QString toStringLine(NCPStep *step, int language);
    QString toStringArc(NCPStep *step, int language);
    QString toStringWait(NCPStep *step, int language);
    QString toStringPath(NCPStep *step, int language);

private:
    QString toStringJig(NCPStep *step);
    QString toStringStandardStack(int stackIndex);
    QString toStringFreeStack(int stackIndex);
    QString getStackPointInfoString(int type, int pointNo);
    QString getPointNameForMove(int pointNo);
    QString getLineInfoString(NCPCommandLine *command, int pointNo);
    QString getAxisName(int axisIndex);

private:
    TeachingProgramPageModel    *m_programPageModel = nullptr;
    ModbusManager               *m_modbus = ModbusManager::get_instance();
    ResourceManager             *m_resource;
    AxisPointInfo               *m_axisPointInfo = nullptr;
};

#endif // NCPCOMMANDSTRING_H
