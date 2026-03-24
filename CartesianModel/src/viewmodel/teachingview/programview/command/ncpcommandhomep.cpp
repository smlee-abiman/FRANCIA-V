#include "ncpcommandhomep.h"

//------------------------------------------------------
// NCPCommandHomep.
//------------------------------------------------------
NCPCommandHomep::NCPCommandHomep(QObject *parent)
    : NCPCommandHome(parent)
{
}
//------------------------------------------------------
NCPCommandHomep::NCPCommandHomep(ResourceManager *resource, QObject *parent)
    : NCPCommandHome(resource, parent)
{
}

NCPCommand* NCPCommandHomep::clone() const
{
    auto selfClone = new NCPCommandHomep(m_resource);
    if (selfClone != nullptr) {
        selfClone->m_option = m_option;
        for (auto axis : m_axisList)
            selfClone->m_axisList.append(axis);
    }
    return selfClone;
}
