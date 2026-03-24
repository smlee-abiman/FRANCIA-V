#ifndef VARIABLEVIEWMODEL_H
#define VARIABLEVIEWMODEL_H

#include "viewmodelbase.h"

#define MENU1_PENSET_START_ROW              73
#define MENU2_1_PENSET_START_ROW            81
#define MENU2_2_PENSET_START_ROW            89
#define MENU3_PENSET_START_ROW              97
#define MENU4_PENSET_START_ROW              105
#define MENU5_PENSET_START_ROW              113
#define MENU6_1_PENSET_START_ROW            121
#define MENU6_2_PENSET_START_ROW            129
#define MENU7_1_PENSET_START_ROW            137
#define MENU7_2_PENSET_START_ROW            145
#define MENU8_1_PENSET_START_ROW            153
#define MENU8_2_PENSET_START_ROW            161
#define MENU1_PENSET_COLUMN_MAX             16
#define MENU2_1_PENSET_COLUMN_MAX           24
#define MENU2_2_PENSET_COLUMN_MAX           16
#define MENU3_PENSET_COLUMN_MAX             16
#define MENU4_PENSET_COLUMN_MAX             13
#define MENU5_PENSET_COLUMN_MAX             10
#define MENU6_1_PENSET_COLUMN_MAX           8
#define MENU6_2_PENSET_COLUMN_MAX           8
#define MENU7_1_PENSET_COLUMN_MAX           8
#define MENU7_2_PENSET_COLUMN_MAX           8
#define MENU8_1_PENSET_COLUMN_MAX           8
#define MENU8_2_PENSET_COLUMN_MAX           8

class VariableViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(int menuIndex READ menuIndex WRITE setMenuIndex NOTIFY menuIndexChanged)//菜单索引
    Q_PROPERTY(QString outVariableStr READ outVariableStr WRITE setOutVariableStr NOTIFY outVariableStrChanged)//输出的变量字符
    Q_PROPERTY(QString outMark MEMBER m_outMark NOTIFY outMarkChanged)
    Q_PROPERTY(QString outMsg MEMBER m_outMsg NOTIFY outMsgChanged)
    Q_PROPERTY(QString outSymbol MEMBER m_outSymbol NOTIFY outSymbolChanged)
    Q_PROPERTY(int outValue MEMBER m_outValue NOTIFY outValueChanged)
    Q_PROPERTY(int outAddr MEMBER m_outAddr NOTIFY outAddrChanged)
    Q_PROPERTY(QList<bool> menu_State READ menu_State WRITE setMenu_State NOTIFY menu_StateChanged)
    Q_PROPERTY(int variable1SelectIndex READ variable1SelectIndex WRITE setVariable1SelectIndex NOTIFY variable1SelectIndexChanged)//变量1号菜单选择索引
    Q_PROPERTY(int variable2_1SelectIndex READ variable2_1SelectIndex WRITE setVariable2_1SelectIndex NOTIFY variable2_1SelectIndexChanged)//变量2_1号菜单选择索引
    Q_PROPERTY(int variable2_2SelectIndex READ variable2_2SelectIndex WRITE setVariable2_2SelectIndex NOTIFY variable2_2SelectIndexChanged)//变量2_2号菜单选择索引
    Q_PROPERTY(int variable3SelectIndex READ variable3SelectIndex WRITE setVariable3SelectIndex NOTIFY variable3SelectIndexChanged)//变量3号菜单选择索引
    Q_PROPERTY(int variable4SelectIndex READ variable4SelectIndex WRITE setVariable4SelectIndex NOTIFY variable4SelectIndexChanged)//变量4号菜单选择索引
    Q_PROPERTY(int variable5SelectIndex READ variable5SelectIndex WRITE setVariable5SelectIndex NOTIFY variable5SelectIndexChanged)//变量5号菜单选择索引
    Q_PROPERTY(int variable6_1SelectIndex READ variable6_1SelectIndex WRITE setVariable6_1SelectIndex NOTIFY variable6_1SelectIndexChanged)//变量6_1号菜单选择索引
    Q_PROPERTY(int variable6_2SelectIndex READ variable6_2SelectIndex WRITE setVariable6_2SelectIndex NOTIFY variable6_2SelectIndexChanged)//变量6_2号菜单选择索引
    Q_PROPERTY(int variable7_1SelectIndex READ variable7_1SelectIndex WRITE setVariable7_1SelectIndex NOTIFY variable7_1SelectIndexChanged)//变量7_1号菜单选择索引
    Q_PROPERTY(int variable7_2SelectIndex READ variable7_2SelectIndex WRITE setVariable7_2SelectIndex NOTIFY variable7_2SelectIndexChanged)//变量7_2号菜单选择索引
    Q_PROPERTY(int variable8_1SelectIndex READ variable8_1SelectIndex WRITE setVariable8_1SelectIndex NOTIFY variable8_1SelectIndexChanged)//变量8_1号菜单选择索引
    Q_PROPERTY(int variable8_2SelectIndex READ variable8_2SelectIndex WRITE setVariable8_2SelectIndex NOTIFY variable8_2SelectIndexChanged)//变量8_2号菜单选择索引

    Q_PROPERTY(QList<int> menu1Addr MEMBER m_menu1_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu2_1Addr MEMBER m_menu2_1_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu2_2Addr MEMBER m_menu2_2_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu3Addr MEMBER m_menu3_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu4Addr MEMBER m_menu4_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu5Addr MEMBER m_menu5_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu6_1Addr MEMBER m_menu6_1_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu6_2Addr MEMBER m_menu6_2_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu7_1Addr MEMBER m_menu7_1_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu7_2Addr MEMBER m_menu7_2_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu8_1Addr MEMBER m_menu8_1_Addr CONSTANT)
    Q_PROPERTY(QList<int> menu8_2Addr MEMBER m_menu8_2_Addr CONSTANT)
    Q_PROPERTY(int menu1_Num READ menu1_Num CONSTANT)
    Q_PROPERTY(int menu2_1_Num READ menu2_1_Num CONSTANT)
    Q_PROPERTY(int menu2_2_Num READ menu2_2_Num CONSTANT)
    Q_PROPERTY(int menu3_Num READ menu3_Num CONSTANT)
    Q_PROPERTY(int menu4_Num READ menu4_Num CONSTANT)
    Q_PROPERTY(int menu5_Num READ menu5_Num CONSTANT)
    Q_PROPERTY(int menu6_1_Num READ menu6_1_Num CONSTANT)
    Q_PROPERTY(int menu6_2_Num READ menu6_2_Num CONSTANT)
    Q_PROPERTY(int menu7_1_Num READ menu7_1_Num CONSTANT)
    Q_PROPERTY(int menu7_2_Num READ menu7_2_Num CONSTANT)
    Q_PROPERTY(int menu8_1_Num READ menu8_1_Num CONSTANT)
    Q_PROPERTY(int menu8_2_Num READ menu8_2_Num CONSTANT)

    Q_PROPERTY(QList<bool> menu1_State READ menu1_State WRITE setMenu1_State NOTIFY menu1_StateChanged)
    Q_PROPERTY(QList<bool> menu2_1_State READ menu2_1_State WRITE setMenu2_1_State NOTIFY menu2_1_StateChanged)
    Q_PROPERTY(QList<bool> menu3_State READ menu3_State WRITE setMenu3_State NOTIFY menu3_StateChanged)
    Q_PROPERTY(QList<bool> menu4_State READ menu4_State WRITE setMenu4_State NOTIFY menu4_StateChanged)
    Q_PROPERTY(QList<bool> menu5_State READ menu5_State WRITE setMenu5_State NOTIFY menu5_StateChanged)
    Q_PROPERTY(QList<bool> menu6_1_State READ menu6_1_State WRITE setMenu6_1_State NOTIFY menu6_1_StateChanged)
    Q_PROPERTY(QList<bool> menu7_1_State READ menu7_1_State WRITE setMenu7_1_State NOTIFY menu7_1_StateChanged)
    Q_PROPERTY(QList<bool> menu8_1_State READ menu8_1_State WRITE setMenu8_1_State NOTIFY menu8_1_StateChanged)

    Q_PROPERTY(bool menu2_Type READ menu2_Type WRITE setMenu2_Type NOTIFY menu2_TypeChanged)
    Q_PROPERTY(bool menu6_Type READ menu6_Type WRITE setMenu6_Type NOTIFY menu6_TypeChanged)
    Q_PROPERTY(bool menu7_Type READ menu7_Type WRITE setMenu7_Type NOTIFY menu7_TypeChanged)
    Q_PROPERTY(bool menu8_Type READ menu8_Type WRITE setMenu8_Type NOTIFY menu8_TypeChanged)

    Q_PROPERTY(QList<int> menu2_2_Value READ menu2_2_Value WRITE setMenu2_2_Value NOTIFY menu2_2_ValueChanged)//2-2变量字节输出的值
    Q_PROPERTY(QList<int> menu6_2_Value READ menu6_2_Value WRITE setMenu6_2_Value NOTIFY menu6_2_ValueChanged)//6-2变量字节输出的值
    Q_PROPERTY(QList<int> menu7_2_Value READ menu7_2_Value WRITE setMenu7_2_Value NOTIFY menu7_2_ValueChanged)//7-2变量字节输出的值
    Q_PROPERTY(QList<int> menu8_2_Value READ menu8_2_Value WRITE setMenu8_2_Value NOTIFY menu8_2_ValueChanged)//8-2变量字节输出的值

    Q_PROPERTY(QList<QString> menu6_2_Symbol READ menu6_2_Symbol WRITE setMenu6_2_Symbol NOTIFY menu6_2_SymbolChanged)//6-2变量画面符号
    Q_PROPERTY(QList<QString> menu7_2_Symbol READ menu7_2_Symbol WRITE setMenu7_2_Symbol NOTIFY menu7_2_SymbolChanged)//7-2变量画面符号
    Q_PROPERTY(QList<QString> menu8_2_Symbol READ menu8_2_Symbol WRITE setMenu8_2_Symbol NOTIFY menu8_2_SymbolChanged)//8-2变量画面符号

