/*
 * Manage Layout, format, and raw data from controller.
 * Arrange necessary information from CsvReader and ModBus and make container for QML.
 * Provide utility functions to QML.
 */

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QDateTime>
#include <QObject>
#include <QPointer>
#include <QThread>
#include <QMutex>

#include "csvreader.h"
#include "packetdata.h"
#include "dataformat.h"
#include "../enums.h"

class TcpAxisDataClient;

typedef enum {
    INPUT_IO = 129,
    OUTPUT_IO = 897,
} SymbolStartAt;

class ResourceManager : public QObject
{
    Q_OBJECT

    // Properties for QML communication
    Q_PROPERTY(QString imagePath MEMBER m_imagePath CONSTANT)
    Q_PROPERTY(QJsonObject functionList READ functionList CONSTANT)
    Q_PROPERTY(QList<QObject *> manualJogs READ manualJogs CONSTANT)
    Q_PROPERTY(QList<QObject *> manualKeys READ manualKeys CONSTANT)
    Q_PROPERTY(int mmPage READ mmPage WRITE setMmPage NOTIFY mmPageChanged)
    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QList<int> menu READ menu NOTIFY menuChanged)
    Q_PROPERTY(QString applicationName READ applicationName WRITE setApplicationName NOTIFY applicationNameChanged)
    Q_PROPERTY(QString penMsgCsvVersion READ penMsgCsvVersion CONSTANT)
    Q_PROPERTY(QString penSetCsvVersion READ penSetCsvVersion CONSTANT)
    Q_PROPERTY(QString modbusAddressCsvVersion READ modbusAddressCsvVersion CONSTANT)

    // Dummy, will be replaced from...
    // 直交型
    Q_PROPERTY(int screenId READ screenId WRITE setScreenId NOTIFY screenIdChanged)//屏幕ID
    Q_PROPERTY(QString titleString READ titleString WRITE setTitleString NOTIFY titleStringChanged)//标题
    Q_PROPERTY(QString stateName READ stateName WRITE setStateName NOTIFY stateNameChanged)//操作状态名称
    Q_PROPERTY(int manipulateMode READ manipulateMode WRITE setManipulateMode NOTIFY manipulateModeChanged)//操作模式
    Q_PROPERTY(int menu_id READ menu_id WRITE setmenu_id NOTIFY menu_idChanged)
    Q_PROPERTY(int axis_select READ axis_select WRITE setaxis_select NOTIFY axis_selectChanged)//轴选择
    Q_PROPERTY(int ioexpress_switch READ ioexpress_switch WRITE setioexpress_switch NOTIFY ioexpress_switchChanged)//IO显示切换
    Q_PROPERTY(int Menu_select READ Menu_select WRITE setMenu_select NOTIFY Menu_selectChanged)//菜单选择
    Q_PROPERTY(QList<QString> userpassword READ userpassword WRITE setuserpassword NOTIFY userpasswordChanged)//用户密码
    Q_PROPERTY(QList<bool> IOsignal READ IOsignal WRITE setIOsignal NOTIFY IOsignalChanged)//IO显示
    Q_PROPERTY(QList<bool> Servo_on READ Servo_on WRITE setServo_on NOTIFY Servo_onChanged)
    Q_PROPERTY(QList<int> Mechanical_parameters READ Mechanical_parameters WRITE setMechanical_parameters NOTIFY Mechanical_parametersChanged)//机械参数
    Q_PROPERTY(bool Jog READ Jog WRITE setJog NOTIFY JogChanged)//原点复归中
    Q_PROPERTY(QList<double> Axis_pos READ Axis_pos WRITE setAxis_pos NOTIFY Axis_posChanged)//轴位置
    Q_PROPERTY(QList<double> Point READ Point WRITE setPoint NOTIFY PointChanged)//点位置
    Q_PROPERTY(QList<bool> Point_s READ Point_s WRITE setPoint_s NOTIFY Point_sChanged)//点选择
    Q_PROPERTY(QList<int> Point_speed READ Point_speed WRITE setPoint_speed NOTIFY Point_speedChanged)//点速度
    Q_PROPERTY(QList<int> Pos_index READ Pos_index WRITE setPos_index NOTIFY Pos_indexChanged)
    Q_PROPERTY(int Alm_type READ Alm_type WRITE setAlm_type NOTIFY Alm_typeChanged)//
    Q_PROPERTY(QList<bool> Maintain_save READ Maintain_save WRITE setMaintain_save NOTIFY Maintain_saveChanged)//维护完成
    Q_PROPERTY(bool SW READ SW WRITE setSW NOTIFY SWChanged)//使能
    // Dummy, will be replaced until...
    Q_PROPERTY(QList<bool> IOreverse READ IOreverse WRITE setIOreverse NOTIFY IOreverseChanged)//IO反转
    Q_PROPERTY(QList<bool> SystemMode READ SystemMode WRITE setSystemMode NOTIFY SystemModeChanged)//系统模式

