#include "programmenu2_4_3item.h"

ProgramMenu2_4_3Item::ProgramMenu2_4_3Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu2_4_3Item::init()
{
    setSelect(false);
    setPosition(0);
}

void ProgramMenu2_4_3Item::setPosition(int position)
{
    if (m_position != position) {
        m_position = position;
        emit positionChanged();
    }
}

void ProgramMenu2_4_3Item::setSelect(bool select)
{
    if (m_select != select) {
        m_select = select;
        emit selectChanged();
    }
}
