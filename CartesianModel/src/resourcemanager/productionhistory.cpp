#include "productionhistory.h"

ProductionHistory::ProductionHistory(QObject *parent) : QObject(parent) {}

ProductionHistory::ProductionHistory(int production, int open, int eject, QObject *parent)
    : QObject(parent)
{
    m_prodution = production;
    m_open = open;
    m_eject = eject;
}