public:
    explicit ResourceManager(const QString &dataPath, QObject *parent = nullptr);
    ~ResourceManager() override;

    void initialize();
    void terminate();

    CsvReader *getCsvReaderInstance();

    Q_INVOKABLE QString label(int group, int id) const;
    Q_INVOKABLE QString label(int group, int id, int language) const;

    Q_INVOKABLE QString rgb(int group, int id) const;
    Q_INVOKABLE QString dataColor(int group, int id);
    QList<QList<quint16>> formatList(int group, int startId, int count, bool isZeroValueSkip = true);
    QList<QList<int>> getIntValueList(int group, int startId, int count, bool isZeroValueSkip = true);
    QList<QList<int>> getIntHexValueList(int group, int startId, int count, bool isZeroValueSkip = true);
    QString unitFormat(int unit) const;

    Q_INVOKABLE QString getSystemSettingAppDirPathNonRealTarget();
    Q_INVOKABLE void changeApplication(QString nextAppPath, QString args) const;
    Q_INVOKABLE void changeApplication(QString nextAppPath, QString args, QString afterNextAppPath, QString afterNextArgs) const;
    Q_INVOKABLE bool isLinux() const;

    // Have image directry thay this application is actually use.
    inline QString imagePath() { return m_imagePath; }

    // The list that handles QML file and Icon directory for each functions
    inline const QJsonObject functionList() const { return m_reader.functionList(); }
    Q_INVOKABLE QString getTopIconFilename(QString qmlFile);

    QString getUnitFromFormat(int unit);

    // Treats '生産予定数' in left pane of Auto running mode.
    Q_INVOKABLE int maxProduction();

    // Treats '1回取出数' in left pane of Auto running mode.
    Q_INVOKABLE int maxOnce();

    // Play beep
    Q_INVOKABLE void beep() {}

    QList<QObject *> manualJogs() { return m_manualJogs; }
    Q_INVOKABLE void updateManualJogs(int axisSelectNo);

    QList<QObject *> manualKeys() { return m_manualKeys; }

    //QList<QObject *> productionHistory(int left, int right);

    int language() const { return m_language; }

    QList<int> menu() const{
        if (m_menu.length() == 0)
            return m_reader.menu();
        else
            return m_menu;
    }

    int mmPage() const { return m_mmPage; }

    QList<int> axisorder(){return m_AxisOrder;}

    QString applicationName() const { return m_applicationName; }

    QString penMsgCsvVersion(){ return m_reader.getPenMsgCsvVersion(); }
    QString penSetCsvVersion(){ return m_reader.getPenSetCsvVersion(); }
    QString modbusAddressCsvVersion(){ return m_reader.getMODBUSAddressCsvVersion(); }

    Q_INVOKABLE QString getPenSetStringValue(const QString &group, int row, int column) { return m_reader.getPenSetStringValue(group, row, column); }
    double getPenSetDoubleValue(const QString &group, int row, int column) { return m_reader.getPenSetDoubleValue(group, row, column); }
    QVector<int> getIntRowArray(const QString &group, int row) { return m_reader.getIntRowArray(group, row); }

    Q_INVOKABLE QString symbolAt(int index) { return m_reader.symbolAt(index); }
    Q_INVOKABLE int indexAt(const QString &symbol) { return m_reader.indexAt(symbol); }
    Q_INVOKABLE int getPenSetIntValue(const QString &group, int row, int column) { return m_reader.getPenSetIntValue(group, row, column); }
    Q_INVOKABLE QString getMessageStringValue(const QString &group, int row, int language) { return m_reader.getMessageStringValue(group, row, language); }

    Q_INVOKABLE QString formatData(int data, int format);
    Q_INVOKABLE QString formatData(int data, int format, int shift, bool isDefaultDisp = false);
    Q_INVOKABLE QString formatData(short data, int format, int shift);
    Q_INVOKABLE bool formatIsDecimals(int format);

    Q_INVOKABLE void reloadCsvReader();

    // Dummy, will be replaced from...
    int screenId() const{
        return m_screenId;
    }
    QString titleString() const{
        return m_titleString;
    }
    QString stateName() const{
        return m_stateName;
    }
    int manipulateMode() const{
        return m_manipulateMode;
    }
    int menu_id() const{
        return m_menu_id;
    }
    int axis_select() const{
        return m_axis_select;
    }
    int ioexpress_switch() const{
        return m_ioexpress_switch;
    }
    int Menu_select() const{
        return m_Menu_select;
    }

    QList<QString> userpassword() const{
        return m_userpassword;
    }
    QList<bool> IOsignal() const{
        return m_IOsignal;
    }
    QList<bool> Servo_on() const{
        return m_Servo_on;
    }
    QList<int> Mechanical_parameters() const{
        return m_Mechanical_parameters;
    }
    bool Jog() const{
        return m_Jog;
    }
    QList<double> Axis_pos() const{
        return m_Axis_pos;
    }
    QList<double> Point() const{
        return m_Point;
    }
    QList<bool> Point_s() const{
        return m_Point_s;
    }
    QList<int> Point_speed() const{
        return m_Point_speed;
    }
    QList<int> Pos_index() const{
        return m_Pos_index;
    }
    int Alm_type() const{
        return m_Alm_type;
    }
    QList<bool> Maintain_save() const{
        return m_Maintain_save;
    }
    bool SW() const{
        return m_SW;
    }



    Q_INVOKABLE inline ushort pageChange(ushort page) {
        ushort oldPage = m_requestedPage;
        m_requestedPage = page;
        return oldPage;
    }
    // Dummy, will be replaced until...
    QList<bool> IOreverse() const{
        return m_IOreverse;
    }

    QList<bool> SystemMode() const{
        return m_SystemMode;
    }


