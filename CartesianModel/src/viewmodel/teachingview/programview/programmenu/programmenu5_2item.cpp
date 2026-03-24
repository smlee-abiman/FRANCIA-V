#include "programmenu5_2item.h"
#include <QDebug>

ProgramMenu5_2Item::ProgramMenu5_2Item(QObject *parent) : QObject(parent)
{

}

void ProgramMenu5_2Item::init()
{
    m_optional = false;
    m_selectState = false;
    m_mark = "";
    m_value = 0;
    m_msgLabelIndex = -1;
}

void ProgramMenu5_2Item::setOptional(bool option)
{
    if (m_optional != option) {
        m_optional = option;
        emit optionalChanged();
    }
}

void ProgramMenu5_2Item::setSelectState(bool state)
{
    if (m_selectState != state) {
        m_selectState = state;
        emit selectStateChanged();
    }
}

void ProgramMenu5_2Item::setOpeStr(const QString &constOpeStr) {
    auto opeStr = (constOpeStr == "#") ? "≠" : constOpeStr;
    if (m_opeStr == opeStr) { return; }
    m_opeStr = opeStr;
    emit opeStrChanged();
}

void ProgramMenu5_2Item::setMark(const QString &mark)
{
    if (m_mark != mark) {
        m_mark = mark;
        emit markChanged();
    }
}

void ProgramMenu5_2Item::setValue(int value)
{
    if (m_value != value) {
        m_value = value;
        emit valueChanged();
    }
}

void ProgramMenu5_2Item::setVarIndex(int varIndex)
{
    if (m_varIndex != varIndex) {
        m_varIndex = varIndex;
        emit varIndexChanged();
    }
}

void ProgramMenu5_2Item::setMsgLabelIndex(int index)
{
    if (m_msgLabelIndex != index) {
        m_msgLabelIndex = index;
        emit msgLabelIndexChanged();
    }
}
