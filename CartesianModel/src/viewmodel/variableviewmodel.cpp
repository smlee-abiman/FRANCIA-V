#include "variableviewmodel.h"

VariableViewModel::VariableViewModel(ResourceManager *resource, CommunicationThread * comm)
    : ViewModelBase(resource , comm)
{
    menuInit();
}

VariableViewModel::~VariableViewModel()
{

}

void VariableViewModel::menuInit()
{
    for (int i = 0; i < MENU1_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU1_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu1_Num ++;
                m_menu1_Addr.append(addr.toInt());
                m_menu1_State.append(false);
            }else {
                continue;
            }
        }
    }//输入
    for (int i = 0; i < MENU2_1_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU2_1_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu2_1_Num ++;
                m_menu2_1_Addr.append(addr.toInt());
                m_menu2_1_State.append(false);
            }else {
                continue;
            }
        }
    }//内部输出—bit
    for (int i = 0; i < MENU2_2_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU2_2_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu2_2_Num ++;
                m_menu2_2_Addr.append(addr.toInt());
                m_menu2_2_Value.append(0);
            }else {
                continue;
            }
        }
    }//内部输出—byte
    for (int i = 0; i < MENU3_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU3_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu3_Num ++;
                m_menu3_Addr.append(addr.toInt());
                m_menu3_State.append(false);
            }else {
                continue;
            }
        }
    }//外部输出
    for (int i = 0; i < MENU4_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU4_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu4_Num ++;
                m_menu4_Addr.append(addr.toInt());
                m_menu4_State.append(false);
            }else {
                continue;
            }
        }
    }//计时器
    for (int i = 0; i < MENU5_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU5_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu5_Num ++;
                m_menu5_Addr.append(addr.toInt());
                m_menu5_State.append(false);
            }else {
                continue;
            }
        }
    }//计数器
    for (int i = 0; i < MENU6_1_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU6_1_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu6_1_Num ++;
                m_menu6_1_Addr.append(addr.toInt());
                m_menu6_1_State.append(false);
            }else {
                continue;
            }
        }
    }//轴控制-bit
    for (int i = 0; i < MENU6_2_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU6_2_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu6_2_Num ++;
                m_menu6_2_Addr.append(addr.toInt());
                m_menu6_2_Symbol.append("=");
                m_menu6_2_Value.append(0);
            }else {
                continue;
            }
        }
    }//轴控制-byte
    for (int i = 0; i < MENU7_1_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU7_1_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu7_1_Num ++;
                m_menu7_1_Addr.append(addr.toInt());
                m_menu7_1_State.append(false);
            }else {
                continue;
            }
        }
    }//用户-bit
    for (int i = 0; i < MENU7_2_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU7_2_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu7_2_Num ++;
                m_menu7_2_Addr.append(addr.toInt());
                m_menu7_2_Symbol.append("=");
                m_menu7_2_Value.append(0);
            }else {
                continue;
            }
        }
    }//用户-byte
    for (int i = 0; i < MENU8_1_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU8_1_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu8_1_Num ++;
                m_menu8_1_Addr.append(addr.toInt());
                m_menu8_1_State.append(false);
            }else {
                continue;
            }
        }
    }//系统-bit
    for (int i = 0; i < MENU8_2_PENSET_COLUMN_MAX; i++) {
        for (int j = 0; j < 8; j++) {
            auto addr = m_resource->getPenSetStringValue("16",j+MENU8_2_PENSET_START_ROW,i);
            if (!addr.isEmpty()){
                m_menu8_2_Num ++;
                m_menu8_2_Addr.append(addr.toInt());
                m_menu8_2_Symbol.append("=");
                m_menu8_2_Value.append(0);
            }else {
                continue;
            }
        }
    }//系统-byte
}

