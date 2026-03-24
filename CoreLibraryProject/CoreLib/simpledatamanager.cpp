#include "simpledatamanager.h"

SimpleDataManager::SimpleDataManager(QObject *parent) : QObject(parent)
{
    //-- with BIT Type Memory --------------------------------------------
    m_ioCategoryLengthAmount = 0;
    //m_ioCategoryMaxLength = 0;
    for (int idx=0; idx<m_ioCategoryLength.length(); idx++) {
        m_ioCategoryOffset.append(m_ioCategoryLengthAmount);
        m_ioCategoryLengthAmount += m_ioCategoryLength.at(idx);
        //m_ioCategoryMaxLength = qMax(m_ioCategoryMaxLength, m_ioCategoryLength.at(idx));
    }

    m_ioListAll.reserve(m_ioCategoryLengthAmount);
    for(uint idx = 0; idx<m_ioCategoryLengthAmount; idx++) {
        m_ioListAll.append(new PacketData());
    }

    //-- with BYTE Type Memory --------------------------------------------
    makeByteTypeCategoryGroup();

    m_byteTypeListAll.reserve(m_byteTypeAmountItems);
    for(uint idx = 0; idx<m_byteTypeAmountItems; idx++) {
        m_byteTypeListAll.append(new PacketData());
    }

    //-- with WORD Type Memory --------------------------------------------
    makeWordTypeCategoryGroup();

    m_wordTypeListAll.reserve(m_wordTypeAmountItems);
    for(uint idx = 0; idx<m_wordTypeAmountItems; idx++) {
        m_wordTypeListAll.append(new PacketData());
    }


    //-- with LONG Type Memory --------------------------------------------
    makeLongTypeCategoryGroup();

    m_longTypeListAll.reserve(m_longTypeAmountItems);
    for(uint idx = 0; idx<m_longTypeAmountItems; idx++) {
        m_longTypeListAll.append(new PacketData());
    }


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
}

//SimpleDataManager::~SimpleDataManager()
//{
//    qDeleteAll(m_ioList);
//}

void SimpleDataManager::InitDummyParam()
{
    PacketData *p;
    for(int idx = 0; idx<m_ioListAll.length(); idx++) {
        p = reinterpret_cast<PacketData *>(m_ioListAll.at(idx));
        p->setValue((0 == (idx % 5)) ? "1" : "0");
        p->setLabel(tr("BIT項目名") + QString("%1").arg(idx, 4, 16,QChar('0')).toUpper());
    }

    for(int idx = 0; idx<m_byteTypeListAll.length(); idx++) {
        p = reinterpret_cast<PacketData *>(m_byteTypeListAll.at(idx));
        p->setValue(QString::number(idx % 256));
        p->setLabel(tr("BYTE項目名") + QString("%1").arg(idx, 4, 16,QChar('0')).toUpper());
    }

    for(int idx = 0; idx<m_wordTypeListAll.length(); idx++) {
        p = reinterpret_cast<PacketData *>(m_wordTypeListAll.at(idx));
        p->setValue(QString::number(idx % (256*256)));
        p->setLabel(tr("WORD項目名") + QString("%1").arg(idx, 4, 16,QChar('0')).toUpper());
    }

    for(int idx = 0; idx<m_longTypeListAll.length(); idx++) {
        p = reinterpret_cast<PacketData *>(m_longTypeListAll.at(idx));
        p->setValue(QString::number(idx % (256*256)));
        p->setLabel(tr("WORD項目名") + QString("%1").arg(idx, 4, 16,QChar('0')).toUpper());
    }
}

void SimpleDataManager::saveIoList()
{
    if (m_ioCategoryId < 0) return;
    uint idxDst = m_ioCategoryOffset.at(m_ioCategoryId);
    for (uint idx=0; idx<m_ioCategoryLength.at(m_ioCategoryId); idx++, idxDst++) {
        m_ioListAll[idxDst] = m_ioListDisp.at(idx);
    }
}

void SimpleDataManager::setIoList()
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


