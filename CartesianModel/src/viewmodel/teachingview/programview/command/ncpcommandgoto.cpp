#include <QRegularExpression>
#include "ncpcommandgoto.h"

//------------------------------------------------------
// NCPCommandGoTo.
//------------------------------------------------------
NCPCommandGoTo::NCPCommandGoTo(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandGoTo::NCPCommandGoTo(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
NCPCommandGoTo::NCPCommandGoTo(ResourceManager *resource, int labelNo, QObject *parent)
    : NCPCommand(resource, parent)
{
    setLabelNo(labelNo);
}
//------------------------------------------------------
void NCPCommandGoTo::setLabelNo(int labelNo)
{
    m_labelNo = labelNo;
}
//------------------------------------------------------
bool NCPCommandGoTo::parse(const QString &commandLine)
{
    // qDebug() << "parse " << commandLine;
    QRegularExpression commandRe("(\\s+|\\t+)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }

    // parse label number.
    QRegularExpression labelNumberRe("L(\\d{3})");
    auto match = labelNumberRe.match(splitList.at(1));
    if (!match.hasMatch()) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }
    auto ok = false;
    auto value = match.captured(1).toInt(&ok);
    if (!ok) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }
    setLabelNo(value);
    return true;
}
//------------------------------------------------------
QString NCPCommandGoTo::nameString() const
{
    return m_resource->label(41, 537);
}
//------------------------------------------------------
QString NCPCommandGoTo::toString() const
{
    return QString("L%1").arg(m_labelNo, 3, 10, QChar('0'));
}
//------------------------------------------------------
QString NCPCommandGoTo::toCode() const
{
    return QString("GOTO\tL%1").arg(m_labelNo, 3, 10, QChar('0'));
}
//------------------------------------------------------
NCPCommand *NCPCommandGoTo::clone() const
{
    return new NCPCommandGoTo(m_resource, m_labelNo);
}
