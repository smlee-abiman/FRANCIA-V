#include "corelibsystemcontrol.h"

#ifdef REAL_TARGET
#include <stdlib.h> // for system()
#endif // REAL_TARGET

// commit filesystem caches to disk.
void system_sync()
{
#ifdef REAL_TARGET
    int ret = system("sync");
    (void)ret;
#endif // REAL_TARGET
}

CoreLibSystemControl::CoreLibSystemControl()
{

}

CoreLibSystemControl::~CoreLibSystemControl()
{

}

#ifdef Q_OS_LINUX
void CoreLibSystemControl::setBackLightBrightnessValue()
{
#ifdef REAL_TARGET
    int ret = system("/lib/systemd/systemd-backlight save backlight:backlight");
    if (ret < 0){
        qCritical() << "system func failed.";
    }
#endif // REAL_TARGET
}

bool CoreLibSystemControl::setNetworkSetting(QString &ipAddress, QString &subNetMask)
{
    // get SubNetMask Num
    uchar subNetMaskNum = getSubNetMaskNum(subNetMask);
    QString subNetMaskNumStr = QString::number(subNetMaskNum);
    QFile file(systemEth0ServiceFilePath);
    if (!file.open(QIODevice::ReadWrite)){
        return false;
    }
    QFile outFile(systemEth0ServiceFilePathTemp);
    if (!outFile.open(QIODevice::ReadWrite)){
        return false;
    }
    QTextStream ts(&file);
    QTextStream out(&outFile);
    QString line;
    QString execStartStr("ExecStart=");
    QString execStopStr("ExecStop=");
    QString replaceStr = ipAddress + "/" + subNetMaskNumStr;
    QRegExp re("(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])(\\/[1-9]|\\/(1|2)[0-9]|\\/3[0-2])?");
    do {
        line = ts.readLine();
        if (line.contains(execStartStr, Qt::CaseSensitive)) {
            out << line.replace(re, replaceStr);
            out << "\n";
            continue;
        }
        if (line.contains(execStopStr, Qt::CaseSensitive)) {
            out << line.replace(re, replaceStr);
            out << "\n";
            continue;
        }
        out << line;
        out << "\n";
    } while(!line.isNull());
    file.close();
    outFile.close();
    QFile org(systemEth0ServiceFilePath);
    org.remove();
    QFile temp(systemEth0ServiceFilePathTemp);
    temp.rename(systemEth0ServiceFilePath);

    system_sync();

    return true;
}

