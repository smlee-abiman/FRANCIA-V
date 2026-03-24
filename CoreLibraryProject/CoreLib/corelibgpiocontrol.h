#ifndef CORELIBGPIOCONTROL_H
#define CORELIBGPIOCONTROL_H

#include "corelibdrivermanager.h"
#include <QTimer>
#include <QMutex>
#ifdef CORELIB_UNIT_TEST
#include "corelibunittestmockmethod.h"
#define READ4 mMockMethod._mock_read4
#define WRITE4 mMockMethod._mock_write4
#define LSEEK mMockMethod._mock_lseek
#else // CORELIB_UNIT_TEST
#define READ4 read
#define WRITE4 write
#define LSEEK lseek
#endif // CORELIB_UNIT_TEST

#define INPUT_DATA_BUF_MAX  128
#ifdef REAL_TARGET
#else // REAL_TARGET
// 0000 0000 0000 0000 1100 0001 1111 1111
#define DEFAULT_EMULATE_KEY_VALUE 0xC1FF
// 0000 0000 0000 0100 0000 0000 0000 0000
#define DEFAULT_EMULATE_JOG_DIAL_SW 0x40000
// 0000 0000 0011 1000 0000 0000 0000 0000
#define DEFAULT_EMULATE_SELECT_SW 0x00380000
// 0000 0000 0100 0000 0000 0000 0000 0000
#define DEFAULT_EMULATE_DEADMAN_SW 0x00400000
#endif // REAL_TARGET

typedef struct {
    uchar   pitch;
    ushort  onTime;
    ushort  offTime;
    uchar   repeat;
} stBuzzerOutput;

#ifdef REAL_TARGET
#define WDIOC_KEEPALIVE 0x80045705
#define WDIOC_SETTIMEOUT 0xc0045706
#define WDT_TIMEOUT_NUM 60               // seconds
#endif // REAL_TARGET

class CoreLibGpioControl : public CoreLibDriverManager
{
public:
    CoreLibGpioControl();
    ~CoreLibGpioControl();
    int controlBuzzer(bool isBuzzer, stBuzzerOutput& buzzerOutput);
    int getInputDataBuffer(QBitArray *pInputDataBuf, uchar holdCount);
    bool controlLed(bool isLed, LedType ledType);
#ifdef REAL_TARGET
    bool enableWdt(bool isEnable);
    int controlWDT(unsigned int cmd);
#endif // REAL_TARGET

#ifdef REAL_TARGET
#else // REAL_TARGET
public:
    void emulateKey01();
    void emulateKey02();
    void emulateKey03();
    void emulateKey04();
    void emulateKey05();
    void emulateKey06();
    void emulateKey07();
    void emulateKey08();
    void emulateKey09();
    void emulateKey10();
    void emulateKey11();
    void emulateKey12();
    void emulateKey13();
    void emulateKey14();
    void emulateKey15();
    void emulateKey16();
    void emulateKey17();
    void emulateKey18();
    void emulateKeyAllOff();
    void emulateJogDialSwOn();
    void emulateJogDialSwOff();
    void emulateDeadManSwOn();
    void emulateDeadManSwOff();
    void emulateSelectSwAuto();
    void emulateSelectSwManual();
    void emulateSelectSwOrigin();
    void emulateSelectSwMenu();
    void emulateSelectSwOff();

private:
    quint32 mEmulateInputDataBuf =
        DEFAULT_EMULATE_KEY_VALUE + DEFAULT_EMULATE_JOG_DIAL_SW +
        DEFAULT_EMULATE_SELECT_SW + DEFAULT_EMULATE_DEADMAN_SW;
#endif // REAL_TARGET

private:

};

#endif // CORELIBGPIOCONTROL_H