signals:
    void languageChanged(int language);
    void menuChanged(const QList<int> &menu);
    void mmPageChanged(int mmPage);
    void applicationNameChanged(QString applicationName);

    // Dummy, will be replaced from...
    void screenIdChanged(int screenId);
    void titleStringChanged(QString titleString);
    void stateNameChanged(QString stateName);
    void manipulateModeChanged(int manipulateMode);
    void menu_idChanged(int menu_id);
    void axis_selectChanged(int axis_select);
    void ioexpress_switchChanged(int ioexpress_switch);
    void Menu_selectChanged(int Menu_select);
    void userpasswordChanged(QList<QString> userpassword);
    void IOsignalChanged(QList<bool> IOsignal);
    void Servo_onChanged(QList<bool> Servo_on);
    void Mechanical_parametersChanged(QList<int> Mechanical_parameters);
    void JogChanged(bool Jog);
    void Axis_posChanged(QList<double> Axis_pos);
    void PointChanged(QList<double> Point);
    void Point_sChanged(QList<bool> Point_s);
    void Point_speedChanged(QList<int> Point_speed);
    void Pos_indexChanged(QList<int> Pos_index);
    void Alm_typeChanged(int Alm_type);
    void Maintain_saveChanged(QList<bool> Maintain_save);
    void SWChanged(bool SW);

    // Dummy, will be replaced until...
    void IOreverseChanged(QList<bool> IOreverse);
    void SystemModeChanged(QList<bool> SystemMode);


