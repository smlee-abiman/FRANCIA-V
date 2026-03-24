#include "corelibgpiocontrol.h"
#ifdef REAL_TARGET
#include <sys/ioctl.h>
#endif // REAL_TARGET

CoreLibGpioControl::CoreLibGpioControl()
{
}

CoreLibGpioControl::~CoreLibGpioControl()
{
}

int CoreLibGpioControl::controlBuzzer(bool isBuzzer, stBuzzerOutput &buzzerOutput)
{
    int ret = 0;
    int buzzeParam = 0;
    if (isBuzzer == true){  // Buzzer On
        buzzeParam = buzzerOutput.pitch << 24;
        buzzeParam |= qMin((buzzerOutput.onTime * 1000), 100) << 14;
        buzzeParam |= qMin((buzzerOutput.offTime * 1000), 100) << 4;
        buzzeParam |= qMin((int)buzzerOutput.repeat, 15);
    }
    ret = LSEEK(mDevFd[DriverControlTypeGpioBuzzer], 0, SEEK_SET);
    if (ret < 0){
        qCritical() << "dev file lseek error " << ret << "errno: "<< errno << "isBuzzer:" << isBuzzer;
        return ret;
    }
    ret = WRITE4(mDevFd[DriverControlTypeGpioBuzzer], &buzzeParam, 4);
    if (ret < 0){
        qCritical() << "dev file write error " << ret << "errno: "<< errno << "isBuzzer:" << isBuzzer;
    }
    return ret;
}

int CoreLibGpioControl::getInputDataBuffer(QBitArray *pInputDataBuf, uchar holdCount)
{
#if defined(REAL_TARGET) || defined(CORELIB_UNIT_TEST)
    int ret = -1;
    int inputDataBuf[INPUT_DATA_BUF_MAX] = { 0 };
    if (pInputDataBuf == nullptr || holdCount > INPUT_DATA_BUF_MAX){
        qCritical("pInputDataBuf[%p] holdNumber[%d]", pInputDataBuf, holdCount);
        return ret;
    }
    ret = LSEEK(mDevFd[DriverControlTypeGpioInputDataBuf], 0, SEEK_SET);
    if (ret < 0){
        qCritical("%s:%d dev file seek error ret[%d] errno[%d]", __func__, __LINE__, ret, errno);
        return ret;
    }
    ret = READ4(mDevFd[DriverControlTypeGpioInputDataBuf], inputDataBuf, sizeof(int)*INPUT_DATA_BUF_MAX);
    if (ret < 0){
        qCritical() << "dev file read error " << ret << "errno: "<< errno;
        return ret;
    }
    for(int i = 1; i <= (int)holdCount; i++){
        for(int j = 0; j < 32; j++){
            pInputDataBuf[i - 1].setBit(31 - j,
                (((inputDataBuf[INPUT_DATA_BUF_MAX - i] >> (31 - j)) & ~0xFFFFFFFE) == 0x000000001 ? 1 : 0));
        }
    }
#else // defined(REAL_TARGET) || defined(CORELIB_UNIT_TEST)
    int ret = 0;

    for(int i = 1; i <= (int)holdCount; i++){
        for(int j = 0; j < 32; j++){
            pInputDataBuf[i - 1].setBit(31 - j,
                (((mEmulateInputDataBuf >> (31 - j)) & ~0xFFFFFFFE) == 0x000000001 ? 1 : 0));
        }
    }
#endif // defined(REAL_TARGET) || defined(CORELIB_UNIT_TEST)
    return ret;
}

bool CoreLibGpioControl::controlLed(bool isLed, LedType ledType)
{
    DriverControlType drvType =
        (ledType == LedTypeBlue ? DriverControlTypeGpioLedBlue: \
         ledType == LedTypeGreen ? DriverControlTypeGpioLedGreen: \
         ledType == LedTypeRed ? DriverControlTypeGpioLedRed: DriverControlTypeMax);
    if (drvType >= DriverControlTypeMax || mDevFile[drvType] == nullptr){
        qInfo() << "drvType is invalid " << drvType;
        return false;
    }
    QString ledCtrl = (isLed == true ? "255" : "0");
    QTextStream stream(mDevFile[drvType]);
    stream << ledCtrl;
    stream.flush();
    return true;
}

#ifdef REAL_TARGET
bool CoreLibGpioControl::enableWdt(bool isEnable)
{
    if (isEnable){
        QString wdtCtrl = (isEnable == true ? "1" : "0");
        DriverControlType drvType = DriverControlTypeWatchDogTimerInh;
        QTextStream stream(mDevFile[drvType]);
        stream << wdtCtrl;
        stream.flush();
    }
    return true;
}

