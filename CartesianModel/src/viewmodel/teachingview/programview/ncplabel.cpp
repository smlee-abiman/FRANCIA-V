#include "ncplabel.h"

NCPLabel::NCPLabel(int labelNo, QObject *parent)
    : QObject(parent)
    , mNo(labelNo)
    , mProgramNo(0)
{
}
