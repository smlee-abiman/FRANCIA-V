/*
 * This class handles modBus data architecture and parse it store cache struture.
 * Also accept the request to the controller. Formatize it and throw controller communication block.
 */

#ifndef MODBUS_H
#define MODBUS_H

#include <cstring>
#include <QMetaType>
#include <QMutex>
#include <QDebug>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(sharedcontrol)
//Q_DECLARE_LOGGING_CATEGORY(commandcontrol)

// Axe information (address 0x52 ~ 0xb4)
struct s_axis_info
{
    ulong currentPosition;
    ushort currentSpeed;
    ushort padding; // To align long word alignment for following
    ulong encoderPulse;
    ushort zPosition;
    short currentTorque;
};
Q_DECLARE_METATYPE(s_axis_info)

// Common data area on each read address (0x00 ~ 0x2c)
typedef struct
{
    uchar fileNumber[2];
    uchar fileComment[20];
    uchar currentTime[6];
    uchar modBusMemory[2];
    uchar wholeSpeed;
    uchar teachingSpeed;
    uchar alertScreen[4]; // Alert1, Alert2, Alert Display, Working Display
    uchar userLevel;
    uchar language;
    uchar reserve[6];
} s_read_address_header;
Q_DECLARE_METATYPE(s_read_address_header *)

// Common area for most of read addresses (0x2c ~ 0x4e)
typedef struct
{
    ulong numOfGood;
    ulong numOfBad;
    ulong ejectTime;
    ulong openTime; // Format
    ulong creationTime; // Format
    ulong numOfPlanned;
    ushort numOnce;
    uchar expectedTime[6];
    uchar timeBeforeNote[2];
    ushort padding; // Align end of structure to be 4 byte aligned.
} s_read_address_common;
Q_DECLARE_METATYPE(s_read_address_common *)

// Common area for read address 0000, 0001, 0002 (0x4e ~ 0xb4)
typedef struct
{
    uchar origin[4];
    s_axis_info axises[7];
} s_read_address_base;
Q_DECLARE_METATYPE(s_read_address_base *)

// I/O status (0xb4 ~ 0xd4) in read address 0000
typedef struct
{
    uchar io[4 * 8 * 8 / 8];
} s_read_address_0000_;

typedef struct
{
    s_read_address_header head;
    s_read_address_common common;
    s_read_address_base base;
    s_read_address_0000_ extend;
} s_read_address_0000;
Q_DECLARE_METATYPE(s_read_address_0000 *)

// Mode status (0xb4 ~ 0xbe) in read address 0001
typedef struct
{
    uchar mode[4 * 8 * 3 / 8];
} s_read_address_0001_;

typedef struct
{
    s_read_address_header head;
    s_read_address_common common;
    s_read_address_base base;
    s_read_address_0001_ extend;
} s_read_address_0001;
Q_DECLARE_METATYPE(s_read_address_0001 *)

// Timer status (0xb4 ~ 0xd6) in read address 0002
typedef struct
{
    uchar timer[4 * 8 * 12 / 8];
} s_read_address_0002_;

typedef struct
{
    s_read_address_header head;
    s_read_address_common common;
    s_read_address_base base;
    s_read_address_0002_ extend;
} s_read_address_0002;
Q_DECLARE_METATYPE(s_read_address_0002 *)

// Production History (0x4e ~ 0xf8) in read address 0004
typedef struct
{
    uchar time[6];
    ushort eject;
    ushort open;
    ushort creation;
    ushort flag;
} s_production_history;
Q_DECLARE_METATYPE(s_production_history *)

typedef struct
{
    ushort historyNo;
    s_production_history hist[14];
} s_read_address_0004_;

typedef struct
{
    s_read_address_header head;
    s_read_address_common common;
    s_read_address_0004_ hist;
} s_read_address_0004;
Q_DECLARE_METATYPE(s_read_address_0004 *)

// Menu Icon Layout Data (0x2c ~ 0xbc) in read address 0005
typedef struct
{
    uchar shortcut[4];
    uchar line1[6];
    uchar line2[6];
    uchar line3[6];
    uchar line4[6];
} s_menu_icons;
Q_DECLARE_METATYPE(s_menu_icons *)

typedef struct
{
    s_read_address_header head;
    s_menu_icons icons[5];
} s_read_address_0005;
Q_DECLARE_METATYPE(s_read_address_0005 *)

