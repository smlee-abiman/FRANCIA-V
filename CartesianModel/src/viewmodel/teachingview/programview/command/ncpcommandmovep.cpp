#include "ncpcommandmovep.h"

//------------------------------------------------------
// NCPCommandMovep.
//------------------------------------------------------
NCPCommandMovep::NCPCommandMovep(QObject *parent)
    : NCPCommandMove(parent)
{
}
//------------------------------------------------------
NCPCommandMovep::NCPCommandMovep(ResourceManager *resource, QObject *parent)
    : NCPCommandMove(resource, parent)
{
}

NCPCommand* NCPCommandMovep::clone() const
 {
     auto selfClone = new NCPCommandMovep(m_resource);
     if (selfClone != nullptr) {
         for (auto axis : m_axisList)
             selfClone->m_axisList.append(axis);
         selfClone->m_relative = m_relative;
         selfClone->m_pointNo = m_pointNo;
     }
     return selfClone;
 }
