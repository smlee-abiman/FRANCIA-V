#include "programmenu1_4item.h"
#include <QDebug>

ProgramMenu1_4Item::ProgramMenu1_4Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu1_4Item::init()
{
    m_optional = false;
    m_selectState = false;
    m_mark = "";
    m_msgLabelIndex = -1;
    m_value = 0;
}

void ProgramMenu1_4Item::setOptional(bool option)
{
    if (m_optional != option) {
        m_optional = option;
        emit optionalChanged();
    }
}

void ProgramMenu1_4Item::setSelectState(bool state)
{
    if (m_selectState != state) {
        m_selectState = state;
        emit selectStateChanged();
    }
}

void ProgramMenu1_4Item::setMark(const QString &mark)
{
    if (m_mark != mark) {
        m_mark = mark;
        emit markChanged();
    }
}

void ProgramMenu1_4Item::setMsgLabelIndex(int index)
{
    if (m_msgLabelIndex != index) {
        m_msgLabelIndex = index;
        emit msgLabelIndexChanged();
    }
}

void ProgramMenu1_4Item::setValue(int value)
{
    if (m_value != value) {
        m_value = value;
        emit valueChanged();
    }
}