// Write buffer
typedef struct
{
    ushort read_request;
    ushort transfer;
    uchar key;
    uchar screen1;
    uchar screen2;
    uchar alerm;
    uchar registers[16];
    ulong changeWholeSpeed;
    ulong changeTeachingSpeed;
    ushort language;
    ushort reserve1;
    ushort userLevel;
    ushort reserved2;
    uchar reserved3[0xd8] = { 0 };
} sending_buffer;

// ModBusMemory accessor (mask bit)
typedef enum {
    modORGM = 0x0001,
    modMANUM = 0x0002,
    modAUTOM = 0x0004,
    modSTART = 0x0008,
    modALMF = 0x0010,
    modMSEL = 0x0020,
    modSSEL = 0x0040,
    modMANREQ = 0x0080,
    modACTION = 0x0100,
    modDATNG = 0x4000,
    modEMGA = 0x8000
} modeBusMemory;

//
// The best way to access shared memory is overmapped structure.
// However shared memory data alignment is not consider padding of the CPU memory access boudary.
// Thus we need to recreate each of structures from raw dtata.
// This class has some utility functions to parse raw data.
//
class ModBus
{
public:
    explicit ModBus()
    {
        m_mutexInterpret = new QMutex(QMutex::Recursive);
        m_mutexShared = new QMutex(QMutex::Recursive);
        m_mutexWrite = new QMutex(QMutex::Recursive);
#if defined(WINNT) || 1 // TODO: add support for the real system
        m_readBuffer = reinterpret_cast<uchar *>(std::malloc(0x206f8));
        m_writeBuffer = reinterpret_cast<uchar *>(std::malloc(0x400));
        m_interpret =
                reinterpret_cast<uchar *>(std::malloc(0x400)); // With margin including padding
        m_writeCache = reinterpret_cast<uchar *>(std::malloc(0x200));

#    if 0
        // Test
        for (int i = 0 ; i < 255; ++i) {
            *(m_readBuffer + i) = static_cast<uchar>(rand());
        }
#    endif
        memset(m_readBuffer, 0, 0x206f8);
        memset(m_writeBuffer, 0, 0x400);
        memset(m_interpret, 0, 0x400);
        memset(m_writeCache, 0, 0x200);
        pageRequest(0);
#else
        // In the real system may use fixed address for communication via modBus
        m_readBuffer = reinterpret_cast<uchar *>(0x8000000); // Example
        m_writeBuffer = reinterpret_cast<uchar *>(0x9000000); // Example
        m_interpret = reinterpret_cast<uchar *>(0xa000000); // Example
#endif
    }

    ~ModBus()
    {
#if defined(WINNT) || 1 // TODO: add support for the real system
        free(m_readBuffer);
        free(m_writeBuffer);
        free(m_interpret);
        free(m_writeCache);
#endif
    }

    // Request read address (page means read address number)
    void pageRequest(ushort page);

    // Lock and acuire modBus address
    void *acquireBuffer()
    {
        m_mutexShared->lock();
        return m_readBuffer;
    }

    // Release modBus address
    inline void releaseBuffer() { m_mutexShared->unlock(); }

    // Lock internal read address cache and get its memory address as header block
    s_read_address_header *lockHeaderAddress()
    {
        // Lock
        m_mutexInterpret->lock();
        return reinterpret_cast<s_read_address_header *>(m_interpret);
    }

    // Lock internal read address cache and get its memory address as common block
    s_read_address_common *lockCommonAddress()
    {
        // Lock
        m_mutexInterpret->lock();
        return reinterpret_cast<s_read_address_common *>(m_interpret + 44);
    }

    // Lock internal read address cache and get its memory address as read address 0000
    s_read_address_0000 *lockAddress0000()
    {
        // Check is ready
        if (m_currentAddress != 0) {
            return nullptr;
        }

        // Lock
        m_mutexInterpret->lock();
        return reinterpret_cast<s_read_address_0000 *>(m_interpret);
    }

    // Lock internal read address cache and get its memory address as read address 0001
    s_read_address_0001 *lockAddress0001()
    {
        // Check is ready
        if (m_currentAddress != 1) {
            return nullptr;
        }

        // Lock
        m_mutexInterpret->lock();
        return reinterpret_cast<s_read_address_0001 *>(m_interpret);
    }

    // Lock internal read address cache and get its memory address as read address 0002
    s_read_address_0002 *lockAddress0002()
    {
        // Check is ready
        if (m_currentAddress != 2) {
            return nullptr;
        }

        // Lock
        m_mutexInterpret->lock();
        return reinterpret_cast<s_read_address_0002 *>(m_interpret);
    }

