#include "programmenu4_3_1.h"
#include "../../teachingprogrampagemodel.h"
#include "../command/ncpcommandcinc.h"

//------------------------------------------------------
// ProgramMenu4_3_1.
//------------------------------------------------------
ProgramMenu4_3_1::ProgramMenu4_3_1(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
    , m_selectedCountType1Addr(-1)
{
    readPenSetData();
}
//------------------------------------------------------
ProgramMenu4_3_1::~ProgramMenu4_3_1()
{
    m_countType1Addr.clear();
}
//------------------------------------------------------
void ProgramMenu4_3_1::show(NCPCommand* command)
{
    auto cIncCommand = dynamic_cast<NCPCommandCInc*>(command);
    auto ioAddress = (cIncCommand) ? m_resource->indexAt(cIncCommand->symbolStr()) : -1;
    setSelectedCountType1Addr(ioAddress);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_3_1::createNcpCommand()
{
    if (m_selectedCountType1Addr == -1) { return nullptr; }
    auto cIncCommand = new NCPCommandCInc(m_resource);
    if (!cIncCommand) { return nullptr; }
    auto symbolStr = m_resource->symbolAt(m_selectedCountType1Addr);
    cIncCommand->setSymbolStr(symbolStr);
    return cIncCommand;
}
//------------------------------------------------------
void ProgramMenu4_3_1::onActivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_3_1::onDeactivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_3_1::readPenSetData()
{
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 41;
    const int endRow = startRow + 8;
    m_countType1Addr.clear();
    for (int column = 0; column < 4; column++) {
        for (int row = startRow; row < endRow; row++) {
            auto ioAddress = m_resource->getPenSetIntValue(group, row, column);
            m_countType1Addr.append(ioAddress);
        }
    }
}
//------------------------------------------------------
void ProgramMenu4_3_1::setSelectedCountType1Addr(int selectedCountType1Addr)
{
    if (m_selectedCountType1Addr == selectedCountType1Addr) { return; }
    m_selectedCountType1Addr = selectedCountType1Addr;
    emit selectedCountType1AddrChanged(m_selectedCountType1Addr);
}
