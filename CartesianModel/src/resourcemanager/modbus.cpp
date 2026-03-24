#include "modbus.h"
#include <QDebug>

//Q_LOGGING_CATEGORY(commandcontrol, "commandcontrol")
//
// parseAddress0000
// Parse read data in shared memory and store to the safe place if necessary.
//  input: check comparison, true - do data compare, false - overwrite forcibly
//  output: true - data is the same as previous
//          false - data is differnt from previous, need to be redraw
//
bool ModBus::parseAddress0000(bool isCompare)
{
    s_read_address_0000 dat;
    std::memset(&dat, 0, sizeof(s_read_address_0000));
    bool cmp = true;
    m_mutexShared->lock();
    {
        cmp = parseHeader(&dat.head);
        if (parseCommon(&dat.common, offsetof(s_read_address_0000, common)) == false)
            cmp = false;
        if (parseBase(&dat.base, offsetof(s_read_address_0000, base)) == false)
            cmp = false;
        int offset = sizeof(s_read_address_header) + sizeof(s_read_address_common)
                + sizeof(s_read_address_base);

        offset = 0xb4;

        std::memcpy(reinterpret_cast<void *>(dat.extend.io), m_readBuffer + offset,
                    sizeof(dat.extend.io));
    }
    m_mutexShared->unlock();

    m_dirty = false;
    m_currentAddress = 0;

    s_read_address_0000 *s = reinterpret_cast<s_read_address_0000 *>(m_interpret);
    if (isCompare == false) {
        cmp = false;
        m_mutexInterpret->lock();
        {
            std::memcpy(s, &dat, sizeof(dat));
        }
        m_mutexInterpret->unlock();
        qCDebug(sharedcontrol) << "Parse Address (Force): 000" << sizeof(dat);
    } else {
        if (std::memcmp(&dat, reinterpret_cast<s_read_address_0000 *>(m_interpret),
                        sizeof(s_read_address_0000))
            != 0)
            cmp = false;
        if (cmp == false) {
            m_mutexInterpret->lock();
            {
                std::memcpy(s, &dat, sizeof(s_read_address_0000));
            }
            m_mutexInterpret->unlock();
            qCDebug(sharedcontrol) << "Parse Address (Update): 000" << sizeof(dat);
        }
    }

    return cmp;
}

bool ModBus::parseAddress0001(bool isCompare)
{
    s_read_address_0001 dat;

    bool cmp = true;
    m_mutexShared->lock();
    {
        cmp = parseHeader(&dat.head);
        if (parseCommon(&dat.common, offsetof(s_read_address_0001, common)) == false)
            cmp = false;
        if (parseBase(&dat.base, offsetof(s_read_address_0001, base)) == false)
            cmp = false;

        int offset =
                0xb4; // remove padding  data in s_read_address_common and s_axis_info structure.

        std::memcpy(reinterpret_cast<void *>(&(dat.extend)), m_readBuffer + offset,
                    sizeof(dat.extend.mode));
    }
    m_mutexShared->unlock();

    m_dirty = false;
    m_currentAddress = 1;

    s_read_address_0001 *s = reinterpret_cast<s_read_address_0001 *>(m_interpret);
    if (isCompare == false) {
        cmp = false;
        m_mutexInterpret->lock();
        {
            std::memcpy(s, &dat, sizeof(dat));
        }
        m_mutexInterpret->unlock();
    } else {
        if (std::memcmp(&dat, reinterpret_cast<s_read_address_0001 *>(m_interpret), sizeof(dat))
            != 0)
            cmp = false;
        if (cmp == false) {
            m_mutexInterpret->lock();
            {
                std::memcpy(s, &dat, sizeof(dat));
            }
            m_mutexInterpret->unlock();
            qCDebug(sharedcontrol) << "Parse Address: 001" << sizeof(dat);
        }
    }

    return cmp;
}

