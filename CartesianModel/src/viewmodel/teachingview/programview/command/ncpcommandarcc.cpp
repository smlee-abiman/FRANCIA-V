#include "ncpcommandarcc.h"

//------------------------------------------------------
// NCPCommandArcc.
//------------------------------------------------------
NCPCommandArcc::NCPCommandArcc(QObject *parent)
    : NCPCommandArc(parent)
{
}
//------------------------------------------------------
NCPCommandArcc::NCPCommandArcc(ResourceManager *resource, QObject *parent)
    : NCPCommandArc(resource, parent)
{
}
//------------------------------------------------------
NCPCommand* NCPCommandArcc::clone() const
{
    auto selfClone = new NCPCommandArcc(m_resource);
    if (selfClone != nullptr) {
        for (auto axis : m_axisList)
            selfClone->m_axisList.append(axis);
        selfClone->m_pointNo = m_pointNo;
        selfClone->m_endPointNo = m_endPointNo;
    }
    return selfClone;
}
