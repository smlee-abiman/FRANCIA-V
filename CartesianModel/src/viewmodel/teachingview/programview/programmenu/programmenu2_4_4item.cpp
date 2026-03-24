#include "programmenu2_4_4item.h"

ProgramMenu2_4_4Item::ProgramMenu2_4_4Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu2_4_4Item::init()
{
    setSpeed(0);
    setSelect(false);
}

void ProgramMenu2_4_4Item::setSpeed(int speed)
{
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged();
    }
}

void ProgramMenu2_4_4Item::setSelect(bool select)
{
    if (m_select != select) {
        m_select = select;
        emit selectChanged();
    }
}