public slots:
    void setLanguage(int language);
    void setMmPage(int mmPage);
    void setApplicationName(QString applicationName);

    void setScreenId(int screenId){
        if (m_screenId == screenId)
            return;

        m_screenId = screenId;
        emit screenIdChanged(m_screenId);
    }
    void setTitleString(QString titleString){
        if (m_titleString == titleString)
            return;

        m_titleString = titleString;
        emit titleStringChanged(m_titleString);
    }
    void setStateName(QString stateName){
        if (m_stateName == stateName)
            return;

        m_stateName = stateName;
        emit stateNameChanged(m_stateName);
    }
    void setManipulateMode(int manipulateMode){
        if (m_manipulateMode == manipulateMode)
            return;

        m_manipulateMode = manipulateMode;
        emit manipulateModeChanged(m_manipulateMode);
    }
    void setmenu_id(int menu_id){
        if (m_menu_id == menu_id)
            return;

        m_menu_id = menu_id;
        emit menu_idChanged(m_menu_id);
    }
    void setaxis_select(int axis_select){
        if (m_axis_select == axis_select)
            return;

        m_axis_select = axis_select;
        emit axis_selectChanged(m_axis_select);
    }
    void setioexpress_switch(int ioexpress_switch){
        if (m_ioexpress_switch == ioexpress_switch)
            return;

        m_ioexpress_switch = ioexpress_switch;
        emit ioexpress_switchChanged(m_ioexpress_switch);
    }
    void setMenu_select(int Menu_select){
        if (m_Menu_select == Menu_select)
            return;

        m_Menu_select = Menu_select;
        emit Menu_selectChanged(m_Menu_select);
    }
    void setuserpassword(QList<QString> userpassword){
        if (m_userpassword == userpassword)
            return;

        m_userpassword = userpassword;
        emit userpasswordChanged(m_userpassword);
    }
    void setIOsignal(QList<bool> IOsignal){
        if (m_IOsignal == IOsignal)
            return;

        m_IOsignal = IOsignal;
        emit IOsignalChanged(m_IOsignal);
    }
    void setServo_on(QList<bool> Servo_on){
        if (m_Servo_on == Servo_on)
            return;

        m_Servo_on = Servo_on;
        emit Servo_onChanged(m_Servo_on);
    }
    void setMechanical_parameters(QList<int> Mechanical_parameters){
        if (m_Mechanical_parameters == Mechanical_parameters)
            return;

        m_Mechanical_parameters = Mechanical_parameters;
        emit Mechanical_parametersChanged(m_Mechanical_parameters);
    }
    void setJog(bool Jog){
        if (m_Jog == Jog)
            return;

        m_Jog = Jog;
        emit JogChanged(m_Jog);
    }
    void setAxis_pos(QList<double> Axis_pos){
        if (m_Axis_pos == Axis_pos)
            return;

        m_Axis_pos = Axis_pos;
        emit Axis_posChanged(m_Axis_pos);
    }
    void setPoint(QList<double> Point){
        if (m_Point == Point)
            return;

        m_Point = Point;
        emit PointChanged(m_Point);
    }
    void setPoint_s(QList<bool> Point_s){
        if (m_Point_s == Point_s)
            return;

        m_Point_s = Point_s;
        emit Point_sChanged(m_Point_s);
    }
    void setPoint_speed(QList<int> Point_speed){
        if (m_Point_speed == Point_speed)
            return;

        m_Point_speed = Point_speed;
        emit Point_speedChanged(m_Point_speed);
    }
    void setPos_index(QList<int> Pos_index){
        if (m_Pos_index == Pos_index)
            return;

        m_Pos_index = Pos_index;
        emit Pos_indexChanged(m_Pos_index);
    }
    void setAlm_type(int Alm_type){
        if (m_Alm_type == Alm_type)
            return;

        m_Alm_type = Alm_type;
        emit Alm_typeChanged(m_Alm_type);
    }
    void setMaintain_save(QList<bool> Maintain_save){
        if (m_Maintain_save == Maintain_save)
            return;

        m_Maintain_save = Maintain_save;
        emit Maintain_saveChanged(m_Maintain_save);
    }
    void setSW(bool SW){
        if (m_SW == SW)
            return;

        m_SW = SW;
        emit SWChanged(m_SW);
    }


    // Dummy, will be replaced until...
    void setIOreverse(QList<bool> IOreverse){
        if (m_IOreverse == IOreverse)
            return;

        m_IOreverse = IOreverse;
        emit IOreverseChanged(m_IOreverse);
    }

    void setSystemMode(QList<bool> SystemMode){
        if (m_SystemMode == SystemMode)
            return;

        m_SystemMode = SystemMode;
        emit SystemModeChanged(m_SystemMode);
    }

