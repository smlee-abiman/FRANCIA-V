#ifndef CORELIBDEBUGDATAMANAGER_H
#define CORELIBDEBUGDATAMANAGER_H

#include <QObject>
#include <QString>
#include <QTextCodec>
#include "corelibmodbuscommunication.h"
#include "corelibmemorydatamanager.h"
#include "import_src/libcsvreader.h"

class DebugData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool boolValue READ boolValue NOTIFY boolValueChanged)
    Q_PROPERTY(ushort address READ address WRITE setAddress)
    Q_PROPERTY(bool isString READ isString WRITE setIsString)
    Q_PROPERTY(bool isPatternParam READ isPatternParam WRITE setIsPatternParam NOTIFY isPatternParamChanged)

public:
    explicit DebugData(QObject *parent = nullptr);
    explicit DebugData(QString label, QString value, ushort address, QObject *parent = nullptr);
    explicit DebugData(QString label, QString value, ushort address, short bit, QObject *parent = nullptr);

    ~DebugData() {
    }

    QString label() const
    {
        return m_label;
    }

    QString value() const
    {
        if(m_isData == false){
            return m_value;
        }
        else if(m_bit == -1){

            if(m_isString){
                char c_byte[128];
                memset(c_byte, 0, sizeof(c_byte));
                memcpy(c_byte, p_memData, m_dataSize);
                return QString(c_byte);
            }
            else{
                uchar c_byte[128];
                memset(c_byte, 0, sizeof(c_byte));
                memcpy(c_byte, p_memData, m_dataSize);
                switch(m_dataSize){
                case 1:
                    {
                        uchar c = c_byte[0];
                        int i = c;
                        return QString::number(i);
                    }
                    break;
                case 2:
                    {
                        ushort s = 0;
                        s += (ushort)c_byte[0] << 8;
                        s += (ushort)c_byte[1];
                        int i = s;
                        return QString::number(i);
                    }
                    break;
                case 4:
                    {
                        ulong l = 0;
                        l += (ulong)c_byte[0] << 24;
                        l += (ulong)c_byte[1] << 16;
                        l += (ulong)c_byte[2] << 8;
                        l += (ulong)c_byte[3];
                        int i = l;
                        return QString::number(i);
                    }
                    break;
                default:
                    return QString("-1");
                    break;
                }
            }
        }
        else{
            char c_byte[1];
            memset(c_byte, 0, sizeof(c_byte));
            memcpy(c_byte, p_memData, 1);
            int x = (c_byte[0] >> (7 - m_bit)) & 1;

            return QString::number(x);
        }
    }

    bool boolValue() const
    {
        if(m_isData != false && 0 <= m_bit){
            if(value() == "1"){
                return true;
            }
        }

        return false;
    }

    ushort address() const
    {
        return m_address;
    }

    bool isString() const
    {
        return m_isString;
    }

    bool isPatternParam() const
    {
        return m_isPatternParam;
    }

    QModbusPdu::FunctionCode funcCode() const
    {
        return m_funcCode;
    }

    ushort readAddress() const
    {
        return m_readAddress;
    }

    ushort writeAddress() const
    {
        return m_writeAddress;
    }

    ushort comSize() const
    {
        return m_comSize;
    }

signals:

    void labelChanged(QString label);
    void valueChanged(QString value, ushort address, short bit);
    void boolValueChanged(bool bValue);
    void isPatternParamChanged(bool flag);

