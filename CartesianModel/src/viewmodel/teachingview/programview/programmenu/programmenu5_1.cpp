#include "programmenu5_1.h"
#include "../command/ncpcommandpause.h"
#include "../command/ncpcommandend.h"
#include "../command/ncpcommandalm.h"
#include <QRegularExpression>

ProgramMenu5_1::ProgramMenu5_1(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
{
}

void ProgramMenu5_1::onActivate()
{
    //qDebug("ProgramMenu5_1 %s:%d", __func__, __LINE__);
}

void ProgramMenu5_1::onDeactivate()
{
    //qDebug("ProgramMenu5_1 %s:%d", __func__, __LINE__);
}

NCPCommand *ProgramMenu5_1::createNcpCommand()
{
    //qDebug("ProgramMenu5_1 %s:%d m_curSelectIndex[%d]", __func__, __LINE__, m_curSelectIndex);
    switch(m_curSelectIndex){
    case (int)SystemItem::SystemItemApplyStop:{
        auto command = new NCPCommandPause(m_resource, (int)NCPCommandPause::PauseCategory::PauseCategoryOn);
        return command;
    }
    case (int)SystemItem::SystemItemCancelStop:{
        auto command = new NCPCommandPause(m_resource, (int)NCPCommandPause::PauseCategory::PauseCategoryOff);
        return command;
    }
    case (int)SystemItem::SystemItemProgEnd:{
        auto command = new NCPCommandEnd(m_resource);
        return command;
    }
    case (int)SystemItem::SystemItemAlarm:{
        if (m_curAlarmSelectIndex < 0){
            qDebug("ProgramMenu5_1 %s:%d m_curAlarmSelectIndex[%d] is invalid", __func__, __LINE__, m_curAlarmSelectIndex);
            return nullptr;
        }
        auto command = new NCPCommandAlm(m_resource, m_curAlarmSelectIndex);
        return command;
    }
    default:
        qDebug("ProgramMenu5_1 %s:%d m_curSelectIndex[%d] is invalid", __func__, __LINE__, m_curSelectIndex);
        return nullptr;
    }
}

void ProgramMenu5_1::show(NCPCommand *command)
{
    //qDebug() << "ProgramMenu5_1::show command type:" << command->type();
    switch (command->type()) {
    case NCPCommand::Type::Pause:{
        auto pauseCommand = dynamic_cast<NCPCommandPause*>(command);
        QString commandLine = pauseCommand->toCode();
        QRegularExpression commandRe("(\\s+|\\t+|,)");
        QStringList splitList = commandLine.split(commandRe);
        if (splitList.size() < 2) {
            qDebug() << "ProgramMenu5_1::show:" << "Syntax error commandLine:[" << commandLine << "]";
            return;
        }
        if(splitList[1] == "ON"){
            setItemSelect((int)SystemItem::SystemItemApplyStop + 1);
        } else if (splitList[1] == "OFF"){
            setItemSelect((int)SystemItem::SystemItemCancelStop + 1);
        }
        break;
    }
    case NCPCommand::Type::End:
        setItemSelect((int)SystemItem::SystemItemProgEnd + 1);
        break;
    case NCPCommand::Type::Alm:{
        auto almCommand = dynamic_cast<NCPCommandAlm*>(command);
        QString commandLine = almCommand->toCode();
        QRegularExpression commandRe("(\\s+|\\t+|,)");
        QStringList splitList = commandLine.split(commandRe);
        if (splitList.size() < 2) {
            qDebug() << "ProgramMenu5_1::show:" << "Syntax error commandLine:[" << commandLine << "]";
            return;
        }
        bool is_ok;
        int almIndex = splitList[1].toInt(&is_ok, 10);
        if (!is_ok || (almIndex < 1 || almIndex > 55)){
            qDebug() << "error is_ok: " << is_ok << "almIndex:" << almIndex;
            return;
        }
        setItemSelect((int)SystemItem::SystemItemAlarm + 1);
        setCurAlarmSelectIndex(almIndex - 1);
        break;
    }
    default:
        break;
    }
}

void ProgramMenu5_1::setCurSelectIndex(int curSelectIndex)
{
    if (m_curSelectIndex == curSelectIndex){
        return;
    }
    m_curSelectIndex = curSelectIndex;
    emit curSelectIndexChanged(m_curSelectIndex);
}

void ProgramMenu5_1::setCurAlarmSelectIndex(int curAlarmSelectIndex)
{
    if (m_curAlarmSelectIndex == curAlarmSelectIndex){
        return;
    }
    m_curAlarmSelectIndex = curAlarmSelectIndex;
    emit curAlarmSelectIndexChanged(m_curAlarmSelectIndex);
}

void ProgramMenu5_1::setItemSelect(int itemSelect)
{
    if (m_itemSelect == itemSelect){
        return;
    }
    m_itemSelect = itemSelect;
    emit itemSelectChanged(m_itemSelect);
}