void SimpleDataManager::makeByteTypeCategoryGroup()
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
    m_byteTypeAmountItems = 0;
    uint byteTypeCategoryGroupLength = 0;
    for (idxCateg = 0; idxCateg < m_byteTypeCategoryCapt.length(); idxCateg++ ) {
        m_byteTypeCategoryOffset.append(iOffset);
        iOffset += m_byteTypeCategoryLength.at(idxCateg);

        strGroupCapt += (m_byteTypeCategoryCapt[idxCateg] + "／");
        byteTypeCategoryGroupLength += (m_byteTypeCategoryLength.at(idxCateg)                   // number of byte type memory
                                      + m_byteTypeCategoryLength.at(idxCateg) / 8               // number of address label
                                      + ((m_byteTypeCategoryLength.at(idxCateg) % 8) ? 1 : 0)   // number of address label
                                      + 1);                                                     // for category caption
        m_byteTypeAmountItems += m_byteTypeCategoryLength.at(idxCateg);
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

void SimpleDataManager::saveByteTypeList()
{
    if (m_byteTypeCategoryGroupId < 0) { return; }
    int idxCate; idxCate = m_byteTypeCategoryHeadIdxOfGroup[m_byteTypeCategoryGroupId];
    uint idxDstPacket;  idxDstPacket = m_byteTypeCategoryOffset.at(idxCate);
    int idxSrcPacket = 0;
    PacketData *pSrcPacket;
    //for (uint idx=0; idx<m_byteTypeCategoryLength.at(m_ioCategoryId); idx++, idxDst++) {
    while (idxSrcPacket < m_byteTypeListDisp.length()) {
        pSrcPacket = reinterpret_cast<PacketData *>(m_byteTypeListDisp.at(idxSrcPacket));
        if ("_ADR" != pSrcPacket->label() && "H" != pSrcPacket->value()) {
            m_byteTypeListAll[idxDstPacket] = m_byteTypeListDisp.at(idxSrcPacket);
            idxDstPacket ++;
        }
        idxSrcPacket ++;
    }
}

void SimpleDataManager::setByteTypeList()
{
    if (-1 == m_byteTypeCategoryGroupId) { return; }
    if (0 < m_byteTypeListDisp.length()) { m_byteTypeListDisp.clear(); }
    int iNewLen = m_byteTypeCategoryGroupLength.at(m_byteTypeCategoryGroupId);
    m_byteTypeListDisp.reserve(iNewLen);
    uint idxPacket = 0;
    while (idxPacket < m_byteTypeCategoryGroupLength.at(m_byteTypeCategoryGroupId)) {
        m_byteTypeListDisp.append(new PacketData());
        idxPacket ++;
    }

    idxPacket = 0;
    PacketData *pPkt;
    int idxCate = m_byteTypeCategoryHeadIdxOfGroup[m_byteTypeCategoryGroupId];
    uint idxSrcPacket;  idxSrcPacket = m_byteTypeCategoryOffset.at(idxCate);
    //PacketData *pSrcPkt;
    while (m_byteTypeCategoryGroupId == m_byteTypeCategoryGroup[idxCate]) {
        //--- set row with category caption
        pPkt = reinterpret_cast<PacketData *>(m_byteTypeListDisp.at(idxPacket));
        pPkt->setLabel(m_byteTypeCategoryCapt.at(idxCate));
        pPkt->setValue(tr("H"));
        idxPacket ++;
        uint idxItem = 0;
        while (idxItem < m_byteTypeCategoryLength.at(idxCate)) {
            //if (0 == (idxItem % 4)) {     // ERROR になる
            if (0 == (idxItem % 8)) {
                //--- set column with address
                pPkt = reinterpret_cast<PacketData *>(m_byteTypeListDisp.at(idxPacket));
                pPkt->setLabel(tr("_ADR"));
                pPkt->setValue(QString::number(m_byteTypeCategoryOffsetTop + m_byteTypeCategoryOffset.at(idxCate) + idxItem));
                //pPkt ++;
                idxPacket ++;
            }
            //--- set each byte type memory item
            m_byteTypeListDisp[idxPacket] = m_byteTypeListAll.at(idxSrcPacket);
            idxPacket ++;
            idxSrcPacket ++;
            idxItem ++;
        }
        idxCate ++;
    }
    emit byteTypeListDispChanged(m_byteTypeListDisp);
}

void SimpleDataManager::makeWordTypeCategoryGroup()
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
    m_wordTypeAmountItems = 0;
    uint wordTypeCategoryGroupLength = 0;
    for (idxCateg = 0; idxCateg < m_wordTypeCategoryCapt.length(); idxCateg++ ) {
        m_wordTypeCategoryOffset.append(iOffset);
        iOffset += m_wordTypeCategoryLength.at(idxCateg);

        strGroupCapt += (m_wordTypeCategoryCapt[idxCateg] + "／");
        wordTypeCategoryGroupLength += (m_wordTypeCategoryLength.at(idxCateg)                   // number of byte type memory
                                      + m_wordTypeCategoryLength.at(idxCateg) / 8               // number of address label
                                      + ((m_wordTypeCategoryLength.at(idxCateg) % 8) ? 1 : 0)   // number of address label
                                      + 1);                                                     // for category caption
        m_wordTypeAmountItems += m_wordTypeCategoryLength.at(idxCateg);
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

void SimpleDataManager::saveWordTypeList()
{
    if (m_wordTypeCategoryGroupId < 0) { return; }
    int idxCate; idxCate = m_wordTypeCategoryHeadIdxOfGroup[m_wordTypeCategoryGroupId];
    uint idxDstPacket;  idxDstPacket = m_wordTypeCategoryOffset.at(idxCate);
    int idxSrcPacket = 0;
    PacketData *pSrcPacket;
    //for (uint idx=0; idx<m_byteTypeCategoryLength.at(m_ioCategoryId); idx++, idxDst++) {
    while (idxSrcPacket < m_wordTypeListDisp.length()) {
        pSrcPacket = reinterpret_cast<PacketData *>(m_wordTypeListDisp.at(idxSrcPacket));
        if ("_ADR" != pSrcPacket->label() && "H" != pSrcPacket->value()) {
            m_wordTypeListAll[idxDstPacket] = m_wordTypeListDisp.at(idxSrcPacket);
            idxDstPacket ++;
        }
        idxSrcPacket ++;
    }
}

void SimpleDataManager::setWordTypeList()
{
    if (-1 == m_wordTypeCategoryGroupId) { return; }
    if (0 < m_wordTypeListDisp.length()) { m_wordTypeListDisp.clear(); }
    int iNewLen = m_wordTypeCategoryGroupLength.at(m_wordTypeCategoryGroupId);
    m_wordTypeListDisp.reserve(iNewLen);
    uint idxPacket = 0;
    while (idxPacket < m_wordTypeCategoryGroupLength.at(m_wordTypeCategoryGroupId)) {
        m_wordTypeListDisp.append(new PacketData());
        idxPacket ++;
    }

    idxPacket = 0;
    PacketData *pPkt;
    int idxCate = m_wordTypeCategoryHeadIdxOfGroup[m_wordTypeCategoryGroupId];
    uint idxSrcPacket;  idxSrcPacket = m_wordTypeCategoryOffset.at(idxCate);
    //PacketData *pSrcPkt;
    while (m_wordTypeCategoryGroupId == m_wordTypeCategoryGroup[idxCate]) {
        //--- set row with category caption
        pPkt = reinterpret_cast<PacketData *>(m_wordTypeListDisp.at(idxPacket));
        pPkt->setLabel(m_wordTypeCategoryCapt.at(idxCate));
        pPkt->setValue(tr("H"));
        idxPacket ++;
        uint idxItem = 0;
        while (idxItem < m_wordTypeCategoryLength.at(idxCate)) {
            //if (0 == (idxItem % 4)) {     // ERROR になる
            if (0 == (idxItem % 8)) {
                //--- set column with address
                pPkt = reinterpret_cast<PacketData *>(m_wordTypeListDisp.at(idxPacket));
                pPkt->setLabel(tr("_ADR"));
                pPkt->setValue(QString::number(m_wordTypeCategoryOffsetTop + m_wordTypeCategoryOffset.at(idxCate) + idxItem));
                //pPkt ++;
                idxPacket ++;
            }
            //--- set each byte type memory item
            m_wordTypeListDisp[idxPacket] = m_wordTypeListAll.at(idxSrcPacket);
            idxPacket ++;
            idxSrcPacket ++;
            idxItem ++;
        }
        idxCate ++;
    }
    emit wordTypeListDispChanged(m_wordTypeListDisp);
}

void SimpleDataManager::makeLongTypeCategoryGroup()
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
    m_longTypeAmountItems = 0;
    uint longTypeCategoryGroupLength = 0;
    for (idxCateg = 0; idxCateg < m_longTypeCategoryCapt.length(); idxCateg++ ) {
        m_longTypeCategoryOffset.append(iOffset);
        iOffset += m_longTypeCategoryLength.at(idxCateg);

        strGroupCapt += (m_longTypeCategoryCapt[idxCateg] + "／");
        longTypeCategoryGroupLength += (m_longTypeCategoryLength.at(idxCateg)                   // number of byte type memory
                                      + m_longTypeCategoryLength.at(idxCateg) / 4              // number of address label
                                      + ((m_longTypeCategoryLength.at(idxCateg) % 8) ? 1 : 0)   // number of address label
                                      + 1);                                                     // for category caption
        m_longTypeAmountItems += m_longTypeCategoryLength.at(idxCateg);
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

void SimpleDataManager::saveLongTypeList()
{
    if (m_longTypeCategoryGroupId < 0) { return; }
    int idxCate; idxCate = m_longTypeCategoryHeadIdxOfGroup[m_longTypeCategoryGroupId];
    uint idxDstPacket;  idxDstPacket = m_longTypeCategoryOffset.at(idxCate);
    int idxSrcPacket = 0;
    PacketData *pSrcPacket;
    //for (uint idx=0; idx<m_byteTypeCategoryLength.at(m_ioCategoryId); idx++, idxDst++) {
    while (idxSrcPacket < m_longTypeListDisp.length()) {
        pSrcPacket = reinterpret_cast<PacketData *>(m_longTypeListDisp.at(idxSrcPacket));
        if ("_ADR" != pSrcPacket->label() && "H" != pSrcPacket->value()) {
            m_longTypeListAll[idxDstPacket] = m_longTypeListDisp.at(idxSrcPacket);
            idxDstPacket ++;
        }
        idxSrcPacket ++;
    }
}

void SimpleDataManager::setLongTypeList()
{
    if (-1 == m_longTypeCategoryGroupId) { return; }
    if (0 < m_longTypeListDisp.length()) { m_longTypeListDisp.clear(); }
    int iNewLen = m_longTypeCategoryGroupLength.at(m_longTypeCategoryGroupId);
    m_longTypeListDisp.reserve(iNewLen);
    uint idxPacket = 0;
    while (idxPacket < m_longTypeCategoryGroupLength.at(m_longTypeCategoryGroupId)) {
        m_longTypeListDisp.append(new PacketData());
        idxPacket ++;
    }

    idxPacket = 0;
    PacketData *pPkt;
    int idxCate = m_longTypeCategoryHeadIdxOfGroup[m_longTypeCategoryGroupId];
    uint idxSrcPacket;  idxSrcPacket = m_longTypeCategoryOffset.at(idxCate);
    //PacketData *pSrcPkt;
    while (m_longTypeCategoryGroupId == m_longTypeCategoryGroup[idxCate]) {
        //--- set row with category caption
        pPkt = reinterpret_cast<PacketData *>(m_longTypeListDisp.at(idxPacket));
        pPkt->setLabel(m_longTypeCategoryCapt.at(idxCate));
        pPkt->setValue(tr("H"));
        idxPacket ++;
        uint idxItem = 0;
        while (idxItem < m_longTypeCategoryLength.at(idxCate)) {
            if (0 == (idxItem % 4)) {     // ERROR になる
            //if (0 == (idxItem % 8)) {
                //--- set column with address
                pPkt = reinterpret_cast<PacketData *>(m_longTypeListDisp.at(idxPacket));
                pPkt->setLabel(tr("_ADR"));
                pPkt->setValue(QString::number(m_longTypeCategoryOffsetTop + m_longTypeCategoryOffset.at(idxCate) + idxItem));
                //pPkt ++;
                idxPacket ++;
            }
            //--- set each byte type memory item
            m_longTypeListDisp[idxPacket] = m_longTypeListAll.at(idxSrcPacket);
            idxPacket ++;
            idxSrcPacket ++;
            idxItem ++;
        }
        idxCate ++;
    }
    emit longTypeListDispChanged(m_longTypeListDisp);
}