bool ModBus::parseAddress0002(bool isCompare)
{
    s_read_address_0002 dat;

    bool cmp = true;
    m_mutexShared->lock();
    {
        cmp = parseHeader(&dat.head);
        if (parseCommon(&dat.common, offsetof(s_read_address_0002, common)) == false)
            cmp = false;
        if (parseBase(&dat.base, offsetof(s_read_address_0002, base)) == false)
            cmp = false;
        int offset = sizeof(s_read_address_header) + sizeof(s_read_address_common)
                + sizeof(s_read_address_base);

        offset = 0xb4;

        std::memcpy(reinterpret_cast<void *>(&(dat.extend)), m_readBuffer + offset,
                    sizeof(dat.extend.timer));
    }
    m_mutexShared->unlock();
    m_dirty = false;
    m_currentAddress = 2;

    s_read_address_0002 *s = reinterpret_cast<s_read_address_0002 *>(m_interpret);
    if (isCompare == false) {
        cmp = true;
        m_mutexInterpret->lock();
        {
            std::memcpy(s, &dat, sizeof(dat));
        }
        m_mutexInterpret->unlock();
    } else {
        if (std::memcmp(&dat, reinterpret_cast<s_read_address_0002 *>(m_interpret), sizeof(dat))
            != 0)
            cmp = false;
        if (cmp == false) {
            m_mutexInterpret->lock();
            {
                std::memcpy(s, &dat, sizeof(dat));
            }
            m_mutexInterpret->unlock();
            qCDebug(sharedcontrol) << "Parse Address: 002" << sizeof(dat);
        }
    }
    return cmp;
}

bool ModBus::parseAddress0004(bool isCompare)
{
    s_read_address_0004 dat;

    bool cmp = true;

    m_mutexShared->lock();
    {
        cmp = parseHeader(&dat.head);
        if (parseCommon(&dat.common, offsetof(s_read_address_0004, common)) == false)
            cmp = false;
        uint offset = 0x2c;
        dat.hist.historyNo = toUShort(m_readBuffer + offset + 34);
        for (uint i = 0; i < 14; ++i) {
            uint start = offset + 34 + 2 + i * sizeof(s_read_address_0004);
            ushort a = toUShort(m_readBuffer + start);
            if (a == 65532)
                break;
            dat.hist.hist[i] = parseProductionRecord(m_readBuffer + start);
        }
    }
    m_mutexShared->unlock();

    m_dirty = false;
    m_currentAddress = 4;

    s_read_address_0004 *s = reinterpret_cast<s_read_address_0004 *>(m_interpret);
    if (isCompare == false) {
        cmp = false;
        m_mutexInterpret->lock();
        {
            std::memcpy(s, &dat, sizeof(dat));
        }
        m_mutexInterpret->unlock();
    } else {
        if (std::memcmp(&dat, reinterpret_cast<s_read_address_0004 *>(m_interpret), sizeof(dat))
            != 0)
            cmp = false;
        if (cmp == false) {
            m_mutexInterpret->lock();
            {
                std::memcpy(s, &dat, sizeof(dat));
            }
            m_mutexInterpret->unlock();
            qCDebug(sharedcontrol) << "Parse Address: 004";
        }
    }

    return cmp;
}

bool ModBus::parseAddress0005(bool isCompare)
{
    s_read_address_0005 dat;

    bool cmp = true;

    ulong offset = 0x2c;
    m_mutexShared->lock();
    {
        cmp = parseHeader(&dat.head);
        for (ulong i = 0; i < 5; ++i) {
            ulong start = offset + i * 28;
            dat.icons[i] = parseIcon(m_readBuffer + start);
        }
        if (memcmp(&dat.icons, reinterpret_cast<s_menu_icons *>(m_interpret + offset),
                   sizeof(dat.icons))
            != 0)
            cmp = false;
    }
    m_mutexShared->unlock();
    m_dirty = false;
    m_currentAddress = 5;

    s_read_address_0005 *s = reinterpret_cast<s_read_address_0005 *>(m_interpret);
    if (isCompare == false) {
        cmp = true;
        m_mutexInterpret->lock();
        {
            std::memcpy(s, &dat, sizeof(dat));
        }
        m_mutexInterpret->unlock();
    } else {
        if (std::memcmp(&dat, reinterpret_cast<s_read_address_0005 *>(m_interpret), sizeof(dat))
            != 0)
            cmp = false;
        if (cmp == false) {
            m_mutexInterpret->lock();
            {
                std::memcpy(s, &dat, sizeof(dat));
            }
            m_mutexInterpret->unlock();
            qCDebug(sharedcontrol) << "Parse Address: 005" << sizeof(dat);
        }
    }

    return cmp;
}

