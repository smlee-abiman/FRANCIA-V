#include "iostatus.h"

IoStatus::IoStatus(int bankNo, bool isIoBank, QObject *parent)
 : QObject(parent),
   m_bankNo(bankNo),
   m_isIoBank(isIoBank)
{

}

IoStatus::IoStatus(QObject *parent) : QObject(parent)
{

}

void IoStatus::setStatus(const QList<int> &statuslist)
{
    if (m_status != statuslist) {
        m_status = statuslist;
        emit statusChanged();
    }
}
