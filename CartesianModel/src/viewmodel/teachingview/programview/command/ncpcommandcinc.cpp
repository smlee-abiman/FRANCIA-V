#include <QRegularExpression>
#include "ncpcommandcinc.h"


//------------------------------------------------------
// NCPCommandCInc.
//------------------------------------------------------
NCPCommandCInc::NCPCommandCInc(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandCInc::NCPCommandCInc(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
bool NCPCommandCInc::parse(const QString& commandLine)
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
QString NCPCommandCInc::nameString() const
{
    return m_resource->label(41, 625, m_resource->language());
}
//------------------------------------------------------
QString NCPCommandCInc::toString() const
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
QString NCPCommandCInc::toCode() const
{
    return QString("CINC\t%1").arg(m_symbolStr);
}
//------------------------------------------------------
NCPCommand *NCPCommandCInc::clone() const
{
    auto selfClone = new NCPCommandCInc(m_resource);
    selfClone->setSymbolStr(m_symbolStr);
    return selfClone;
}
