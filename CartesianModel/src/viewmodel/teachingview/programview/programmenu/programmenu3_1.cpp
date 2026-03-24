#include "programmenu3_1.h"
#include "../command/ncpcommandwait.h"
#include <QRegularExpression>

ProgramMenu3_1::ProgramMenu3_1(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
}

NCPCommand *ProgramMenu3_1::createNcpCommand()
{
    if (m_curDelayTimeValue == ""){
        //qDebug() << "!!! m_curDelayTimeValue:" << m_curDelayTimeValue;
        return nullptr;
    }
    auto command = new NCPCommandWait(m_resource, m_curDelayTimeValue, -1, -1, 0);
    return command;
}

void ProgramMenu3_1::show(NCPCommand *command)
{
    //qDebug() << "command type:" << command->type();
    auto waitCommand = dynamic_cast<NCPCommandWait*>(command);
    QString commandLine = waitCommand->toCode();
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    QStringList splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug() << "ProgramMenu3_1::show:" << "Syntax error commandLine:[" << commandLine << "]";
        return;
    }
    //qDebug() << "splitList is value : " << splitList[1];
    setCurDelayTimeValue(splitList[1]);
}

void ProgramMenu3_1::onActivate()
{
    //qDebug("ProgramMenu3_1 %s:%d", __func__, __LINE__);
}

void ProgramMenu3_1::onDeactivate()
{
    //qDebug("ProgramMenu3_1 %s:%d", __func__, __LINE__);
}

void ProgramMenu3_1::setCurDelayTimeValue(QString curDelayTimeValue)
{
    //qDebug() << "curDelayTimeValue:" << curDelayTimeValue;
    if (m_curDelayTimeValue == curDelayTimeValue){
        return;
    }
    m_curDelayTimeValue = curDelayTimeValue;
    emit curDelayTimeValueChanged(m_curDelayTimeValue);
}
