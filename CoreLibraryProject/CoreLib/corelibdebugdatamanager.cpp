#include "corelibdebugdatamanager.h"
#include <QDebug>
#include <QTextCodec>

namespace {
QString readFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
//        qCWarning(corelibdebugdatamanager) << "read error file name : " << filename
//                             << "error info:" << file.errorString();
        return {};
    }
    QByteArray tst = file.read(2);
    file.seek(0);
    bool isUTF16 = (tst == QByteArrayLiteral("\xff\xfe"));
    if (isUTF16) {
        QByteArray ba = file.readAll();
        return QTextCodec::codecForMib(1015)->toUnicode(ba);
    } else {
        return QString::fromUtf8(file.readAll());
    }
}
} // namespace

DebugData::DebugData(QObject *parent) : QObject(parent)
{
    m_bit = -1;
    m_isString = false;
    m_isData = true;
}

DebugData::DebugData(QString label, QString value, ushort address, QObject *parent) : QObject(parent) {
    m_label = label;
    m_value = value;
    m_address = address;
    m_bit = -1;
    m_isString = false;
    m_isData = true;
}

DebugData::DebugData(QString label, QString value, ushort address, short bit, QObject *parent) : QObject(parent) {
    m_label = label;
    m_value = value;
    m_address = address;
    m_bit = bit;
    m_isString = false;
    m_isData = true;
}

DebugDataList::DebugDataList(QObject *parent) : QObject(parent)
{
}

void DebugDataList::addDebugData(DebugData* debugData)
{
    m_debugDataList.append(debugData);
}

