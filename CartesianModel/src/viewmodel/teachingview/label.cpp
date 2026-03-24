#include "label.h"

Label::Label(QObject *parent)
    : QObject(parent)
{

}

void Label::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged(m_name);
    }
}

void Label::setColor(const QString &color)
{
    if (m_color != color) {
        m_color = color;
        emit colorChanged(m_color);
    }
}
