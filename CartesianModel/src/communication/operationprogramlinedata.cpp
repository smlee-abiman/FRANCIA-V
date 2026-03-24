#include "operationprogramlinedata.h"

int OperationProgramLineData::line()
{
    return m_line;
}

void OperationProgramLineData::setLine(int line)
{
    if(m_line == line) return;

    m_line = line;
    emit lineChanged(m_line);
}

QString OperationProgramLineData::data()
{
    return m_data;
}

void OperationProgramLineData::setData(QString data)
{
    if(m_data == data) return;

    m_data = data;
    emit dataChanged(m_data);
}
