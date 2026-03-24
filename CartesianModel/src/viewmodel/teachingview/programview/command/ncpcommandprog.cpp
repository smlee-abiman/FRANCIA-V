#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

#include "ncpcommandprog.h"

//------------------------------------------------------
// NCPCommandProg.
//------------------------------------------------------
NCPCommandProg::NCPCommandProg(QObject *parent)
    : NCPCommand(parent)
    , mNo(-1)
{
}
//------------------------------------------------------
NCPCommandProg::NCPCommandProg(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
    , mNo(-1)
{
}
//------------------------------------------------------
NCPCommandProg::NCPCommandProg(ResourceManager *resource, const QString& commandLine, QObject *parent)
    : NCPCommand(resource, parent)
    , mNo(-1)
{
    NCPCommandProg::parse(commandLine);
}
//------------------------------------------------------
NCPCommandProg::~NCPCommandProg()
{
}
//------------------------------------------------------
bool NCPCommandProg::parse(const QString& commandLine)
{
// qDebug() << "parse " << commandLine;

    QRegularExpression commandRe("(\\s+|\\t+)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }

    // parse program number.
    QRegularExpression programNumberRe("#(\\d{3})");
    auto match = programNumberRe.match(splitList.at(1));
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
    mNo = value;
    return true;
}
//------------------------------------------------------
QString NCPCommandProg::nameString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandProg::toString() const
{
    return "";  // todo:
}
//------------------------------------------------------
QString NCPCommandProg::toCode() const
{
    return QString("PROG\t#%1").arg(mNo, 3, 10, QChar('0'));
}
//------------------------------------------------------
NCPCommand* NCPCommandProg::clone() const
{
    auto selfClone = new NCPCommandProg(m_resource);
    selfClone->mNo = mNo;
    return selfClone;
}
