#include "ncpcommandfactory.h"
#include "command/ncpcommandunknown.h"
#include "command/ncpcommandprog.h"
#include "command/ncpcommandgosub.h"
#include "command/ncpcommandon.h"
#include "command/ncpcommandoff.h"
#include "command/ncpcommandout.h"
#include "command/ncpcommandoutp.h"
#include "command/ncpcommandin.h"
#include "command/ncpcommandret.h"
#include "command/ncpcommandcinc.h"
#include "command/ncpcommandmove.h"
#include "command/ncpcommandmovep.h"
#include "command/ncpcommandwait.h"
#include "command/ncpcommandwaitpos.h"
#include "command/ncpcommandbreak.h"
#include "command/ncpcommandacc.h"
#include "command/ncpcommandhome.h"
#include "command/ncpcommandhomep.h"
#include "command/ncpcommandline.h"
#include "command/ncpcommandlinec.h"
#include "command/ncpcommandpath.h"
#include "command/ncpcommandarc.h"
#include "command/ncpcommandarcc.h"
#include "command/ncpcommandcclr.h"
#include "command/ncpcommandinc.h"
#include "command/ncpcommanddec.h"
#include "command/ncpcommandifgosub.h"
#include "command/ncpcommandgoto.h"
#include "command/ncpcommandifgoto.h"
#include "command/ncpcommandpause.h"
#include "command/ncpcommandend.h"
#include "command/ncpcommandalm.h"

NCPCommandFactory::NCPCommandFactory(ResourceManager *resource) :
    m_resource(resource)
{

}

NCPCommand *NCPCommandFactory::createCommand(const QString& commandString, const QString &commandLine)
{
    NCPCommand *command;
    // qDebug() << "parse " << commandString << "," << commandLine;
    if (commandString == "PROG")          { command = new NCPCommandProg(m_resource); }
    else if (commandString == "CINC")     { command = new NCPCommandCInc(m_resource); }
    else if (commandString == "CCLR")     { command = new NCPCommandCClr(m_resource); }
    else if (commandString == "INC")      { command = new NCPCommandInc(m_resource); }
    else if (commandString == "DEC")      { command = new NCPCommandDec(m_resource); }
    else if (commandString == "ON")       { command = new NCPCommandOn(m_resource); }
    else if (commandString == "OFF")      { command = new NCPCommandOff(m_resource); }
    else if (commandString == "IN")       { command = new NCPCommandIn(m_resource); }
    else if (commandString == "WAIT") {
        if (commandLine.contains("POS", Qt::CaseInsensitive)) { command = new NCPCommandWaitPOS(m_resource); }
        else { command = new NCPCommandWait(m_resource); }
    }
    else if (commandString == "PAUSE")    { command = new NCPCommandPause(m_resource); }
    else if (commandString == "GOSUB")    { command = new NCPCommandGoSub(m_resource); }
    else if (commandString == "GOTO")     { command = new NCPCommandGoTo(m_resource); }
    else if (commandString == "IF") {
        if (commandLine.contains("GOSUB", Qt::CaseInsensitive))     { command = new NCPCommandIfGoSub(m_resource); }
        else if (commandLine.contains("GOTO", Qt::CaseInsensitive)) { command = new NCPCommandIfGoTo(m_resource); }
        else { command = new NCPCommandUnknown(); }
    }
    else if (commandString == "RET")      { command = new NCPCommandRet(m_resource); }
    else if (commandString == "END")      { command = new NCPCommandEnd(m_resource); }
    // else if (commandString == "NEXT")     { command = new NCPCommandUnknown(); }   // not supported.
    // else if (commandString == "SET")      { command = new NCPCommandUnknown(); }   // not supported.
    else if (commandString == "MOVE")     { command = new NCPCommandMove(m_resource); }
    else if (commandString == "MOVEP")    { command = new NCPCommandMovep(m_resource); }
    else if (commandString == "HOME")     { command = new NCPCommandHome(m_resource); }
    else if (commandString == "HOMEP")    { command = new NCPCommandHomep(m_resource); }
    else if (commandString == "PATH")     { command = new NCPCommandPath(m_resource); }
    else if (commandString == "BREAK")    { command = new NCPCommandBreak(m_resource); }
    else if (commandString == "LINE")     { command = new NCPCommandLine(m_resource); }
    else if (commandString == "LINEC")    { command = new NCPCommandLinec(m_resource); }
    else if (commandString == "ARC")      { command = new NCPCommandArc(m_resource); }
    else if (commandString == "ARCC")     { command = new NCPCommandArcc(m_resource); }
    // else if (commandString == "SHIFT")    { command = new NCPCommandUnknown(); }   // not supported.
    // else if (commandString == "ASPD")     { command = new NCPCommandUnknown(); }   // not supported.
    else if (commandString == "OUT")      { command = new NCPCommandOut(m_resource); }
    else if (commandString == "OUTP")     { command = new NCPCommandOutp(m_resource);}
    // else if (commandString == "JOG")      { command = new NCPCommandUnknown(); }   // not supported.
    // else if (commandString == "SRV")      { command = new NCPCommandUnknown(); }   // not supported.
    else if (commandString == "ACC")      { command = new NCPCommandAcc(m_resource); }
    else if (commandString == "ALM")      { command = new NCPCommandAlm(m_resource); }
    else                                  { command = new NCPCommandUnknown(); }
    return command;
}