void VariableViewModel::setVariableStr(int menuIndex)
{
    //QString variableStr;
    switch (menuIndex) {
    case 0:
        m_outAddr = m_menu1_Addr.value(m_variable1SelectIndex);
        m_outMark = m_resource->symbolAt(m_outAddr);
        m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
        m_outValue = m_menu1_State.value(m_variable1SelectIndex);
        m_outSymbol = "";
        m_outVariableStr = m_outMark + "(" + m_outMsg + ")" + " " + ((m_outValue == 1) ? "ON" : "OFF");
        break;
    case 1:
        if(m_menu2_Type == false){
            m_outAddr = m_menu2_1_Addr.value(m_variable2_1SelectIndex);
            m_outMark = m_resource->symbolAt(m_outAddr);
            m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
            m_outValue = m_menu2_1_State.value(m_variable2_1SelectIndex);
            m_outSymbol = "";
            m_outVariableStr = m_outMark + "(" + m_outMsg + ")" + " " + ((m_outValue == 1) ? "ON" : "OFF");
        }else {
            m_outAddr = m_menu2_2_Addr.value(m_variable2_2SelectIndex);
            m_outMark = m_resource->symbolAt(m_outAddr);
            m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
            m_outValue = m_menu2_2_Value.value(m_variable2_2SelectIndex);
            m_outSymbol = "=";
            m_outVariableStr = m_outMark + "(" + m_outMsg + ")" +  "=" + QString::number(m_outValue);
        }
        break;
    case 2:
        m_outAddr = m_menu3_Addr.value(m_variable3SelectIndex);
        m_outMark = m_resource->symbolAt(m_outAddr);
        m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
        m_outValue = m_menu3_State.value(m_variable3SelectIndex);
        m_outSymbol = "";
        m_outVariableStr = m_outMark + "(" + m_outMsg + ")" + " " + ((m_outValue == 1) ? "ON" : "OFF");
        break;
    case 3:
        m_outAddr = m_menu4_Addr.value(m_variable4SelectIndex);
        m_outMark = m_resource->symbolAt(m_outAddr);
        m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
        m_outValue = m_menu4_State.value(m_variable4SelectIndex);
        m_outSymbol = "";
        m_outVariableStr = m_outMark + "(" + m_outMsg + ")" + " " + ((m_outValue == 1) ? "ON" : "OFF");
        break;
    case 4:
        m_outAddr = m_menu5_Addr.value(m_variable5SelectIndex);
        m_outMark = m_resource->symbolAt(m_outAddr);
        m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
        m_outValue = m_menu5_State.value(m_variable5SelectIndex);
        m_outSymbol = "";
        m_outVariableStr = m_outMark + "(" + m_outMsg + ")" + " " + ((m_outValue == 1) ? "ON" : "OFF");
        break;
    case 5:
        if(m_menu6_Type == false){
            m_outAddr = m_menu6_1_Addr.value(m_variable6_1SelectIndex);
            m_outMark = m_resource->symbolAt(m_outAddr);
            m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
            m_outValue = m_menu6_1_State.value(m_variable6_1SelectIndex);
            m_outSymbol = "";
            m_outVariableStr = m_outMark + "(" + m_outMsg + ")" + " " + ((m_outValue == 1) ? "ON" : "OFF");
        }else {
            m_outAddr = m_menu6_2_Addr.value(m_variable6_2SelectIndex);
            m_outMark = m_resource->symbolAt(m_outAddr);
            m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
            m_outValue = m_menu6_2_Value.value(m_variable6_2SelectIndex);
            m_outSymbol = m_menu6_2_Symbol.value(m_variable6_2SelectIndex);
            m_outVariableStr = m_outMark + "(" + m_outMsg + ")" +  m_outSymbol + QString::number(m_outValue);
        }
        break;
    case 6:
        if(m_menu7_Type == false){
            m_outAddr = m_menu7_1_Addr.value(m_variable7_1SelectIndex);
            m_outMark = m_resource->symbolAt(m_outAddr);
            m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
            m_outValue = m_menu7_1_State.value(m_variable7_1SelectIndex);
            m_outSymbol = "";
            m_outVariableStr = m_outMark + "(" + m_outMsg + ")" + " " + ((m_outValue == 1) ? "ON" : "OFF");
        }else {
            m_outAddr = m_menu7_2_Addr.value(m_variable7_2SelectIndex);
            m_outMark = m_resource->symbolAt(m_outAddr);
            m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
            m_outValue = m_menu7_2_Value.value(m_variable7_2SelectIndex);
            m_outSymbol = m_menu7_2_Symbol.value(m_variable7_2SelectIndex);
            m_outVariableStr = m_outMark + "(" + m_outMsg + ")" +  m_outSymbol + QString::number(m_outValue);
        }
        break;
    case 7:
        if(m_menu8_Type == false){
            m_outAddr = m_menu8_1_Addr.value(m_variable8_1SelectIndex);
            m_outMark = m_resource->symbolAt(m_outAddr);
            m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
            m_outValue = m_menu8_1_State.value(m_variable8_1SelectIndex);
            m_outSymbol = "";
            m_outVariableStr = m_outMark + "(" + m_outMsg + ")" + " " + ((m_outValue == 1) ? "ON" : "OFF");
        }else {
            m_outAddr = m_menu8_2_Addr.value(m_variable8_2SelectIndex);
            m_outMark = m_resource->symbolAt(m_outAddr);
            m_outMsg = m_resource->getMessageStringValue("1", m_outAddr, m_resource->language());
            m_outValue = m_menu8_2_Value.value(m_variable8_2SelectIndex);
            m_outSymbol = m_menu8_2_Symbol.value(m_variable8_2SelectIndex);
            m_outVariableStr = m_outMark + "(" + m_outMsg + ")" +  m_outSymbol + QString::number(m_outValue);
        }
        break;
    }
}