public slots:

    void setLabel(QString label)
    {
        if (m_label == label)
            return;

        m_label = label;
    }

    void setValue(QString value)
    {
        if(m_isData == false){
            if (m_value == value)
                return;

            m_value = value;
        }
        else{
            if(m_bit == -1){
                char c_byte[128];
                memset(c_byte, 0, sizeof(c_byte));
                memcpy(c_byte, p_memData, m_dataSize);

                if (QString(c_byte) == value)
                    return;

                if(m_isString){
                    QByteArray qbyte = value.toLocal8Bit();
                    memset(p_memData, 0, m_dataSize);
                    memcpy(p_memData, qbyte.data(), m_dataSize);
                }
                else{
                    switch(m_dataSize){
                    case 1:
                        {
                            uchar c = (char)value.toUInt();
                            p_memData[0] = c;
                        }
                        break;
                    case 2:
                        {
                            ushort s = value.toUShort();
                            p_memData[0] = ((s >> 8) & 0xFF);
                            p_memData[1] = (s & 0xFF);
                        }
                        break;
                    case 4:
                        {
                            ulong l = value.toULong();
                            p_memData[0] = ((l >> 24) & 0xFF);
                            p_memData[1] = ((l >> 16) & 0xFF);
                            p_memData[2] = ((l >> 8) & 0xFF);
                            p_memData[3] = (l & 0xFF);
                        }
                        break;
                    default:
                        break;
                    }
                }

                emit valueChanged(value, m_address, m_bit);
            }
            else{
                char c_byte[1];
                memset(c_byte, 0, sizeof(c_byte));
                memcpy(c_byte, p_memData, 1);
                int bit = (c_byte[0] >> (7 - m_bit)) & 1;
                if(value == "1" && bit == 1){
                    return;
                }
                else if(value != "1" && bit == 0){
                    return;
                }

                c_byte[0] ^= (1 << (7 - m_bit));
                memcpy(p_memData, c_byte, 1);
                QString bValue = QString::number(p_memData[0], 2);

                emit valueChanged(bValue, m_address, m_bit);
                emit boolValueChanged(bValue == "1");
            }
        }
    }

    void setAddress(ushort address)
    {
        if (m_address == address)
            return;

        m_address = address;
    }

    void setBit(short bit)
    {
        if (m_bit == bit)
            return;

        m_bit = bit;
    }

    void setIsString(bool flag)
    {
        if (m_isString == flag)
            return;

        m_isString = flag;
    }

    void setIsPatternParam(bool flag)
    {
        if (m_isPatternParam == flag)
            return;

        m_isPatternParam = flag;
        emit isPatternParamChanged(m_isPatternParam);
    }

    void setIsData(bool flag)
    {
        if (m_isData == flag)
            return;

        m_isData = flag;
    }

    void setMemData(uchar* dataTop, ushort size)
    {
        if (p_memData == dataTop && m_dataSize == size)
            return;

        p_memData = dataTop;
        m_dataSize = size;
    }

private:
    uchar*                      p_memData;
    ushort                      m_dataSize;
    QString                     m_label;
    QString                     m_value;
    ushort                      m_address;                                  // realAddress
    short                       m_bit;                                      // realAddress_bit
    bool                        m_isString;
    bool                        m_isPatternParam = false;
    bool                        m_isData;

    QModbusPdu::FunctionCode    m_funcCode;                                   // function code
    ushort                      m_readAddress;                                // address(read)
    ushort                      m_writeAddress;                               // address(write)
    ushort                      m_comSize;                                    // communication size(byte)
};

Q_DECLARE_METATYPE(DebugData *);

class DebugDataList : public QObject
{
    Q_OBJECT

public:
    explicit DebugDataList(QObject *parent = nullptr);
    void addDebugData(DebugData* debugData);
    QList<QObject *>* getDebugDataListPointer(){ return &m_debugDataList; };

private:
    QList<QObject *> m_debugDataList;

signals:

};

class CoreLibDebugDataManager : public QObject
{
    Q_OBJECT

    //--- BIT Type Memory ------------------------------------------
    Q_PROPERTY(QStringList ioCategoryCapt READ ioCategoryCapt NOTIFY ioCategoryCaptChanged)
    Q_PROPERTY(int ioCategoryId READ ioCategoryId WRITE setIoCategoryId NOTIFY ioCategoryIdChanged)
    Q_PROPERTY(QList<QObject *> ioListDisp READ ioListDisp NOTIFY ioListDispChanged)
    Q_PROPERTY(QList<bool> ioCategoryReadOnly READ ioCategoryReadOnly)
    Q_PROPERTY(QList<int> ioCategoryOffset READ ioCategoryOffset)