int CoreLibGpioControl::controlWDT(unsigned int cmd)
{
    int result = 0;
    DriverControlType drvType = DriverControlTypeWatchDogTimerClock;
    if (mDevFd[drvType] <= 0){
        qCritical("%s:%d mDevFd[%d], drvType[%d]", __func__, __LINE__, mDevFd[drvType], drvType);
        return -1;
    }
    int arg = 0;
    switch(cmd){
    case WDIOC_KEEPALIVE:
        arg = 0;
        break;
    case WDIOC_SETTIMEOUT:
        arg = WDT_TIMEOUT_NUM;
        break;
    default:
        return -1;
    }
    result = ioctl(mDevFd[drvType], cmd, &arg);
    if (result < 0){
        qCritical("%s:%d result[%d] devfd[%d] 0x%08x errno[%d]", __func__, __LINE__, result, mDevFd[drvType], cmd, errno);
    }
    return result;
}
#endif // REAL_TARGET

#ifdef REAL_TARGET
#else // REAL_TARGET
void CoreLibGpioControl::emulateKey01() {
    mEmulateInputDataBuf &= 0xFFFF71FE;  // SEL0 & KIN0
}
void CoreLibGpioControl::emulateKey02() {
    mEmulateInputDataBuf &= 0xFFFFB1FE;  // SEL1 & KIN0
}
void CoreLibGpioControl::emulateKey03() {
    mEmulateInputDataBuf &= 0xFFFF71FD;  // SEL0 & KIN1
}
void CoreLibGpioControl::emulateKey04() {
    mEmulateInputDataBuf &= 0xFFFFB1FD;  // SEL1 & KIN1
}
void CoreLibGpioControl::emulateKey05() {
    mEmulateInputDataBuf &= 0xFFFF71FB;  // SEL0 & KIN2
}
void CoreLibGpioControl::emulateKey06() {
    mEmulateInputDataBuf &= 0xFFFFB1FB;  // SEL1 & KIN2
}
void CoreLibGpioControl::emulateKey07() {
    mEmulateInputDataBuf &= 0xFFFF71F7;  // SEL0 & KIN3
}
void CoreLibGpioControl::emulateKey08() {
    mEmulateInputDataBuf &= 0xFFFFB1F7;  // SEL1 & KIN3
}
void CoreLibGpioControl::emulateKey09() {
    mEmulateInputDataBuf &= 0xFFFF71EF;  // SEL0 & KIN4
}
void CoreLibGpioControl::emulateKey10() {
    mEmulateInputDataBuf &= 0xFFFFB1EF;  // SEL1 & KIN4
}
void CoreLibGpioControl::emulateKey11() {
    mEmulateInputDataBuf &= 0xFFFF71DF;  // SEL0 & KIN5
}
void CoreLibGpioControl::emulateKey12() {
    mEmulateInputDataBuf &= 0xFFFFB1DF;  // SEL1 & KIN5
}
void CoreLibGpioControl::emulateKey13() {
    mEmulateInputDataBuf &= 0xFFFF71BF;  // SEL0 & KIN6
}
void CoreLibGpioControl::emulateKey14() {
    mEmulateInputDataBuf &= 0xFFFFB1BF;  // SEL1 & KIN6
}
void CoreLibGpioControl::emulateKey15() {
    mEmulateInputDataBuf &= 0xFFFF717F;  // SEL0 & KIN7
}
void CoreLibGpioControl::emulateKey16() {
    mEmulateInputDataBuf &= 0xFFFFB17F;  // SEL1 & KIN7
}
void CoreLibGpioControl::emulateKey17() {
    mEmulateInputDataBuf &= 0xFFFF70FF;  // SEL0 & KIN8
}
void CoreLibGpioControl::emulateKey18() {
    mEmulateInputDataBuf &= 0xFFFFB0FF;  // SEL1 & KIN8
}
void CoreLibGpioControl::emulateKeyAllOff()
{
    mEmulateInputDataBuf |= DEFAULT_EMULATE_KEY_VALUE;
}
void CoreLibGpioControl::emulateJogDialSwOn()
{
    mEmulateInputDataBuf &= 0xFFFBFFFF;
}
void CoreLibGpioControl::emulateJogDialSwOff()
{
    mEmulateInputDataBuf |= DEFAULT_EMULATE_JOG_DIAL_SW;
}
void CoreLibGpioControl::emulateDeadManSwOn()
{
    mEmulateInputDataBuf &= 0xFFBFFFFF;
}
void CoreLibGpioControl::emulateDeadManSwOff()
{
    mEmulateInputDataBuf |= ~0xFFBFFFFF;
}

void CoreLibGpioControl::emulateSelectSwAuto()
{
    mEmulateInputDataBuf &= 0xFFE7FFFF;
}
void CoreLibGpioControl::emulateSelectSwManual()
{
    mEmulateInputDataBuf &= 0xFFEFFFFF;
}
void CoreLibGpioControl::emulateSelectSwOrigin()
{
    mEmulateInputDataBuf &= 0xFFCFFFFF;
}
void CoreLibGpioControl::emulateSelectSwMenu()
{
    mEmulateInputDataBuf &= 0xFFCFFFFF;
}
void CoreLibGpioControl::emulateSelectSwOff()
{
    mEmulateInputDataBuf |= ~0xFFC7FFFF;
}
#endif