CoreLibDebugDataManager::CoreLibDebugDataManager(const QString &dataPath, QObject *parent)
    : QObject(parent),
      mbiInfo{"debug_screen", "127.0.0.1", 502, 1, 450, 1},
      modBusStatus(0),
      m_reader(dataPath + QStringLiteral("/settings")),
      m_settingsPath(dataPath + QStringLiteral("/settings"))
{
    connect(mMemoryDataMgr, &CoreLibMemoryDataManager::updateAllValue, this, &CoreLibDebugDataManager::updateAllValue);

    parseCategoryCapt();
    parseCategoryGroup();
    parseCategoryIsString();
    parseCategoryLength();
    parseReadonly();

    m_ioCategoryCapt = m_category_capt.stringValues("1");
    m_ioCategoryLength = m_category_length.uintValues("1");
    m_ioCategoryReadOnly = m_readonly.boolValues("1");

    m_byteTypeCategoryCapt = m_category_capt.stringValues("2");
    m_byteTypeCategoryGroup = m_category_group.intValues("2");
    m_byteTypeCategoryLength = m_category_length.uintValues("2");
    m_byteTypeCategoryReadOnly = m_readonly.boolValues("2");

    m_wordTypeCategoryCapt = m_category_capt.stringValues("3");
    m_wordTypeCategoryGroup = m_category_group.intValues("3");
    m_wordTypeCategoryLength = m_category_length.uintValues("3");
    m_wordTypeCategoryReadOnly = m_readonly.boolValues("3");

    m_longTypeCategoryCapt = m_category_capt.stringValues("4");
    m_longTypeCategoryGroup = m_category_group.intValues("4");
    m_longTypeCategoryLength = m_category_length.uintValues("4");
    m_longTypeCategoryReadOnly = m_readonly.boolValues("4");

    m_otherTypeCategoryCapt = m_category_capt.stringValues("5");
    m_otherTypeCategoryGroup = m_category_group.intValues("5");
    m_otherTypeCategoryIsString = m_category_is_string.boolValues("5");
    m_otherTypeCategoryLength = m_category_length.uintValues("5");
    m_otherTypeCategoryReadOnly = m_readonly.boolValues("5");

    modBusCom = new CoreLibModBusCommunication(mbiInfo);
    mMutexReadModBus = new QMutex(QMutex::Recursive);

    //-- with BIT Type Memory --------------------------------------------
    uint ioCategoryLengthAmount = 0;
    //m_ioCategoryMaxLength = 0;
    for (int idx=0; idx<m_ioCategoryLength.length(); idx++) {
        m_ioCategoryOffset.append(ioCategoryLengthAmount);
        ioCategoryLengthAmount += m_ioCategoryLength.at(idx);
    }

    //-- with BYTE Type Memory --------------------------------------------
    makeByteTypeCategoryGroup();

    //-- with WORD Type Memory --------------------------------------------
    makeWordTypeCategoryGroup();

    //-- with LONG Type Memory --------------------------------------------
    makeLongTypeCategoryGroup();

    //-- with OTHER Type Memory --------------------------------------------
    makeOtherTypeCategoryGroup();


    //-- with POINT Type Memory --------------------------------------------
    // dummy data.
    DebugData *testDataCategory = new DebugData();
    testDataCategory->setIsData(false);
    testDataCategory->setValue("H");
    testDataCategory->setLabel("DummyCategory");
    DebugData *testDataTransparent = new DebugData();
    testDataTransparent->setIsData(false);
    testDataTransparent->setValue("");
    testDataTransparent->setLabel("");
    testDataTransparent->setIsPatternParam(true);
    DebugData *testData00 = new DebugData();
    testData00->setIsData(false);
    testData00->setValue("10");
    testData00->setLabel("_ADR");
    DebugData *testData01 = new DebugData();
    testData01->setIsData(false);
    testData01->setValue("Test01");
    testData01->setLabel("Label01");
    DebugData *testData02 = new DebugData();
    testData02->setIsData(false);
    testData02->setValue("Test02");
    testData02->setLabel("Label02");
    DebugData *testData03 = new DebugData();
    testData03->setIsData(false);
    testData03->setValue("Test03");
    testData03->setLabel("Label03");
    testData03->setIsPatternParam(true);
    DebugData *testData04 = new DebugData();
    testData04->setIsData(false);
    testData04->setValue("Test04");
    testData04->setLabel("Label04");
    testData04->setIsPatternParam(true);
    DebugData *testData05 = new DebugData();
    testData05->setIsData(false);
    testData05->setValue("Test05");
    testData05->setLabel("Label05");
    testData05->setIsPatternParam(true);
    DebugData *testData06 = new DebugData();
    testData06->setIsData(false);
    testData06->setValue("Test06");
    testData06->setLabel("Label06");
    testData06->setIsPatternParam(true);
    DebugData *testData07 = new DebugData();
    testData07->setIsData(false);
    testData07->setValue("Test07");
    testData07->setLabel("Label07");
    DebugData *testData08 = new DebugData();
    testData08->setIsData(false);
    testData08->setValue("Test08");
    testData08->setLabel("Label08");
    DebugData *testData09 = new DebugData();
    testData09->setIsData(false);
    testData09->setValue("Test09");
    testData09->setLabel("Label09");
    DebugData *testData10 = new DebugData();
    testData10->setIsData(false);
    testData10->setValue("Test10");
    testData10->setLabel("Label10");
    DebugData *testData11 = new DebugData();
    testData11->setIsData(false);
    testData11->setValue("Test11");
    testData11->setLabel("Label11");
    DebugData *testData12 = new DebugData();
    testData12->setIsData(false);
    testData12->setValue("Test12");
    testData12->setLabel("Label12");

    DebugDataList *debugDataDummyListCategory = new DebugDataList();
    debugDataDummyListCategory->addDebugData(testDataCategory);

    DebugDataList *debugDataDummyList01 = new DebugDataList(); // common dummy
    debugDataDummyList01->addDebugData(testData00);
    debugDataDummyList01->addDebugData(testData01);

    DebugDataList *debugDataDummyList02 = new DebugDataList(); // common dummy
    debugDataDummyList02->addDebugData(testData00);
    debugDataDummyList02->addDebugData(testData01);
    debugDataDummyList02->addDebugData(testData02);

    DebugDataList *debugDataDummyList03 = new DebugDataList(); // common dummy
    debugDataDummyList03->addDebugData(testData00);
    debugDataDummyList03->addDebugData(testData01);
    debugDataDummyList03->addDebugData(testData02);
    debugDataDummyList03->addDebugData(testData03);

    DebugDataList *debugDataDummyList04 = new DebugDataList(); // common dummy
    debugDataDummyList04->addDebugData(testData00);
    debugDataDummyList04->addDebugData(testData01);
    debugDataDummyList04->addDebugData(testData02);
    debugDataDummyList04->addDebugData(testData03);
    debugDataDummyList04->addDebugData(testData04);

    DebugDataList *debugDataDummyList05 = new DebugDataList(); // common dummy
    debugDataDummyList05->addDebugData(testData00);
    debugDataDummyList05->addDebugData(testData01);
    debugDataDummyList05->addDebugData(testData02);
    debugDataDummyList05->addDebugData(testData03);
    debugDataDummyList05->addDebugData(testData04);
    debugDataDummyList05->addDebugData(testData05);

    DebugDataList *debugDataDummyList06 = new DebugDataList(); // common dummy
    debugDataDummyList06->addDebugData(testData00);
    debugDataDummyList06->addDebugData(testData01);
    debugDataDummyList06->addDebugData(testData02);
    debugDataDummyList06->addDebugData(testData03);
    debugDataDummyList06->addDebugData(testData04);
    debugDataDummyList06->addDebugData(testData05);
    debugDataDummyList06->addDebugData(testData06);

    DebugDataList *debugDataDummyList07 = new DebugDataList(); // common dummy
    debugDataDummyList07->addDebugData(testData00);
    debugDataDummyList07->addDebugData(testData01);
    debugDataDummyList07->addDebugData(testData02);
    debugDataDummyList07->addDebugData(testData03);
    debugDataDummyList07->addDebugData(testData04);
    debugDataDummyList07->addDebugData(testData05);
    debugDataDummyList07->addDebugData(testData06);
    debugDataDummyList07->addDebugData(testData07);

    DebugDataList *debugDataDummyList08 = new DebugDataList(); // common dummy
    debugDataDummyList08->addDebugData(testData00);
    debugDataDummyList08->addDebugData(testData01);
    debugDataDummyList08->addDebugData(testData02);
    debugDataDummyList08->addDebugData(testData03);
    debugDataDummyList08->addDebugData(testData04);
    debugDataDummyList08->addDebugData(testData05);
    debugDataDummyList08->addDebugData(testData06);
    debugDataDummyList08->addDebugData(testData07);
    debugDataDummyList08->addDebugData(testData08);

    DebugDataList *debugDataDummyList09 = new DebugDataList(); // common dummy
    debugDataDummyList09->addDebugData(testData00);
    debugDataDummyList09->addDebugData(testData01);
    debugDataDummyList09->addDebugData(testData02);
    debugDataDummyList09->addDebugData(testData03);
    debugDataDummyList09->addDebugData(testData04);
    debugDataDummyList09->addDebugData(testData05);
    debugDataDummyList09->addDebugData(testData06);
    debugDataDummyList09->addDebugData(testData07);
    debugDataDummyList09->addDebugData(testData08);
    debugDataDummyList09->addDebugData(testData09);

    DebugDataList *debugDataDummyList10 = new DebugDataList(); // common dummy
    debugDataDummyList10->addDebugData(testData00);
    debugDataDummyList10->addDebugData(testData01);
    debugDataDummyList10->addDebugData(testData02);
    debugDataDummyList10->addDebugData(testData03);
    debugDataDummyList10->addDebugData(testData04);
    debugDataDummyList10->addDebugData(testData05);
    debugDataDummyList10->addDebugData(testData06);
    debugDataDummyList10->addDebugData(testData07);
    debugDataDummyList10->addDebugData(testData08);
    debugDataDummyList10->addDebugData(testData09);
    debugDataDummyList10->addDebugData(testData10);

    DebugDataList *debugDataDummyList11 = new DebugDataList(); // common dummy
    debugDataDummyList11->addDebugData(testData00);
    debugDataDummyList11->addDebugData(testData01);
    debugDataDummyList11->addDebugData(testData02);
    debugDataDummyList11->addDebugData(testData03);
    debugDataDummyList11->addDebugData(testData04);
    debugDataDummyList11->addDebugData(testData05);
    debugDataDummyList11->addDebugData(testData06);
    debugDataDummyList11->addDebugData(testData07);
    debugDataDummyList11->addDebugData(testData08);
    debugDataDummyList11->addDebugData(testData09);
    debugDataDummyList11->addDebugData(testData10);
    debugDataDummyList11->addDebugData(testData11);

    DebugDataList *debugDataDummyList12 = new DebugDataList(); // common dummy
    debugDataDummyList12->addDebugData(testData00);
    debugDataDummyList12->addDebugData(testData01);
    debugDataDummyList12->addDebugData(testData02);
    debugDataDummyList12->addDebugData(testData03);
    debugDataDummyList12->addDebugData(testData04);
    debugDataDummyList12->addDebugData(testData05);
    debugDataDummyList12->addDebugData(testData06);
    debugDataDummyList12->addDebugData(testData07);
    debugDataDummyList12->addDebugData(testData08);
    debugDataDummyList12->addDebugData(testData09);
    debugDataDummyList12->addDebugData(testData10);
    debugDataDummyList12->addDebugData(testData11);
    debugDataDummyList12->addDebugData(testData12);

    DebugDataList *dummyPatternTop = new DebugDataList(); // dummy header 1 top
    dummyPatternTop->addDebugData(testData00);
    dummyPatternTop->addDebugData(testData01);
    dummyPatternTop->addDebugData(testData02);

    DebugDataList *dummyPatternBottom = new DebugDataList(); // dummy header 1 bottom
    dummyPatternBottom->addDebugData(testDataTransparent);
    dummyPatternBottom->addDebugData(testDataTransparent);
    dummyPatternBottom->addDebugData(testData03);
    dummyPatternBottom->addDebugData(testData04);
    dummyPatternBottom->addDebugData(testData05);
    dummyPatternBottom->addDebugData(testData06);

    m_pointHeaderList01.append(debugDataDummyListCategory);
    m_pointHeaderList01.append(debugDataDummyList01);
    m_pointHeaderList01.append(debugDataDummyList01);
    m_pointHeaderList01.append(debugDataDummyList01);
    m_pointHeaderList01.append(debugDataDummyList01);
    m_pointHeaderList01.append(debugDataDummyList01);
    m_pointHeaderList01.append(debugDataDummyList01);
    m_pointHeaderList01.append(dummyPatternTop);
    m_pointHeaderList01.append(dummyPatternBottom);
    m_pointHeaderList01.append(dummyPatternTop);
    m_pointHeaderList01.append(dummyPatternBottom);
    m_pointHeaderList01.append(dummyPatternTop);
    m_pointHeaderList01.append(dummyPatternBottom);
    m_pointHeaderList01.append(dummyPatternTop);
    m_pointHeaderList01.append(dummyPatternBottom);

    m_pointHeaderList02.append(debugDataDummyListCategory);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList07);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList06);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList08);
    m_pointHeaderList02.append(debugDataDummyList08);

    m_pointDataList.append(debugDataDummyListCategory);
    m_pointDataList.append(debugDataDummyList01);
    m_pointDataList.append(debugDataDummyList02);
    m_pointDataList.append(debugDataDummyList03);
    m_pointDataList.append(debugDataDummyList04);
    m_pointDataList.append(debugDataDummyList05);
    m_pointDataList.append(debugDataDummyList06);
    m_pointDataList.append(debugDataDummyList07);
    m_pointDataList.append(debugDataDummyList08);
    m_pointDataList.append(debugDataDummyList07);
    m_pointDataList.append(debugDataDummyList07);
    m_pointDataList.append(debugDataDummyList09);

    // Initalyze dummy parameter
    InitDummyParam();

    m_ioCategoryId = 0;
    setIoList();

    m_byteTypeCategoryGroupId = 0;
    setByteTypeList();

    m_wordTypeCategoryGroupId = 0;
    setWordTypeList();

    m_longTypeCategoryGroupId = 0;
    setLongTypeList();

    m_otherTypeCategoryGroupId = 0;
    setOtherTypeList();
}

