#ifndef IODATA_H
#define IODATA_H

#include <QObject>

class IoData
{
public:
    typedef enum {
        Bit,
        Byte
    } Type;
    IoData(Type type, int address, int value);
    IoData();
    void setType(Type type);
    void setIoAddress(int address);
    void setValue(int value);
    Type type() const { return m_type; }
    int ioAddress() const { return m_ioAddress; }
    int value() const { return m_value; }
private:
    Type    m_type = Type::Bit;
    int     m_ioAddress = 0;
    int     m_value = 0;
};

#endif // IODATA_H
