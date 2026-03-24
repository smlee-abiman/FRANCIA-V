#ifndef CORELIBSYSTEMCONTROL_H
#define CORELIBSYSTEMCONTROL_H

#include "corelibdrivermanager.h"
#include <sys/types.h>
#ifdef Q_OS_LINUX
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#endif // Q_OS_LINUX
#include <QTextStream>
#ifdef CORELIB_UNIT_TEST
#include "corelibunittestmockmethod.h"
#define SOCKET mMockMethod._mock_socket
#define IOCTL1 mMockMethod._mock_ioctl1
#else // CORELIB_UNIT_TEST
#define SOCKET socket
#define IOCTL1 ioctl
#endif // CORELIB_UNIT_TEST

#define NOT_USE_LINUX_NETWORK_SOCKET_INTERFACE

#define MAC_ADDRESS_MAX_SIZE 6
#define ETH_NAME_STR_MAX_SIZE 10
#define SUBNET_TO_BIT_MASK(subnet) \
    (subnet == 0xFFFFFFFF ? 32 : \
     subnet == 0xFFFFFFFE ? 31 : \
     subnet == 0xFFFFFFFC ? 30 : \
     subnet == 0xFFFFFFF8 ? 29 : \
     subnet == 0xFFFFFFF0 ? 28 : \
     subnet == 0xFFFFFFE0 ? 27 : \
     subnet == 0xFFFFFFC0 ? 26 : \
     subnet == 0xFFFFFF80 ? 25 : \
     subnet == 0xFFFFFF00 ? 24 : \
     subnet == 0xFFFFFE00 ? 23 : \
     subnet == 0xFFFFFC00 ? 22 : \
     subnet == 0xFFFFF800 ? 21 : \
     subnet == 0xFFFFF000 ? 20 : \
     subnet == 0xFFFFE000 ? 19 : \
     subnet == 0xFFFFC000 ? 18 : \
     subnet == 0xFFFF8000 ? 17 : \
     subnet == 0xFFFF0000 ? 16 : \
     subnet == 0xFFFE0000 ? 15 : \
     subnet == 0xFFFC0000 ? 14 : \
     subnet == 0xFFF80000 ? 13 : \
     subnet == 0xFFF00000 ? 12 : \
     subnet == 0xFFE00000 ? 11 : \
     subnet == 0xFFC00000 ? 10 : \
     subnet == 0xFF800000 ? 9 : \
     subnet == 0xFF000000 ? 8 : \
     subnet == 0xFE000000 ? 7 : \
     subnet == 0xFC000000 ? 6 : \
     subnet == 0xF8000000 ? 5 : \
     subnet == 0xF0000000 ? 4 : \
     subnet == 0xE0000000 ? 3 : \
     subnet == 0xC0000000 ? 2 : \
     subnet == 0x80000000 ? 1 : 0)

#define BIT_MASK_TO_SUBNET(bitmask) \
    (bitmask == 32 ? "255.255.255.255" : \
     bitmask == 31 ? "255.255.255.254" : \
     bitmask == 30 ? "255.255.255.252" : \
     bitmask == 29 ? "255.255.255.248" : \
     bitmask == 28 ? "255.255.255.240" : \
     bitmask == 27 ? "255.255.255.224" : \
     bitmask == 26 ? "255.255.255.192" : \
     bitmask == 25 ? "255.255.255.128" : \
     bitmask == 24 ? "255.255.255.0" : \
     bitmask == 23 ? "255.255.254.0" : \
     bitmask == 22 ? "255.255.252.0" : \
     bitmask == 21 ? "255.255.248.0" : \
     bitmask == 20 ? "255.255.240.0" : \
     bitmask == 19 ? "255.255.224.0" : \
     bitmask == 18 ? "255.255.192.0" : \
     bitmask == 17 ? "255.255.128.0" : \
     bitmask == 16 ? "255.255.0.0" : \
     bitmask == 15 ? "255.254.0.0" : \
     bitmask == 14 ? "255.252.0.0" : \
     bitmask == 13 ? "255.248.0.0" : \
     bitmask == 12 ? "255.240.0.0" : \
     bitmask == 11 ? "255.224.0.0" : \
     bitmask == 10 ? "255.192.0.0" : \
     bitmask == 9  ? "255.128.0.0" : \
     bitmask == 8  ? "255.0.0.0" : \
     bitmask == 7  ? "254.0.0.0" : \
     bitmask == 6  ? "252.0.0.0" : \
     bitmask == 5  ? "248.0.0.0" : \
     bitmask == 4  ? "240.0.0.0" : \
     bitmask == 3  ? "224.0.0.0" : \
     bitmask == 2  ? "192.0.0.0" : \
     bitmask == 1  ? "128.0.0.0" : "255.255.255.255")

class CoreLibSystemControl : public CoreLibDriverManager
{
public:
    CoreLibSystemControl();
    ~CoreLibSystemControl();
    void setBackLightBrightnessValue();
    bool setMACAddress(QString &macAddress, QString ifrNameStr = "eth0");
    bool getMACAddress(QString *macAddress, QString ifrNameStr = "eth0");
    bool setNetworkSetting(QString &ipAddress, QString &subNetMask);
    bool getNetworkSetting(QString *ipAddress, QString *subNetMask);
    int setSystemDateTime(QString systemDateTime);

private:
#ifdef Q_OS_LINUX
    typedef struct {
        char macAddress[MAC_ADDRESS_MAX_SIZE];
    }stMacAddressInfo;

    const QString systemEth0ServiceFilePath = "/lib/systemd/system/itz3000-eth0.service";
    const QString systemEth0ServiceFilePathTemp = "/lib/systemd/system/itz3000-eth0.service.tmp";
    uchar getSubNetMaskNum(QString &subNetMask);
    bool getMacAddressBitMask(QString &macAddress, stMacAddressInfo *macAddressInfo);
    QString getSubNetMaskStr(QString& bitMaskStr);
    bool setMacAddressStr(QString &macAddress, QString ifrNameStr);
#endif // Q_OS_LINUX
    bool qStringToChar(QString qstr, char *buff, int buff_size);
};

#endif // CORELIBSYSTEMCONTROL_H
