#ifndef CORELIBUNITTESTMOCKMETHOD_H
#define CORELIBUNITTESTMOCKMETHOD_H

#ifdef CORELIB_UNIT_TEST
#include <gmock/gmock.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/types.h>
#include <QDebug>
#include <QModbusTcpClient>
#include <QFile>
#include <QString>
#include <sys/socket.h>

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::AtLeast;
using ::testing::DefaultValue;
using ::testing::SetArrayArgument;

class CoreLibUnitTestMockMethod
{
public:
    QString dummyStr = "";
    void setMock(void* mockIns);
    CoreLibUnitTestMockMethod()
    {

        setMock(this);

        ON_CALL(*this, mock_open(_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_close(_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_read1(_,_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_read2(_,_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_read4(_,_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_read_event(_,_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_lseek(_,_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_modbus_connect()).WillByDefault(Return(true));
        ON_CALL(*this, mock_modbus_send_read_request(_,_)).WillByDefault(Return(nullptr));
        ON_CALL(*this, mock_modbus_send_write_request(_,_)).WillByDefault(Return(nullptr));
        ON_CALL(*this, mock_socket(_,_,_)).WillByDefault(Return(1));
        ON_CALL(*this, mock_ioctl1(_,_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_write1(_,_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_write2(_,_,_)).WillByDefault(Return(0));
        ON_CALL(*this, mock_write4(_,_,_)).WillByDefault(Return(0));
        //ON_CALL(*this, mock_qfile_exists()).WillByDefault(Return(true));
        //ON_CALL(*this, mock_qfile_open(_)).WillByDefault(Return(true));
        ON_CALL(*this, mock_get_dev_file(_)).WillByDefault(Return(dummyStr));
        setMock(this);
    }
    MOCK_METHOD2(mock_open, int (const char *, int));
    MOCK_METHOD1(mock_close, int (int));
    MOCK_METHOD3(mock_read1, ssize_t (int, char*, size_t));
    MOCK_METHOD3(mock_read2, ssize_t (int, ushort*, size_t));
    MOCK_METHOD3(mock_read4, ssize_t (int, int*, size_t));
    MOCK_METHOD3(mock_read_event, ssize_t (int, struct input_event*, size_t));
    MOCK_METHOD3(mock_lseek, off_t  (int, off_t, int));
    MOCK_METHOD0(mock_modbus_connect, bool (void));
    MOCK_METHOD2(mock_modbus_send_read_request, QModbusReply* (QModbusDataUnit&, int));
    MOCK_METHOD2(mock_modbus_send_write_request, QModbusReply* (QModbusDataUnit&, int));
    MOCK_METHOD3(mock_socket, int (int, int, int));
    MOCK_METHOD3(mock_ioctl1, int (int, unsigned long, struct ifreq *));
    MOCK_METHOD3(mock_write1, ssize_t (int, char*, size_t));
    MOCK_METHOD3(mock_write2, ssize_t (int, ushort*, size_t));
    MOCK_METHOD3(mock_write4, ssize_t (int, int*, size_t));
    //MOCK_METHOD1(mock_qfile_setfilename, void (const QString&));
    //MOCK_METHOD0(mock_qfile_exists, bool (void));
    //MOCK_METHOD1(mock_qfile_open, bool (QIODevice::OpenMode));
    //MOCK_METHOD0(mock_qfile_close, void (void));
    MOCK_METHOD1(mock_get_dev_file, QString (uchar));

    int _mock_open(const char *, int);
    int _mock_close(int);
    int _mock_read1(int, char*, size_t);
    int _mock_read2(int, ushort*, size_t);
    int _mock_read4(int, int*, size_t);
    int _mock_read_event(int, struct input_event*, size_t);
    off_t _mock_lseek(int, off_t, int);
    bool _mock_modbus_connect(void);
    QModbusReply* _mock_modbus_send_read_request(QModbusDataUnit&, int);
    QModbusReply* _mock_modbus_send_write_request(QModbusDataUnit&, int);
    int _mock_socket(int, int, int);
    int _mock_ioctl1(int, unsigned long, struct ifreq *);
    int _mock_write1(int, char*, size_t);
    int _mock_write2(int, ushort*, size_t);
    int _mock_write4(int, int*, size_t);
    //void _mock_qfile_setfilename(const QString&);
    //bool _mock_qfile_exists(void);
    //bool _mock_qfile_open(QIODevice::OpenMode);
    //void _mock_qfile_close(void);
    QString _mock_get_dev_file(uchar);

private:
    CoreLibUnitTestMockMethod *_mMock;

};
#endif // CORELIB_UNIT_TEST
#endif // CORELIBUNITTESTMOCKMETHOD_H
