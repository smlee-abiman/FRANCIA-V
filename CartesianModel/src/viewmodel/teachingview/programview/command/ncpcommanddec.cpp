#include <QRegularExpression>
#include "ncpcommanddec.h"

//------------------------------------------------------
// NCPCommandDec.
//------------------------------------------------------
NCPCommandDec::NCPCommandDec(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandDec::NCPCommandDec(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandDec::parse(const QString &commandLine)
{
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug() << "Syntax error commandLine:[" << commandLine << "]";
        m_symbolStr = "";
        return false;
    }
    m_symbolStr = splitList.at(1);
    return true;
}
//------------------------------------------------------
QString NCPCommandDec::nameString() const
{
    return m_resource->label(41, 628, m_resource->language());
}
//------------------------------------------------------
QString NCPCommandDec::toString() const
{
    auto string = QString("");
    QString symbolName("");
    if (!m_symbolStr.isEmpty()) {
        auto ioAddress = m_resource->indexAt(m_symbolStr);
        if (0 <= ioAddress) {
            auto name = m_resource->getMessageStringValue("1", ioAddress, m_resource->language());
            if (!name.isEmpty()) {
                symbolName = QString(" (%1)").arg(name);
            }
        }
    }
    string.append(m_symbolStr);
    string.append(symbolName);
    return string.trimmed();
}
//------------------------------------------------------
QString NCPCommandDec::toCode() const
{
    return QString("DEC\t%1").arg(m_symbolStr);
}
//------------------------------------------------------
NCPCommand *NCPCommandDec::clone() const
{
    auto selfClone = new NCPCommandDec(m_resource);
    selfClone->setSymbolStr(m_symbolStr);
    return selfClone;
}