    // Lock internal read address cache and get its memory address as read address 0004
    s_read_address_0004 *lockAddress0004()
    {
        // Check is ready
        if (m_currentAddress != 4) {
            return nullptr;
        }

        // Lock
        m_mutexInterpret->lock();
        return reinterpret_cast<s_read_address_0004 *>(m_interpret);
    }

    // Lock internal read address cache and get its memory address as read address 0005
    s_read_address_0005 *lockAddress0005()
    {
        // Check is ready
        if (m_currentAddress != 5) {
            return nullptr;
        }

        // Lock
        m_mutexInterpret->lock();
        return reinterpret_cast<s_read_address_0005 *>(m_interpret);
    }

    // Release locked internal read address cache
    inline void unlockAddress() { m_mutexInterpret->unlock(); }

    // Parse each of read address buffer
    bool parseAddress0000(bool isCompare); // I/O
    bool parseAddress0001(bool isCompare); // Mode status
    bool parseAddress0002(bool isCompare); // Timer status
    bool parseAddress0004(bool isCompare); // Production History
    bool parseAddress0005(bool isCompare); // Menu layout

    // Send request(s) to the controller
    void sendData();

    // Compare and copy read address to internal buffer
    bool parseAddress();

    // Return modBus memory (offset: 0x1c from top of read address)
    ushort modBusMemory();

    // Return current page (Read address number)
    ushort requestedPage() { return m_pageRequest; }

    // Get modBus Register status
    bool ORGM() { return ((modBusMemory() & modORGM) != 0); }
    bool MANUM() { return ((modBusMemory() & modMANUM) != 0); }
    bool AUTOM() { return ((modBusMemory() & modAUTOM) != 0); }
    bool START() { return ((modBusMemory() & modSTART) != 0); }
    bool ALMF() { return ((modBusMemory() & modALMF) != 0); }
    bool MSEL() { return ((modBusMemory() & modMSEL) != 0); }
    bool SSEL() { return ((modBusMemory() & modSSEL) != 0); }
    bool MANREQ() { return ((modBusMemory() & modMANREQ) != 0); }
    bool ACTION() { return ((modBusMemory() & modACTION) != 0); }
    bool DATNG() { return ((modBusMemory() & modDATNG) != 0); }
    bool EMGA() { return ((modBusMemory() & modEMGA) != 0); }

    // Apply modBus related properties
    void changeWholeSpeed(ulong i);
    void changeTeachingSpeed(ulong i);
    void changeLanguage(ushort i);

    // Handle key press event to write cache (will be used when sendData() is called)
    void sendKey(uchar key, ushort screen);

    //
    // Utility functions for value conversion from memory image
    //
    // Byte array to signed short
    inline short toShort(const uchar *v) { return (*v | static_cast<short>(*(v + 1) << 8)); }

    // Byte array to signed long
    inline long toLong(const uchar *v)
    {
        return (*v | *(v + 1) << 8 | *(v + 2) << 16 | *(v + 3) << 24);
    }

    // Byte array to unsigned short
    inline ushort toUShort(const uchar *v) { return (*v | static_cast<ushort>(*(v + 1) << 8)); }

    // Byte array to signed long
    inline ulong toULong(const uchar *v)
    {
        return (static_cast<ulong>(*v) | static_cast<ulong>(*(v + 1)) << 8
                | static_cast<ulong>(*(v + 2)) << 16 | static_cast<ulong>(*(v + 3)) << 24);
    }

private:
    // Internal functions called from frontend, no need to consider thread safe...
    bool parseHeader(s_read_address_header *head);
    bool parseCommon(s_read_address_common *common, uint offset);
    bool parseBase(s_read_address_base *base, uint offset);
    // Inner elements in read address block.
    // Read address 0000 ~ 0002
    s_axis_info parseAxis(const uchar *v);
    // Read Address 0005
    s_menu_icons parseIcon(const uchar *v);
    // Read Address 0004
    s_production_history parseProductionRecord(const uchar *v);

    // Calcurate CRC from the data.
    ushort CRC(uchar *data, int size);

    // Buffer address
    uchar *m_readBuffer; // Shared Memory with controller for receiving
    uchar *m_writeBuffer; // Sending Buffer
    uchar *m_interpret; // Interface to UI domain (tentative? finally mapped structure is the
                        // best...)
    uchar *m_writeCache;

    // Mutexes
    QMutex *m_mutexShared;
    QMutex *m_mutexInterpret;
    QMutex *m_mutexWrite;

    int m_currentAddress = -1; // Current page (read address) number
    bool m_dirty = true; // New read request is comming
    ushort m_pageRequest = 0; // Requested page (not yet processed)
};

#endif // MODBUS_H
