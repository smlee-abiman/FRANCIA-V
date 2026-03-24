#include "programmenu4_1item.h"

ProgramMenu4_1Item::ProgramMenu4_1Item(QObject *parent)
    : QObject(parent)
{
    init();
}

void ProgramMenu4_1Item::init()
{
    m_optional = false;
    m_selectState = false;
    m_relation = "and";
    m_symbol = "";
    m_symbol1 = "";
    m_opeStr = "";
    m_value = 0;
    m_ioAddress = -1;
}

void ProgramMenu4_1Item::setOptional(bool option) {
    if (m_optional == option) { return; }
    m_optional = option;
    emit optionalChanged();
}

void ProgramMenu4_1Item::setSelectState(bool state) {
    if (m_selectState == state) { return; }
    m_selectState = state;
    emit selectStateChanged();
}

void ProgramMenu4_1Item::setRelation(const QString &constRelation) {
    auto relation = (constRelation == ",") ? "and" : constRelation;
    if (m_relation == relation) { return; }
    m_relation = relation;
    emit relationChanged();
}

void ProgramMenu4_1Item::setSymbol(const QString &symbol) {
    if (m_symbol == symbol) { return; }
    m_symbol = symbol;
    emit symbolChanged();
}

void ProgramMenu4_1Item::setSymbol1(const QString &symbol1)
{
    if (m_symbol1 == symbol1) { return; }
    m_symbol1 = symbol1;
    emit symbol1Changed();
}

void ProgramMenu4_1Item::setOpeStr(const QString &constOpeStr) {
    auto opeStr = (constOpeStr == "#") ? "≠" : constOpeStr;
    if (m_opeStr == opeStr) { return; }
    m_opeStr = opeStr;
    emit opeStrChanged();
}

void ProgramMenu4_1Item::setValue(int value) {
    if (m_value == value) { return; }
    m_value = value;
    emit valueChanged();
}

void ProgramMenu4_1Item::setIoAddress(int ioAddress) {
    if (m_ioAddress == ioAddress) { return; }
    m_ioAddress = ioAddress;
    emit ioAddressChanged();
}