void ModBus::sendData()
{
    m_mutexWrite->lock();
    sending_buffer *b = reinterpret_cast<sending_buffer *>(m_writeCache);
    uchar *ptr = m_writeBuffer;
    // Require Read address
    *ptr++ = 1;
    *ptr++ = 2;
    ushort readaddr = b->read_request;
    *reinterpret_cast<ushort *>(ptr) = readaddr;
    ptr = ptr + 2;
    *ptr++ = 0;
    *ptr++ = 0xfe;
    ushort crc = CRC(m_writeBuffer, 6);
    *reinterpret_cast<ushort *>(ptr) = crc;

    // Wait for response

    // HR write
    ptr = m_writeBuffer;
    *ptr++ = 1;
    *ptr++ = 0x10;
    *ptr++ = 0;
    *ptr++ = 0;
    *ptr++ = 0;
    *ptr++ = 0x7f;
    *ptr++ = 0xfe;
    std::memcpy(ptr, reinterpret_cast<uchar *>(&b->transfer), 254);
    crc = CRC(m_writeBuffer, 261);
    *reinterpret_cast<ushort *>(m_writeBuffer + 262) = crc;

    // Wait for response

    // Clean the buffer
    std::memset(m_writeCache, 0, 0x200);
    b->read_request = readaddr; // If new request is not comming until next cycle, the previous one
                                // will be used.

    m_mutexWrite->unlock();
}

bool ModBus::parseAddress()
{
    if (m_dirty == false) {
        // Try to update the last object
        switch (m_currentAddress) {
        case 0:
            return parseAddress0000(true);
        case 1:
            return parseAddress0001(true);
        case 2:
            return parseAddress0002(true);
        case 4:
            return parseAddress0004(true);
        case 5:
            return parseAddress0005(true);
        }
        return false;
    }

    switch (m_pageRequest) {
    case 0:
        parseAddress0000(false);
        break;
    case 1:
        parseAddress0001(false);
        break;
    case 2:
        parseAddress0002(false);
        break;
    case 4:
        parseAddress0004(false);
        break;
    case 5:
        parseAddress0005(false);
        break;
    }
    return false;
}

ushort ModBus::modBusMemory()
{
    s_read_address_header *h = reinterpret_cast<s_read_address_header *>(m_interpret);
    return toUShort(h->modBusMemory);
}

void ModBus::pageRequest(ushort page)
{
    if (m_currentAddress != page) {
        m_pageRequest = page;
        m_mutexWrite->lock();
        sending_buffer *b = reinterpret_cast<sending_buffer *>(m_writeCache);
        b->read_request = page;
        m_mutexWrite->unlock();
//        qCDebug(commandcontrol) << "Change page" << page;

        m_dirty = true;
//#define DEBUG
#ifdef DEBUG
        uchar *dat = reinterpret_cast<uchar *>(acquireBuffer());
        for (int i = 0; i < 256; ++i) {
            *dat++ = static_cast<uchar>(rand());
        }
        releaseBuffer();
#endif
    }
}
void ModBus::changeWholeSpeed(ulong i)
{
    m_mutexWrite->lock();
    sending_buffer *b = reinterpret_cast<sending_buffer *>(m_writeCache);
    b->changeWholeSpeed = i;
    m_mutexWrite->unlock();
//    qCDebug(commandcontrol) << "Change whole speed" << i;
}

void ModBus::changeTeachingSpeed(ulong i)
{
    m_mutexWrite->lock();
    sending_buffer *b = reinterpret_cast<sending_buffer *>(m_writeCache);
    b->changeTeachingSpeed = i;
    m_mutexWrite->unlock();
//    qCDebug(commandcontrol) << "Change teaching speed" << i;
}

void ModBus::changeLanguage(ushort i)
{
    m_mutexWrite->lock();
    sending_buffer *b = reinterpret_cast<sending_buffer *>(m_writeCache);
    b->language = i;
    m_mutexWrite->unlock();
//    qCDebug(commandcontrol) << "Change language" << i;
}

void ModBus::sendKey(uchar key, ushort screen)
{
    if (screen >= 1000)
        screen = screen - 999;
    m_mutexWrite->lock();
    sending_buffer *b = reinterpret_cast<sending_buffer *>(m_writeCache);
    b->key = key;
    b->screen1 = static_cast<uchar>((screen & 0xff00) >> 8);
    b->screen2 = static_cast<uchar>((screen & 0x00ff));
//    qCDebug(commandcontrol) << "Send key" << key << " -- " << b->screen1 << ", " << b->screen2;
    m_mutexWrite->unlock();
}