public:
    explicit VariableViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~VariableViewModel();

    int menuIndex(){
        return m_menuIndex;
    }
    QString outVariableStr(){
        return m_outVariableStr;
    }
    QList<bool> menu_State() const{
        return m_menu_State;
    }

    int variable1SelectIndex(){
        return m_variable1SelectIndex;
    }
    int variable2_1SelectIndex(){
        return m_variable2_1SelectIndex;
    }
    int variable2_2SelectIndex(){
        return m_variable2_2SelectIndex;
    }
    int variable3SelectIndex(){
        return m_variable3SelectIndex;
    }
    int variable4SelectIndex(){
        return m_variable4SelectIndex;
    }
    int variable5SelectIndex(){
        return m_variable5SelectIndex;
    }
    int variable6_1SelectIndex(){
        return m_variable6_1SelectIndex;
    }
    int variable6_2SelectIndex(){
        return m_variable6_2SelectIndex;
    }
    int variable7_1SelectIndex(){
        return m_variable7_1SelectIndex;
    }
    int variable7_2SelectIndex(){
        return m_variable7_2SelectIndex;
    }
    int variable8_1SelectIndex(){
        return m_variable8_1SelectIndex;
    }
    int variable8_2SelectIndex(){
        return m_variable8_2SelectIndex;
    }

    int menu1_Num() const {
        return m_menu1_Num;
    }
    int menu2_1_Num() const {
        return m_menu2_1_Num;
    }
    int menu2_2_Num() const {
        return m_menu2_2_Num;
    }
    int menu3_Num() const {
        return m_menu3_Num;
    }
    int menu4_Num() const {
        return m_menu4_Num;
    }
    int menu5_Num() const {
        return m_menu5_Num;
    }
    int menu6_1_Num() const {
        return m_menu6_1_Num;
    }
    int menu6_2_Num() const {
        return m_menu6_2_Num;
    }
    int menu7_1_Num() const {
        return m_menu7_1_Num;
    }
    int menu7_2_Num() const {
        return m_menu7_2_Num;
    }
    int menu8_1_Num() const {
        return m_menu8_1_Num;
    }
    int menu8_2_Num() const {
        return m_menu8_2_Num;
    }

    QList<bool> menu1_State() {
        return m_menu1_State;
    }
    QList<bool> menu2_1_State() {
        return m_menu2_1_State;
    }
    QList<bool> menu3_State() {
        return m_menu3_State;
    }
    QList<bool> menu4_State() {
        return m_menu4_State;
    }
    QList<bool> menu5_State() {
        return m_menu5_State;
    }
    QList<bool> menu6_1_State() {
        return m_menu6_1_State;
    }
    QList<bool> menu7_1_State() {
        return m_menu7_1_State;
    }
    QList<bool> menu8_1_State() {
        return m_menu8_1_State;
    }
    bool menu2_Type() {
        return m_menu2_Type;
    }
    bool menu6_Type() {
        return m_menu6_Type;
    }
    bool menu7_Type() {
        return m_menu7_Type;
    }
    bool menu8_Type() {
        return m_menu8_Type;
    }

    QList<int> menu2_2_Value() {
        return m_menu2_2_Value;
    }
    QList<int> menu6_2_Value() {
        return m_menu6_2_Value;
    }
    QList<int> menu7_2_Value() {
        return m_menu7_2_Value;
    }
    QList<int> menu8_2_Value() {
        return m_menu8_2_Value;
    }

    QList<QString> menu6_2_Symbol() {
        return m_menu6_2_Symbol;
    }
    QList<QString> menu7_2_Symbol() {
        return m_menu7_2_Symbol;
    }
    QList<QString> menu8_2_Symbol() {
        return m_menu8_2_Symbol;
    }

    //函数
    void menuInit();
    Q_INVOKABLE void setVariableStr(int menuIndex);

