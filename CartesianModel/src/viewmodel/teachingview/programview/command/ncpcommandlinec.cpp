#include "ncpcommandlinec.h"

//------------------------------------------------------
// NCPCommandLinec.
//------------------------------------------------------
NCPCommandLinec::NCPCommandLinec(QObject *parent)
    : NCPCommandLine(parent)
{
}
//------------------------------------------------------
NCPCommandLinec::NCPCommandLinec(ResourceManager *resource, QObject *parent)
    : NCPCommandLine(resource, parent)
{
}
//------------------------------------------------------

NCPCommand* NCPCommandLinec::clone() const
{
    auto selfClone = new NCPCommandLinec(m_resource);
    if (selfClone != nullptr) {
        for (auto axis : m_axisList)
            selfClone->m_axisList.append(axis);
        selfClone->m_pointNo = m_pointNo;
    }
    return selfClone;
}