void CoreLibDebugDataManager::parseCategoryCapt()
{
    m_category_capt.parse(readFile(m_settingsPath + QStringLiteral("/set_folder/category_capt.csv")), ",");
}

void CoreLibDebugDataManager::parseCategoryGroup()
{
    m_category_group.parse(readFile(m_settingsPath + QStringLiteral("/set_folder/category_group.csv")), ",");
}

void CoreLibDebugDataManager::parseCategoryIsString()
{
    m_category_is_string.parse(readFile(m_settingsPath + QStringLiteral("/set_folder/category_is_string.csv")), ",");
}

void CoreLibDebugDataManager::parseCategoryLength()
{
    m_category_length.parse(readFile(m_settingsPath + QStringLiteral("/set_folder/category_length.csv")), ",");
}

void CoreLibDebugDataManager::parseReadonly()
{
    m_readonly.parse(readFile(m_settingsPath + QStringLiteral("/set_folder/readonly.csv")), ",");
}

void CoreLibDebugDataManager::InitDummyParam()
{
    int otherCnt = 0;

    foreach(CoreLibMemoryDataManager::stModBusAddressTable table, mMemoryDataMgr->addrTableList){
        switch(table.type){
        case CoreLibMemoryDataManager::ModBusMemoryType::bit_type:
            for(int offset = 0; offset < table.size; offset++){
                for(int bit = 0; bit < 8; bit++){
                    DebugData* data = new DebugData();
                    data->setMemData(&mMemoryDataMgr->mModBusData[table.realAddr+offset], 1);
                    data->setLabel(tr("BIT項目名") + QString::number(table.realAddr + offset, 16).rightJustified(4, '0').toUpper() + tr(":") + QString::number(bit));
                    data->setAddress(table.realAddr+offset);
                    data->setBit(bit);
                    connect(data, &DebugData::valueChanged, this, &CoreLibDebugDataManager::setValue);
                    connect(data, &DebugData::boolValueChanged, this, &CoreLibDebugDataManager::boolValueChanged);
                    m_ioListAll.append(data);
                    m_allDataMap.insert(data->address(), data);
                }
            }
            break;
        case CoreLibMemoryDataManager::ModBusMemoryType::byte_type:
            for(int offset = 0; offset < table.size; offset++){
                DebugData* data = new DebugData();
                data->setMemData(&mMemoryDataMgr->mModBusData[table.realAddr+offset], 1);
                data->setLabel(tr("BYTE項目名") + QString::number(table.realAddr + offset, 16).rightJustified(4, '0').toUpper());
                data->setAddress(table.realAddr+offset);
                connect(data, &DebugData::valueChanged, this, &CoreLibDebugDataManager::setValue);
                m_byteTypeListAll.append(data);
                m_allDataMap.insert(data->address(), data);
            }
            break;
        case CoreLibMemoryDataManager::ModBusMemoryType::word_type:
            for(int offset = 0; offset < table.size; offset+=2){
                DebugData* data = new DebugData();
                data->setMemData(&mMemoryDataMgr->mModBusData[table.realAddr+offset], 2);
                data->setLabel(tr("WORD項目名") + QString::number(table.realAddr + offset, 16).rightJustified(4, '0').toUpper());
                data->setAddress(table.realAddr+offset);
                connect(data, &DebugData::valueChanged, this, &CoreLibDebugDataManager::setValue);
                m_wordTypeListAll.append(data);
                m_allDataMap.insert(data->address(), data);
            }
            break;
        case CoreLibMemoryDataManager::ModBusMemoryType::long_type:
            for(int offset = 0; offset < table.size; offset+=4){
                DebugData* data = new DebugData();
                data->setMemData(&mMemoryDataMgr->mModBusData[table.realAddr+offset], 4);
                data->setLabel(tr("LONG項目名") + QString::number(table.realAddr + offset, 16).rightJustified(4, '0').toUpper());
                data->setAddress(table.realAddr+offset);
                connect(data, &DebugData::valueChanged, this, &CoreLibDebugDataManager::setValue);
                m_longTypeListAll.append(data);
                m_allDataMap.insert(data->address(), data);
            }
            break;
        case CoreLibMemoryDataManager::ModBusMemoryType::string_type:
            {
                if(m_otherTypeCategoryIsString[otherCnt]){
                    DebugData* data = new DebugData();
                    data->setMemData(&mMemoryDataMgr->mModBusData[table.realAddr], table.size);
                    data->setLabel(tr("OTHER項目名") + QString::number(table.func04, 16).rightJustified(4, '0').toUpper());
                    data->setAddress(table.realAddr);
                    data->setIsString(true);
                    connect(data, &DebugData::valueChanged, this, &CoreLibDebugDataManager::setValue);
                    m_otherTypeListAll.append(data);
                    m_allDataMap.insert(data->address(), data);
                }
                else{
                    for(int offset = 0; offset < table.size; offset++){
                        DebugData* data = new DebugData();
                        data->setMemData(&mMemoryDataMgr->mModBusData[table.realAddr+offset], 1);
                        data->setLabel(tr("OTHER項目名") + QString::number(table.realAddr + offset, 16).rightJustified(4, '0').toUpper());
                        data->setAddress(table.realAddr+offset);
                        connect(data, &DebugData::valueChanged, this, &CoreLibDebugDataManager::setValue);
                        m_otherTypeListAll.append(data);
                        m_allDataMap.insert(data->address(), data);
                    }
                }

                otherCnt++;
            }
            break;
        case CoreLibMemoryDataManager::ModBusMemoryType::point_type:
            break;
        default:
            break;
        }
    }
}

