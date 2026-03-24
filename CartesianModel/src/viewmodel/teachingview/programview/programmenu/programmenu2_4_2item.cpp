#include "programmenu2_4_2item.h"

ProgramMenu2_4_2Item::ProgramMenu2_4_2Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu2_4_2Item::init()
{
    setSelect(false);
    setHalfwayPosition(0);
    setEndPosition(0);
    setSpeed(0);
}

void ProgramMenu2_4_2Item::setSelect(bool select)
{
    if (m_select != select) {
        m_select = select;
        selectChanged();
    }
}


void ProgramMenu2_4_2Item::setHalfwayPosition(int position)
{
    if (m_halfwayPosition != position) {
        m_halfwayPosition = position;
        emit halfwayPositionChanged();
    }
}

void ProgramMenu2_4_2Item::setEndPosition(int position)
{
    if (m_endPosition != position) {
        m_endPosition = position;
        emit endPositionChanged();
    }
}

void ProgramMenu2_4_2Item::setSpeed(int speed)
{
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged();
    }
}