    //--- BYTE Type Memory ------------------------------------------
    Q_PROPERTY(QStringList byteTypeCategoryGroupCapt READ byteTypeCategoryGroupCapt NOTIFY byteTypeCategoryGroupCaptChanged)
    Q_PROPERTY(int byteTypeCategoryGroupId READ byteTypeCategoryGroupId WRITE setByteTypeCategoryGroupId NOTIFY byteTypeCategoryGroupIdChanged)
    Q_PROPERTY(QList<QObject *> byteTypeListDisp READ byteTypeListDisp NOTIFY byteTypeListDispChanged)
    //Q_PROPERTY(QList<bool> byteTypeCategoryReadOnly READ byteTypeCategoryReadOnly)          // remove
    //Q_PROPERTY(QList<uint> byteTypeCategoryOffset READ byteTypeCategoryOffset)              // remove


    //--- WORD Type Memory ------------------------------------------
    Q_PROPERTY(QStringList wordTypeCategoryGroupCapt READ wordTypeCategoryGroupCapt NOTIFY wordTypeCategoryGroupCaptChanged)
    Q_PROPERTY(int wordTypeCategoryGroupId READ wordTypeCategoryGroupId WRITE setWordTypeCategoryGroupId NOTIFY wordTypeCategoryGroupIdChanged)
    Q_PROPERTY(QList<QObject *> wordTypeListDisp READ wordTypeListDisp NOTIFY wordTypeListDispChanged)


    //--- LONG Type Memory ------------------------------------------
    Q_PROPERTY(QStringList longTypeCategoryGroupCapt READ longTypeCategoryGroupCapt NOTIFY longTypeCategoryGroupCaptChanged)
    Q_PROPERTY(int longTypeCategoryGroupId READ longTypeCategoryGroupId WRITE setLongTypeCategoryGroupId NOTIFY longTypeCategoryGroupIdChanged)
    Q_PROPERTY(QList<QObject *> longTypeListDisp READ longTypeListDisp NOTIFY longTypeListDispChanged)


    //--- OTHER Type Memory ------------------------------------------
    Q_PROPERTY(QStringList otherTypeCategoryGroupCapt READ otherTypeCategoryGroupCapt NOTIFY otherTypeCategoryGroupCaptChanged)
    Q_PROPERTY(int otherTypeCategoryGroupId READ otherTypeCategoryGroupId WRITE setOtherTypeCategoryGroupId NOTIFY otherTypeCategoryGroupIdChanged)
    Q_PROPERTY(QList<QObject *> otherTypeListDisp READ otherTypeListDisp NOTIFY otherTypeListDispChanged)
    Q_PROPERTY(bool otherTypeIsString READ otherTypeIsString WRITE setOtherTypeIsString)


    //--- POINT Type Memory ------------------------------------------
    Q_PROPERTY(QStringList pointTypeCategoryGroupCapt READ pointTypeCategoryGroupCapt NOTIFY pointTypeCategoryGroupCaptChanged)
    Q_PROPERTY(QList<QObject*> pointHeaderList01 MEMBER m_pointHeaderList01 CONSTANT)
    Q_PROPERTY(QList<QObject*> pointHeaderList02 MEMBER m_pointHeaderList02 CONSTANT)
    Q_PROPERTY(QList<QObject*> pointDataList MEMBER m_pointDataList CONSTANT)

private:
    explicit CoreLibDebugDataManager(const QString &dataPath, QObject *parent = nullptr);
    ~CoreLibDebugDataManager() = default;

    static CoreLibDebugDataManager* instance;
public:
    Q_INVOKABLE QList<QObject*> getRowPointHeaderList01(int index) {
        DebugDataList* debugDataList = (DebugDataList*)m_pointHeaderList01[index];
        return *(debugDataList->getDebugDataListPointer());
    }

    Q_INVOKABLE QList<QObject*> getRowPointHeaderList02(int index) {
        DebugDataList* debugDataList = (DebugDataList*)m_pointHeaderList02[index];
        return *(debugDataList->getDebugDataListPointer());
    }

    Q_INVOKABLE QList<QObject*> getDebugDataList(int index) {
        DebugDataList* debugDataList = (DebugDataList*)m_pointDataList[index];
        return *(debugDataList->getDebugDataListPointer());
    }


    static CoreLibDebugDataManager* create_instance(const QString &dataPath) {
        if(!instance){
            instance = new CoreLibDebugDataManager(dataPath);
        }
        return instance;
    }

    static CoreLibDebugDataManager* get_instance() {
        return instance;
    }

    static void destroy_instance() {
        delete instance;
        instance = nullptr;
    }

