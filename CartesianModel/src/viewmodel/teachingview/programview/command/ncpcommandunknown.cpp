#include <QDebug>

#include "ncpcommandunknown.h"

//------------------------------------------------------
// NCPCommandUnknown.
//------------------------------------------------------
NCPCommandUnknown::NCPCommandUnknown(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandUnknown::NCPCommandUnknown(const QString& commandLine, QObject *parent)
    : NCPCommand(parent)
{
    NCPCommandUnknown::parse(commandLine);
}
//------------------------------------------------------
NCPCommandUnknown::~NCPCommandUnknown()
{
}
//------------------------------------------------------
bool NCPCommandUnknown::parse(const QString& commandLine)
{
// qDebug() << "parse " << commandLine;
    mCommandLine = commandLine;
    return true;
}
//------------------------------------------------------
NCPCommand *NCPCommandUnknown::clone() const
{
    return new NCPCommandUnknown(mCommandLine);
}
