#include "programmenu4_3_4.h"
#include "../../teachingprogrampagemodel.h"
#include "../command/ncpcommanddec.h"

//------------------------------------------------------
// ProgramMenu4_3_4.
//------------------------------------------------------
ProgramMenu4_3_4::ProgramMenu4_3_4(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
    , m_selectedCountType4Addr(-1)
{
    readPenSetData();
}
//------------------------------------------------------
ProgramMenu4_3_4::~ProgramMenu4_3_4()
{
    m_countType4Addr.clear();
}
//------------------------------------------------------
void ProgramMenu4_3_4::show(NCPCommand* command)
{
    auto decCommand = dynamic_cast<NCPCommandDec*>(command);
    auto ioAddress = (decCommand) ? m_resource->indexAt(decCommand->symbolStr()) : -1;
    setSelectedCountType4Addr(ioAddress);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_3_4::createNcpCommand()
{
    if (m_selectedCountType4Addr == -1) { return nullptr; }
    auto decCommand = new NCPCommandDec(m_resource);
    if (!decCommand) { return nullptr; }
    auto symbolStr = m_resource->symbolAt(m_selectedCountType4Addr);
    decCommand->setSymbolStr(symbolStr);
    return decCommand;
}
//------------------------------------------------------
void ProgramMenu4_3_4::onActivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_3_4::onDeactivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_3_4::readPenSetData()
{
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 65;
    const int endRow = startRow + 8;
    m_countType4Addr.clear();
    for (int row = startRow; row < endRow; row++) {
        auto ioAddress = m_resource->getPenSetIntValue(group, row, 0);
        m_countType4Addr.append(ioAddress);
    }
}
//------------------------------------------------------
void ProgramMenu4_3_4::setSelectedCountType4Addr(int selectedCountType4Addr)
{
    if (m_selectedCountType4Addr == selectedCountType4Addr) { return; }
    m_selectedCountType4Addr = selectedCountType4Addr;
    emit selectedCountType4AddrChanged(m_selectedCountType4Addr);
}