void CoreLibDebugDataManager::setIoList()
{
    int iNewLen = m_ioCategoryLength.at(m_ioCategoryId);
    if (iNewLen < m_ioListDisp.length()) {
        m_ioListDisp.clear();
    }
    if (iNewLen != m_ioListDisp.length()) {
        m_ioListDisp.reserve(iNewLen);
    }
    int idxSrc = m_ioCategoryOffset.at(m_ioCategoryId);
    int idx = 0;
    for ( ; idx < m_ioListDisp.length(); idx++, idxSrc++) {
        m_ioListDisp[idx] = m_ioListAll.at(idxSrc);
    }
    for ( ; idx < iNewLen; idx++, idxSrc++) {
        m_ioListDisp.append(m_ioListAll.at(idxSrc));
    }
    emit ioListDispChanged(m_ioListDisp);
}


void CoreLibDebugDataManager::makeByteTypeCategoryGroup()
{
    int idxCateg;
    if (0 < m_byteTypeCategoryOffset.length()) { m_byteTypeCategoryOffset.clear(); }
    if (0 < m_byteTypeCategoryGroupLength.length()) { m_byteTypeCategoryGroupLength.clear(); }
    if (0 < m_byteTypeCategoryHeadIdxOfGroup.length()) { m_byteTypeCategoryHeadIdxOfGroup.clear(); }
    int idxGroup = 0;
    //uint iOffset; iOffset = m_byteTypeCategoryOffsetTop;
    uint iOffset; iOffset = 0;
    m_byteTypeCategoryHeadIdxOfGroup.append(idxGroup);
    QString strGroupCapt = "";
    uint byteTypeCategoryGroupLength = 0;
    for (idxCateg = 0; idxCateg < m_byteTypeCategoryCapt.length(); idxCateg++ ) {
        m_byteTypeCategoryOffset.append(iOffset);
        iOffset += m_byteTypeCategoryLength.at(idxCateg);

        strGroupCapt += (m_byteTypeCategoryCapt[idxCateg] + "／");
        byteTypeCategoryGroupLength += (m_byteTypeCategoryLength.at(idxCateg)                   // number of byte type memory
                                      + m_byteTypeCategoryLength.at(idxCateg) / 8               // number of address label
                                      + ((m_byteTypeCategoryLength.at(idxCateg) % 8) ? 1 : 0)   // number of address label
                                      + 1);                                                     // for category caption
        if (idxGroup != m_byteTypeCategoryGroup[idxCateg + 1]) {
            m_byteTypeCategoryGroupCapt.append(strGroupCapt.left(strGroupCapt.length()-1));
            strGroupCapt.clear();
            m_byteTypeCategoryGroupLength.append(byteTypeCategoryGroupLength);
            byteTypeCategoryGroupLength = 0;
            idxGroup = m_byteTypeCategoryGroup[idxCateg + 1];
            if (-1 != idxGroup) {
                m_byteTypeCategoryHeadIdxOfGroup.append(idxCateg + 1);
            }
        }
    }
}