private:
    // Parse the data from shared memory and conver to the data that can be understand by QML
    void prepareManualKeys();
    void updateMenuList(QList<int> receiveData);
    void initManualJogs();
    void initAxisOrder();
    QString doFormatData(int data, DataFormat dataFormat, int shift, bool isDefaultDisp = false);

    CsvReader m_reader;

    // Property itself
    const QString                   m_imagePath;
    QList<QObject *>                m_manualJogs;
    QList<QObject *>                m_manualKeys;
    int                             m_language = 3; // default to Japanese -> 25.06.17 default to Korea
    mutable QHash<quint32, QString> m_labelsCache; // for the current language
    mutable QHash<quint32, QString> m_rgbCache;
    mutable QHash<quint32, QString> m_dataFieldColorCache;
    QList<int>                      m_menu;
    int                             m_mmPage = 0;
    int                             m_manualSelectAxis;
    QString                         m_applicationName = "";

    // Dummy, will be replaced from...
    int                 m_screenId = 0;
    QString             m_titleString = label(20, 0);
    QString             m_stateName = label(20, 16);
    QString             m_comment = "";
    int                 m_stepWholeSpeed = 1;
    int                 m_manipulateMode = 0; // Default mode is original
    bool                m_manipulateModeDirty = false;
    ushort              m_requestedPage = 0;
    int                 m_menu_id = 1000;
    int                 m_axis_select = 5;
    int                 m_ioexpress_switch = 0;
    int                 m_Menu_select = 0;
    QList<QString>      m_userpassword;
    QList<bool>         m_IOsignal = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    QList<bool>         m_Servo_on={0,0,0,0,0};
    QList<int>          m_Mechanical_parameters = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    bool                m_Jog = false;
    QList<double>       m_Axis_pos={999.99,888.88,777.77,666.66,555.55};
    QList<double>       m_Point;
    QList<bool>         m_Point_s;
    QList<int>          m_Point_speed;
    QList<int>          m_Pos_index;
    int                 m_Alm_type = 0;
    QList<bool>         m_Maintain_save;
    bool                m_SW = false;
    // Dummy, will be replaced until...
    QList<bool>         m_IOreverse={0,0,0,0,0,0};
    QList<bool>         m_SystemMode = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    const int           mDefaultValuePtn1 = 0x80000000;
    const int           mDefaultValuePtn2 = 0x81000000;
    const QString       mDefaultValueStr = "****.**";
    QList<int>          m_AxisOrder;
};

#endif // RESOURCEMANAGER_H
