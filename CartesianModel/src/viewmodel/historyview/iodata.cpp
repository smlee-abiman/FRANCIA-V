#include "iodata.h"

IoData::IoData()
{

}

IoData::IoData(Type type, int address, int value)
{
    setType(type);
    setIoAddress(address);
    setValue(value);
}

void IoData::setType(Type type)
{
    m_type = type;
}

void IoData::setIoAddress(int address)
{
    m_ioAddress = address;
}

void IoData::setValue(int value)
{
    m_value = value;
}

