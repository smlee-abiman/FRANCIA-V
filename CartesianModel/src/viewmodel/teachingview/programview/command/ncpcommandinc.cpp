#include <QRegularExpression>
#include "ncpcommandinc.h"

//------------------------------------------------------
// NCPCommandInc.
//------------------------------------------------------
NCPCommandInc::NCPCommandInc(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandInc::NCPCommandInc(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandInc::parse(const QString &commandLine)
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
QString NCPCommandInc::nameString() const
{
    return m_resource->label(41, 627, m_resource->language());
}
//------------------------------------------------------
QString NCPCommandInc::toString() const
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
QString NCPCommandInc::toCode() const
{
    return QString("INC\t%1").arg(m_symbolStr);
}
//------------------------------------------------------
NCPCommand *NCPCommandInc::clone() const
{
    auto selfClone = new NCPCommandInc(m_resource);
    selfClone->setSymbolStr(m_symbolStr);
    return selfClone;
}