void CoreLibDebugDataManager::setByteTypeList()
{
    if (-1 == m_byteTypeCategoryGroupId) { return; }
    if (0 < m_byteTypeListDisp.length()) { m_byteTypeListDisp.clear(); }
    int iNewLen = m_byteTypeCategoryGroupLength.at(m_byteTypeCategoryGroupId);
    m_byteTypeListDisp.reserve(iNewLen);
    uint idxDebug = 0;
    while (idxDebug < m_byteTypeCategoryGroupLength.at(m_byteTypeCategoryGroupId)) {
        m_byteTypeListDisp.append(new DebugData());
        idxDebug ++;
    }

    idxDebug = 0;
    DebugData *pDbg;
    int idxCate = m_byteTypeCategoryHeadIdxOfGroup[m_byteTypeCategoryGroupId];
    uint idxSrcPacket;  idxSrcPacket = m_byteTypeCategoryOffset.at(idxCate);
    //PacketData *pSrcPkt;
    while (m_byteTypeCategoryGroupId == m_byteTypeCategoryGroup[idxCate]) {
        //--- set row with category caption
        pDbg = reinterpret_cast<DebugData *>(m_byteTypeListDisp.at(idxDebug));
        pDbg->setLabel(m_byteTypeCategoryCapt.at(idxCate));
        pDbg->setIsData(false);
        pDbg->setValue(tr("H"));
        idxDebug ++;
        uint idxItem = 0;
        while (idxItem < m_byteTypeCategoryLength.at(idxCate)) {
            //if (0 == (idxItem % 4)) {     // ERROR になる
            if (0 == (idxItem % 8)) {
                //--- set column with address
                pDbg = reinterpret_cast<DebugData *>(m_byteTypeListDisp.at(idxDebug));
                pDbg->setLabel(tr("_ADR"));
                pDbg->setIsData(false);
                pDbg->setValue(QString::number(m_byteTypeCategoryOffsetTop + m_byteTypeCategoryOffset.at(idxCate) + idxItem));
                //pPkt ++;
                idxDebug ++;
            }
            //--- set each byte type memory item
            m_byteTypeListDisp[idxDebug] = m_byteTypeListAll.at(idxSrcPacket);
            idxDebug ++;
            idxSrcPacket ++;
            idxItem ++;
        }
        idxCate ++;
    }
    emit byteTypeListDispChanged(m_byteTypeListDisp);
}