    //--- BIT Type Memory ------------------------------------------
    QStringList ioCategoryCapt() const
    {
        return m_ioCategoryCapt;
    }

    int ioCategoryId() const
    {
        return m_ioCategoryId;
    }

    void setIoCategoryId(int ioCategoryId)
    {
        if (m_ioCategoryId == ioCategoryId) {
            return;
        }
        m_ioCategoryId = ioCategoryId;
        setIoList();
    }

    QList<QObject *> ioListDisp() {
        return m_ioListDisp;
    }

    QList<bool> ioCategoryReadOnly() {
        return m_ioCategoryReadOnly;
    }

    QList<int> ioCategoryOffset() {
        return m_ioCategoryOffset;
    }

    //--- BYTE Type Memory ------------------------------------------
/*
    QStringList byteTypeCategoryCapt() const
    {
        return m_byteTypeCategoryCapt;
    }
*/
    QStringList byteTypeCategoryGroupCapt() const
    {
        return m_byteTypeCategoryGroupCapt;
    }

    int byteTypeCategoryGroupId() const
    {
        return m_byteTypeCategoryGroupId;
    }

    void setByteTypeCategoryGroupId(int byteTypeCategoryGroupId)
    {
        if (m_byteTypeCategoryGroupId == byteTypeCategoryGroupId) {
            return;
        }
        m_byteTypeCategoryGroupId = byteTypeCategoryGroupId;
        setByteTypeList();
    }

    QList<QObject *> byteTypeListDisp() {
        return m_byteTypeListDisp;
    }

    QList<bool> byteTypeCategoryReadOnly() {
        return m_byteTypeCategoryReadOnly;
    }

    QList<uint> byteTypeCategoryOffset() {
        return m_byteTypeCategoryOffset;
    }


    //--- WORD Type Memory ------------------------------------------

    QStringList wordTypeCategoryGroupCapt() const
    {
        return m_wordTypeCategoryGroupCapt;
    }

    int wordTypeCategoryGroupId() const
    {
        return m_wordTypeCategoryGroupId;
    }

    void setWordTypeCategoryGroupId(int wordTypeCategoryGroupId)
    {
        if (m_wordTypeCategoryGroupId == wordTypeCategoryGroupId) {
            return;
        }
        m_wordTypeCategoryGroupId = wordTypeCategoryGroupId;
        setWordTypeList();
    }

    QList<QObject *> wordTypeListDisp() {
        return m_wordTypeListDisp;
    }

    QList<bool> wordTypeCategoryReadOnly() {
        return m_wordTypeCategoryReadOnly;
    }

    QList<uint> wordTypeCategoryOffset() {
        return m_wordTypeCategoryOffset;
    }


    //--- LONG Type Memory ------------------------------------------

    QStringList longTypeCategoryGroupCapt() const
    {
        return m_longTypeCategoryGroupCapt;
    }

    int longTypeCategoryGroupId() const
    {
        return m_longTypeCategoryGroupId;
    }

    void setLongTypeCategoryGroupId(int longTypeCategoryGroupId)
    {
        if (m_longTypeCategoryGroupId == longTypeCategoryGroupId) {
            return;
        }
        m_longTypeCategoryGroupId = longTypeCategoryGroupId;
        setLongTypeList();
    }

    QList<QObject *> longTypeListDisp() {
        return m_longTypeListDisp;
    }

    QList<bool> longTypeCategoryReadOnly() {
        return m_longTypeCategoryReadOnly;
    }

    QList<uint> longTypeCategoryOffset() {
        return m_longTypeCategoryOffset;
    }


    //--- OTHER Type Memory ------------------------------------------

    QStringList otherTypeCategoryGroupCapt() const
    {
        return m_otherTypeCategoryGroupCapt;
    }

    int otherTypeCategoryGroupId() const
    {
        return m_otherTypeCategoryGroupId;
    }

    void setOtherTypeCategoryGroupId(int otherTypeCategoryGroupId)
    {
        if (m_otherTypeCategoryGroupId == otherTypeCategoryGroupId) {
            return;
        }
        m_otherTypeCategoryGroupId = otherTypeCategoryGroupId;
        setOtherTypeList();
    }

