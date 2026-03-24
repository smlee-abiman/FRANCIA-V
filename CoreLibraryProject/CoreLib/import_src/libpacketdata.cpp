#include "libpacketdata.h"

LibPacketData::LibPacketData(QObject *parent) : QObject(parent)
{

}

LibPacketData::LibPacketData(QString label, QString value, QObject *parent) : QObject(parent) {
    m_label = label;
    m_value = value;
    m_unit = "";
}

LibPacketData::LibPacketData(QString label, QString value, QString unit, QObject *parent) : QObject(parent) {
    m_label = label;
    m_value = value;
    m_unit = unit;
}