bool CoreLibSystemControl::getNetworkSetting(QString *ipAddress, QString *subNetMask)
{
    QFile file(systemEth0ServiceFilePath);
    if (!file.open(QIODevice::ReadWrite)){
        return false;
    }
    QTextStream ts(&file);
    QString line;
    QString extractStr;
    int ipAddressTopPos = 0;
    int devPos = 0;
    QString execStartStr("ExecStart=");
    QString devStr("dev ");
    QRegExp re("(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])(\\/[1-9]|\\/(1|2)[0-9]|\\/3[0-2])?");
    do {
        line = ts.readLine();
        if (line.contains(execStartStr, Qt::CaseSensitive)) {
            //qDebug() << "line : " << line;
            break;
        }
    } while(!line.isNull());
    ipAddressTopPos = re.indexIn(line);
    devPos = line.indexOf(devStr);
    //qDebug("pos:%d devPos:%d", ipAddressTopPos, devPos);
    extractStr = line.mid(ipAddressTopPos, ((devPos - 1) - ipAddressTopPos));
    if (extractStr == ""){
        qCritical() << "getNetworkSetting failed extractStr : " << extractStr;
        file.close();
        return false;
    }
    QStringList extractStrList = extractStr.split("/");
    *ipAddress =  extractStrList[0];
    // get Sub Net Mask
    *subNetMask = getSubNetMaskStr(extractStrList[1]);
    if (*subNetMask == ""){
        qCritical() << "subNetMask is null : " << subNetMask;
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool CoreLibSystemControl::setMACAddress(QString &macAddress, QString ifrNameStr)
{
#ifdef NOT_USE_LINUX_NETWORK_SOCKET_INTERFACE
    qDebug() << "ifrNameStr:" << ifrNameStr;
#else  // NOT_USE_LINUX_NETWORK_SOCKET_INTERFACE
    int fd = 0, ret = 0;
    struct ifreq ifr;
    stMacAddressInfo macAddressInfo = { 0 };
    char ifrName[ETH_NAME_STR_MAX_SIZE];

    qInfo() << "ifrNameStr:" << ifrNameStr;
    if (ifrNameStr.size() > ETH_NAME_STR_MAX_SIZE
        || qStringToChar(ifrNameStr, ifrName, ifrNameStr.size() + 1) == false){
        qCritical() << "ifrNameStr size :" << ifrNameStr.size();
        return false;
    }
    qInfo("ifrName[%s]", ifrName);
    // get mac address bit mask
    if (getMacAddressBitMask(macAddress, &macAddressInfo) == false){
        qCritical() << "getMacAddressBitMask failed : ";
        return false;
    }
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd <= 0){
        qCritical() << "setMACAddress:socket failed : " << fd << "errno: "<< errno;
        return false;
    }
    /* eth-name set */
    strncpy(ifr.ifr_name, (const char *)ifrName, IFNAMSIZ-1);
    /* down */
    ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
    if (ret != 0) {
        qCritical() << "setMACAddress:ioctl down1 failed : " << ret << "errno: "<< errno;
        close(fd);
        return false;
    }
    /* flags change */
    ifr.ifr_flags &= ~IFF_UP;
    ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
    if (ret != 0) {
        qCritical() << "setMACAddress:ioctl down2 failed : " << ret << "errno: "<< errno;
        close(fd);
        return false;
    }
    /* set MAC address */
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    for(int i = 0; i < MAC_ADDRESS_MAX_SIZE; i++){
        ifr.ifr_hwaddr.sa_data[i] = macAddressInfo.macAddress[i];
    }
    ret = ioctl(fd, SIOCSIFHWADDR, &ifr);
    if (ret != 0) {
        qCritical() << "setMACAddress:ioctl failed : " << ret << "errno: "<< errno;
        close(fd);
        return false;
    }
    /* up */
    ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
    if (ret != 0) {
        qCritical() << "setMACAddress:ioctl up1 failed : " << ret << "errno: "<< errno;
        close(fd);
        return false;
    }
    /* flags change */
    ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
    ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
    if (ret != 0) {
        qCritical() << "setMACAddress:ioctl up2 failed : " << ret << "errno: "<< errno;
        close(fd);
        return false;
    }
    close(fd);
#endif // NOT_USE_LINUX_NETWORK_SOCKET_INTERFACE
    if (!setMacAddressStr(macAddress, ifrNameStr)){
        qCritical() << "setMacAddressStr failed : ";
        return false;
    }
    qInfo() << "setMACAddress success : ";
    return true;
}

bool CoreLibSystemControl::getMACAddress(QString *macAddress, QString ifrNameStr)
{
    int fd = 0, ret = 0;
    struct ifreq ifr;
    QString tmpStr;
    char ifrName[ETH_NAME_STR_MAX_SIZE];

    qInfo() << "ifrNameStr:" << ifrNameStr;
    if (ifrNameStr.size() > ETH_NAME_STR_MAX_SIZE
        || (qStringToChar(ifrNameStr, ifrName, ifrNameStr.size() + 1) == false)){
        qCritical() << "ifrNameStr size :" << ifrNameStr.size();
        return false;
    }
    qInfo("ifrName[%s]", ifrName);

    if (macAddress == nullptr){
        qCritical() << "macAddressInfo is null";
        return false;
    }
    fd = SOCKET(AF_INET, SOCK_DGRAM, 0);
    if (fd <= 0){
        qCritical() << "getMACAddress:socket failed : " << fd << "errno: "<< errno;
        return false;
    }
    /* eth-name set */
    strncpy(ifr.ifr_name, (const char*)ifrName, IFNAMSIZ-1);
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    ret = IOCTL1(fd, SIOCGIFHWADDR, &ifr);
    if (ret != 0) {
        qCritical() << "getMACAddress:ioctl failed : " << ret << "errno: "<< errno;
        CLOSE(fd);
        return false;
    }
    for(int i = 0; i < MAC_ADDRESS_MAX_SIZE; i++){
        if((uchar)ifr.ifr_hwaddr.sa_data[i] <= 0x0F){
            macAddress->append("0");
        }
        macAddress->append(QString::number((uchar)ifr.ifr_hwaddr.sa_data[i], 16));
        if (i < MAC_ADDRESS_MAX_SIZE - 1){
            macAddress->append(":");
        }
    }
    qInfo() << "macAddress: " << *macAddress;
    CLOSE(fd);
    return true;
}

uchar CoreLibSystemControl::getSubNetMaskNum(QString &subNetMask)
{
    int subNetMaskStrSize = subNetMask.size();
    char subNetMaskTmp[4] = { 0 };
    unsigned int subNetMaskNum = 0;
    QString tmpStr = "";
    bool is_ok;
    int j = 0;
    for(int i = 0; i < subNetMaskStrSize; i++){
        if (subNetMask[i] == '.'){
            subNetMaskTmp[j] = (char)tmpStr.toInt(&is_ok);
            if (!is_ok){
                qDebug() << "string to int change failed : " << tmpStr;
                continue;
            }
            tmpStr.clear();
            j++;
        } else {
            tmpStr.append(subNetMask[i]);
            if (i >= subNetMaskStrSize - 1){
                subNetMaskTmp[j] = (char)tmpStr.toInt(&is_ok);
                if (!is_ok){
                    qDebug() << "string to int change failed : " << tmpStr;
                    continue;
                }
                tmpStr.clear();
            }
        }
    }
    for (int i = 0; i < 4; i++){
        subNetMaskNum |= subNetMaskTmp[3 - i] << i * 8;
    }
    return SUBNET_TO_BIT_MASK(subNetMaskNum);
}

bool CoreLibSystemControl::getMacAddressBitMask(QString &macAddress, stMacAddressInfo *macAddressInfo)
{
    if (macAddressInfo == nullptr){
        qCritical() << "macAddressInfo is null : ";
        return false;
    }
    int macAddressStrSize = macAddress.size();
    int j = 0;
    QString tmpStr = "";
    bool is_ok;
    for(int i = 0; i < macAddressStrSize; i++){
        if (macAddress[i] == ':'){
            macAddressInfo->macAddress[j] = (char)tmpStr.toInt(&is_ok, 16);
            if (!is_ok){
                qDebug() << "string to int change failed : " << tmpStr;
                continue;
            }
            tmpStr.clear();
            j++;
        } else {
            tmpStr.append(macAddress[i]);
            if (i >= macAddressStrSize - 1){
                macAddressInfo->macAddress[j] = (char)tmpStr.toInt(&is_ok, 16);
                if (!is_ok){
                    qDebug() << "string to int change failed : " << tmpStr;
                    continue;
                }
                tmpStr.clear();
            }
        }
    }
    return true;
}

QString CoreLibSystemControl::getSubNetMaskStr(QString& bitMaskStr)
{
    bool is_ok;
    int num = bitMaskStr.toInt(&is_ok, 10);
    if (!is_ok)
    {
        qCritical() << "str is not Numerical value : " << bitMaskStr;
        return "";
    }
    return BIT_MASK_TO_SUBNET(num);
}

bool CoreLibSystemControl::setMacAddressStr(QString &macAddress, QString ifrNameStr)
{
    QFile file(systemEth0ServiceFilePath);
    if (!file.open(QIODevice::ReadWrite)){
        qCritical() << "open failed: " << systemEth0ServiceFilePath;
        return false;
    }
    QFile outFile(systemEth0ServiceFilePathTemp);
    if (!outFile.open(QIODevice::ReadWrite)){
        qCritical() << "open failed: " << systemEth0ServiceFilePathTemp;
        file.close();
        return false;
    }
    QTextStream searchTextStream(&file);
    QTextStream out(&outFile);
    QString line;
    QString insertStr;
    QString extractStr;
    QString execStartPreStr("ExecStartPre=");
    QString etherStr("ether ");
    uchar firstSearchCnt = 0;
    uchar secondSearchCnt = 0;
    //QRegExp re("(([0-9]|[a-f]|[A-F]){2}:){5}([0-9]|[a-f]|[A-F]){2}?");
    while (!searchTextStream.atEnd()) {
        line = searchTextStream.readLine();
        if (line.contains(execStartPreStr, Qt::CaseSensitive)) {
            firstSearchCnt++;
        }
    }
    if (!searchTextStream.seek(0)) {
        qCritical() << "seek(0) failed: " << systemEth0ServiceFilePath;
        file.close();
        outFile.close();
        return false;
    }
    insertStr = execStartPreStr + "/sbin/ifconfig " + ifrNameStr + " hw ether " + macAddress;
    qInfo() << "setMacAddressStr firstSearchCnt: " << firstSearchCnt;
    qInfo() << "insertStr: " << insertStr;
    if (firstSearchCnt >= 2) {
        while (!searchTextStream.atEnd()) {
            line = searchTextStream.readLine();
            if ((line.isNull() || line.isEmpty()) &&
                searchTextStream.atEnd()) {
                break;
            }
            if (line.contains(execStartPreStr, Qt::CaseSensitive)) {
                out << ((secondSearchCnt == 0) ? insertStr : line) << "\n";
                secondSearchCnt++;
            } else {
                out << line << "\n";
            }
        }
    } else {
        // case firstSearchCnt == 1.
        while (!searchTextStream.atEnd()) {
            line = searchTextStream.readLine();
            if ((line.isNull() || line.isEmpty()) &&
                searchTextStream.atEnd()) {
                break;
            }
            if (line.contains(execStartPreStr, Qt::CaseSensitive)) {
                out << insertStr << "\n";
            }
            out << line << "\n";
        }
    }
    file.close();
    outFile.close();
    if (!file.remove()) {
        qCritical() << "remove failed: " << systemEth0ServiceFilePath;
        return false;
    }
    if (!outFile.rename(systemEth0ServiceFilePath)) {
        qCritical() << "rename failed:" << systemEth0ServiceFilePathTemp << "to" << systemEth0ServiceFilePath;
        return false;
    }

    system_sync();

    return true;
}

int CoreLibSystemControl::setSystemDateTime(QString systemDateTime)
{
    int result = 0;
#ifdef REAL_TARGET
    QChar quotationAscii = 0x22;
    QString commandArgStr = "";
    commandArgStr.append(quotationAscii.toLatin1());
    for(int i = 0; i < systemDateTime.count(); i++){
        commandArgStr.append(systemDateTime.at(i));
    }
    commandArgStr.append(quotationAscii.toLatin1());
    QString commandString = "date -s " + commandArgStr;
    QByteArray str_array = commandString.toLocal8Bit();
    const char *command = str_array.constData();
    //qDebug("command[%s]", command);
    result = system(command);
    if (result < 0){
        qCritical("%s:%d command[%s] is failed[%d]", __func__, __LINE__, command, result);
    }
#else   // REAL_TARGET
    (void)systemDateTime;
#endif // REAL_TARGET
    return result;
}
#else
void CoreLibSystemControl::setBackLightBrightnessValue()
{

}

bool CoreLibSystemControl::setMACAddress(QString &macAddress, QString ifrNameStr)
{
    qInfo() << "macAddress: " << macAddress << "ifrNameStr" << ifrNameStr;
    return true;
}
bool CoreLibSystemControl::getMACAddress(QString *macAddress, QString ifrNameStr)
{
    if (!macAddress){
        return false;
    }
    qInfo() << "ifrNameStr" << ifrNameStr;
    return true;
}
bool CoreLibSystemControl::setNetworkSetting(QString &ipAddress, QString &subNetMask)
{
    qInfo() << "ipAddress: " << ipAddress << "subNetMask" << subNetMask;
    return true;
}
bool CoreLibSystemControl::getNetworkSetting(QString *ipAddress, QString *subNetMask)
{
    if (!ipAddress || !subNetMask){
        return false;
    }
    return true;
}

int CoreLibSystemControl::setSystemDateTime(QString systemDateTime)
{
    (void)systemDateTime;
    return 0;
}
#endif // Q_OS_LINUX

bool CoreLibSystemControl::qStringToChar(QString qstr, char *buff, int buff_size)
{
    //バイト配列
    QByteArray qbyte = qstr.toLocal8Bit();
    //バイト配列の長さ
    int qstr_len = qbyte.length();
    //長さ 0 ？
    if (qstr_len == 0 || buff_size <= 0){
        return false;
    }
    //バッファ1文字？
    if (buff_size == 1)
    {
        buff[0] = '\0';
        return false;
    }
    //コピーする長さ
    buff_size--;
    int len = (qstr_len > buff_size) ? buff_size : qstr_len;
    //コピー
    memcpy(buff, qbyte.data(), len);
    //終端
    buff[len] = '\0';
    return true;
}


