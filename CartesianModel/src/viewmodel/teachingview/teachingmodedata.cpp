#include "teachingmodedata.h"

TeachingModeData::TeachingModeData(QObject *parent)
    : QObject(parent)
{
    m_label = new Label(this);
    m_label1 = new Label(this);
    m_description = new Label(this);
}

void TeachingModeData::setLabelName(const QString &name)
{
    m_label->setName(name);
}

void TeachingModeData::setLabelData(const QString &name, const QString &color)
{
    m_label->setName(name);
    m_label->setColor(color);
}

void TeachingModeData::setLabel1Data(const QString &name, const QString &color)
{
    m_label1->setName(name);
    m_label1->setColor(color);
}

void TeachingModeData::setDescriptionName(const QString &name)
{
    m_description->setName(name);
}

void TeachingModeData::setDescriptionData(const QString &name, const QString &color)
{
    m_description->setName(name);
    m_description->setColor(color);
}

void TeachingModeData::setLabel(Label *label)
{
    if (m_label != label) {
        m_label = label;
        emit labelChanged(m_label);
    }
}

void TeachingModeData::setLabel1(Label *label1)
{
    if (m_label1 != label1) {
        m_label1 = label1;
        emit label1Changed(m_label1);
    }
}

void TeachingModeData::setDescription(Label *label)
{
    if (m_description != label) {
        m_description = label;
        emit descriptionChanged(m_description);
    }
}

void TeachingModeData::setStatus(int status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged(m_status);
    }
}

void TeachingModeData::setKeyNo(int no)
{
    m_keyNo = no;
}

void TeachingModeData::setDi(int di)
{
    m_di = di;
}

void TeachingModeData::setKey_on(int key_on)
{
    m_key_on = key_on;
}

void TeachingModeData::setKey_off(int key_off)
{
    m_key_off = key_off;
}