signals:
    void menuIndexChanged(int menuIndex);
    void outVariableStrChanged(QString outVariableStr);
    void outMarkChanged();
    void outMsgChanged();
    void outSymbolChanged();
    void outValueChanged();
    void outAddrChanged();
    void menu_StateChanged(QList<bool> menu_State);

    void variable1SelectIndexChanged(int variable1SelectIndex);
    void variable2_1SelectIndexChanged(int variable2_1SelectIndex);
    void variable2_2SelectIndexChanged(int variable2_2SelectIndex);
    void variable3SelectIndexChanged(int variable3SelectIndex);
    void variable4SelectIndexChanged(int variable4SelectIndex);
    void variable5SelectIndexChanged(int variable5SelectIndex);
    void variable6_1SelectIndexChanged(int variable6_1SelectIndex);
    void variable6_2SelectIndexChanged(int variable6_2SelectIndex);
    void variable7_1SelectIndexChanged(int variable7_1SelectIndex);
    void variable7_2SelectIndexChanged(int variable7_2SelectIndex);
    void variable8_1SelectIndexChanged(int variable8_1SelectIndex);
    void variable8_2SelectIndexChanged(int variable8_2SelectIndex);

    void menu1_StateChanged(QList<bool> menu1_State);
    void menu2_1_StateChanged(QList<bool> menu2_1_State);
    void menu3_StateChanged(QList<bool> menu3_State);
    void menu4_StateChanged(QList<bool> menu4_State);
    void menu5_StateChanged(QList<bool> menu5_State);
    void menu6_1_StateChanged(QList<bool> menu6_1_State);
    void menu7_1_StateChanged(QList<bool> menu7_1_State);
    void menu8_1_StateChanged(QList<bool> menu8_1_State);
    void menu2_TypeChanged(bool menu2_Type);
    void menu6_TypeChanged(bool menu6_Type);
    void menu7_TypeChanged(bool menu7_Type);
    void menu8_TypeChanged(bool menu8_Type);

    void menu2_2_ValueChanged(QList<int> menu2_2_Value);
    void menu6_2_ValueChanged(QList<int> menu6_2_Value);
    void menu7_2_ValueChanged(QList<int> menu7_2_Value);
    void menu8_2_ValueChanged(QList<int> menu8_2_Value);

    void menu6_2_SymbolChanged(QList<QString> menu6_2_Symbol);
    void menu7_2_SymbolChanged(QList<QString> menu7_2_Symbol);
    void menu8_2_SymbolChanged(QList<QString> menu8_2_Symbol);