bool ModBus::parseHeader(s_read_address_header *head)
{
    std::memcpy(head->fileNumber, m_readBuffer, 2);
    std::memcpy(head->fileComment, m_readBuffer + 2, 20);
    std::memcpy(head->currentTime, m_readBuffer + 22, 6);
    std::memcpy(head->modBusMemory, m_readBuffer + 28, 2);
    head->wholeSpeed = static_cast<uchar>(*(m_readBuffer + 30));
    head->teachingSpeed = static_cast<uchar>(*(m_readBuffer + 31));
    std::memcpy(head->alertScreen, m_readBuffer + 32, 4);
    head->userLevel = *(m_readBuffer + 36);
    head->language = *(m_readBuffer + 37);
    std::memset(head->reserve, 0, 6);

    s_read_address_header *h = reinterpret_cast<s_read_address_header *>(m_interpret);
    bool cmp = std::memcmp(head, h, sizeof(s_read_address_header)) == 0 ? true : false;
    if (cmp == false) {
        std::memcpy(h, head, sizeof(s_read_address_header));
    }
    //    qCDebug(sharedcontrol) << "Header comparison - " << cmp;
    return cmp;
}

bool ModBus::parseCommon(s_read_address_common *common, uint offset)
{
    uint margin = 0x2c;

    common->numOfGood = toULong(m_readBuffer + margin);
    common->numOfBad = toULong(m_readBuffer + margin + 4);
    common->ejectTime = toULong(m_readBuffer + margin + 8);
    common->openTime = toULong(m_readBuffer + margin + 12);
    common->creationTime = toULong(m_readBuffer + margin + 16);
    common->numOfPlanned = toULong(m_readBuffer + margin + 20);
    common->numOnce = toUShort(m_readBuffer + margin + 24);
    std::memcpy(&common->expectedTime, m_readBuffer + margin + 26, 6);
    std::memcpy(&common->timeBeforeNote, m_readBuffer + margin + 32, 2);
    common->padding = 0;

    s_read_address_common *b = reinterpret_cast<s_read_address_common *>(m_interpret + offset);

    bool cmp = std::memcmp(common, b, sizeof(s_read_address_common)) == 0 ? true : false;
    if (cmp == false) {
        std::memcpy(b, common, sizeof(s_read_address_common));
    }
    //    qCDebug(sharedcontrol) << "Common comparison - " << cmp;
    return cmp;
}

s_axis_info ModBus::parseAxis(const uchar *v)
{
    s_axis_info a;
    a.currentPosition = toULong(v);
    a.currentSpeed = toUShort(v + 4);
    a.padding = 0;
    a.encoderPulse = toULong(v + 8);
    a.zPosition = toUShort(v + 12);
    a.currentTorque = toShort(v + 14);

    return a;
}

s_menu_icons ModBus::parseIcon(const uchar *v)
{
    s_menu_icons m;
    std::memcpy(&m.shortcut, v, 4);
    std::memcpy(&m.line1, v + 4, 6);
    std::memcpy(&m.line2, v + 10, 6);
    std::memcpy(&m.line3, v + 16, 6);
    std::memcpy(&m.line4, v + 22, 6);

    return m;
}

bool ModBus::parseBase(s_read_address_base *base, uint offset)
{
    uint margin = 0x52;

    std::memcpy(&base->origin, m_readBuffer + margin, 4);
    for (ulong i = 0; i < 7; ++i) {
        ulong start = margin + 4 + i * 16;
        base->axises[i] = parseAxis(m_readBuffer + start);
    }

    s_read_address_base *b = reinterpret_cast<s_read_address_base *>(m_interpret + offset);
    bool cmp = std::memcmp(base, b, sizeof(*base)) == 0 ? true : false;
    if (cmp == false) {
        std::memcpy(b, base, sizeof(s_read_address_base));
    }
    //    qCDebug(sharedcontrol) << "Base comparison - " << cmp;
    return cmp;
}

s_production_history ModBus::parseProductionRecord(const uchar *v)
{
    s_production_history h;
    std::memcpy(&h.time, v, 6);
    h.eject = toUShort(v + 6);
    h.open = toUShort(v + 8);
    h.creation = toUShort(v + 10);
    h.flag = toUShort(v + 12);
    return h;
}

ushort ModBus::CRC(uchar *data, int size)
{

    ushort short_c = 0;
    const unsigned char *ptr;

    ptr = data;

    ushort reg = 0xffff;
    if (ptr != nullptr)
        return 0;
    for (int a = 0; a < size; a++) {

        short_c = reg ^ static_cast<ushort>(*ptr);

        for (int i = 0; i < 8; ++i) {
            if ((short_c & 0x1) != 0) {
                short_c = short_c >> 1;
                short_c = 0xa001 ^ short_c;
            } else {
                short_c = short_c >> 1;
            }
        }
        reg = short_c;

        ptr++;
    }

    return static_cast<uchar>((short_c & 0xff00) >> 8)
            | static_cast<uchar>((short_c & 0x00ff) << 8);
}