    QList<QObject *> otherTypeListDisp() {
        return m_otherTypeListDisp;
    }

    QList<bool> otherTypeCategoryReadOnly() {
        return m_otherTypeCategoryReadOnly;
    }

    QList<uint> otherTypeCategoryOffset() {
        return m_otherTypeCategoryOffset;
    }

    bool otherTypeIsString() {
        return m_otherTypeIsString;
    }

    void setOtherTypeIsString(bool flag) {
        m_otherTypeIsString = flag;
    }

    //--- POINT Type Memory ------------------------------------------

    QStringList pointTypeCategoryGroupCapt() const
    {
        return m_pointTypeCategoryGroupCapt;
    }

signals:
    //--- BIT Type Memory ------------------------------------------
    void ioListDispChanged(const QList<QObject *> &m_ioListDisp);
    void ioCategoryCaptChanged(const QList<QObject *> &ioCategoryCapt);
    void ioCategoryIdChanged(const QList<bool> &ioCategoryId);

    //--- BYTE Type Memory ------------------------------------------
    void byteTypeListDispChanged(const QList<QObject *> &m_byteTypeListDisp);
    void byteTypeCategoryGroupCaptChanged(const QList<QObject *> &byteTypeCategoryGroupCapt);
    void byteTypeCategoryGroupIdChanged(const QList<bool> &byteTypeCategoryGroupId);

    //--- WORD Type Memory ------------------------------------------
    void wordTypeListDispChanged(const QList<QObject *> &m_wordTypeListDisp);
    void wordTypeCategoryGroupCaptChanged(const QList<QObject *> &wordTypeCategoryGroupCapt);
    void wordTypeCategoryGroupIdChanged(const QList<bool> &wordTypeCategoryGroupId);

    //--- LONG Type Memory ------------------------------------------
    void longTypeListDispChanged(const QList<QObject *> &m_longTypeListDisp);
    void longTypeCategoryGroupCaptChanged(const QList<QObject *> &longTypeCategoryGroupCapt);
    void longTypeCategoryGroupIdChanged(const QList<bool> &longTypeCategoryGroupId);

    //--- OTHER Type Memory ------------------------------------------
    void otherTypeListDispChanged(const QList<QObject *> &m_otherTypeListDisp);
    void otherTypeCategoryGroupCaptChanged(const QList<QObject *> &otherTypeCategoryGroupCapt);
    void otherTypeCategoryGroupIdChanged(const QList<bool> &otherTypeCategoryGroupId);

    //--- POINT Type Memory ------------------------------------------
    void pointTypeCategoryGroupCaptChanged(const QList<QObject *> &pointTypeCategoryGroupCapt);

    void memoryDataIsUpdated();
private:
    QMap<ushort, DebugData*> m_allDataMap;

    //--- BIT Type Memory ------------------------------------------
    QStringList m_ioCategoryCapt;
    QList<int> m_ioCategoryOffset;
    QList<uint> m_ioCategoryLength;
    //uint m_ioCategoryMaxLength;
    QList<bool> m_ioCategoryReadOnly;

    int m_ioCategoryId = -1;
    QList<QObject *> m_ioListDisp;
    QList<QObject *> m_ioListAll;
    void setIoList();

    //--- BYTE Type Memory ------------------------------------------
    QStringList m_byteTypeCategoryCapt;
    QList<int> m_byteTypeCategoryGroup;
    QStringList m_byteTypeCategoryGroupCapt;

    QList<uint> m_byteTypeCategoryOffset;
    uint m_byteTypeCategoryOffsetTop = 0x02A0;      // <ToDo> Read from CSV file
    QList<uint> m_byteTypeCategoryLength;
    QList<uint> m_byteTypeCategoryGroupLength;
    QList<uint> m_byteTypeCategoryHeadIdxOfGroup;
    //uint m_byteTypeCategoryGroupMaxLength;
    QList<bool> m_byteTypeCategoryReadOnly;

    int m_byteTypeCategoryGroupId = -1;
    QList<QObject *> m_byteTypeListDisp;
    QList<QObject *> m_byteTypeListAll;
    void setByteTypeList();
    void makeByteTypeCategoryGroup();



    //--- WORD Type Memory ------------------------------------------
    QStringList m_wordTypeCategoryCapt;
    QList<int> m_wordTypeCategoryGroup;
    QStringList m_wordTypeCategoryGroupCapt;

