#include "programmenu4_3_3.h"
#include "../../teachingprogrampagemodel.h"
#include "../command/ncpcommandinc.h"

//------------------------------------------------------
// ProgramMenu4_3_3.
//------------------------------------------------------
ProgramMenu4_3_3::ProgramMenu4_3_3(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
    , m_selectedCountType3Addr(-1)
{
    readPenSetData();
}
//------------------------------------------------------
ProgramMenu4_3_3::~ProgramMenu4_3_3()
{
    m_countType3Addr.clear();
}
//------------------------------------------------------
void ProgramMenu4_3_3::show(NCPCommand* command)
{
    auto incCommand = dynamic_cast<NCPCommandInc*>(command);
    auto ioAddress = (incCommand) ? m_resource->indexAt(incCommand->symbolStr()) : -1;
    setSelectedCountType3Addr(ioAddress);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_3_3::createNcpCommand()
{
    if (m_selectedCountType3Addr == -1) { return nullptr; }
    auto incCommand = new NCPCommandInc(m_resource);
    if (!incCommand) { return nullptr; }
    auto symbolStr = m_resource->symbolAt(m_selectedCountType3Addr);
    incCommand->setSymbolStr(symbolStr);
    return incCommand;
}
//------------------------------------------------------
void ProgramMenu4_3_3::onActivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_3_3::onDeactivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_3_3::readPenSetData()
{
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 57;
    const int endRow = startRow + 8;
    m_countType3Addr.clear();
    for (int row = startRow; row < endRow; row++) {
        auto ioAddress = m_resource->getPenSetIntValue(group, row, 0);
        m_countType3Addr.append(ioAddress);
    }
}
//------------------------------------------------------
void ProgramMenu4_3_3::setSelectedCountType3Addr(int selectedCountType3Addr)
{
    if (m_selectedCountType3Addr == selectedCountType3Addr) { return; }
    m_selectedCountType3Addr = selectedCountType3Addr;
    emit selectedCountType3AddrChanged(m_selectedCountType3Addr);
}
