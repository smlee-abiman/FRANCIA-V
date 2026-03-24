#include "corelibunittestmockmethod.h"

#ifdef CORELIB_UNIT_TEST
void CoreLibUnitTestMockMethod::setMock(void* mockIns)
{
    _mMock = (CoreLibUnitTestMockMethod*)mockIns;
}

int CoreLibUnitTestMockMethod::_mock_open(const char *pathname, int flags)
{
    return _mMock->mock_open(pathname, flags);
}

int CoreLibUnitTestMockMethod::_mock_close(int fd)
{
    return _mMock->mock_close(fd);
}

int CoreLibUnitTestMockMethod::_mock_read1(int fd, char *buf, size_t count)
{
    return _mMock->mock_read1(fd, buf, count);
}
int CoreLibUnitTestMockMethod::_mock_read2(int fd, ushort *buf, size_t count)
{
    return _mMock->mock_read2(fd, buf, count);
}
int CoreLibUnitTestMockMethod::_mock_read4(int fd, int *buf, size_t count)
{
    return _mMock->mock_read4(fd, buf, count);
}
int CoreLibUnitTestMockMethod::_mock_read_event(int fd, struct input_event *event, size_t count)
{
    return _mMock->mock_read_event(fd, event, count);
}
off_t CoreLibUnitTestMockMethod::_mock_lseek(int fd, off_t offset, int whence)
{
    return _mMock->mock_lseek(fd, offset, whence);
}

bool CoreLibUnitTestMockMethod::_mock_modbus_connect(void)
{
    return _mMock->mock_modbus_connect();
}
QModbusReply* CoreLibUnitTestMockMethod::_mock_modbus_send_read_request(QModbusDataUnit &read, int serverAddress)
{
    return _mMock->mock_modbus_send_read_request(read, serverAddress);
}
QModbusReply* CoreLibUnitTestMockMethod::_mock_modbus_send_write_request(QModbusDataUnit &write, int serverAddress)
{
    return _mMock->mock_modbus_send_write_request(write, serverAddress);
}
int CoreLibUnitTestMockMethod::_mock_socket(int socket_family, int socket_type, int protocol)
{
    return _mMock->mock_socket(socket_family, socket_type, protocol);
}
int CoreLibUnitTestMockMethod::_mock_ioctl1(int fd, unsigned long request, struct ifreq *ifr)
{
    return _mMock->mock_ioctl1(fd, request, ifr);
}
int CoreLibUnitTestMockMethod::_mock_write1(int fd, char *buf, size_t count)
{
    return _mMock->mock_write1(fd, buf, count);
}
int CoreLibUnitTestMockMethod::_mock_write2(int fd, ushort *buf, size_t count)
{
    return _mMock->mock_write2(fd, buf, count);
}
int CoreLibUnitTestMockMethod::_mock_write4(int fd, int *buf, size_t count)
{
    return _mMock->mock_write4(fd, buf, count);
}
#if 0
void CoreLibUnitTestMockMethod::_mock_qfile_setfilename(const QString& name)
{
    return _mMock->mock_qfile_setfilename(name);
}
bool CoreLibUnitTestMockMethod::_mock_qfile_exists(void)
{
    return _mMock->mock_qfile_exists();
}
bool CoreLibUnitTestMockMethod::_mock_qfile_open(QIODevice::OpenMode mode)
{
    return _mMock->mock_qfile_open(mode);
}
void CoreLibUnitTestMockMethod::_mock_qfile_close(void)
{
    return _mMock->mock_qfile_close();
}
#endif
QString CoreLibUnitTestMockMethod::_mock_get_dev_file(uchar drvType)
{
    return _mMock->mock_get_dev_file(drvType);
}
#endif // #ifdef CORELIB_UNIT_TEST
