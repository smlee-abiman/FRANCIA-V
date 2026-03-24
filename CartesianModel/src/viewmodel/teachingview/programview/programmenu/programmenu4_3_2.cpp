#include "programmenu4_3_2.h"
#include "../../teachingprogrampagemodel.h"
#include "../command/ncpcommandcclr.h"

//------------------------------------------------------
// ProgramMenu4_3_2.
//------------------------------------------------------
ProgramMenu4_3_2::ProgramMenu4_3_2(ResourceManager *resource,CommunicationThread *comm)
    : ProgramMenu(resource,comm)
    , m_selectedCountType2Addr(-1)
{
    readPenSetData();
}
//------------------------------------------------------
ProgramMenu4_3_2::~ProgramMenu4_3_2()
{
    m_countType2Addr.clear();
}
//------------------------------------------------------
void ProgramMenu4_3_2::show(NCPCommand* command)
{
    auto cClrCommand = dynamic_cast<NCPCommandCClr*>(command);
    auto ioAddress = (cClrCommand) ? m_resource->indexAt(cClrCommand->symbolStr()) : -1;
    setSelectedCountType2Addr(ioAddress);
}
//------------------------------------------------------
NCPCommand *ProgramMenu4_3_2::createNcpCommand()
{
    if (m_selectedCountType2Addr == -1) { return nullptr; }
    auto cClrCommand = new NCPCommandCClr(m_resource);
    if (!cClrCommand) { return nullptr; }
    auto symbolStr = m_resource->symbolAt(m_selectedCountType2Addr);
    cClrCommand->setSymbolStr(symbolStr);
    return cClrCommand;
}
//------------------------------------------------------
void ProgramMenu4_3_2::onActivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_3_2::onDeactivate()
{
    // nop.
}
//------------------------------------------------------
void ProgramMenu4_3_2::readPenSetData()
{
    const QString group = TeachingProgramPageModel::pensetGroup();
    const int startRow = 49;
    const int endRow = startRow + 8;
    m_countType2Addr.clear();
    for (int column = 0; column < 4; column++) {
        for (int row = startRow; row < endRow; row++) {
            auto ioAddress = m_resource->getPenSetIntValue(group, row, column);
            m_countType2Addr.append(ioAddress);
        }
    }
}
//------------------------------------------------------
void ProgramMenu4_3_2::setSelectedCountType2Addr(int selectedCountType2Addr)
{
    if (m_selectedCountType2Addr == selectedCountType2Addr) { return; }
    m_selectedCountType2Addr = selectedCountType2Addr;
    emit selectedCountType2AddrChanged(m_selectedCountType2Addr);
}