void CoreLibDebugDataManager::makeWordTypeCategoryGroup()
{
    int idxCateg;
    if (0 < m_wordTypeCategoryOffset.length()) { m_wordTypeCategoryOffset.clear(); }
    if (0 < m_wordTypeCategoryGroupLength.length()) { m_wordTypeCategoryGroupLength.clear(); }
    if (0 < m_wordTypeCategoryHeadIdxOfGroup.length()) { m_wordTypeCategoryHeadIdxOfGroup.clear(); }
    int idxGroup = 0;
    //uint iOffset; iOffset = m_byteTypeCategoryOffsetTop;
    uint iOffset; iOffset = 0;
    m_wordTypeCategoryHeadIdxOfGroup.append(idxGroup);
    QString strGroupCapt = "";
    uint wordTypeCategoryGroupLength = 0;
    for (idxCateg = 0; idxCateg < m_wordTypeCategoryCapt.length(); idxCateg++ ) {
        m_wordTypeCategoryOffset.append(iOffset);
        iOffset += m_wordTypeCategoryLength.at(idxCateg);

        strGroupCapt += (m_wordTypeCategoryCapt[idxCateg] + "／");
        wordTypeCategoryGroupLength += (m_wordTypeCategoryLength.at(idxCateg)                   // number of byte type memory
                                      + m_wordTypeCategoryLength.at(idxCateg) / 8               // number of address label
                                      + ((m_wordTypeCategoryLength.at(idxCateg) % 8) ? 1 : 0)   // number of address label
                                      + 1);                                                     // for category caption
        if (idxGroup != m_wordTypeCategoryGroup[idxCateg + 1]) {
            m_wordTypeCategoryGroupCapt.append(strGroupCapt.left(strGroupCapt.length()-1));
            strGroupCapt.clear();
            m_wordTypeCategoryGroupLength.append(wordTypeCategoryGroupLength);
            wordTypeCategoryGroupLength = 0;
            idxGroup = m_wordTypeCategoryGroup[idxCateg + 1];
            if (-1 != idxGroup) {
                m_wordTypeCategoryHeadIdxOfGroup.append(idxCateg + 1);
            }
        }
    }
}

void CoreLibDebugDataManager::setWordTypeList()
{
    if (-1 == m_wordTypeCategoryGroupId) { return; }
    if (0 < m_wordTypeListDisp.length()) { m_wordTypeListDisp.clear(); }
    int iNewLen = m_wordTypeCategoryGroupLength.at(m_wordTypeCategoryGroupId);
    m_wordTypeListDisp.reserve(iNewLen);
    uint idxDebug = 0;
    while (idxDebug < m_wordTypeCategoryGroupLength.at(m_wordTypeCategoryGroupId)) {
        m_wordTypeListDisp.append(new DebugData());
        idxDebug ++;
    }

    idxDebug = 0;
    DebugData *pDbg;
    int idxCate = m_wordTypeCategoryHeadIdxOfGroup[m_wordTypeCategoryGroupId];
    uint idxSrcPacket;  idxSrcPacket = m_wordTypeCategoryOffset.at(idxCate);
    //PacketData *pSrcPkt;
    while (m_wordTypeCategoryGroupId == m_wordTypeCategoryGroup[idxCate]) {
        //--- set row with category caption
        pDbg = reinterpret_cast<DebugData *>(m_wordTypeListDisp.at(idxDebug));
        pDbg->setLabel(m_wordTypeCategoryCapt.at(idxCate));
        pDbg->setIsData(false);
        pDbg->setValue(tr("H"));
        idxDebug ++;
        uint idxItem = 0;
        while (idxItem < m_wordTypeCategoryLength.at(idxCate)) {
            //if (0 == (idxItem % 4)) {     // ERROR になる
            if (0 == (idxItem % 8)) {
                //--- set column with address
                pDbg = reinterpret_cast<DebugData *>(m_wordTypeListDisp.at(idxDebug));
                pDbg->setLabel(tr("_ADR"));
                pDbg->setIsData(false);
                pDbg->setValue(QString::number(m_wordTypeCategoryOffsetTop + m_wordTypeCategoryOffset.at(idxCate) + idxItem));
                //pPkt ++;
                idxDebug ++;
            }
            //--- set each byte type memory item
            m_wordTypeListDisp[idxDebug] = m_wordTypeListAll.at(idxSrcPacket);
            idxDebug ++;
            idxSrcPacket ++;
            idxItem ++;
        }
        idxCate ++;
    }
    emit wordTypeListDispChanged(m_wordTypeListDisp);
}

void CoreLibDebugDataManager::makeLongTypeCategoryGroup()
{
    int idxCateg;
    if (0 < m_longTypeCategoryOffset.length()) { m_longTypeCategoryOffset.clear(); }
    if (0 < m_longTypeCategoryGroupLength.length()) { m_longTypeCategoryGroupLength.clear(); }
    if (0 < m_longTypeCategoryHeadIdxOfGroup.length()) { m_longTypeCategoryHeadIdxOfGroup.clear(); }
    int idxGroup = 0;
    //uint iOffset; iOffset = m_byteTypeCategoryOffsetTop;
    uint iOffset; iOffset = 0;
    m_longTypeCategoryHeadIdxOfGroup.append(idxGroup);
    QString strGroupCapt = "";
    uint longTypeCategoryGroupLength = 0;
    for (idxCateg = 0; idxCateg < m_longTypeCategoryCapt.length(); idxCateg++ ) {
        m_longTypeCategoryOffset.append(iOffset);
        iOffset += m_longTypeCategoryLength.at(idxCateg);

        strGroupCapt += (m_longTypeCategoryCapt[idxCateg] + "／");
        longTypeCategoryGroupLength += (m_longTypeCategoryLength.at(idxCateg)                   // number of byte type memory
                                      + m_longTypeCategoryLength.at(idxCateg) / 4              // number of address label
                                      + ((m_longTypeCategoryLength.at(idxCateg) % 8) ? 1 : 0)   // number of address label
                                      + 1);                                                     // for category caption
        if (idxGroup != m_longTypeCategoryGroup[idxCateg + 1]) {
            m_longTypeCategoryGroupCapt.append(strGroupCapt.left(strGroupCapt.length()-1));
            strGroupCapt.clear();
            m_longTypeCategoryGroupLength.append(longTypeCategoryGroupLength);
            longTypeCategoryGroupLength = 0;
            idxGroup = m_longTypeCategoryGroup[idxCateg + 1];
            if (-1 != idxGroup) {
                m_longTypeCategoryHeadIdxOfGroup.append(idxCateg + 1);
            }
        }
    }
}