public slots:
    void setMenuIndex(int menuIndex){
        if(m_menuIndex == menuIndex)
            return;
        m_menuIndex = menuIndex;
        emit menuIndexChanged(menuIndex);
    }
    void setOutVariableStr(QString outVariableStr){
        if(m_outVariableStr == outVariableStr)
            return;
        m_outVariableStr = outVariableStr;
        emit outVariableStrChanged(outVariableStr);
    }

    void setVariable1SelectIndex(int variable1SelectIndex){
        if(m_variable1SelectIndex == variable1SelectIndex)
            return;
        m_variable1SelectIndex = variable1SelectIndex;
        emit variable1SelectIndexChanged(variable1SelectIndex);
    }
    void setVariable2_1SelectIndex(int variable2_1SelectIndex){
        if(m_variable2_1SelectIndex == variable2_1SelectIndex)
            return;
        m_variable2_1SelectIndex = variable2_1SelectIndex;
        emit variable2_1SelectIndexChanged(variable2_1SelectIndex);
    }
    void setVariable2_2SelectIndex(int variable2_2SelectIndex){
        if(m_variable2_2SelectIndex == variable2_2SelectIndex)
            return;
        m_variable2_2SelectIndex = variable2_2SelectIndex;
        emit variable2_2SelectIndexChanged(variable2_2SelectIndex);
    }
    void setVariable3SelectIndex(int variable3SelectIndex){
        if(m_variable3SelectIndex == variable3SelectIndex)
            return;
        m_variable3SelectIndex = variable3SelectIndex;
        emit variable3SelectIndexChanged(variable3SelectIndex);
    }
    void setVariable4SelectIndex(int variable4SelectIndex){
        if(m_variable4SelectIndex == variable4SelectIndex)
            return;
        m_variable4SelectIndex = variable4SelectIndex;
        emit variable4SelectIndexChanged(variable4SelectIndex);
    }
    void setVariable5SelectIndex(int variable5SelectIndex){
        if(m_variable5SelectIndex == variable5SelectIndex)
            return;
        m_variable5SelectIndex = variable5SelectIndex;
        emit variable5SelectIndexChanged(variable5SelectIndex);
    }
    void setVariable6_1SelectIndex(int variable6_1SelectIndex){
        if(m_variable6_1SelectIndex == variable6_1SelectIndex)
            return;
        m_variable6_1SelectIndex = variable6_1SelectIndex;
        emit variable6_1SelectIndexChanged(variable6_1SelectIndex);
    }
    void setVariable6_2SelectIndex(int variable6_2SelectIndex){
        if(m_variable6_2SelectIndex == variable6_2SelectIndex)
            return;
        m_variable6_2SelectIndex = variable6_2SelectIndex;
        emit variable6_2SelectIndexChanged(variable6_2SelectIndex);
    }
    void setVariable7_1SelectIndex(int variable7_1SelectIndex){
        if(m_variable7_1SelectIndex == variable7_1SelectIndex)
            return;
        m_variable7_1SelectIndex = variable7_1SelectIndex;
        emit variable7_1SelectIndexChanged(variable7_1SelectIndex);
    }
    void setVariable7_2SelectIndex(int variable7_2SelectIndex){
        if(m_variable7_2SelectIndex == variable7_2SelectIndex)
            return;
        m_variable7_2SelectIndex = variable7_2SelectIndex;
        emit variable7_2SelectIndexChanged(variable7_2SelectIndex);
    }
    void setVariable8_1SelectIndex(int variable8_1SelectIndex){
        if(m_variable8_1SelectIndex == variable8_1SelectIndex)
            return;
        m_variable8_1SelectIndex = variable8_1SelectIndex;
        emit variable8_1SelectIndexChanged(variable8_1SelectIndex);
    }
    void setVariable8_2SelectIndex(int variable8_2SelectIndex){
        if(m_variable8_2SelectIndex == variable8_2SelectIndex)
            return;
        m_variable8_2SelectIndex = variable8_2SelectIndex;
        emit variable8_2SelectIndexChanged(variable8_2SelectIndex);
    }

    void setMenu_State(QList<bool> menu_State){
        if(m_menu_State == menu_State)
            return;
        m_menu_State = menu_State;
        emit menu_StateChanged(menu_State);
    }
    void setMenu1_State(QList<bool> menu1_State){
        if(m_menu1_State == menu1_State)
            return;
        m_menu1_State = menu1_State;
        emit menu1_StateChanged(menu1_State);
    }
    void setMenu2_1_State(QList<bool> menu2_1_State){
        if(m_menu2_1_State == menu2_1_State)
            return;
        m_menu2_1_State = menu2_1_State;
        emit menu2_1_StateChanged(menu2_1_State);
    }
    void setMenu3_State(QList<bool> menu3_State){
        if(m_menu3_State == menu3_State)
            return;
        m_menu3_State = menu3_State;
        emit menu3_StateChanged(menu3_State);
    }
    void setMenu4_State(QList<bool> menu4_State){
        if(m_menu4_State == menu4_State)
            return;
        m_menu4_State = menu4_State;
        emit menu4_StateChanged(menu4_State);
    }
    void setMenu5_State(QList<bool> menu5_State){
        if(m_menu5_State == menu5_State)
            return;
        m_menu5_State = menu5_State;
        emit menu5_StateChanged(menu5_State);
    }
    void setMenu6_1_State(QList<bool> menu6_1_State){
        if(m_menu6_1_State == menu6_1_State)
            return;
        m_menu6_1_State = menu6_1_State;
        emit menu6_1_StateChanged(menu6_1_State);
    }
    void setMenu7_1_State(QList<bool> menu7_1_State){
        if(m_menu7_1_State == menu7_1_State)
            return;
        m_menu7_1_State = menu7_1_State;
        emit menu7_1_StateChanged(menu7_1_State);
    }
    void setMenu8_1_State(QList<bool> menu8_1_State){
        if(m_menu8_1_State == menu8_1_State)
            return;
        m_menu8_1_State = menu8_1_State;
        emit menu8_1_StateChanged(menu8_1_State);
    }
    void setMenu2_Type(bool menu2_Type){
        if(m_menu2_Type == menu2_Type)
            return;
        m_menu2_Type = menu2_Type;
        emit menu2_TypeChanged(menu2_Type);
    }
    void setMenu6_Type(bool menu6_Type){
        if(m_menu6_Type == menu6_Type)
            return;
        m_menu6_Type = menu6_Type;
        emit menu6_TypeChanged(menu6_Type);
    }
    void setMenu7_Type(bool menu7_Type){
        if(m_menu7_Type == menu7_Type)
            return;
        m_menu7_Type = menu7_Type;
        emit menu7_TypeChanged(menu7_Type);
    }
    void setMenu8_Type(bool menu8_Type){
        if(m_menu8_Type == menu8_Type)
            return;
        m_menu8_Type = menu8_Type;
        emit menu8_TypeChanged(menu8_Type);
    }

    void setMenu2_2_Value(QList<int> menu2_2_Value){
        if(m_menu2_2_Value == menu2_2_Value)
            return;
        m_menu2_2_Value = menu2_2_Value;
        emit menu2_2_ValueChanged(menu2_2_Value);
    }
    void setMenu6_2_Value(QList<int> menu6_2_Value){
        if(m_menu6_2_Value == menu6_2_Value)
            return;
        m_menu6_2_Value = menu6_2_Value;
        emit menu6_2_ValueChanged(menu6_2_Value);
    }
    void setMenu7_2_Value(QList<int> menu7_2_Value){
        if(m_menu7_2_Value == menu7_2_Value)
            return;
        m_menu7_2_Value = menu7_2_Value;
        emit menu7_2_ValueChanged(menu7_2_Value);
    }
    void setMenu8_2_Value(QList<int> menu8_2_Value){
        if(m_menu8_2_Value == menu8_2_Value)
            return;
        m_menu8_2_Value = menu8_2_Value;
        emit menu8_2_ValueChanged(menu8_2_Value);
    }

    void setMenu6_2_Symbol(QList<QString> menu6_2_Symbol){
        if(m_menu6_2_Symbol == menu6_2_Symbol)
            return;
        m_menu6_2_Symbol = menu6_2_Symbol;
        emit menu6_2_SymbolChanged(menu6_2_Symbol);
    }
    void setMenu7_2_Symbol(QList<QString> menu7_2_Symbol){
        if(m_menu7_2_Symbol == menu7_2_Symbol)
            return;
        m_menu7_2_Symbol = menu7_2_Symbol;
        emit menu7_2_SymbolChanged(menu7_2_Symbol);
    }
    void setMenu8_2_Symbol(QList<QString> menu8_2_Symbol){
        if(m_menu8_2_Symbol == menu8_2_Symbol)
            return;
        m_menu8_2_Symbol = menu8_2_Symbol;
        emit menu8_2_SymbolChanged(menu8_2_Symbol);
    }