    QList<uint> m_wordTypeCategoryOffset;
    uint m_wordTypeCategoryOffsetTop = 0x04A0;      // <ToDo> Read from CSV file
    QList<uint> m_wordTypeCategoryLength;
    QList<uint> m_wordTypeCategoryGroupLength;
    QList<uint> m_wordTypeCategoryHeadIdxOfGroup;
    //uint m_byteTypeCategoryGroupMaxLength;
    QList<bool> m_wordTypeCategoryReadOnly;

    int m_wordTypeCategoryGroupId = -1;
    QList<QObject *> m_wordTypeListDisp;
    QList<QObject *> m_wordTypeListAll;
    void setWordTypeList();
    void makeWordTypeCategoryGroup();


    //--- LONG Type Memory ------------------------------------------
    QStringList m_longTypeCategoryCapt;
    QList<int> m_longTypeCategoryGroup;
    QStringList m_longTypeCategoryGroupCapt;

    QList<uint> m_longTypeCategoryOffset;
    uint m_longTypeCategoryOffsetTop = 0x2C00;      // <ToDo> Read from CSV file
    QList<uint> m_longTypeCategoryLength;
    QList<uint> m_longTypeCategoryGroupLength;
    QList<uint> m_longTypeCategoryHeadIdxOfGroup;
    //uint m_byteTypeCategoryGroupMaxLength;
    QList<bool> m_longTypeCategoryReadOnly;

    int m_longTypeCategoryGroupId = -1;
    QList<QObject *> m_longTypeListDisp;
    QList<QObject *> m_longTypeListAll;

    void setLongTypeList();
    void makeLongTypeCategoryGroup();


    //--- OTHER Type Memory ------------------------------------------
    QStringList m_otherTypeCategoryCapt;
    QList<int> m_otherTypeCategoryGroup;
    QList<bool> m_otherTypeCategoryIsString;
    QStringList m_otherTypeCategoryGroupCapt;

    QList<uint> m_otherTypeCategoryOffset;
    uint m_otherTypeCategoryOffsetTop = 0x6300;      // <ToDo> Read from CSV file
    QList<uint> m_otherTypeCategoryLength;
    QList<uint> m_otherTypeCategoryGroupLength;
    QList<uint> m_otherTypeCategoryHeadIdxOfGroup;
    //uint m_byteTypeCategoryGroupMaxLength;
    QList<bool> m_otherTypeCategoryReadOnly;

    int m_otherTypeCategoryGroupId = -1;
    QList<QObject *> m_otherTypeListDisp;
    QList<QObject *> m_otherTypeListAll;
    bool m_otherTypeIsString;

    void setOtherTypeList();
    void makeOtherTypeCategoryGroup();

    //--- POINT Type Memory ------------------------------------------
    QStringList m_pointTypeCategoryGroupCapt;
    QList<QObject*> m_pointHeaderList01;
    QList<QObject*> m_pointHeaderList02;
    QList<QObject*> m_pointDataList;

    //初期化
    void InitDummyParam();

    void parseCategoryCapt();
    void parseCategoryGroup();
    void parseCategoryIsString();
    void parseCategoryLength();
    void parseReadonly();

    CoreLibModBusDataManager *mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    CoreLibMemoryDataManager *mMemoryDataMgr = CoreLibMemoryDataManager::get_instance();
    stModBusInitInfo mbiInfo;
    stModBusCommunicationData mModBusComData;
    CoreLibModBusCommunication *modBusCom;
    DebugData *mpDebugData;
    int modBusStatus;
    QMutex *mMutexReadModBus;
    LibCSVReader m_reader;

    const QString m_settingsPath;
    LibCsvDocument m_category_capt;
    LibCsvDocument m_category_group;
    LibCsvDocument m_category_is_string;
    LibCsvDocument m_category_length;
    LibCsvDocument m_readonly;

public:
    QByteArray getModBusSummaryData(int group);
    void sendAllValueChange();

public slots:
    void setValue(QString value, ushort address, short bit);
    void boolValueChanged(bool bValue);
    void updateAllValue();
};

#endif // CORELIBDEBUGDATAMANAGER_H