void CoreLibDebugDataManager::setLongTypeList()
{
    if (-1 == m_longTypeCategoryGroupId) { return; }
    if (0 < m_longTypeListDisp.length()) { m_longTypeListDisp.clear(); }
    int iNewLen = m_longTypeCategoryGroupLength.at(m_longTypeCategoryGroupId);
    m_longTypeListDisp.reserve(iNewLen);
    uint idxDebug = 0;
    while (idxDebug < m_longTypeCategoryGroupLength.at(m_longTypeCategoryGroupId)) {
        m_longTypeListDisp.append(new DebugData());
        idxDebug ++;
    }

    idxDebug = 0;
    DebugData *pPkt;
    int idxCate = m_longTypeCategoryHeadIdxOfGroup[m_longTypeCategoryGroupId];
    uint idxSrcPacket;  idxSrcPacket = m_longTypeCategoryOffset.at(idxCate);
    //PacketData *pSrcPkt;
    while (m_longTypeCategoryGroupId == m_longTypeCategoryGroup[idxCate]) {
        //--- set row with category caption
        pPkt = reinterpret_cast<DebugData *>(m_longTypeListDisp.at(idxDebug));
        pPkt->setLabel(m_longTypeCategoryCapt.at(idxCate));
        pPkt->setIsData(false);
        pPkt->setValue(tr("H"));
        idxDebug ++;
        uint idxItem = 0;
        while (idxItem < m_longTypeCategoryLength.at(idxCate)) {
            if (0 == (idxItem % 4)) {     // ERROR になる
            //if (0 == (idxItem % 8)) {
                //--- set column with address
                pPkt = reinterpret_cast<DebugData *>(m_longTypeListDisp.at(idxDebug));
                pPkt->setLabel(tr("_ADR"));
                pPkt->setIsData(false);
                pPkt->setValue(QString::number(m_longTypeCategoryOffsetTop + m_longTypeCategoryOffset.at(idxCate) + idxItem));
                //pPkt ++;
                idxDebug ++;
            }
            //--- set each byte type memory item
            m_longTypeListDisp[idxDebug] = m_longTypeListAll.at(idxSrcPacket);
            idxDebug ++;
            idxSrcPacket ++;
            idxItem ++;
        }
        idxCate ++;
    }
    emit longTypeListDispChanged(m_longTypeListDisp);
}

void CoreLibDebugDataManager::makeOtherTypeCategoryGroup()
{
    int idxCateg;
    if (0 < m_otherTypeCategoryOffset.length()) { m_otherTypeCategoryOffset.clear(); }
    if (0 < m_otherTypeCategoryGroupLength.length()) { m_otherTypeCategoryGroupLength.clear(); }
    if (0 < m_otherTypeCategoryHeadIdxOfGroup.length()) { m_otherTypeCategoryHeadIdxOfGroup.clear(); }
    int idxGroup = 0;
    //uint iOffset; iOffset = m_byteTypeCategoryOffsetTop;
    uint iOffset; iOffset = 0;
    m_otherTypeCategoryHeadIdxOfGroup.append(idxGroup);
    QString strGroupCapt = "";
    uint otherTypeCategoryGroupLength = 0;
    for (idxCateg = 0; idxCateg < m_otherTypeCategoryCapt.length(); idxCateg++ ) {
        m_otherTypeCategoryOffset.append(iOffset);

        strGroupCapt += (m_otherTypeCategoryCapt[idxCateg] + "／");
        if(m_otherTypeCategoryIsString[idxCateg]){
            otherTypeCategoryGroupLength += (1                                                      // number of other type memory
                                          + 1                                                       // number of address label
                                          + 1);                                                     // for category caption
            iOffset += 1;
        }
        else{
            otherTypeCategoryGroupLength += (m_otherTypeCategoryLength.at(idxCateg)                   // number of byte type memory
                                          + m_otherTypeCategoryLength.at(idxCateg) / 8              // number of address label
                                          + ((m_otherTypeCategoryLength.at(idxCateg) % 8) ? 1 : 0)   // number of address label
                                          + 1);                                                     // for category caption
            iOffset += m_otherTypeCategoryLength.at(idxCateg);
        }
        if (idxGroup != m_otherTypeCategoryGroup[idxCateg + 1]) {
            m_otherTypeCategoryGroupCapt.append(strGroupCapt.left(strGroupCapt.length()-1));
            strGroupCapt.clear();
            m_otherTypeCategoryGroupLength.append(otherTypeCategoryGroupLength);
            otherTypeCategoryGroupLength = 0;
            idxGroup = m_otherTypeCategoryGroup[idxCateg + 1];
            if (-1 != idxGroup) {
                m_otherTypeCategoryHeadIdxOfGroup.append(idxCateg + 1);
            }
        }
    }
}