private:
    int m_menuIndex = 0;
    QString m_outVariableStr;//变量选择要输出的变量字符过渡
    QString m_outMark = "";
    QString m_outMsg = "";
    QString m_outSymbol = "";
    int     m_outValue = 0;
    int     m_outAddr = 0;

    QList<bool> m_menu_State = {1,1,1,1,1,1,1,1};

    int m_variable1SelectIndex = -1;
    int m_variable2_1SelectIndex = -1;
    int m_variable2_2SelectIndex = -1;
    int m_variable3SelectIndex = -1;
    int m_variable4SelectIndex = -1;
    int m_variable5SelectIndex = -1;
    int m_variable6_1SelectIndex = -1;
    int m_variable6_2SelectIndex = -1;
    int m_variable7_1SelectIndex = -1;
    int m_variable7_2SelectIndex = -1;
    int m_variable8_1SelectIndex = -1;
    int m_variable8_2SelectIndex = -1;

    int m_menu1_Num = 0;
    int m_menu2_1_Num = 0;
    int m_menu2_2_Num = 0;
    int m_menu3_Num = 0;
    int m_menu4_Num = 0;
    int m_menu5_Num = 0;
    int m_menu6_1_Num = 0;
    int m_menu6_2_Num = 0;
    int m_menu7_1_Num = 0;
    int m_menu7_2_Num = 0;
    int m_menu8_1_Num = 0;
    int m_menu8_2_Num = 0;

    QList<int>      m_menu1_Addr;
    QList<int>      m_menu2_1_Addr;
    QList<int>      m_menu2_2_Addr;
    QList<int>      m_menu3_Addr;
    QList<int>      m_menu4_Addr;
    QList<int>      m_menu5_Addr;
    QList<int>      m_menu6_1_Addr;
    QList<int>      m_menu6_2_Addr;
    QList<int>      m_menu7_1_Addr;
    QList<int>      m_menu7_2_Addr;
    QList<int>      m_menu8_1_Addr;
    QList<int>      m_menu8_2_Addr;

    QList<bool> m_menu1_State;
    QList<bool> m_menu2_1_State;
    QList<bool> m_menu3_State;
    QList<bool> m_menu4_State;
    QList<bool> m_menu5_State;
    QList<bool> m_menu6_1_State;
    QList<bool> m_menu7_1_State;
    QList<bool> m_menu8_1_State;

    bool m_menu2_Type = 0;
    bool m_menu6_Type = 0;
    bool m_menu7_Type = 0;
    bool m_menu8_Type = 0;

    QList<int> m_menu2_2_Value;
    QList<int> m_menu6_2_Value;
    QList<int> m_menu7_2_Value;
    QList<int> m_menu8_2_Value;

    QList<QString> m_menu6_2_Symbol;
    QList<QString> m_menu7_2_Symbol;
    QList<QString> m_menu8_2_Symbol;
};

#endif // VARIABLEVIEWMODEL_H
