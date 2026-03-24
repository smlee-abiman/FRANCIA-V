#include "programmenu2_4_6item.h"

ProgramMenu2_4_6Item::ProgramMenu2_4_6Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu2_4_6Item::init()
{
    setAcceleration(0);
    setSelect(false);
}

void ProgramMenu2_4_6Item::setAcceleration(int value)
{
    if (m_acceleration != value) {
        m_acceleration = value;
        emit accelerationChanged();
    }
}

void ProgramMenu2_4_6Item::setSelect(bool select)
{
    if (m_select != select) {
        m_select = select;
        emit selectChanged();
    }
}