void CoreLibDebugDataManager::setOtherTypeList()
{
    if (-1 == m_otherTypeCategoryGroupId) { return; }
    if (0 < m_otherTypeListDisp.length()) { m_otherTypeListDisp.clear(); }
    int iNewLen = m_otherTypeCategoryGroupLength.at(m_otherTypeCategoryGroupId);
    m_otherTypeListDisp.reserve(iNewLen);
    uint idxDebug = 0;
    while (idxDebug < m_otherTypeCategoryGroupLength.at(m_otherTypeCategoryGroupId)) {
        m_otherTypeListDisp.append(new DebugData());
        idxDebug ++;
    }

    idxDebug = 0;
    DebugData *pPkt;
    int idxCate = m_otherTypeCategoryHeadIdxOfGroup[m_otherTypeCategoryGroupId];
    uint idxSrcPacket;  idxSrcPacket = m_otherTypeCategoryOffset.at(idxCate);
    //PacketData *pSrcPkt;
    while (m_otherTypeCategoryGroupId == m_otherTypeCategoryGroup[idxCate]) {
        //--- set row with category caption
        pPkt = reinterpret_cast<DebugData *>(m_otherTypeListDisp.at(idxDebug));
        pPkt->setLabel(m_otherTypeCategoryCapt.at(idxCate));
        pPkt->setIsData(false);
        pPkt->setValue(tr("H"));
        idxDebug ++;
        uint idxItem = 0;
        while (idxItem < m_otherTypeCategoryLength.at(idxCate)) {
            if (m_otherTypeCategoryIsString[idxCate]){
                if (0 == (idxItem % m_otherTypeCategoryLength.at(idxCate))) {     // ERROR になる
                //if (0 == (idxItem % 8)) {
                    //--- set column with address
                    pPkt = reinterpret_cast<DebugData *>(m_otherTypeListDisp.at(idxDebug));
                    pPkt->setLabel(tr("_ADR"));
                    pPkt->setIsData(false);
                    pPkt->setIsString(m_otherTypeCategoryIsString[idxCate]);
                    pPkt->setValue(QString::number(m_otherTypeCategoryOffsetTop + m_otherTypeCategoryOffset.at(idxCate) + idxItem));
                    //pPkt ++;
                    idxDebug ++;
                }
                idxItem += m_otherTypeCategoryLength.at(idxCate);
            }
            else{
                if (0 == (idxItem % 8)) {     // ERROR になる
                //if (0 == (idxItem % 8)) {
                    //--- set column with address
                    pPkt = reinterpret_cast<DebugData *>(m_otherTypeListDisp.at(idxDebug));
                    pPkt->setLabel(tr("_ADR"));
                    pPkt->setIsData(false);
                    pPkt->setIsString(m_otherTypeCategoryIsString[idxCate]);
                    pPkt->setValue(QString::number(m_otherTypeCategoryOffsetTop + m_otherTypeCategoryOffset.at(idxCate) + idxItem));
                    //pPkt ++;
                    idxDebug ++;
                }
                idxItem ++;
            }
            //--- set each byte type memory item
            m_otherTypeListDisp[idxDebug] = m_otherTypeListAll.at(idxSrcPacket);
            idxDebug ++;
            idxSrcPacket ++;
        }
        idxCate ++;
    }
    emit otherTypeListDispChanged(m_otherTypeListDisp);
}

void CoreLibDebugDataManager::setValue(QString value, ushort address, short bit)
{
    if(bit != -2){
        qDebug() << mMemoryDataMgr->mModBusData[address];
        qDebug() << "value:" << value << " address:" << address << " bit:" << bit;
    }
    emit memoryDataIsUpdated();
}

void CoreLibDebugDataManager::boolValueChanged(bool bValue)
{
    Q_UNUSED(bValue);
    //qDebug() << "value:" << bValue;
    emit memoryDataIsUpdated();
}

QByteArray CoreLibDebugDataManager::getModBusSummaryData(int group)
{
    QByteArray byteAry;

    QJsonArray addrList = m_reader.modBusAddressList(group);

    foreach (QJsonValue row, addrList) {
        QJsonArray cols = row.toArray();
        foreach(QJsonValue col, cols){
            int l_addr = col.toInt();
            CoreLibMemoryDataManager::stModBusAddressTable *table = mMemoryDataMgr->mMapRealAddrToTable[l_addr];
            for(int i = 0; i < table->size; i++){
                byteAry.append(mMemoryDataMgr->mModBusData[table->realAddr+i]);
            }
        }
    }

    return byteAry;
}

void CoreLibDebugDataManager::sendAllValueChange(){
    foreach(QObject * pItem, m_ioListDisp){
        DebugData *pData = reinterpret_cast<DebugData *>(pItem);

        pData->valueChanged(pData->value(), pData->address(), -2);
        pData->boolValueChanged(pData->boolValue());
    }

    foreach(QObject * pItem, m_byteTypeListDisp){
        DebugData *pData = reinterpret_cast<DebugData *>(pItem);

        pData->valueChanged(pData->value(), pData->address(), -2);
    }

    foreach(QObject * pItem, m_wordTypeListDisp){
        DebugData *pData = reinterpret_cast<DebugData *>(pItem);

        pData->valueChanged(pData->value(), pData->address(), -2);
    }

    foreach(QObject * pItem, m_longTypeListDisp){
        DebugData *pData = reinterpret_cast<DebugData *>(pItem);

        pData->valueChanged(pData->value(), pData->address(), -2);
    }

    foreach(QObject * pItem, m_otherTypeListDisp){
        DebugData *pData = reinterpret_cast<DebugData *>(pItem);

        pData->valueChanged(pData->value(), pData->address(), -2);
    }
/*
    foreach(QObject * pItem, m_ioListDisp){
        DebugData *pData = reinterpret_cast<DebugData *>(pItem);

        pData->setValue(pData->value());
        pData->valueChanged(pData->value(), pData->address(), -2);
    }
*/
}

void CoreLibDebugDataManager::updateAllValue(){
    sendAllValueChange();
}

CoreLibDebugDataManager* CoreLibDebugDataManager::instance = nullptr;



