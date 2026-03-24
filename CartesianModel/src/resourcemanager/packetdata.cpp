#include "packetdata.h"

PacketData::PacketData(QObject *parent) : QObject(parent) {}

PacketData::PacketData(QString label, QString value, QObject *parent) : QObject(parent)
{
    m_label = label;
    m_value = value;
    m_unit = "";
    m_color = "";
}

PacketData::PacketData(QString label, QString value, QString unit, QObject *parent)
    : QObject(parent)
{
    m_label = label;
    m_value = value;
    m_unit = unit;
    m_color = "";
}
