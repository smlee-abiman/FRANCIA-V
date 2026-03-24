#include "programmenu2_1item.h"

ProgramMenu2_1Item::ProgramMenu2_1Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu2_1Item::init()
{
    setSelect(false);
    setPositon(0);
    setSpeed(0);
}

void ProgramMenu2_1Item::setSelect(bool select)
{
    if (m_select != select) {
        m_select = select;
        selectChanged();
    }
}

void ProgramMenu2_1Item::setPositon(int position)
{
    if (m_position != position) {
        m_position = position;
        emit positionChanged();
    }
}

void ProgramMenu2_1Item::setSpeed(int speed)
{
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged();
    }
}
